//
// Created by root on 18-7-12.
//
#ifndef MYSTUDY_STUDY_FFMPEG
#define MYSTUDY_STUDY_FFMPEG

#include "../include/MyHead.h"

/***
    视频文件格式（容器格式）
    视频编解码器（视频编码格式）
    音频编解码器（音频编码格式）
    音频编码的作用:
        将音频采样数据(PCM等)压缩成为音频码流,从而降低音频的数据量.
    aac比mp3的所占用的体积相对来说要少.
    aac压缩方法:比较复杂.可以将音频数据压缩10倍以上.
 */

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

void alexander_fill_audio(void *udata, Uint8 *stream, int len);

void print_file_audio_info(AVFormatContext *avformat_context, AVCodecContext *audio_avcodec_context,
                           int in_sample_fmt, int audio_stream_index) {
    ///////////////////////////////输出音频信息///////////////////////////////

    //输出音频信息
    printf("\n");
    printf("Complete name：         \t%s\n", avformat_context->filename);
    printf("Format long name：     \t%s\n", avformat_context->iformat->long_name);
    printf("Codec long name：     \t%s\n", audio_avcodec_context->codec->long_name);
    printf("Format：             \t%s\n", avformat_context->iformat->name);
    int duration = (avformat_context->duration) / 1000000L;
    int hour = duration / 3600;//小时
    int minute = duration % 3600 / 60;//分钟
    int second = duration % 60;//秒
    printf("Duration：             \t%02d:%02d:%02d\n", hour, minute, second);
    printf("Channel(s)：         \t%d\n", audio_avcodec_context->channels);
    printf("Bit rate：             \t%.1f(Kbps)\n", (audio_avcodec_context->bit_rate) / 1000.0f);
    printf("Sample rate：         \t%.1f(KHz)\n", (audio_avcodec_context->sample_rate) / 1000.0f);
    char *sample_fmt = NULL;
    switch (in_sample_fmt) {
        case AV_SAMPLE_FMT_NONE:
            sample_fmt = "AV_SAMPLE_FMT_NONE";
            break;
        case AV_SAMPLE_FMT_U8:
            sample_fmt = "AV_SAMPLE_FMT_U8";
            break;
        case AV_SAMPLE_FMT_S16:
            sample_fmt = "AV_SAMPLE_FMT_S16";
            break;
        case AV_SAMPLE_FMT_S32:
            sample_fmt = "AV_SAMPLE_FMT_S32";
            break;
        case AV_SAMPLE_FMT_FLT:
            sample_fmt = "AV_SAMPLE_FMT_FLT";
            break;
        case AV_SAMPLE_FMT_DBL:
            sample_fmt = "AV_SAMPLE_FMT_DBL";
            break;
        case AV_SAMPLE_FMT_U8P:
            sample_fmt = "AV_SAMPLE_FMT_U8P";
            break;
        case AV_SAMPLE_FMT_S16P:
            sample_fmt = "AV_SAMPLE_FMT_S16P";
            break;
        case AV_SAMPLE_FMT_S32P:
            sample_fmt = "AV_SAMPLE_FMT_S32P";
            break;
        case AV_SAMPLE_FMT_FLTP:
            sample_fmt = "AV_SAMPLE_FMT_FLTP";
            break;
        case AV_SAMPLE_FMT_DBLP:
            sample_fmt = "AV_SAMPLE_FMT_DBLP";
            break;
        case AV_SAMPLE_FMT_S64:
            sample_fmt = "AV_SAMPLE_FMT_S64";
            break;
        case AV_SAMPLE_FMT_S64P:
            sample_fmt = "AV_SAMPLE_FMT_S64P";
            break;
        case AV_SAMPLE_FMT_NB:
            sample_fmt = "AV_SAMPLE_FMT_NB";
            break;
        default:
            sample_fmt = "";
    }
    printf("Sample format：         \t%d\t%s\n", in_sample_fmt, sample_fmt);
    printf("Stream all_pkts_size：         \t%d\n", avformat_context->nb_streams);
    printf("Stream index：         \t%d\n", audio_stream_index);
    AVDictionaryEntry *avdictionary_entry = NULL;
    while (avdictionary_entry = av_dict_get(avformat_context->streams[audio_stream_index]->metadata,
                                            "",
                                            avdictionary_entry, AV_DICT_IGNORE_SUFFIX)) {
        printf("Writing library：     \t%s\n", avdictionary_entry->value);
    }
    printf("Compile information： \t%s\n", avcodec_configuration());
    printf("\n");
    /*puts("FFMPEG支持的所有输入文件格式：");
    AVInputFormat *pInputFormat = srcAVFormatContext->iformat;
    while (pInputFormat) {
        printf("%s ", pInputFormat->name);
        pInputFormat = pInputFormat->next;
    }*/
    // 下面的内容还不知道怎么得到
//    printf("File all_pkts_size： %ld\n", );
//    printf("Format version： %s\n", );
//    printf("Format profile：%s\n", );
//    printf("Bit rate mode： %d\n", );
//    printf("Compression mode： %d\n", );
//    printf("Encoding settings： %ld\n", );
}


#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

void video_encode_example(const char *filename, int codec_id) {
    AVCodec *codec;
    AVCodecContext *c = NULL;
    int i = 0;
    int out_size = 0;
    int outbuf_size = 0;
    int inbuf_size = 0;

    FILE *f, *InputFileHead;
    AVFrame *picture;
    uint8_t *outbuf, *inbuf;
    int had_output = 0;

    int index = 0;

    printf("Video encoding\n");


    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder((AVCodecID) codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }


    c = avcodec_alloc_context3(codec);
//    picture = avcodec_alloc_frame(); //这里分配了帧



    /* put sample parameters */
    c->bit_rate = 400000 * 2;
    /* resolution must be a multiple of two */
    c->width = 640;//1920;//352;
    c->height = 480;//1080;//288;
    /* frames per second */
    c->time_base.num = 1; //= (AVRational){1,25};
    c->time_base.den = 24;
    c->gop_size = 10; /* emit one intra frame every ten frames */
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;


//下面的参数影响着编码出视频的质量,没有这两个参数视频会有些模糊的马赛克,加入这两个参数值就没有了
//Q值最小值
    c->qmin = 1;
//Q值最大值.
    c->qmax = 5;


//    if (codec_id == CODEC_ID_H264)
//        av_opt_set(c->priv_data, "preset", "slow", 0);


    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }


    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }


//编码一张位图图像
    InputFileHead = fopen("C:\\a.bmp", "r"); //注意：这里的位图文件路径设置成你的文件，同时注意位图文件的宽高也作同样的修改

    /* alloc image and output buffer */
    outbuf_size = 100000 + 12 * c->width * c->height;
    outbuf = (uint8_t *) malloc(outbuf_size);


//分配内存大小
    inbuf_size = c->width * c->height * 3;
    inbuf = (uint8_t *) malloc(inbuf_size);


    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    av_image_alloc(picture->data, picture->linesize,
                   c->width, c->height, c->pix_fmt, 1);


//----------------------------------------------------------------------------------------------
//这里用来加载一张图片来进行编码
    fseek(InputFileHead, 54, SEEK_SET);
    fread(inbuf, 1, inbuf_size, InputFileHead);

    struct SwsContext *sws;
//下面的位图转完是倒立的
    uint8_t *rgb_src[3] = {inbuf, NULL, NULL};
    int rgb_stride[3] = {3 * c->width, 0, 0};

//    sws = sws_getContext(c->width, c->height, PIX_FMT_RGB24, c->width, c->height, AV_PIX_FMT_YUV420P, 2, NULL, NULL, NULL);
    sws_scale(sws, (const uint8_t *const *) rgb_src, rgb_stride, 0, c->height, picture->data, picture->linesize);


//----------------------------------------------------------------------------------------------


    /* encode 1 second of video */

    while (i < 350/*feof(InputFile)*/) {
        fflush(stdout);


        /* encode the image */
//        out_size = avcodec_encode_video(c, outbuf, outbuf_size, picture);
        had_output |= out_size;
        printf("encoding frame %3d (all_pkts_size=%5d)\n", i++, out_size);
        fwrite(outbuf, 1, out_size, f);
    }



    /* get the delayed frames */
    for (; out_size || !had_output; i++) {
        fflush(stdout);


//        out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
        had_output |= out_size;
        printf("write frame %3d (all_pkts_size=%5d)\n", i, out_size);
        fwrite(outbuf, 1, out_size, f);
    }


    /* add sequence end code to have a real mpeg file */
    outbuf[0] = 0x00;
    outbuf[1] = 0x00;
    outbuf[2] = 0x01;
    outbuf[3] = 0xb7;
    fwrite(outbuf, 1, 4, f);
    fclose(f);
    free(outbuf);


    avcodec_close(c);
    av_free(c);
    //av_free(picture->data[0]);
    av_free(picture);
    printf("\n");
}

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
//Output PCM
#define OUTPUT_PCM 1
//Use SDL
#define PLAYBACK_AUDIO_WITH_SDL2 1


/***
 * The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled
 * len: The length (in bytes) of the audio buffer
 * 回调函数
*/
void alexander_fill_audio(void *udata, Uint8 *stream, int len) {
    if (audio_len == 0)        /*  Only  play  if  we  have  data  left  */
        return;
    len = (len > audio_len ? audio_len : len);    /*  Mix  as  much  data  as  possible  */

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

//int playbackAudio() {
//    AVFormatContext *pFormatCtx;
//    int i, audioStreamIndex;
//    AVCodecContext *pCodecCtx;
//    AVCodec *pCodec;
//
//    char url[] = "WavinFlag.aac";
//    //char url[]="WavinFlag.mp3";
//    //char url[]="72bian.wma";
//
//    av_register_all();
//    avformat_network_init();
//    pFormatCtx = avformat_alloc_context();
//    //Open
//    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {
//        printf("Couldn't open input stream.\n");
//        return -1;
//    }
//    // Retrieve stream information
//    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
//        printf("Couldn't find stream information.\n");
//        return -1;
//    }
//    // Dump valid information onto standard error
//    av_dump_format(pFormatCtx, 0, url, false);
//
//    // Find the first audio stream
//    audioStreamIndex = -1;
//    for (i = 0; i < pFormatCtx->nb_streams; i++)
//        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
//            audioStreamIndex = i;
//            break;
//        }
//
//    if (audioStreamIndex == -1) {
//        printf("Didn't find a audio stream.\n");
//        return -1;
//    }
//
//    // Get a pointer to the codec context for the audio stream
//    pCodecCtx = pFormatCtx->streams[audioStreamIndex]->codec;
//
//    // Find the decoder for the audio stream
//    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
//    if (pCodec == NULL) {
//        printf("Codec not found.\n");
//        return -1;
//    }
//
//    // Open codec
//    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
//        printf("Could not open codec.\n");
//        return -1;
//    }
//
//    FILE *pFile = NULL;
//#if OUTPUT_PCM
//    pFile = fopen("output.pcm", "wb");
//#endif
//
//    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket));
//    av_init_packet(packet);
//
//    //Out Audio Param
//    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
//    //AAC:1024  MP3:1152
//    int out_nb_samples = pCodecCtx->frame_size;
//    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
//    int out_sample_rate = 44100;
//    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
//    //Out Buffer Size
//    int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
//
//    uint8_t *out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
//
//    AVFrame *pFrame;
//    pFrame = avcodec_alloc_frame();
////SDL------------------
//#if USE_SDL
//    //Init
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
//        printf("Could not initialize SDL - %s\n", SDL_GetError());
//        return -1;
//    }
//    //SDL_AudioSpec
//    SDL_AudioSpec wanted_spec;
//    wanted_spec.freq = out_sample_rate;
//    wanted_spec.format = AUDIO_S16SYS;
//    wanted_spec.channels = out_channels;
//    wanted_spec.silence = 0;
//    wanted_spec.samples = out_nb_samples;
//    wanted_spec.callback = fill_audio;
//    wanted_spec.userdata = pCodecCtx;
//
//    if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
//        printf("can't open audio.\n");
//        return -1;
//    }
//#endif
//    printf("Bitrate:\t %3d\n", pFormatCtx->bit_rate);
//    printf("Decoder Name:\t %s\n", pCodecCtx->codec->long_name);
//    printf("Channels:\t %d\n", pCodecCtx->channels);
//    printf("Sample per Second\t %d \n", pCodecCtx->sample_rate);
//
//    uint32_t ret, len = 0;
//    int got_picture;
//    int index = 0;
//    //FIX:Some Codec's Context Information is missing
//    int64_t in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
//    //Swr
//    struct SwrContext *au_convert_ctx;
//    au_convert_ctx = swr_alloc();
//    au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
//                                        in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
//    swr_init(au_convert_ctx);
//
//    //Play
//    SDL_PauseAudio(0);
//
//    while (av_read_frame(pFormatCtx, packet) >= 0) {
//        if (packet->stream_index == audioStreamIndex) {
//
//            ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_picture, packet);
//            if (ret < 0) {
//                printf("Error in decoding audio frame.\n");
//                return -1;
//            }
//            if (got_picture > 0) {
//                swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **) pFrame->data,
//                            pFrame->nb_samples);
//
//                printf("index:%5d\t pts:%lld\t packet all_pkts_size:%d\n", index, packet->pts, packet->all_pkts_size);
//
//#if OUTPUT_PCM
//                //Write PCM
//                fwrite(out_buffer, 1, out_buffer_size, pFile);
//#endif
//
//                index++;
//            }
////SDL------------------
//#if USE_SDL
//            //Set audio buffer (PCM data)
//            audio_chunk = (Uint8 *) out_buffer;
//            //Audio buffer length
//            audio_len = out_buffer_size;
//
//            audio_pos = audio_chunk;
//
//            while (audio_len > 0)//Wait until finish
//                SDL_Delay(1);
//#endif
//        }
//        av_free_packet(packet);
//    }
//
//    swr_free(&au_convert_ctx);
//
//#if USE_SDL
//    SDL_CloseAudio();//Close SDL
//    SDL_Quit();
//#endif
//
//#if OUTPUT_PCM
//    fclose(pFile);
//#endif
//    av_free(out_buffer);
//    avcodec_close(pCodecCtx);
//    av_close_input_file(pFormatCtx);
//
//    return 0;
//}

//int audio_decoder() {
//    const char *input_filename = "";
//    av_register_all();
//    AVFormatContext *container = avformat_alloc_context();
//    if (avformat_open_input(&container, input_filename, NULL, NULL) < 0) {
//        printf("Could not open file");
//        return -1;
//    }
//    if (avformat_find_stream_info(container, NULL) < 0) {
//        printf("Could not find file info");
//        return -1;
//    }
//    av_dump_format(container, 0, input_filename, false);
//    int stream_id = -1;
//    int i;
//    for (i = 0; i < container->nb_streams; i++) {
//        if (container->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
//            stream_id = i;
//            break;
//        }
//    }
//    if (stream_id == -1) {
//        printf("Could not find Audio Stream");
//    }
//    AVDictionary *metadata = container->metadata;
//    AVCodecContext *ctx = container->streams[stream_id]->codec;
//    AVCodec *codec = avcodec_find_decoder(ctx->codec_id);
//    if (codec == NULL) {
//        printf("cannot find codec!");
//    }
//    if (avcodec_open2(ctx, codec, NULL) < 0) {
//        printf("Codec cannot be found");
//    }
//    AVSampleFormat sfmt = ctx->sample_fmt;
//    AVPacket packet;
//    av_init_packet(&packet);
//    AVFrame *frame = avcodec_alloc_frame();
//    // int buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;
//    int buffer_size = 64;
//    uint8_t buffer[buffer_size];
//    packet.data = buffer;
//    packet.all_pkts_size = buffer_size;
//    FILE *outfile = fopen("test.raw", "wb");
//    int len;
//    int frameFinished = 0;
//    while (av_read_frame(container, &packet) >= 0) {
//        if (packet.stream_index == stream_id) {
//            //printf("Audio Frame read \n");
//            int len = avcodec_decode_audio4(ctx, frame, &frameFinished, &packet);
//            if (frameFinished) {
//                if (sfmt == AV_SAMPLE_FMT_S16P) { // Audacity: 16bit PCM little endian stereo
//                    int16_t *ptr_l = (int16_t *) frame->extended_data[0];
//                    int16_t *ptr_r = (int16_t *) frame->extended_data[1];
//                    for (int i = 0; i < frame->nb_samples; i++) {
//                        fwrite(ptr_l++, sizeof(int16_t), 1, outfile);
//                        fwrite(ptr_r++, sizeof(int16_t), 1, outfile);
//                    }
//                } else if (sfmt ==
//                           AV_SAMPLE_FMT_FLTP) { //Audacity: big endian 32bit stereo start offset 7 (but has noise)
//                    float *ptr_l = (float *) frame->extended_data[0];
//                    float *ptr_r = (float *) frame->extended_data[1];
//                    for (int i = 0; i < frame->nb_samples; i++) {
//                        fwrite(ptr_l++, sizeof(float), 1, outfile);
//                        fwrite(ptr_r++, sizeof(float), 1, outfile);
//                    }
//                }
//            }
//        }
//    }
//    fclose(outfile);
//    avformat_close_input(&container);
//    return 0;
//}



int simplest_ffmpeg_audio_decoder() {
    int i, audioStreamIndex = -1;
    int result;

    char filePath[] = "/root/音乐/01_VBR_16kHz_64kbps_Stereo.mp3";
    FILE *pFile = fopen("/root/音乐/output.pcm", "wb");

    AVFormatContext *avFormatContext = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVCodecParameters *audioAVCodecParameters = NULL;
    AVCodec *audioAVCodecDecoder = NULL;
    AVSampleFormat audioAVSampleFormat;

    AVFrame *srcAVFrame = NULL, *dstAVFrame = NULL;
    // AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
    AVPacket *avPacket = NULL;
    struct SwrContext *audioSwrContext;

    // 注册FFmpeg所有编解码器
    av_register_all();
    avformat_network_init();
    // 得到AVFormatContext结构体
    avFormatContext = avformat_alloc_context();

    // 打开文件
    if (avformat_open_input(&avFormatContext, filePath, NULL, NULL)) {
        printf("Couldn't open input stream.\n");
        return -1;
    }

    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    av_dump_format(avFormatContext, 0, filePath, false);

    int nb_streams = avFormatContext->nb_streams;
    printf("avFormatContext->nb_streams = %d\n", nb_streams);// 2
    for (i = 0; i < nb_streams; i++) {
        AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
        printf("avMediaType = %d\n", avMediaType);
        if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            audioAVCodecParameters = avFormatContext->streams[audioStreamIndex]->codecpar;
            break;
        }
    }
    printf("audioIndex = %d\n", audioStreamIndex);// 0
    if (audioStreamIndex == -1) {
        printf("Didn't find a video stream.\n");
        return -1;
    }

    // 得到AVCodec结构体
    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    audioAVCodecDecoder = avcodec_find_decoder(audioAVCodecParameters->codec_id);
    if (audioAVCodecDecoder == NULL) {
        printf("Codec not found.\n");
        return -1;
    }

    // 必须要申请内存
    audioAVCodecContext = avcodec_alloc_context3(NULL);
    if (audioAVCodecContext == NULL) {
        printf("videoAVCodecContext is NULL or audioAVCodecContext is NULL.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(audioAVCodecContext, audioAVCodecParameters) < 0) {
        printf("copy audioAVCodecParameters to context fail\n");
        return -1;
    }

    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }

    srcAVFrame = av_frame_alloc();
    dstAVFrame = av_frame_alloc();
    avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(avPacket);

    audioAVSampleFormat = audioAVCodecContext->sample_fmt;
    printf("audioAVSampleFormat = %d\n", audioAVSampleFormat);

    const char *sample_fmt_name = av_get_sample_fmt_name(audioAVSampleFormat);
    printf("sample_fmt_name = %d\n", sample_fmt_name);

    uint8_t *out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    //Out Audio Param
    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
    //nb_samples: AAC-1024 MP3-1152
    int out_nb_samples = audioAVCodecContext->frame_size;
    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    int out_sample_rate = 44100;
    int out_nb_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    //Out Buffer Size
    int out_buffer_size = av_samples_get_buffer_size(NULL, out_nb_channels, out_nb_samples, out_sample_fmt, 1);
    //FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout = av_get_default_channel_layout(audioAVCodecContext->channels);
    // Swr init
    audioSwrContext = swr_alloc();
    audioSwrContext = swr_alloc_set_opts(audioSwrContext,
                                         out_channel_layout,
                                         out_sample_fmt,
                                         out_sample_rate,
                                         in_channel_layout,
                                         audioAVCodecContext->sample_fmt,
                                         audioAVCodecContext->sample_rate,
                                         0,
                                         NULL);
    swr_init(audioSwrContext);

    printf("while loop start.\n");

    int index = 0;

    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        if (avPacket->stream_index == audioStreamIndex) {
            result = avcodec_send_packet(audioAVCodecContext, avPacket);
            // printf("audio result = %d\n", result);
            switch (result) {
                case 0:
                    break;
                case AVERROR(EAGAIN):
                    printf("AUDIO AVERROR(EAGAIN)\n");
                    return -1;
                case AVERROR(EINVAL):
                    printf("AUDIO AVERROR(EINVAL)\n");
                    return -1;
                case AVERROR(ENOMEM):
                    printf("AUDIO AVERROR(ENOMEM)\n");
                    return -1;
                case AVERROR_EOF:
                    printf("AUDIO AVERROR_EOF\n");
                    return -1;
                default:
                    // [mp3float @ 0x3b10e40] Header missing
                    printf("AUDIO OTHER ERROR\n");
                    return -1;
            }
            while (avcodec_receive_frame(audioAVCodecContext, srcAVFrame) >= 0) {
                // Audacity: 16bit PCM little endian stereo
                if (audioAVSampleFormat == AV_SAMPLE_FMT_S16P) {// 6
                    int16_t *ptr_l = (int16_t *) srcAVFrame->extended_data[0];
                    int16_t *ptr_r = (int16_t *) srcAVFrame->extended_data[1];
                    for (int i = 0; i < srcAVFrame->nb_samples; i++) {
                        /*fwrite(ptr_l++, sizeof(int16_t), 1, outfile);
                        fwrite(ptr_r++, sizeof(int16_t), 1, outfile);*/
                    }
                    //Audacity: big endian 32bit stereo start offset 7 (but has noise)
                } else if (audioAVSampleFormat == AV_SAMPLE_FMT_FLTP) {// 8
                    swr_convert(audioSwrContext,
                                &out_buffer,
                                MAX_AUDIO_FRAME_SIZE,
                                (const uint8_t **) srcAVFrame->data,
                                srcAVFrame->nb_samples);
                    /*swr_convert(audioSwrContext,
                                dstAVFrame->data,
                                MAX_AUDIO_FRAME_SIZE,
                                (const uint8_t **) decodedAVFrame->data,
                                decodedAVFrame->nb_samples);*/

                    printf("index:%5d\tavPacket->pts:%lld\tavPacket->all_pkts_size:%d\n", index, avPacket->pts,
                           avPacket->size);
                    //Write PCM
                    fwrite(out_buffer, 1, out_buffer_size, pFile);
                    index++;
                }
            }
        }
        av_packet_unref(avPacket);
    }

    printf("while loop end.\n");

    swr_free(&audioSwrContext);
    swr_close(audioSwrContext);
    av_frame_free(&srcAVFrame);
    av_frame_free(&dstAVFrame);
    avcodec_close(audioAVCodecContext);
    avcodec_parameters_free(&audioAVCodecParameters);
    avformat_close_input(&avFormatContext);

    return 0;
}


int initOutputEncoder(AVFormatContext *avFormatContext,
                      AVCodecParameters *videoAVCodecParameters,
                      int videoIndex) {
    // output encoder initialize
    AVCodecContext *videoAVCodecContext = NULL;
    AVCodec *videoAVCodecEncoder = NULL;
    videoAVCodecEncoder = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!videoAVCodecEncoder) {
        printf("Can not find encoder! (没有找到合适的编码器！)\n");
        return -1;
    }
    videoAVCodecContext = avcodec_alloc_context3(videoAVCodecEncoder);
    videoAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    videoAVCodecContext->width = videoAVCodecParameters->width;
    videoAVCodecContext->height = videoAVCodecParameters->height;
    videoAVCodecContext->time_base.num = 1;
    videoAVCodecContext->time_base.den = 25;
    videoAVCodecContext->bit_rate = 400000;
    videoAVCodecContext->gop_size = 250;
    /* Some formats,for example,flv, want stream headers to be separate. */
    if (avFormatContext->oformat->flags & AVFMT_GLOBALHEADER) {
        videoAVCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    //H264 codec param
    //pCodecCtx->me_range = 16;
    //pCodecCtx->max_qdiff = 4;
    //pCodecCtx->qcompress = 0.6;
    videoAVCodecContext->qmin = 10;
    videoAVCodecContext->qmax = 51;
    // Optional Param
    videoAVCodecContext->max_b_frames = 3;
    // Set H264 preset and tune
    AVDictionary *param = 0;
    av_dict_set(&param, "preset", "fast", 0);
    av_dict_set(&param, "tune", "zerolatency", 0);

    if (avcodec_open2(videoAVCodecContext, videoAVCodecEncoder, &param) < 0) {
        printf("Failed to open encoder! (编码器打开失败！)\n");
        return -1;
    }
    return 0;
}


static void encode(AVCodecContext *enc_ctx,
                   AVFrame *frame,
                   AVPacket *pkt,
                   FILE *outfile) {
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3" PRId64 "\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        printf("Write packet %3" PRId64 " (all_pkts_size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

// 需要引入独立的x264 库：先下载x264的源码，编译之后；然后configure ffmpeg时加上enable x264 的选
int yuv_2_h264() {
    const char *filename, *codec_name;
    const AVCodec *codec;
    AVCodecContext *videoAVCodecContext = NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = {0, 0, 1, 0xb7};

    filename = "/root/视频/sintel_480x272_yuv420p.yuv";
    codec_name = "libx264";

    av_register_all();

    /* find the mpeg1video encoder */
//    codec = avcodec_find_encoder_by_name(codec_name);
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    videoAVCodecContext = avcodec_alloc_context3(codec);
    if (!videoAVCodecContext) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    videoAVCodecContext->bit_rate = 400000;
    /* resolution must be a multiple of two */
    videoAVCodecContext->width = 352;
    videoAVCodecContext->height = 288;
    /* frames per second */
    videoAVCodecContext->time_base = (AVRational) {1, 25};
    videoAVCodecContext->framerate = (AVRational) {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    videoAVCodecContext->gop_size = 10;
    videoAVCodecContext->max_b_frames = 1;
    videoAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(videoAVCodecContext->priv_data, "preset", "slow", 0);

    /* open it */
    ret = avcodec_open2(videoAVCodecContext, codec, NULL);
    if (ret < 0) {
        //fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }

    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = videoAVCodecContext->pix_fmt;
    frame->width = videoAVCodecContext->width;
    frame->height = videoAVCodecContext->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }

    /* encode 1 second of video */
    for (i = 0; i < 25; i++) {
        fflush(stdout);

        /* make sure the frame data is writable */
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);

        /* prepare a dummy image */
        /* Y */
        for (y = 0; y < videoAVCodecContext->height; y++) {
            for (x = 0; x < videoAVCodecContext->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
            }
        }

        /* Cb and Cr */
        for (y = 0; y < videoAVCodecContext->height / 2; y++) {
            for (x = 0; x < videoAVCodecContext->width / 2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }

        frame->pts = i;

        /* encode the image */
        encode(videoAVCodecContext, frame, pkt, f);
    }

    /* flush the encoder */
    encode(videoAVCodecContext, NULL, pkt, f);

    /* add sequence end code to have a real MPEG file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_free_context(&videoAVCodecContext);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}

/***
    调用:
    //All picture's resolution is 1280x720
    //Gray Bar, from 16 to 235
    gen_yuv420p_graybar(1280,720,10,16,235);
    //Color Bar
    gen_rgb24_colorbar(1280,720);
    //10 bars, RGB changed from 255,0,0 to 0,0,255
    gen_rgb24_rgbgradient_bar(1280,720,10,255,0,0,0,0,255);
    //10 bars, RGB changed from 0,0,0 to 128,128,128
    gen_yuv420p_yuvgradient_bar(1280,720,10,0,0,0,128,128,128);
    //RGB24 to BMP
    rgb24_to_bmp("colorbar_1280x720_rgb24.rgb","colorbar_1280x720_rgb24.bmp",1280,720);
    //Red stripe
    gen_rgb24_stripe(1280,720,255,0,0);
    //Gen color video
    gen_allcolor_video();
 */

/**
 * Generate Picture contains Stripe in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param r			Red component of stripe
 * @param g			Green component of stripe
 * @param b			Blue component of stripe
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_stripe(int width,
                     int height,
                     unsigned char r,
                     unsigned char g,
                     unsigned char b) {

    unsigned char *data = NULL;
    char filename[100] = {0};
    FILE *fp = NULL;
    int i = 0, j = 0;

    //Check
    if (width <= 0 || height <= 0) {
        printf("Error: Width, Height cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }

    data = (unsigned char *) malloc(width * height * 3);

    sprintf(filename, "rgbstripe_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            if (i % 2 != 0) {
                data[(j * width + i) * 3 + 0] = r;
                data[(j * width + i) * 3 + 1] = g;
                data[(j * width + i) * 3 + 2] = b;
            } else {//White
                data[(j * width + i) * 3 + 0] = 255;
                data[(j * width + i) * 3 + 1] = 255;
                data[(j * width + i) * 3 + 2] = 255;
            }
        }
    }
    fwrite(data, width * height * 3, 1, fp);
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);
    return 0;
}


/**
 * Generate Picture contains Gray Bar changing from Black to White in YUV420P Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param ymin		the minimum value of luminance.
 * @param ymax		the maximum value of luminance.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_yuv420p_graybar(int width,
                        int height,
                        int barnum,
                        unsigned char ymin,
                        unsigned char ymax) {

    int barwidth;
    float lum_inc;
    unsigned char lum_temp;
    int uv_width, uv_height;
    FILE *fp = NULL;
    unsigned char *data_y = NULL;
    unsigned char *data_u = NULL;
    unsigned char *data_v = NULL;
    int t = 0, i = 0, j = 0;
    char filename[100] = {0};

    //Check
    if (width <= 0 || height <= 0 || barnum <= 0) {
        printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
        barnum = 10;
    }
    if (width % barnum != 0) {
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");
    }
    barwidth = width / barnum;
    lum_inc = ((float) (ymax - ymin)) / ((float) (barnum - 1));
    uv_width = width / 2;
    uv_height = height / 2;

    data_y = (unsigned char *) malloc(width * height);
    data_u = (unsigned char *) malloc(uv_width * uv_height);
    data_v = (unsigned char *) malloc(uv_width * uv_height);

    sprintf(filename, "graybar_%dx%d_yuv420p.yuv", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("Y, U, V value from picture's left to right:\n");
    for (t = 0; t < (width / barwidth); t++) {
        lum_temp = ymin + (char) (t * lum_inc);
        printf("%3d, 128, 128\n", lum_temp);
    }
    //Gen Data
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            t = i / barwidth;
            lum_temp = ymin + (char) (t * lum_inc);
            data_y[j * width + i] = lum_temp;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            data_u[j * uv_width + i] = 128;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            data_v[j * uv_width + i] = 128;
        }
    }
    fwrite(data_y, width * height, 1, fp);
    fwrite(data_u, uv_width * uv_height, 1, fp);
    fwrite(data_v, uv_width * uv_height, 1, fp);
    fclose(fp);
    free(data_y);
    free(data_u);
    free(data_v);
    printf("Finish generate %s!\n", filename);
}

/**
 * Generate Picture contains standard Color Bar in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_colorbar(int width, int height) {

    unsigned char *data = NULL;
    int barwidth;
    char filename[100] = {0};
    FILE *fp = NULL;
    int i = 0, j = 0;
    int lum;
    float r_coeff = 0.299, g_coeff = 0.587, b_coeff = 0.114;

    //Check
    if (width <= 0 || height <= 0) {
        printf("Error: Width, Height cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }
    if (width % 8 != 0)
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");

    data = (unsigned char *) malloc(width * height * 3);
    barwidth = width / 8;

    sprintf(filename, "colorbar_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    printf("Luminance (Y) component value of colors from left to right:\n");
    lum = r_coeff * 255.0 + g_coeff * 255.0 + b_coeff * 255.0;
    printf("[White]  \tR,G,B=255,255,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 255.0 + g_coeff * 255.0 + b_coeff * 0.0;
    printf("[Yellow] \tR,G,B=255,255,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 255.0 + b_coeff * 255.0;
    printf("[Cyan]   \tR,G,B=  0,255,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 255.0 + b_coeff * 0.0;
    printf("[Green]  \tR,G,B=  0,255,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 255.0 + g_coeff * 0.0 + b_coeff * 255.0;
    printf("[Magenta]\tR,G,B=255,  0,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 255.0 + g_coeff * 0.0 + b_coeff * 0.0;
    printf("[Red]    \tR,G,B=255,  0,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 0.0 + b_coeff * 255.0;
    printf("[Blue]   \tR,G,B=  0,  0,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 0.0 + b_coeff * 0.0;
    printf("[Black]  \tR,G,B=  0,  0,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            int barnum = i / barwidth;
            switch (barnum) {
                case 0: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 255;
                    break;
                }
                case 1: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
                case 2: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 255;
                    break;
                }
                case 3: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
                case 4: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 255;
                    break;
                }
                case 5: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
                case 6: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 255;

                    break;
                }
                case 7: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
            }

        }
    }
    fwrite(data, width * height * 3, 1, fp);
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);
}

/**
 * Generate Picture contains Color Bar Changing from source color
 * to destination color in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param src_r		Red component of source color.
 * @param src_g		Green component of source color.
 * @param src_b		Blue component of source color.
 * @param dst_r		Red component of destination color.
 * @param dst_g		Green component of destination color.
 * @param dst_b		Blue component of destination color.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_rgbgradient_bar(int width, int height, int barnum,
                              unsigned char src_r,
                              unsigned char src_g,
                              unsigned char src_b,
                              unsigned char dst_r,
                              unsigned char dst_g,
                              unsigned char dst_b) {

    unsigned char *data = NULL;
    int barwidth;
    float r_inc, g_inc, b_inc;
    unsigned char r_temp, g_temp, b_temp;
    char filename[100] = {0};
    FILE *fp = NULL;
    int t = 0, i = 0, j = 0;

    //Check
    if (width <= 0 || height <= 0 || barnum <= 0) {
        printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }
    if (width % barnum != 0)
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");


    data = (unsigned char *) malloc(width * height * 3);
    barwidth = width / barnum;
    r_inc = ((float) (dst_r - src_r)) / ((float) (barnum - 1));
    g_inc = ((float) (dst_g - src_g)) / ((float) (barnum - 1));
    b_inc = ((float) (dst_b - src_b)) / ((float) (barnum - 1));

    sprintf(filename, "rgbgradientbar_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("R, G, B value from picture's left to right:\n");
    for (t = 0; t < (width / barwidth); t++) {
        r_temp = src_r + (char) (t * r_inc);
        g_temp = src_g + (char) (t * g_inc);
        b_temp = src_b + (char) (t * b_inc);
        printf("%3d, %3d, %3d\n", r_temp, g_temp, b_temp);
    }

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            t = i / barwidth;
            r_temp = src_r + (char) (t * r_inc);
            g_temp = src_g + (char) (t * g_inc);
            b_temp = src_b + (char) (t * b_inc);
            data[(j * width + i) * 3 + 0] = r_temp;
            data[(j * width + i) * 3 + 1] = g_temp;
            data[(j * width + i) * 3 + 2] = b_temp;
        }
    }
    fwrite(data, width * height * 3, 1, fp);
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);
    return 0;
}

/**
 * Generate Picture contains Color Bar Changing from source color
 * to destination color in YUV420P Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param src_y		Luma component of source color.
 * @param src_u		U component of source color.
 * @param src_v		V component of source color.
 * @param dst_y		Luma component of destination color.
 * @param dst_u		U component of destination color.
 * @param dst_v		V component of destination color.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_yuv420p_yuvgradient_bar(int width, int height, int barnum,
                                unsigned char src_y,
                                unsigned char src_u,
                                unsigned char src_v,
                                unsigned char dst_y,
                                unsigned char dst_u,
                                unsigned char dst_v) {

    int uv_width, uv_height;
    unsigned char *data_y = NULL;
    unsigned char *data_u = NULL;
    unsigned char *data_v = NULL;
    FILE *fp = NULL;
    int barwidth, uv_barwidth;
    float y_inc, u_inc, v_inc = 0;
    unsigned char y_temp, u_temp, v_temp = 0;
    char filename[100] = {0};
    int t = 0, i = 0, j = 0;
    //Check
    if (width <= 0 || height <= 0 || barnum <= 0) {
        printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }
    if (width % barnum != 0)
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");

    uv_width = width / 2;
    uv_height = height / 2;
    data_y = (unsigned char *) malloc(width * height);
    data_u = (unsigned char *) malloc(uv_width * uv_height);
    data_v = (unsigned char *) malloc(uv_width * uv_height);
    barwidth = width / barnum;
    uv_barwidth = barwidth / (width / uv_width);
    y_inc = ((float) (dst_y - src_y)) / ((float) (barnum - 1));
    u_inc = ((float) (dst_u - src_u)) / ((float) (barnum - 1));
    v_inc = ((float) (dst_v - src_v)) / ((float) (barnum - 1));

    sprintf(filename, "yuvgradientbar_%dx%d_yuv420p.yuv", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("Y, U, V value from picture's left to right:\n");
    for (t = 0; t < (width / barwidth); t++) {
        y_temp = src_y + (char) (t * y_inc);
        u_temp = src_u + (char) (t * u_inc);
        v_temp = src_v + (char) (t * v_inc);
        printf("%3d, %3d, %3d\n", y_temp, u_temp, v_temp);
    }

    //Gen Data
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            t = i / barwidth;
            y_temp = src_y + (char) (t * y_inc);
            data_y[j * width + i] = y_temp;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            t = i / uv_barwidth;
            u_temp = src_u + (char) (t * u_inc);
            data_u[j * uv_width + i] = u_temp;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            t = i / uv_barwidth;
            v_temp = src_v + (char) (t * v_inc);
            data_v[j * uv_width + i] = v_temp;
        }
    }
    fwrite(data_y, width * height, 1, fp);
    fwrite(data_u, uv_width * uv_height, 1, fp);
    fwrite(data_v, uv_width * uv_height, 1, fp);
    fclose(fp);
    free(data_y);
    free(data_u);
    free(data_v);
    printf("Finish generate %s!\n", filename);
    return 0;
}

/**
 * Convert RGB24 format to BMP format
 *
 * @param rgb24path		path of input RGB24 file.
 * @param bmppath		path of output BMP file
 * @param width			the width of picture.
 * @param height		the height of picture.
 * @return 0 if finished, -1 if there are errors.
 */
int rgb24_to_bmp(char *rgb24path, char *bmppath, int width, int height) {
    typedef struct {
        long imageSize;
        long blank;
        long startPosition;
    } BmpHead;

    typedef struct {
        long Length;
        long width;
        long height;
        unsigned short colorPlane;
        unsigned short bitColor;
        long zipFormat;
        long realSize;
        long xPels;
        long yPels;
        long colorUse;
        long colorImportant;
    } InfoHead;

    int i = 0, j = 0;
    BmpHead m_BMPHeader = {0};
    InfoHead m_BMPInfoHeader = {0};
    char bfType[2] = {'B', 'M'};
    int header_size = sizeof(bfType) + sizeof(BmpHead) + sizeof(InfoHead);
    unsigned char *rgb24_buffer = NULL;
    FILE *fp_rgb24 = NULL, *fp_bmp = NULL;

    if ((fp_rgb24 = fopen(rgb24path, "rb")) == NULL) {
        printf("Error: Cannot open input RGB24 file.\n");
        return -1;
    }
    if ((fp_bmp = fopen(bmppath, "wb")) == NULL) {
        printf("Error: Cannot open output BMP file.\n");
        return -1;
    }

    rgb24_buffer = (unsigned char *) malloc(width * height * 3);
    fread(rgb24_buffer, 1, width * height * 3, fp_rgb24);

    m_BMPHeader.imageSize = 3 * width * height + header_size;
    m_BMPHeader.startPosition = header_size;

    m_BMPInfoHeader.Length = sizeof(InfoHead);
    m_BMPInfoHeader.width = width;
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    m_BMPInfoHeader.height = -height;
    m_BMPInfoHeader.colorPlane = 1;
    m_BMPInfoHeader.bitColor = 24;
    m_BMPInfoHeader.realSize = 3 * width * height;

    fwrite(bfType, 1, sizeof(bfType), fp_bmp);
    fwrite(&m_BMPHeader, 1, sizeof(m_BMPHeader), fp_bmp);
    fwrite(&m_BMPInfoHeader, 1, sizeof(m_BMPInfoHeader), fp_bmp);

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            char temp = rgb24_buffer[(j * width + i) * 3 + 2];
            rgb24_buffer[(j * width + i) * 3 + 2] = rgb24_buffer[(j * width + i) * 3 + 0];
            rgb24_buffer[(j * width + i) * 3 + 0] = temp;
        }
    }
    fwrite(rgb24_buffer, 3 * width * height, 1, fp_bmp);
    fclose(fp_rgb24);
    fclose(fp_bmp);
    free(rgb24_buffer);
    printf("Finish generate %s!\n", bmppath);
    return 0;
}


/**
 * Generate a video in 256x256 and has 256 frames that contains all the colors.
 * Each color is shown in 1 pixel. They are mapped as follows:
 * In RGB24
 * R component's value is increasing with the growth of width (X-axis);
 * G component's value is increasing with the growth of height (Y-axis);
 * B component's value is increasing with the growth of frame number (Z-axis).
 * In YUV444P
 * U component's value is increasing with the growth of width (X-axis);
 * V component's value is increasing with the growth of height (Y-axis);
 * Y component's value is increasing with the growth of frame number (Z-axis).
 *
 * This function now support to draw YUV444P/RGB24 format pixel.
 *
 * @return 0 if finished, -1 if there are errors.
 */
int gen_allcolor_video() {

    unsigned char *data = NULL;
    char filename[100] = {0};
    FILE *fp = NULL;
    int width = 256, height = 256, frames = 256;
    int i = 0, j = 0, k = 0;

    //From left to right (width, X-axis),R increasing from 0 to255
    //From Top to bottom (height, Y-axis),G increasing from 0 to255
    //From 0 to 255 frames (time, Z-axis),B increasing from 0 to255
    data = (unsigned char *) malloc(width * height * 3);
    sprintf(filename, "allcolor_xr_yg_zb_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }
    for (k = 0; k < frames; k++) {
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i++) {
                data[(j * width + i) * 3 + 0] = i;
                data[(j * width + i) * 3 + 1] = j;
                data[(j * width + i) * 3 + 2] = k;
            }
        }
        fwrite(data, width * height * 3, 1, fp);
        printf("Finish generate frame %d!\n", k);
    }
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);

    //From left to right (width, X-axis),U increasing from 0 to255
    //From Top to bottom (height, Y-axis),V increasing from 0 to255
    //From 0 to 255 frames (time, Z-axis),Y increasing from 0 to255
    data = (unsigned char *) malloc(width * height);
    sprintf(filename, "allcolor_xu_yv_zy_%dx%d_yuv444p.yuv", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }
    for (k = 0; k < frames; k++) {
        for (j = 0; j < height; j++) {//Y
            for (i = 0; i < width; i++) {
                data[j * width + i] = k;
            }
        }
        fwrite(data, width * height, 1, fp);
        for (j = 0; j < height; j++) {//U
            for (i = 0; i < width; i++) {
                data[j * width + i] = i;
            }
        }
        fwrite(data, width * height, 1, fp);
        for (j = 0; j < height; j++) {//V
            for (i = 0; i < width; i++) {
                data[j * width + i] = j;
            }
        }
        fwrite(data, width * height, 1, fp);
        printf("Finish generate frame %d!\n", k);
    }
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);

    return 0;
}

int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index) {
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    /*if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
          CODEC_CAP_DELAY))
        return 0;*/
    while (1) {
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_audio2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
                                    NULL, &got_frame);
        av_frame_free(NULL);
        if (ret < 0)
            break;
        if (!got_frame) {
            ret = 0;
            break;
        }
        printf("Flush Encoder: Succeed to encode 1 frame!\tall_pkts_size:%5d\n", enc_pkt.size);
        /* mux encoded frame */
        ret = av_write_frame(fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }
    return ret;
}

/***
 pcm ---> aac
 成功
 */
int pcm2aac() {
    AVFormatContext *pFormatCtx;
    AVOutputFormat *fmt;
    AVStream *audio_st;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;

    uint8_t *frame_buf;
    AVFrame *pFrame;
    AVPacket pkt;

    int got_frame = 0;
    int ret = 0;
    int size = 0;

    FILE *in_file = NULL;                            //Raw PCM data
    int framenum = 1000;                          //Audio frame number
    const char *out_file = "/root/音乐/tdjm.aac";          //Output URL
    int i;

    in_file = fopen("/root/音乐/pcm/tdjm.pcm", "rb");

    av_register_all();

    //Method 1.
//    pFormatCtx = avformat_alloc_context();
//    fmt = av_guess_format(NULL, outFile, NULL);
//    pFormatCtx->oformat = fmt;

    //Method 2.
    avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
    fmt = pFormatCtx->oformat;

    //Open output URL
    if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
        printf("Failed to open output file!\n");
        return -1;
    }

    audio_st = avformat_new_stream(pFormatCtx, 0);
    if (audio_st == NULL) {
        return -1;
    }
    pCodecCtx = audio_st->codec;
    pCodecCtx->codec_id = fmt->audio_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    pCodecCtx->sample_fmt = AV_SAMPLE_FMT_S16;
    pCodecCtx->sample_rate = 44100;
    pCodecCtx->bit_rate = 320000;
    pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);

    //Show some information
//    av_dump_format(pFormatCtx, 0, outFile, 1);

//    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    pCodec = avcodec_find_encoder_by_name("libfdk_aac");
    if (!pCodec) {
        printf("Can not find encoder!\n");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Failed to open encoder!\n");
        return -1;
    }
    pFrame = av_frame_alloc();
    pFrame->nb_samples = pCodecCtx->frame_size;
    pFrame->format = pCodecCtx->sample_fmt;

    size = av_samples_get_buffer_size(NULL, pCodecCtx->channels, pCodecCtx->frame_size, pCodecCtx->sample_fmt, 1);
    frame_buf = (uint8_t *) av_malloc(size);
    avcodec_fill_audio_frame(pFrame, pCodecCtx->channels, pCodecCtx->sample_fmt, (const uint8_t *) frame_buf, size, 1);

    printf("Failed to read raw data! \n");
    //Write Header
    avformat_write_header(pFormatCtx, NULL);

    av_new_packet(&pkt, size);

    for (i = 0; i < framenum; i++) {
        //Read PCM
        if (fread(frame_buf, 1, size, in_file) <= 0) {
            printf("Failed to read raw data! \n");
            return -1;
        } else if (feof(in_file)) {
            break;
        }
        pFrame->data[0] = frame_buf;  //PCM Data

        pFrame->pts = i * 100;
        got_frame = 0;
        //Encode
        ret = avcodec_encode_audio2(pCodecCtx, &pkt, pFrame, &got_frame);
        if (ret < 0) {
            printf("Failed to encode!\n");
            return -1;
        }
        if (got_frame == 1) {
            printf("Succeed to encode 1 frame! \tall_pkts_size:%5d\n", pkt.size);
            pkt.stream_index = audio_st->index;
            ret = av_write_frame(pFormatCtx, &pkt);
            av_free_packet(&pkt);
        }
    }

    //Flush Encoder
    ret = flush_encoder(pFormatCtx, 0);
    if (ret < 0) {
        printf("Flushing encoder failed\n");
        return -1;
    }

    //Write Trailer
    av_write_trailer(pFormatCtx);

    //Clean
    if (audio_st) {
        avcodec_close(audio_st->codec);
        av_free(pFrame);
        av_free(frame_buf);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    fclose(in_file);

    return 0;
}

/***
 pcm ---> aac
 成功
 */
int pcm2aac(const char *in_pcm_path, const char *out_aac_path) {
    AVFrame *frame;
    AVCodec *codec = NULL;
    AVPacket packet;
    AVCodecContext *codecContext;
    int readSize = 0;
    int ret = 0, getPacket;
    FILE *fileIn, *fileOut;
    int frameCount = 0;
    /* register all the codecs */
    av_register_all();

    //1.我们需要读一帧一帧的数据，所以需要AVFrame结构
    //读出的一帧数据保存在AVFrame中。
    frame = av_frame_alloc();
    frame->channels = 2;
    frame->sample_rate = 44100;
//    frame->sample_rate = 11025;
    //样本格式
    frame->format = AV_SAMPLE_FMT_S16;
    frame->nb_samples = 1024;
    frame->data[0] = (uint8_t *) av_malloc(1024 * 4);

    //2.读出来的数据保存在AVPacket中，因此，我们还需要AVPacket结构体
    //初始化packet
    memset(&packet, 0, sizeof(AVPacket));
    av_init_packet(&packet);


    //3.读出来的数据，我们需要编码，因此需要编码器
    //下面的函数找到h.264类型的编码器
    /* find the mpeg1 video encoder */
    // 不行
    // codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    // 可行
    codec = avcodec_find_encoder_by_name("libfdk_aac");
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    //有了编码器，我们还需要编码器的上下文环境，用来控制编码的过程
    codecContext = avcodec_alloc_context3(codec);//分配AVCodecContext实例
    if (!codecContext) {
        fprintf(stderr, "Could not allocate video codec context\n");
        return -1;
    }
    /* put sample parameters */
    codecContext->sample_rate = frame->sample_rate;
    codecContext->channels = frame->channels;
    codecContext->sample_fmt = (enum AVSampleFormat) frame->format;
    /* select other audio parameters supported by the encoder */
    codecContext->channel_layout = AV_CH_LAYOUT_STEREO;
    //准备好了编码器和编码器上下文环境，现在可以打开编码器了
    //根据编码器上下文打开编码器
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }
    fileIn = fopen(in_pcm_path, "rb+");
    //4.准备输出文件
    fileOut = fopen(out_aac_path, "wb+");
    //下面开始编码
    while (1) {
        //读一帧数据出来
        readSize = fread(frame->data[0], 1, 1024 * 4, fileIn);
        if (readSize == 0) {
            fprintf(stdout, "end of file\n");
            frameCount++;
            break;
        }
        //初始化packet
        av_init_packet(&packet);
        /* encode the image */
        frame->pts = frameCount;
        ret = avcodec_encode_audio2(codecContext, &packet, frame, &getPacket); //将AVFrame中的像素信息编码为AVPacket中的码流
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            goto out;
        }

        if (getPacket) {
            frameCount++;
            //获得一个完整的编码帧
            printf("Write frame %3d (all_pkts_size=%5d)\n", frameCount, packet.size);
            fwrite(packet.data, 1, packet.size, fileOut);
            av_packet_unref(&packet);
        }
    }//while end

    /* flush buffer */
    for (getPacket = 1; getPacket; frameCount++) {
        frame->pts = frameCount;
        ret = avcodec_encode_audio2(codecContext, &packet, NULL, &getPacket);       //输出编码器中剩余的码流
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            goto out;
        }
        if (getPacket) {
            printf("flush buffer Write frame %3d (all_pkts_size=%5d)\n", frameCount, packet.size);
            fwrite(packet.data, 1, packet.size, fileOut);
            av_packet_unref(&packet);
        }
    } //for (got_output = 1; got_output; frameIdx++)

    out:
    fclose(fileIn);
    fclose(fileOut);
    av_frame_free(&frame);
    avcodec_close(codecContext);
    av_free(codecContext);
    return 0;
}

int encode(AVCodecContext *audioAVCodecContext,
           AVFrame *needToEncodedAVFrame,
           AVPacket *encodedAVPacket,
           FILE *outputFile,
           int &endocedFrameCount) {
    int result = avcodec_send_frame(audioAVCodecContext, needToEncodedAVFrame);
    // printf("audio result = %d\n", result);
    switch (result) {
        case 0:
            break;
        case AVERROR(EAGAIN):
            printf("AUDIO AVERROR(EAGAIN)\n");
            return -1;
        case AVERROR(EINVAL):
            printf("AUDIO AVERROR(EINVAL)\n");
            return -1;
        case AVERROR(ENOMEM):
            printf("AUDIO AVERROR(ENOMEM)\n");
            return -1;
        case AVERROR_EOF:
            printf("AUDIO AVERROR_EOF\n");
            return -1;
        default:
            printf("AUDIO OTHER ERROR\n");
            return -1;
    }
    // 一般情况下while循环也只执行一次
    while (avcodec_receive_packet(audioAVCodecContext, encodedAVPacket) >= 0) {
        printf("Succeed to encode frame: %5d\tall_pkts_size:%5d\n", ++endocedFrameCount, (*encodedAVPacket).size);
        fwrite((*encodedAVPacket).data, 1, (*encodedAVPacket).size, outputFile);
        av_packet_unref(encodedAVPacket);
    }
    return 0;
}

/***
 来自雷神的代码
 使用这个方法进行编码
 只能对pcm文件进行编码
 我在这里有个疑问:
 想要编码的采样率,比特率和声道数与源pcm的采样率,比特率和声道数一样,编码出来的音频才能正常播放吗

 音频编码一般只需要设置:
 1.采样率sample_rete
 2.比特率bit_rete
 3.声道数channels
 4.采样格式sample_fmt
 只要设置正确应该就能正常播放了
 */
int simplest_ffmpeg_audio_encoder_pure() {
    AVCodec *audioAVCodecEncoder = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVFrame *needToEncodedAVFrame = NULL;
    AVPacket *avPacket = NULL;
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    uint8_t *frame_buf = NULL;

    int endocedFrameCount = 0;

    char filename_in[] = "/root/音乐/audio.pcm";

    char filename_out[] = "/root/音乐/audio2.mp2";

    AVCodecID codec_id = AV_CODEC_ID_MP2;
    audioAVCodecEncoder = avcodec_find_encoder(codec_id);
    if (!audioAVCodecEncoder) {
        printf("Codec not found\n");
        return -1;
    }

    audioAVCodecContext = avcodec_alloc_context3(audioAVCodecEncoder);
    if (!audioAVCodecContext) {
        printf("Could not allocate video codec context\n");
        return -1;
    }
    audioAVCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;
    audioAVCodecContext->codec_id = codec_id;
    // 比特率降低后,音频体积也会降低
    audioAVCodecContext->bit_rate = 320000;// 64kbps/s
    // Specified sample format s16p is invalid or not supported(mp2)
    audioAVCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
    // 下面几个参数的设置参照官方sample
    // Specified sample rate 64000 is not supported(mp2)
    audioAVCodecContext->sample_rate = 48000;// 44100Hz(mp2最大支持这个48000)
    // Specified channel layout '3.0' is not supported(mp2)
    // 声道布局
    audioAVCodecContext->channel_layout = AV_CH_LAYOUT_STEREO;// 立体声
    // 双声道(根据声道布局得到声道数)
    audioAVCodecContext->channels = av_get_channel_layout_nb_channels(audioAVCodecContext->channel_layout);
    printf("audioAVCodecContext->channel_layout = %ld\n", audioAVCodecContext->channel_layout);
    printf("audioAVCodecContext->channels = %d\n", audioAVCodecContext->channels);

    // 打开编码器
    if (avcodec_open2(audioAVCodecContext, audioAVCodecEncoder, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }

    needToEncodedAVFrame = av_frame_alloc();
    if (!needToEncodedAVFrame) {
        fprintf(stderr, "Could not allocate audio frame\n");
        exit(1);
    }
    needToEncodedAVFrame->nb_samples = audioAVCodecContext->frame_size;
    needToEncodedAVFrame->format = audioAVCodecContext->sample_fmt;
    needToEncodedAVFrame->channel_layout = audioAVCodecContext->channel_layout;
    int samples_get_buffer_size = av_samples_get_buffer_size(NULL,
                                                             audioAVCodecContext->channels,
                                                             audioAVCodecContext->frame_size,
                                                             audioAVCodecContext->sample_fmt,
                                                             1);
    printf("samples_get_buffer_size = %d\n", samples_get_buffer_size);
    printf("audioAVCodecContext->frame_size = %d\n", audioAVCodecContext->frame_size);
    frame_buf = (uint8_t *) av_malloc(samples_get_buffer_size);
    avcodec_fill_audio_frame(needToEncodedAVFrame,
                             audioAVCodecContext->channels,
                             audioAVCodecContext->sample_fmt,
                             (const uint8_t *) frame_buf,
                             samples_get_buffer_size,
                             1);

    //Input raw data
    inputFile = fopen(filename_in, "rb");
    if (!inputFile) {
        printf("Could not open %s\n", filename_in);
        return -1;
    }
    //Output bitstream
    outputFile = fopen(filename_out, "wb");
    if (!outputFile) {
        printf("Could not open %s\n", filename_out);
        return -1;
    }

    /* packet for holding encoded output */
    avPacket = av_packet_alloc();
    if (!avPacket) {
        fprintf(stderr, "could not allocate the packet\n");
        exit(1);
    }

    //Encode
    for (;;) {
        /*av_init_packet(&avPacket);
        avPacket.data = NULL;    // packet data will be allocated by the encoder
        avPacket.all_pkts_size = 0;*/
        //Read raw data(raw:未经加工的)
        if (fread(frame_buf, 1, samples_get_buffer_size, inputFile) <= 0) {
            printf("Failed to read raw data! \n");
            return -1;
        } else if (feof(inputFile)) {
            break;
        }

        encode(audioAVCodecContext, needToEncodedAVFrame, avPacket, outputFile, endocedFrameCount);
    }

    encode(audioAVCodecContext, NULL, avPacket, outputFile, endocedFrameCount);

    fclose(outputFile);
    av_free(frame_buf);
    av_freep(&needToEncodedAVFrame->data[0]);
    av_frame_free(&needToEncodedAVFrame);
    av_packet_free(&avPacket);
    avcodec_close(audioAVCodecContext);
    av_free(audioAVCodecContext);

    return 0;
}

/***
 使用了libmp3lame库
 输入文件只能是pcm文件,其他封装文件都不行.
 因为是用fread函数读取数据的,如果是其他封装文件,那么读取出来的数据肯定是有问题的.
 封装文件需要解码后才可以,这里没有解码.
 */
int pcm2mp3(char *inPath, char *outPath) {
    int status = 0;
    lame_global_flags *global_flags;
    int ret_code;
    FILE *in_file;
    FILE *out_file;
    short *unencoded_pcm_buffer;
    int in_samples;
    unsigned char *encoded_mp3_buffer;
    int mp3_bytes;

    global_flags = lame_init();
    if (global_flags == NULL) {
        printf("lame_init failed\n");
        status = -1;
        goto exit;
    }

    // 设置被输入编码器的原始数据的采样率
    lame_set_in_samplerate(global_flags, 8000);
    // 设置最终mp3编码输出的声音的采样率，如果不设置则和输入采样率一样
    lame_set_out_samplerate(global_flags, 44100);
    // 设置被输入编码器的原始数据的声道数
    lame_set_num_channels(global_flags, 1);
    // 设置最终mp3编码输出的声道模式，如果不设置则和输入声道数一样。参数是枚举，STEREO代表双声道，MONO代表单声道
    lame_set_mode(global_flags, STEREO);
    // 设置比特率控制模式，默认是CBR，但是通常我们都会设置VBR。
    // 参数是枚举，vbr_off代表CBR，vbr_abr代表ABR
    //（因为ABR不常见，所以本文不对ABR做讲解）vbr_mtrh代表VBR
    lame_set_VBR(global_flags, vbr_off);
    // 设置CBR的比特率，只有在CBR模式下才生效
    lame_set_brate(global_flags, 8);
    // 2 = high 5 = medium 7 = low
    lame_set_quality(global_flags, 2);
    // 设置VBR的比特率，只有在VBR模式下才生效。
    // lame_set_VBR_mean_bitrate_kbps(global_flags, );

    ret_code = lame_init_params(global_flags);
    if (ret_code < 0) {
        printf("lame_init_params returned %d\n", ret_code);
        status = -1;
        goto close_lame;
    }

    in_file = fopen(inPath, "rb");
    out_file = fopen(outPath, "wb");

    unencoded_pcm_buffer = (short *) malloc(INBUF_SIZE * 2);
    encoded_mp3_buffer = (unsigned char *) malloc(MP3BUFSIZE);

    do {
        in_samples = fread(unencoded_pcm_buffer, 2, INBUF_SIZE, in_file);
        printf("input_samples is %d.\n", in_samples);
        //fprintf(stderr, "input_samples is %d./n", input_samples);
        /***
         编码PCM数据
            lame_encode_buffer或lame_encode_buffer_interleaved：将PCM数据送入编码器，获取编码出的mp3数据。这些数据写入文件就是mp3文件。
            其中lame_encode_buffer输入的参数中是双声道的数据分别输入的，lame_encode_buffer_interleaved输入的参数中双声道数据是交错在一起输入的。具体使用哪个需要看采集到的数据是哪种格式的，不过现在的设备采集到的数据大部分都是双声道数据是交错在一起。
            单声道输入只能使用lame_encode_buffer，把单声道数据当成左声道数据传入，右声道传NULL即可。
            调用这两个函数时需要传入一块内存来获取编码器出的数据，这块内存的大小lame给出了一种建议的计算方式：采样率/20+7200。
         */
        mp3_bytes = lame_encode_buffer_interleaved(global_flags,
                                                   unencoded_pcm_buffer,
                                                   in_samples / 2,
                                                   encoded_mp3_buffer,
                                                   MP3BUFSIZE);
        fprintf(stdout, "mp3_bytes is %d\n", mp3_bytes);
        if (mp3_bytes < 0) {
            printf("lame_encode_buffer_interleaved returned %d\n", mp3_bytes);
            status = -1;
            goto free_buffers;
        } else if (mp3_bytes > 0) {
            fwrite(encoded_mp3_buffer, 1, mp3_bytes, out_file);
        }
    } while (in_samples == INBUF_SIZE);

    //刷新编码器缓冲，获取残留在编码器缓冲里的数据。这部分数据也需要写入mp3文件
    mp3_bytes = lame_encode_flush(global_flags, encoded_mp3_buffer, sizeof(encoded_mp3_buffer));
    if (mp3_bytes > 0) {
        printf("writing %d mp3 bytes\n", mp3_bytes);
        fwrite(encoded_mp3_buffer, 1, mp3_bytes, out_file);
    }

    free_buffers:
    free(encoded_mp3_buffer);
    free(unencoded_pcm_buffer);

    fclose(out_file);
    fclose(in_file);
    close_lame:
    lame_close(global_flags);

    exit:
    return status;
}

/***
 * 网上的代码
 * 实现将音频的三大要素：声道，样本，采样率变更的demo
 * 例如双声道变成单声道，44100->48000，float->s16等
 * @param audio_avcodec_context
 * @param pAudioDecodeFrame
 * @param out_sample_fmt
 * @param out_nb_channels
 * @param out_sample_rate
 * @param out_buf
 * @return
 */
int AudioResampling(AVCodecContext *audio_avcodec_context, AVFrame *pAudioDecodeFrame,
                    int out_sample_fmt, int out_nb_channels, int out_sample_rate, uint8_t *out_buf) {
    //////////////////////////////////////////////////////////////////////////
    SwrContext *swr_ctx = NULL;
    int data_size = 0;
    int ret = 0;
    int64_t src_ch_layout = AV_CH_LAYOUT_STEREO; //初始化这样根据不同文件做调整
    int64_t dst_ch_layout = AV_CH_LAYOUT_STEREO; //这里设定ok
    int dst_nb_channels = 0;
    int dst_linesize = 0;
    int src_nb_samples = 0;
    int dst_nb_samples = 0;
    int max_dst_nb_samples = 0;
    uint8_t **dst_data = NULL;
    int resampled_data_size = 0;

    //重新采样
    if (swr_ctx) {
        swr_free(&swr_ctx);
    }
    swr_ctx = swr_alloc();
    if (!swr_ctx) {
        printf("swr_alloc error \n");
        return -1;
    }
    src_ch_layout = (audio_avcodec_context->channel_layout &&
                     audio_avcodec_context->channels ==
                     av_get_channel_layout_nb_channels(audio_avcodec_context->channel_layout)) ?
                    audio_avcodec_context->channel_layout :
                    av_get_default_channel_layout(audio_avcodec_context->channels);

    if (out_nb_channels == 1) {
        dst_ch_layout = AV_CH_LAYOUT_MONO;
    } else if (out_nb_channels == 2) {
        dst_ch_layout = AV_CH_LAYOUT_STEREO;
    } else {
        //可扩展
    }

    if (src_ch_layout <= 0) {
        printf("src_ch_layout error \n");
        return -1;
    }

    src_nb_samples = pAudioDecodeFrame->nb_samples;
    if (src_nb_samples <= 0) {
        printf("src_nb_samples error \n");
        return -1;
    }

    /* set options */
    av_opt_set_int(swr_ctx, "in_channel_layout", src_ch_layout, 0);
    av_opt_set_int(swr_ctx, "in_sample_rate", audio_avcodec_context->sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", audio_avcodec_context->sample_fmt, 0);

    av_opt_set_int(swr_ctx, "out_channel_layout", dst_ch_layout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", out_sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", (AVSampleFormat) out_sample_fmt, 0);
    swr_init(swr_ctx);

    max_dst_nb_samples = dst_nb_samples =
            av_rescale_rnd(src_nb_samples, out_sample_rate, audio_avcodec_context->sample_rate, AV_ROUND_UP);
    if (max_dst_nb_samples <= 0) {
        printf("av_rescale_rnd error \n");
        return -1;
    }

    dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);
    ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
                                             dst_nb_samples, (AVSampleFormat) out_sample_fmt, 0);
    if (ret < 0) {
        printf("av_samples_alloc_array_and_samples error \n");
        return -1;
    }


    dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, audio_avcodec_context->sample_rate) +
                                    src_nb_samples, out_sample_rate, audio_avcodec_context->sample_rate, AV_ROUND_UP);
    if (dst_nb_samples <= 0) {
        printf("av_rescale_rnd error \n");
        return -1;
    }
    if (dst_nb_samples > max_dst_nb_samples) {
        av_free(dst_data[0]);
        ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels,
                               dst_nb_samples, (AVSampleFormat) out_sample_fmt, 1);
        max_dst_nb_samples = dst_nb_samples;
    }

    data_size = av_samples_get_buffer_size(NULL, audio_avcodec_context->channels,
                                           pAudioDecodeFrame->nb_samples,
                                           audio_avcodec_context->sample_fmt, 1);
    if (data_size <= 0) {
        printf("av_samples_get_buffer_size error \n");
        return -1;
    }
    resampled_data_size = data_size;

    if (swr_ctx) {
        ret = swr_convert(swr_ctx,
                          dst_data,
                          dst_nb_samples,
                          (const uint8_t **) pAudioDecodeFrame->data,
                          pAudioDecodeFrame->nb_samples);
        if (ret <= 0) {
            printf("swr_convert error \n");
            return -1;
        }

        resampled_data_size = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
                                                         ret, (AVSampleFormat) out_sample_fmt, 1);
        if (resampled_data_size <= 0) {
            printf("av_samples_get_buffer_size error \n");
            return -1;
        }
    } else {
        printf("audioSwrContext null error \n");
        return -1;
    }

    //将值返回去
    memcpy(out_buf, dst_data[0], resampled_data_size);

    if (dst_data) {
        av_freep(&dst_data[0]);
    }
    av_freep(&dst_data);
    dst_data = NULL;

    if (swr_ctx) {
        swr_free(&swr_ctx);
    }
    return resampled_data_size;
}

/***
 来自网上的代码
 使用这个方法解码成的pcm文件再编码成mp3文件后,声音变了,这种声音蛮好听的
 */
int decode_audio2(const char *input_file_name, const char *output_file_name) {
    //1.注册组件
    av_register_all();
    //封装格式上下文
    AVFormatContext *av_format_context = avformat_alloc_context();

    //2.打开输入音频文件
    if (avformat_open_input(&av_format_context, input_file_name, NULL, NULL) != 0) {
        printf("%s\n", "打开输入音频文件失败");
        return -1;
    }
    //3.获取音频信息
    if (avformat_find_stream_info(av_format_context, NULL) < 0) {
        printf("%s\n", "获取音频信息失败");
        return -1;
    }

    //音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
    int audio_stream_index = -1;
    int i;
    for (i = 0; i < av_format_context->nb_streams; i++) {
        //根据类型判断是否是音频流
        if (av_format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    //4.获取解码器
    //根据索引拿到对应的流,根据流拿到解码器上下文
    AVCodecContext *audio_av_codec_context = av_format_context->streams[audio_stream_index]->codec;
    //再根据上下文拿到编解码id，通过该id拿到解码器
    AVCodec *audio_av_codec = avcodec_find_decoder(audio_av_codec_context->codec_id);
    if (audio_av_codec == NULL) {
        printf("%s\n", "无法解码");
        return -1;
    }
    //5.打开解码器
    if (avcodec_open2(audio_av_codec_context, audio_av_codec, NULL) < 0) {
        printf("%s\n", "编码器无法打开");
        return -1;
    }

    //编码数据
    AVPacket *av_packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    //解压缩数据
    AVFrame *frame = av_frame_alloc();

    //frame->16bit 44100 PCM 统一音频采样格式与采样率
    SwrContext *swr_context = swr_alloc();
    //重采样设置选项-----------------------------------------------------------start
    //输入的采样格式
    enum AVSampleFormat in_sample_fmt = audio_av_codec_context->sample_fmt;
    //输出的采样格式 16bit PCM
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入的采样率
    int in_sample_rate = audio_av_codec_context->sample_rate;
    //输出的采样率
    int out_sample_rate = 44100;
    //输入的声道布局
    uint64_t in_ch_layout = audio_av_codec_context->channel_layout;
    //输出的声道布局
    uint64_t out_ch_layout = AV_CH_LAYOUT_MONO;

    swr_alloc_set_opts(swr_context, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt,
                       in_sample_rate, 0, NULL);
    swr_init(swr_context);
    //重采样设置选项-----------------------------------------------------------end
    //获取输出的声道个数
    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
    //存储pcm数据
    uint8_t *out_buffer = (uint8_t *) av_malloc(2 * 44100);
    FILE *fp_pcm = fopen(output_file_name, "wb");
    int ret, got_frame, framecount = 0;
    //6.一帧一帧读取压缩的音频数据AVPacket
    while (av_read_frame(av_format_context, av_packet) >= 0) {
        if (av_packet->stream_index == audio_stream_index) {
            //解码AVPacket->AVFrame
            ret = avcodec_decode_audio4(audio_av_codec_context, frame, &got_frame, av_packet);
            if (ret < 0) {
                printf("%s\n", "解码完成");
            }
            //非0，正在解码
            if (got_frame) {
                printf("解码%d帧\n", framecount++);
                swr_convert(swr_context, &out_buffer, 2 * 44100, (const uint8_t **) frame->data, frame->nb_samples);
                //获取sample的size
                int out_buffer_size = av_samples_get_buffer_size(NULL, out_channel_nb,
                                                                 frame->nb_samples,
                                                                 out_sample_fmt, 1);
                //写入文件进行测试
                fwrite(out_buffer, 1, out_buffer_size, fp_pcm);
            }
        }
        av_free_packet(av_packet);
    }


    fclose(fp_pcm);
    av_frame_free(&frame);
    av_free(out_buffer);
    swr_free(&swr_context);
    avcodec_close(audio_av_codec_context);
    avformat_close_input(&av_format_context);
    return 0;
}

//样本枚举
enum AVSampleFormat_t {
    AV_SAMPLE_FMT_NONE_t = -1,
    AV_SAMPLE_FMT_U8_t,          ///< unsigned 8 bits
    AV_SAMPLE_FMT_S16_t,         ///< signed 16 bits
    AV_SAMPLE_FMT_S32_t,         ///< signed 32 bits
    AV_SAMPLE_FMT_FLT_t,         ///< float
    AV_SAMPLE_FMT_DBL_t,         ///< double

    AV_SAMPLE_FMT_U8P_t,         ///< unsigned 8 bits, planar
    AV_SAMPLE_FMT_S16P_t,        ///< signed 16 bits, planar
    AV_SAMPLE_FMT_S32P_t,        ///< signed 32 bits, planar
    AV_SAMPLE_FMT_FLTP_t,        ///< float, planar
    AV_SAMPLE_FMT_DBLP_t,        ///< double, planar

    AV_SAMPLE_FMT_NB_t           ///< Number of sample formats. DO NOT USE if linking dynamically
};

//多路输出每一路的信息结构体
typedef struct Out_stream_info_t {
    //user info
    int user_stream_id;                 //多路输出每一路的ID
    //video param
    int m_dwWidth;
    int m_dwHeight;
    double m_dbFrameRate;               //帧率
    int m_video_codecID;
    int m_video_pixelfromat;
    int m_bit_rate;                     //码率
    int m_gop_size;
    int m_max_b_frame;
    int m_thread_count;                 //用cpu内核数目
    //audio param
    int m_dwChannelCount;               //声道
    AVSampleFormat_t m_dwBitsPerSample; //样本
    int m_dwFrequency;                  //采样率
    int m_audio_codecID;

    //ffmpeg out pram
    AVAudioFifo *m_audiofifo;          //音频存放pcm数据
    int64_t m_first_audio_pts;          //第一帧的音频pts
    int m_is_first_audio_pts;           //是否已经记录第一帧音频时间戳
    AVFormatContext *m_ocodec;         //输出流context
    int m_writeheader_seccess;          //写头成功也就是写的头支持里面填写的音视频格式例如采样率等等
    AVStream *m_ovideo_st;
    AVStream *m_oaudio_st;
    AVCodec *m_audio_codec;
    AVCodec *m_video_codec;
    AVPacket m_pkt;
    AVBitStreamFilterContext *m_vbsf_aac_adtstoasc;     //aac->adts to asc过滤器
    struct SwsContext *m_img_convert_ctx_video;
    int m_sws_flags;                    //差值算法,双三次
    AVFrame *m_pout_video_frame;
    AVFrame *m_pout_audio_frame;
    SwrContext *m_swr_ctx;
    char m_outurlname[256];             //输出的url地址

    Out_stream_info_t() {
        //user info
        user_stream_id = 0;             //多路输出每一路的ID
        //video param
        m_dwWidth = 640;
        m_dwHeight = 480;
        m_dbFrameRate = 25;  //帧率
        m_video_codecID = (int) AV_CODEC_ID_H264;
        m_video_pixelfromat = (int) AV_PIX_FMT_YUV420P;
        m_bit_rate = 400000;                //码率
        m_gop_size = 12;
        m_max_b_frame = 0;
        m_thread_count = 2;                 //用cpu内核数目
        //audio param
        m_dwChannelCount = 2;               //声道
        m_dwBitsPerSample = AV_SAMPLE_FMT_S16_t; //样本
        m_dwFrequency = 44100;              //采样率
        m_audio_codecID = (int) AV_CODEC_ID_AAC;

        //ffmpeg out pram
        m_audiofifo = NULL;                 //音频存放pcm数据
        m_first_audio_pts = 0;              //第一帧的音频pts
        m_is_first_audio_pts = 0;           //是否已经记录第一帧音频时间戳
        m_ocodec = NULL;                    //输出流context
        m_writeheader_seccess = 0;
        m_ovideo_st = NULL;
        m_oaudio_st = NULL;
        m_audio_codec = NULL;
        m_video_codec = NULL;
        //m_pkt;
        m_vbsf_aac_adtstoasc = NULL;        //aac->adts to asc过滤器
        m_img_convert_ctx_video = NULL;
        m_sws_flags = SWS_BICUBIC;          //差值算法,双三次
        m_pout_video_frame = NULL;
        m_pout_audio_frame = NULL;
        m_swr_ctx = NULL;
        memset(m_outurlname, 0, 256);         //清零
    }
} Out_stream_info;

SwrContext *ffmpeg_init_pcm_resample(Out_stream_info *out_stream_info, AVFrame *in_frame, AVFrame *out_frame) {
    SwrContext *swr_ctx = NULL;
    swr_ctx = swr_alloc();
    if (!swr_ctx) {
        printf("swr_alloc error \n");
        return NULL;
    }
    AVCodecContext *audio_dec_ctx = NULL;
    AVSampleFormat sample_fmt;
    sample_fmt = (AVSampleFormat) out_stream_info->m_dwBitsPerSample; //样本
    int out_channel_layout = av_get_default_channel_layout(out_stream_info->m_dwChannelCount);
    if (audio_dec_ctx->channel_layout == 0) {
        int channels = 0;
        audio_dec_ctx->channel_layout = av_get_default_channel_layout(channels);
    }
    /* set options */
    av_opt_set_int(swr_ctx, "in_channel_layout", audio_dec_ctx->channel_layout, 0);
    av_opt_set_int(swr_ctx, "in_sample_rate", audio_dec_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", audio_dec_ctx->sample_fmt, 0);
    av_opt_set_int(swr_ctx, "out_channel_layout", out_channel_layout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", out_stream_info->m_dwFrequency, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", sample_fmt, 0);
    swr_init(swr_ctx);

    int64_t src_nb_samples = in_frame->nb_samples;
    //计算输出的samples 和采样率有关 例如：48000转44100，samples则是从1152转为1059，除法
    out_frame->nb_samples = av_rescale_rnd(src_nb_samples, out_stream_info->m_dwFrequency, audio_dec_ctx->sample_rate,
                                           AV_ROUND_UP);

    int ret = av_samples_alloc(out_frame->data, &out_frame->linesize[0],
                               out_stream_info->m_dwChannelCount, out_frame->nb_samples,
                               out_stream_info->m_oaudio_st->codec->sample_fmt, 1);
    if (ret < 0) {
        return NULL;
    }

    out_stream_info->m_audiofifo = av_audio_fifo_alloc(out_stream_info->m_oaudio_st->codec->sample_fmt,
                                                       out_stream_info->m_oaudio_st->codec->channels,
                                                       out_frame->nb_samples);

    return swr_ctx;
}

int ffmpeg_preform_pcm_resample(Out_stream_info *out_stream_info, SwrContext *pSwrCtx, AVFrame *in_frame,
                                AVFrame *out_frame) {
    int ret = 0;
    int samples_out_per_size = 0;              //转换之后的samples大小

    if (pSwrCtx != NULL) {
        //这里注意下samples_out_per_size这个值和 out_frame->nb_samples这个值有时候不一样，ffmpeg里面做了策略不是问题。
        samples_out_per_size = swr_convert(pSwrCtx, out_frame->data, out_frame->nb_samples,
                                           (const uint8_t **) in_frame->data, in_frame->nb_samples);
        if (samples_out_per_size < 0) {
            return -1;
        }

        AVCodecContext *audio_dec_ctx = NULL;

        int buffersize_in = av_samples_get_buffer_size(&in_frame->linesize[0], audio_dec_ctx->channels,
                                                       in_frame->nb_samples, audio_dec_ctx->sample_fmt, 1);

        //修改分包内存
        int buffersize_out = av_samples_get_buffer_size(&out_frame->linesize[0],
                                                        out_stream_info->m_oaudio_st->codec->channels,
                                                        samples_out_per_size,
                                                        out_stream_info->m_oaudio_st->codec->sample_fmt, 1);

        int fifo_size = av_audio_fifo_size(out_stream_info->m_audiofifo);
        fifo_size = av_audio_fifo_realloc(out_stream_info->m_audiofifo,
                                          av_audio_fifo_size(out_stream_info->m_audiofifo) + out_frame->nb_samples);
        av_audio_fifo_write(out_stream_info->m_audiofifo, (void **) out_frame->data, samples_out_per_size);
        fifo_size = av_audio_fifo_size(out_stream_info->m_audiofifo);

        out_frame->pkt_pts = in_frame->pkt_pts;
        out_frame->pkt_dts = in_frame->pkt_dts;
        //有时pkt_pts和pkt_dts不同，并且pkt_pts是编码前的dts,这里要给avframe传入pkt_dts而不能用pkt_pts
        //out_frame->pts = out_frame->pkt_pts;
        out_frame->pts = in_frame->pkt_dts;

        FILE *pcm_file = NULL;
        //测试用
        if (out_stream_info->user_stream_id == 11) {
            if (pcm_file == NULL) {
                pcm_file = fopen("11.pcm", "wb");
            }
            int wtiresize = fwrite(out_frame->data[0], buffersize_out, 1, pcm_file);
            fflush(pcm_file);
        }
    }
    ret = 1;
    return ret;
}

void ffmpeg_uinit_pcm_resample(SwrContext *swr_ctx, AVAudioFifo *audiofifo) {
    if (swr_ctx) {
        swr_free(&swr_ctx);
        swr_ctx = NULL;
    }
    if (audiofifo) {
        av_audio_fifo_free(audiofifo);
        audiofifo = NULL;
    }
}

/***
 代码注释比较好
 马上要废弃的代码,但是现在还能用(2018/7/20)
 */
int crazydiode_video_devoder() {
    const char *input_file_name_path = "/root/视频/haoke.avi";
    const char *output_file_name_path = "/root/视频/haoke.yuv";
    AVFormatContext *avformat_context = NULL;
    AVCodecContext *video_av_codec_context = NULL;

    //1.注册所有组件
    av_register_all();
    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
    avformat_context = avformat_alloc_context();
    //2.打开输入视频文件
    if (avformat_open_input(&avformat_context, input_file_name_path, NULL, NULL) != 0) {
        printf("%s", "");
        return -1;
    }
    //3.获取视频文件信息
    if (avformat_find_stream_info(avformat_context, NULL) < 0) {
        printf("%s", "");
        return -1;
    }

    int nb_streams = avformat_context->nb_streams;
    //获取视频流的索引位置
    //遍历所有类型的流（音频流、视频流、字幕流），找到视频流
    int video_stream_index = -1;
    int i;
    for (i = 0; i < nb_streams; i++) {
        //流的类型
        if (avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    if (video_stream_index == -1) {
        printf("%s", "");
        return -1;
    }

    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    video_av_codec_context = avformat_context->streams[video_stream_index]->codec;
    //4.根据编解码上下文中的编码id查找对应的解码
    AVCodec *video_decoder_av_codec = avcodec_find_decoder(video_av_codec_context->codec_id);
    if (video_decoder_av_codec == NULL) {
        printf("%s", "");
        return -1;
    }
    //5.打开解码器
    if (!avcodec_is_open(video_av_codec_context)) {
        if (avcodec_open2(video_av_codec_context, video_decoder_av_codec, NULL) < 0) {
            printf("%s", "");
            return -1;
        }
    }

    // 输出视频信息
    printf("视频的文件格式：%s\n", avformat_context->iformat->name);
    printf("视频时长：%d\n", (avformat_context->duration) / 1000000);
    printf("视频的宽高：%d,%d\n", video_av_codec_context->width, video_av_codec_context->height);
    printf("解码器的名称：%s\n", video_decoder_av_codec->name);

    //准备读取
    //AVPacket用于存储一帧一帧的压缩数据（H264）
    //缓冲区，开辟空间
    AVPacket *avpacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    //AVFrame用于存储解码后的像素数据(YUV)
    //内存分配
    AVFrame *src_avframe = av_frame_alloc();
    AVFrame *dst_avframe = av_frame_alloc();
    int av_picture_get_size = avpicture_get_size(AV_PIX_FMT_YUV420P, video_av_codec_context->width,
                                                 video_av_codec_context->height);
    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = (uint8_t *) av_malloc(av_picture_get_size);
    //初始化缓冲区
    avpicture_fill((AVPicture *) dst_avframe, out_buffer, AV_PIX_FMT_YUV420P, video_av_codec_context->width,
                   video_av_codec_context->height);
    //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
    struct SwsContext *video_sws_context = sws_getContext(video_av_codec_context->width, video_av_codec_context->height,
                                                          video_av_codec_context->pix_fmt,
                                                          video_av_codec_context->width, video_av_codec_context->height,
                                                          AV_PIX_FMT_YUV420P,
                                                          SWS_BICUBIC,
                                                          NULL, NULL, NULL);

    int get_picture, ret;
    FILE *output_file = fopen(output_file_name_path, "wb+");
    int frame_count = 0;

    printf("while loop start.\n");

    int y_size = video_av_codec_context->width * video_av_codec_context->height;
    //6.一帧一帧的读取压缩数据
    while (av_read_frame(avformat_context, avpacket) >= 0) {
        //只要视频压缩数据（根据流的索引位置判断）
        if (avpacket->stream_index == video_stream_index) {
            //7.解码一帧视频压缩数据，得到视频像素数据
            ret = avcodec_decode_video2(video_av_codec_context, src_avframe, &get_picture, avpacket);
            if (ret < 0) {
                printf("%s", "");
                break;
            }

            //为0说明解码完成，非0正在解码
            if (get_picture) {
                //AVFrame转为像素格式YUV420，宽高
                //2 6输入、输出数据
                //3 7输入、输出画面一行的数据的大小 AVFrame 转换是一行一行转换的
                //4 输入数据第一列要转码的位置 从0开始
                //5 输入画面的高度
                sws_scale(video_sws_context,
                          (const uint8_t *const *) src_avframe->data,
                          src_avframe->linesize, 0,
                          video_av_codec_context->height,
                          dst_avframe->data,
                          dst_avframe->linesize);
                //输出到YUV文件
                //AVFrame像素帧写入文件
                //data解码后的图像像素数据（音频采样数据）
                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感
                //U V 个数是Y的1/4
                fwrite(dst_avframe->data[0], 1, y_size, output_file);
                fwrite(dst_avframe->data[1], 1, y_size / 4, output_file);
                fwrite(dst_avframe->data[2], 1, y_size / 4, output_file);

                ++frame_count;
            }
        }
        //释放资源
        av_free_packet(avpacket);
    }
    printf("总共解码%d帧\n", frame_count);

    printf("while loop end.\n");

    fclose(output_file);
    av_frame_free(&src_avframe);
    av_frame_free(&dst_avframe);
    avcodec_close(video_av_codec_context);
    avformat_free_context(avformat_context);
}

/***

 */
int crazydiode_audio_devoder() {
    const char in_file_path[] = "/root/音乐/01_VBR_16kHz_64kbps_Stereo.mp3";
    const char out_file_path[] = "/root/音乐/01_VBR_16kHz_64kbps_Stereo.pcm";
    AVFormatContext *avformat_context = NULL;
    AVCodecContext *audio_avcodec_context = NULL;

    //1.注册组件
    av_register_all();
    //封装格式上下文
    avformat_context = avformat_alloc_context();
    //2.打开输入音频文件
    if (avformat_open_input(&avformat_context, in_file_path, NULL, NULL) != 0) {
        printf("%s\n", "");
        return -1;
    }
    //3.获取音频信息
    if (avformat_find_stream_info(avformat_context, NULL) < 0) {
        printf("%s\n", "");
        return -1;
    }

//    av_dump_format(srcAVFormatContext, 0, inFilePath, false);

    //音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
    int nb_samples = avformat_context->nb_streams;
    int audio_stream_index = -1;
    int i;
    for (i = 0; i < nb_samples; i++) {
        //根据类型判断是否是音频流
        if (avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    if (audio_stream_index == -1) {
        printf("%s\n", "");
        return -1;
    }

    //4.获取解码器
    //根据索引拿到对应的流,根据流拿到解码器上下文
    audio_avcodec_context = avformat_context->streams[audio_stream_index]->codec;
    //再根据上下文拿到编解码id，通过该id拿到解码器
    AVCodec *audio_decoder_avcodec = avcodec_find_decoder(audio_avcodec_context->codec_id);
    if (audio_decoder_avcodec == NULL) {
        printf("%s\n", "");
        return -1;
    }
    //5.打开解码器
    if (avcodec_open2(audio_avcodec_context, audio_decoder_avcodec, NULL) < 0) {
        printf("%s\n", "");
        return -1;
    }

    //重采样设置选项-----------------------------------------------------------start
    //frame->16bit 44100 PCM 统一音频采样格式与采样率
    //输入的采样格式
    enum AVSampleFormat in_sample_fmt = audio_avcodec_context->sample_fmt;
    //输出的采样格式 16bit PCM
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入的采样率
    int in_sample_rate = audio_avcodec_context->sample_rate;
    //输出的采样率
    int out_sample_rate = 44100;
    //输入的声道布局
    uint64_t in_channel_layout = audio_avcodec_context->channel_layout;
    //输出的声道布局(单声道)
    uint64_t out_channel_layout = AV_CH_LAYOUT_MONO;
    //由声道布局得到声道数
    //获取输入的声道个数
    int in_nb_channels = audio_avcodec_context->channels;
    //int in_nb_channels = av_get_channel_layout_nb_channels(in_channel_layout);
    //获取输出的声道个数
    int out_nb_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    //重采样设置选项-----------------------------------------------------------end

    //打印文件信息
    print_file_audio_info(avformat_context, audio_avcodec_context, in_sample_fmt, audio_stream_index);

#if NEED_TO_DECODER
    SwrContext *audio_swr_context = swr_alloc();
    swr_alloc_set_opts(audio_swr_context, out_channel_layout, out_sample_fmt, out_sample_rate,
                       in_channel_layout, in_sample_fmt, in_sample_rate, 0, NULL);
    swr_init(audio_swr_context);

    //存放将要编码的数据
    AVPacket *avpacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    //存放编码后的数据
    AVFrame *decoded_avframe = av_frame_alloc();
    //存储pcm数据(把解码后的数据经过转化后存在这个变量中)
    uint8_t *out_buffer = (uint8_t *) av_malloc(2 * 44100);

    FILE *out_file = fopen(out_file_path, "wb");
    int ret, get_frame, frame_count = 0;
    //6.一帧一帧读取压缩的音频数据AVPacket
    while (av_read_frame(avformat_context, avpacket) >= 0) {
        if (avpacket->stream_index == audio_stream_index) {
            //解码AVPacket->AVFrame
            ret = avcodec_decode_audio4(audio_avcodec_context, decoded_avframe, &get_frame, avpacket);
            if (ret < 0) {
                printf("%s\n", "");
                break;
            }
            //非0，正在解码
            if (get_frame) {
                ++frame_count;
                swr_convert(audio_swr_context, &out_buffer, 2 * 44100,
                            (const uint8_t **) decoded_avframe->data, decoded_avframe->nb_samples);
                //获取sample的size
                int out_buffer_size = av_samples_get_buffer_size(NULL, out_nb_channels, decoded_avframe->nb_samples,
                                                                 out_sample_fmt, 1);
                //写入文件进行测试
                fwrite(out_buffer, 1, out_buffer_size, out_file);
            }
        }
        av_free_packet(avpacket);
    }
    printf("总共解码%d帧\n", frame_count);

    fclose(out_file);
    av_frame_free(&decoded_avframe);
    av_free(out_buffer);
    swr_free(&audio_swr_context);
#endif
    avcodec_close(audio_avcodec_context);
    avformat_close_input(&avformat_context);

    return 0;
}

/***
 * 分离YUV420P像素数据中的Y、U、V分量
 *
 * simplest_yuv420_split("lena_256x256_yuv420p.yuv",256,256,1);
 *
 * Split Y, U, V planes in YUV420P file.
 * @param url  Location of Input YUV file.
 * @param w    Width of Input YUV file.
 * @param h    Height of Input YUV file.
 * @param num  Number of frames to process.
 * 如果视频帧的宽和高分别为w和h，那么一帧YUV420P像素数据一共占用w*h*3/2 Byte的数据。
 * 其中前w*h Byte存储Y，接着的w*h*1/4 Byte存储U，最后w*h*1/4 Byte存储V。
 * 上述调用函数的代码运行后，将会把一张分辨率为256x256的名称为
 * lena_256x256_yuv420p.yuv的YUV420P格式的像素数据文件分离成为三个文件：
   output_420_y.y：纯Y数据，分辨率为256x256。
   output_420_u.y：纯U数据，分辨率为128x128。
   output_420_v.y：纯V数据，分辨率为128x128。
   注：本文中像素的采样位数一律为8bit。由于1Byte=8bit，所以一个像素的一个分量的采样值占用1Byte。
 */
int simplest_yuv420p_split(char *url, int w, int h, int num) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("/root/视频/tomcat_video/yuv/output_420_y.y", "wb+");
    FILE *fp2 = fopen("/root/视频/tomcat_video/yuv/output_420_u.y", "wb+");
    FILE *fp3 = fopen("/root/视频/tomcat_video/yuv/output_420_v.y", "wb+");

    unsigned char *pic = (unsigned char *) malloc(w * h * 3 / 2);

    // 有多少帧
    for (int i = 0; i < num; i++) {
        // w * h * 3 / 2 = w * h * + w * h / 4 + w * h / 4
        fread(pic, 1, w * h * 3 / 2, fp);
        //y
        fwrite(pic, 1, w * h, fp1);
        //u
        fwrite(pic + w * h, 1, w * h / 4, fp2);
        //v
        fwrite(pic + w * h * 5 / 4, 1, w * h / 4, fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

int simplest_yuv444p_split(char *url, int w, int h, int num) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("/root/视频/tomcat_video/yuv/output_yuv444p_y.y", "wb+");
    FILE *fp2 = fopen("/root/视频/tomcat_video/yuv/output_yuv444p_u.y", "wb+");
    FILE *fp3 = fopen("/root/视频/tomcat_video/yuv/output_yuv444p_v.y", "wb+");

    unsigned char *pic = (unsigned char *) malloc(w * h * 3);

    for (int i = 0; i < num; i++) {
        fread(pic, 1, w * h * 3, fp);
        //Y
        fwrite(pic, 1, w * h, fp1);
        //U
        fwrite(pic + w * h, 1, w * h, fp2);
        //V
        fwrite(pic + w * h * 2, 1, w * h, fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

/***
 分离YUV444P像素数据中的Y、U、V分量
 如果视频帧的宽和高分别为w和h，那么一帧YUV444P像素数据一共占用w*h*3 Byte的数据。
 其中前w*h Byte存储Y，接着的w*h Byte存储U，最后w*h Byte存储V。
 上述调用函数的代码运行后，将会把一张分辨率为256x256的名称为
 lena_256x256_yuv444p.yuv的YUV444P格式的像素数据文件分离成为三个文件：
 output_444_y.y：纯Y数据，分辨率为256x256。
 output_444_u.y：纯U数据，分辨率为256x256。
 output_444_v.y：纯V数据，分辨率为256x256。
 */
int simplest_yuv444_split(char *url, int w, int h, int num) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("/root/视频/output_444_y.y", "wb+");
    FILE *fp2 = fopen("/root/视频/output_444_u.y", "wb+");
    FILE *fp3 = fopen("/root/视频/output_444_v.y", "wb+");

    unsigned char *pic = (unsigned char *) malloc(w * h * 3);

    for (int i = 0; i < num; i++) {
        fread(pic, 1, w * h * 3, fp);
        fwrite(pic, 1, w * h, fp1);
        fwrite(pic + w * h, 1, w * h, fp2);
        fwrite(pic + w * h * 2, 1, w * h, fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

/***
 如果想把YUV格式像素数据变成灰度图像，只需要将U、V分量设置成128即可。
 这是因为U、V是图像中的经过偏置处理的色度分量。色度分量在偏置处理前的
 取值范围是-128至127，这时候的无色对应的是“0”值。经过偏置后色度分量
 取值变成了0至255，因而此时的无色对应的就是128了。上述调用函数的代码运行后，
 将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.yuv的YUV420P
 格式的像素数据文件处理成名称为output_gray.yuv的YUV420P格式的像素数据文件。
 */
int simplest_yuv420_gray(char *url, int w, int h, int num) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("/root/视频/output_gray,yuv", "wb+");

    unsigned char *pic = (unsigned char *) malloc(w * h * 3 / 2);

    for (int i = 0; i < num; i++) {
        fread(pic, 1, w * h * 3 / 2, fp);
        //Gray(把uv方向上的色度设置成128)
        memset(pic + w * h, 128, w * h / 2);
        fwrite(pic, 1, w * h * 3 / 2, fp1);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);

    return 0;
}

int wav2mp3() {
    int read, write;
    FILE *pcm = fopen("/Users/liuchan_xin/Desktop/音视频播放与保存例子/ipcamera.wav", "rb");
    FILE *mp3 = fopen("/Users/liuchan_xin/Desktop/file.mp3", "wb");

    int PCM_SIZE = 640;
    int MP3_SIZE = 8192;

    short int pcm_buffer[PCM_SIZE];
    unsigned char mp3_buffer[MP3_SIZE];

    lame_t lame = lame_init();

    lame_set_num_channels(lame, 1);
    lame_set_in_samplerate(lame, 8000);
    lame_set_brate(lame, 8);
//    lame_set_mode(lame, 3);
    lame_set_quality(lame, 2); /* 2=high 5 = medium 7=low */

    lame_init_params(lame);

    do {
        read = fread(pcm_buffer, sizeof(short int), PCM_SIZE, pcm);

        if (read == 0)
            write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
        else {
            write = lame_encode_buffer(lame,
                                       pcm_buffer, NULL,
                                       read, mp3_buffer, MP3_SIZE);
        }

        fwrite(mp3_buffer, write, 1, mp3);
    } while (read != 0);

    lame_close(lame);
    fclose(mp3);
    fclose(pcm);

    return 0;
}

int audio_recorder(const char *out_file_path) {
    long loops;
    int rc;
    int size;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;
    char *buffer;
    FILE *fd_out;
    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(&handle, "default",
                      SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params,
                                 SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(handle, params, 2);

    /* 11025 bits/second sampling rate (CD quality) */
    val = 11025;
    snd_pcm_hw_params_set_rate_near(handle, params,
                                    &val, &dir);

    /* Set period all_pkts_size to 32 frames. */
    //frames = 32;
    // snd_pcm_hw_params_set_period_size_near(handle,
    //                             params, &frames, &dir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params,
                                      &frames, &dir);
    size = frames * 4; /* 2 bytes/sample, 2 channels */
    buffer = (char *) malloc(size);

    /* We want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params,
                                      &val, &dir);
    loops = 5000000 / val;
    fprintf(stdout, "loops = %d\n", loops);
    fd_out = fopen(out_file_path, "wb+");
    while (loops > 0) {
        loops--;
        rc = snd_pcm_readi(handle, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr,
                    "error from read: %s\n",
                    snd_strerror(rc));
        } else if (rc != (int) frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
        }
        rc = fwrite(buffer, 1, size, fd_out);
        if (rc != size)
            fprintf(stderr,
                    "short write: wrote %d bytes\n", rc);
        fprintf(stdout, "loops = %d\n", loops);
    }
    fclose(fd_out);
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);

    return 0;
}

/************************************************************/

/****函数功能：音视频解码并重采样为规定采样率的WAV文件**************/

/****作者:dreamboy*******************************************/

/****参数：**************************************************/

/****1.outfilename：输出WAV文件路径***************************/

/****2.sample_rate：输出WAV文件的采样率***********************/

/****3.channels：输出WAV文件的通道数量************************/

/****4.inputfilename：输入WAV文件路径************************/

/****说明：当sample_rate或channels为0是表示，

               该参数在输入文件基础上保持不变
			   // 只支持采集点为16位的，即 aCodecCtx->sample_fmt=AV_SAMPLE_FMT_S16
			   // 如果源文件 采样率和通道数本为16000kHz、1，则不进行转换
			   ******************/

int AudioConvertFunc(const char *outfilename, int sample_rate, int channels, const char *inputfilename) {
    AVCodec *aCodec = NULL;
    AVPacket *packet = NULL;
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *aCodecCtx = NULL;
    //ReSampleContext *ResampleCtx = NULL;
    AVFrame *decoded_frame = NULL;
    int datasize;

    unsigned int i;
    int len, ret, buffer_size, count, audio_stream_index = -1, totle_samplenum = 0;

    FILE *outfile = NULL;// *infile;
    //head_pama pt;

    int16_t *audio_buffer = NULL;
    int16_t *resamplebuff = NULL;
    int ResampleChange = 0;
    int ChannelsChange = 0;


    packet = (AVPacket *) malloc(sizeof(AVPacket));
    if (packet == NULL) {
        return -1;
    }
    packet->data = NULL;

    //buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE * 2;
    audio_buffer = (int16_t *) av_malloc(buffer_size);
    if (audio_buffer == NULL) {
        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        return -2;
    }

    av_register_all();

    av_init_packet(packet);

    //pFormatCtx = avformat_alloc_context();
    ret = avformat_open_input(&pFormatCtx, inputfilename, NULL, NULL);

    if (ret < 0) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }

        return 1;
    }

    //ret = av_find_stream_info(pFormatCtx);

    if (ret < 0) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }

        return 2;
    }

    audio_stream_index = -1;
    for (i = 0; i < (signed) pFormatCtx->nb_streams; i++) {

        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream_index < 0) {
            audio_stream_index = i;
            break;
        }
    }

    if (audio_stream_index == -1) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }

        return 3;
    }

    aCodecCtx = pFormatCtx->streams[audio_stream_index]->codec;
    if (aCodecCtx == NULL) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }
        return 4;
    }
    aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
    if (!aCodec) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }
        /*if (aCodecCtx!=NULL)
        {
         avcodec_close(aCodecCtx);
         aCodecCtx=NULL;
        }*/
        return 5;
    }
    //resample init
    if (channels == 0) {
        channels = aCodecCtx->channels;
    }
    if (sample_rate == 0) {
        sample_rate = aCodecCtx->sample_rate;
    }
    //if (aCodecCtx->channels!=channels)
    //{
    // ChannelsChange=1;
    // ResampleChange=1;
    //}
    if (aCodecCtx->sample_rate != sample_rate || aCodecCtx->channels != channels) {
        ResampleChange = 1;
    }
    if (ResampleChange == 1) {
        //ResampleCtx = av_audio_resample_init(channels,aCodecCtx->channels,sample_rate,aCodecCtx->sample_rate,aCodecCtx->sample_fmt,aCodecCtx->sample_fmt,8,10,0,1.0);

        /*ResampleCtx = av_audio_resample_init(channels, aCodecCtx->channels, sample_rate, aCodecCtx->sample_rate,
                                             AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_S16, 16, 10, 0, 1.0);*/
        //if (ResampleCtx == NULL) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }
        /*if (aCodecCtx!=NULL)
        {
         avcodec_close(aCodecCtx);
         aCodecCtx=NULL;
        }*/
        ResampleChange = 0;
        return 6;
//        }
        resamplebuff = (int16_t *) malloc(buffer_size);
        if (resamplebuff == NULL) {
            if (audio_buffer != NULL) {
                av_free(audio_buffer);
                audio_buffer = NULL;
            }

            if (packet->data != NULL) {
                av_free_packet(packet);
                packet->data = NULL;
            }
            if (packet != NULL) {
                free(packet);
                packet = NULL;
            }
            if (pFormatCtx != NULL) {
                //av_close_input_file(pFormatCtx);
                pFormatCtx = NULL;
            }
            /*if (aCodecCtx!=NULL)
            {
             avcodec_close(aCodecCtx);
             aCodecCtx=NULL;
            }*/

            /*if (ResampleChange == 1 && ResampleCtx != NULL) {
                audio_resample_close(ResampleCtx);
                ResampleCtx = NULL;
            }*/
            return 7;
        }
    }
    int sec = 0;// 单位秒
    if (pFormatCtx->duration > 0)// 文件播放时长
    {
        sec = pFormatCtx->duration / 1000000;
    }

    datasize = sec * sample_rate;
    if (avcodec_open2(aCodecCtx, aCodec, NULL) < 0) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }
        if (aCodecCtx != NULL) {
            avcodec_close(aCodecCtx);
            aCodecCtx = NULL;
        }

        /*if (ResampleChange == 1 && ResampleCtx != NULL && resamplebuff != NULL) {
            audio_resample_close(ResampleCtx);
            ResampleCtx = NULL;
            free(resamplebuff);
            resamplebuff = NULL;
        }*/
        ResampleChange = 0;
        return 8;
    }

    //pt.bits = 16;
    //pt.channels = channels;
    //pt.rate = sample_rate;

    outfile = fopen(outfilename, "wb");
    if (!outfile) {
        if (audio_buffer != NULL) {
            av_free(audio_buffer);
            audio_buffer = NULL;
        }

        if (packet->data != NULL) {
            av_free_packet(packet);
            packet->data = NULL;
        }
        if (packet != NULL) {
            free(packet);
            packet = NULL;
        }
        if (pFormatCtx != NULL) {
            //av_close_input_file(pFormatCtx);
            pFormatCtx = NULL;
        }
        if (aCodecCtx != NULL) {
            avcodec_close(aCodecCtx);
            aCodecCtx = NULL;
        }

        /*if (ResampleChange == 1 && ResampleCtx != NULL && resamplebuff != NULL) {
            audio_resample_close(ResampleCtx);
            ResampleCtx = NULL;
            free(resamplebuff);
            resamplebuff = NULL;
        }*/
        ResampleChange = 0;
        return 9;
    }

    fseek(outfile, 44, SEEK_SET);

    while (av_read_frame(pFormatCtx, packet) >= 0) {
        //CheckMessageQueue();
        if (packet->stream_index == audio_stream_index) {
            //while(packet->size > 0)
            //{
            //buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE * 100;
            //len = avcodec_decode_audio4(aCodecCtx, audio_buffer, &buffer_size, packet);

            if (len < 0) {
                break;
            }

            if (buffer_size > 0) {
                //resample
                if (ResampleChange == 1) {
                    int samples = buffer_size / ((aCodecCtx->channels) * 2);
                    int resamplenum = 0;
                    /*resamplenum = audio_resample(ResampleCtx,
                                                 resamplebuff,
                                                 audio_buffer,
                                                 samples);*/
                    count = fwrite(resamplebuff, 2 * channels, resamplenum, outfile);
                } else {
                    count = fwrite(audio_buffer, 2 * aCodecCtx->channels, buffer_size / ((aCodecCtx->channels) * 2),
                                   outfile);
                }
                totle_samplenum += count;
            }
            if (packet->data != NULL) {
                av_free_packet(packet);
                packet->data = NULL;
            }
            //packet->size -= len;
            //packet->data += len;
            //}
            if (datasize != 0 && totle_samplenum >= datasize) {
                break;
            }
        }
    }

    fseek(outfile, 0, SEEK_SET);
//wav_write_header(outfile, pt, totle_samplenum);

    if (outfile != NULL) {
        fclose(outfile);
        outfile = NULL;
    }

    if (audio_buffer != NULL) {
        av_free(audio_buffer);
        audio_buffer = NULL;
    }

    if (aCodecCtx != NULL) {
        avcodec_close(aCodecCtx);
        aCodecCtx = NULL;
    }

    if (packet != NULL) {
        free(packet);//
        packet = NULL;
    }

    if (pFormatCtx != NULL) {
        //av_close_input_file(pFormatCtx);
        pFormatCtx = NULL;
    }

    if (ResampleChange == 1) {
        if (resamplebuff != NULL) {
            free(resamplebuff);
            resamplebuff = NULL;
        }
        /*if (ResampleCtx != NULL) {
            audio_resample_close(ResampleCtx);
            ResampleCtx = NULL;
        }*/
    }
    if (totle_samplenum <= sample_rate * 5) {
        return 10;
    }
    return 0;
}

/*int main(int argc, char **argv) {
    AudioConvertFunc("test_48k_16.wav", 48000, 1, "test_8k_16.wav");
    return 0;
}*/

int test(float) {
    return 0;
}


#endif //MYSTUDY_STUDY_FFMPEG