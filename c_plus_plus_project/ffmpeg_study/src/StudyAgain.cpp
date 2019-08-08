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
    int allAVPackets = 0;
    // 所有AVPacket占用的空间大小
    int allAVPacketsSize = 0;
    // 互斥量
    SDL_mutex *sdlMutex = NULL;
    // 条件变量
    SDL_cond *sdlCond = NULL;
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
    AVPacket *avPacket = NULL;
    // 存储非压缩数据(视频对应RGB/YUV像素数据,音频对应PCM采样数据)
    AVFrame *srcAVFrame = NULL;
    // 用于格式转换
    AVFrame *dstAVFrame = NULL;
    // 有些东西需要通过它去得到
    AVCodecParameters *avCodecParameters = NULL;
    int streamIndex = -1;
    int framesCount = 0;
    // 存储原始数据
    unsigned char *outBuffer1 = NULL;
    unsigned char *outBuffer2 = NULL;
    unsigned char *outBuffer3 = NULL;
    size_t outBufferSize = 0;
    // 视频使用到sws_scale函数时需要定义这些变量,音频也要用到
    unsigned char *srcData[4] = {NULL}, dstData[4] = {NULL};

    SDL_Thread *renderThread = NULL;
};

struct AudioWrapper {
    struct Wrapper father;
    SwrContext *swrContext = NULL;
    // 从音频源或视频源中得到(采样格式)
    enum AVSampleFormat srcAVSampleFormat = AV_SAMPLE_FMT_NONE;
    // 输出的采样格式16bit PCM
    enum AVSampleFormat dstAVSampleFormat = AV_SAMPLE_FMT_S16;
    // 从音频源或视频源中得到
    // 采样率
    int srcSampleRate = 0;
    int srcNbSamples = 0;
    // 声道数
    int srcNbChannels = 0;
    // 由srcNbChannels能得到srcChannelLayout,也能由srcChannelLayout得到srcNbChannels
    int srcChannelLayout = 0;
    // 用户设置
    int dstSampleRate = 44100;
    int dstNbSamples = 0;
    // 由dstChannelLayout去获到
    int dstNbChannels = 0;
    // 双声道输出
    //int dstChannelLayout = AV_CH_LAYOUT_STEREO;
    int64_t dstChannelLayout = 0;

    // SDL
    SDL_AudioSpec srcSDLAudioSpec;
    SDL_AudioSpec dstSDLAudioSpec;

    AVPacketQueue avPacketQueue;
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

//格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
// AVFormatContext相当于Android的MediaExtractor,保存了音视频的Format信息(MediaFormat)
//AVFormatContext *avFormatContext = NULL;
//char *inFilePath2 = "/root/视频/tomcat_video/shape_of_my_heart.mp4";
char *inFilePath2 = "/root/音乐/KuGou/蔡国权-不装饰你的梦.mp3";
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

void initAVPacketQueue(struct AVPacketQueue *queue) {
    memset(queue, 0, sizeof(struct AVPacketQueue));
    //创建的互斥量默认是未上锁的
    queue->sdlMutex = SDL_CreateMutex();
    //创建条件变量
    queue->sdlCond = SDL_CreateCond();
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

int openAndFindAVFormatContext() {
    // AVFormatContext初始化，里面设置结构体的一些默认信息
    // 相当于Java中创建对象
    audioWrapper.father.avFormatContext = avformat_alloc_context();
    if (audioWrapper.father.avFormatContext == NULL) {
        return -1;
    }
    // 获取基本的文件信息
    if (avformat_open_input(&audioWrapper.father.avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    // 文件中的流信息
    if (avformat_find_stream_info(audioWrapper.father.avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    videoWrapper.father.avFormatContext = avformat_alloc_context();
    if (videoWrapper.father.avFormatContext == NULL) {
        return -1;
    }
    if (avformat_open_input(&videoWrapper.father.avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(videoWrapper.father.avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }
}

/***
 找音视频流轨道
 */
int findStreamIndex() {
    if (audioWrapper.father.avFormatContext == NULL
        || videoWrapper.father.avFormatContext == NULL) {
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
    // video stream index
    streams = videoWrapper.father.avFormatContext->nb_streams;
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

    if (audioWrapper.father.streamIndex == -1
        && videoWrapper.father.streamIndex == -1) {
        printf("Didn't find a stream.\n");
        return -1;
    } else {
        printf("audioStreamIndex: %d\n", audioWrapper.father.streamIndex);
        printf("videoStreamIndex: %d\n", videoWrapper.father.streamIndex);
        return 0;
    }
}

int findAndOpenAVCodec() {
    if ((audioWrapper.father.avCodecParameters == NULL
         && videoWrapper.father.avCodecParameters == NULL)
        || (audioWrapper.father.streamIndex == -1
            && videoWrapper.father.streamIndex == -1)) {
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
    //audioWrapper.srcChannelLayout = av_get_default_channel_layout(audioWrapper.srcNbChannels);
    audioWrapper.srcAVSampleFormat = audioWrapper.father.avCodecContext->sample_fmt;
    printf("---------------------------------\n");
    printf("srcSampleRate       : %d\n", audioWrapper.srcSampleRate);
    printf("srcNbSamples        : %d\n", audioWrapper.srcNbSamples);
    printf("srcNbChannels       : %d\n", audioWrapper.srcNbChannels);
    printf("srcChannelLayout    : %d\n", audioWrapper.srcChannelLayout);
    printf("srcAVSampleFormat   : %d\n", audioWrapper.srcAVSampleFormat);
    printf("---------------------------------\n");
    // dst
    audioWrapper.dstNbSamples = audioWrapper.srcNbSamples;
    audioWrapper.dstNbChannels = audioWrapper.srcNbChannels;
    // audioWrapper.dstChannelLayout = audioWrapper.srcChannelLayout;
    if (!audioWrapper.dstChannelLayout
        || audioWrapper.dstChannelLayout != av_get_channel_layout_nb_channels(audioWrapper.dstChannelLayout)) {
        audioWrapper.dstChannelLayout = av_get_default_channel_layout(audioWrapper.dstNbChannels);
        printf("dstChannelLayout1   : %d\n", audioWrapper.dstChannelLayout);
        //why?
        audioWrapper.dstChannelLayout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
        printf("dstChannelLayout2   : %d\n", audioWrapper.dstChannelLayout);
    }
    audioWrapper.dstNbChannels = av_get_channel_layout_nb_channels(audioWrapper.dstChannelLayout);

    printf("dstSampleRate       : %d\n", audioWrapper.dstSampleRate);
    printf("dstNbSamples        : %d\n", audioWrapper.dstNbSamples);
    printf("dstNbChannels       : %d\n", audioWrapper.dstNbChannels);
    //printf("dstChannelLayout    : %d\n", audioWrapper.dstChannelLayout);
    printf("dstAVSampleFormat   : %d\n", audioWrapper.dstAVSampleFormat);
    printf("---------------------------------\n");

    // avPacket ---> srcAVFrame ---> dstAVFrame ---> 播放声音
    audioWrapper.father.avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(audioWrapper.father.avPacket);
    audioWrapper.father.srcAVFrame = av_frame_alloc();
    audioWrapper.father.dstAVFrame = av_frame_alloc();

    int samplesGetBufferSize = av_samples_get_buffer_size(
            audioWrapper.father.dstAVFrame->linesize,
            audioWrapper.dstNbChannels, audioWrapper.dstNbSamples, audioWrapper.dstAVSampleFormat,
            1);
    printf("samplesGetBufferSize: %d\n", samplesGetBufferSize);
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
        return -1;
    }

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
    printf("srcWidth            : %d\n", videoWrapper.srcWidth);
    printf("srcHeight           : %d\n", videoWrapper.srcHeight);
    printf("srcAVPixelFormat    : %d\n", videoWrapper.srcAVPixelFormat);
    // 播放视频时两个必须相同
    videoWrapper.dstWidth = videoWrapper.srcWidth;
    videoWrapper.dstHeight = videoWrapper.srcHeight;
    videoWrapper.dstAVPixelFormat = videoWrapper.srcAVPixelFormat;
    videoWrapper.srcArea = videoWrapper.srcWidth * videoWrapper.srcHeight;

    // avPacket ---> srcAVFrame ---> dstAVFrame ---> 渲染画面
    videoWrapper.father.avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(videoWrapper.father.avPacket);
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

    return 0;
}

int putAVPacketToQueue(struct AVPacketQueue *packet_queue, AVPacket *avpacket) {
    if (SDL_LockMutex(packet_queue->sdlMutex) == 0) {
        //需要把AVPacket类型构造成AVPacketList类型,因此先要构造一个AVPacketList指针
        AVPacketList *avpacket_list = NULL;
        avpacket_list = (AVPacketList *) av_malloc(sizeof(AVPacketList));
        if (!avpacket_list) {
            return -1;
        }
        avpacket_list->pkt = *avpacket;
        avpacket_list->next = NULL;

        //SDL_LockMutex(packet_queue->sdl_mutex);

        // 第一次为NULL
        if (!packet_queue->lastAVPacketList) {
            packet_queue->firstAVPacketList = avpacket_list;
            fprintf(stdout,
                    "packet_queue->first_pkt->pkt.pos = %ld\n",
                    packet_queue->firstAVPacketList->pkt.pos);
        } else {
            packet_queue->lastAVPacketList->next = avpacket_list;
        }
        packet_queue->lastAVPacketList = avpacket_list;
        packet_queue->allAVPackets++;
        packet_queue->allAVPacketsSize += avpacket_list->pkt.size;
        //?
        SDL_CondSignal(packet_queue->sdlCond);

        SDL_UnlockMutex(packet_queue->sdlMutex);
    }
    return 0;
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

// SDL提供的回调函数,数据只有往这个回调函数的stream中填充数据才能发出声音
void sdlAudioCallback(void *userdata, uint8_t *stream, int len) {
    SDL_memset(stream, 0, len);
    if (audio_len <= 0) {
        return;
    }
    // 比较剩余未读取的音频数据的长度和所需要的长度.尽最大可能的给予其音频数据
    len = ((uint32_t) len > audio_len ? audio_len : len);

    // SDL_MixAudio的作用和memcpy类似,这里将audio_pos的数据传递给stream
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);

    //audio_pos是记录out_buffer（存放我们读取音频数据的缓冲区）当前读取的位置
    //audio_len是记录out_buffer剩余未读数据的长度
    audio_pos += len; //audio_pos前进到新的位置
    audio_len -= len; //audio_len的长度做相应的减少
}

int audioRender(void *opaque) {
    printf("%s\n", "audioRender() start");

    // 暂停/继续音频回调处理。参数1表暂停，0表继续。
    // 打开音频设备后默认未启动回调处理，通过调用SDL_PauseAudio(0)来启动回调处理。
    // 这样就可以在打开音频设备后先为回调函数安全初始化数据，一切就绪后再启动音频回调。
    // 在暂停期间，会将静音值往音频设备写。
    SDL_PauseAudio(0);

    for (;;) {
        if (threadExitFlag == 1) {
            printf("%s\n", "audioRender() break");
            break;
        }

        if (audioWrapper.avPacketQueue.allAVPacketsSize > MAX_AUDIO_QUEUE_SIZE) {
            SDL_Delay(10);
            continue;
        }

        while (1) {
            // 读取一帧压缩数据放到avPacket
            int readFrame = av_read_frame(audioWrapper.father.avFormatContext, audioWrapper.father.avPacket);
            //printf("readFrame           : %d\n", readFrame);
            if (readFrame < 0) {
                /*threadExitFlag = 1;
                break;*/
            }

            if (audioWrapper.father.avPacket->stream_index == audioWrapper.father.streamIndex) {
                //printf("Audio break.\n");
                audioWrapper.father.framesCount++;
                putAVPacketToQueue(&audioWrapper.avPacketQueue, audioWrapper.father.avPacket);
                break;
            }
        }

        // 把aac数据(压缩数据)写入文件
        // fwrite(audioWrapper.father.avPacket->data, 1, audioWrapper.father.avPacket->size, fp_aac);

        // 解码一帧压缩数据
        if (avcodec_send_packet(audioWrapper.father.avCodecContext, audioWrapper.father.avPacket) < 0) {
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
        av_packet_unref(audioWrapper.father.avPacket);
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
                while (1) {
                    // 读取一帧压缩数据放到avPacket
                    int readFrame = av_read_frame(videoWrapper.father.avFormatContext, videoWrapper.father.avPacket);
                    //printf("readFrame           : %d\n", readFrame);
                    if (readFrame < 0) {
                        threadExitFlag = 1;
                    }

                    if (videoWrapper.father.avPacket->stream_index == videoWrapper.father.streamIndex) {
                        //printf("Video break.\n");
                        break;
                    }
                }

                // 把H264数据(压缩数据)写入文件
                // fwrite(videoWrapper.father.avPacket->data, 1, videoWrapper.father.avPacket->size, fp_h264);

                // 解码一帧压缩数据
                if (avcodec_send_packet(videoWrapper.father.avCodecContext, videoWrapper.father.avPacket) < 0) {
                    printf("Video Decode Error.\n");
                    return -1;
                }

                // 对压缩数据进行解码,解码后的数据放到srcAVFrame(保存的是非压缩数据)
                while (avcodec_receive_frame(videoWrapper.father.avCodecContext, videoWrapper.father.srcAVFrame) == 0) {
                    // 进行格式的转换,转换后的数据放在dstAVFrame
                    sws_scale(videoWrapper.swsContext,
                              (const unsigned char *const *) videoWrapper.father.srcAVFrame->data,
                              videoWrapper.father.srcAVFrame->linesize,
                              0, videoWrapper.srcHeight,
                              videoWrapper.father.dstAVFrame->data,
                              videoWrapper.father.dstAVFrame->linesize);
                    // 目标格式为AV_PIX_FMT_YUV420P时可以使用下面方式保存成文件
                    // fwrite(videoWrapper.father.dstAVFrame->data[0], 1, videoWrapper.srcArea, f_yuv);      //Y
                    // fwrite(videoWrapper.father.dstAVFrame->data[1], 1, videoWrapper.srcArea / 4, f_yuv);  //U
                    // fwrite(videoWrapper.father.dstAVFrame->data[2], 1, videoWrapper.srcArea / 4, f_yuv);  //V
                }

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

                av_packet_unref(videoWrapper.father.avPacket);
                continue;
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

int initSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("SDL_Init() failed: %s\n", SDL_GetError());
        return -1;
    }
}

int initAudioSDL() {
    // B2. 打开音频设备并创建音频处理线程
    // B2.1 打开音频设备，获取SDL设备支持的音频参数srcSDLAudioSpec(期望的参数是dstSDLAudioSpec，实际得到srcSDLAudioSpec)
    // 1) SDL提供两种使音频设备取得音频数据方法：
    //    a. push，SDL以特定的频率调用回调函数，在回调函数中取得音频数据
    //    b. pull，用户程序以特定的频率调用SDL_QueueAudio()，向音频设备提供数据。此种情况dstSDLAudioSpec.callback=NULL
    // 2) 音频设备打开后播放静音，不启动回调，调用SDL_PauseAudio(0)后启动回调，开始正常播放音频
    // 采样率
    audioWrapper.dstSDLAudioSpec.freq = audioWrapper.dstSampleRate;
    // S表带符号，16是采样深度，SYS表采用系统字节序
    audioWrapper.dstSDLAudioSpec.format = AUDIO_S16SYS;
    // 声道数
    audioWrapper.dstSDLAudioSpec.channels = audioWrapper.dstNbChannels;
    // 静音值
    audioWrapper.dstSDLAudioSpec.silence = 0;
    // SDL声音缓冲区尺寸，单位是单声道采样点尺寸x通道数
    // audioWrapper.dstSDLAudioSpec.samples = audioWrapper.dstNbSamples;
    audioWrapper.dstSDLAudioSpec.samples = 1024;
    // 回调函数，若为NULL，则应使用SDL_QueueAudio()机制
    audioWrapper.dstSDLAudioSpec.callback = sdlAudioCallback;
    // 提供给回调函数的参数
    audioWrapper.dstSDLAudioSpec.userdata = &audioWrapper;
    if (SDL_OpenAudio(&audioWrapper.dstSDLAudioSpec, &audioWrapper.srcSDLAudioSpec) < 0) {
        printf("SDL_OpenAudio() failed: %s\n", SDL_GetError());
        return -1;
    }

    const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
    while (SDL_OpenAudio(&audioWrapper.dstSDLAudioSpec, &audioWrapper.srcSDLAudioSpec) < 0) {
        fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", audioWrapper.dstNbChannels, SDL_GetError());
        audioWrapper.dstSDLAudioSpec.channels = next_nb_channels[FFMIN(7, audioWrapper.dstSDLAudioSpec.channels)];
        if (!audioWrapper.dstSDLAudioSpec.channels) {
            fprintf(stderr, "No more channel combinations to tyu, audio open failed\n");
            return -1;
        }
        audioWrapper.dstChannelLayout = av_get_default_channel_layout(audioWrapper.dstNbChannels);
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

    fprintf(stdout, "%d: dstSDLAudioSpec.freq    : %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.freq);
    fprintf(stdout, "%d: dstSDLAudioSpec.channels: %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.channels);
    fprintf(stdout, "%d: dstSDLAudioSpec.format  : %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.format);
    fprintf(stdout, "%d: dstSDLAudioSpec.samples : %d\n", __LINE__, audioWrapper.dstSDLAudioSpec.samples);

    fprintf(stdout, "%d: srcSDLAudioSpec.freq    : %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.freq);
    fprintf(stdout, "%d: srcSDLAudioSpec.channels: %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.channels);
    fprintf(stdout, "%d: srcSDLAudioSpec.format  : %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.format);
    fprintf(stdout, "%d: srcSDLAudioSpec.samples : %d\n", __LINE__, audioWrapper.srcSDLAudioSpec.samples);

    audioWrapper.srcAVSampleFormat = audioWrapper.dstAVSampleFormat = AV_SAMPLE_FMT_S16;
    audioWrapper.srcSampleRate = audioWrapper.dstSampleRate = audioWrapper.srcSDLAudioSpec.freq;
    audioWrapper.srcChannelLayout = audioWrapper.dstChannelLayout;
    audioWrapper.srcNbChannels = audioWrapper.dstNbChannels = audioWrapper.srcSDLAudioSpec.channels;

    audioWrapper.father.avFormatContext->streams[audioWrapper.father.streamIndex]->discard = AVDISCARD_DEFAULT;

    return 0;
}

int initVideoSDL() {
    if (videoWrapper.srcWidth == 0 || videoWrapper.srcHeight == 0) {
        printf("%s", "没有设置srcWidth和srcHeight的值");
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

    sdlThread = SDL_CreateThread(pushEventThread, NULL, NULL);
    if (sdlThread == NULL) {
        printf("SDL_CreateThread() failed: %s\n", SDL_GetError());
        return -1;
    }
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
    if (audioWrapper.father.renderThread != NULL) {
        SDL_DetachThread(audioWrapper.father.renderThread);
        audioWrapper.father.renderThread = NULL;
    }
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
    if (audioWrapper.father.avPacket != NULL) {
        av_free_packet(audioWrapper.father.avPacket);
        audioWrapper.father.avPacket = NULL;
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
    if (videoWrapper.father.renderThread != NULL) {
        SDL_DetachThread(videoWrapper.father.renderThread);
        videoWrapper.father.renderThread = NULL;
    }
    if (videoWrapper.father.outBuffer1 != NULL) {
        av_free(videoWrapper.father.outBuffer1);
        videoWrapper.father.outBuffer1 = NULL;
    }
    if (videoWrapper.father.outBuffer2 != NULL) {
        av_free(videoWrapper.father.outBuffer2);
        videoWrapper.father.outBuffer2 = NULL;
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
    if (videoWrapper.father.avPacket != NULL) {
        av_free_packet(videoWrapper.father.avPacket);
        videoWrapper.father.avPacket = NULL;
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

int alexanderAudioPlayerWithSDL() {
    printf("%s\n", "alexanderAudioPlayerWithSDL() start");

    if (openAndFindAVFormatContext() < 0) {
        return -1;
    }

    findStreamIndex();

    if (findAndOpenAVCodec() < 0) {
        return -1;
    }

    if (createSwrContent() < 0) {
        printf("%s\n", "");
        return -1;
    }

    if (initSDL() < 0) {
        return -1;
    }
    if (initAudioSDL() < 0) {
        return -1;
    }

    initAVPacketQueue(&audioWrapper.avPacketQueue);

    // 创建子线程.audioRender和audioRender函数中的代码就是在子线程中执行的
    audioWrapper.father.renderThread = SDL_CreateThread(audioRender, NULL, NULL);

    int status = 0;
    // 如果没有下面的等待函数,那么子线程可能连执行的机会都没有
    // 等待audioRender函数里的代码执行完后才往下走,不然一直阻塞在这里
    SDL_WaitThread(audioWrapper.father.renderThread, &status);
    printf("alexanderAudioPlayerWithSDL() audio status: %d\n", status);

    close2();

    printf("%s\n", "alexanderAudioPlayerWithSDL() start");
}

/***
 run this method and playback video
 */
int alexanderVideoPlayerWithSDL() {
    printf("%s\n", "alexanderVideoPlayerWithSDL() start");
    // initAV();

    if (openAndFindAVFormatContext() < 0) {
        return -1;
    }

    //avDumpFormat();

    findStreamIndex();

    if (findAndOpenAVCodec() < 0) {
        return -1;
    }

    if (createSwrContent() < 0) {
        printf("%s\n", "");
        return -1;
    }

    if (createSwsContext() < 0) {
        printf("%s\n", "");
        return -1;
    }

    if (initSDL() < 0) {
        return -1;
    }
    if (initAudioSDL() < 0) {
        return -1;
    }
    if (initVideoSDL() < 0) {
        return -1;
    }

    // 创建子线程.audioRender和audioRender函数中的代码就是在子线程中执行的
    audioWrapper.father.renderThread = SDL_CreateThread(audioRender, NULL, NULL);
    videoWrapper.father.renderThread = SDL_CreateThread(videoRender, NULL, NULL);

    // 如果没有下面两个等待函数,那么子线程可能连执行的机会都没有
    int status = 0;
    // 等待audioRender函数里的代码执行完后才往下走,不然一直阻塞在这里
    SDL_WaitThread(audioWrapper.father.renderThread, &status);
    printf("alexanderVideoPlayerWithSDL() audio status: %d\n", status);
    // 等待videoRender函数里的代码执行完后才往下走,不然一直阻塞在这里
    SDL_WaitThread(videoWrapper.father.renderThread, &status);
    printf("alexanderVideoPlayerWithSDL() video status: %d\n", status);

    printf("%s\n", "alexanderVideoPlayerWithSDL() end");

    close2();

    return 0;
}


#endif //MYSTUDY_FFMPEG_STUDYAGAIN