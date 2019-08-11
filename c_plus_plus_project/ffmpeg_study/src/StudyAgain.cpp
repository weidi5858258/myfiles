//
// Created by root on 19-8-2.
//

#ifndef MYSTUDY_FFMPEG_STUDYAGAIN
#define MYSTUDY_FFMPEG_STUDYAGAIN

#include "../include/MyHead.h"

/***
 int av_image_get_buffer_size(enum AVPixelFormat pix_fmt, int width, int height, int align);
 通过指定像素格式、图像宽、图像高来计算所需的内存大小
 参数align:
 此参数是设定内存对齐的对齐数，也就是按多大的字节进行内存对齐。
 比如设置为1，表示按1字节对齐，那么得到的结果就是与实际的内存大小一样。
 再比如设置为4，表示按4字节对齐。也就是内存的起始地址必须是4的整倍数。
 int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
                               enum AVSampleFormat sample_fmt, int align);
 */

typedef struct AVPacketQueue {
    AVPacketList *firstAVPacketList = NULL;
    AVPacketList *lastAVPacketList = NULL;
    // 有多少个AVPacketList
    int allAVPacketsCount = 0;
    // 所有AVPacket占用的空间大小
    int64_t allAVPacketsSize = 0;
};

// 子类都要用到的部分
struct Wrapper {
    AVFormatContext *avFormatContext = NULL;
    AVCodecContext *avCodecContext = NULL;
    // 解码器
    AVCodec *decoderAVCodec = NULL;
    // 编码器
    AVCodec *encoderAVCodec = NULL;
    // 存储压缩数据(视频对应H.264等码流数据,音频对应AAC/MP3等码流数据)
    // AVPacket *avPacket = NULL;
    // 存储非压缩数据(视频对应RGB/YUV像素数据,音频对应PCM采样数据)
    AVFrame *srcAVFrame = NULL;
    // 用于格式转换(音频用不到)
    AVFrame *dstAVFrame = NULL;
    // 有些东西需要通过它去得到
    AVCodecParameters *avCodecParameters = NULL;
    int streamIndex = -1;
    // 读取了多少个AVPacket
    int readFramesCount = 0;
    // 处理了多少个AVPacket
    int handleFramesCount = 0;
    // 存储原始数据
    unsigned char *outBuffer1 = NULL;
    unsigned char *outBuffer2 = NULL;
    unsigned char *outBuffer3 = NULL;
    size_t outBufferSize = 0;
    // 视频使用到sws_scale函数时需要定义这些变量,音频也要用到
    unsigned char *srcData[4] = {NULL}, dstData[4] = {NULL};

    struct AVPacketQueue *queue1 = NULL;
    struct AVPacketQueue *queue2 = NULL;
    bool isHandlingForQueue1 = false;
    bool isHandlingForQueue2 = false;
};

struct AudioWrapper {
    struct Wrapper father;
    SwrContext *swrContext = NULL;
    // 从音频源或视频源中得到
    // 采样率
    int srcSampleRate = 0;
    int dstSampleRate = 44100;
    // 声道数
    int srcNbChannels = 0;
    // 由dstChannelLayout去获到
    int dstNbChannels = 0;
    int srcNbSamples = 0;
    int dstNbSamples = 0;
    // 由srcNbChannels能得到srcChannelLayout,也能由srcChannelLayout得到srcNbChannels
    int srcChannelLayout = 0;
    int dstChannelLayout = 0;
    // 双声道输出
    // int dstChannelLayout = AV_CH_LAYOUT_STEREO;
    // 从音频源或视频源中得到(采样格式)
    enum AVSampleFormat srcAVSampleFormat = AV_SAMPLE_FMT_NONE;
    // 输出的采样格式16bit PCM
    enum AVSampleFormat dstAVSampleFormat = AV_SAMPLE_FMT_S16;

    // 要播放的数据存在于playBuffer中
    DECLARE_ALIGNED(16, unsigned char, playBuffer)[MAX_AUDIO_FRAME_SIZE * 4];

    //解码一次得到的数据量
    unsigned int decodedDataSize = 0;
    //用于标记已处理过的数据位置(针对audio_decoded_data_size的位置)
    unsigned int decodedDataSizeIndex = 0;

    // SDL
    SDL_AudioSpec srcSDLAudioSpec;
    SDL_AudioSpec dstSDLAudioSpec;
};

struct VideoWrapper {
    struct Wrapper father;
    SwsContext *swsContext = NULL;
    // 从视频源中得到
    enum AVPixelFormat srcAVPixelFormat = AV_PIX_FMT_NONE;
    // 从原来的像素格式转换为想要的视频格式(可能应用于不需要播放视频的场景)
    // 播放时dstAVPixelFormat必须跟srcAVPixelFormat的值一样,不然画面有问题
    enum AVPixelFormat dstAVPixelFormat = AV_PIX_FMT_RGB24;
    // 从视频源中得到的宽高
    int srcWidth = 0, srcHeight = 0;
    size_t srcArea = 0;
    // 想要播放的窗口大小,可以直接使用srcWidth和srcHeight
    int dstWidth = 720, dstHeight = 360;
    size_t dstArea = 0;
    // 使用到sws_scale函数时需要定义这些变量
    int srcLineSize[4] = {0}, dstLineSize[4] = {0};

    // SDL
};

struct AudioWrapper audioWrapper;
struct VideoWrapper videoWrapper;

pthread_mutex_t lockMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t lockCondition = PTHREAD_COND_INITIALIZER;

//格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
// AVFormatContext相当于Android的MediaExtractor,保存了音视频的Format信息(MediaFormat)
//AVFormatContext *avFormatContext = NULL;

// 自己电脑上的文件路径
//char *inFilePath2 = "/root/视频/tomcat_video/流浪的地球.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/疾速备战.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/shape_of_my_heart.mp4";
//char *inFilePath2 = "/root/音乐/KuGou/蔡国权-不装饰你的梦.mp3";
char *inFilePath2 = "/root/音乐/KuGou/冷漠、云菲菲 - 伤心城市.mp3";
// 公司电脑上的文件路径
//char *inFilePath2 = "http://ok.xzokzyzy.com/20190606/1940_094739d9/%E6%80%92%E6%B5%B7%E6%BD%9C%E6%B2%99&%E7%A7%A6%E5%B2%AD%E7%A5%9E%E6%A0%91%E7%AC%AC01%E9%9B%86.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/AC3Plus_mountainbike-cyberlink_1920_1080.mp4";
//char *inFilePath2 = "/root/音乐/alexander_music/容易受伤的女人.mp3";

///////////////////////////SDL2///////////////////////////

struct SDL_Rect sdlRect;
SDL_Window *sdlWindow = NULL;
SDL_Renderer *sdlRenderer = NULL;
SDL_Texture *sdlTexture = NULL;
SDL_Thread *sdlThread = NULL;
//struct SDL_AudioSpec sdlAudioSpec;
SDL_Event sdlEvent;

//用于线程
int threadPauseFlag = 0;
int threadExitFlag = 0;

#define MAX_AUDIO_QUEUE_SIZE (1 * 1024 * 1024)
#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

static uint32_t audio_len = 0; // 音频数据缓冲区中未读数据剩余的长度
static unsigned char *audio_pos = NULL; // 音频缓冲区中读取的位置

////////////////////////////////////////////////////////////////////////////////////

static int getAVPacketFromQueue(struct AVPacketQueue *packet_queue, AVPacket *avpacket);

int audioDecodeFrame() {
    int ret, get_nb_samples_per_channel;
    int got_frame_ptr = 0;
    int64_t get_ch_layout_from_decoded_avframe;
    // 压缩数据
    AVPacket *avPacket = av_packet_alloc();
    unsigned char *audio_pkt_data = NULL;
    int audio_pkt_size = 0;
    // decodedAVFrame为解码后的数据
    AVFrame *decodedAVFrame = audioWrapper.father.srcAVFrame;

    for (;;) {
        while (audio_pkt_size > 0) {
            av_frame_unref(decodedAVFrame);
            /***
             当AVPacket中装得是音频时,有可能一个AVPacket中有多个AVFrame,
             而某些解码器只会解出第一个AVFrame,这种情况我们必须循环解码出后续AVFrame.
             */
            ret = avcodec_decode_audio4(audioWrapper.father.avCodecContext,
                                        decodedAVFrame,
                                        &got_frame_ptr,
                                        avPacket);
            if (ret < 0) {
                fprintf(stderr, "ret = %d\n", ret);
                // error, skip the frame
                audio_pkt_size = 0;
                break;
            }

            audio_pkt_data += ret;
            audio_pkt_size -= ret;

            if (!got_frame_ptr) {
                //fprintf(stderr, "got_frame_ptr = %d\n", got_frame_ptr);
                continue;
            }

            // 执行到这里我们得到了一个AVFrame
            // 得到这个AVFrame的声音布局,比如立体声
            get_ch_layout_from_decoded_avframe =
                    (decodedAVFrame->channel_layout != 0
                     && decodedAVFrame->channels ==
                        av_get_channel_layout_nb_channels(decodedAVFrame->channel_layout))
                    ?
                    decodedAVFrame->channel_layout
                    :
                    av_get_default_channel_layout(decodedAVFrame->channels);

            if (audioWrapper.srcSampleRate != decodedAVFrame->sample_rate) {
                fprintf(stdout, "SampleRate变了 srcSampleRate: %d now: %d\n",
                        audioWrapper.srcSampleRate, decodedAVFrame->sample_rate);
            }
            if (audioWrapper.srcAVSampleFormat != decodedAVFrame->format) {
                fprintf(stdout, "AVSampleFormat变了 srcAVSampleFormat: %d now: %d\n",
                        audioWrapper.srcAVSampleFormat, decodedAVFrame->format);
            }
            if (audioWrapper.srcChannelLayout != get_ch_layout_from_decoded_avframe) {
                fprintf(stdout, "ChannelLayout变了 srcChannelLayout: %d now: %d\n",
                        audioWrapper.srcChannelLayout, get_ch_layout_from_decoded_avframe);
            }

            /***
             接下来判断我们之前设置SDL时设置的声音格式(AV_SAMPLE_FMT_S16),声道布局,
             采样频率,每个AVFrame的每个声道采样数与
             得到的该AVFrame分别是否相同,如有任意不同,我们就需要swr_convert该AVFrame,
             然后才能符合之前设置好的SDL的需要,才能播放.
             */
            if (audioWrapper.srcSampleRate != decodedAVFrame->sample_rate
                || audioWrapper.srcAVSampleFormat != decodedAVFrame->format
                || audioWrapper.srcChannelLayout != get_ch_layout_from_decoded_avframe) {
                printf("---------------------------------\n");
                printf("nowSampleRate       : %d\n", decodedAVFrame->sample_rate);
                printf("nowAVSampleFormat   : %d\n", decodedAVFrame->format);
                printf("nowChannelLayout    : %d\n", get_ch_layout_from_decoded_avframe);
                printf("nowNbChannels       : %d\n", decodedAVFrame->channels);
                printf("nowNbSamples        : %d\n", decodedAVFrame->nb_samples);
                printf("---------------------------------\n");

                if (audioWrapper.swrContext) {
                    swr_free(&audioWrapper.swrContext);
                }
                fprintf(stdout, "audio_state->audioSwrContext swr_alloc_set_opts.\n");
                audioWrapper.swrContext = swr_alloc();
                swr_alloc_set_opts(audioWrapper.swrContext,
                                   audioWrapper.dstChannelLayout,
                                   audioWrapper.dstAVSampleFormat,
                                   audioWrapper.dstSampleRate,
                                   get_ch_layout_from_decoded_avframe,
                                   (enum AVSampleFormat) decodedAVFrame->format,
                                   decodedAVFrame->sample_rate,
                                   0, NULL);
                if (!audioWrapper.swrContext || swr_init(audioWrapper.swrContext) < 0) {
                    fprintf(stderr, "swr_init() failed\n");
                    break;
                } else {
                    fprintf(stdout, "audio_state->audioSwrContext is created.\n");
                }

                audioWrapper.srcSampleRate = decodedAVFrame->sample_rate;
                audioWrapper.srcNbChannels = decodedAVFrame->channels;
                audioWrapper.srcAVSampleFormat = (enum AVSampleFormat) decodedAVFrame->format;
                audioWrapper.srcNbSamples = decodedAVFrame->nb_samples;
                audioWrapper.srcChannelLayout = get_ch_layout_from_decoded_avframe;
            }

            /***
             转换该AVFrame到设置好的SDL需要的样子,有些旧的代码示例最主要就是少了这一部分,
             往往一些音频能播,一些不能播,这就是原因,比如有些源文件音频恰巧是AV_SAMPLE_FMT_S16的.
             swr_convert 返回的是转换后每个声道(channel)的采样数
             */
            unsigned char *out[] = {audioWrapper.playBuffer};
            int out_count = sizeof(audioWrapper.playBuffer)
                            / audioWrapper.dstNbChannels
                            / av_get_bytes_per_sample(audioWrapper.dstAVSampleFormat);
            const unsigned char **in = (const unsigned char **) decodedAVFrame->extended_data;
            int in_count = decodedAVFrame->nb_samples;
            // 转换后的数据存在audioWrapper.outBuffer中,也就是要播放的数据
            // 大小为decodedAVFrame->nb_samples
            get_nb_samples_per_channel = swr_convert(audioWrapper.swrContext,
                                                     out,
                                                     out_count,
                                                     in,
                                                     in_count);
            if (get_nb_samples_per_channel < 0) {
                fprintf(stderr, "swr_convert() failed\n");
                break;
            }

            // 声道数 x 每个声道采样数 x 每个样本字节数
            // We have data, return it and come back for more later
            return audioWrapper.dstNbChannels
                   * get_nb_samples_per_channel
                   * av_get_bytes_per_sample(audioWrapper.dstAVSampleFormat);
        }//while end

        if (avPacket->data) {
            av_free_packet(avPacket);
        }
        memset(avPacket, 0, sizeof(*avPacket));
        if (audioWrapper.father.isHandlingForQueue1
            && audioWrapper.father.queue1->allAVPacketsCount > 0) {
            audioWrapper.father.handleFramesCount++;
            getAVPacketFromQueue(audioWrapper.father.queue1, avPacket);
            if (audioWrapper.father.queue1->allAVPacketsCount == 0) {
                memset(audioWrapper.father.queue1, 0, sizeof(struct AVPacketQueue));
                audioWrapper.father.isHandlingForQueue1 = false;
                audioWrapper.father.isHandlingForQueue2 = true;
                fprintf(stdout, "audioDecodeFrame() audio Queue2\n");
                fprintf(stdout, "audioDecodeFrame() audio allAVPacketsCount2: %d\n",
                        audioWrapper.father.queue2->allAVPacketsCount);
            }
        } else if (audioWrapper.father.isHandlingForQueue2
                   && audioWrapper.father.queue2->allAVPacketsCount > 0) {
            audioWrapper.father.handleFramesCount++;
            getAVPacketFromQueue(audioWrapper.father.queue2, avPacket);
            if (audioWrapper.father.queue2->allAVPacketsCount == 0) {
                memset(audioWrapper.father.queue2, 0, sizeof(struct AVPacketQueue));
                audioWrapper.father.isHandlingForQueue1 = true;
                audioWrapper.father.isHandlingForQueue2 = false;
                fprintf(stdout, "audioDecodeFrame() audio Queue1\n");
                fprintf(stdout, "audioDecodeFrame() audio allAVPacketsCount1: %d\n",
                        audioWrapper.father.queue1->allAVPacketsCount);
            }
        } else {
            audioWrapper.father.isHandlingForQueue1 = false;
            audioWrapper.father.isHandlingForQueue2 = false;
            break;
        }

        audio_pkt_data = avPacket->data;
        audio_pkt_size = avPacket->size;
    }//for end
}

// SDL提供的回调函数,数据只有往这个回调函数的stream中填充数据才能发出声音
void sdlAudioCallback(void *userdata, uint8_t *sdl_need_stream_data, int sdl_max_handle_data_size) {
    int max_handle_data_size, audio_decoded_data_size;

    if (audioWrapper.father.queue1->allAVPacketsCount == 0
        && audioWrapper.father.queue2->allAVPacketsCount == 0
        /*&& !audioWrapper.father.isHandlingForQueue1
        && !audioWrapper.father.isHandlingForQueue2*/) {
        // 唤醒线程
        pthread_mutex_lock(&lockMutex);
        fprintf(stdout, "sdlAudioCallback() pthread_cond_signal() return\n");
        pthread_cond_signal(&lockCondition);
        pthread_mutex_unlock(&lockMutex);
        return;
    }

    while (sdl_max_handle_data_size > 0) {
        if (audioWrapper.decodedDataSizeIndex >= audioWrapper.decodedDataSize) {
            audio_decoded_data_size = audioDecodeFrame();
            //fprintf(stdout, "audio_decoded_data_size = %d\n", audio_decoded_data_size);

            if (audio_decoded_data_size < 0) {
                /* silence */
                audioWrapper.decodedDataSize = 1024;
                memset(audioWrapper.playBuffer, 0, audioWrapper.decodedDataSize);
            } else {
                audioWrapper.decodedDataSize = audio_decoded_data_size;
            }
            //fprintf(stdout, "audio_callback: audio_decoded_data_size = %d\n", audio_decoded_data_size);
            //fprintf(stdout, "audio_callback: audio_state->audio_decoded_data_size = %d\n", audio_state->audio_decoded_data_size);
            audioWrapper.decodedDataSizeIndex = 0;
        }

        max_handle_data_size = audioWrapper.decodedDataSize - audioWrapper.decodedDataSizeIndex;
        if (max_handle_data_size > sdl_max_handle_data_size) {
            max_handle_data_size = sdl_max_handle_data_size;
        }

        memcpy(sdl_need_stream_data,
               (unsigned char *) audioWrapper.playBuffer + audioWrapper.decodedDataSizeIndex,
               max_handle_data_size);
        sdl_max_handle_data_size -= max_handle_data_size;
        sdl_need_stream_data += max_handle_data_size;
        audioWrapper.decodedDataSizeIndex += max_handle_data_size;
        //fprintf(stdout, "audio_callback: audio_state->audio_decoded_data_size_index = %d\n", audio_state->audio_decoded_data_size_index);
    }//while end
}

// 已经不需要调用了
void initAV() {
    avcodec_register_all();
    // 注册复用器和编解码器，所有的使用ffmpeg，首先必须调用这个函数
    av_register_all();
    // 用于从网络接收数据，如果不是网络接收数据，可不用（如本例可不用）
    avformat_network_init();
    // 注册设备的函数，如用获取摄像头数据或音频等，需要此函数先注册
    // avdevice_register_all();
}

int initSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("SDL_Init() failed: %s\n", SDL_GetError());
        return -1;
    }
}

int initAudioSDL() {
    printf("%s\n", "initAudioSDL() start");
    // B2. 打开音频设备并创建音频处理线程
    // B2.1 打开音频设备，获取SDL设备支持的音频参数srcSDLAudioSpec(期望的参数是dstSDLAudioSpec，实际得到srcSDLAudioSpec)
    // 1) SDL提供两种使音频设备取得音频数据方法：
    //    a. push，SDL以特定的频率调用回调函数，在回调函数中取得音频数据
    //    b. pull，用户程序以特定的频率调用SDL_QueueAudio()，向音频设备提供数据。此种情况dstSDLAudioSpec.callback=NULL
    // 2) 音频设备打开后播放静音，不启动回调，调用SDL_PauseAudio(0)后启动回调，开始正常播放音频
    // 第一步
    // 采样率
    audioWrapper.dstSDLAudioSpec.freq = audioWrapper.srcSampleRate;
    // 声道数
    audioWrapper.dstSDLAudioSpec.channels = audioWrapper.dstNbChannels;
    // S表带符号，16是采样深度，SYS表采用系统字节序
    audioWrapper.dstSDLAudioSpec.format = AUDIO_S16SYS;
    // 静音值
    audioWrapper.dstSDLAudioSpec.silence = 0;
    // SDL声音缓冲区尺寸，单位是单声道采样点尺寸x通道数
    // audioWrapper.dstSDLAudioSpec.samples = audioWrapper.dstNbSamples;
    audioWrapper.dstSDLAudioSpec.samples = 1024;
    // 回调函数，若为NULL，则应使用SDL_QueueAudio()机制
    audioWrapper.dstSDLAudioSpec.callback = sdlAudioCallback;
    // 提供给回调函数的参数
    audioWrapper.dstSDLAudioSpec.userdata = &audioWrapper;

    const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
    // 第二步
    while (SDL_OpenAudio(&audioWrapper.dstSDLAudioSpec, &audioWrapper.srcSDLAudioSpec) < 0) {
        fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", audioWrapper.dstNbChannels, SDL_GetError());
        audioWrapper.dstSDLAudioSpec.channels = next_nb_channels[FFMIN(7, audioWrapper.dstSDLAudioSpec.channels)];
        if (!audioWrapper.dstSDLAudioSpec.channels) {
            fprintf(stderr, "No more channel combinations to tyu, audio open failed\n");
            return -1;
        }
        audioWrapper.dstChannelLayout = av_get_default_channel_layout(audioWrapper.dstNbChannels);
        printf("dstChannelLayout    : %d\n", audioWrapper.dstChannelLayout);
    }
    if (audioWrapper.srcSDLAudioSpec.format != AUDIO_S16SYS) {
        fprintf(stderr, "SDL advised audio format %d is not supported!\n", audioWrapper.srcSDLAudioSpec.format);
        return -1;
    }
    if (audioWrapper.srcSDLAudioSpec.channels != audioWrapper.dstSDLAudioSpec.channels) {
        audioWrapper.dstChannelLayout = av_get_default_channel_layout(audioWrapper.srcSDLAudioSpec.channels);
        if (!audioWrapper.dstChannelLayout) {
            fprintf(stderr, "SDL advised channel count %d is not supported!\n", audioWrapper.srcSDLAudioSpec.channels);
            return -1;
        }
    }

    fprintf(stdout, "%d: srcSDLAudioSpec.freq    : %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.freq);
    fprintf(stdout, "%d: srcSDLAudioSpec.channels: %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.channels);
    fprintf(stdout, "%d: srcSDLAudioSpec.format  : %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.format);
    fprintf(stdout, "%d: srcSDLAudioSpec.samples : %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.samples);

    fprintf(stdout, "%d: dstSDLAudioSpec.freq    : %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.freq);
    fprintf(stdout, "%d: dstSDLAudioSpec.channels: %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.channels);
    fprintf(stdout, "%d: dstSDLAudioSpec.format  : %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.format);
    fprintf(stdout, "%d: dstSDLAudioSpec.samples : %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.samples);

    audioWrapper.srcAVSampleFormat = audioWrapper.dstAVSampleFormat = AV_SAMPLE_FMT_S16;
    audioWrapper.srcSampleRate = audioWrapper.dstSampleRate = audioWrapper.srcSDLAudioSpec.freq;
    audioWrapper.srcChannelLayout = audioWrapper.dstChannelLayout;
    audioWrapper.srcNbChannels = audioWrapper.dstNbChannels = audioWrapper.srcSDLAudioSpec.channels;

    audioWrapper.father.avFormatContext->streams[audioWrapper.father.streamIndex]->discard = AVDISCARD_DEFAULT;

    // 第三步
    SDL_PauseAudio(0);

    printf("%s\n", "initAudioSDL() end");
    return 0;
}

int initVideoSDL() {
    printf("%s\n", "initVideoSDL() start");
    if (videoWrapper.srcWidth == 0 || videoWrapper.srcHeight == 0) {
        printf("%s\n", "没有设置srcWidth和srcHeight的值");
        return -1;
    }

    //SDL 2.0 Support for multiple windows
    sdlWindow = SDL_CreateWindow(inFilePath2,
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 videoWrapper.dstWidth, videoWrapper.dstHeight,
                                 SDL_WINDOW_OPENGL);
    if (sdlWindow == NULL) {
        printf("SDL_CreateWindow() failed: %s\n", SDL_GetError());
        return -1;
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    if (sdlRenderer == NULL) {
        printf("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
        return -1;
    }

    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    sdlTexture = SDL_CreateTexture(sdlRenderer,
                                   SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                   videoWrapper.srcWidth, videoWrapper.srcHeight);
    if (sdlTexture == NULL) {
        printf("SDL_CreateTexture() failed: %s\n", SDL_GetError());
        return -1;
    }

    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = videoWrapper.srcWidth;
    sdlRect.h = videoWrapper.srcHeight;

    /*sdlThread = SDL_CreateThread(pushEventThread, NULL, NULL);
    if (sdlThread == NULL) {
        printf("SDL_CreateThread() failed: %s\n", SDL_GetError());
        return -1;
    }*/
    printf("%s\n", "initVideoSDL() end");
    return 0;
}

int openAndFindAVFormatContextForAudio() {
    // AVFormatContext初始化，里面设置结构体的一些默认信息
    // 相当于Java中创建对象
    audioWrapper.father.avFormatContext = avformat_alloc_context();
    if (audioWrapper.father.avFormatContext == NULL) {
        return -1;
    }
    // 获取基本的文件信息
    if (avformat_open_input(&audioWrapper.father.avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open audio input stream.\n");
        return -1;
    }
    // 文件中的流信息
    if (avformat_find_stream_info(audioWrapper.father.avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    return 0;
}

int openAndFindAVFormatContextForVideo() {
    videoWrapper.father.avFormatContext = avformat_alloc_context();
    if (videoWrapper.father.avFormatContext == NULL) {
        printf("avFormatContext is NULL.\n");
        return -1;
    }
    if (avformat_open_input(&videoWrapper.father.avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open video input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(videoWrapper.father.avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    return 0;
}

/***
 找音视频流轨道
 */
int findStreamIndexForAudio() {
    if (audioWrapper.father.avFormatContext == NULL) {
        return -1;
    }
    // audio stream index
    int streams = audioWrapper.father.avFormatContext->nb_streams;
    for (int i = 0; i < streams; i++) {
        // 得到AVCodecParameters
        audioWrapper.father.avCodecParameters =
                audioWrapper.father.avFormatContext->streams[i]->codecpar;
        if (audioWrapper.father.avCodecParameters != NULL) {
            AVMediaType mediaType = audioWrapper.father.avCodecParameters->codec_type;
            if (mediaType == AVMEDIA_TYPE_AUDIO) {
                audioWrapper.father.streamIndex = i;
                break;
            }
        }
    }

    if (audioWrapper.father.streamIndex == -1) {
        printf("Didn't find audio stream.\n");
        return -1;
    } else {
        printf("audioStreamIndex: %d\n", audioWrapper.father.streamIndex);
        return 0;
    }
}

int findStreamIndexForVideo() {
    if (videoWrapper.father.avFormatContext == NULL) {
        return -1;
    }
    // video stream index
    int streams = videoWrapper.father.avFormatContext->nb_streams;
    for (int i = 0; i < streams; i++) {
        // 得到AVCodecParameters
        videoWrapper.father.avCodecParameters =
                videoWrapper.father.avFormatContext->streams[i]->codecpar;
        if (videoWrapper.father.avCodecParameters != NULL) {
            AVMediaType mediaType = videoWrapper.father.avCodecParameters->codec_type;
            if (mediaType == AVMEDIA_TYPE_VIDEO) {
                videoWrapper.father.streamIndex = i;
                break;
            }
        }
    }

    if (videoWrapper.father.streamIndex == -1) {
        printf("Didn't find video stream.\n");
        return -1;
    } else {
        printf("videoStreamIndex: %d\n", videoWrapper.father.streamIndex);
        return 0;
    }
}

int findAndOpenAVCodecForAudio() {
    if (audioWrapper.father.avCodecParameters == NULL
        || audioWrapper.father.streamIndex == -1) {
        return -1;
    }
    // audio
    if (audioWrapper.father.streamIndex != -1) {
        // 获取音频解码器
        // 先通过AVCodecParameters找到AVCodec
        audioWrapper.father.decoderAVCodec = avcodec_find_decoder(audioWrapper.father.avCodecParameters->codec_id);
        if (audioWrapper.father.decoderAVCodec != NULL) {
            // 获取解码器上下文
            // 再通过AVCodec得到AVCodecContext
            audioWrapper.father.avCodecContext = avcodec_alloc_context3(audioWrapper.father.decoderAVCodec);
            if (audioWrapper.father.avCodecContext != NULL) {
                // 关联操作
                if (avcodec_parameters_to_context(
                        audioWrapper.father.avCodecContext, audioWrapper.father.avCodecParameters) < 0) {
                    return -1;
                } else {
                    // 打开AVCodec
                    if (avcodec_open2(
                            audioWrapper.father.avCodecContext, audioWrapper.father.decoderAVCodec, NULL) != 0) {
                        printf("Could not open audio codec.\n");
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}

int findAndOpenAVCodecForVideo() {
    if (videoWrapper.father.avCodecParameters == NULL
        || videoWrapper.father.streamIndex == -1) {
        return -1;
    }
    // video
    if (videoWrapper.father.streamIndex != -1) {
        videoWrapper.father.decoderAVCodec = avcodec_find_decoder(videoWrapper.father.avCodecParameters->codec_id);
        if (videoWrapper.father.decoderAVCodec != NULL) {
            videoWrapper.father.avCodecContext = avcodec_alloc_context3(videoWrapper.father.decoderAVCodec);
            if (videoWrapper.father.avCodecContext != NULL) {
                if (avcodec_parameters_to_context(
                        videoWrapper.father.avCodecContext, videoWrapper.father.avCodecParameters) < 0) {
                    return -1;
                } else {
                    if (avcodec_open2(
                            videoWrapper.father.avCodecContext, videoWrapper.father.decoderAVCodec, NULL) != 0) {
                        printf("Could not open video codec.\n");
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}

int createSwrContent() {
    // src
    audioWrapper.srcSampleRate = audioWrapper.father.avCodecContext->sample_rate;
    audioWrapper.srcNbSamples = audioWrapper.father.avCodecContext->frame_size;
    audioWrapper.srcNbChannels = audioWrapper.father.avCodecContext->channels;
    audioWrapper.srcChannelLayout = audioWrapper.father.avCodecContext->channel_layout;
    audioWrapper.srcAVSampleFormat = audioWrapper.father.avCodecContext->sample_fmt;
    printf("---------------------------------\n");
    printf("srcSampleRate       : %d\n", audioWrapper.srcSampleRate);
    printf("srcNbSamples        : %d\n", audioWrapper.srcNbSamples);
    printf("srcNbChannels       : %d\n", audioWrapper.srcNbChannels);
    printf("srcAVSampleFormat   : %d\n", audioWrapper.srcAVSampleFormat);
    printf("srcChannelLayout1   : %d\n", audioWrapper.srcChannelLayout);
    // 有些视频从源视频中得到的channel_layout与使用函数得到的channel_layout结果是一样的
    // 但是还是要使用函数得到的channel_layout为好
    audioWrapper.srcChannelLayout = av_get_default_channel_layout(audioWrapper.srcNbChannels);
    printf("srcChannelLayout2   : %d\n", audioWrapper.srcChannelLayout);
    printf("---------------------------------\n");
    if (audioWrapper.srcNbSamples <= 0) {
        audioWrapper.srcNbSamples = 1024;
    }
    // dst
    audioWrapper.dstSampleRate = audioWrapper.srcSampleRate;
    audioWrapper.dstNbSamples = audioWrapper.srcNbSamples;
    audioWrapper.dstNbChannels = audioWrapper.srcNbChannels;
    if (!audioWrapper.dstChannelLayout
        || audioWrapper.dstChannelLayout != av_get_channel_layout_nb_channels(audioWrapper.dstChannelLayout)) {
        audioWrapper.dstChannelLayout = av_get_default_channel_layout(audioWrapper.dstNbChannels);
        printf("dstChannelLayout1   : %d\n", audioWrapper.dstChannelLayout);
        // why?
        audioWrapper.dstChannelLayout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
        printf("dstChannelLayout2   : %d\n", audioWrapper.dstChannelLayout);
    }
    audioWrapper.dstNbChannels = av_get_channel_layout_nb_channels(audioWrapper.dstChannelLayout);

    printf("dstSampleRate       : %d\n", audioWrapper.dstSampleRate);
    printf("dstNbSamples        : %d\n", audioWrapper.dstNbSamples);
    printf("dstNbChannels       : %d\n", audioWrapper.dstNbChannels);
    printf("dstAVSampleFormat   : %d\n", audioWrapper.dstAVSampleFormat);
    printf("---------------------------------\n");

    // avPacket ---> srcAVFrame ---> dstAVFrame ---> 播放声音
    audioWrapper.father.srcAVFrame = av_frame_alloc();
    audioWrapper.father.dstAVFrame = av_frame_alloc();

    /*int samplesGetBufferSize = av_samples_get_buffer_size(
            audioWrapper.father.dstAVFrame->linesize,
            audioWrapper.dstNbChannels, audioWrapper.dstNbSamples, audioWrapper.dstAVSampleFormat,
            1);
    printf("samplesGetBufferSize: %d\n", samplesGetBufferSize);
    if (samplesGetBufferSize <= 0) {
        samplesGetBufferSize = 4096;
    }
    audioWrapper.father.outBufferSize = samplesGetBufferSize;
    // 存储音频帧的原始数据
    audioWrapper.father.outBuffer1 = (unsigned char *) av_malloc(samplesGetBufferSize);
    int samplesFillArrays = av_samples_fill_arrays(
            audioWrapper.father.dstAVFrame->data,
            audioWrapper.father.dstAVFrame->linesize,
            audioWrapper.father.outBuffer1,
            audioWrapper.dstNbChannels,
            audioWrapper.dstNbSamples,
            audioWrapper.dstAVSampleFormat,
            1);
    if (samplesFillArrays < 0) {
        printf("samplesFillArrays   : %d\n", samplesFillArrays);
        return -1;
    }*/

    /***
     struct SwrContext *s,
     int64_t out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
     int64_t  in_ch_layout, enum AVSampleFormat  in_sample_fmt, int  in_sample_rate,
     int log_offset, void *log_ctx
     */
    audioWrapper.swrContext = swr_alloc();
    //audioWrapper.swrContext =
    swr_alloc_set_opts(audioWrapper.swrContext,
                       audioWrapper.dstChannelLayout,  // out_ch_layout
                       audioWrapper.dstAVSampleFormat, // out_sample_fmt
                       audioWrapper.dstSampleRate,     // out_sample_rate
                       audioWrapper.srcChannelLayout,  // in_ch_layout
                       audioWrapper.srcAVSampleFormat, // in_sample_fmt
                       audioWrapper.srcSampleRate,     // in_sample_rate
                       0,                              // log_offset
                       NULL);                          // log_ctx
    if (audioWrapper.swrContext == NULL) {
        printf("%s\n", "audioSwrContext is NULL.");
        return -1;
    }

    int ret = swr_init(audioWrapper.swrContext);
    if (ret != 0) {
        printf("%s\n", "audioSwrContext swr_init failed.");
    } else {
        printf("%s\n", "audioSwrContext swr_init success.");
    }

    return 0;
}

int createSwsContext() {
    videoWrapper.srcWidth = videoWrapper.father.avCodecContext->width;
    videoWrapper.srcHeight = videoWrapper.father.avCodecContext->height;
    videoWrapper.srcAVPixelFormat = videoWrapper.father.avCodecContext->pix_fmt;
    printf("---------------------------------\n");
    printf("srcWidth            : %d\n", videoWrapper.srcWidth);
    printf("srcHeight           : %d\n", videoWrapper.srcHeight);
    printf("srcAVPixelFormat    : %d\n", videoWrapper.srcAVPixelFormat);
    // 播放视频时两个必须相同
    videoWrapper.dstWidth = videoWrapper.srcWidth;
    videoWrapper.dstHeight = videoWrapper.srcHeight;
    videoWrapper.dstAVPixelFormat = videoWrapper.srcAVPixelFormat;
    videoWrapper.srcArea = videoWrapper.srcWidth * videoWrapper.srcHeight;

    // avPacket ---> srcAVFrame ---> dstAVFrame ---> 渲染画面
    videoWrapper.father.srcAVFrame = av_frame_alloc();
    videoWrapper.father.dstAVFrame = av_frame_alloc();

    // srcXXX与dstXXX的参数必须要按照下面这样去设置,不然播放画面会有问题的
    // 根据视频源得到的AVPixelFormat,Width和Height计算出一帧视频所需要的空间大小
    int imageGetBufferSize = av_image_get_buffer_size(
            videoWrapper.dstAVPixelFormat, videoWrapper.srcWidth, videoWrapper.srcHeight, 1);
    printf("imageGetBufferSize  : %d\n", imageGetBufferSize);
    videoWrapper.father.outBufferSize = imageGetBufferSize;
    // 存储视频帧的原始数据
    videoWrapper.father.outBuffer1 = (unsigned char *) av_malloc(imageGetBufferSize);
    // 类似于格式化刚刚申请的内存(关联操作:dstAVFrame, outBuffer1, dstAVPixelFormat)
    int imageFillArrays = av_image_fill_arrays(
            // uint8_t *dst_data[4]
            videoWrapper.father.dstAVFrame->data,
            // int dst_linesize[4]
            videoWrapper.father.dstAVFrame->linesize,
            // const uint8_t *src
            videoWrapper.father.outBuffer1,
            videoWrapper.dstAVPixelFormat,
            videoWrapper.srcWidth, videoWrapper.srcHeight, 1);
    if (imageFillArrays < 0) {
        printf("imageFillArrays     : %d\n", imageFillArrays);
        return -1;
    }
    videoWrapper.swsContext = sws_getContext(
            videoWrapper.srcWidth, videoWrapper.srcHeight, videoWrapper.srcAVPixelFormat,
            videoWrapper.srcWidth, videoWrapper.srcHeight, videoWrapper.dstAVPixelFormat,
            SWS_BICUBIC,//flags
            NULL, NULL, NULL);
    if (videoWrapper.swsContext == NULL) {
        printf("%s\n", "videoSwsContext is NULL.");
        return -1;
    }
    printf("---------------------------------\n");

    return 0;
}

int putAVPacketToQueue(struct AVPacketQueue *packet_queue, AVPacket *avpacket) {
    // 需要把AVPacket类型构造成AVPacketList类型,因此先要构造一个AVPacketList指针
    AVPacketList *avpacket_list = NULL;
    avpacket_list = (AVPacketList *) av_malloc(sizeof(AVPacketList));
    if (!avpacket_list) {
        return -1;
    }
    avpacket_list->pkt = *avpacket;
    avpacket_list->next = NULL;

    // 第一次为NULL
    if (!packet_queue->lastAVPacketList) {
        packet_queue->firstAVPacketList = avpacket_list;
        /*fprintf(stdout,
                "packet_queue->first_pkt->pkt.pos = %ld\n",
                packet_queue->firstAVPacketList->pkt.pos);*/
    } else {
        packet_queue->lastAVPacketList->next = avpacket_list;
    }
    packet_queue->lastAVPacketList = avpacket_list;
    packet_queue->allAVPacketsCount++;
    packet_queue->allAVPacketsSize += avpacket_list->pkt.size;

    return 0;
}

static int getAVPacketFromQueue(struct AVPacketQueue *packet_queue, AVPacket *avpacket) {
    AVPacketList *avpacket_list = packet_queue->firstAVPacketList;
    if (avpacket_list) {
        packet_queue->firstAVPacketList = avpacket_list->next;
        if (!packet_queue->firstAVPacketList) {
            packet_queue->lastAVPacketList = NULL;
            // fprintf(stdout, "%s\n", "没有数据了");
        }
        *avpacket = avpacket_list->pkt;
        packet_queue->allAVPacketsCount--;
        packet_queue->allAVPacketsSize -= avpacket_list->pkt.size;

        av_free(avpacket_list);
        return 0;
    }

    return -1;
}

// 线程
// REFRESH_EVENT和BREAK_EVENT是用户自定义的消息
int pushEventThread(void *opaque) {
    threadExitFlag = 0;
    threadPauseFlag = 0;
    while (!threadExitFlag) {
        if (!threadPauseFlag) {
            SDL_Event event;
            event.type = REFRESH_EVENT;
            // 发送消息函数
            SDL_PushEvent(&event);
        }
        SDL_Delay(40);
    }
    threadExitFlag = 0;
    threadPauseFlag = 0;
    // Break
    SDL_Event event;
    event.type = BREAK_EVENT;
    SDL_PushEvent(&event);
    printf("SDL_PushEvent BREAK_EVENT\n");

    return 0;
}

int audioRender(void *opaque) {
    printf("%s\n", "audioRender() start");

    // 暂停/继续音频回调处理。参数1表暂停，0表继续。
    // 打开音频设备后默认未启动回调处理，通过调用SDL_PauseAudio(0)来启动回调处理。
    // 这样就可以在打开音频设备后先为回调函数安全初始化数据，一切就绪后再启动音频回调。
    // 在暂停期间，会将静音值往音频设备写。
    SDL_PauseAudio(0);

    AVPacket *srcAVPacket = av_packet_alloc();
    for (;;) {
        if (threadExitFlag == 1) {
            printf("%s\n", "audioRender() break");
            break;
        }

        /*if (audioWrapper.avPacketQueue.allAVPacketsSize > MAX_AUDIO_QUEUE_SIZE) {
            SDL_Delay(10);
            continue;
        }*/

        while (1) {
            // 读取一帧压缩数据放到avPacket
            int readFrame = av_read_frame(audioWrapper.father.avFormatContext, srcAVPacket);
            //printf("readFrame           : %d\n", readFrame);
            if (readFrame < 0) {
                /*threadExitFlag = 1;
                break;*/
            }

            /*if (srcAVPacket->stream_index == audioWrapper.father.streamIndex) {
                //printf("Audio break.\n");
                audioWrapper.father.readFramesCount++;
                putAVPacketToQueue(&audioWrapper.avPacketQueue, srcAVPacket);
                break;
            }*/
        }

        // 把aac数据(压缩数据)写入文件
        // fwrite(audioWrapper.father.avPacket->data, 1, audioWrapper.father.avPacket->size, fp_aac);

        // 解码一帧压缩数据
        if (avcodec_send_packet(audioWrapper.father.avCodecContext, srcAVPacket) < 0) {
            printf("Audio Decode Error.\n");
            return -1;
        }

        // 对压缩数据进行解码,解码后的数据放到srcAVFrame(保存的是非压缩数据)
        while (avcodec_receive_frame(audioWrapper.father.avCodecContext, audioWrapper.father.srcAVFrame) == 0) {
            /*swr_convert(audioWrapper.swrContext,
                        audioWrapper.father.dstAVFrame->data,
                        audioWrapper.father.outBufferSize,
                        (const uint8_t **) audioWrapper.father.srcAVFrame->data,
                        audioWrapper.srcNbSamples);*/

            // 将音频的采样率转换成本机能播出的采样率
            swr_convert(audioWrapper.swrContext,
                        &audioWrapper.father.outBuffer1,
//                        audioWrapper.father.outBufferSize,
                        audioWrapper.srcNbSamples,
                        (const uint8_t **) audioWrapper.father.srcAVFrame->data,
                        audioWrapper.srcNbSamples);

            // 在此处等待sdl_audio_callback将之前传递的音频数据播放完再向其中发送新的数据
            while (audio_len > 0) {
                SDL_Delay(1);
            }

            // 将读取到的数据存入音频缓冲区
            // 记录音频数据的长度
            audio_len = audioWrapper.father.outBufferSize;
            //audio_pos = audioWrapper.father.dstAVFrame->data[0];
            audio_pos = audioWrapper.father.outBuffer1;
        }

        // av_frame_unref(audioWrapper.father.srcAVFrame);
        av_packet_unref(srcAVPacket);
    }// for(;;) end

    printf("%s\n", "audioRender() end");
}

int videoRender(void *opaque) {
    printf("%s\n", "videoRender() start");

    printf("videoRender() sdlEvent.type: %d\n", REFRESH_EVENT);
    printf("videoRender() sdlEvent.type: %d\n", BREAK_EVENT);
    printf("videoRender() sdlEvent.type: %d\n", SDL_KEYDOWN);
    printf("videoRender() sdlEvent.type: %d\n", SDL_QUIT);

    for (;;) {
        // 不断地接收消息
        SDL_WaitEvent(&sdlEvent);
        //printf("SDL_WaitEvent sdlEvent.type: %d\n", sdlEvent.type);
        switch (sdlEvent.type) {
            case REFRESH_EVENT: {// 32769
                break;
            }
            case BREAK_EVENT: {// 32770
                printf("%s\n", "videoRender() BREAK_EVENT");
                threadExitFlag = 1;
                break;
            }
            case SDL_KEYDOWN: {// 768
                // Pause
                if (sdlEvent.key.keysym.sym == SDLK_SPACE) {
                    printf("%s\n", "videoRender() SDLK_SPACE");
                    threadPauseFlag = !threadPauseFlag;
                    continue;
                }
            }
            case SDL_QUIT: {// 256
                // 关闭播放窗口时会接收到这个事件
                printf("%s\n", "videoRender() SDL_QUIT");
                threadExitFlag = 1;
                continue;
            }
            default:
                continue;
        }

        break;
    }// for(;;) end

    printf("%s\n", "videoRender() end");
}

void *handleVideoData(void *opaque);

void *readData(void *opaque) {
    printf("%s\n", "readData() start");
    if (opaque == NULL) {
        return NULL;
    }
    Wrapper *wrapper = (Wrapper *) opaque;

    bool onlyOne = true;
    threadExitFlag = 0;
    wrapper->isHandlingForQueue1 = false;
    wrapper->isHandlingForQueue2 = false;
    wrapper->readFramesCount = 0;
    wrapper->handleFramesCount = 0;
    AVPacket *srcAVPacket = av_packet_alloc(), *dstAVPacket = av_packet_alloc();
    for (;;) {
        if (threadExitFlag == 1) {
            break;
        }

        av_packet_unref(srcAVPacket);
        memset(srcAVPacket, 0, sizeof(*srcAVPacket));
        memset(dstAVPacket, 0, sizeof(*dstAVPacket));

        while (1) {
            // 读取一帧压缩数据放到avPacket
            // 0 if OK, < 0 on error or end of file
            // 有时读一次跳出,有时读多次跳出
            int readFrame =
                    av_read_frame(wrapper->avFormatContext, srcAVPacket);
            //printf("readFrame           : %d\n", readFrame);
            if (readFrame < 0) {
                fprintf(stderr, "readFrame           : %d\n", readFrame);
                if (wrapper->queue1->allAVPacketsCount > 0) {
                    if (!wrapper->isHandlingForQueue1
                        && !wrapper->isHandlingForQueue2) {
                        wrapper->isHandlingForQueue1 = true;
                    }
                }
                if (wrapper->queue2->allAVPacketsCount > 0) {
                    if (!wrapper->isHandlingForQueue2) {
                        wrapper->isHandlingForQueue2 = true;
                    }
                }

                // 为了调用initAudioSDL()
                if (audioWrapper.dstSDLAudioSpec.channels == 0) {
                    // 唤醒线程
                    pthread_mutex_lock(&lockMutex);
                    fprintf(stdout, "readData() pthread_cond_signal() break\n");
                    pthread_cond_signal(&lockCondition);
                    pthread_mutex_unlock(&lockMutex);
                }

                threadExitFlag = 1;
                break;
            }

            if (srcAVPacket->stream_index == wrapper->streamIndex) {
                if (!wrapper->isHandlingForQueue1) {
                    wrapper->readFramesCount++;
                    // 非常非常非常必须的
                    av_copy_packet(dstAVPacket, srcAVPacket);
                    putAVPacketToQueue(wrapper->queue1, dstAVPacket);
                    if (wrapper->queue1->allAVPacketsCount == 500 && onlyOne) {
                        onlyOne = false;
                        wrapper->isHandlingForQueue1 = true;
                        wrapper->isHandlingForQueue2 = false;
                        fprintf(stdout, "readData() allAVPacketsCount: %ld\n",
                                wrapper->queue1->allAVPacketsCount);
                        fprintf(stdout, "readData() allAVPacketsSize : %ld\n",
                                wrapper->queue1->allAVPacketsSize);
                        // 开始解码
                        fprintf(stdout, "readData() 开始解码\n");
                        // 为了调用initAudioSDL()
                        if (audioWrapper.dstSDLAudioSpec.channels == 0) {
                            // 唤醒线程
                            pthread_mutex_lock(&lockMutex);
                            fprintf(stdout, "readData() pthread_cond_signal()\n");
                            // 相当于java的notify()
                            pthread_cond_signal(&lockCondition);
                            pthread_mutex_unlock(&lockMutex);
                        }
                    }
                } else if (!wrapper->isHandlingForQueue2) {
                    wrapper->readFramesCount++;
                    av_copy_packet(dstAVPacket, srcAVPacket);
                    putAVPacketToQueue(wrapper->queue2, dstAVPacket);
                }
                //fprintf(stdout, "av_read_frame break\n");
                break;
            } else {
                // 遇到其他流时释放
                av_packet_unref(srcAVPacket);
            }
        }// while(1) end
    }// for(;;) end

    fprintf(stdout, "readData() readFramesCount                : %d\n", wrapper->readFramesCount);

    printf("%s\n", "readData() end");
    return NULL;
}

void *handleAudioData(void *opaque) {
    printf("%s\n", "handleAudioData() start");

    // 等待读取一定的数据
    // 线程等待
    pthread_mutex_lock(&lockMutex);
    fprintf(stdout, "handleAudioData() pthread_cond_wait() start\n");
    // 相当于java的wait()
    pthread_cond_wait(&lockCondition, &lockMutex);
    fprintf(stdout, "handleAudioData() pthread_cond_wait() end\n");
    pthread_mutex_unlock(&lockMutex);

    // 调用后会马上回调sdlAudioCallback(...)
    if (initAudioSDL() < 0) {
        return NULL;
    }

    // 等待结束
    // 线程等待
    pthread_mutex_lock(&lockMutex);
    fprintf(stdout, "handleAudioData() pthread_cond_wait() start\n");
    // 相当于java的wait()
    pthread_cond_wait(&lockCondition, &lockMutex);
    fprintf(stdout, "handleAudioData() pthread_cond_wait() end\n");
    pthread_mutex_unlock(&lockMutex);

    printf("%s\n", "handleAudioData() end");
    return NULL;
}

void *handleVideoData(void *opaque) {
    printf("%s\n", "handleVideoData() start");

    // 线程等待
    pthread_mutex_lock(&lockMutex);
    fprintf(stdout, "handleVideoData() pthread_cond_wait() start\n");
    // 相当于java的wait()
    pthread_cond_wait(&lockCondition, &lockMutex);
    fprintf(stdout, "handleVideoData() pthread_cond_wait() end\n");
    pthread_mutex_unlock(&lockMutex);

    clock_t startTime = clock();
    fprintf(stdout, "handleVideoData() startTime: %ld\n", startTime);
    // 必须创建
    AVPacket *avPacket = av_packet_alloc();
    for (;;) {
        memset(avPacket, 0, sizeof(*avPacket));
        if (videoWrapper.father.isHandlingForQueue1
            && videoWrapper.father.queue1->allAVPacketsCount > 0) {
            videoWrapper.father.handleFramesCount++;
            getAVPacketFromQueue(videoWrapper.father.queue1, avPacket);
            if (videoWrapper.father.queue1->allAVPacketsCount == 0) {
                memset(videoWrapper.father.queue1, 0, sizeof(struct AVPacketQueue));
                videoWrapper.father.isHandlingForQueue1 = false;
                videoWrapper.father.isHandlingForQueue2 = true;
                fprintf(stdout, "handleVideoData() video allAVPacketsCount2: %d\n",
                        videoWrapper.father.queue2->allAVPacketsCount);
            }
        } else if (videoWrapper.father.isHandlingForQueue2
                   && videoWrapper.father.queue2->allAVPacketsCount > 0) {
            videoWrapper.father.handleFramesCount++;
            getAVPacketFromQueue(videoWrapper.father.queue2, avPacket);
            if (videoWrapper.father.queue2->allAVPacketsCount == 0) {
                memset(videoWrapper.father.queue2, 0, sizeof(struct AVPacketQueue));
                videoWrapper.father.isHandlingForQueue1 = true;
                videoWrapper.father.isHandlingForQueue2 = false;
                fprintf(stdout, "handleVideoData() video allAVPacketsCount1: %d\n",
                        videoWrapper.father.queue1->allAVPacketsCount);
            }
        } else {
            break;
        }

        if (!avPacket) {
            if (videoWrapper.father.queue1->allAVPacketsCount == 0
                && videoWrapper.father.queue2->allAVPacketsCount == 0) {
                break;
            }
            continue;
        }

        // 发送压缩数据去进行解码
        if (avcodec_send_packet(videoWrapper.father.avCodecContext, avPacket) < 0) {
            fprintf(stderr, "video decode error.\n");
            return NULL;
        }
        // 对压缩数据进行解码,解码后的数据放到srcAVFrame(保存的是非压缩数据)
        while (1) {
            int receiveFrame =
                    avcodec_receive_frame(videoWrapper.father.avCodecContext, videoWrapper.father.srcAVFrame);
            // fprintf(stdout, "receiveFrame: %d\n", receiveFrame);
            if (receiveFrame != 0) {
                break;
            }

            // 进行格式的转换,转换后的数据放在dstAVFrame
            sws_scale(videoWrapper.swsContext,
                      (const unsigned char *const *) videoWrapper.father.srcAVFrame->data,
                      videoWrapper.father.srcAVFrame->linesize,
                      0, videoWrapper.srcHeight,
                      videoWrapper.father.dstAVFrame->data,
                      videoWrapper.father.dstAVFrame->linesize);
        }
        av_packet_unref(avPacket);

        // 视频的话前三个有值(可能是data[0], data[1], data[2]分别存了YUV的值吧)
        /*fprintf(stdout, "handleData() dstAVFrame->linesize[0]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[0]);
        fprintf(stdout, "handleData() dstAVFrame->linesize[1]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[1]);
        fprintf(stdout, "handleData() dstAVFrame->linesize[2]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[2]);
        fprintf(stdout, "handleData() dstAVFrame->linesize[3]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[3]);
        fprintf(stdout, "handleData() dstAVFrame->linesize[4]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[4]);
        fprintf(stdout, "handleData() dstAVFrame->linesize[5]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[5]);
        fprintf(stdout, "handleData() dstAVFrame->linesize[6]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[6]);
        fprintf(stdout, "handleVideoData() dstAVFrame->linesize[7]: %d\n",
                videoWrapper.father.dstAVFrame->linesize[7]);*/

        // SDL Start---------------------
        // sws_scale()函数不调用,直接播放srcAVFrame中的数据也可以,只是画质没有转换过的好
        SDL_UpdateTexture(sdlTexture, NULL,
                          videoWrapper.father.dstAVFrame->data[0],
                          videoWrapper.father.dstAVFrame->linesize[0]);
        SDL_RenderClear(sdlRenderer);
        //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);
        // SDL End-----------------------
        SDL_Delay(40);

        /*fprintf(stdout, "handleData() dstAVFrame->pkt_pts: %ld\n",
          videoWrapper.father.dstAVFrame->best_effort_timestamp);
        fprintf(stdout, "handleVideoData() clock() - startTime: %ld\n", clock() - startTime);*/
        /*while (avPacket->pts > clock() - startTime) {
            SDL_Delay(1);
        }*/
    }// for(;;) end

    fprintf(stdout, "handleVideoData() video handleFramesCount : %d\n",
            videoWrapper.father.handleFramesCount);

    printf("%s\n", "handleVideoData() end");
    return NULL;
}

void close2() {
    printf("%s\n", "close() start");
    // 清理工作
    if (sdlWindow != NULL) {
        SDL_Quit();
    }
    if (sdlThread != NULL) {
        SDL_DetachThread(sdlThread);
        sdlThread = NULL;
    }
    SDL_CloseAudio();

    //audio
    if (audioWrapper.father.outBuffer1 != NULL) {
        av_free(audioWrapper.father.outBuffer1);
        audioWrapper.father.outBuffer1 = NULL;
    }
    if (audioWrapper.father.outBuffer2 != NULL) {
        av_free(audioWrapper.father.outBuffer2);
        audioWrapper.father.outBuffer2 = NULL;
    }
    if (audioWrapper.father.outBuffer3 != NULL) {
        av_free(audioWrapper.father.outBuffer3);
        audioWrapper.father.outBuffer3 = NULL;
    }
    if (audioWrapper.father.srcData[0] != NULL) {
        av_freep(&audioWrapper.father.srcData[0]);
        audioWrapper.father.srcData[0] = NULL;
    }
    if (audioWrapper.father.dstData[0] != NULL) {
        av_freep(&audioWrapper.father.dstData[0]);
        audioWrapper.father.dstData[0] = NULL;
    }
    if (audioWrapper.swrContext != NULL) {
        swr_free(&audioWrapper.swrContext);
        audioWrapper.swrContext = NULL;
    }
    if (audioWrapper.father.srcAVFrame != NULL) {
        av_frame_free(&audioWrapper.father.srcAVFrame);
        audioWrapper.father.srcAVFrame = NULL;
    }
    if (audioWrapper.father.dstAVFrame != NULL) {
        av_frame_free(&audioWrapper.father.dstAVFrame);
        audioWrapper.father.dstAVFrame = NULL;
    }
    if (audioWrapper.father.avCodecParameters != NULL) {
        avcodec_parameters_free(&audioWrapper.father.avCodecParameters);
        audioWrapper.father.avCodecParameters = NULL;
    }
    if (audioWrapper.father.avCodecContext != NULL) {
        avcodec_close(audioWrapper.father.avCodecContext);
        av_free(audioWrapper.father.avCodecContext);
        audioWrapper.father.avCodecContext = NULL;
    }
    //video
    if (videoWrapper.father.outBuffer1 != NULL) {
        av_free(videoWrapper.father.outBuffer1);
        videoWrapper.father.outBuffer1 = NULL;
    }
    if (videoWrapper.father.outBuffer2 != NULL) {
        av_free(videoWrapper.father.outBuffer2);
        //videoWrapper.father.outBuffer2 = NULL;
    }
    if (videoWrapper.father.outBuffer3 != NULL) {
        av_free(videoWrapper.father.outBuffer3);
        videoWrapper.father.outBuffer3 = NULL;
    }
    if (videoWrapper.father.srcData[0] != NULL) {
        av_freep(&videoWrapper.father.srcData[0]);
        videoWrapper.father.srcData[0] = NULL;
    }
    if (videoWrapper.father.dstData[0] != NULL) {
        av_freep(&videoWrapper.father.dstData[0]);
        videoWrapper.father.dstData[0] = NULL;
    }
    if (videoWrapper.swsContext != NULL) {
        sws_freeContext(videoWrapper.swsContext);
        videoWrapper.swsContext = NULL;
    }
    if (videoWrapper.father.srcAVFrame != NULL) {
        av_frame_free(&videoWrapper.father.srcAVFrame);
        videoWrapper.father.srcAVFrame = NULL;
    }
    if (videoWrapper.father.dstAVFrame != NULL) {
        av_frame_free(&videoWrapper.father.dstAVFrame);
        videoWrapper.father.dstAVFrame = NULL;
    }
    if (videoWrapper.father.avCodecParameters != NULL) {
        avcodec_parameters_free(&videoWrapper.father.avCodecParameters);
        videoWrapper.father.avCodecParameters = NULL;
    }
    if (videoWrapper.father.avCodecContext != NULL) {
        avcodec_close(videoWrapper.father.avCodecContext);
        av_free(videoWrapper.father.avCodecContext);
        videoWrapper.father.avCodecContext = NULL;
    }

    /*if (avFormatContext != NULL) {
        avformat_close_input(&avFormatContext);
        avFormatContext = NULL;
    }*/

    /*if (inFile != NULL) {
        fclose(inFile);
        inFile = NULL;
    }
    if (outFile != NULL) {
        fclose(outFile);
        outFile = NULL;
    }*/
    printf("%s\n", "close() end");
}

void avDumpFormat() {
#if AV_DUMP_FORMAT
    printf("-------------File Information-------------\n");
    av_dump_format(audioWrapper.father.avFormatContext, 0, inFilePath2, 0);
    printf("------------------------------------------\n");
    av_dump_format(videoWrapper.father.avFormatContext, 0, inFilePath2, 0);
    printf("------------------------------------------\n");
#endif
}

/***

 */

int alexanderAudioPlayerWithSDL() {
    printf("%s\n", "alexanderAudioPlayerWithSDL() start");

    memset(&audioWrapper, 0, sizeof(struct AudioWrapper));

    if (initSDL() < 0) {
        return -1;
    }
    if (openAndFindAVFormatContextForAudio() < 0) {
        return -1;
    }
    //avDumpFormat();
    if (findStreamIndexForAudio() < 0) {
        return -1;
    }
    if (findAndOpenAVCodecForAudio() < 0) {
        return -1;
    }
    if (createSwrContent() < 0) {
        printf("%s\n", "");
        return -1;
    }
    /*if (initAudioSDL() < 0) {
        return -1;
    }*/

    fprintf(stdout, "alexanderAudioPlayerWithSDL() audioWrapper.dstSDLAudioSpec.channels: %d\n",
            audioWrapper.dstSDLAudioSpec.channels);

    audioWrapper.father.queue1 = (struct AVPacketQueue *) malloc(sizeof(struct AVPacketQueue));
    audioWrapper.father.queue2 = (struct AVPacketQueue *) malloc(sizeof(struct AVPacketQueue));
    memset(audioWrapper.father.queue1, 0, sizeof(struct AVPacketQueue));
    memset(audioWrapper.father.queue2, 0, sizeof(struct AVPacketQueue));

    pthread_t readDataThread;
    pthread_t handleDataThread;
    // 创建线程
    pthread_create(&readDataThread, NULL, readData, &audioWrapper);
    pthread_create(&handleDataThread, NULL, handleAudioData, NULL);
    // 等待线程执行完
    pthread_join(readDataThread, NULL);
    pthread_join(handleDataThread, NULL);
    // 取消线程
    pthread_cancel(readDataThread);
    pthread_cancel(handleDataThread);

    pthread_mutex_destroy(&lockMutex);
    pthread_cond_destroy(&lockCondition);

    free(audioWrapper.father.queue1);
    free(audioWrapper.father.queue2);

    close2();

    printf("%s\n", "alexanderAudioPlayerWithSDL() start");
}

/***
 run this method and playback video
 */
int alexanderVideoPlayerWithSDL() {
    printf("%s\n", "alexanderVideoPlayerWithSDL() start");

    memset(&videoWrapper, 0, sizeof(struct VideoWrapper));

    // initAV();
    if (initSDL() < 0) {
        return -1;
    }

    bool needAudio = false;
    if (needAudio) {
        // audio
        if (openAndFindAVFormatContextForAudio() < 0) {
            return -1;
        }
        //avDumpFormat();
        if (findStreamIndexForAudio() < 0) {
            return -1;
        }
        if (findAndOpenAVCodecForAudio() < 0) {
            return -1;
        }
        if (createSwrContent() < 0) {
            printf("%s\n", "");
            return -1;
        }
        if (initAudioSDL() < 0) {
            return -1;
        }
    }

    // video
    if (openAndFindAVFormatContextForVideo() < 0) {
        return -1;
    }
    //avDumpFormat();
    if (findStreamIndexForVideo() < 0) {
        return -1;
    }
    if (findAndOpenAVCodecForVideo() < 0) {
        return -1;
    }
    if (createSwsContext() < 0) {
        printf("%s\n", "");
        return -1;
    }
    if (initVideoSDL() < 0) {
        return -1;
    }

    videoWrapper.father.queue1 = (struct AVPacketQueue *) malloc(sizeof(struct AVPacketQueue));
    videoWrapper.father.queue2 = (struct AVPacketQueue *) malloc(sizeof(struct AVPacketQueue));
    memset(videoWrapper.father.queue1, 0, sizeof(struct AVPacketQueue));
    memset(videoWrapper.father.queue2, 0, sizeof(struct AVPacketQueue));

    pthread_t readDataThread, handleDataThread;
    // 创建线程
    pthread_create(&readDataThread, NULL, readData, &videoWrapper);
    pthread_create(&handleDataThread, NULL, handleVideoData, NULL);
    // 等待线程执行完
    pthread_join(readDataThread, NULL);
    pthread_join(handleDataThread, NULL);
    // 取消线程
    pthread_cancel(readDataThread);
    pthread_cancel(handleDataThread);
    pthread_mutex_destroy(&lockMutex);
    pthread_cond_destroy(&lockCondition);

    free(videoWrapper.father.queue1);
    free(videoWrapper.father.queue2);

    printf("%s\n", "alexanderVideoPlayerWithSDL() end");

    close2();

    return 0;
}


#endif //MYSTUDY_FFMPEG_STUDYAGAIN