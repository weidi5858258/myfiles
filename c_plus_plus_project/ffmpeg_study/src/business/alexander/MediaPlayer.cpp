//
// Created by root on 19-8-8.
//








/***
usleep(1000*40);//等待40毫秒
av_gettime_relative() 单位:微秒

NV21(yuv420sp)和I420(yuv420p)
NV21的排列是YYYYYYYY VUVU  -> YUV420SP
I420的排列是YYYYYYYY UU VV -> YUV420P
ffmpeg中jpeg编码输入要求是YUVJ420P格式.
1.YUV420P to YUVJ420P
2.YUVJ420P to jpeg

在FFMPEG中，图像原始数据包括两种：planar和packed。
planar就是将几个分量分开存，比如YUV420中，data[0]专门存Y，data[1]专门存U，data[2]专门存V。
而packed则是打包存，所有数据都存在data[0]中。

大多数图像处理软件在处理时是需要RGB格式的图像.
默认的视频流是压缩的YUV格式，Android下是YUV420SP.
Android提供的SurfaceView、GLSurfaceView、TextureView等控件只支持RGB格式的渲染.
RGB 转换成 YUV
Y=Y  = (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
V=Cr =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128
U=Cb =  -( 0.148 * R) - (0.291 * G) + (0.439 * B) + 128
YUV 转换成 RGB
R = 1.164 (Y - 16)  +  1.596 (V  -  128)
G = 1.164 (Y - 16)  -  0.813 (V  -  128)  -  0.391 (U  -  128)
B = 1.164 (Y - 16)  +  2.018 (U  -  128)

R=Y+1.4075*(V-128)
G=Y-0.3455*(U-128) – 0.7169*(V-128)
B=Y+1.779*(U-128)

public native int[] decodeYUV420SP(byte[] buf, int width, int height);
返回的结果是一个ARGB_8888格式的颜色数组.
mBitmap = Bitmap.createBitmap(data, width, height, Config.ARGB_8888);

保存YUV420P格式的数据，用以下代码：
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),1,output);
fwrite(pFrameYUV->data[1],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
fwrite(pFrameYUV->data[2],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
保存RGB24格式的数据，用以下代码：
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height)*3,1,output);
保存UYVY格式的数据，用以下代码：
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),2,output);

 https://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html
 图文详解YUV420数据格式
 https://blog.csdn.net/cgwang_1580/article/details/79595958
 常用视频像素格式NV12、NV2、I420、YV12、YUYV

 int av_image_get_buffer_size(enum AVPixelFormat pix_fmt, int width, int height, int align);
 函数的作用是通过指定像素格式、图像宽、图像高来计算所需的内存大小
 重点说明一个参数align:此参数是设定内存对齐的对齐数,也就是按多大的字节进行内存对齐.
 比如设置为1,表示按1字节对齐,那么得到的结果就是与实际的内存大小一样.
 再比如设置为4,表示按4字节对齐.也就是内存的起始地址必须是4的整倍数.

 int av_image_alloc(
     uint8_t *pointers[4],
     int linesizes[4],
     int w,
     int h,
     enum AVPixelFormat pix_fmt,
     int align);
 pointers[4]：保存图像通道的地址.
 如果是RGB,则前三个指针分别指向R,G,B的内存地址,第四个指针保留不用.
 linesizes[4]：保存图像每个通道的内存对齐的步长,即一行的对齐内存的宽度,此值大小等于图像宽度.
 w:       要申请内存的图像宽度.
 h:       要申请内存的图像高度.
 pix_fmt: 要申请内存的图像的像素格式.
 align:   用于内存对齐的值.
 返回值：所申请的内存空间的总大小.如果是负值,表示申请失败.

 int av_image_fill_arrays(
     uint8_t *dst_data[4],
     int dst_linesize[4],
     const uint8_t *src,
     enum AVPixelFormat pix_fmt,
     int width,
     int height,
     int align);
 av_image_fill_arrays()函数自身不具备内存申请的功能,
 此函数类似于格式化已经申请的内存,即通过av_image_alloc()函数申请的内存空间.
 dst_data[4]:     [out]对申请的内存格式化为三个通道后,分别保存其地址.
 dst_linesize[4]: [out]格式化的内存的步长(即内存对齐后的宽度).
 *src:            [in]av_image_alloc()函数申请的内存地址.
 pix_fmt:         [in]申请 src内存时的像素格式.
 width:           [in]申请src内存时指定的宽度.
 height:          [in]申请scr内存时指定的高度.
 align:           [in]申请src内存时指定的对齐字节数.

int sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
                                          const int srcStride[], int srcSliceY, int srcSliceH,
                                          uint8_t *const dst[], const int dstStride[]);
// c参数指定使用包含图像数据格式转换信息的struct SwsContext对象
// srcSlice参数用于指定的原图像数据缓冲区地址
// srcStride参数用于指定原数据缓冲区一行数据的大小
// srcSliceY参数用于指定从原图像的第几行开始转换
// srcSliceH参数用于指定转换到原图像的第几行.
// dst参数用于指定存放生成图像数据的缓冲区地址
// dstStride参数指定存放生成图像的一行数据大小的缓冲区

typedef struct AVFrame {
    #define AV_NUM_DATA_POINTERS 8
    // 此指针数组是用于存放数据缓冲区地址，因有可能是平面的数据，所有用了多个指针变量存放不同分量的数据缓冲区
    uint8_t *data[AV_NUM_DATA_POINTERS];
    // 存放每个缓冲区的一行数据的字节数
    int linesize[AV_NUM_DATA_POINTERS];
    ...
}AVFrame

yuv422转换成420p:
1. 创建AVFrame对象
    AVFrame  *frm422 = av_frame_alloc();
    AVFrame  *frm420p = av_frame_alloc();
2. 绑定数据缓冲区
    av_image_fill_arrays(frm422->data, frm422->linesize, buf_422, AV_PIX_FMT_YUYV422, w, h, 16);
    av_image_fill_arrays(frm420p->data, frm420p->linesize, buf_420p, AV_PIX_FMT_YUV420P, w, h, 16);
3. 指定原数据格式，分辨率及目标数据格式，分辨率
    struct SwsContext *sws = sws_getContext(w, h, AV_PIX_FMT_YUYV422,
                                            w,h, AV_PIX_FMT_YUV420P,
                                            SWS_BILINEAR,
                                            NULL, NULL, NULL);
4. 转换并调整分辨率
    int ret = sws_scale(sws, frm422->data, frm422->linesize, 0, h, frm420p->data, frm420p->linesize);
5. 回收空间
    av_frame_free(&frm422);
    av_frame_free(&frm420p);
    sws_freeContext(sws);

argb转换成yuv420p:

int MyDataCovert::argb32Toyuv420p(uint8_t *buf_argb, uint8_t *buf_420p, int w, int h)
{
    AVFrame  *frmArgb = av_frame_alloc();
    AVFrame  *frm420p = av_frame_alloc();

    //绑定数据缓冲区
    avpicture_fill((AVPicture *)frmArgb, buf_argb, AV_PIX_FMT_BGRA, w, h);
    avpicture_fill((AVPicture *)frm420p, buf_420p, AV_PIX_FMT_YUV420P, w, h);

    //指定原数据格式，分辨率及目标数据格式，分辨率
    struct SwsContext *sws = sws_getContext(w, h, AV_PIX_FMT_BGRA,
                                            w,h, AV_PIX_FMT_YUV420P,
                                            SWS_BILINEAR,
                                            NULL, NULL, NULL);

    //转换
    int ret = sws_scale(sws, frmArgb->data, frmArgb->linesize, 0, h, frm420p->data, frm420p->linesize);
    av_frame_free(&frmArgb);
    av_frame_free(&frm420p);
    sws_freeContext(sws);
    return  (ret == h) ? 0 : -1;
}

// 在jni层或者其他cpp文件中创建线程是不行的
pthread_t audioReadDataThread, audioHandleDataThread;
// 创建线程
pthread_create(&audioReadDataThread, NULL, readData, audioWrapper->father);
pthread_create(&audioHandleDataThread, NULL, handleAudioData, NULL);
// 等待线程执行完
pthread_join(audioReadDataThread, NULL);
pthread_join(audioHandleDataThread, NULL);
// 取消线程
//pthread_cancel(audioReadDataThread);
//pthread_cancel(audioHandleDataThread);

pthread_t videoReadDataThread, videoHandleDataThread;
// 创建线程
pthread_create(&videoReadDataThread, NULL, readData, videoWrapper->father);
pthread_create(&videoHandleDataThread, NULL, handleVideoData, NULL);
// 等待线程执行完
pthread_join(videoReadDataThread, NULL);
pthread_join(videoHandleDataThread, NULL);
// 取消线程
//pthread_cancel(videoReadDataThread);
//pthread_cancel(videoHandleDataThread);
 */

#include "MediaPlayer.h"

#define LOG "player_alexander"

static char inFilePath[2048];
AVFormatContext *avFormatContext = NULL;
struct AudioWrapper *audioWrapper = NULL;
struct VideoWrapper *videoWrapper = NULL;
static pthread_mutex_t readLockMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t readLockCondition = PTHREAD_COND_INITIALIZER;
bool isLocal = false;
bool isReading = false;
bool isAudioHandling = false;
bool isInterrupted = false;
bool runOneTime = true;
// seek时间
int64_t timeStamp = -1;
static long curProgress = 0;
static long preProgress = 0;
// 视频播放时每帧之间的暂停时间,单位为ms
static int videoSleepTime = 11;

double TIME_DIFFERENCE = 1.000000;// 0.180000
// 当前音频时间戳
static double audioPts = 0;
// 当前视频时间戳
static double videoPts = 0;
// 上一个时间戳
static double videoPtsPre = 0;

extern int use_mode;

namespace alexander_media {


#define RUN_COUNTS 88
    int runCounts = 0;
    double averageTimeDiff = 0;
    double timeDiff[RUN_COUNTS];

    int64_t startReadTime = -1;
    int64_t endReadTime = -1;

    char *getStrAVPixelFormat(AVPixelFormat format);

    void closeOther();

    void notifyToRead() {
        pthread_mutex_lock(&readLockMutex);
        pthread_cond_signal(&readLockCondition);
        pthread_mutex_unlock(&readLockMutex);
    }

    void notifyToReadWait() {
        pthread_mutex_lock(&readLockMutex);
        pthread_cond_wait(&readLockCondition, &readLockMutex);
        pthread_mutex_unlock(&readLockMutex);
    }

    // 通知读线程开始读(其中有一个队列空的情况)
    void notifyToRead(Wrapper *wrapper) {
        pthread_mutex_lock(&wrapper->readLockMutex);
        pthread_cond_signal(&wrapper->readLockCondition);
        pthread_mutex_unlock(&wrapper->readLockMutex);
    }

    // 通知读线程开始等待(队列都满的情况)
    void notifyToReadWait(Wrapper *wrapper) {
        pthread_mutex_lock(&wrapper->readLockMutex);
        pthread_cond_wait(&wrapper->readLockCondition, &wrapper->readLockMutex);
        pthread_mutex_unlock(&wrapper->readLockMutex);
    }

    // 通知处理线程开始处理(几种情况需要唤醒:开始播放时,cache缓存时)
    void notifyToHandle(Wrapper *wrapper) {
        pthread_mutex_lock(&wrapper->handleLockMutex);
        pthread_cond_signal(&wrapper->handleLockCondition);
        pthread_mutex_unlock(&wrapper->handleLockMutex);
    }

    // 通知处理线程开始等待
    void notifyToHandleWait(Wrapper *wrapper) {
        pthread_mutex_lock(&wrapper->handleLockMutex);
        pthread_cond_wait(&wrapper->handleLockCondition, &wrapper->handleLockMutex);
        pthread_mutex_unlock(&wrapper->handleLockMutex);
    }

    int read_thread_interrupt_cb(void *opaque) {
        // 必须通过传参方式进行判断,不能用全局变量判断
        AudioWrapper *audioWrapper = (AudioWrapper *) opaque;
        endReadTime = av_gettime_relative();
        if (!audioWrapper->father->isReading) {
            printf("read_thread_interrupt_cb() return 1 退出了\n");
            return 1;
        } else if ((audioWrapper->father->isPausedForCache || !audioWrapper->father->isStarted)
                   && startReadTime > 0
                   && (endReadTime - startReadTime) > MAX_RELATIVE_TIME) {
            printf("read_thread_interrupt_cb() return 1 超时了\n");
            isInterrupted = true;
            onError(0x101, "读取数据超时");
            return 1;
        }
        return 0;
    }

    // 已经不需要调用了
    void initAV() {
        av_register_all();
        // 用于从网络接收数据,如果不是网络接收数据,可不用（如本例可不用）
        avcodec_register_all();

        // 注册设备的函数,如用获取摄像头数据或音频等,需要此函数先注册
        // avdevice_register_all();
        // 注册复用器和编解码器,所有的使用ffmpeg,首先必须调用这个函数
        avformat_network_init();

        printf("ffmpeg [av_version_info()] version: %s\n", av_version_info());

        readLockMutex = PTHREAD_MUTEX_INITIALIZER;
        readLockCondition = PTHREAD_COND_INITIALIZER;
        videoSleepTime = 11;
        preProgress = 0;
        audioPts = 0.0;
        videoPts = 0.0;
        videoPtsPre = 0;
        runOneTime = true;
        runCounts = 0;
        averageTimeDiff = 0.0;
        memset(timeDiff, '0', sizeof(timeDiff));
        isInterrupted = false;
        startReadTime = -1;
        endReadTime = -1;
    }

    void initAudio() {
        if (audioWrapper != NULL && audioWrapper->father != NULL) {
            av_free(audioWrapper->father);
            audioWrapper->father = NULL;
        }
        if (audioWrapper != NULL) {
            av_free(audioWrapper);
            audioWrapper = NULL;
        }
        // 这里是先有儿子,再有父亲了.其实应该先构造父亲,再把父亲信息传给儿子.
        audioWrapper = (struct AudioWrapper *) av_mallocz(sizeof(struct AudioWrapper));
        memset(audioWrapper, 0, sizeof(struct AudioWrapper));
        audioWrapper->father = (struct Wrapper *) av_mallocz(sizeof(struct Wrapper));
        memset(audioWrapper->father, 0, sizeof(struct Wrapper));

        audioWrapper->father->type = TYPE_AUDIO;
        if (isLocal) {
            audioWrapper->father->list1LimitCounts = MAX_AVPACKET_COUNT_AUDIO_LOCAL;
        } else {
            audioWrapper->father->list1LimitCounts = MAX_AVPACKET_COUNT_AUDIO_HTTP;
        }
        audioWrapper->father->list2LimitCounts = MAX_AVPACKET_COUNT;
        printf("initAudio() list1LimitCounts: %d\n", audioWrapper->father->list1LimitCounts);
        printf("initAudio() list2LimitCounts: %d\n", audioWrapper->father->list2LimitCounts);
        audioWrapper->father->streamIndex = -1;
        audioWrapper->father->readFramesCount = 0;
        audioWrapper->father->handleFramesCount = 0;
        audioWrapper->father->isStarted = false;
        audioWrapper->father->isReading = true;
        audioWrapper->father->isHandling = true;
        audioWrapper->father->isPausedForUser = false;
        audioWrapper->father->isPausedForCache = false;
        audioWrapper->father->isPausedForSeek = false;
        audioWrapper->father->needToSeek = false;
        audioWrapper->father->allowDecode = false;
        audioWrapper->father->isHandleList1Full = false;
//        audioWrapper->father->isReadList2Full = false;
        audioWrapper->father->list1 = new std::list<AVPacket>();
        audioWrapper->father->list2 = new std::list<AVPacket>();

        audioWrapper->father->duration = -1;
        audioWrapper->father->timestamp = -1;
        audioWrapper->father->readLockMutex = PTHREAD_MUTEX_INITIALIZER;
        audioWrapper->father->readLockCondition = PTHREAD_COND_INITIALIZER;
        audioWrapper->father->handleLockMutex = PTHREAD_MUTEX_INITIALIZER;
        audioWrapper->father->handleLockCondition = PTHREAD_COND_INITIALIZER;

        audioWrapper->dstSampleRate = 44100;
        audioWrapper->dstAVSampleFormat = AV_SAMPLE_FMT_S16;
        audioWrapper->dstChannelLayout = AV_CH_LAYOUT_STEREO;
    }

    void initVideo() {
        if (videoWrapper != NULL && videoWrapper->father != NULL) {
            av_free(videoWrapper->father);
            videoWrapper->father = NULL;
        }
        if (videoWrapper != NULL) {
            av_free(videoWrapper);
            videoWrapper = NULL;
        }

        videoWrapper = (struct VideoWrapper *) av_mallocz(sizeof(struct VideoWrapper));
        memset(videoWrapper, 0, sizeof(struct VideoWrapper));
        videoWrapper->father = (struct Wrapper *) av_mallocz(sizeof(struct Wrapper));
        memset(videoWrapper->father, 0, sizeof(struct Wrapper));

        videoWrapper->father->type = TYPE_VIDEO;
        if (isLocal) {
            videoWrapper->father->list1LimitCounts = MAX_AVPACKET_COUNT_VIDEO_LOCAL;
        } else {
            videoWrapper->father->list1LimitCounts = MAX_AVPACKET_COUNT_VIDEO_HTTP;
        }
        videoWrapper->father->list2LimitCounts = MAX_AVPACKET_COUNT;
        printf("initVideo() list1LimitCounts: %d\n", videoWrapper->father->list1LimitCounts);
        printf("initVideo() list2LimitCounts: %d\n", videoWrapper->father->list2LimitCounts);
        videoWrapper->father->streamIndex = -1;
        videoWrapper->father->readFramesCount = 0;
        videoWrapper->father->handleFramesCount = 0;
        videoWrapper->father->isStarted = false;
        videoWrapper->father->isReading = true;
        videoWrapper->father->isHandling = true;
        videoWrapper->father->isPausedForUser = false;
        videoWrapper->father->isPausedForCache = false;
        videoWrapper->father->isPausedForSeek = false;
        videoWrapper->father->needToSeek = false;
        videoWrapper->father->allowDecode = false;
        videoWrapper->father->isHandleList1Full = false;
//        videoWrapper->father->isReadList2Full = false;
        videoWrapper->father->list1 = new std::list<AVPacket>();
        videoWrapper->father->list2 = new std::list<AVPacket>();

        videoWrapper->father->duration = -1;
        videoWrapper->father->timestamp = -1;
        videoWrapper->father->readLockMutex = PTHREAD_MUTEX_INITIALIZER;
        videoWrapper->father->readLockCondition = PTHREAD_COND_INITIALIZER;
        videoWrapper->father->handleLockMutex = PTHREAD_MUTEX_INITIALIZER;
        videoWrapper->father->handleLockCondition = PTHREAD_COND_INITIALIZER;

        // Android支持的目标像素格式
        // AV_PIX_FMT_RGB32
        // AV_PIX_FMT_RGBA
        videoWrapper->dstAVPixelFormat = AV_PIX_FMT_RGBA;
        videoWrapper->srcWidth = 0;
        videoWrapper->srcHeight = 0;
    }

    int openAndFindAVFormatContext() {
        printf("openAndFindAVFormatContext() start\n");
        if (avFormatContext != NULL) {
            printf("openAndFindAVFormatContext() videoAVFormatContext isn't NULL\n");
            avformat_free_context(avFormatContext);
            avFormatContext = NULL;
        }
        avFormatContext = avformat_alloc_context();
        if (avFormatContext == NULL) {
            printf("videoAVFormatContext is NULL.\n");
            return -1;
        }
        if (!isLocal) {
            avFormatContext->interrupt_callback.callback = read_thread_interrupt_cb;
            avFormatContext->interrupt_callback.opaque = audioWrapper;
            /*AVDictionary *options = NULL;
            av_dict_set(&options, "stimeout", "10000000", 0);*/
            int64_t startTime = av_gettime_relative();
            if (avformat_open_input(&avFormatContext,
                                    inFilePath,
                                    NULL, NULL) != 0) {
                // 这里就是某些视频初始化失败的地方
                printf("Couldn't open input stream.\n");
                return -1;
            }
            int64_t endTime = av_gettime_relative();
            printf("openAndFindAVFormatContext() avformat_open_input: %ld\n",
                   (long) ((endTime - startTime) / 1000));
        } else {
            if (avformat_open_input(&avFormatContext,
                                    inFilePath,
                                    NULL, NULL) != 0) {
                printf("Couldn't open input stream.\n");
                return -1;
            }
        }
        if (avformat_find_stream_info(avFormatContext, NULL) != 0) {
            printf("Couldn't find stream information.\n");
            return -1;
        }
        printf("openAndFindAVFormatContext() end\n");
        return 0;
    }

    int findStreamIndex() {
        if (avFormatContext == NULL) {
            return -1;
        }
        printf("findStreamIndex() start\n");
        // stream counts
        int streams = avFormatContext->nb_streams;
        printf("Stream counts   : %d\n", streams);
        for (int i = 0; i < streams; i++) {
            // 得到AVCodecParameters
            AVCodecParameters *avCodecParameters = avFormatContext->streams[i]->codecpar;
            if (avCodecParameters != NULL) {
                AVMediaType mediaType = avCodecParameters->codec_type;
                switch (mediaType) {
                    case AVMEDIA_TYPE_AUDIO: {
                        audioWrapper->father->streamIndex = i;
                        audioWrapper->father->avCodecParameters = avCodecParameters;
                        printf("audioStreamIndex: %d\n", audioWrapper->father->streamIndex);
                        break;
                    }
                    case AVMEDIA_TYPE_VIDEO: {
                        videoWrapper->father->streamIndex = i;
                        videoWrapper->father->avCodecParameters = avCodecParameters;
                        printf("videoStreamIndex: %d\n", videoWrapper->father->streamIndex);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        if (audioWrapper->father->streamIndex != -1
            && videoWrapper->father->streamIndex != -1) {
            use_mode = USE_MODE_MEDIA;
            videoWrapper->father->avStream = avFormatContext->streams[videoWrapper->father->streamIndex];
            audioWrapper->father->avStream = avFormatContext->streams[audioWrapper->father->streamIndex];
            printf("findStreamIndex() USE_MODE_MEDIA\n");
        } else if (audioWrapper->father->streamIndex == -1
                   && videoWrapper->father->streamIndex != -1) {
            use_mode = USE_MODE_ONLY_VIDEO;
            videoWrapper->father->avStream = avFormatContext->streams[videoWrapper->father->streamIndex];
            printf("findStreamIndex() USE_MODE_ONLY_VIDEO\n");
        } else if (audioWrapper->father->streamIndex != -1
                   && videoWrapper->father->streamIndex == -1) {
            use_mode = USE_MODE_ONLY_AUDIO;
            audioWrapper->father->avStream = avFormatContext->streams[audioWrapper->father->streamIndex];
            printf("findStreamIndex() USE_MODE_ONLY_AUDIO\n");
        } else {
            printf("Didn't find audio or video stream.\n");
            return -1;
        }

        printf("findStreamIndex() end\n");
        return 0;
    }

    int findAndOpenAVCodecForAudio() {
        if (use_mode == USE_MODE_ONLY_VIDEO) {
            return 0;
        }
        printf("findAndOpenAVCodecForAudio() start\n");
        // audio
        if (audioWrapper->father->streamIndex != -1) {
            // 获取音频解码器
            // 先通过AVCodecParameters找到AVCodec
            AVCodecID codecID = audioWrapper->father->avCodecParameters->codec_id;
            // audio是没有下面这些东西的
            switch (codecID) {
                case AV_CODEC_ID_HEVC: {
                    printf("findAndOpenAVCodecForAudio() hevc_mediacodec\n");
                    // 硬解码265
                    audioWrapper->father->decoderAVCodec = avcodec_find_decoder_by_name(
                            "hevc_mediacodec");
                    break;
                }
                case AV_CODEC_ID_H264: {
                    printf("findAndOpenAVCodecForAudio() h264_mediacodec\n");
                    // 硬解码264
                    audioWrapper->father->decoderAVCodec = avcodec_find_decoder_by_name(
                            "h264_mediacodec");
                    break;
                }
                case AV_CODEC_ID_MPEG4: {
                    printf("findAndOpenAVCodecForAudio() mpeg4_mediacodec\n");
                    // 硬解码mpeg4
                    audioWrapper->father->decoderAVCodec = avcodec_find_decoder_by_name(
                            "mpeg4_mediacodec");
                    break;
                }
                default: {
                    printf("findAndOpenAVCodecForAudio() codecID\n");
                    // 软解
                    // audioWrapper->father->decoderAVCodec = avcodec_find_decoder(codecID);
                    break;
                }
            }
            audioWrapper->father->decoderAVCodec = avcodec_find_decoder(codecID);
            if (audioWrapper->father->decoderAVCodec != NULL) {
                // 获取解码器上下文
                // 再通过AVCodec得到AVCodecContext
                audioWrapper->father->avCodecContext = avcodec_alloc_context3(
                        audioWrapper->father->decoderAVCodec);
                if (audioWrapper->father->avCodecContext != NULL) {
                    // 关联操作
                    if (avcodec_parameters_to_context(
                            audioWrapper->father->avCodecContext,
                            audioWrapper->father->avCodecParameters) < 0) {
                        return -1;
                    } else {
                        // 打开AVCodec
                        if (avcodec_open2(
                                audioWrapper->father->avCodecContext,
                                audioWrapper->father->decoderAVCodec, NULL) != 0) {
                            printf("Could not open audio codec.\n");
                            return -1;
                        }
                    }
                }
            }
        }

        printf("findAndOpenAVCodecForAudio() end\n");
        return 0;
    }

    int findAndOpenAVCodecForVideo() {
        if (use_mode == USE_MODE_ONLY_AUDIO) {
            return 0;
        }
        printf("findAndOpenAVCodecForVideo() start\n");
        // video
        if (videoWrapper->father->streamIndex != -1) {
            // avcodec_find_decoder_by_name
            AVCodecID codecID = videoWrapper->father->avCodecParameters->codec_id;
            switch (codecID) {
                case AV_CODEC_ID_HEVC: {
                    printf("findAndOpenAVCodecForVideo() hevc_mediacodec\n");
                    // 硬解码265
                    videoWrapper->father->decoderAVCodec = avcodec_find_decoder_by_name(
                            "hevc_mediacodec");
                    break;
                }
                case AV_CODEC_ID_H264: {
                    printf("findAndOpenAVCodecForVideo() h264_mediacodec\n");
                    // 硬解码264
                    videoWrapper->father->decoderAVCodec = avcodec_find_decoder_by_name(
                            "h264_mediacodec");
                    break;
                }
                case AV_CODEC_ID_MPEG4: {
                    printf("findAndOpenAVCodecForVideo() mpeg4_mediacodec\n");
                    // 硬解码mpeg4
                    videoWrapper->father->decoderAVCodec = avcodec_find_decoder_by_name(
                            "mpeg4_mediacodec");
                    break;
                }
                default: {
                    printf("findAndOpenAVCodecForVideo() codecID\n");
                    // 软解
                    // videoWrapper->father->decoderAVCodec = avcodec_find_decoder(codecID);
                    break;
                }
            }
            // 有相应的so库时这句就不要执行了
            videoWrapper->father->decoderAVCodec = avcodec_find_decoder(codecID);
            if (videoWrapper->father->decoderAVCodec != NULL) {
                videoWrapper->father->avCodecContext = avcodec_alloc_context3(
                        videoWrapper->father->decoderAVCodec);
                if (videoWrapper->father->avCodecContext != NULL) {
                    if (avcodec_parameters_to_context(
                            videoWrapper->father->avCodecContext,
                            videoWrapper->father->avCodecParameters) < 0) {
                        return -1;
                    } else {
                        if (avcodec_open2(
                                videoWrapper->father->avCodecContext,
                                videoWrapper->father->decoderAVCodec, NULL) != 0) {
                            printf("Could not open video codec.\n");
                            return -1;
                        }
                    }
                }
            }
        }

        printf("findAndOpenAVCodecForVideo() end\n");
        return 0;
    }

    int createSwrContent() {
        if (use_mode == USE_MODE_ONLY_VIDEO) {
            return 0;
        }
        printf("createSwrContent() start\n");
        // src
        audioWrapper->srcSampleRate = audioWrapper->father->avCodecContext->sample_rate;
        audioWrapper->srcNbSamples = audioWrapper->father->avCodecContext->frame_size;
        audioWrapper->srcNbChannels = audioWrapper->father->avCodecContext->channels;
        audioWrapper->srcChannelLayout = audioWrapper->father->avCodecContext->channel_layout;
        audioWrapper->srcAVSampleFormat = audioWrapper->father->avCodecContext->sample_fmt;
        printf("---------------------------------\n");
        printf("srcSampleRate       : %d\n", audioWrapper->srcSampleRate);
        printf("srcNbChannels       : %d\n", audioWrapper->srcNbChannels);
        printf("srcAVSampleFormat   : %d\n", audioWrapper->srcAVSampleFormat);
        printf("srcNbSamples        : %d\n", audioWrapper->srcNbSamples);
        printf("srcChannelLayout1   : %d\n", audioWrapper->srcChannelLayout);
        // 有些视频从源视频中得到的channel_layout与使用函数得到的channel_layout结果是一样的
        // 但是还是要使用函数得到的channel_layout为好
        audioWrapper->srcChannelLayout = av_get_default_channel_layout(audioWrapper->srcNbChannels);
        printf("srcChannelLayout2   : %d\n", audioWrapper->srcChannelLayout);
        printf("---------------------------------\n");
        if (audioWrapper->srcNbSamples <= 0) {
            audioWrapper->srcNbSamples = 1024;
        }
        // dst
        // Android中跟音频有关的参数: dstSampleRate dstNbChannels 位宽
        // dstSampleRate, dstAVSampleFormat和dstChannelLayout指定
        // 然后通过下面处理后在Java端就能创建AudioTrack对象了
        // 不然像有些5声道,6声道就创建不了,因此没有声音
        audioWrapper->dstSampleRate = audioWrapper->srcSampleRate;
        audioWrapper->dstNbSamples = audioWrapper->srcNbSamples;
        audioWrapper->dstNbChannels = av_get_channel_layout_nb_channels(
                audioWrapper->dstChannelLayout);

        printf("dstSampleRate       : %d\n", audioWrapper->dstSampleRate);
        printf("dstNbChannels       : %d\n", audioWrapper->dstNbChannels);
        printf("dstAVSampleFormat   : %d\n", audioWrapper->dstAVSampleFormat);
        printf("dstNbSamples        : %d\n", audioWrapper->dstNbSamples);
        printf("---------------------------------\n");

        audioWrapper->swrContext = swr_alloc();
        swr_alloc_set_opts(audioWrapper->swrContext,
                           audioWrapper->dstChannelLayout,  // out_ch_layout
                           audioWrapper->dstAVSampleFormat, // out_sample_fmt
                           audioWrapper->dstSampleRate,     // out_sample_rate
                           audioWrapper->srcChannelLayout,  // in_ch_layout
                           audioWrapper->srcAVSampleFormat, // in_sample_fmt
                           audioWrapper->srcSampleRate,     // in_sample_rate
                           0,                               // log_offset
                           NULL);                           // log_ctx
        if (audioWrapper->swrContext == NULL) {
            printf("%s\n", "createSwrContent() swrContext is NULL");
            return -1;
        }

        int ret = swr_init(audioWrapper->swrContext);
        if (ret != 0) {
            printf("%s\n", "createSwrContent() swrContext swr_init failed");
            return -1;
        } else {
            printf("%s\n", "createSwrContent() swrContext swr_init success");
        }

        // 这个对应关系还不知道怎么弄
        int audioFormat = 2;
        switch (audioWrapper->dstAVSampleFormat) {
            case AV_SAMPLE_FMT_NONE: {
                break;
            }
            case AV_SAMPLE_FMT_U8: {
                break;
            }
            case AV_SAMPLE_FMT_S16: {
                audioFormat = 2;
                break;
            }
            case AV_SAMPLE_FMT_S32: {
                break;
            }
            case AV_SAMPLE_FMT_S64: {
                break;
            }
            case AV_SAMPLE_FMT_FLT: {
                break;
            }
            case AV_SAMPLE_FMT_DBL: {
                break;
            }
            case AV_SAMPLE_FMT_U8P: {
                break;
            }
            case AV_SAMPLE_FMT_S16P: {
                break;
            }
            case AV_SAMPLE_FMT_S32P: {
                break;
            }
            case AV_SAMPLE_FMT_S64P: {
                break;
            }
            case AV_SAMPLE_FMT_FLTP: {
                break;
            }
            case AV_SAMPLE_FMT_DBLP: {
                break;
            }
            case AV_SAMPLE_FMT_NB: {
                break;
            }
            default:
                break;
        }

        printf("%s\n", "createSwrContent() createAudioTrack start");
        createAudioTrack(audioWrapper->dstSampleRate,
                         audioWrapper->dstNbChannels,
                         audioFormat);
        printf("%s\n", "createSwrContent() createAudioTrack end");

        // avPacket ---> decodedAVFrame ---> dstAVFrame ---> 播放声音
        audioWrapper->decodedAVFrame = av_frame_alloc();
        // 16bit 44100 PCM 数据,16bit是2个字节
        audioWrapper->father->outBuffer1 = (unsigned char *) av_malloc(MAX_AUDIO_FRAME_SIZE);
        audioWrapper->father->outBufferSize = MAX_AUDIO_FRAME_SIZE;

        printf("createSwrContent() end\n");
        return 0;
    }

    int createSwsContext() {
        if (use_mode == USE_MODE_ONLY_AUDIO) {
            return 0;
        }
        printf("createSwsContext() start\n");
        videoWrapper->srcWidth = videoWrapper->father->avCodecContext->width;
        videoWrapper->srcHeight = videoWrapper->father->avCodecContext->height;
        videoWrapper->srcAVPixelFormat = videoWrapper->father->avCodecContext->pix_fmt;
        //onChangeWindow(videoWrapper->srcWidth, videoWrapper->srcHeight);
        printf("---------------------------------\n");
        printf("srcWidth            : %d\n", videoWrapper->srcWidth);
        printf("srcHeight           : %d\n", videoWrapper->srcHeight);
        printf("srcAVPixelFormat    : %d %s\n",
               videoWrapper->srcAVPixelFormat, getStrAVPixelFormat(videoWrapper->srcAVPixelFormat));
        printf("dstAVPixelFormat    : %d %s\n",
               videoWrapper->dstAVPixelFormat, getStrAVPixelFormat(videoWrapper->dstAVPixelFormat));
        videoWrapper->dstWidth = videoWrapper->srcWidth;
        videoWrapper->dstHeight = videoWrapper->srcHeight;
        videoWrapper->srcArea = videoWrapper->srcWidth * videoWrapper->srcHeight;
        videoWrapper->dstArea = videoWrapper->srcArea;

        // decodedAVFrame为解码后的数据
        // avPacket ---> decodedAVFrame ---> rgbAVFrame ---> 渲染画面
        videoWrapper->decodedAVFrame = av_frame_alloc();
        videoWrapper->rgbAVFrame = av_frame_alloc();

        int imageGetBufferSize = av_image_get_buffer_size(
                videoWrapper->dstAVPixelFormat, videoWrapper->srcWidth, videoWrapper->srcHeight, 1);
        videoWrapper->father->outBufferSize = imageGetBufferSize * sizeof(unsigned char);
        printf("imageGetBufferSize1 : %d\n", imageGetBufferSize);
        printf("imageGetBufferSize2 : %d\n", videoWrapper->father->outBufferSize);
        videoWrapper->father->outBuffer1 =
                (unsigned char *) av_malloc(videoWrapper->father->outBufferSize);
        int imageFillArrays = av_image_fill_arrays(
                videoWrapper->rgbAVFrame->data,
                videoWrapper->rgbAVFrame->linesize,
                videoWrapper->father->outBuffer1,
                videoWrapper->dstAVPixelFormat,
                videoWrapper->srcWidth,
                videoWrapper->srcHeight,
                1);
        if (imageFillArrays < 0) {
            printf("imageFillArrays     : %d\n", imageFillArrays);
            return -1;
        }
        // 由于解码出来的帧格式不是RGBA,在渲染之前需要进行格式转换
        // 现在swsContext知道程序员想要得到什么样的像素格式了
        videoWrapper->swsContext = sws_getContext(
                videoWrapper->srcWidth, videoWrapper->srcHeight, videoWrapper->srcAVPixelFormat,
                videoWrapper->srcWidth, videoWrapper->srcHeight, videoWrapper->dstAVPixelFormat,
                // SWS_BICUBIC SWS_BILINEAR 原分辨率与目标分辨率不一致时使用哪种算法来调整.
                SWS_BICUBIC,
                NULL, NULL,
                // 指定调整图像缩放的算法,可设为NULL使用默认算法.
                NULL);
        if (videoWrapper->swsContext == NULL) {
            printf("%s\n", "videoSwsContext is NULL.");
            return -1;
        }
        printf("---------------------------------\n");

        printf("createSwsContext() end\n");
        return 0;
    }

    int seekToImpl() {
        // seekTo
        printf("seekToImpl() sleep start\n");
        while (!audioWrapper->father->needToSeek
               || !videoWrapper->father->needToSeek) {
            if (!audioWrapper->father->isHandling || !videoWrapper->father->isHandling) {
                return 0;
            }
            av_usleep(1000);
        }
        printf("seekToImpl() sleep end\n");
        printf("seekToImpl() audio list2 size: %d\n", audioWrapper->father->list2->size());
        printf("seekToImpl() video list2 size: %d\n", videoWrapper->father->list2->size());
        if (audioWrapper->father->list2->size() != 0) {
            std::list<AVPacket>::iterator iter;
            for (iter = audioWrapper->father->list2->begin();
                 iter != audioWrapper->father->list2->end();
                 iter++) {
                AVPacket avPacket = *iter;
                av_packet_unref(&avPacket);
            }
            audioWrapper->father->list2->clear();
        }
        if (videoWrapper->father->list2->size() != 0) {
            std::list<AVPacket>::iterator iter;
            for (iter = videoWrapper->father->list2->begin();
                 iter != videoWrapper->father->list2->end();
                 iter++) {
                AVPacket avPacket = *iter;
                av_packet_unref(&avPacket);
            }
            videoWrapper->father->list2->clear();
        }
        printf("seekToImpl() av_seek_frame start\n");
        printf("seekToImpl() timestamp: %ld\n", (long) timeStamp);
        //printf("seekToImpl() timestamp: %"PRIu64"\n", timestamp);
        av_seek_frame(avFormatContext, -1,
                      timeStamp * AV_TIME_BASE,
                      AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
        // 清空解码器的缓存
        avcodec_flush_buffers(audioWrapper->father->avCodecContext);
        avcodec_flush_buffers(videoWrapper->father->avCodecContext);
        timeStamp = -1;
        preProgress = 0;
        videoPtsPre = 0;
        audioWrapper->father->isPausedForSeek = false;
        videoWrapper->father->isPausedForSeek = false;
        audioWrapper->father->isStarted = false;
        videoWrapper->father->isStarted = false;
        printf("seekToImpl() av_seek_frame end\n");
        printf("==================================================================\n");
        return 0;
    }

    int readDataImpl(Wrapper *wrapper, AVPacket *srcAVPacket, AVPacket *copyAVPacket) {
        wrapper->readFramesCount++;
        // 复制数据
        av_packet_ref(copyAVPacket, srcAVPacket);
        av_packet_unref(srcAVPacket);

        // 保存数据
        pthread_mutex_lock(&wrapper->readLockMutex);
        wrapper->list2->push_back(*copyAVPacket);
        size_t list2Size = wrapper->list2->size();
        pthread_mutex_unlock(&wrapper->readLockMutex);

        if (!isLocal) {
            if (wrapper->type == TYPE_AUDIO) {
                if (list2Size % 500 == 0) {
                    printf("readDataImpl() audio list2Size: %d\n", list2Size);
                }
            } else {
                if (list2Size % 500 == 0) {
                    printf("readDataImpl() video list2Size: %d\n", list2Size);
                }
            }
        }

        if (!wrapper->isHandleList1Full
            && list2Size == wrapper->list1LimitCounts) {
            /***
             什么时候走这里?
             1.开始播放缓冲好的时候
             2.因为缓冲原因
             3.seek后
             */
            // 下面两个都不行
            // std::move(wrapper->list2->begin(), wrapper->list2->end(), std::back_inserter(wrapper->list1));
            // wrapper->list1->swap((std::list<AVPacket> &) wrapper->list2);
            // 把list2中的内容全部复制给list1
            pthread_mutex_lock(&wrapper->readLockMutex);
            wrapper->list1->clear();
            wrapper->list1->assign(wrapper->list2->begin(), wrapper->list2->end());
            wrapper->list2->clear();
            wrapper->isHandleList1Full = true;
            pthread_mutex_unlock(&wrapper->readLockMutex);
            notifyToHandle(wrapper);

            if (wrapper->type == TYPE_AUDIO) {
                printf("readDataImpl() audio 填满数据了\n");
            } else {
                printf("readDataImpl() video 填满数据了\n");
            }
        } else if (wrapper->type == TYPE_VIDEO
                   && list2Size >= wrapper->list2LimitCounts) {
            printf("readDataImpl() audio list1: %d\n", audioWrapper->father->list1->size());
            printf("readDataImpl() audio list2: %d\n", audioWrapper->father->list2->size());
            printf("readDataImpl() video list1: %d\n", videoWrapper->father->list1->size());
            printf("readDataImpl() video list2: %d\n", videoWrapper->father->list2->size());
            if (audioWrapper->father->list2->size() > audioWrapper->father->list1LimitCounts) {
                printf("readDataImpl() notifyToReadWait start\n");
                notifyToReadWait(videoWrapper->father);
                printf("readDataImpl() notifyToReadWait end\n");
            }
        }
        return 0;
    }

    void *readData(void *opaque) {
        printf("%s\n", "readData() start");

        if (audioWrapper == NULL
            || audioWrapper->father == NULL
            || videoWrapper == NULL
            || videoWrapper->father == NULL) {
            return NULL;
        } else if (!audioWrapper->father->isReading
                   || !audioWrapper->father->isHandling
                   || !videoWrapper->father->isReading
                   || !videoWrapper->father->isHandling) {
            closeAudio();
            closeVideo();
            closeOther();
            onFinished();
            printf("%s\n", "readData() finish");
            return NULL;
        }

        AVPacket *srcAVPacket = av_packet_alloc();
        AVPacket *copyAVPacket = av_packet_alloc();

        // seekTo
        if (timeStamp > 0) {
            printf("readData() timeStamp: %ld\n", (long) timeStamp);
            audioWrapper->father->needToSeek = true;
            videoWrapper->father->needToSeek = true;
            audioWrapper->father->isPausedForSeek = true;
            videoWrapper->father->isPausedForSeek = true;
        }

        isReading = true;
        /***
         有几种情况:
         1.list1中先存满n个,然后list2多次存取
         2.list1中先存满n个,然后list2一次性存满
         3.list1中还没满n个文件就读完了
         */
        for (;;) {
            // exit
            if (!audioWrapper->father->isReading
                && !videoWrapper->father->isReading) {
                // for (;;) end
                break;
            }

            if ((audioWrapper->father->isPausedForSeek || videoWrapper->father->isPausedForSeek)
                && timeStamp >= 0) {
                // seekTo
                seekToImpl();
            }

            startReadTime = av_gettime_relative();
            int readFrame = av_read_frame(avFormatContext, srcAVPacket);
            endReadTime = av_gettime_relative();

            if (isInterrupted) {
                stop();
                break;
            }

            // 0 if OK, < 0 on error or end of file
            if (readFrame < 0) {
                if (readFrame != -12 && readFrame != AVERROR_EOF) {
                    printf("readData() readFrame  : %d\n", readFrame);
                    continue;
                }
                // readData() AVERROR_EOF readFrame: -12 (Cannot allocate memory)
                // readData() AVERROR_EOF readFrame: -1094995529
                // readData() AVERROR_EOF readFrame: -1414092869 超时
                // readData() AVERROR_EOF readFrame: -541478725 文件已经读完了
                printf("readData() AVERROR_EOF: %d\n", AVERROR_EOF);
                printf("readData() readFrame  : %d\n", readFrame);
                printf("readData() audio list2: %d\n", audioWrapper->father->list2->size());
                printf("readData() video list2: %d\n", videoWrapper->father->list2->size());

                // 读到文件末尾了
                audioWrapper->father->isReading = false;
                videoWrapper->father->isReading = false;
                audioWrapper->father->isHandleList1Full = true;
                videoWrapper->father->isHandleList1Full = true;
                // 说明歌曲长度比较短,达不到"规定值",因此处理数据线程还在等待
                notifyToHandle(audioWrapper->father);
                notifyToHandle(videoWrapper->father);

                // 不退出线程
                printf("readData() notifyToReadWait start\n");
                notifyToReadWait();
                printf("readData() notifyToReadWait end\n");
                if (audioWrapper->father->isPausedForSeek
                    || videoWrapper->father->isPausedForSeek) {
                    printf("readData() start seek\n");
                    audioWrapper->father->isReading = true;
                    videoWrapper->father->isReading = true;
                    continue;
                } else {
                    // for (;;) end
                    break;
                }
            }// 文件已读完

            if (srcAVPacket->stream_index == audioWrapper->father->streamIndex) {
                if (audioWrapper->father->isReading) {
                    readDataImpl(audioWrapper->father, srcAVPacket, copyAVPacket);
                }
            } else if (srcAVPacket->stream_index == videoWrapper->father->streamIndex) {
                if (videoWrapper->father->isReading) {
                    readDataImpl(videoWrapper->father, srcAVPacket, copyAVPacket);
                }
            }
        }// for(;;) end

        if (srcAVPacket != NULL) {
            av_packet_unref(srcAVPacket);
            srcAVPacket = NULL;
        }

        isReading = false;

        printf("%s\n", "readData() end");
        return NULL;
    }

    int handleAudioDataImpl(AVStream *stream, AVFrame *decodedAVFrame) {
        // 转换音频
        int ret = swr_convert(
                audioWrapper->swrContext,
                // 输出缓冲区
                &audioWrapper->father->outBuffer1,
                // 每通道采样的可用空间量
                MAX_AUDIO_FRAME_SIZE,
                // 输入缓冲区
                (const uint8_t **) decodedAVFrame->data,
                // 一个通道中可用的输入采样数量
                decodedAVFrame->nb_samples);
        if (ret >= 0) {
            audioWrapper->father->isStarted = true;
            while (!videoWrapper->father->isStarted) {
                if (audioWrapper->father->isPausedForSeek
                    || !audioWrapper->father->isHandling) {
                    return 0;
                }
                av_usleep(1000);
            }
            if (runOneTime
                && audioWrapper->father->isStarted
                && videoWrapper->father->isStarted) {
                printf("handleAudioDataImpl() 音视频都已经准备好,开始播放!!!\n");
                runOneTime = false;
                // 回调(通知到java层)
                onPlayed();
            }

            ////////////////////////////////////////////////////////////////////

            audioPts = decodedAVFrame->pts * av_q2d(stream->time_base);
            curProgress = (long) audioPts;// 秒
            if (curProgress > preProgress) {
                preProgress = curProgress;
                onProgressUpdated(curProgress);
            }

            ////////////////////////////////////////////////////////////////////

            // 获取给定音频参数所需的缓冲区大小
            int out_buffer_size = av_samples_get_buffer_size(
                    NULL,
                    // 输出的声道个数
                    audioWrapper->dstNbChannels,
                    // 一个通道中音频采样数量
                    decodedAVFrame->nb_samples,
                    // 输出采样格式16bit
                    audioWrapper->dstAVSampleFormat,
                    // 缓冲区大小对齐（0 = 默认值,1 = 不对齐）
                    1);
            write(audioWrapper->father->outBuffer1, 0, out_buffer_size);
        } else {
            printf("audio 转换时出错 %d", ret);
        }
        return ret;
    }

    int handleVideoDataImpl(AVStream *stream, AVFrame *decodedAVFrame) {
        videoWrapper->father->isStarted = true;
        while (!audioWrapper->father->isStarted) {
            if (videoWrapper->father->isPausedForSeek
                || !videoWrapper->father->isHandling) {
                printf("handleVideoDataImpl() videoWrapper->father->isStarted return\n");
                return 0;
            }
            av_usleep(1000);
        }

        /***
         以音频为基准,同步视频到音频
         1.视频慢了则加快播放或丢掉部分视频帧
         2.视频快了则延迟播放,继续渲染上一帧
         音频需要正常播放才是好的体验
         */
        videoPts = decodedAVFrame->pts * av_q2d(stream->time_base);
        if (videoPts > 0 && audioPts > 0) {
            double tempTimeDifference = videoPts - audioPts;
            if (runCounts < RUN_COUNTS) {
                if (tempTimeDifference > 0) {
                    timeDiff[runCounts++] = tempTimeDifference;
                    //printf("handleVideoDataImpl() video - audio      : %lf\n", tempTimeDifference);
                }
            } else if (runCounts == RUN_COUNTS) {
                runCounts++;
                double totleTimeDiff = 0;
                for (int i = 0; i < RUN_COUNTS; i++) {
                    if (videoWrapper->father->isPausedForSeek
                        || !videoWrapper->father->isHandling) {
                        printf("handleVideoDataImpl() RUN_COUNTS return\n");
                        return 0;
                    }
                    totleTimeDiff += timeDiff[i];
                }
                averageTimeDiff = totleTimeDiff / RUN_COUNTS;
                printf("handleVideoDataImpl() averageTimeDiff    : %lf\n", averageTimeDiff);
            }
            if (tempTimeDifference < 0) {
                // 正常情况下videoTimeDifference比audioTimeDifference大一些
                // 如果发现小了,说明视频播放慢了,应丢弃这些帧
                // break后videoTimeDifference增长的速度会加快
                // videoPts = audioPts + averageTimeDiff;
                return 0;
            }

            if (tempTimeDifference > 2.000000) {
                // 不好的现象.为什么会出现这种情况还不知道?
                //printf("handleVideoDataImpl() audioTimeDifference: %lf\n", audioPts);
                //printf("handleVideoDataImpl() videoTimeDifference: %lf\n", videoPts);
                //printf("handleVideoDataImpl() video - audio      : %lf\n", tempTimeDifference);
                videoPts = audioPts + averageTimeDiff;
                //printf("handleVideoDataImpl() videoTimeDifference: %lf\n", videoPts);
            }
            // 如果videoTimeDifference比audioTimeDifference大出了一定的范围
            // 那么说明视频播放快了,应等待音频
            while (videoPts - audioPts > TIME_DIFFERENCE) {
                if (videoWrapper->father->isPausedForSeek
                    || !videoWrapper->father->isHandling) {
                    printf("handleVideoDataImpl() TIME_DIFFERENCE return\n");
                    return 0;
                }
                av_usleep(1000);
            }
        }

        // 渲染画面
        if (videoWrapper->father->isHandling) {
            // 3.lock锁定下一个即将要绘制的Surface
            //ANativeWindow_lock(pANativeWindow, &mANativeWindow_Buffer, NULL);

            // 把decodedAVFrame的数据经过格式转换后保存到rgbAVFrame中
            sws_scale(videoWrapper->swsContext,
                      (uint8_t const *const *) decodedAVFrame->data,
                      decodedAVFrame->linesize,
                      0,
                      videoWrapper->srcHeight,
                      videoWrapper->rgbAVFrame->data,
                      videoWrapper->rgbAVFrame->linesize);
            // 这段代码非常关键,还看不懂啥意思
            // 把rgbAVFrame里面的数据复制到outBuffer中就能渲染画面了
            uint8_t *src = videoWrapper->rgbAVFrame->data[0];
            // 一行的长度
            int srcStride = videoWrapper->rgbAVFrame->linesize[0];
            /*uint8_t *dst = (uint8_t *) mANativeWindow_Buffer.bits;
            int dstStride = mANativeWindow_Buffer.stride * 4;
            // 由于window的stride和帧的stride不同,因此需要逐行复制
            for (int h = 0; h < videoWrapper->srcHeight; h++) {
                memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
            }*/

            ////////////////////////////////////////////////////////

            // timeDifference = 0.040000
            // 单位: 毫秒
            int tempSleep = ((int) ((videoPts - videoPtsPre) * 1000)) - 30;
            if (videoSleepTime != tempSleep) {
                videoSleepTime = tempSleep;
                //printf("handleVideoDataImpl() videoSleepTime     : %d\n", videoSleepTime);
            }
            if (videoSleepTime < 12 && videoSleepTime > 0) {
                videoSleep(videoSleepTime);
            } else {
                if (videoSleepTime > 0) {
                    // 好像是个比较合理的值
                    videoSleep(11);
                }
                // videoSleepTime <= 0时不需要sleep
            }
            videoPtsPre = videoPts;

            ////////////////////////////////////////////////////////

            // 6.unlock绘制
            //ANativeWindow_unlockAndPost(pANativeWindow);
        }
    }

    int handleDataClose(Wrapper *wrapper) {
        // 让"读线程"退出
        notifyToRead();

        if (wrapper->type == TYPE_AUDIO) {
            printf("handleData() for (;;) audio end\n");
            isAudioHandling = false;
            printf("%s\n", "handleData() audio end");
        } else {
            printf("handleData() for (;;) video end\n");
            while (isReading || isAudioHandling) {
                av_usleep(1000);
            }
            printf("%s\n", "handleData() video end");
            closeAudio();
            closeVideo();
            closeOther();
            // 必须保证每次退出都要执行到
            onFinished();
            printf("%s\n", "Safe Exit");
        }
    }

    void *handleData(void *opaque) {
        if (!opaque) {
            return NULL;
        }
        Wrapper *wrapper = NULL;
        int *type = (int *) opaque;
        if (*type == TYPE_AUDIO) {
            wrapper = audioWrapper->father;
        } else {
            wrapper = videoWrapper->father;
        }
        if (!wrapper) {
            printf("%s\n", "wrapper is NULL");
            return NULL;
        }

        // 线程等待
        if (wrapper->type == TYPE_AUDIO) {
            printf("handleData() wait() audio start\n");
        } else {
            printf("handleData() wait() video start\n");
        }
        notifyToHandleWait(wrapper);
        if (wrapper->type == TYPE_AUDIO) {
            printf("handleData() wait() audio end\n");
        } else {
            printf("handleData() wait() video end\n");
        }

        if (!wrapper->isHandling) {
            handleDataClose(wrapper);
            return NULL;
        }

        if (wrapper->type == TYPE_AUDIO) {
            printf("handleData() for (;;) audio start\n");
            isAudioHandling = true;
        } else {
            printf("handleData() ANativeWindow_setBuffersGeometry() start\n");
            // 2.设置缓冲区的属性（宽、高、像素格式）,像素格式要和SurfaceView的像素格式一直
            /*ANativeWindow_setBuffersGeometry(pANativeWindow,
                                             videoWrapper->srcWidth,
                                             videoWrapper->srcHeight,
                                             WINDOW_FORMAT_RGBA_8888);*/
            printf("handleData() ANativeWindow_setBuffersGeometry() end\n");
            printf("handleData() for (;;) video start\n");
        }

        AVStream *stream = avFormatContext->streams[wrapper->streamIndex];
        AVPacket *srcAVPacket = av_packet_alloc();
        AVPacket *copyAVPacket = av_packet_alloc();
        // decodedAVFrame为解码后的数据
        AVFrame *decodedAVFrame = NULL;
        if (wrapper->type == TYPE_AUDIO) {
            decodedAVFrame = audioWrapper->decodedAVFrame;
        } else {
            decodedAVFrame = videoWrapper->decodedAVFrame;
        }

        int ret = 0;
        bool maybeHasException = false;
        for (;;) {
            if (!wrapper->isHandling) {
                // for (;;) end
                break;
            }

            // region 暂停装置

            if (wrapper->isPausedForUser
                || wrapper->isPausedForCache
                || wrapper->isPausedForSeek) {
                bool isPausedForUser = wrapper->isPausedForUser;
                bool isPausedForSeek = wrapper->isPausedForSeek;
                if (isPausedForSeek) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() wait() Seek  audio start\n");
                        printf("handleData() audio list1 size: %d\n", wrapper->list1->size());
                    } else {
                        printf("handleData() wait() Seek  video start\n");
                        printf("handleData() video list1 size: %d\n", wrapper->list1->size());
                    }
                    if (wrapper->list1->size() != 0) {
                        std::list<AVPacket>::iterator iter;
                        for (iter = wrapper->list1->begin();
                             iter != wrapper->list1->end();
                             iter++) {
                            AVPacket avPacket = *iter;
                            av_packet_unref(&avPacket);
                        }
                        wrapper->list1->clear();
                    }
                    wrapper->isHandleList1Full = false;
                    wrapper->needToSeek = true;
                } else if (isPausedForUser) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() wait() User  audio start\n");
                    } else {
                        printf("handleData() wait() User  video start\n");
                    }
                } else {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() wait() Cache audio start 被动暂停\n");
                    } else {
                        printf("handleData() wait() Cache video start 被动暂停\n");
                    }
                }
                notifyToHandleWait(wrapper);
                if (wrapper->isPausedForUser
                    || wrapper->isPausedForCache
                    || wrapper->isPausedForSeek) {
                    continue;
                }
                if (isPausedForSeek) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() wait() Seek  audio end\n");
                    } else {
                        printf("handleData() wait() Seek  video end\n");
                    }
                } else if (isPausedForUser) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() wait() User  audio end\n");
                    } else {
                        printf("handleData() wait() User  video end\n");
                    }
                } else {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() wait() Cache audio end   被动暂停\n");
                    } else {
                        printf("handleData() wait() Cache video end   被动暂停\n");
                    }
                }
            }// 暂停装置 end

            // endregion

            if (!wrapper->isHandling) {
                // for (;;) end
                break;
            }

            /*if (!isLocal) {
                if (wrapper->list1->size() >= wrapper->list1LimitCounts) {
                    wrapper->startHandleTime = av_gettime_relative();
                    maybeHasException = true;
                }
            }*/

            // region 从队列中取出一个AVPacket

            wrapper->allowDecode = false;
            if (wrapper->list1->size() > 0) {
                srcAVPacket = &wrapper->list1->front();
                // 内容copy
                av_packet_ref(copyAVPacket, srcAVPacket);
                av_packet_unref(srcAVPacket);
                wrapper->list1->pop_front();
                wrapper->handleFramesCount++;
                wrapper->allowDecode = true;
            }

            if (!isLocal) {
                size_t list1Size = wrapper->list1->size();
                if (wrapper->type == TYPE_AUDIO) {
                    if (list1Size % 1000 == 0) {
                        printf("handleData()   audio list1Size: %d\n", list1Size);
                    }
                } else {
                    if (list1Size % 1000 == 0) {
                        printf("handleData()   video list1Size: %d\n", list1Size);
                    }
                }
            }

            // endregion

            if (!isLocal) {
                if (maybeHasException && wrapper->list1->size() == 0) {
                    wrapper->endHandleTime = av_gettime_relative();
                    /*if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() audio      handleTime: %ld\n",
                             (long) (wrapper->endHandleTime - wrapper->startHandleTime));
                    } else {
                        printf("handleData() video      handleTime: %ld\n",
                             (long) (wrapper->endHandleTime - wrapper->startHandleTime));
                    }*/
                    // 如果不是本地视频,从一千个左右的数据到0个数据的时间不超过30秒,那么就有问题了.
                    if ((wrapper->endHandleTime - wrapper->startHandleTime) <= 30000000) {
                        printf("handleData() maybeHasException\n");
                        //onError(0x102, "播放时发生异常");
                        stop();
                        break;
                    } else {
                        maybeHasException = false;
                    }
                }
            }

            // region 复制数据

            if (wrapper->isReading) {
                if (wrapper->list1->size() == 0) {
                    wrapper->isHandleList1Full = false;
                    if (wrapper->list2->size() > 0) {
                        pthread_mutex_lock(&wrapper->readLockMutex);
                        wrapper->list1->clear();
                        wrapper->list1->assign(wrapper->list2->begin(), wrapper->list2->end());
                        wrapper->list2->clear();
                        wrapper->isHandleList1Full = true;
                        pthread_mutex_unlock(&wrapper->readLockMutex);

                        printf("===================================================\n");
                        if (wrapper->type == TYPE_AUDIO) {
                            printf("handleData() audio 接下去要处理的数据有 list1: %d\n",
                                   wrapper->list1->size());
                            printf("handleData() audio                   list2: %d\n",
                                   wrapper->list2->size());
                            printf("handleData() video 接下去要处理的数据有 list1: %d\n",
                                   videoWrapper->father->list1->size());
                            printf("handleData() video                   list2: %d\n",
                                   videoWrapper->father->list2->size());
                        } else {
                            printf("handleData() video 接下去要处理的数据有 list1: %d\n",
                                   wrapper->list1->size());
                            printf("handleData() video                   list2: %d\n",
                                   wrapper->list2->size());
                            printf("handleData() audio 接下去要处理的数据有 list1: %d\n",
                                   audioWrapper->father->list1->size());
                            printf("handleData() audio                   list2: %d\n",
                                   audioWrapper->father->list2->size());
                        }
                        printf("===================================================\n");
                    }
                    notifyToRead(videoWrapper->father);
                }
            } else {
                if (wrapper->list1->size() > 0) {
                    // 还有数据,先用完再说
                } else {
                    if (wrapper->list2->size() > 0) {
                        // 把剩余的数据全部复制过来
                        pthread_mutex_lock(&wrapper->readLockMutex);
                        wrapper->list1->clear();
                        wrapper->list1->assign(wrapper->list2->begin(), wrapper->list2->end());
                        wrapper->list2->clear();
                        pthread_mutex_unlock(&wrapper->readLockMutex);

                        printf("===================================================\n");
                        if (wrapper->type == TYPE_AUDIO) {
                            printf("handleData() audio 最后要处理的数据还有 list1: %d\n",
                                   wrapper->list1->size());
                            printf("handleData() audio                   list2: %d\n",
                                   wrapper->list2->size());
                            printf("handleData() video                   list1: %d\n",
                                   videoWrapper->father->list1->size());
                            printf("handleData() video                   list2: %d\n",
                                   videoWrapper->father->list2->size());
                        } else {
                            printf("handleData() video 最后要处理的数据还有 list1: %d\n",
                                   wrapper->list1->size());
                            printf("handleData() video                   list2: %d\n",
                                   wrapper->list2->size());
                            printf("handleData() audio                   list1: %d\n",
                                   audioWrapper->father->list1->size());
                            printf("handleData() audio                   list2: %d\n",
                                   audioWrapper->father->list2->size());
                        }
                        printf("===================================================\n");
                    } else {
                        wrapper->isHandling = false;
                    }
                }
            }

            // endregion

            // region 缓冲处理

            if (wrapper->isReading
                && wrapper->isHandling
                && !wrapper->isHandleList1Full
                && wrapper->list1->size() == 0
                && wrapper->list2->size() == 0) {

                printf("---------------------------------------------------\n");
                if (wrapper->type == TYPE_AUDIO) {
                    printf("handleData() audio                   list1: %d\n",
                           wrapper->list1->size());
                    printf("handleData() audio                   list2: %d\n",
                           wrapper->list2->size());
                    printf("handleData() video                   list1: %d\n",
                           videoWrapper->father->list1->size());
                    printf("handleData() video                   list2: %d\n",
                           videoWrapper->father->list2->size());
                } else {
                    printf("handleData() video                   list1: %d\n",
                           wrapper->list1->size());
                    printf("handleData() video                   list2: %d\n",
                           wrapper->list2->size());
                    printf("handleData() audio                   list1: %d\n",
                           audioWrapper->father->list1->size());
                    printf("handleData() audio                   list2: %d\n",
                           audioWrapper->father->list2->size());
                }
                printf("---------------------------------------------------\n");

                // 开始暂停
                onPaused();

                // 通知"读"
                notifyToRead(videoWrapper->father);
                if (wrapper->type == TYPE_AUDIO) {
                    // 音频Cache引起的暂停
                    audioWrapper->father->isPausedForCache = true;

                    // 让视频也同时暂停
                    videoWrapper->father->isPausedForCache = true;
                    // 音频自身暂停
                    printf("handleData() wait() Cache audio start 主动暂停\n");
                    notifyToHandleWait(audioWrapper->father);
                    if (wrapper->isPausedForSeek) {
                        audioWrapper->father->isPausedForCache = false;
                        videoWrapper->father->isPausedForCache = false;
                        continue;
                    }
                    printf("handleData() wait() Cache audio end   主动暂停\n");

                    audioWrapper->father->isPausedForCache = false;
                    // 通知视频结束暂停
                    videoWrapper->father->isPausedForCache = false;
                    notifyToHandle(videoWrapper->father);
                } else {
                    // 视频Cache引起的暂停
                    videoWrapper->father->isPausedForCache = true;

                    // 让音频也同时暂停
                    audioWrapper->father->isPausedForCache = true;
                    // 视频自身暂停
                    printf("handleData() wait() Cache video start 主动暂停\n");
                    notifyToHandleWait(videoWrapper->father);
                    if (wrapper->isPausedForSeek) {
                        audioWrapper->father->isPausedForCache = false;
                        videoWrapper->father->isPausedForCache = false;
                        continue;
                    }
                    printf("handleData() wait() Cache video end   主动暂停\n");

                    videoWrapper->father->isPausedForCache = false;
                    // 通知音频结束暂停
                    audioWrapper->father->isPausedForCache = false;
                    notifyToHandle(audioWrapper->father);
                }

                // 开始播放
                onPlayed();

                printf("***************************************************\n");
                if (wrapper->type == TYPE_AUDIO) {
                    printf("handleData() audio                   list1: %d\n",
                           wrapper->list1->size());
                    printf("handleData() audio                   list2: %d\n",
                           wrapper->list2->size());
                    printf("handleData() video                   list1: %d\n",
                           videoWrapper->father->list1->size());
                    printf("handleData() video                   list2: %d\n",
                           videoWrapper->father->list2->size());
                } else {
                    printf("handleData() video                   list1: %d\n",
                           wrapper->list1->size());
                    printf("handleData() video                   list2: %d\n",
                           wrapper->list2->size());
                    printf("handleData() audio                   list1: %d\n",
                           audioWrapper->father->list1->size());
                    printf("handleData() audio                   list2: %d\n",
                           audioWrapper->father->list2->size());
                }
                printf("***************************************************\n");
            }

            // endregion

            // region 解码过程

            if (!wrapper->isHandling) {
                // for (;;) end
                break;
            }

            if (!wrapper->allowDecode) {
                continue;
            }

            ret = avcodec_send_packet(wrapper->avCodecContext, copyAVPacket);
            av_packet_unref(copyAVPacket);
            switch (ret) {
                case AVERROR(EAGAIN):
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() audio avcodec_send_packet   ret: %d\n", ret);// -11
                    } else {
                        printf("handleData() video avcodec_send_packet   ret: %d\n", ret);
                    }
                    break;
                case AVERROR(EINVAL):
                case AVERROR(ENOMEM):
                case AVERROR_EOF:
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("audio 发送数据包到解码器时出错 %d", ret);// -22
                    } else {
                        printf("video 发送数据包到解码器时出错 %d", ret);
                    }
                    //wrapper->isHandling = false;
                    break;
                case 0:
                    break;
                default:
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("audio 发送数据包时出现异常 %d", ret);// -1094995529
                    } else {
                        printf("video 发送数据包时出现异常 %d", ret);
                    }
                    break;
            }// switch (ret) end

            if (!wrapper->isHandling) {
                // for (;;) end
                break;
            }

            if (ret != 0) {
                continue;
            }

            ret = avcodec_receive_frame(wrapper->avCodecContext, decodedAVFrame);
            switch (ret) {
                // 输出是不可用的,必须发送新的输入
                case AVERROR(EAGAIN):
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("handleData() audio avcodec_receive_frame ret: %d\n", ret);
                    } else {
                        printf("handleData() video avcodec_receive_frame ret: %d\n", ret);
                    }
                    break;
                case AVERROR(EINVAL):
                    // codec打不开,或者是一个encoder
                case AVERROR_EOF:
                    // 已经完全刷新,不会再有输出帧了
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("audio 从解码器接收解码帧时出错 %d", ret);
                    } else {
                        printf("video 从解码器接收解码帧时出错 %d", ret);
                    }
                    //wrapper->isHandling = false;
                    break;
                case 0: {
                    // 解码成功,返回一个输出帧
                    break;
                }
                default:
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("audio 接收解码帧时出现异常 %d", ret);
                    } else {
                        printf("video 接收解码帧时出现异常 %d", ret);
                    }
                    break;
            }// switch (ret) end

            if (!wrapper->isHandling) {
                // for (;;) end
                break;
            }

            if (ret != 0) {
                continue;
            }

            // endregion

            ///////////////////////////////////////////////////////////////////

            // 播放声音和渲染画面
            if (wrapper->type == TYPE_AUDIO) {
                handleAudioDataImpl(stream, decodedAVFrame);
            } else {
                handleVideoDataImpl(stream, decodedAVFrame);
            }

            ///////////////////////////////////////////////////////////////////

            // 设置结束标志
            if (!wrapper->isReading
                && wrapper->list1->size() == 0
                && wrapper->list2->size() == 0) {
                wrapper->isHandling = false;
            }
        }//for(;;) end

        if (srcAVPacket != NULL) {
            av_packet_unref(srcAVPacket);
            // app crash 上面的copyAVPacket调用却没事,why
            // av_packet_free(&srcAVPacket);
            srcAVPacket = NULL;
        }
        if (copyAVPacket != NULL) {
            av_packet_free(&copyAVPacket);
            copyAVPacket = NULL;
        }

        handleDataClose(wrapper);

        return NULL;
    }

    void closeAudio() {
        // audio
        if (audioWrapper == NULL
            || audioWrapper->father == NULL) {
            return;
        }
        printf("%s\n", "closeAudio() start");
        if (audioWrapper->father->outBuffer1 != NULL) {
            av_free(audioWrapper->father->outBuffer1);
            audioWrapper->father->outBuffer1 = NULL;
        }
        if (audioWrapper->father->outBuffer2 != NULL) {
            av_free(audioWrapper->father->outBuffer2);
            audioWrapper->father->outBuffer2 = NULL;
        }
        if (audioWrapper->father->outBuffer3 != NULL) {
            av_free(audioWrapper->father->outBuffer3);
            audioWrapper->father->outBuffer3 = NULL;
        }
        /*if (audioWrapper->father->srcData[0] != NULL) {
            av_freep(&audioWrapper->father->srcData[0]);
            audioWrapper->father->srcData[0] = NULL;
        }
        if (audioWrapper->father->dstData[0] != NULL) {
            av_freep(&audioWrapper->father->dstData[0]);
            audioWrapper->father->dstData[0] = NULL;
        }*/
        if (audioWrapper->swrContext != NULL) {
            swr_free(&audioWrapper->swrContext);
            audioWrapper->swrContext = NULL;
        }
        if (audioWrapper->decodedAVFrame != NULL) {
            av_frame_free(&audioWrapper->decodedAVFrame);
            audioWrapper->decodedAVFrame = NULL;
        }
        if (audioWrapper->father->avCodecContext != NULL) {
            avcodec_close(audioWrapper->father->avCodecContext);
            av_free(audioWrapper->father->avCodecContext);
            audioWrapper->father->avCodecContext = NULL;
        }
        pthread_mutex_destroy(&audioWrapper->father->readLockMutex);
        pthread_cond_destroy(&audioWrapper->father->readLockCondition);
        pthread_mutex_destroy(&audioWrapper->father->handleLockMutex);
        pthread_cond_destroy(&audioWrapper->father->handleLockCondition);

        if (audioWrapper->father->list1->size() != 0) {
            printf("closeAudio() list1 is not empty, %d\n", audioWrapper->father->list1->size());
            std::list<AVPacket>::iterator iter;
            for (iter = audioWrapper->father->list1->begin();
                 iter != audioWrapper->father->list1->end();
                 iter++) {
                AVPacket avPacket = *iter;
                av_packet_unref(&avPacket);
            }
        }
        if (audioWrapper->father->list2->size() != 0) {
            printf("closeAudio() list2 is not empty, %d\n", audioWrapper->father->list2->size());
            std::list<AVPacket>::iterator iter;
            for (iter = audioWrapper->father->list2->begin();
                 iter != audioWrapper->father->list2->end();
                 iter++) {
                AVPacket avPacket = *iter;
                av_packet_unref(&avPacket);
            }
        }
        delete (audioWrapper->father->list1);
        delete (audioWrapper->father->list2);
        audioWrapper->father->list1 = NULL;
        audioWrapper->father->list2 = NULL;

        av_free(audioWrapper->father);
        audioWrapper->father = NULL;
        av_free(audioWrapper);
        audioWrapper = NULL;
        printf("%s\n", "closeAudio() end");
    }

    void closeVideo() {
        /*if (pANativeWindow != NULL) {
            // 7.释放资源
            ANativeWindow_release(pANativeWindow);
            pANativeWindow = NULL;
        }*/
        pthread_mutex_destroy(&readLockMutex);
        pthread_cond_destroy(&readLockCondition);
        // video
        if (videoWrapper == NULL
            || videoWrapper->father == NULL) {
            return;
        }
        printf("%s\n", "closeVideo() start");
        if (videoWrapper->father->outBuffer1 != NULL) {
            av_free(videoWrapper->father->outBuffer1);
            videoWrapper->father->outBuffer1 = NULL;
        }
        if (videoWrapper->father->outBuffer2 != NULL) {
            av_free(videoWrapper->father->outBuffer2);
            videoWrapper->father->outBuffer2 = NULL;
        }
        if (videoWrapper->father->outBuffer3 != NULL) {
            av_free(videoWrapper->father->outBuffer3);
            videoWrapper->father->outBuffer3 = NULL;
        }
        /*if (videoWrapper->father->srcData[0] != NULL) {
            av_freep(&videoWrapper->father->srcData[0]);
            videoWrapper->father->srcData[0] = NULL;
        }
        if (videoWrapper->father->dstData[0] != NULL) {
            av_freep(&videoWrapper->father->dstData[0]);
            videoWrapper->father->dstData[0] = NULL;
        }*/
        if (videoWrapper->swsContext != NULL) {
            sws_freeContext(videoWrapper->swsContext);
            videoWrapper->swsContext = NULL;
        }
        if (videoWrapper->decodedAVFrame != NULL) {
            av_frame_free(&videoWrapper->decodedAVFrame);
            videoWrapper->decodedAVFrame = NULL;
        }
        if (videoWrapper->rgbAVFrame != NULL) {
            av_frame_free(&videoWrapper->rgbAVFrame);
            videoWrapper->rgbAVFrame = NULL;
        }
        if (videoWrapper->father->avCodecContext != NULL) {
            avcodec_close(videoWrapper->father->avCodecContext);
            av_free(videoWrapper->father->avCodecContext);
            videoWrapper->father->avCodecContext = NULL;
        }

        pthread_mutex_destroy(&videoWrapper->father->readLockMutex);
        pthread_cond_destroy(&videoWrapper->father->readLockCondition);
        pthread_mutex_destroy(&videoWrapper->father->handleLockMutex);
        pthread_cond_destroy(&videoWrapper->father->handleLockCondition);

        if (videoWrapper->father->list1->size() != 0) {
            printf("closeVideo() list1 is not empty, %d\n", videoWrapper->father->list1->size());
            std::list<AVPacket>::iterator iter;
            for (iter = videoWrapper->father->list1->begin();
                 iter != videoWrapper->father->list1->end();
                 iter++) {
                AVPacket avPacket = *iter;
                av_packet_unref(&avPacket);
            }
        }
        if (videoWrapper->father->list2->size() != 0) {
            printf("closeVideo() list2 is not empty, %d\n", videoWrapper->father->list2->size());
            std::list<AVPacket>::iterator iter;
            for (iter = videoWrapper->father->list2->begin();
                 iter != videoWrapper->father->list2->end();
                 iter++) {
                AVPacket avPacket = *iter;
                av_packet_unref(&avPacket);
            }
        }
        delete (videoWrapper->father->list1);
        delete (videoWrapper->father->list2);
        videoWrapper->father->list1 = NULL;
        videoWrapper->father->list2 = NULL;

        av_free(videoWrapper->father);
        videoWrapper->father = NULL;
        av_free(videoWrapper);
        videoWrapper = NULL;

        /*if (videoAVFormatContext != NULL) {
            avformat_close_input(&videoAVFormatContext);
            videoAVFormatContext = NULL;
        }*/

        /*if (inFile != NULL) {
            fclose(inFile);
            inFile = NULL;
        }
        if (outFile != NULL) {
            fclose(outFile);
            outFile = NULL;
        }*/
        printf("%s\n", "closeVideo() end");
    }

    void closeOther() {
        if (avFormatContext != NULL) {
            avformat_free_context(avFormatContext);
            avFormatContext = NULL;
        }
    }

    int initPlayer() {
        printf("%s\n", "initPlayer() start");

        onReady();

        initAV();
        initAudio();
        initVideo();
        if (openAndFindAVFormatContext() < 0) {
            printf("openAndFindAVFormatContext() failed\n");
            closeAudio();
            closeVideo();
            closeOther();
            onError(0x100, "openAndFindAVFormatContext() failed");
            return -1;
        }
        if (findStreamIndex() < 0) {
            printf("findStreamIndex() failed\n");
            closeAudio();
            closeVideo();
            closeOther();
            onError(0x100, "findStreamIndex() failed");
            return -1;
        }
        if (findAndOpenAVCodecForAudio() < 0) {
            printf("findAndOpenAVCodecForAudio() failed\n");
            closeAudio();
            closeVideo();
            closeOther();
            onError(0x100, "findAndOpenAVCodecForAudio() failed");
            return -1;
        }
        if (findAndOpenAVCodecForVideo() < 0) {
            printf("findAndOpenAVCodecForVideo() failed\n");
            closeAudio();
            closeVideo();
            closeOther();
            onError(0x100, "findAndOpenAVCodecForVideo() failed");
            return -1;
        }
        if (createSwrContent() < 0) {
            printf("createSwrContent() failed\n");
            closeAudio();
            closeVideo();
            closeOther();
            onError(0x100, "createSwrContent() failed");
            return -1;
        }
        if (createSwsContext() < 0) {
            printf("createSwsContext() failed\n");
            closeAudio();
            closeVideo();
            closeOther();
            onError(0x100, "createSwsContext() failed");
            return -1;
        }

        if (!audioWrapper->father->isReading
            || !audioWrapper->father->isHandling
            || !videoWrapper->father->isReading
            || !videoWrapper->father->isHandling) {
            closeAudio();
            closeVideo();
            closeOther();
            onFinished();
            printf("%s\n", "initPlayer() finish");
            return -1;
        }

        int64_t duration = avFormatContext->duration / AV_TIME_BASE;
        printf("initPlayer()      duration: %ld\n", (long) duration);
        if (avFormatContext->duration != AV_NOPTS_VALUE) {
            // 得到的是秒数
            duration = (avFormatContext->duration + 5000) / AV_TIME_BASE;
            int hours, mins, seconds;
            seconds = duration;
            mins = seconds / 60;
            seconds %= 60;
            hours = mins / 60;
            mins %= 60;
            // 00:54:16
            // 单位: 秒
            printf("initPlayer() media seconds: %d\n", (int) duration);
            printf("initPlayer() media          %02d:%02d:%02d\n", hours, mins, seconds);
        }
        switch (use_mode) {
            case USE_MODE_MEDIA: {
                audioWrapper->father->duration =
                videoWrapper->father->duration = duration;
                //onChangeWindow(videoWrapper->srcWidth, videoWrapper->srcHeight);
                break;
            }
            case USE_MODE_ONLY_VIDEO: {
                videoWrapper->father->duration = duration;
                closeAudio();
                //onChangeWindow(videoWrapper->srcWidth, videoWrapper->srcHeight);
                break;
            }
            case USE_MODE_ONLY_AUDIO: {
                audioWrapper->father->duration = duration;
                closeVideo();
                //onChangeWindow(0, 0);
                break;
            }
            default:
                break;
        }

        printf("%s\n", "initPlayer() end");
        return 0;
    }

    /*void setJniParameters(JNIEnv *env, const char *filePath, jobject surfaceJavaObject) {
        memset(inFilePath, '\0', sizeof(inFilePath));
        av_strlcpy(inFilePath, filePath, sizeof(inFilePath));
        printf("setJniParameters() filePath  : %s", inFilePath);

        isLocal = false;
        char *result = strstr(inFilePath, "http://");
        if (result == NULL) {
            result = strstr(inFilePath, "https://");
            if (result == NULL) {
                result = strstr(inFilePath, "rtmp://");
                if (result == NULL) {
                    result = strstr(inFilePath, "rtsp://");
                    if (result == NULL) {
                        isLocal = true;
                    }
                }
            }
        }
        printf("setJniParameters() isLocal   : %d", isLocal);

        if (pANativeWindow != NULL) {
            ANativeWindow_release(pANativeWindow);
            pANativeWindow = NULL;
        }
        // 1.获取一个关联Surface的NativeWindow窗体
        pANativeWindow = ANativeWindow_fromSurface(env, surfaceJavaObject);
    }*/

    int play() {
        if (audioWrapper != NULL && audioWrapper->father != NULL) {
            audioWrapper->father->isPausedForUser = false;
            if (!audioWrapper->father->isPausedForCache) {
                notifyToHandle(audioWrapper->father);
            }
        }
        if (videoWrapper != NULL && videoWrapper->father != NULL) {
            videoWrapper->father->isPausedForUser = false;
            if (!videoWrapper->father->isPausedForCache) {
                notifyToHandle(videoWrapper->father);
            }
        }
        return 0;
    }

    int pause() {
        printf("pause() start\n");
        if (audioWrapper != NULL
            && audioWrapper->father != NULL) {
            audioWrapper->father->isPausedForUser = true;
        }
        if (videoWrapper != NULL
            && videoWrapper->father != NULL) {
            videoWrapper->father->isPausedForUser = true;
        }
        printf("pause() end\n");
        return 0;
    }

    int stop() {
        printf("stop() start\n");
        if (audioWrapper != NULL
            && audioWrapper->father != NULL) {
            // audio
            audioWrapper->father->isStarted = false;
            audioWrapper->father->isReading = false;
            audioWrapper->father->isHandling = false;
            audioWrapper->father->isPausedForUser = false;
            audioWrapper->father->isPausedForCache = false;
            audioWrapper->father->isPausedForSeek = false;
            audioWrapper->father->isHandleList1Full = false;
            notifyToRead(audioWrapper->father);
            notifyToHandle(audioWrapper->father);
        }

        if (videoWrapper != NULL
            && videoWrapper->father != NULL) {
            // video
            videoWrapper->father->isStarted = false;
            videoWrapper->father->isReading = false;
            videoWrapper->father->isHandling = false;
            videoWrapper->father->isPausedForUser = false;
            videoWrapper->father->isPausedForCache = false;
            videoWrapper->father->isPausedForSeek = false;
            videoWrapper->father->isHandleList1Full = false;
            notifyToRead();
            notifyToRead(videoWrapper->father);
            notifyToHandle(videoWrapper->father);
        }
        printf("stop() end\n");

        return 0;
    }

    int release() {
        stop();
        return 0;
    }

    // 有没有在运行,即使暂停状态也是运行状态
    bool isRunning() {
        bool audioRunning = false;
        bool videoRunning = false;
        if (audioWrapper != NULL
            && audioWrapper->father != NULL) {
            audioRunning = audioWrapper->father->isStarted
                           && audioWrapper->father->isHandling;
        }
        if (videoWrapper != NULL
            && videoWrapper->father != NULL) {
            videoRunning = videoWrapper->father->isStarted
                           && videoWrapper->father->isHandling;
        }
        return audioRunning && videoRunning;
    }

    // 有没有在播放,暂停状态不算播放状态
    bool isPlaying() {
        bool audioPlaying = false;
        bool videoPlaying = false;
        if (audioWrapper != NULL
            && audioWrapper->father != NULL) {
            audioPlaying = audioWrapper->father->isStarted
                           && audioWrapper->father->isHandling
                           && !audioWrapper->father->isPausedForUser
                           && !audioWrapper->father->isPausedForCache;
        }
        if (videoWrapper != NULL
            && videoWrapper->father != NULL) {
            videoPlaying = videoWrapper->father->isStarted
                           && videoWrapper->father->isHandling
                           && !videoWrapper->father->isPausedForUser
                           && !videoWrapper->father->isPausedForCache;
        }
        return audioPlaying && videoPlaying;
    }

    /***
     1.暂停状态下seek
     2.播放状态下seek

     bug
     ---------->onClick() mProgress: 0 00:00
     ==================================================================
     seekTo() timestamp: 1402537096
     ---------->onClick() mProgress: 0 00:00
     ==================================================================
     seekTo() timestamp: 1402537096
     */
    int seekTo(int64_t timestamp) {// 单位秒.比如seek到100秒,就传100
        printf("==================================================================\n");
        printf("seekTo() timeStamp: %ld\n", (long) timestamp);

        if ((long) timestamp > 0
            && (audioWrapper == NULL
                || videoWrapper == NULL)) {
            timeStamp = timestamp;
            return 0;
        }

        if ((long) timestamp < 0
            || audioWrapper == NULL
            || audioWrapper->father == NULL
            || audioWrapper->father->isPausedForSeek
            || videoWrapper == NULL
            || videoWrapper->father == NULL
            || videoWrapper->father->isPausedForSeek
            || getDuration() < 0
            || ((long) timestamp) > getDuration()) {
            return -1;
        }

        printf("seekTo() signal() to Read and Handle\n");
        timeStamp = timestamp;
        audioWrapper->father->isPausedForSeek = true;
        videoWrapper->father->isPausedForSeek = true;
        audioWrapper->father->needToSeek = false;
        videoWrapper->father->needToSeek = false;
        notifyToHandle(audioWrapper->father);
        notifyToHandle(videoWrapper->father);
        notifyToRead(audioWrapper->father);
        notifyToRead(videoWrapper->father);
        notifyToRead();

        return 0;
    }

    // 返回值单位是秒
    long getDuration() {
        int64_t duration = -1;
        switch (use_mode) {
            case USE_MODE_MEDIA:
            case USE_MODE_ONLY_VIDEO: {
                if (videoWrapper != NULL
                    && videoWrapper->father != NULL) {
                    duration = videoWrapper->father->duration;
                }
                break;
            }
            case USE_MODE_ONLY_AUDIO: {
                if (audioWrapper != NULL
                    && audioWrapper->father != NULL) {
                    duration = audioWrapper->father->duration;
                }
                break;
            }
            default:
                break;
        }

        return duration;
    }

    void stepAdd(int64_t addStep) {
        /*++videoSleepTime;
        char dest[50];
        sprintf(dest, "videoSleepTime: %d\n", videoSleepTime);
        onInfo(dest);
        printf("stepAdd()      videoSleepTime: %d\n", videoSleepTime);*/

        if (getDuration() > 0) {
            printf("stepAdd() addStep: %ld\n", (long) addStep);
            seekTo(curProgress + addStep);
        }
    }

    void stepSubtract(int64_t subtractStep) {
        /*--videoSleepTime;
        char dest[50];
        sprintf(dest, "videoSleepTime: %d\n", videoSleepTime);
        onInfo(dest);
        printf("stepSubtract() videoSleepTime: %d\n", videoSleepTime);*/

        if (getDuration() > 0) {
            printf("stepAdd() subtractStep: %ld\n", (long) subtractStep);
            seekTo(curProgress - subtractStep);
        }
    }

    /***
     char src[40];
     char dest[100];
     memset(dest, '\0', sizeof(dest));
     strcpy(src, "This is runoob.com");
     strcpy(dest, src);

     原型：int strlen ( const char *str )
     功能：返回字符串的实际长度,不含 '\0'.
     strlen之所以不包含'\0',是因为它在计数的途中遇到'\0'结束.
     char buf[100] = "hello";
     printf("%d\n", strlen(buf));// 5
     printf("%d\n", sizeof(buf));// 100
     */

    char *getStrAVPixelFormat(AVPixelFormat format) {
        char info[25] = {0};
        switch (format) {
            case AV_PIX_FMT_NONE:
                strncpy(info, "AV_PIX_FMT_NONE", strlen("AV_PIX_FMT_NONE"));
                break;
            case AV_PIX_FMT_YUV420P:// 0
                strncpy(info, "AV_PIX_FMT_YUV420P", strlen("AV_PIX_FMT_YUV420P"));
                break;
            case AV_PIX_FMT_YUYV422:
                strncpy(info, "AV_PIX_FMT_YUYV422", strlen("AV_PIX_FMT_YUYV422"));
                break;
            case AV_PIX_FMT_RGB24:
                strncpy(info, "AV_PIX_FMT_RGB24", strlen("AV_PIX_FMT_RGB24"));
                break;
            case AV_PIX_FMT_BGR24:
                strncpy(info, "AV_PIX_FMT_BGR24", strlen("AV_PIX_FMT_BGR24"));
                break;
            case AV_PIX_FMT_YUV422P:
                strncpy(info, "AV_PIX_FMT_YUV422P", strlen("AV_PIX_FMT_YUV422P"));
                break;
            case AV_PIX_FMT_YUV444P:
                strncpy(info, "AV_PIX_FMT_YUV444P", strlen("AV_PIX_FMT_YUV444P"));
                break;
            case AV_PIX_FMT_YUV410P:
                strncpy(info, "AV_PIX_FMT_YUV410P", strlen("AV_PIX_FMT_YUV410P"));
                break;
            case AV_PIX_FMT_YUV411P:
                strncpy(info, "AV_PIX_FMT_YUV411P", strlen("AV_PIX_FMT_YUV411P"));
                break;
            case AV_PIX_FMT_GRAY8:
                strncpy(info, "AV_PIX_FMT_GRAY8", strlen("AV_PIX_FMT_GRAY8"));
                break;
            case AV_PIX_FMT_MONOWHITE:
                strncpy(info, "AV_PIX_FMT_MONOWHITE", strlen("AV_PIX_FMT_MONOWHITE"));
                break;
            case AV_PIX_FMT_MONOBLACK:
                strncpy(info, "AV_PIX_FMT_MONOBLACK", strlen("AV_PIX_FMT_MONOBLACK"));
                break;
            case AV_PIX_FMT_PAL8:
                strncpy(info, "AV_PIX_FMT_PAL8", strlen("AV_PIX_FMT_PAL8"));
                break;
            case AV_PIX_FMT_YUVJ420P:
                strncpy(info, "AV_PIX_FMT_YUVJ420P", strlen("AV_PIX_FMT_YUVJ420P"));
                break;
            case AV_PIX_FMT_YUVJ422P:
                strncpy(info, "AV_PIX_FMT_YUVJ422P", strlen("AV_PIX_FMT_YUVJ422P"));
                break;
            case AV_PIX_FMT_YUVJ444P:
                strncpy(info, "AV_PIX_FMT_YUVJ444P", strlen("AV_PIX_FMT_YUVJ444P"));
                break;
            case AV_PIX_FMT_UYVY422:
                strncpy(info, "AV_PIX_FMT_UYVY422", strlen("AV_PIX_FMT_UYVY422"));
                break;
            case AV_PIX_FMT_UYYVYY411:
                strncpy(info, "AV_PIX_FMT_UYYVYY411", strlen("AV_PIX_FMT_UYYVYY411"));
                break;
            case AV_PIX_FMT_BGR8:
                strncpy(info, "AV_PIX_FMT_BGR8", strlen("AV_PIX_FMT_BGR8"));
                break;
            case AV_PIX_FMT_BGR4:
                strncpy(info, "AV_PIX_FMT_BGR4", strlen("AV_PIX_FMT_BGR4"));
                break;
            case AV_PIX_FMT_BGR4_BYTE:
                strncpy(info, "AV_PIX_FMT_BGR4_BYTE", strlen("AV_PIX_FMT_BGR4_BYTE"));
                break;
            case AV_PIX_FMT_RGB8:
                strncpy(info, "AV_PIX_FMT_RGB8", strlen("AV_PIX_FMT_RGB8"));
                break;
            case AV_PIX_FMT_RGB4:
                strncpy(info, "AV_PIX_FMT_RGB4", strlen("AV_PIX_FMT_RGB4"));
                break;
            case AV_PIX_FMT_RGB4_BYTE:
                strncpy(info, "AV_PIX_FMT_RGB4_BYTE", strlen("AV_PIX_FMT_RGB4_BYTE"));
                break;
            case AV_PIX_FMT_NV12:
                strncpy(info, "AV_PIX_FMT_NV12", strlen("AV_PIX_FMT_NV12"));
                break;
            case AV_PIX_FMT_NV21:
                strncpy(info, "AV_PIX_FMT_NV21", strlen("AV_PIX_FMT_NV21"));
                break;
            case AV_PIX_FMT_ARGB:
                strncpy(info, "AV_PIX_FMT_ARGB", strlen("AV_PIX_FMT_ARGB"));
                break;
            case AV_PIX_FMT_RGBA:
                strncpy(info, "AV_PIX_FMT_RGBA", strlen("AV_PIX_FMT_RGBA"));
                break;
            case AV_PIX_FMT_ABGR:
                strncpy(info, "AV_PIX_FMT_ABGR", strlen("AV_PIX_FMT_ABGR"));
                break;
            case AV_PIX_FMT_BGRA:
                strncpy(info, "AV_PIX_FMT_BGRA", strlen("AV_PIX_FMT_BGRA"));
                break;
            case AV_PIX_FMT_GRAY16BE:
                strncpy(info, "AV_PIX_FMT_GRAY16BE", strlen("AV_PIX_FMT_GRAY16BE"));
                break;
            case AV_PIX_FMT_GRAY16LE:
                strncpy(info, "AV_PIX_FMT_GRAY16LE", strlen("AV_PIX_FMT_GRAY16LE"));
                break;
            case AV_PIX_FMT_YUV440P:
                strncpy(info, "AV_PIX_FMT_YUV440P", strlen("AV_PIX_FMT_YUV440P"));
                break;
            case AV_PIX_FMT_YUVJ440P:
                strncpy(info, "AV_PIX_FMT_YUVJ440P", strlen("AV_PIX_FMT_YUVJ440P"));
                break;
            case AV_PIX_FMT_YUVA420P:
                strncpy(info, "AV_PIX_FMT_YUVA420P", strlen("AV_PIX_FMT_YUVA420P"));
                break;
            case AV_PIX_FMT_RGB48BE:
                strncpy(info, "AV_PIX_FMT_RGB48BE", strlen("AV_PIX_FMT_RGB48BE"));
                break;
            case AV_PIX_FMT_RGB48LE:
                strncpy(info, "AV_PIX_FMT_RGB48LE", strlen("AV_PIX_FMT_RGB48LE"));
                break;
            case AV_PIX_FMT_RGB565BE:
                strncpy(info, "AV_PIX_FMT_RGB565BE", strlen("AV_PIX_FMT_RGB565BE"));
                break;
            case AV_PIX_FMT_RGB565LE:
                strncpy(info, "AV_PIX_FMT_RGB565LE", strlen("AV_PIX_FMT_RGB565LE"));
                break;
            case AV_PIX_FMT_RGB555BE:
                strncpy(info, "AV_PIX_FMT_RGB555BE", strlen("AV_PIX_FMT_RGB555BE"));
                break;
            case AV_PIX_FMT_RGB555LE:
                strncpy(info, "AV_PIX_FMT_RGB555LE", strlen("AV_PIX_FMT_RGB555LE"));
                break;
            case AV_PIX_FMT_BGR565BE:
                strncpy(info, "AV_PIX_FMT_BGR565BE", strlen("AV_PIX_FMT_BGR565BE"));
                break;
            case AV_PIX_FMT_BGR565LE:
                strncpy(info, "AV_PIX_FMT_BGR565LE", strlen("AV_PIX_FMT_BGR565LE"));
                break;
            case AV_PIX_FMT_BGR555BE:
                strncpy(info, "AV_PIX_FMT_BGR555BE", strlen("AV_PIX_FMT_BGR555BE"));
                break;
            case AV_PIX_FMT_BGR555LE:
                strncpy(info, "AV_PIX_FMT_BGR555LE", strlen("AV_PIX_FMT_BGR555LE"));
                break;
            default:
                strncpy(info, "AV_PIX_FMT_NONE", strlen("AV_PIX_FMT_NONE"));
                break;
        }

        return info;
    }

    void startPlayer() {
        //char filePath[] = "https://cdn1.ibizastream.biz:441/free/1/playlist_dvr.m3u8";// *
        char filePath[] = "/Users/alexander/Downloads/千千阙歌.mp4";
        memset(inFilePath, '\0', sizeof(inFilePath));
        av_strlcpy(inFilePath, filePath, sizeof(inFilePath));
        printf("startPlayer() filePath  : %s", inFilePath);

        int ret = initPlayer();
        closeAudio();
        closeVideo();
        closeOther();
        printf("startPlayer() ret: %d\n", ret);
    }

}
