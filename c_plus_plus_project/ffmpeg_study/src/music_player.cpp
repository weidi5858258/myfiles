//
// Created by root on 18-7-31.
//

#ifndef MYSTUDY_FFMPEG_MUSIC_PLAYER
#define MYSTUDY_FFMPEG_MUSIC_PLAYER

#include "../include/MyHead.h"

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIOQ_SIZE (1 * 1024 * 1024)
#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT (SDL_USEREVENT + 2)

//该字段存在于旧版本的ffmpeg中，此处粘贴过来使用，勿怪！
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    //有多少个AVPacketList
    int all_pkts;
    //所有AVPacket占用的空间大小
    int all_pkts_size;
    //互斥量
    SDL_mutex *sdl_mutex;
    //条件变量
    SDL_cond *sdl_cond;
} PacketQueue;

typedef struct AudioState {
    char in_file_path[1024];
    PacketQueue packet_queue;
    int audio_stream_index = -1;

    AVFormatContext *avformat_context = NULL;
    AVStream *audio_avstream = NULL;
    AVFrame *audio_avframe = NULL;
    struct SwrContext *audio_swr_context = NULL;

    AVPacket audio_avpacket;
    //下面两个变量的数据就是audio_avpacket中的data和size数据
    uint8_t *audio_pkt_data = NULL;
    int audio_pkt_size;

    //解码一次得到的数据量
    unsigned int audio_decoded_data_size;
    //用于标记已处理过的数据位置(针对audio_decoded_data_size的位置)
    unsigned int audio_decoded_data_size_index;

    uint8_t *audio_buf = NULL;
    DECLARE_ALIGNED(16, uint8_t, audio_buf2)[AVCODEC_MAX_AUDIO_FRAME_SIZE * 4];

    //采样格式(每个采样点所占的字节数)
    enum AVSampleFormat src_avsample_format;
    enum AVSampleFormat dst_avsample_format;
    //声音布局
    int64_t src_ch_layout;
    int64_t dst_ch_layout;
    //声道数量
    int src_nb_channels;
    int dst_nb_channels;
    //采样率(每秒采集多少个样点)
    int src_sample_rate;
    int dst_sample_rate;

    //退出标志
    int quit;

    SDL_Thread *sdl_thread = NULL;
} AudioState;

AudioState *global_audio_state;

void packet_queue_init(PacketQueue *packet_queue) {
    memset(packet_queue, 0, sizeof(PacketQueue));
    //创建的互斥量默认是未上锁的
    packet_queue->sdl_mutex = SDL_CreateMutex();
    //创建条件变量
    packet_queue->sdl_cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue *packet_queue, AVPacket *avpacket) {
    if (SDL_LockMutex(packet_queue->sdl_mutex) == 0) {
        //需要把AVPacket类型构造成AVPacketList类型,因此先要构造一个AVPacketList指针
        AVPacketList *avpacket_list = NULL;
        avpacket_list = (AVPacketList *) av_malloc(sizeof(AVPacketList));
        if (!avpacket_list) {
            return -1;
        }
        avpacket_list->pkt = *avpacket;
        avpacket_list->next = NULL;

        //SDL_LockMutex(packet_queue->sdl_mutex);

        //第一次为NULL
        if (!packet_queue->last_pkt) {
            packet_queue->first_pkt = avpacket_list;
            fprintf(stdout,
                    "packet_queue->first_pkt->pkt.pos = %ld\n",
                    packet_queue->first_pkt->pkt.pos);
        } else {
            packet_queue->last_pkt->next = avpacket_list;
        }
        packet_queue->last_pkt = avpacket_list;
        packet_queue->all_pkts++;
        packet_queue->all_pkts_size += avpacket_list->pkt.size;
        //?
        SDL_CondSignal(packet_queue->sdl_cond);

        SDL_UnlockMutex(packet_queue->sdl_mutex);
    }
    return 0;
}

static int packet_queue_get(PacketQueue *packet_queue, AVPacket *avpacket, int block) {
    SDL_LockMutex(packet_queue->sdl_mutex);

    AVPacketList *avpacket_list = NULL;
    int ret;

    //SDL_LockMutex(packet_queue->sdl_mutex);

    for (;;) {
        if (global_audio_state->quit) {
            ret = -1;
            break;
        }

        avpacket_list = packet_queue->first_pkt;
        if (avpacket_list) {
            packet_queue->first_pkt = avpacket_list->next;
            if (!packet_queue->first_pkt) {
                packet_queue->last_pkt = NULL;
                fprintf(stdout, "%s\n", "已经没有数据了");
            }
            packet_queue->all_pkts--;
            packet_queue->all_pkts_size -= avpacket_list->pkt.size;
            *avpacket = avpacket_list->pkt;

            av_free(avpacket_list);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            fprintf(stdout, "%s\n", "SDL_CondWait start.");
            SDL_Event event;
            event.type = FF_QUIT_EVENT;
            SDL_PushEvent(&event);
            //信号激活后，返回0,否则返回错误代码
            //SDL_ConWait必须在互斥量锁住之后才能调用。该函数会解锁锁住的互斥量，并等待拥有该锁的线程激活信号。激活后，会重新上锁。
            SDL_CondWait(packet_queue->sdl_cond, packet_queue->sdl_mutex);
            fprintf(stdout, "%s\n", "SDL_CondWait end.");
        }
    }

    SDL_UnlockMutex(packet_queue->sdl_mutex);
    return ret;
}

int audio_decode_frame(AudioState *audio_state) {
    int ret, get_nb_samples_per_channel, decoded_data_size;
    AVPacket *avpacket = &audio_state->audio_avpacket;
    int got_frame_ptr = 0;
    int64_t get_ch_layout_from_decoded_avframe;
    int get_nb_samples_from_decoded_avframe, resampled_data_size;

    //fprintf(stdout, "%s\n", "audio_decode_frame");
    for (;;) {
        //fprintf(stdout, "audio_state->audio_pkt_size = %d\n", audio_state->audio_pkt_size);
        while (audio_state->audio_pkt_size > 0) {
            if (!audio_state->audio_avframe) {
                if (!(audio_state->audio_avframe = av_frame_alloc())) {
                    return AVERROR(ENOMEM);
                }
            } else {
                av_frame_unref(audio_state->audio_avframe);
            }
            /***
             * 当AVPacket中装得是音频时，有可能一个AVPacket中有多个AVFrame，
             * 而某些解码器只会解出第一个AVFrame，这种情况我们必须循环解码出后续AVFrame
             */
            ret = avcodec_decode_audio4(audio_state->audio_avstream->codec,
                                        audio_state->audio_avframe,
                                        &got_frame_ptr,
                                        avpacket);
            if (ret < 0) {
                fprintf(stderr, "ret = %d\n", ret);
                // error, skip the frame
                audio_state->audio_pkt_size = 0;
                break;
            }

            audio_state->audio_pkt_data += ret;
            audio_state->audio_pkt_size -= ret;

            if (!got_frame_ptr) {
                fprintf(stderr, "got_frame_ptr = %d\n", got_frame_ptr);
                continue;
            }

            //执行到这里我们得到了一个AVFrame(audio_state->audio_avframe为解码后的AVFrame)
            decoded_data_size = av_samples_get_buffer_size(NULL,
                                                           audio_state->audio_avframe->channels,
                                                           audio_state->audio_avframe->nb_samples,
                                                           (enum AVSampleFormat) audio_state->audio_avframe->format,
                                                           1);

            //得到这个AVFrame的声音布局，比如立体声
            get_ch_layout_from_decoded_avframe =
                    (audio_state->audio_avframe->channel_layout
                     && audio_state->audio_avframe->channels
                        == av_get_channel_layout_nb_channels(audio_state->audio_avframe->channel_layout))
                    ?
                    audio_state->audio_avframe->channel_layout
                    :
                    av_get_default_channel_layout(audio_state->audio_avframe->channels);

            //这个AVFrame每个声道的采样数
            get_nb_samples_from_decoded_avframe = audio_state->audio_avframe->nb_samples;

            /***
             * 接下来判断我们之前设置SDL时设置的声音格式(AV_SAMPLE_FMT_S16)，声道布局，
             * 采样频率，每个AVFrame的每个声道采样数与
             * 得到的该AVFrame分别是否相同，如有任意不同，我们就需要swr_convert该AVFrame，
             * 然后才能符合之前设置好的SDL的需要，才能播放
             */
            if (audio_state->audio_avframe->format != audio_state->src_avsample_format
                || get_ch_layout_from_decoded_avframe != audio_state->src_ch_layout
                || audio_state->audio_avframe->sample_rate != audio_state->src_sample_rate
                || (get_nb_samples_from_decoded_avframe != audio_state->audio_avframe->nb_samples
                    && !audio_state->audio_swr_context)) {
                if (audio_state->audio_swr_context) {
                    swr_free(&audio_state->audio_swr_context);
                }
                fprintf(stdout, "audio_state->audioSwrContext swr_alloc_set_opts.\n");
                audio_state->audio_swr_context = swr_alloc_set_opts(NULL,
                                                                    audio_state->dst_ch_layout,
                                                                    audio_state->dst_avsample_format,
                                                                    audio_state->dst_sample_rate,
                                                                    get_ch_layout_from_decoded_avframe,
                                                                    (enum AVSampleFormat) audio_state->audio_avframe->format,
                                                                    audio_state->audio_avframe->sample_rate,
                                                                    0, NULL);
                if (!audio_state->audio_swr_context || swr_init(audio_state->audio_swr_context) < 0) {
                    fprintf(stderr, "swr_init() failed\n");
                    break;
                } else {
                    fprintf(stdout, "audio_state->audioSwrContext is created.\n");
                }
                audio_state->src_ch_layout = get_ch_layout_from_decoded_avframe;
                audio_state->src_nb_channels = audio_state->audio_avstream->codec->channels;
                audio_state->src_sample_rate = audio_state->audio_avstream->codec->sample_rate;
                audio_state->src_avsample_format = audio_state->audio_avstream->codec->sample_fmt;
            }

            if (audio_state->audio_swr_context) {
                //fprintf(stdout, "audio_state->audioSwrContext is not NULL.\n");

                //if条件永不满足
                if (get_nb_samples_from_decoded_avframe != audio_state->audio_avframe->nb_samples) {
                    fprintf(stdout, "swr_set_compensation\n");
                    if (swr_set_compensation(audio_state->audio_swr_context,
                                             (get_nb_samples_from_decoded_avframe -
                                              audio_state->audio_avframe->nb_samples)
                                             * audio_state->dst_sample_rate
                                             / audio_state->audio_avframe->sample_rate,
                                             get_nb_samples_from_decoded_avframe * audio_state->dst_sample_rate
                                             / audio_state->audio_avframe->sample_rate) < 0) {
                        fprintf(stderr, "swr_set_compensation() failed\n");
                        break;
                    }
                }

                /***
                 * 转换该AVFrame到设置好的SDL需要的样子，有些旧的代码示例最主要就是少了这一部分，
                 * 往往一些音频能播，一些不能播，这就是原因，比如有些源文件音频恰巧是AV_SAMPLE_FMT_S16的。
                 * swr_convert 返回的是转换后每个声道(channel)的采样数
                 */
                // const uint8_t *in[] = { audio_state->audio_avframe->data[0] };
                uint8_t *out[] = {audio_state->audio_buf2};
                int out_count = sizeof(audio_state->audio_buf2)
                                / audio_state->dst_nb_channels
                                / av_get_bytes_per_sample(audio_state->dst_avsample_format);
                const uint8_t **in = (const uint8_t **) audio_state->audio_avframe->extended_data;
                int in_count = audio_state->audio_avframe->nb_samples;
                get_nb_samples_per_channel = swr_convert(audio_state->audio_swr_context,
                                                         out, out_count,
                                                         in, in_count);
                if (get_nb_samples_per_channel < 0) {
                    fprintf(stderr, "swr_convert() failed\n");
                    break;
                }
                if (get_nb_samples_per_channel
                    == sizeof(audio_state->audio_buf2) / audio_state->dst_nb_channels
                       / av_get_bytes_per_sample(audio_state->dst_avsample_format)) {
                    fprintf(stderr,
                            "warning: audio buffer audio_state probably too small\n");
                    swr_init(audio_state->audio_swr_context);
                }
                audio_state->audio_buf = audio_state->audio_buf2;

                //声道数 x 每声道采样数 x 每个采样字节数
                resampled_data_size = audio_state->dst_nb_channels
                                      * get_nb_samples_per_channel
                                      * av_get_bytes_per_sample(audio_state->dst_avsample_format);
            } else {
                resampled_data_size = decoded_data_size;
                audio_state->audio_buf = audio_state->audio_avframe->data[0];
                fprintf(stdout, "resampled_data_size = %d\n", resampled_data_size);
            }
            // We have data, return it and come back for more later
            return resampled_data_size;
        }//while end

        if (avpacket->data) {
            av_free_packet(avpacket);
        }
        memset(avpacket, 0, sizeof(*avpacket));
        if (audio_state->quit) {
            return -1;
        }
        if (packet_queue_get(&audio_state->packet_queue, avpacket, 1) < 0) {
            return -1;
        }

        audio_state->audio_pkt_data = avpacket->data;
        audio_state->audio_pkt_size = avpacket->size;
    }//for end
}

/***
 *
 * @param userdata
 * @param sdl_need_stream_data SDL播放声音时需要的声音数据
 * @param sdl_max_handle_data_size SDL回调一次最多只能处理的字节数,大于这个数时多余的数据就浪费了,因此不能超过这个数
 max_handle_data_size这个值不能大于sdl_max_handle_data_size的值,不然多余数据就被丢弃了,播放就会出问题
 audio_decoded_data_size为一次解码得到的数据量,这个数据量是明显大于len值的.
 这样做的好处就是解码一次可以为SDL提供多次的数据,这样就可以减少解码次数;
 当然这样做的前提是要设计一个比较大的空间来存放解码好的数据.
 */
void audio_callback(void *userdata, uint8_t *sdl_need_stream_data, int sdl_max_handle_data_size) {
    //fprintf(stdout, "audio_callback: sdl_max_handle_data_size = %d\n", sdl_max_handle_data_size);//2048
    AudioState *audio_state = (AudioState *) userdata;
    int max_handle_data_size, audio_decoded_data_size;

    while (sdl_max_handle_data_size > 0) {
        if (audio_state->audio_decoded_data_size_index >= audio_state->audio_decoded_data_size) {
            audio_decoded_data_size = audio_decode_frame(audio_state);
            //fprintf(stdout, "audio_decoded_data_size = %d\n", audio_decoded_data_size);

            if (audio_decoded_data_size < 0) {
                /* silence */
                audio_state->audio_decoded_data_size = 1024;
                memset(audio_state->audio_buf, 0, audio_state->audio_decoded_data_size);
            } else {
                audio_state->audio_decoded_data_size = audio_decoded_data_size;
            }
            //fprintf(stdout, "audio_callback: audio_decoded_data_size = %d\n", audio_decoded_data_size);
            //fprintf(stdout, "audio_callback: audio_state->audio_decoded_data_size = %d\n", audio_state->audio_decoded_data_size);
            audio_state->audio_decoded_data_size_index = 0;
        }

        max_handle_data_size = audio_state->audio_decoded_data_size - audio_state->audio_decoded_data_size_index;
        if (max_handle_data_size > sdl_max_handle_data_size) {
            max_handle_data_size = sdl_max_handle_data_size;
        }

        memcpy(sdl_need_stream_data,
               (uint8_t *) audio_state->audio_buf + audio_state->audio_decoded_data_size_index,
               max_handle_data_size);
        sdl_max_handle_data_size -= max_handle_data_size;
        sdl_need_stream_data += max_handle_data_size;
        audio_state->audio_decoded_data_size_index += max_handle_data_size;
        //fprintf(stdout, "audio_callback: audio_state->audio_decoded_data_size_index = %d\n", audio_state->audio_decoded_data_size_index);
    }//while end
}

/**
 * 设置SDL播放声音的参数如声音采样格式，声道布局，静音值
 */
int stream_component_open(AudioState *audio_state, int audio_stream_index) {
    AVFormatContext *avformat_context = audio_state->avformat_context;
    AVCodecContext *audio_avcodec_context;
    AVCodec *audio_avcodec_decoder;
    SDL_AudioSpec sdl_audio_spec, spec;
    int64_t wanted_channel_layout = 0;
    int wanted_nb_channels = -1;
    int wanted_sample_rate = -1;
    const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};

    if (audio_stream_index < 0 || audio_stream_index >= avformat_context->nb_streams) {
        return -1;
    }

    audio_avcodec_context = avformat_context->streams[audio_stream_index]->codec;

    wanted_sample_rate = audio_avcodec_context->sample_rate;
    wanted_nb_channels = audio_avcodec_context->channels;
    fprintf(stdout, "wanted_sample_rate = %d\n", wanted_sample_rate);
    fprintf(stdout, "wanted_nb_channels = %d\n", wanted_nb_channels);
    //得到声道数要不要这样子做?
    if (!wanted_channel_layout
        || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
        fprintf(stdout, "wanted_channel_layout = %d\n", wanted_channel_layout);
        //why?
        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
        fprintf(stdout, "wanted_channel_layout = %d\n", wanted_channel_layout);
    }

    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
    fprintf(stdout, "wanted_nb_channels = %d\n", wanted_nb_channels);
    if (wanted_nb_channels <= 0 || wanted_sample_rate <= 0) {
        fprintf(stderr, "Invalid sample rate or channel count!\n");
        return -1;
    }

    //1.需要为SDL_AudioSpec设置下面这些参数
    sdl_audio_spec.freq = wanted_sample_rate;
    sdl_audio_spec.channels = wanted_nb_channels;
    sdl_audio_spec.format = AUDIO_S16SYS;
    sdl_audio_spec.silence = 0;
    sdl_audio_spec.samples = 1024;
    sdl_audio_spec.callback = audio_callback;
    sdl_audio_spec.userdata = audio_state;

    //2.
    while (SDL_OpenAudio(&sdl_audio_spec, &spec) < 0) {
        fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", sdl_audio_spec.channels, SDL_GetError());
        sdl_audio_spec.channels = next_nb_channels[FFMIN(7, sdl_audio_spec.channels)];
        if (!sdl_audio_spec.channels) {
            fprintf(stderr,
                    "No more channel combinations to tyu, audio open failed\n");
            return -1;
        }
        wanted_channel_layout = av_get_default_channel_layout(sdl_audio_spec.channels);
        fprintf(stdout, "wanted_channel_layout = %d\n", wanted_channel_layout);
    }
    if (spec.format != AUDIO_S16SYS) {
        fprintf(stderr, "SDL advised audio format %d is not supported!\n", spec.format);
        return -1;
    }
    if (spec.channels != sdl_audio_spec.channels) {
        wanted_channel_layout = av_get_default_channel_layout(spec.channels);
        if (!wanted_channel_layout) {
            fprintf(stderr, "SDL advised channel count %d is not supported!\n", spec.channels);
            return -1;
        }
    }

    fprintf(stdout, "%d: sdl_audio_spec.format = %d\n", __LINE__, sdl_audio_spec.format);
    fprintf(stdout, "%d: sdl_audio_spec.samples = %d\n", __LINE__, sdl_audio_spec.samples);
    fprintf(stdout, "%d: sdl_audio_spec.channels = %d\n", __LINE__, sdl_audio_spec.channels);
    fprintf(stdout, "%d: sdl_audio_spec.freq = %d\n", __LINE__, sdl_audio_spec.freq);

    fprintf(stdout, "%d: spec.format = %d\n", __LINE__, spec.format);
    fprintf(stdout, "%d: spec.samples = %d\n", __LINE__, spec.samples);
    fprintf(stdout, "%d: spec.channels = %d\n", __LINE__, spec.channels);
    fprintf(stdout, "%d: spec.freq = %d\n", __LINE__, spec.freq);

    audio_state->src_avsample_format = audio_state->dst_avsample_format = AV_SAMPLE_FMT_S16;
    audio_state->src_sample_rate = audio_state->dst_sample_rate = spec.freq;
    audio_state->src_ch_layout = audio_state->dst_ch_layout = wanted_channel_layout;
    audio_state->src_nb_channels = audio_state->dst_nb_channels = spec.channels;

    audio_avcodec_decoder = avcodec_find_decoder(audio_avcodec_context->codec_id);
    if (!audio_avcodec_decoder
        || (avcodec_open2(audio_avcodec_context, audio_avcodec_decoder, NULL) < 0)) {
        fprintf(stderr, "Unsupported audioAVCodecDecoder!\n");
        return -1;
    }

    avformat_context->streams[audio_stream_index]->discard = AVDISCARD_DEFAULT;
    switch (audio_avcodec_context->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            audio_state->audio_stream_index = audio_stream_index;
            audio_state->audio_avstream = avformat_context->streams[audio_stream_index];
            audio_state->audio_decoded_data_size = 0;
            audio_state->audio_decoded_data_size_index = 0;
            memset(&audio_state->audio_avpacket, 0, sizeof(audio_state->audio_avpacket));
            break;
        default:
            break;
    }

    packet_queue_init(&audio_state->packet_queue);
    //3.
    SDL_PauseAudio(0);

    fprintf(stdout, "%s\n", "SDL_PauseAudio(0)");
    fprintf(stdout, "audio_state->audio_decoded_data_size_index = %d\n", audio_state->audio_decoded_data_size_index);
    fprintf(stdout, "audio_state->audio_decoded_data_size = %d\n", audio_state->audio_decoded_data_size);
}

int audio_frame_count2 = 0;

/**
 * demuxing出AVPacket
 */
static int decode_thread(void *arg) {
    AudioState *audio_state = (AudioState *) arg;
    AVFormatContext *avformat_context = NULL;
    AVPacket pkt, *avpacket = &pkt;
    int ret, i, audio_stream_index = -1;

    audio_state->audio_stream_index = -1;
    global_audio_state = audio_state;

    av_register_all();
    if (avformat_open_input(&avformat_context, audio_state->in_file_path, NULL, NULL) != 0) {
        return -1;
    }
    if (avformat_find_stream_info(avformat_context, NULL) < 0) {
        return -1;
    }
    audio_state->avformat_context = avformat_context;
    //av_dump_format(avformat_context, 0, audio_state->inFilePath, 0);
    for (i = 0; i < avformat_context->nb_streams; i++) {
        if (avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO
            && audio_stream_index < 0) {
            audio_stream_index = i;
            break;
        }
    }
    if (audio_stream_index >= 0) {
        stream_component_open(audio_state, audio_stream_index);
    } else if (audio_state->audio_stream_index < 0) {
        fprintf(stderr, "%s: could not open codecs\n", audio_state->in_file_path);
        goto fail;
    }

    fprintf(stdout, "%s\n", "for loop start.");

    // main decode loop
    for (;;) {
        if (audio_state->quit)
            break;
        if (audio_state->packet_queue.all_pkts_size > MAX_AUDIOQ_SIZE) {
            SDL_Delay(10);
            continue;
        }
        ret = av_read_frame(audio_state->avformat_context, avpacket);
        if (ret < 0) {
            //if (ret == AVERROR_EOF || url_feof(is->avformat_context->pb)) {
            if (ret == AVERROR_EOF) {
                break;
            }
            if (audio_state->avformat_context->pb && audio_state->avformat_context->pb->error) {
                break;
            }
            continue;
        }

        /***
         这个应用的设计思想是在这个地方不立即进行解码,
         只是把需要解码的数据(avpacket)先存放到一个队列中,
         等到SDL需要数据播放声音时,才开始从队列中取原始数据解码.
         */
        //audio
        if (avpacket->stream_index == audio_state->audio_stream_index) {
            audio_frame_count2++;
            packet_queue_put(&audio_state->packet_queue, avpacket);
        } else {
            av_free_packet(avpacket);
        }
    }// for(;;) end

    fprintf(stdout, "%s\n", "for loop end.");
    fprintf(stdout, "audio_frame_count = %d\n", audio_frame_count2);

    while (!audio_state->quit) {
        SDL_Delay(100);
        //fprintf(stdout, "%s\n", "SDL_Delay(100).");
    }

    fprintf(stdout, "%s\n", "while loop end.");

    fail:
    {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        event.user.data1 = audio_state;
        SDL_PushEvent(&event);
    }

    return 0;
}

/***
 播放效果还是不错的,代码比较好
 alexander_music_player ---> decode_thread ---> stream_component_open ---> audio_callback ---> audio_decode_frame

 alexander_music_player ---> decode_thread ---> for ---> packet_queue_put
 */
int alexander_music_player(const char *in_file_name) {
    if (in_file_name == NULL) {
        fprintf(stdout, "%s\n", "Please input music name!");
        return -1;
    }

    AudioState *audio_state = NULL;
    audio_state = (AudioState *) av_mallocz(sizeof(AudioState));
    // 复制文件路径
    av_strlcpy(audio_state->in_file_path, in_file_name, sizeof(audio_state->in_file_path));

    if (SDL_Init(SDL_INIT_AUDIO)) {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    // 创建解码线程
    audio_state->sdl_thread = SDL_CreateThread(decode_thread, NULL, audio_state);
    if (!audio_state->sdl_thread) {
        av_free(audio_state);
        return -1;
    }

    /*int status = 0;
    // 如果没有下面的等待函数,那么子线程可能连执行的机会都没有
    // 等待audioRender函数里的代码执行完后才往下走,不然一直阻塞在这里
    SDL_WaitThread(audio_state->sdl_thread, &status);
    printf("alexander_music_player() audio status: %d\n", status);*/

    SDL_Event sdl_event;
    for (;;) {
        fprintf(stdout, "%s\n", "for(): SDL_WaitEvent start.");
        SDL_WaitEvent(&sdl_event);
        fprintf(stdout, "%s\n", "for(): SDL_WaitEvent end.");
        switch (sdl_event.type) {
            case FF_QUIT_EVENT:
            case SDL_QUIT:
                //激活信号(成功返回 0，否则返回错误代码)
                //SDL_ConSignal会激活等待的一个线程(根据优先级)，而不是所有等待的线程。
                SDL_CondSignal(audio_state->packet_queue.sdl_cond);
                audio_state->quit = 1;
                if (audio_state->sdl_thread != NULL) {
                    SDL_DetachThread(audio_state->sdl_thread);
                    audio_state->sdl_thread = NULL;
                }
                //SDL_Quit()调用之后才可以调用其后的函数
                SDL_Quit();
                if (audio_state->packet_queue.sdl_mutex != NULL) {
                    SDL_DestroyMutex(audio_state->packet_queue.sdl_mutex);
                }
                if (audio_state->packet_queue.sdl_cond != NULL) {
                    SDL_DestroyCond(audio_state->packet_queue.sdl_cond);
                }
                fprintf(stdout, "%s\n", "exit(0).");
                exit(0);
            default:
                break;
        }
    }// for(;;) end

    return 0;
}

// 视频的话前三个有值(可能是data[0], data[1], data[2]分别存了YUV的值吧)
/*fprintf(stdout, "handleData() dstAVFrame->linesize[0]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[0]);
fprintf(stdout, "handleData() dstAVFrame->linesize[1]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[1]);
fprintf(stdout, "handleData() dstAVFrame->linesize[2]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[2]);
fprintf(stdout, "handleData() dstAVFrame->linesize[3]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[3]);
fprintf(stdout, "handleData() dstAVFrame->linesize[4]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[4]);
fprintf(stdout, "handleData() dstAVFrame->linesize[5]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[5]);
fprintf(stdout, "handleData() dstAVFrame->linesize[6]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[6]);
fprintf(stdout, "handleVideoData() dstAVFrame->linesize[7]: %d\n",
        videoWrapper->father->dstAVFrame->linesize[7]);*/


#endif //MYSTUDY_FFMPEG_MUSIC_PLAYER