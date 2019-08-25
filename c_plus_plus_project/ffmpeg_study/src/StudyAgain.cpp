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
 此参数是设定内存对齐的对齐数,也就是按多大的字节进行内存对齐。
 比如设置为1,表示按1字节对齐,那么得到的结果就是与实际的内存大小一样。
 再比如设置为4,表示按4字节对齐。也就是内存的起始地址必须是4的整倍数。
 int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
                               enum AVSampleFormat sample_fmt, int align);
 */

#define TYPE_UNKNOW -1
#define TYPE_AUDIO 1
#define TYPE_VIDEO 2

#define NEXT_READ_UNKNOW -1
#define NEXT_READ_QUEUE1 1
#define NEXT_READ_QUEUE2 2

#define NEXT_HANDLE_UNKNOW -1
#define NEXT_HANDLE_QUEUE1 1
#define NEXT_HANDLE_QUEUE2 2

#define MAX_AVPACKET_COUNT_AUDIO_HTTP 3000
#define MAX_AVPACKET_COUNT_VIDEO_HTTP 3000

#define MAX_AVPACKET_COUNT_AUDIO_LOCAL 100
#define MAX_AVPACKET_COUNT_VIDEO_LOCAL 100

#define USE_AUDIO
#define USE_VIDEO

typedef struct AVPacketQueue {
    AVPacketList *firstAVPacketList = NULL;
    AVPacketList *lastAVPacketList = NULL;
    // 有多少个AVPacketList
    int allAVPacketsCount = 0;
    // 所有AVPacket占用的空间大小
    int64_t allAVPacketsSize = 0;
};

// 子类都要用到的部分
// 在这里初始化都是没用的,所有变量会被置为0
// 只有分配内存后再初始化才有效
struct Wrapper {
    int type = TYPE_UNKNOW;
    AVFormatContext *avFormatContext = NULL;
    AVCodecContext *avCodecContext = NULL;
    // 有些东西需要通过它去得到
    AVCodecParameters *avCodecParameters = NULL;
    // 解码器
    AVCodec *decoderAVCodec = NULL;
    // 编码器
    AVCodec *encoderAVCodec = NULL;
    // 存储压缩数据(视频对应H.264等码流数据,音频对应AAC/MP3等码流数据)
    AVPacket *avPacket = NULL;
    // 存储非压缩数据(视频对应RGB/YUV像素数据,音频对应PCM采样数据)
    // 一个视频没有解码之前读出的数据是压缩数据,把压缩数据解码后就是原始数据
    // 解码后的原始数据(像素格式可能不是我们想要的,如果是想要的,那么没必要再调用sws_scale函数了)
    AVFrame *decodedAVFrame = NULL;
    // 用于格式转换(音频用不到)
    AVFrame *dstAVFrame = NULL;
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
    bool isReadQueue1Full = false;
    bool isReadQueue2Full = false;
    int nextRead = NEXT_READ_UNKNOW;
    int nextHandle = NEXT_HANDLE_UNKNOW;
    // 队列中最多保存多少个AVPacket
    int maxAVPacketsCount = 0;

    bool isStarted = false;
    bool isReading = false;
    bool isHandling = false;
    // 因为user所以pause
    bool isPausedForUser = false;
    // 因为cache所以pause
    bool isPausedForCache = false;
    // seek的初始化条件有没有完成,true表示完成
    bool seekToInit = false;

    // 单位: 秒
    int64_t duration = 0;
    // 单位: 秒
    int64_t timestamp = 0;

    pthread_mutex_t readLockMutex;
    pthread_cond_t readLockCondition;
    pthread_mutex_t handleLockMutex;
    pthread_cond_t handleLockCondition;
};

struct AudioWrapper {
    struct Wrapper *father = NULL;
    SwrContext *swrContext = NULL;
    // 从音频源或视频源中得到
    // 采样率
    int srcSampleRate = 0;
    int dstSampleRate = 0;
    // 声道数
    int srcNbChannels = 0;
    // 由dstChannelLayout去获到
    int dstNbChannels = 0;
    int srcNbSamples = 0;
    int dstNbSamples = 0;
    // 由srcNbChannels能得到srcChannelLayout,也能由srcChannelLayout得到srcNbChannels
    int srcChannelLayout = 0;
    // 双声道输出
    int dstChannelLayout = 0;
    // 从音频源或视频源中得到(采样格式)
    enum AVSampleFormat srcAVSampleFormat = AV_SAMPLE_FMT_NONE;
    // 输出的采样格式16bit PCM
    enum AVSampleFormat dstAVSampleFormat = AV_SAMPLE_FMT_S16;

    // 要播放的数据存放于playBuffer中
    DECLARE_ALIGNED(16, unsigned char, playBuffer)[MAX_AUDIO_FRAME_SIZE * 4];

    // 解码一次得到的数据量
    unsigned int decodedDataSize = 0;
    // 用于标记已处理过的数据位置(针对audio_decoded_data_size的位置)
    unsigned int decodedDataSizeIndex = 0;

    // SDL
    SDL_AudioSpec srcSDLAudioSpec;
    SDL_AudioSpec dstSDLAudioSpec;
};

struct VideoWrapper {
    struct Wrapper *father = NULL;
    SwsContext *swsContext = NULL;
    // 从视频源中得到
    enum AVPixelFormat srcAVPixelFormat = AV_PIX_FMT_NONE;
    // 从原来的像素格式转换为想要的视频格式(可能应用于不需要播放视频的场景)
    // 播放时dstAVPixelFormat必须跟srcAVPixelFormat的值一样,不然画面有问题
    enum AVPixelFormat dstAVPixelFormat = AV_PIX_FMT_RGB24;
    // 解码后的原始数据(像素格式是我们想要的)
    AVFrame *wantedAVFrame = NULL;
    // 从视频源中得到的宽高
    int srcWidth = 0, srcHeight = 0;
    size_t srcArea = 0;
    // 想要播放的窗口大小,可以直接使用srcWidth和srcHeight
    // 还不能指定想要的宽高,现在只有把想要的宽高跟源视频宽高设成一样时内容显示才正常,不然不正常
    int dstWidth = 720, dstHeight = 360;
    size_t dstArea = 0;
    // 使用到sws_scale函数时需要定义这些变量
    int srcLineSize[4] = {NULL}, dstLineSize[4] = {NULL};
};

struct AudioWrapper *audioWrapper = NULL;
struct VideoWrapper *videoWrapper = NULL;

//pthread_mutex_t lockMutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t lockCondition = PTHREAD_COND_INITIALIZER;

//格式上下结构体,,可以理解为存储数据流的文件,伴随整个生命周期
// AVFormatContext相当于Android的MediaExtractor,保存了音视频的Format信息(MediaFormat)
//AVFormatContext *avFormatContext = NULL;

// 自己电脑上的文件路径
//char *inFilePath2 = "/root/视频/tomcat_video/流浪的地球.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/疾速备战.mp4";
char *inFilePath2 = "/root/视频/tomcat_video/痞子英雄2-黎明升起.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/shape_of_my_heart.mp4";
//char *inFilePath2 = "/root/音乐/KuGou/蔡国权-不装饰你的梦.mp3";
//char *inFilePath2 = "/root/音乐/KuGou/冷漠、云菲菲 - 伤心城市.mp3";
// 公司电脑上的文件路径
//char *inFilePath2 = "http://ok.xzokzyzy.com/20190606/1940_094739d9/%E6%80%92%E6%B5%B7%E6%BD%9C%E6%B2%99&%E7%A7%A6%E5%B2%AD%E7%A5%9E%E6%A0%91%E7%AC%AC01%E9%9B%86.mp4";
//char *inFilePath2 = "http://xunlei.xiazai-zuida.com/1908/%E6%89%AB%E6%AF%922.HD1280%E9%AB%98%E6%B8%85%E5%9B%BD%E8%AF%AD%E4%B8%AD%E5%AD%97%E7%89%88.mp4";
//char *inFilePath2 = "http://xunlei.jingpin88.com/20171026/cQ7hsCrN/mp4/cQ7hsCrN.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/AC3Plus_mountainbike-cyberlink_1920_1080.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/AC3Plus_mountainbike-cyberlink_1920_1080.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/test.mp4";
//char *inFilePath2 = "/root/视频/tomcat_video/kingsman.mp4";
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

static bool isLocal = false;

double TIME_DIFFERENCE = 1.000000;// 0.180000
double audioTimeDifference = 0;
double videoTimeDifference = 0;
double totalTimeDifference = 0;
long totalTimeDifferenceCount = 0;
long preProgress = 0;
long videoSleep = 0;
long sleepStep = 0;

bool needLog = false;

////////////////////////////////////////////////////////////////////////////////////

static int getAVPacketFromQueue(struct AVPacketQueue *packet_queue, AVPacket *avpacket);

int audioDecodeFrame() {
    int ret, get_nb_samples_per_channel;
    int got_frame_ptr = 0;
    int64_t get_ch_layout_from_decoded_avframe;
    // 压缩数据
    AVPacket *avPacket = audioWrapper->father->avPacket;
    unsigned char *audio_pkt_data = NULL;
    int audio_pkt_size = 0;
    // decodedAVFrame为解码后的原始数据
    AVFrame *decodedAVFrame = audioWrapper->father->decodedAVFrame;
    AVStream *stream =
            audioWrapper->father->avFormatContext->streams[audioWrapper->father->streamIndex];

    for (;;) {
        while (audio_pkt_size > 0) {
            av_frame_unref(decodedAVFrame);
            /***
             当AVPacket中装得是音频时,有可能一个AVPacket中有多个AVFrame,
             而某些解码器只会解出第一个AVFrame,这种情况我们必须循环解码出后续AVFrame.
             */
            ret = avcodec_decode_audio4(audioWrapper->father->avCodecContext,
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

            if (!audioWrapper->father->isStarted) {
                audioWrapper->father->isStarted = true;
            }
#ifdef USE_VIDEO
            while (videoWrapper != NULL
                   && videoWrapper->father != NULL
                   && !videoWrapper->father->isStarted) {
                usleep(1000);
            }
            audioTimeDifference = decodedAVFrame->pts * av_q2d(stream->time_base);
            //printf("handleAudioData() nowPts : %lf\n", audioTimeDifference);
#endif

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

            if (audioWrapper->srcSampleRate != decodedAVFrame->sample_rate) {
                printf("SampleRate变了 srcSampleRate: %d now: %d\n",
                       audioWrapper->srcSampleRate, decodedAVFrame->sample_rate);
            }
            if (audioWrapper->srcAVSampleFormat != decodedAVFrame->format) {
                printf("AVSampleFormat变了 srcAVSampleFormat: %d now: %d\n",
                       audioWrapper->srcAVSampleFormat, decodedAVFrame->format);
            }
            if (audioWrapper->srcChannelLayout != get_ch_layout_from_decoded_avframe) {
                printf("ChannelLayout变了 srcChannelLayout: %d now: %d\n",
                       audioWrapper->srcChannelLayout, get_ch_layout_from_decoded_avframe);
            }

            /***
             接下来判断我们之前设置SDL时设置的声音格式(AV_SAMPLE_FMT_S16),声道布局,
             采样频率,每个AVFrame的每个声道采样数与
             得到的该AVFrame分别是否相同,如有任意不同,我们就需要swr_convert该AVFrame,
             然后才能符合之前设置好的SDL的需要,才能播放.
             */
            if (audioWrapper->srcSampleRate != decodedAVFrame->sample_rate
                || audioWrapper->srcAVSampleFormat != decodedAVFrame->format
                || audioWrapper->srcChannelLayout != get_ch_layout_from_decoded_avframe) {
                printf("---------------------------------\n");
                printf("nowSampleRate       : %d\n", decodedAVFrame->sample_rate);
                printf("nowNbSamples        : %d\n", decodedAVFrame->nb_samples);
                printf("nowAVSampleFormat   : %d\n", decodedAVFrame->format);
                printf("nowChannelLayout    : %d\n", get_ch_layout_from_decoded_avframe);
                printf("nowNbChannels       : %d\n", decodedAVFrame->channels);
                printf("---------------------------------\n");

                if (audioWrapper->swrContext) {
                    swr_free(&audioWrapper->swrContext);
                }
                printf("audio_state->audioSwrContext swr_alloc_set_opts.\n");
                audioWrapper->swrContext = swr_alloc();
                swr_alloc_set_opts(audioWrapper->swrContext,
                                   audioWrapper->dstChannelLayout,
                                   audioWrapper->dstAVSampleFormat,
                                   audioWrapper->dstSampleRate,
                                   get_ch_layout_from_decoded_avframe,
                                   (enum AVSampleFormat) decodedAVFrame->format,
                                   decodedAVFrame->sample_rate,
                                   0, NULL);
                if (!audioWrapper->swrContext || swr_init(audioWrapper->swrContext) < 0) {
                    fprintf(stderr, "swr_init() failed\n");
                    break;
                } else {
                    fprintf(stdout, "audio_state->audioSwrContext is created.\n");
                }

                audioWrapper->srcSampleRate = decodedAVFrame->sample_rate;
                audioWrapper->srcNbChannels = decodedAVFrame->channels;
                audioWrapper->srcAVSampleFormat = (enum AVSampleFormat) decodedAVFrame->format;
                audioWrapper->srcNbSamples = decodedAVFrame->nb_samples;
                audioWrapper->srcChannelLayout = get_ch_layout_from_decoded_avframe;
            }

            /***
             转换该AVFrame到设置好的SDL需要的样子,有些旧的代码示例最主要就是少了这一部分,
             往往一些音频能播,一些不能播,这就是原因,比如有些源文件音频恰巧是AV_SAMPLE_FMT_S16的.
             swr_convert 返回的是转换后每个声道(channel)的采样数
             */
            unsigned char *out[] = {audioWrapper->playBuffer};
            int out_count = sizeof(audioWrapper->playBuffer)
                            / audioWrapper->dstNbChannels
                            / av_get_bytes_per_sample(audioWrapper->dstAVSampleFormat);
            const unsigned char **in = (const unsigned char **) decodedAVFrame->extended_data;
            int in_count = decodedAVFrame->nb_samples;
            // 转换后的数据存在audioWrapper->outBuffer中,也就是要播放的数据
            // 大小为decodedAVFrame->nb_samples
            get_nb_samples_per_channel = swr_convert(audioWrapper->swrContext,
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
            return audioWrapper->dstNbChannels
                   * get_nb_samples_per_channel
                   * av_get_bytes_per_sample(audioWrapper->dstAVSampleFormat);
        }//while end

        av_free_packet(avPacket);

        memset(avPacket, 0, sizeof(*avPacket));
        if (audioWrapper->father->nextHandle == NEXT_HANDLE_QUEUE1
            && audioWrapper->father->isReadQueue1Full
            && audioWrapper->father->queue1->allAVPacketsCount > 0) {
            audioWrapper->father->handleFramesCount++;
            getAVPacketFromQueue(audioWrapper->father->queue1, avPacket);
            if (audioWrapper->father->queue1->allAVPacketsCount == 0) {
                memset(audioWrapper->father->queue1, 0, sizeof(struct AVPacketQueue));
                audioWrapper->father->isReadQueue1Full = false;
                audioWrapper->father->nextHandle = NEXT_HANDLE_QUEUE2;
                if (needLog) {
                    printf("handleAudioData() Queue1 用完了\n");
                    printf("handleAudioData() Queue2 isReadQueue2Full : %d\n",
                           audioWrapper->father->isReadQueue2Full);
                    printf("handleAudioData() Queue2 allAVPacketsCount: %d\n",
                           audioWrapper->father->queue2->allAVPacketsCount);
                }
                if (audioWrapper->father->isReading) {
                    if (needLog) {
                        printf("handleAudioData() signal() readLockCondition\n");
                    }
                    pthread_mutex_lock(&audioWrapper->father->readLockMutex);
                    pthread_cond_signal(&audioWrapper->father->readLockCondition);
                    pthread_mutex_unlock(&audioWrapper->father->readLockMutex);
                }
            }
        } else if (audioWrapper->father->nextHandle == NEXT_HANDLE_QUEUE2
                   && audioWrapper->father->isReadQueue2Full
                   && audioWrapper->father->queue2->allAVPacketsCount > 0) {
            audioWrapper->father->handleFramesCount++;
            getAVPacketFromQueue(audioWrapper->father->queue2, avPacket);
            if (audioWrapper->father->queue2->allAVPacketsCount == 0) {
                memset(audioWrapper->father->queue2, 0, sizeof(struct AVPacketQueue));
                audioWrapper->father->isReadQueue2Full = false;
                audioWrapper->father->nextHandle = NEXT_HANDLE_QUEUE1;
                if (needLog) {
                    printf("handleAudioData() Queue2 用完了\n");
                    printf("handleAudioData() Queue1 isReadQueue1Full : %d\n",
                           audioWrapper->father->isReadQueue1Full);
                    printf("handleAudioData() Queue1 allAVPacketsCount: %d\n",
                           audioWrapper->father->queue1->allAVPacketsCount);
                }
                if (audioWrapper->father->isReading) {
                    if (needLog) {
                        printf("handleAudioData() signal() readLockCondition\n");
                    }
                    pthread_mutex_lock(&audioWrapper->father->readLockMutex);
                    pthread_cond_signal(&audioWrapper->father->readLockCondition);
                    pthread_mutex_unlock(&audioWrapper->father->readLockMutex);
                }
            }
        } else if (audioWrapper->father->isReading
                   && !audioWrapper->father->isReadQueue1Full
                   && !audioWrapper->father->isReadQueue2Full) {
            //onPaused();
            // 音频Cache引起的暂停
#ifdef USE_VIDEO
            // 让视频也同时暂停
            if (videoWrapper != NULL && videoWrapper->father != NULL) {
                videoWrapper->father->isPausedForCache = true;
            }
#endif
            audioWrapper->father->isPausedForCache = true;
            printf("handleAudioData() wait() Cache start\n");
            pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
            pthread_cond_wait(&audioWrapper->father->handleLockCondition,
                              &audioWrapper->father->handleLockMutex);
            pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
            printf("handleAudioData() wait() Cache end\n");
            audioWrapper->father->isPausedForCache = false;
            // 通知视频结束暂停
#ifdef USE_VIDEO
            if (videoWrapper != NULL && videoWrapper->father != NULL) {
                videoWrapper->father->isPausedForCache = false;
                pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
                pthread_cond_signal(&videoWrapper->father->handleLockCondition);
                pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
            }
#endif
            //onPlayed();
            continue;
        } else if (!audioWrapper->father->isReading
                   && audioWrapper->father->queue1->allAVPacketsCount == 0
                   && audioWrapper->father->queue2->allAVPacketsCount == 0) {
            audioWrapper->father->isHandling = false;
            // for (;;) end
            break;
        }

        audio_pkt_data = avPacket->data;
        audio_pkt_size = avPacket->size;
    }//for end
}

// SDL提供的回调函数,数据只有往这个回调函数的stream中填充数据才能发出声音
void sdlAudioCallback(void *userdata, uint8_t *sdl_need_stream_data, int sdl_max_handle_data_size) {
    int max_handle_data_size, audio_decoded_data_size;

    if (!audioWrapper->father->isHandling
        || (audioWrapper->father->queue1->allAVPacketsCount == 0
            && audioWrapper->father->queue2->allAVPacketsCount == 0)
        /*&& !audioWrapper->father->isReadQueue1Full
        && !audioWrapper->father->isReadQueue2Full*/) {
        fprintf(stdout, "sdlAudioCallback() isReadQueue1Full: %d\n", audioWrapper->father->isReadQueue1Full);
        fprintf(stdout, "sdlAudioCallback() isReadQueue2Full: %d\n", audioWrapper->father->isReadQueue2Full);
        fprintf(stdout, "sdlAudioCallback() pthread_cond_signal() return\n");
        // 唤醒线程
        pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
        pthread_cond_signal(&audioWrapper->father->handleLockCondition);
        pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
        return;
    }

    // 2048(意思就是SDL一次想要读取2048个字节的数据,这些数据放在sdl_need_stream_data中)
    // printf("sdl_max_handle_data_size = %d\n", sdl_max_handle_data_size);
    while (sdl_max_handle_data_size > 0) {
        if (audioWrapper->decodedDataSizeIndex >= audioWrapper->decodedDataSize) {
            audio_decoded_data_size = audioDecodeFrame();
            // 4096
            //fprintf(stdout, "audio_decoded_data_size = %d\n", audio_decoded_data_size);

            if (audio_decoded_data_size < 0) {
                /* silence */
                audioWrapper->decodedDataSize = 1024;
                memset(audioWrapper->playBuffer, 0, audioWrapper->decodedDataSize);
            } else {
                audioWrapper->decodedDataSize = audio_decoded_data_size;
            }
            audioWrapper->decodedDataSizeIndex = 0;
        }

        max_handle_data_size = audioWrapper->decodedDataSize - audioWrapper->decodedDataSizeIndex;
        if (max_handle_data_size > sdl_max_handle_data_size) {
            max_handle_data_size = sdl_max_handle_data_size;
        }

        memcpy(sdl_need_stream_data,
               (unsigned char *) audioWrapper->playBuffer + audioWrapper->decodedDataSizeIndex,
               max_handle_data_size);
        sdl_max_handle_data_size -= max_handle_data_size;
        sdl_need_stream_data += max_handle_data_size;
        audioWrapper->decodedDataSizeIndex += max_handle_data_size;
    }//while end
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
    // B2.1 打开音频设备,获取SDL设备支持的音频参数srcSDLAudioSpec(期望的参数是dstSDLAudioSpec,实际得到srcSDLAudioSpec)
    // 1) SDL提供两种使音频设备取得音频数据方法：
    //    a. push,SDL以特定的频率调用回调函数,在回调函数中取得音频数据
    //    b. pull,用户程序以特定的频率调用SDL_QueueAudio(),向音频设备提供数据。此种情况dstSDLAudioSpec.callback=NULL
    // 2) 音频设备打开后播放静音,不启动回调,调用SDL_PauseAudio(0)后启动回调,开始正常播放音频
    // 第一步
    // 采样率
    audioWrapper->dstSDLAudioSpec.freq = audioWrapper->srcSampleRate;
    // 声道数
    audioWrapper->dstSDLAudioSpec.channels = audioWrapper->dstNbChannels;
    // S表带符号,16是采样深度,SYS表采用系统字节序
    audioWrapper->dstSDLAudioSpec.format = AUDIO_S16SYS;
    // 静音值
    audioWrapper->dstSDLAudioSpec.silence = 0;
    // SDL声音缓冲区尺寸,单位是单声道采样点尺寸x通道数
    // audioWrapper->dstSDLAudioSpec.samples = audioWrapper->dstNbSamples;
    audioWrapper->dstSDLAudioSpec.samples = 1024;
    // 回调函数,若为NULL,则应使用SDL_QueueAudio()机制
    audioWrapper->dstSDLAudioSpec.callback = sdlAudioCallback;
    // 提供给回调函数的参数
    audioWrapper->dstSDLAudioSpec.userdata = &audioWrapper;

    const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
    // 第二步
    while (SDL_OpenAudio(&audioWrapper->dstSDLAudioSpec, &audioWrapper->srcSDLAudioSpec) < 0) {
        fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", audioWrapper->dstNbChannels, SDL_GetError());
        audioWrapper->dstSDLAudioSpec.channels = next_nb_channels[FFMIN(7, audioWrapper->dstSDLAudioSpec.channels)];
        if (!audioWrapper->dstSDLAudioSpec.channels) {
            fprintf(stderr, "No more channel combinations to tyu, audio open failed\n");
            return -1;
        }
        audioWrapper->dstChannelLayout = av_get_default_channel_layout(audioWrapper->dstNbChannels);
        printf("dstChannelLayout    : %d\n", audioWrapper->dstChannelLayout);
    }
    if (audioWrapper->srcSDLAudioSpec.format != AUDIO_S16SYS) {
        fprintf(stderr, "SDL advised audio format %d is not supported!\n", audioWrapper->srcSDLAudioSpec.format);
        return -1;
    }
    if (audioWrapper->srcSDLAudioSpec.channels != audioWrapper->dstSDLAudioSpec.channels) {
        audioWrapper->dstChannelLayout = av_get_default_channel_layout(audioWrapper->srcSDLAudioSpec.channels);
        if (!audioWrapper->dstChannelLayout) {
            fprintf(stderr, "SDL advised channel count %d is not supported!\n", audioWrapper->srcSDLAudioSpec.channels);
            return -1;
        }
    }

    fprintf(stdout, "%d: srcSDLAudioSpec.freq    : %d\n", __LINE__, audioWrapper->srcSDLAudioSpec.freq);
    fprintf(stdout, "%d: srcSDLAudioSpec.channels: %d\n", __LINE__, audioWrapper->srcSDLAudioSpec.channels);
    fprintf(stdout, "%d: srcSDLAudioSpec.format  : %d\n", __LINE__, audioWrapper->srcSDLAudioSpec.format);
    fprintf(stdout, "%d: srcSDLAudioSpec.samples : %d\n", __LINE__, audioWrapper->srcSDLAudioSpec.samples);

    fprintf(stdout, "%d: dstSDLAudioSpec.freq    : %d\n", __LINE__, audioWrapper->dstSDLAudioSpec.freq);
    fprintf(stdout, "%d: dstSDLAudioSpec.channels: %d\n", __LINE__, audioWrapper->dstSDLAudioSpec.channels);
    fprintf(stdout, "%d: dstSDLAudioSpec.format  : %d\n", __LINE__, audioWrapper->dstSDLAudioSpec.format);
    fprintf(stdout, "%d: dstSDLAudioSpec.samples : %d\n", __LINE__, audioWrapper->dstSDLAudioSpec.samples);

    audioWrapper->srcAVSampleFormat = audioWrapper->dstAVSampleFormat = AV_SAMPLE_FMT_S16;
    audioWrapper->srcSampleRate = audioWrapper->dstSampleRate = audioWrapper->srcSDLAudioSpec.freq;
    audioWrapper->srcChannelLayout = audioWrapper->dstChannelLayout;
    audioWrapper->srcNbChannels = audioWrapper->dstNbChannels = audioWrapper->srcSDLAudioSpec.channels;

    audioWrapper->father->avFormatContext->streams[audioWrapper->father->streamIndex]->discard = AVDISCARD_DEFAULT;

    // 第三步
    SDL_PauseAudio(0);

    printf("%s\n", "initAudioSDL() end");
    return 0;
}

int initVideoSDL() {
    printf("%s\n", "initVideoSDL() start");
    if (videoWrapper->srcWidth == 0 || videoWrapper->srcHeight == 0) {
        printf("%s\n", "没有设置srcWidth和srcHeight的值");
        return -1;
    }

    //SDL 2.0 Support for multiple windows
    sdlWindow = SDL_CreateWindow(inFilePath2,
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 videoWrapper->srcWidth, videoWrapper->srcHeight,
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
                                   videoWrapper->srcWidth, videoWrapper->srcHeight);
    if (sdlTexture == NULL) {
        printf("SDL_CreateTexture() failed: %s\n", SDL_GetError());
        return -1;
    }

    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = videoWrapper->srcWidth;
    sdlRect.h = videoWrapper->srcHeight;

    /*sdlThread = SDL_CreateThread(pushEventThread, NULL, NULL);
    if (sdlThread == NULL) {
        printf("SDL_CreateThread() failed: %s\n", SDL_GetError());
        return -1;
    }*/
    printf("%s\n", "initVideoSDL() end");
    return 0;
}

int openAndFindAVFormatContextForAudio() {
    // AVFormatContext初始化,里面设置结构体的一些默认信息
    // 相当于Java中创建对象
    audioWrapper->father->avFormatContext = avformat_alloc_context();
    if (audioWrapper->father->avFormatContext == NULL) {
        printf("audioWrapper->father->avFormatContext is NULL.\n");
        return -1;
    }
    // 获取基本的文件信息
    if (avformat_open_input(&audioWrapper->father->avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open audio input stream.\n");
        return -1;
    }
    // 文件中的流信息
    if (avformat_find_stream_info(audioWrapper->father->avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    return 0;
}

int openAndFindAVFormatContextForVideo() {
    videoWrapper->father->avFormatContext = avformat_alloc_context();
    if (videoWrapper->father->avFormatContext == NULL) {
        printf("videoWrapper->father->avFormatContext is NULL.\n");
        return -1;
    }
    if (avformat_open_input(&videoWrapper->father->avFormatContext, inFilePath2, NULL, NULL) != 0) {
        printf("Couldn't open video input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(videoWrapper->father->avFormatContext, NULL) != 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    return 0;
}

/***
 找音视频流轨道
 */
int findStreamIndexForAudio() {
    if (audioWrapper->father->avFormatContext == NULL) {
        return -1;
    }
    // audio stream index
    int streams = audioWrapper->father->avFormatContext->nb_streams;
    for (int i = 0; i < streams; i++) {
        // 得到AVCodecParameters
        audioWrapper->father->avCodecParameters =
                audioWrapper->father->avFormatContext->streams[i]->codecpar;
        if (audioWrapper->father->avCodecParameters != NULL) {
            AVMediaType mediaType = audioWrapper->father->avCodecParameters->codec_type;
            if (mediaType == AVMEDIA_TYPE_AUDIO) {
                audioWrapper->father->streamIndex = i;
                break;
            }
        }
    }

    if (audioWrapper->father->streamIndex == -1) {
        printf("Didn't find audio stream.\n");
        return -1;
    } else {
        printf("audioStreamIndex: %d\n", audioWrapper->father->streamIndex);
        return 0;
    }
}

int findStreamIndexForVideo() {
    if (videoWrapper->father->avFormatContext == NULL) {
        return -1;
    }
    // video stream index
    int streams = videoWrapper->father->avFormatContext->nb_streams;
    for (int i = 0; i < streams; i++) {
        // 得到AVCodecParameters
        videoWrapper->father->avCodecParameters =
                videoWrapper->father->avFormatContext->streams[i]->codecpar;
        if (videoWrapper->father->avCodecParameters != NULL) {
            AVMediaType mediaType = videoWrapper->father->avCodecParameters->codec_type;
            if (mediaType == AVMEDIA_TYPE_VIDEO) {
                videoWrapper->father->streamIndex = i;
                break;
            }
        }
    }

    if (videoWrapper->father->streamIndex == -1) {
        printf("Didn't find video stream.\n");
        return -1;
    } else {
        printf("videoStreamIndex: %d\n", videoWrapper->father->streamIndex);
        return 0;
    }
}

int findAndOpenAVCodecForAudio() {
    if (audioWrapper->father->avCodecParameters == NULL
        || audioWrapper->father->streamIndex == -1) {
        return -1;
    }
    // audio
    if (audioWrapper->father->streamIndex != -1) {
        // 获取音频解码器
        // 先通过AVCodecParameters找到AVCodec
        audioWrapper->father->decoderAVCodec = avcodec_find_decoder(audioWrapper->father->avCodecParameters->codec_id);
        if (audioWrapper->father->decoderAVCodec != NULL) {
            // 获取解码器上下文
            // 再通过AVCodec得到AVCodecContext
            audioWrapper->father->avCodecContext = avcodec_alloc_context3(audioWrapper->father->decoderAVCodec);
            if (audioWrapper->father->avCodecContext != NULL) {
                // 关联操作
                if (avcodec_parameters_to_context(
                        audioWrapper->father->avCodecContext, audioWrapper->father->avCodecParameters) < 0) {
                    return -1;
                } else {
                    // 打开AVCodec
                    if (avcodec_open2(
                            audioWrapper->father->avCodecContext, audioWrapper->father->decoderAVCodec, NULL) != 0) {
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
    if (videoWrapper->father->avCodecParameters == NULL
        || videoWrapper->father->streamIndex == -1) {
        return -1;
    }
    // video
    if (videoWrapper->father->streamIndex != -1) {
        videoWrapper->father->decoderAVCodec = avcodec_find_decoder(videoWrapper->father->avCodecParameters->codec_id);
        if (videoWrapper->father->decoderAVCodec != NULL) {
            videoWrapper->father->avCodecContext = avcodec_alloc_context3(videoWrapper->father->decoderAVCodec);
            if (videoWrapper->father->avCodecContext != NULL) {
                if (avcodec_parameters_to_context(
                        videoWrapper->father->avCodecContext, videoWrapper->father->avCodecParameters) < 0) {
                    return -1;
                } else {
                    if (avcodec_open2(
                            videoWrapper->father->avCodecContext, videoWrapper->father->decoderAVCodec, NULL) != 0) {
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
    audioWrapper->srcSampleRate = audioWrapper->father->avCodecContext->sample_rate;
    audioWrapper->srcNbSamples = audioWrapper->father->avCodecContext->frame_size;
    audioWrapper->srcNbChannels = audioWrapper->father->avCodecContext->channels;
    audioWrapper->srcChannelLayout = audioWrapper->father->avCodecContext->channel_layout;
    audioWrapper->srcAVSampleFormat = audioWrapper->father->avCodecContext->sample_fmt;
    printf("---------------------------------\n");
    printf("srcSampleRate       : %d\n", audioWrapper->srcSampleRate);
    printf("srcNbSamples        : %d\n", audioWrapper->srcNbSamples);
    printf("srcNbChannels       : %d\n", audioWrapper->srcNbChannels);
    printf("srcAVSampleFormat   : %d\n", audioWrapper->srcAVSampleFormat);
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
    audioWrapper->dstSampleRate = audioWrapper->srcSampleRate;
    audioWrapper->dstNbSamples = audioWrapper->srcNbSamples;
    audioWrapper->dstNbChannels = audioWrapper->srcNbChannels;
    if (!audioWrapper->dstChannelLayout
        || audioWrapper->dstChannelLayout != av_get_channel_layout_nb_channels(audioWrapper->dstChannelLayout)) {
        audioWrapper->dstChannelLayout = av_get_default_channel_layout(audioWrapper->dstNbChannels);
        printf("dstChannelLayout1   : %d\n", audioWrapper->dstChannelLayout);
        // why?
        audioWrapper->dstChannelLayout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
        printf("dstChannelLayout2   : %d\n", audioWrapper->dstChannelLayout);
    }
    audioWrapper->dstNbChannels = av_get_channel_layout_nb_channels(audioWrapper->dstChannelLayout);

    printf("dstSampleRate       : %d\n", audioWrapper->dstSampleRate);
    printf("dstNbSamples        : %d\n", audioWrapper->dstNbSamples);
    printf("dstNbChannels       : %d\n", audioWrapper->dstNbChannels);
    printf("dstAVSampleFormat   : %d\n", audioWrapper->dstAVSampleFormat);
    printf("---------------------------------\n");

    audioWrapper->father->avPacket = av_packet_alloc();
    // avPacket ---> decodedAVFrame ---> dstAVFrame ---> 播放声音
    audioWrapper->father->decodedAVFrame = av_frame_alloc();
    // audioWrapper->father->dstAVFrame = av_frame_alloc();

    /*int samplesGetBufferSize = av_samples_get_buffer_size(
            audioWrapper->father->dstAVFrame->linesize,
            audioWrapper->dstNbChannels, audioWrapper->dstNbSamples, audioWrapper->dstAVSampleFormat,
            1);
    printf("samplesGetBufferSize: %d\n", samplesGetBufferSize);
    if (samplesGetBufferSize <= 0) {
        samplesGetBufferSize = 4096;
    }
    audioWrapper->father->outBufferSize = samplesGetBufferSize;
    // 存储音频帧的原始数据
    audioWrapper->father->outBuffer1 = (unsigned char *) av_malloc(samplesGetBufferSize);
    int samplesFillArrays = av_samples_fill_arrays(
            audioWrapper->father->dstAVFrame->data,
            audioWrapper->father->dstAVFrame->linesize,
            audioWrapper->father->outBuffer1,
            audioWrapper->dstNbChannels,
            audioWrapper->dstNbSamples,
            audioWrapper->dstAVSampleFormat,
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
    audioWrapper->swrContext = swr_alloc();
    //audioWrapper->swrContext =
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
        printf("%s\n", "audioSwrContext is NULL.");
        return -1;
    }

    int ret = swr_init(audioWrapper->swrContext);
    if (ret != 0) {
        printf("%s\n", "audioSwrContext swr_init failed.");
        return -1;
    } else {
        printf("%s\n", "audioSwrContext swr_init success.");
    }

    if (audioWrapper->father->avFormatContext->duration != AV_NOPTS_VALUE) {
        int64_t duration = audioWrapper->father->avFormatContext->duration + 5000;
        // 得到的是秒数
        audioWrapper->father->duration = duration / AV_TIME_BASE;
    }

    return 0;
}

int createSwsContext() {
    videoWrapper->srcWidth = videoWrapper->father->avCodecContext->width;
    videoWrapper->srcHeight = videoWrapper->father->avCodecContext->height;
    videoWrapper->srcAVPixelFormat = videoWrapper->father->avCodecContext->pix_fmt;
    printf("---------------------------------\n");
    printf("srcWidth            : %d\n", videoWrapper->srcWidth);
    printf("srcHeight           : %d\n", videoWrapper->srcHeight);
    printf("srcAVPixelFormat    : %d\n", videoWrapper->srcAVPixelFormat);
    // 播放视频时两个必须相同
    videoWrapper->dstWidth = videoWrapper->srcWidth;
    videoWrapper->dstHeight = videoWrapper->srcHeight;
    videoWrapper->dstAVPixelFormat = videoWrapper->srcAVPixelFormat;
    printf("dstAVPixelFormat    : %d\n", videoWrapper->dstAVPixelFormat);
    videoWrapper->srcArea = videoWrapper->srcWidth * videoWrapper->srcHeight;
    videoWrapper->dstArea = videoWrapper->srcArea;

    // avPacket ---> decodedAVFrame ---> wantedAVFrame ---> 渲染画面
    //videoWrapper->father->decodedAVFrame = av_frame_alloc();
    //videoWrapper->father->dstAVFrame = av_frame_alloc();
    videoWrapper->father->decodedAVFrame = av_frame_alloc();
    videoWrapper->wantedAVFrame = av_frame_alloc();

    // srcXXX与dstXXX的参数必须要按照下面这样去设置,不然播放画面会有问题的
    // 根据视频源得到的AVPixelFormat,Width和Height计算出一帧视频所需要的空间大小
    int imageGetBufferSize = av_image_get_buffer_size(
            videoWrapper->dstAVPixelFormat, videoWrapper->srcWidth, videoWrapper->srcHeight, 1);
    videoWrapper->father->outBufferSize = imageGetBufferSize * sizeof(unsigned char);
    printf("imageGetBufferSize1 : %d\n", imageGetBufferSize);
    printf("imageGetBufferSize2 : %d\n", videoWrapper->father->outBufferSize);
    // 存储视频帧的原始数据
    videoWrapper->father->outBuffer1 = (unsigned char *) av_malloc(videoWrapper->father->outBufferSize);
    // 类似于格式化刚刚申请的内存(关联操作:wantedAVFrame, outBuffer1, dstAVPixelFormat)
    int imageFillArrays = av_image_fill_arrays(
            // uint8_t *dst_data[4]
            videoWrapper->wantedAVFrame->data,
            // int dst_linesize[4]
            videoWrapper->wantedAVFrame->linesize,
            // const uint8_t *src
            videoWrapper->father->outBuffer1,
            videoWrapper->dstAVPixelFormat,
            videoWrapper->srcWidth,
            videoWrapper->srcHeight,
            1);
    if (imageFillArrays < 0) {
        printf("imageFillArrays     : %d\n", imageFillArrays);
        return -1;
    }
    videoWrapper->swsContext = sws_getContext(
            videoWrapper->srcWidth, videoWrapper->srcHeight, videoWrapper->srcAVPixelFormat,
            videoWrapper->dstWidth, videoWrapper->dstHeight, videoWrapper->dstAVPixelFormat,
            SWS_BICUBIC,//flags
            NULL, NULL, NULL);
    if (videoWrapper->swsContext == NULL) {
        printf("%s\n", "videoSwsContext is NULL.");
        return -1;
    }
    printf("---------------------------------\n");

    if (videoWrapper->father->avFormatContext->duration != AV_NOPTS_VALUE) {
        int64_t duration = videoWrapper->father->avFormatContext->duration + 5000;
        // 得到的是秒数
        videoWrapper->father->duration = duration / AV_TIME_BASE;
    }

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

    // 暂停/继续音频回调处理。参数1表暂停,0表继续。
    // 打开音频设备后默认未启动回调处理,通过调用SDL_PauseAudio(0)来启动回调处理。
    // 这样就可以在打开音频设备后先为回调函数安全初始化数据,一切就绪后再启动音频回调。
    // 在暂停期间,会将静音值往音频设备写。
    SDL_PauseAudio(0);

    AVPacket *srcAVPacket = av_packet_alloc();
    for (;;) {
        if (threadExitFlag == 1) {
            printf("%s\n", "audioRender() break");
            break;
        }

        /*if (audioWrapper->avPacketQueue.allAVPacketsSize > MAX_AUDIO_QUEUE_SIZE) {
            SDL_Delay(10);
            continue;
        }*/

        while (1) {
            // 读取一帧压缩数据放到avPacket
            int readFrame = av_read_frame(audioWrapper->father->avFormatContext, srcAVPacket);
            //printf("readFrame           : %d\n", readFrame);
            if (readFrame < 0) {
                /*threadExitFlag = 1;
                break;*/
            }

            /*if (srcAVPacket->stream_index == audioWrapper->father->streamIndex) {
                //printf("Audio break.\n");
                audioWrapper->father->readFramesCount++;
                putAVPacketToQueue(&audioWrapper->avPacketQueue, srcAVPacket);
                break;
            }*/
        }

        // 把aac数据(压缩数据)写入文件
        // fwrite(audioWrapper->father->avPacket->data, 1, audioWrapper->father->avPacket->size, fp_aac);

        // 解码一帧压缩数据
        if (avcodec_send_packet(audioWrapper->father->avCodecContext, srcAVPacket) < 0) {
            printf("Audio Decode Error.\n");
            return -1;
        }

        // 对压缩数据进行解码,解码后的数据放到srcAVFrame(保存的是非压缩数据)
        while (avcodec_receive_frame(audioWrapper->father->avCodecContext, audioWrapper->father->decodedAVFrame) == 0) {
            /*swr_convert(audioWrapper->swrContext,
                        audioWrapper->father->dstAVFrame->data,
                        audioWrapper->father->outBufferSize,
                        (const uint8_t **) audioWrapper->father->decodedAVFrame->data,
                        audioWrapper->srcNbSamples);*/

            // 将音频的采样率转换成本机能播出的采样率
            swr_convert(audioWrapper->swrContext,
                        &audioWrapper->father->outBuffer1,
//                        audioWrapper->father->outBufferSize,
                        audioWrapper->srcNbSamples,
                        (const uint8_t **) audioWrapper->father->decodedAVFrame->data,
                        audioWrapper->srcNbSamples);

            // 在此处等待sdl_audio_callback将之前传递的音频数据播放完再向其中发送新的数据
            while (audio_len > 0) {
                SDL_Delay(1);
            }

            // 将读取到的数据存入音频缓冲区
            // 记录音频数据的长度
            audio_len = audioWrapper->father->outBufferSize;
            //audio_pos = audioWrapper->father->dstAVFrame->data[0];
            audio_pos = audioWrapper->father->outBuffer1;
        }

        // av_frame_unref(audioWrapper->father->decodedAVFrame);
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

int64_t getDuration();

void *handleVideoData(void *opaque);

static void closeAudio();

static void closeVideo();

int stop();

void *readData(void *opaque) {
    if (opaque == NULL) {
        printf("%s\n", "wrapper is NULL");
        return NULL;
    }
    Wrapper *wrapper = (Wrapper *) opaque;

    int hours, mins, seconds;
    // 得到的是秒数
    seconds = getDuration();
    mins = seconds / 60;
    seconds %= 60;
    hours = mins / 60;
    mins %= 60;
    // 00:54:16
    if (wrapper->type == TYPE_AUDIO) {
        printf("%s\n", "readData() audio start");
        // 单位: 秒
        printf("readData() audio seconds: %d %02d:%02d:%02d\n",
               (int) audioWrapper->father->duration, hours, mins, seconds);
    } else {
        printf("%s\n", "readData() video start");
        printf("readData() video seconds: %d %02d:%02d:%02d\n",
               (int) videoWrapper->father->duration, hours, mins, seconds);
    }

    AVPacket *srcAVPacket = av_packet_alloc();
    AVPacket *dstAVPacket = av_packet_alloc();

    int readFrame = -1;
    wrapper->isReading = true;
    for (;;) {
        // seekTo
        if (wrapper != NULL && wrapper->timestamp != -1) {
            if (wrapper->type == TYPE_AUDIO) {
                printf("%s\n", "readData() audio seek start");
            } else {
                printf("%s\n", "readData() video seek start");
            }

            if (!wrapper->seekToInit) {
                pthread_mutex_lock(&wrapper->readLockMutex);
                pthread_cond_wait(&wrapper->readLockCondition, &wrapper->readLockMutex);
                pthread_mutex_unlock(&wrapper->readLockMutex);
            }
            wrapper->seekToInit = false;

            AVStream *stream =
                    wrapper->avFormatContext->streams[wrapper->streamIndex];
            av_seek_frame(
                    wrapper->avFormatContext,
                    wrapper->streamIndex,
                    //wrapper->timestamp * AV_TIME_BASE,
                    wrapper->timestamp / av_q2d(stream->time_base),
                    AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);

            //千万不能调用
            //avcodec_flush_buffers(wrapper->avCodecContext);

            preProgress = 0;
            wrapper->timestamp = -1;
            av_packet_unref(srcAVPacket);
            if (wrapper->type == TYPE_AUDIO) {
                printf("%s\n", "readData() audio seek end");
            } else {
                printf("%s\n", "readData() video seek end");
            }
        }// seekTo end

        // exit
        if (!wrapper->isReading) {
            // for (;;) end
            break;
        }

        // 读一帧,如果是想要的流,那么break
        // 如果讲到文件尾,那么
        while (1) {
            // exit
            if (!wrapper->isReading) {
                // while(1) end
                break;
            }

            // 读取一帧压缩数据放到avPacket
            // 0 if OK, < 0 on error or end of file
            // 有时读一次跳出,有时读多次跳出
            //printf("readData() av_read_frame\n");
            readFrame = av_read_frame(wrapper->avFormatContext, srcAVPacket);
            /*try {
            } catch (...) {
                printf("readData() av_read_frame error\n");
                continue;
            }*/
            //printf("readFrame           : %d\n", readFrame);
            if (readFrame < 0) {
                if (readFrame == AVERROR_EOF) {
                    wrapper->isReading = false;

                    if (wrapper->type == TYPE_AUDIO) {
                        printf("readData() audio AVERROR_EOF readFrame: %d\n", readFrame);
                    } else {
                        printf("readData() video AVERROR_EOF readFrame: %d\n", readFrame);
                    }
                    if (wrapper->queue1->allAVPacketsCount > 0) {
                        wrapper->isReadQueue1Full = true;
                    }
                    if (wrapper->queue2->allAVPacketsCount > 0) {
                        wrapper->isReadQueue2Full = true;
                    }

                    // 说明歌曲长度比较短,达不到"规定值",因此处理数据线程还在等待
                    if ((wrapper->isReadQueue1Full
                         && !wrapper->isReadQueue2Full
                         && wrapper->queue1->allAVPacketsCount > 0
                         && wrapper->queue2->allAVPacketsCount == 0)
                        || wrapper->isPausedForCache) {
                        if (wrapper->type == TYPE_AUDIO) {
                            printf("readData() audio signal() handleLockCondition break\n");
                        } else {
                            printf("readData() video signal() handleLockCondition break\n");
                        }
                        // 唤醒线程
                        pthread_mutex_lock(&wrapper->handleLockMutex);
                        pthread_cond_signal(&wrapper->handleLockCondition);
                        pthread_mutex_unlock(&wrapper->handleLockMutex);
                    }

                    // while(1) end
                    break;
                }
                /*if (wrapper->type == TYPE_AUDIO) {
                    printf("readData() audio readFrame: %d\n", readFrame);
                } else {
                    printf("readData() video readFrame: %d\n", readFrame);
                }*/
                continue;
            }

            if (srcAVPacket->stream_index != wrapper->streamIndex) {
                // 遇到其他流时释放
                av_packet_unref(srcAVPacket);
                continue;
            }

            break;
        }// while(1) end

        // exit
        if (!wrapper->isReading) {
            // for (;;) end
            break;
        }

        /*if (wrapper->type == TYPE_VIDEO) {
            printf("readData() video data: %u, %u, %u, %u, %u\n",
                 srcAVPacket->data[0],
                 srcAVPacket->data[1],
                 srcAVPacket->data[2],
                 srcAVPacket->data[3],
                 srcAVPacket->data[4],
                 srcAVPacket->data[5]);
        }*/

        wrapper->readFramesCount++;
        // 非常非常非常重要
        av_copy_packet(dstAVPacket, srcAVPacket);

        if (wrapper->nextRead == NEXT_READ_QUEUE1
            && !wrapper->isReadQueue1Full) {
            putAVPacketToQueue(wrapper->queue1, dstAVPacket);
            if (wrapper->queue1->allAVPacketsCount == wrapper->maxAVPacketsCount) {
                wrapper->isReadQueue1Full = true;
                wrapper->nextRead = NEXT_READ_QUEUE2;
                if (needLog) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("readData() audio Queue1 満了\n");
                        printf("readData() audio Queue1 Size : %ld\n",
                               wrapper->queue1->allAVPacketsSize);
                        printf("readData() audio signal() handleLockCondition\n");
                    } else {
                        printf("readData() video Queue1 満了\n");
                        printf("readData() video Queue1 Size : %ld\n",
                               wrapper->queue1->allAVPacketsSize);
                        printf("readData() video signal() handleLockCondition\n");
                    }
                }
                // 唤醒线程
                pthread_mutex_lock(&wrapper->handleLockMutex);
                pthread_cond_signal(&wrapper->handleLockCondition);
                pthread_mutex_unlock(&wrapper->handleLockMutex);
            }
        } else if (wrapper->nextRead == NEXT_READ_QUEUE2
                   && !wrapper->isReadQueue2Full) {
            putAVPacketToQueue(wrapper->queue2, dstAVPacket);
            if (wrapper->queue2->allAVPacketsCount == wrapper->maxAVPacketsCount) {
                wrapper->isReadQueue2Full = true;
                wrapper->nextRead = NEXT_READ_QUEUE1;
                if (wrapper->type == TYPE_AUDIO) {
                    if (needLog) {
                        printf("readData() audio Queue2 満了\n");
                        printf("readData() audio Queue2 Size : %ld\n",
                               wrapper->queue2->allAVPacketsSize);
                    }
                    if (audioWrapper != NULL
                        && audioWrapper->father != NULL
                        && !audioWrapper->father->isPausedForCache) {
                        if (srcAVPacket->data) {
                            av_packet_unref(srcAVPacket);
                        }
                        continue;
                    }
                    if (needLog) {
                        printf("readData() audio signal() handleLockCondition\n");
                    }
                } else {
                    if (needLog) {
                        printf("readData() video Queue2 満了\n");
                        printf("readData() video Queue2 Size : %ld\n",
                               wrapper->queue2->allAVPacketsSize);
                        printf("readData() video signal() handleLockCondition\n");
                    }
                }
                // 唤醒线程
                pthread_mutex_lock(&wrapper->handleLockMutex);
                pthread_cond_signal(&wrapper->handleLockCondition);
                pthread_mutex_unlock(&wrapper->handleLockMutex);
            }
        } else if (wrapper->isReadQueue1Full
                   && wrapper->isReadQueue2Full) {
            if (needLog) {
                // 两个队列都满的话,就进行等待
                if (wrapper->type == TYPE_AUDIO) {
                    printf("readData() audio Queue1和Queue2都満了,好开心( ^_^ )\n");
                    printf("readData() audio wait() readLockCondition start\n");
                } else {
                    printf("readData() video Queue1和Queue2都満了,好开心( ^_^ )\n");
                    printf("readData() video wait() readLockCondition start\n");
                }
            }
            pthread_mutex_lock(&wrapper->readLockMutex);
            pthread_cond_wait(&wrapper->readLockCondition, &wrapper->readLockMutex);
            pthread_mutex_unlock(&wrapper->readLockMutex);
            if (needLog) {
                if (wrapper->type == TYPE_AUDIO) {
                    printf("readData() audio wait() readLockCondition end\n");
                } else {
                    printf("readData() video wait() readLockCondition end\n");
                }
            }

            // 保存"等待前的一帧"
            if (wrapper->nextRead == NEXT_READ_QUEUE1
                && !wrapper->isReadQueue1Full) {
                if (needLog) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("readData() audio next QUEUE1\n");
                    } else {
                        printf("readData() video next QUEUE1\n");
                    }
                }
                putAVPacketToQueue(wrapper->queue1, dstAVPacket);
            } else if (wrapper->nextRead == NEXT_READ_QUEUE2
                       && !wrapper->isReadQueue2Full) {
                if (needLog) {
                    if (wrapper->type == TYPE_AUDIO) {
                        printf("readData() audio next QUEUE2\n");
                    } else {
                        printf("readData() video next QUEUE2\n");
                    }
                }
                putAVPacketToQueue(wrapper->queue2, dstAVPacket);
            }
        }

        av_packet_unref(srcAVPacket);
    }// for(;;) end

    av_packet_unref(srcAVPacket);
    srcAVPacket = NULL;

    if (wrapper->type == TYPE_AUDIO) {
        printf("readData() audio readFramesCount          : %d\n",
               wrapper->readFramesCount);
        printf("%s\n", "readData() audio end");
    } else {
        printf("readData() video readFramesCount          : %d\n",
               wrapper->readFramesCount);
        printf("%s\n", "readData() video end");
    }

    return NULL;
}

void *handleAudioData(void *opaque) {
    printf("%s\n", "handleAudioData() start");

    // 等待读取一定的数据
    // 线程等待
    printf("handleAudioData() wait() start\n");
    pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
    pthread_cond_wait(&audioWrapper->father->handleLockCondition,
                      &audioWrapper->father->handleLockMutex);
    pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
    printf("handleAudioData() wait() end\n");

    // 调用后会马上回调sdlAudioCallback(...)
    if (initAudioSDL() < 0) {
        closeAudio();
        return NULL;
    }

    audioWrapper->father->isHandling = true;

    printf("%s\n", "handleAudioData() end");
    return NULL;
}

void *handleVideoData(void *opaque) {
    printf("%s\n", "handleVideoData() start");

    // 线程等待
    printf("handleVideoData() wait() start\n");
    pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
    pthread_cond_wait(&videoWrapper->father->handleLockCondition,
                      &videoWrapper->father->handleLockMutex);
    pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
    printf("handleVideoData() wait() end\n");

    audioTimeDifference = 0;
    videoTimeDifference = 0;
    totalTimeDifference = 0;
    totalTimeDifferenceCount = 0;

    videoSleep = 0;
    sleepStep = 0;
    long tempSleep = 0;
    preProgress = 0;
    int64_t prePts = 0;
    int64_t nowPts = 0;
    double timeDifference = 0;
    int ret = 0;
    bool onlyOne = true;

    AVStream *stream =
            videoWrapper->father->avFormatContext->streams[videoWrapper->father->streamIndex];
    // 必须创建(存放压缩数据,如H264)
    AVPacket *avPacket = av_packet_alloc();
    videoWrapper->father->isHandling = true;
    printf("handleVideoData() for (;;) start\n");
    for (;;) {
        if (videoWrapper->father->isPausedForUser
            || videoWrapper->father->isPausedForCache) {
            bool isPausedForUser = videoWrapper->father->isPausedForUser;
            if (isPausedForUser) {
                printf("handleVideoData() wait() User  start\n");
            } else {
                printf("handleVideoData() wait() Cache start\n");
            }
            pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
            pthread_cond_wait(&videoWrapper->father->handleLockCondition,
                              &videoWrapper->father->handleLockMutex);
            pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
            if (isPausedForUser) {
                printf("handleVideoData() wait() User  end\n");
            } else {
                printf("handleVideoData() wait() Cache end\n");
            }
        }

        if (!videoWrapper->father->isHandling) {
            // for (;;) end
            break;
        }

        memset(avPacket, 0, sizeof(*avPacket));
        if (videoWrapper->father->nextHandle == NEXT_HANDLE_QUEUE1
            && videoWrapper->father->isReadQueue1Full
            && videoWrapper->father->queue1->allAVPacketsCount > 0) {
            videoWrapper->father->handleFramesCount++;
            getAVPacketFromQueue(videoWrapper->father->queue1, avPacket);
            if (videoWrapper->father->queue1->allAVPacketsCount == 0) {
                memset(videoWrapper->father->queue1, 0, sizeof(struct AVPacketQueue));
                videoWrapper->father->isReadQueue1Full = false;
                videoWrapper->father->nextHandle = NEXT_HANDLE_QUEUE2;
                if (needLog) {
                    printf("handleVideoData() Queue1 用完了\n");
                    printf("handleVideoData() Queue2 isReadQueue2Full : %d\n",
                           videoWrapper->father->isReadQueue2Full);
                    printf("handleVideoData() Queue2 allAVPacketsCount: %d\n",
                           videoWrapper->father->queue2->allAVPacketsCount);
                }
                if (videoWrapper->father->isReading) {
                    if (needLog) {
                        printf("handleVideoData() signal() readLockCondition\n");
                    }
                    pthread_mutex_lock(&videoWrapper->father->readLockMutex);
                    pthread_cond_signal(&videoWrapper->father->readLockCondition);
                    pthread_mutex_unlock(&videoWrapper->father->readLockMutex);
                }
            }
        } else if (videoWrapper->father->nextHandle == NEXT_HANDLE_QUEUE2
                   && videoWrapper->father->isReadQueue2Full
                   && videoWrapper->father->queue2->allAVPacketsCount > 0) {
            videoWrapper->father->handleFramesCount++;
            getAVPacketFromQueue(videoWrapper->father->queue2, avPacket);
            if (videoWrapper->father->queue2->allAVPacketsCount == 0) {
                memset(videoWrapper->father->queue2, 0, sizeof(struct AVPacketQueue));
                videoWrapper->father->isReadQueue2Full = false;
                videoWrapper->father->nextHandle = NEXT_HANDLE_QUEUE1;
                if (needLog) {
                    printf("handleVideoData() Queue2 用完了\n");
                    printf("handleVideoData() Queue1 isReadQueue1Full : %d\n",
                           videoWrapper->father->isReadQueue1Full);
                    printf("handleVideoData() Queue1 allAVPacketsCount: %d\n",
                           videoWrapper->father->queue1->allAVPacketsCount);
                }
                if (videoWrapper->father->isReading) {
                    if (needLog) {
                        printf("handleVideoData() signal() readLockCondition\n");
                    }
                    pthread_mutex_lock(&videoWrapper->father->readLockMutex);
                    pthread_cond_signal(&videoWrapper->father->readLockCondition);
                    pthread_mutex_unlock(&videoWrapper->father->readLockMutex);
                }
            }
        } else if (videoWrapper->father->isReading
                   && !videoWrapper->father->isReadQueue1Full
                   && !videoWrapper->father->isReadQueue2Full) {
            //onPaused();
            // 视频Cache引起的暂停
#ifdef USE_AUDIO
            // 让音频也同时暂停
            if (audioWrapper != NULL && audioWrapper->father != NULL) {
                audioWrapper->father->isPausedForCache = true;
            }
#endif
            videoWrapper->father->isPausedForCache = true;
            printf("handleVideoData() wait() Cache start\n");
            pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
            pthread_cond_wait(&videoWrapper->father->handleLockCondition,
                              &videoWrapper->father->handleLockMutex);
            pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
            printf("handleVideoData() wait() Cache end\n");
            videoWrapper->father->isPausedForCache = false;
            // 通知音频结束暂停
#ifdef USE_AUDIO
            if (audioWrapper != NULL && audioWrapper->father != NULL) {
                audioWrapper->father->isPausedForCache = false;
                pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
                pthread_cond_signal(&audioWrapper->father->handleLockCondition);
                pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
            }
#endif
            //onPlayed();
            continue;
        } else if (!videoWrapper->father->isReading
                   && videoWrapper->father->queue1->allAVPacketsCount == 0
                   && videoWrapper->father->queue2->allAVPacketsCount == 0) {
            videoWrapper->father->isHandling = false;
            //onProgressUpdated(videoWrapper->father->duration);
            printf("handleVideoData() 电影结束,散场\n");
            // for (;;) end
            break;
        }

        if (!avPacket) {
            if (!videoWrapper->father->isReading
                && videoWrapper->father->queue1->allAVPacketsCount == 0
                && videoWrapper->father->queue2->allAVPacketsCount == 0) {
                // for (;;) end
                break;
            }
            continue;
        }

        // 发送压缩数据去进行解码
        ret = avcodec_send_packet(videoWrapper->father->avCodecContext, avPacket);
        switch (ret) {
            case AVERROR(EAGAIN):
                continue;
            case AVERROR(EINVAL):
            case AVERROR(ENOMEM):
            case AVERROR_EOF:
                printf("video 发送数据包到解码器时出错 %d", ret);
                videoWrapper->father->isHandling = false;
                break;
            case 0:
            default:
                break;
        }
        if (!videoWrapper->father->isHandling) {
            // for (;;) end
            break;
        }
        // 对压缩数据进行解码,解码后的数据放到decodedAVFrame(保存的是非压缩数据)
        while (1) {
            ret = avcodec_receive_frame(videoWrapper->father->avCodecContext,
                                        videoWrapper->father->decodedAVFrame);
            switch (ret) {
                case AVERROR(EAGAIN):
                    break;
                case AVERROR(EINVAL):
                case AVERROR_EOF:
                    videoWrapper->father->isHandling = false;
                    break;
                case 0: {
                    nowPts = videoWrapper->father->decodedAVFrame->pts;
                    // 0.040000
                    timeDifference = (nowPts - prePts) * av_q2d(stream->time_base);
                    prePts = nowPts;
                    videoTimeDifference = nowPts * av_q2d(stream->time_base);
                    //printf("handleVideoData() nowPts : %lf\n", videoTimeDifference);
                    long progress = (long) videoTimeDifference;
                    if (progress > preProgress) {
                        preProgress = progress;
                        int hours, mins, seconds;
                        // 得到的是秒数
                        seconds = progress;
                        mins = seconds / 60;
                        seconds %= 60;
                        hours = mins / 60;
                        mins %= 60;
                        //printf("handleVideoData() video 已播放时间: %02d:%02d:%02d\n", hours, mins, seconds);
                        //onProgressUpdated(progress);
                    }

                    if (!videoWrapper->father->isStarted) {
                        videoWrapper->father->isStarted = true;
                    }
#ifdef USE_AUDIO
                    while (audioWrapper != NULL
                           && audioWrapper->father != NULL
                           && !audioWrapper->father->isStarted) {
                        usleep(1000);
                    }
                    if (audioWrapper != NULL
                        && audioWrapper->father != NULL
                        && audioWrapper->father->isStarted
                        && videoWrapper->father->isStarted
                        && onlyOne) {
                        printf("handleVideoData() 音视频都已经准备好,开始播放!!!\n");
                        onlyOne = false;
                        //onPlayed();
                    }

                    // 正常情况下videoTimeDifference比audioTimeDifference大一些
                    // 如果发现小了,说明视频播放慢了,应丢弃这些帧
                    if (videoTimeDifference < audioTimeDifference) {
                        // break后videoTimeDifference增长的速度会加快
                        //LOGD("handleVideoData() audio nowPts : %lf\n", audioTimeDifference);
                        //LOGW("handleVideoData() video nowPts : %lf\n", videoTimeDifference);
                        // switch end
                        break;
                    }
                    // 0.177853 0.155691 0.156806 0.154362
                    double tempTimeDifference = videoTimeDifference - audioTimeDifference;
                    if (tempTimeDifference > 2.000000) {
                        // 不好的现象
                        printf("handleVideoData() video - audio   : %lf\n", tempTimeDifference);
                    }
                    // 如果videoTimeDifference比audioTimeDifference大出了一定的范围
                    // 那么说明视频播放快了,应等待音频
                    while (videoTimeDifference - audioTimeDifference > TIME_DIFFERENCE) {
                        usleep(1000);
                    }
#endif
                    // 进行格式的转换,转换后的数据放在wantedAVFrame
                    // 对于packed格式的数据（例如RGB24）,会存到data[0]里面。
                    // 对于planar格式的数据（例如YUV420P）,则会分开成data[0],data[1],data[2]...
                    // （YUV420P中data[0]存Y,data[1]存U,data[2]存V）
                    sws_scale(videoWrapper->swsContext,
                              (const unsigned char *const *) videoWrapper->father->decodedAVFrame->data,
                              videoWrapper->father->decodedAVFrame->linesize,
                              0,
                              videoWrapper->srcHeight,
                              videoWrapper->wantedAVFrame->data,
                              videoWrapper->wantedAVFrame->linesize);

                    // 睡眠是为了让视频看起来流畅,自然.更音视频同步没有关系
                    // 单位: 毫秒
                    tempSleep = timeDifference * 1000;
                    //tempSleep -= 30;
                    tempSleep += sleepStep;
                    if (videoSleep != tempSleep) {
                        videoSleep = tempSleep;
                        printf("handleVideoData() sleep  : %ld\n", videoSleep);
                    }
                    if (videoSleep < 100 && videoSleep > 0) {
                        usleep(videoSleep * 1000);
                    } else {
                        if (videoSleep > 0) {
                            // 好像是个比较合理的值
                            usleep(40 * 1000);
                        }
                        // sleep <= 0时不需要sleep
                    }

                    // SDL Start---------------------
                    // sws_scale()函数不调用,直接播放decodedAVFrame中的数据也可以,只是画质没有转换过的好
                    SDL_UpdateTexture(sdlTexture, NULL,
                                      videoWrapper->wantedAVFrame->data[0],
                                      videoWrapper->wantedAVFrame->linesize[0]);
                    SDL_RenderClear(sdlRenderer);
                    //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
                    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
                    SDL_RenderPresent(sdlRenderer);
                    // SDL End-----------------------

                    // switch end
                    break;
                }
                default:
                    break;
            }// switch end
            break;
        }// while(1) end
        av_packet_unref(avPacket);
    }// for(;;) end
    printf("handleVideoData() for (;;) end\n");

    av_packet_unref(avPacket);
    avPacket = NULL;

    printf("handleVideoData() video handleFramesCount : %d\n",
           videoWrapper->father->handleFramesCount);
    if (videoWrapper->father->readFramesCount == videoWrapper->father->handleFramesCount) {
        printf("%s\n", "handleVideoData() video正常播放完毕");
    }
    stop();
    closeVideo();

    //onFinished();

    printf("%s\n", "handleVideoData() end");
    return NULL;
}

static void closeAudio() {
    SDL_CloseAudio();
    printf("%s\n", "closeAudio() start");
    // audio
    if (audioWrapper->father->avPacket != NULL) {
        av_packet_unref(audioWrapper->father->avPacket);
        audioWrapper->father->avPacket = NULL;
    }
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
    if (audioWrapper->father->srcData[0] != NULL) {
        av_freep(&audioWrapper->father->srcData[0]);
        audioWrapper->father->srcData[0] = NULL;
    }
    if (audioWrapper->father->dstData[0] != NULL) {
        av_freep(&audioWrapper->father->dstData[0]);
        audioWrapper->father->dstData[0] = NULL;
    }
    if (audioWrapper->swrContext != NULL) {
        swr_free(&audioWrapper->swrContext);
        audioWrapper->swrContext = NULL;
    }
    if (audioWrapper->father->decodedAVFrame != NULL) {
        av_frame_free(&audioWrapper->father->decodedAVFrame);
        audioWrapper->father->decodedAVFrame = NULL;
    }
    if (audioWrapper->father->dstAVFrame != NULL) {
        av_frame_free(&audioWrapper->father->dstAVFrame);
        audioWrapper->father->dstAVFrame = NULL;
    }
    if (audioWrapper->father->avCodecParameters != NULL) {
        avcodec_parameters_free(&audioWrapper->father->avCodecParameters);
        audioWrapper->father->avCodecParameters = NULL;
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
    int count = audioWrapper->father->queue1->allAVPacketsCount;
    if (count > 0) {
        AVPacket *avPacket = av_packet_alloc();
        for (int i = 0; i < count; i++) {
            getAVPacketFromQueue(audioWrapper->father->queue1, avPacket);
            av_packet_unref(avPacket);
        }
        av_packet_unref(avPacket);
    }
    count = audioWrapper->father->queue2->allAVPacketsCount;
    if (count > 0) {
        AVPacket *avPacket = av_packet_alloc();
        for (int i = 0; i < count; i++) {
            getAVPacketFromQueue(audioWrapper->father->queue2, avPacket);
            av_packet_unref(avPacket);
        }
        av_packet_unref(avPacket);
    }
    av_free(audioWrapper->father->queue1);
    av_free(audioWrapper->father->queue2);
    audioWrapper->father->queue1 = NULL;
    audioWrapper->father->queue2 = NULL;
    av_free(audioWrapper->father);
    audioWrapper->father = NULL;
    av_free(audioWrapper);
    audioWrapper = NULL;

    printf("%s\n", "closeAudio() end");
}

static void closeVideo() {
    printf("%s\n", "closeVideo() start");
    // 清理工作
    if (sdlWindow != NULL) {
        SDL_Quit();
        sdlWindow = NULL;
    }
    if (sdlThread != NULL) {
        SDL_DetachThread(sdlThread);
        sdlThread = NULL;
    }
    // video
    if (videoWrapper->father->avPacket != NULL) {
        av_packet_unref(videoWrapper->father->avPacket);
        videoWrapper->father->avPacket = NULL;
    }
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
    if (videoWrapper->father->srcData[0] != NULL) {
        av_freep(&videoWrapper->father->srcData[0]);
        videoWrapper->father->srcData[0] = NULL;
    }
    if (videoWrapper->father->dstData[0] != NULL) {
        av_freep(&videoWrapper->father->dstData[0]);
        videoWrapper->father->dstData[0] = NULL;
    }
    if (videoWrapper->swsContext != NULL) {
        sws_freeContext(videoWrapper->swsContext);
        videoWrapper->swsContext = NULL;
    }
    if (videoWrapper->father->decodedAVFrame != NULL) {
        av_frame_free(&videoWrapper->father->decodedAVFrame);
        videoWrapper->father->decodedAVFrame = NULL;
    }
    if (videoWrapper->father->dstAVFrame != NULL) {
        av_frame_free(&videoWrapper->father->dstAVFrame);
        videoWrapper->father->dstAVFrame = NULL;
    }
    if (videoWrapper->wantedAVFrame != NULL) {
        av_frame_free(&videoWrapper->wantedAVFrame);
        videoWrapper->wantedAVFrame = NULL;
    }
    if (videoWrapper->father->avCodecParameters != NULL) {
        avcodec_parameters_free(&videoWrapper->father->avCodecParameters);
        videoWrapper->father->avCodecParameters = NULL;
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
    int count = videoWrapper->father->queue1->allAVPacketsCount;
    if (count > 0) {
        AVPacket *avPacket = av_packet_alloc();
        for (int i = 0; i < count; i++) {
            getAVPacketFromQueue(videoWrapper->father->queue1, avPacket);
            av_packet_unref(avPacket);
        }
        av_packet_unref(avPacket);
    }
    count = videoWrapper->father->queue2->allAVPacketsCount;
    if (count > 0) {
        AVPacket *avPacket = av_packet_alloc();
        for (int i = 0; i < count; i++) {
            getAVPacketFromQueue(videoWrapper->father->queue2, avPacket);
            av_packet_unref(avPacket);
        }
        av_packet_unref(avPacket);
    }
    av_free(videoWrapper->father->queue1);
    av_free(videoWrapper->father->queue2);
    videoWrapper->father->queue1 = NULL;
    videoWrapper->father->queue2 = NULL;
    av_free(videoWrapper->father);
    videoWrapper->father = NULL;
    av_free(videoWrapper);
    videoWrapper = NULL;

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
    printf("%s\n", "closeVideo() end");
}

void avDumpFormat() {
#if AV_DUMP_FORMAT
    printf("-------------File Information-------------\n");
    av_dump_format(audioWrapper->father->avFormatContext, 0, inFilePath2, 0);
    printf("------------------------------------------\n");
    av_dump_format(videoWrapper->father->avFormatContext, 0, inFilePath2, 0);
    printf("------------------------------------------\n");
#endif
}

int play() {
#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        audioWrapper->father->isPausedForUser = false;
        pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
        pthread_cond_signal(&audioWrapper->father->handleLockCondition);
        pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        videoWrapper->father->isPausedForUser = false;
        pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
        pthread_cond_signal(&videoWrapper->father->handleLockCondition);
        pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
    }
#endif
    return 0;
}

int pause() {
#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        audioWrapper->father->isPausedForUser = true;
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        videoWrapper->father->isPausedForUser = true;
    }
#endif
    return 0;
}

int stop() {
#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        printf("stop() audio start\n");
        audioWrapper->father->isStarted = false;
        audioWrapper->father->isReading = false;
        audioWrapper->father->isHandling = false;
        audioWrapper->father->isPausedForUser = false;
        audioWrapper->father->isPausedForCache = false;
        audioWrapper->father->isReadQueue1Full = false;
        audioWrapper->father->isReadQueue2Full = false;
        //
        pthread_mutex_lock(&audioWrapper->father->readLockMutex);
        pthread_cond_signal(&audioWrapper->father->readLockCondition);
        pthread_mutex_unlock(&audioWrapper->father->readLockMutex);
        //
        pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
        pthread_cond_signal(&audioWrapper->father->handleLockCondition);
        pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
        printf("stop() audio end\n");
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        printf("stop() video start\n");
        videoWrapper->father->isStarted = false;
        videoWrapper->father->isReading = false;
        videoWrapper->father->isHandling = false;
        videoWrapper->father->isPausedForUser = false;
        videoWrapper->father->isPausedForCache = false;
        videoWrapper->father->isReadQueue1Full = false;
        videoWrapper->father->isReadQueue2Full = false;
        //
        pthread_mutex_lock(&videoWrapper->father->readLockMutex);
        pthread_cond_signal(&videoWrapper->father->readLockCondition);
        pthread_mutex_unlock(&videoWrapper->father->readLockMutex);
        //
        pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
        pthread_cond_signal(&videoWrapper->father->handleLockCondition);
        pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
        printf("stop() video end\n");
    }
#endif
    return 0;
}

int release() {
    stop();
    return 0;
}

bool isRunning() {
    bool audioRunning = false;
    bool videoRunning = false;
#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        audioRunning = audioWrapper->father->isReading
                       || audioWrapper->father->isHandling;
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        videoRunning = videoWrapper->father->isReading
                       || videoWrapper->father->isHandling;
    }
#endif
    return audioRunning || videoRunning;
}

bool isPlaying() {
    bool audioPlaying = false;
    bool videoPlaying = false;
#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        audioPlaying = audioWrapper->father->isStarted
                       && audioWrapper->father->isHandling
                       && !audioWrapper->father->isPausedForUser
                       && !audioWrapper->father->isPausedForCache;
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        videoPlaying = videoWrapper->father->isStarted
                       && videoWrapper->father->isHandling
                       && !videoWrapper->father->isPausedForUser
                       && !videoWrapper->father->isPausedForCache;
    }
#endif
    return audioPlaying && videoPlaying;
}

int seekTo(int64_t timestamp) {
    printf("==================================================================\n");
    printf("seekTo() timestamp: %ld\n", timestamp);
#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        audioWrapper->father->timestamp = timestamp;
        pthread_mutex_lock(&audioWrapper->father->readLockMutex);
        pthread_cond_signal(&audioWrapper->father->readLockCondition);
        pthread_mutex_unlock(&audioWrapper->father->readLockMutex);

        audioWrapper->father->nextRead = NEXT_READ_QUEUE1;
        audioWrapper->father->nextHandle = NEXT_HANDLE_QUEUE1;
        audioWrapper->father->isReadQueue1Full = false;
        audioWrapper->father->isReadQueue2Full = false;

        // 如果audio处理线程由于Cache原因而暂停,那么不用处理,继续让它暂停好了
        // 如果audio处理线程由于User原因而暂停,那么需要通知它,使它变成由于Cache原因而暂停
        if (audioWrapper->father->isPausedForUser) {
            audioWrapper->father->isPausedForUser = false;
            pthread_mutex_lock(&audioWrapper->father->handleLockMutex);
            pthread_cond_signal(&audioWrapper->father->handleLockCondition);
            pthread_mutex_unlock(&audioWrapper->father->handleLockMutex);
        }

        // 把Queue1和Queue2队列清空
        int count = audioWrapper->father->queue1->allAVPacketsCount;
        if (count > 0) {
            AVPacket *avPacket = av_packet_alloc();
            for (int i = 0; i < count; i++) {
                getAVPacketFromQueue(audioWrapper->father->queue1, avPacket);
                av_packet_unref(avPacket);
            }
            av_packet_unref(avPacket);
        }
        audioWrapper->father->queue1->firstAVPacketList = NULL;
        audioWrapper->father->queue1->lastAVPacketList = NULL;
        audioWrapper->father->queue1->allAVPacketsCount = 0;
        audioWrapper->father->queue1->allAVPacketsSize = 0;
        memset(audioWrapper->father->queue1, 0, sizeof(struct AVPacketQueue));
        count = audioWrapper->father->queue2->allAVPacketsCount;
        if (count > 0) {
            AVPacket *avPacket = av_packet_alloc();
            for (int i = 0; i < count; i++) {
                getAVPacketFromQueue(audioWrapper->father->queue2, avPacket);
                av_packet_unref(avPacket);
            }
            av_packet_unref(avPacket);
        }
        audioWrapper->father->queue2->firstAVPacketList = NULL;
        audioWrapper->father->queue2->lastAVPacketList = NULL;
        audioWrapper->father->queue2->allAVPacketsCount = 0;
        audioWrapper->father->queue2->allAVPacketsSize = 0;
        memset(audioWrapper->father->queue2, 0, sizeof(struct AVPacketQueue));

        // 通知开始seek
        printf("seekTo() audio signal() handleLockCondition\n");
        pthread_mutex_lock(&audioWrapper->father->readLockMutex);
        pthread_cond_signal(&audioWrapper->father->readLockCondition);
        pthread_mutex_unlock(&audioWrapper->father->readLockMutex);

        audioWrapper->father->seekToInit = true;
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        videoWrapper->father->timestamp = timestamp;
        pthread_mutex_lock(&videoWrapper->father->readLockMutex);
        pthread_cond_signal(&videoWrapper->father->readLockCondition);
        pthread_mutex_unlock(&videoWrapper->father->readLockMutex);

        videoWrapper->father->nextRead = NEXT_READ_QUEUE1;
        videoWrapper->father->nextHandle = NEXT_HANDLE_QUEUE1;
        videoWrapper->father->isReadQueue1Full = false;
        videoWrapper->father->isReadQueue2Full = false;

        // 如果video处理线程由于Cache原因而暂停,那么不用处理,继续让它暂停好了
        // 如果video处理线程由于User原因而暂停,那么需要通知它,使它变成由于Cache原因而暂停
        if (videoWrapper->father->isPausedForUser) {
            videoWrapper->father->isPausedForUser = false;
            pthread_mutex_lock(&videoWrapper->father->handleLockMutex);
            pthread_cond_signal(&videoWrapper->father->handleLockCondition);
            pthread_mutex_unlock(&videoWrapper->father->handleLockMutex);
        }

        // 把Queue1和Queue2队列清空
        int count = videoWrapper->father->queue1->allAVPacketsCount;
        if (count > 0) {
            AVPacket *avPacket = av_packet_alloc();
            for (int i = 0; i < count; i++) {
                getAVPacketFromQueue(videoWrapper->father->queue1, avPacket);
                av_packet_unref(avPacket);
            }
            av_packet_unref(avPacket);
        }
        videoWrapper->father->queue1->firstAVPacketList = NULL;
        videoWrapper->father->queue1->lastAVPacketList = NULL;
        videoWrapper->father->queue1->allAVPacketsCount = 0;
        videoWrapper->father->queue1->allAVPacketsSize = 0;
        memset(videoWrapper->father->queue1, 0, sizeof(struct AVPacketQueue));
        count = videoWrapper->father->queue2->allAVPacketsCount;
        if (count > 0) {
            AVPacket *avPacket = av_packet_alloc();
            for (int i = 0; i < count; i++) {
                getAVPacketFromQueue(videoWrapper->father->queue2, avPacket);
                av_packet_unref(avPacket);
            }
            av_packet_unref(avPacket);
        }
        videoWrapper->father->queue2->firstAVPacketList = NULL;
        videoWrapper->father->queue2->lastAVPacketList = NULL;
        videoWrapper->father->queue2->allAVPacketsCount = 0;
        videoWrapper->father->queue2->allAVPacketsSize = 0;
        memset(videoWrapper->father->queue2, 0, sizeof(struct AVPacketQueue));

        // 通知开始seek
        printf("seekTo() video signal() handleLockCondition\n");
        pthread_mutex_lock(&videoWrapper->father->readLockMutex);
        pthread_cond_signal(&videoWrapper->father->readLockCondition);
        pthread_mutex_unlock(&videoWrapper->father->readLockMutex);

        videoWrapper->father->seekToInit = true;
    }
#endif
    return 0;
}

// 返回值单位是秒
int64_t getDuration() {
    int64_t audioDuration = 0;
    int64_t videoDuration = 0;
    int64_t duration = 0;

#ifdef USE_AUDIO
    if (audioWrapper != NULL && audioWrapper->father != NULL) {
        audioDuration = audioWrapper->father->duration;
    }
#endif
#ifdef USE_VIDEO
    if (videoWrapper != NULL && videoWrapper->father != NULL) {
        videoDuration = videoWrapper->father->duration;
    }
#endif
    if (audioDuration != 0 && videoDuration != 0) {
        duration = audioDuration > videoDuration ? videoDuration : audioDuration;
    } else if (audioDuration != 0 && videoDuration == 0) {
        duration = audioDuration;
    } else if (audioDuration == 0 && videoDuration != 0) {
        duration = videoDuration;
    } else {
        duration = 0;
    }

    return duration;
}

void stepAdd() {
    sleepStep++;
    char dest[50];
    sprintf(dest, "videoSleep: %ld, sleepStep: %ld\n", videoSleep, sleepStep);
    printf("stepAdd()      videoSleep: %ld, sleepStep: %ld\n", videoSleep, sleepStep);
}

void stepSubtract() {
    sleepStep--;
    char dest[50];
    sprintf(dest, "videoSleep: %ld, sleepStep: %ld\n", videoSleep, sleepStep);
    printf("stepSubtract() videoSleep: %ld, sleepStep: %ld\n", videoSleep, sleepStep);
}

// 已经不需要调用了
void initAV() {
    av_register_all();
    // 用于从网络接收数据,如果不是网络接收数据,可不用（如本例可不用）
    avcodec_register_all();
    // 注册复用器和编解码器,所有的使用ffmpeg,首先必须调用这个函数
    avformat_network_init();
    // 注册设备的函数,如用获取摄像头数据或音频等,需要此函数先注册
    // avdevice_register_all();
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
    audioWrapper = (struct AudioWrapper *) av_mallocz(sizeof(struct AudioWrapper));
    memset(audioWrapper, 0, sizeof(struct AudioWrapper));
    audioWrapper->father = (struct Wrapper *) av_mallocz(sizeof(struct Wrapper));
    memset(audioWrapper->father, 0, sizeof(struct Wrapper));

    audioWrapper->father->type = TYPE_AUDIO;
    audioWrapper->father->nextRead = NEXT_READ_QUEUE1;
    audioWrapper->father->nextHandle = NEXT_HANDLE_QUEUE1;
    if (isLocal) {
        audioWrapper->father->maxAVPacketsCount = MAX_AVPACKET_COUNT_AUDIO_LOCAL;
    } else {
        audioWrapper->father->maxAVPacketsCount = MAX_AVPACKET_COUNT_AUDIO_HTTP;
    }
    printf("initAudio() maxAVPacketsCount: %d\n", audioWrapper->father->maxAVPacketsCount);

    audioWrapper->father->streamIndex = -1;
    audioWrapper->father->readFramesCount = 0;
    audioWrapper->father->handleFramesCount = 0;
    audioWrapper->father->isStarted = false;
    audioWrapper->father->isReading = false;
    audioWrapper->father->isHandling = false;
    audioWrapper->father->isPausedForUser = false;
    audioWrapper->father->isPausedForCache = false;
    audioWrapper->father->seekToInit = false;
    audioWrapper->father->isReadQueue1Full = false;
    audioWrapper->father->isReadQueue2Full = false;
    audioWrapper->father->duration = -1;
    audioWrapper->father->timestamp = -1;

    audioWrapper->dstSampleRate = 44100;
    audioWrapper->dstAVSampleFormat = AV_SAMPLE_FMT_S16;
    audioWrapper->dstChannelLayout = AV_CH_LAYOUT_STEREO;

    audioWrapper->decodedDataSize = 0;
    audioWrapper->decodedDataSizeIndex = 0;

    audioWrapper->father->queue1 =
            (struct AVPacketQueue *) av_mallocz(sizeof(struct AVPacketQueue));
    audioWrapper->father->queue2 =
            (struct AVPacketQueue *) av_mallocz(sizeof(struct AVPacketQueue));
    memset(audioWrapper->father->queue1, 0, sizeof(struct AVPacketQueue));
    memset(audioWrapper->father->queue2, 0, sizeof(struct AVPacketQueue));
    audioWrapper->father->queue1->allAVPacketsCount = 0;
    audioWrapper->father->queue1->allAVPacketsSize = 0;
    audioWrapper->father->queue2->allAVPacketsCount = 0;
    audioWrapper->father->queue2->allAVPacketsSize = 0;

    audioWrapper->father->readLockMutex = PTHREAD_MUTEX_INITIALIZER;
    audioWrapper->father->readLockCondition = PTHREAD_COND_INITIALIZER;
    audioWrapper->father->handleLockMutex = PTHREAD_MUTEX_INITIALIZER;
    audioWrapper->father->handleLockCondition = PTHREAD_COND_INITIALIZER;
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
    videoWrapper->father->nextRead = NEXT_READ_QUEUE1;
    videoWrapper->father->nextHandle = NEXT_HANDLE_QUEUE1;
    if (isLocal) {
        videoWrapper->father->maxAVPacketsCount = MAX_AVPACKET_COUNT_VIDEO_LOCAL;
    } else {
        videoWrapper->father->maxAVPacketsCount = MAX_AVPACKET_COUNT_VIDEO_HTTP;
    }
    printf("initVideo() maxAVPacketsCount: %d\n", videoWrapper->father->maxAVPacketsCount);

    videoWrapper->father->streamIndex = -1;
    videoWrapper->father->readFramesCount = 0;
    videoWrapper->father->handleFramesCount = 0;
    videoWrapper->father->isStarted = false;
    videoWrapper->father->isReading = false;
    videoWrapper->father->isHandling = false;
    videoWrapper->father->isPausedForUser = false;
    videoWrapper->father->isPausedForCache = false;
    videoWrapper->father->seekToInit = false;
    videoWrapper->father->isReadQueue1Full = false;
    videoWrapper->father->isReadQueue2Full = false;
    videoWrapper->father->duration = -1;
    videoWrapper->father->timestamp = -1;
    videoWrapper->dstWidth = 720, videoWrapper->dstHeight = 360;

    // Android支持的目标像素格式
    // AV_PIX_FMT_RGB32
    // videoWrapper->dstAVPixelFormat = AV_PIX_FMT_RGBA;
    videoWrapper->dstAVPixelFormat = AV_PIX_FMT_RGB24;

    videoWrapper->father->queue1 =
            (struct AVPacketQueue *) av_mallocz(sizeof(struct AVPacketQueue));
    videoWrapper->father->queue2 =
            (struct AVPacketQueue *) av_mallocz(sizeof(struct AVPacketQueue));
    memset(videoWrapper->father->queue1, 0, sizeof(struct AVPacketQueue));
    memset(videoWrapper->father->queue2, 0, sizeof(struct AVPacketQueue));
    videoWrapper->father->queue1->allAVPacketsCount = 0;
    videoWrapper->father->queue1->allAVPacketsSize = 0;
    videoWrapper->father->queue2->allAVPacketsCount = 0;
    videoWrapper->father->queue2->allAVPacketsSize = 0;

    videoWrapper->father->readLockMutex = PTHREAD_MUTEX_INITIALIZER;
    videoWrapper->father->readLockCondition = PTHREAD_COND_INITIALIZER;
    videoWrapper->father->handleLockMutex = PTHREAD_MUTEX_INITIALIZER;
    videoWrapper->father->handleLockCondition = PTHREAD_COND_INITIALIZER;
}

int initAudioPlayer() {
    printf("%s\n", "initAudioPlayer() start");

    // audio
    initAudio();
    if (openAndFindAVFormatContextForAudio() < 0) {
        printf("openAndFindAVFormatContextForAudio() failed\n");
        closeAudio();
        return -1;
    }
    if (findStreamIndexForAudio() < 0) {
        printf("findStreamIndexForAudio() failed\n");
        closeAudio();
        return -1;
    }
    if (findAndOpenAVCodecForAudio() < 0) {
        printf("findAndOpenAVCodecForAudio() failed\n");
        closeAudio();
        return -1;
    }
    if (createSwrContent() < 0) {
        printf("createSwrContent() failed\n");
        closeAudio();
        return -1;
    }

    printf("%s\n", "initAudioPlayer() end");
    return 0;
}

int initVideoPlayer() {
    printf("%s\n", "initVideoPlayer() start");

    // video
    initVideo();
    if (openAndFindAVFormatContextForVideo() < 0) {
        printf("openAndFindAVFormatContextForVideo() failed\n");
        closeVideo();
        return -1;
    }
    if (findStreamIndexForVideo() < 0) {
        printf("findStreamIndexForVideo() failed\n");
        closeVideo();
        return -1;
    }
    if (findAndOpenAVCodecForVideo() < 0) {
        printf("findAndOpenAVCodecForVideo() failed\n");
        closeVideo();
        return -1;
    }
    if (createSwsContext() < 0) {
        printf("createSwsContext() failed\n");
        closeVideo();
        return -1;
    }
    if (initVideoSDL() < 0) {
        closeVideo();
        return -1;
    }

    printf("%s\n", "initVideoPlayer() end");
    return 0;
}

/***

 */
int alexanderAudioPlayerWithSDL() {
    printf("%s\n", "alexanderAudioPlayerWithSDL() start");

    printf("%s\n", "alexanderAudioPlayerWithSDL() end");
}

/***
 run this method and playback video
 */
int alexanderVideoPlayerWithSDL() {
    printf("%s\n", "alexanderVideoPlayerWithSDL() start");

    char *result = strstr(inFilePath2, "http://");
    if (result == NULL) {
        result = strstr(inFilePath2, "https://");
        if (result == NULL) {
            result = strstr(inFilePath2, "HTTP://");
            if (result == NULL) {
                result = strstr(inFilePath2, "HTTPS://");
                if (result == NULL) {
                    isLocal = true;
                }
            }
        }
    }
    printf("alexanderVideoPlayerWithSDL() isLocal : %d\n", isLocal);

    initSDL();

#ifdef USE_AUDIO
    // audio
    if (initAudioPlayer() < 0) {
        return -1;
    }

    // 线程变量
    pthread_t audioReadDataThread, audioHandleDataThread;
    // 创建线程
    pthread_create(&audioHandleDataThread, NULL, handleAudioData, NULL);
#endif
#ifdef USE_VIDEO
    // video
    if (initVideoPlayer() < 0) {
        return -1;
    }

    pthread_t videoReadDataThread, videoHandleDataThread;
    // 创建线程
    pthread_create(&videoHandleDataThread, NULL, handleVideoData, NULL);
#endif

    sleep(1);

#ifdef USE_AUDIO
    // 创建线程
    pthread_create(&audioReadDataThread, NULL, readData, audioWrapper->father);
#endif
#ifdef USE_VIDEO
    // 创建线程
    pthread_create(&videoReadDataThread, NULL, readData, videoWrapper->father);
#endif

#ifdef USE_AUDIO
    // 等待线程执行完
    pthread_join(audioReadDataThread, NULL);
    pthread_join(audioHandleDataThread, NULL);
#endif
#ifdef USE_VIDEO
    // 等待线程执行完
    pthread_join(videoReadDataThread, NULL);
    pthread_join(videoHandleDataThread, NULL);
#endif

#ifdef USE_AUDIO
    // 取消线程
    pthread_cancel(audioReadDataThread);
    pthread_cancel(audioHandleDataThread);
#endif
#ifdef USE_VIDEO
    // 取消线程
    pthread_cancel(videoReadDataThread);
    pthread_cancel(videoHandleDataThread);
#endif

    printf("%s\n", "alexanderVideoPlayerWithSDL() end");
    return 0;
}


#endif //MYSTUDY_FFMPEG_STUDYAGAIN