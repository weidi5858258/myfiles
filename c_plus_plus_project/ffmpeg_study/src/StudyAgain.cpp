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

// 子类都要用到的部分
struct Wrapper {
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
    AVCodecParameters *avCodecParameters = NULL;
    int streamIndex = -1;
    int frameCount = 0;
    // 存储原始数据
    unsigned char *outBuffer1 = NULL;
    unsigned char *outBuffer2 = NULL;
    unsigned char *outBuffer3 = NULL;
    size_t outBufferSize = 0;
    // 视频使用到sws_scale函数时需要定义这些变量,音频也要用到
    unsigned char *srcData[4] = {NULL}, dstData[4] = {NULL};
};

struct AudioWrapper {
    struct Wrapper father;
    SwrContext *swrContext = NULL;
    // 从音频源或视频源中得到
    enum AVSampleFormat srcAVSampleFormat = AV_SAMPLE_FMT_NONE;
    enum AVSampleFormat dstAVSampleFormat = AV_SAMPLE_FMT_S16;
    int nbChannels = 0;
    int nbSamples = 0;
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
};

struct AudioWrapper audioWrapper;
struct VideoWrapper videoWrapper;

//格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
// AVFormatContext相当于Android的MediaExtractor,保存了音视频的Format信息(MediaFormat)
AVFormatContext *avFormatContext = NULL;
char *inFilePath2 = "/root/视频/tomcat_video/game_of_thrones_5_01.mp4";

///////////////////////////SDL2///////////////////////////

struct SDL_Rect sdlRect;
SDL_Window *sdlWindow = NULL;
SDL_Renderer *sdlRenderer = NULL;
SDL_Texture *sdlTexture = NULL;
SDL_Thread *sdlThread = NULL;
struct SDL_AudioSpec *sdlAudioSpec = NULL;
SDL_Event sdlEvent;

//用于线程
int threadPauseFlag = 0;
int threadExitFlag = 0;

////////////////////////////////////////////////////////////////////////////////////

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
    avFormatContext = avformat_alloc_context();
    if (avFormatContext == NULL) {
        return -1;
    }
    if (avformat_open_input(&avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }
}

/***
 找音视频流轨道
 */
int findStreamIndex() {
    if (avFormatContext == NULL) {
        return -1;
    }
    int streams = avFormatContext->nb_streams;
    for (int i = 0; i < streams; i++) {
        AVMediaType mediaType = avFormatContext->streams[i]->codec->codec_type;
        if (mediaType == AVMEDIA_TYPE_AUDIO) {
            audioWrapper.father.streamIndex = i;
        } else if (mediaType == AVMEDIA_TYPE_VIDEO) {
            videoWrapper.father.streamIndex = i;
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
    if (avFormatContext == NULL
        || (audioWrapper.father.streamIndex == -1
            && videoWrapper.father.streamIndex == -1)) {
        return -1;
    }
    // audio
    if (audioWrapper.father.streamIndex != -1) {
        audioWrapper.father.avCodecContext = avFormatContext->streams[audioWrapper.father.streamIndex]->codec;
        if (audioWrapper.father.avCodecContext != NULL) {
            audioWrapper.father.decoderAVCodec = avcodec_find_decoder(audioWrapper.father.avCodecContext->codec_id);
            if (audioWrapper.father.decoderAVCodec != NULL) {
                if (avcodec_open2(audioWrapper.father.avCodecContext, audioWrapper.father.decoderAVCodec, NULL) != 0) {
                    printf("Could not open audio codec.\n");
                    return -1;
                }
            } else {
                printf("Codec not found audio codec.\n");
                return -1;
            }
        }
    }
    // video
    if (videoWrapper.father.streamIndex != -1) {
        videoWrapper.father.avCodecContext = avFormatContext->streams[videoWrapper.father.streamIndex]->codec;
        if (videoWrapper.father.avCodecContext != NULL) {
            videoWrapper.father.decoderAVCodec = avcodec_find_decoder(videoWrapper.father.avCodecContext->codec_id);
            if (videoWrapper.father.decoderAVCodec != NULL) {
                if (avcodec_open2(videoWrapper.father.avCodecContext, videoWrapper.father.decoderAVCodec, NULL) != 0) {
                    printf("Could not open video codec.\n");
                    return -1;
                }
            } else {
                printf("Codec not found video codec.\n");
                return -1;
            }
        }
    }

    return 0;
}

int createSwrContent() {
    audioWrapper.nbChannels = audioWrapper.father.avCodecContext->channels;
    audioWrapper.nbSamples = audioWrapper.father.avCodecContext->sample_rate;
    audioWrapper.srcAVSampleFormat = audioWrapper.father.avCodecContext->sample_fmt;
    printf("nbChannels          : %d\n", audioWrapper.nbChannels);
    printf("nbSamples           : %d\n", audioWrapper.nbSamples);
    printf("srcAVSampleFormat   : %d\n", audioWrapper.srcAVSampleFormat);

    audioWrapper.father.srcAVFrame = av_frame_alloc();
    audioWrapper.father.dstAVFrame = av_frame_alloc();
    audioWrapper.father.avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(audioWrapper.father.avPacket);

    int samplesGetBufferSize = av_samples_get_buffer_size(
            audioWrapper.father.dstAVFrame->linesize,
            audioWrapper.nbChannels, audioWrapper.nbSamples, audioWrapper.srcAVSampleFormat, 1);
    printf("samplesGetBufferSize: %d\n", samplesGetBufferSize);
    audioWrapper.father.outBuffer1 = (unsigned char *) av_malloc(samplesGetBufferSize);
    av_samples_fill_arrays(
            audioWrapper.father.dstAVFrame->data, audioWrapper.father.dstAVFrame->linesize,
            audioWrapper.father.outBuffer1, audioWrapper.nbChannels,
            audioWrapper.nbSamples, audioWrapper.srcAVSampleFormat, 1);
    /***
     struct SwrContext *s,
      int64_t out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
      int64_t  in_ch_layout, enum AVSampleFormat  in_sample_fmt, int  in_sample_rate,
      int log_offset, void *log_ctx
     */
    /*audioWrapper.swrContext = swr_alloc_set_opts(
            audioWrapper.swrContext,
            );*/
    return 0;
}

int createSwsContext() {
    videoWrapper.srcWidth = videoWrapper.father.avCodecContext->width;
    videoWrapper.srcHeight = videoWrapper.father.avCodecContext->height;
    videoWrapper.srcAVPixelFormat = videoWrapper.father.avCodecContext->pix_fmt;
    // 播放视频时两个必须相同
    // 如果是转换格式后保存成一个文件,那么不能相同
    videoWrapper.dstAVPixelFormat = videoWrapper.srcAVPixelFormat;
    printf("srcWidth            : %d\n", videoWrapper.srcWidth);
    printf("srcHeight           : %d\n", videoWrapper.srcHeight);
    printf("srcAVPixelFormat    : %d\n", videoWrapper.srcAVPixelFormat);

    videoWrapper.father.srcAVFrame = av_frame_alloc();
    videoWrapper.father.dstAVFrame = av_frame_alloc();
    videoWrapper.father.avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(videoWrapper.father.avPacket);

    // srcXXX与dstXXX的参数必须要按照下面这样去设置,不然播放画面会有问题的
    // 根据视频源得到的AVPixelFormat,Width和Height计算出一帧视频所需要的空间大小
    int imageGetBufferSize = av_image_get_buffer_size(
            videoWrapper.dstAVPixelFormat, videoWrapper.srcWidth, videoWrapper.srcHeight, 1);
    printf("imageGetBufferSize  : %d\n", imageGetBufferSize);
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

//线程
int videoRefreshThread(void *opaque) {
    threadExitFlag = 0;
    threadPauseFlag = 0;
    while (!threadExitFlag) {
        if (!threadPauseFlag) {
            SDL_Event event;
            event.type = REFRESH_EVENT;
            SDL_PushEvent(&event);
        }
        SDL_Delay(40);
    }
    threadExitFlag = 0;
    threadPauseFlag = 0;
    //Break
    SDL_Event event;
    event.type = BREAK_EVENT;
    SDL_PushEvent(&event);
    printf("SDL_PushEvent BREAK_EVENT\n");

    return 0;
}

int initSDL() {
    if (videoWrapper.srcWidth == 0 || videoWrapper.srcHeight == 0) {
        printf("%s", "没有设置srcWidth和srcHeight的值");
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    //SDL 2.0 Support for multiple windows
    sdlWindow = SDL_CreateWindow(inFilePath2,
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 videoWrapper.dstWidth, videoWrapper.dstHeight,
                                 SDL_WINDOW_OPENGL);
    if (sdlWindow == NULL) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    sdlTexture = SDL_CreateTexture(sdlRenderer,
                                   SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                   videoWrapper.srcWidth, videoWrapper.srcHeight);
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = videoWrapper.srcWidth;
    sdlRect.h = videoWrapper.srcHeight;

    sdlThread = SDL_CreateThread(videoRefreshThread, NULL, NULL);
}

void close2() {
    // 清理工作
    if (sdlWindow != NULL) {
        SDL_Quit();
    }
    if (sdlThread != NULL) {
        SDL_DetachThread(sdlThread);
        sdlThread = NULL;
    }
    if (sdlAudioSpec != NULL) {
        SDL_CloseAudio();
        sdlAudioSpec = NULL;
    }
    //audio
    if (audioWrapper.father.outBuffer1 != NULL) {
        av_free(audioWrapper.father.outBuffer1);
        audioWrapper.father.outBuffer1 = NULL;
    }
    if (audioWrapper.father.outBuffer2 != NULL) {
        av_free(audioWrapper.father.outBuffer2);
        audioWrapper.father.outBuffer2 = NULL;
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
        videoWrapper.father.outBuffer2 = NULL;
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

    if (avFormatContext != NULL) {
        avformat_close_input(&avFormatContext);
        avFormatContext = NULL;
    }
    /*if (inFile != NULL) {
        fclose(inFile);
        inFile = NULL;
    }
    if (outFile != NULL) {
        fclose(outFile);
        outFile = NULL;
    }*/
}

void avDumpFormat() {
#if AV_DUMP_FORMAT
    printf("-------------File Information-------------\n");
    av_dump_format(avFormatContext, 0, inFilePath2, 0);
    printf("------------------------------------------\n");
#endif
}

int alexanderVideoPlayerWithSDL() {
    //initAV();

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

    if (createSwsContext() < 0) {
        printf("%s\n", "");
        return -1;
    }

    initSDL();
    if (sdlThread == NULL) {
        printf("%s\n", "");
        return -1;
    }

    //avDumpFormat();

    int got_picture_ptr = -1;
    for (;;) {
        //Wait
        SDL_WaitEvent(&sdlEvent);
        if (sdlEvent.type == REFRESH_EVENT) {
            while (1) {
                // 读取一帧压缩数据放到avPacket
                if (av_read_frame(avFormatContext, videoWrapper.father.avPacket) < 0) {
                    threadExitFlag = 1;
                }

                if (videoWrapper.father.avPacket->stream_index == videoWrapper.father.streamIndex) {
                    break;
                }
            }

            // 把H264数据写入fp_h264文件
            // fwrite(videoWrapper.father.avPacket->data, 1, videoWrapper.father.avPacket->size, fp_h264);

            // 解码一帧压缩数据
            if (avcodec_send_packet(videoWrapper.father.avCodecContext, videoWrapper.father.avPacket) < 0) {
                printf("Decode Error.\n");
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
            }

            //SDL---------------------------
            // sws_scale()函数不调用,直接播放srcAVFrame中的数据也可以,只是画质没有转换过的好
            SDL_UpdateTexture(sdlTexture, NULL,
                              videoWrapper.father.dstAVFrame->data[0],
                              videoWrapper.father.dstAVFrame->linesize[0]);
            SDL_RenderClear(sdlRenderer);
            //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
            SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
            SDL_RenderPresent(sdlRenderer);
            //SDL End-----------------------

            /***
            // 这种方法已经过期了
            // 对压缩数据进行解码,解码后的数据放到srcAVFrame(保存的是非压缩数据)
            int ret = avcodec_decode_video2(
                    videoWrapper.father.avCodecContext, videoWrapper.father.srcAVFrame,
                    &got_picture_ptr, videoWrapper.father.avPacket);
            if (ret < 0) {
                printf("Decode Error.\n");
                return -1;
            }
            if (got_picture_ptr) {
                // 进行格式的转换,转换后的数据放在dstAVFrame
                sws_scale(videoWrapper.swsContext,
                          (const unsigned char *const *) videoWrapper.father.srcAVFrame->data,
                          videoWrapper.father.srcAVFrame->linesize,
                          0, videoWrapper.srcHeight,
                          videoWrapper.father.dstAVFrame->data,
                          videoWrapper.father.dstAVFrame->linesize);

                //SDL---------------------------
                // sws_scale()函数不调用,直接播放srcAVFrame中的数据也可以,只是画质没有转换过的好
                SDL_UpdateTexture(sdlTexture, NULL,
                                  videoWrapper.father.dstAVFrame->data[0],
                                  videoWrapper.father.dstAVFrame->linesize[0]);
                SDL_RenderClear(sdlRenderer);
                //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
                SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
                SDL_RenderPresent(sdlRenderer);
                //SDL End-----------------------
            }*/
            av_free_packet(videoWrapper.father.avPacket);
        } else if (sdlEvent.type == SDL_KEYDOWN) {
            //Pause
            if (sdlEvent.key.keysym.sym == SDLK_SPACE) {
                threadPauseFlag = !threadPauseFlag;
            }
        } else if (sdlEvent.type == SDL_QUIT) {
            threadExitFlag = 1;
        } else if (sdlEvent.type == BREAK_EVENT) {
            break;
        }
    }// for(;;) end

    close2();

    return 0;
}


#endif //MYSTUDY_FFMPEG_STUDYAGAIN