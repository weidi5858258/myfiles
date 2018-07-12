//
// Created by root on 18-7-12.
//
#ifndef MYSTUDY_STUDY_FFMPEG
#define MYSTUDY_STUDY_FFMPEG

#include "../include/MyHead.h"


static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

void fill_audio(void *udata, Uint8 *stream, int len);

int simplest_ffmpeg_player2() {
    int i, videoStreamIndex = -1, audioStreamIndex = -1;
    uint8_t *out_buffer;
    int result, got_picture_ptr, got_picture_ptr_audio;

//    char filePath[] = "http://192.168.0.131:8080/video/aaaaa.mp4";
//    char filePath[] = "/root/mydev/tools/apache-tomcat-9.0.0.M19/webapps/ROOT/video/aaaaa.mp4";
    char filePath[] = "/root/视频/aaaaa.rmvb";

    // 屏幕宽高
    int screen_w = 0, screen_h = 0;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Rect sdlRect;

    /***
     下面三个变量都是通过源文件得到的结构体,因此有关源信息可以通过下面变量得到
     */
    AVFormatContext *avFormatContext = NULL;
    // 颜色转换器
    AVCodecContext *videoAVCodecContext = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVCodecParameters *videoAVCodecParameters = NULL;
    AVCodecParameters *audioAVCodecParameters = NULL;
    AVCodec *videoAVCodecDecoder = NULL;
    AVCodec *audioAVCodecDecoder = NULL;
    AVSampleFormat audioAVSampleFormat;
    // 命名规则
    // AVCodec *videoAVCodecEncoder = NULL;
    // AVCodec *audioAVCodecEncoder = NULL;

    AVFrame *srcAVFrame = NULL, *dstAVFrame = NULL;
    // AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
    AVPacket *avPacket = NULL;
    struct SwsContext *videoSwsContext;
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

    int nb_streams = avFormatContext->nb_streams;
    printf("avFormatContext->nb_streams = %d\n", nb_streams);// 2
    for (i = 0; i < nb_streams; i++) {
        AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
        printf("avMediaType = %d\n", avMediaType);
        if (avMediaType == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            videoAVCodecParameters = avFormatContext->streams[videoStreamIndex]->codecpar;
            // break;
        } else if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            audioAVCodecParameters = avFormatContext->streams[audioStreamIndex]->codecpar;
        }
    }
    printf("videoIndex = %d, audioIndex = %d\n", videoStreamIndex, audioStreamIndex);// 0
    if (videoStreamIndex == -1 || audioStreamIndex == -1) {
        printf("Didn't find a video stream.\n");
        return -1;
    }

    screen_w = videoAVCodecParameters->width;// 1280
    screen_h = videoAVCodecParameters->height;// 720
    printf("screen_w = %d, screen_h = %d\n", screen_w, screen_h);

    /***
    extern DECLSPEC int SDLCALL SDL_Init(Uint32 flags)
    SDL_INIT_TIMER：定时器
    SDL_INIT_AUDIO：音频
    SDL_INIT_VIDEO：视频
    SDL_INIT_JOYSTICK：摇杆
    SDL_INIT_HAPTIC：触摸屏
    SDL_INIT_GAMECONTROLLER：游戏控制器
    SDL_INIT_EVENTS：事件
    SDL_INIT_NOPARACHUTE：不捕获关键信号（这个没研究过）
    SDL_INIT_EVERYTHING：包含上述所有选项
     */
    printf("SDL_Init初始化SDL\n");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    /***
    extern DECLSPEC SDL_Window * SDLCALL SDL_CreateWindow(const char *title,
                                                          int x, int y, int w,
                                                          int h, Uint32 flags)
                                                          参数含义如下。
    title  ：窗口标题
    x       ：窗口位置x坐标。也可以设置为SDL_WINDOWPOS_CENTERED或SDL_WINDOWPOS_UNDEFINED。
    y       ：窗口位置y坐标。同上。
    w      ：窗口的宽
    h       ：窗口的高
    flags ：支持下列标识。包括了窗口的是否最大化、最小化，能否调整边界等等属性。
           ::SDL_WINDOW_FULLSCREEN,    ::SDL_WINDOW_OPENGL,
           ::SDL_WINDOW_HIDDEN,        ::SDL_WINDOW_BORDERLESS,
           ::SDL_WINDOW_RESIZABLE,     ::SDL_WINDOW_MAXIMIZED,
           ::SDL_WINDOW_MINIMIZED,     ::SDL_WINDOW_INPUT_GRABBED,
           ::SDL_WINDOW_ALLOW_HIGHDPI.
     返回创建完成的窗口的ID。如果创建失败则返回0。
     */
    printf("SDL_CreateWindow创建窗口\n");
    window = SDL_CreateWindow("Simplest ffmpeg player's Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_w, screen_h,
                              SDL_WINDOW_OPENGL);
    // 如果返回"0"就表示失败
    if (!window) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }
    /***
    extern DECLSPEC SDL_Renderer * SDLCALL SDL_CreateRenderer(SDL_Window * window,
                                           int index, Uint32 flags)
    参数含义如下。
    window    ： 渲染的目标窗口。
    index         ：打算初始化的渲染设备的索引。设置“-1”则初始化默认的渲染设备。
    flags          ：支持以下值（位于SDL_RendererFlags定义中）
        SDL_RENDERER_SOFTWARE ：使用软件渲染
        SDL_RENDERER_ACCELERATED ：使用硬件加速
        SDL_RENDERER_PRESENTVSYNC：和显示器的刷新率同步
        SDL_RENDERER_TARGETTEXTURE ：不太懂
    返回创建完成的渲染器的ID。如果创建失败则返回NULL。
     */
    printf("SDL_CreateRenderer基于窗口创建渲染器\n");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer基于窗口创建渲染器失败\n");
        return -1;
    }
    /***
    IYUV: Y + U + V  (3 planes)
    YV12: Y + V + U  (3 planes)
    extern DECLSPEC SDL_Texture * SDLCALL SDL_CreateTexture(SDL_Renderer * renderer,
                                                    Uint32 format,
                                                    int access, int w,
                                                    int h)
    参数的含义如下。
    renderer：目标渲染器。
    format      ：纹理的格式。后面会详述。
    access      ：可以取以下值（定义位于SDL_TextureAccess中）
        SDL_TEXTUREACCESS_STATIC           ：变化极少
        SDL_TEXTUREACCESS_STREAMING        ：变化频繁
        SDL_TEXTUREACCESS_TARGET           ：暂时没有理解
    w               ：纹理的宽
    h                ：纹理的高
    创建成功则返回纹理的ID，失败返回0。
    在纹理的创建过程中，需要指定纹理的格式（即第二个参数）。
     */
    printf("SDL_CreateTexture创建纹理\n");
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_IYUV,
                                SDL_TEXTUREACCESS_STREAMING,
                                screen_w,
                                screen_h);
    if (texture == NULL) {
        printf("SDL_CreateTexture创建纹理失败\n");
        return -1;
    }
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = screen_w;
    sdlRect.h = screen_h;

    printf("Audio Video init\n");

    // 得到AVCodec结构体
    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    videoAVCodecDecoder = avcodec_find_decoder(videoAVCodecParameters->codec_id);
    audioAVCodecDecoder = avcodec_find_decoder(audioAVCodecParameters->codec_id);
    if (videoAVCodecDecoder == NULL || audioAVCodecDecoder == NULL) {
        printf("Codec not found.\n");
        return -1;
    }

    // 必须要申请内存
    videoAVCodecContext = avcodec_alloc_context3(NULL);
    audioAVCodecContext = avcodec_alloc_context3(NULL);
    if (videoAVCodecContext == NULL || audioAVCodecContext == NULL) {
        printf("videoAVCodecContext is NULL or audioAVCodecContext is NULL.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(videoAVCodecContext, videoAVCodecParameters) < 0) {
        printf("copy videoAVCodecParameters to context fail\n");
        return -1;
    }
    if (avcodec_parameters_to_context(audioAVCodecContext, audioAVCodecParameters) < 0) {
        printf("copy audioAVCodecParameters to context fail\n");
        return -1;
    }

    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(videoAVCodecContext, videoAVCodecDecoder, NULL) < 0
        || avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }

    /***
     得到SwsContext结构体
     struct SwsContext *sws_getContext(int srcW, int srcH,
                                  enum AVPixelFormat srcFormat,
                                  int dstW, int dstH,
                                  enum AVPixelFormat dstFormat,
                                  int flags,
                                  SwsFilter *srcFilter,
                                  SwsFilter *dstFilter,
                                  const double *param);
     */
    reinterpret_cast<AVSampleFormat *>(videoSwsContext = sws_getContext(screen_w, screen_h,
                                                                        videoAVCodecContext->pix_fmt,
                                                                        screen_w, screen_h,
                                                                        AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
                                                                        SWS_BICUBIC,// SWS_POINT
                                                                        NULL, NULL, NULL));
    if (videoSwsContext == NULL) {
        printf("Cannot initialize the conversion context!\n");
        return -1;
    }


    printf("Video output fmt init\n");

    /***
     int av_image_get_buffer_size(enum AVPixelFormat pix_fmt, int width, int height, int align)
     */
    int av_image_get_buffer_size_ =
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
                                     screen_w,
                                     screen_h,
                                     1);
    printf("av_image_get_buffer_size_ = %d\n", av_image_get_buffer_size_);// 1382400
    srcAVFrame = av_frame_alloc();
    dstAVFrame = av_frame_alloc();
    avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    out_buffer = (uint8_t *) av_malloc(av_image_get_buffer_size_);
    /***
    int av_image_fill_arrays(uint8_t *dst_data[4],
                            int dst_linesize[4],
                            const uint8_t *src,
                            enum AVPixelFormat pix_fmt,
                            int width,
                            int height,
                            int align)
     */
    av_image_fill_arrays(dstAVFrame->data,
                         dstAVFrame->linesize,
                         out_buffer,
                         AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
                         screen_w,
                         screen_h,
                         1);

    printf("playback Audio setup\n");

    audioAVSampleFormat = audioAVCodecContext->sample_fmt;
    printf("audioAVSampleFormat = %d\n", audioAVSampleFormat);

    //AAC:1024  MP3:1152
    int out_sample_rate = 44100;
    int out_nb_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    int out_nb_samples = audioAVCodecContext->frame_size;
    /***
     Out Buffer Size
     int av_samples_get_buffer_size(int *linesize,
                                    int nb_channels,
                                    int nb_samples,
                                    enum AVSampleFormat sample_fmt,
                                    int align)
     */
    int out_buffer_size = av_samples_get_buffer_size(NULL,
                                                     out_nb_channels,
                                                     out_nb_samples,
                                                     AV_SAMPLE_FMT_S16,
                                                     1);

    //SDL_AudioSpec
    SDL_AudioSpec wanted_spec;
    wanted_spec.freq = out_sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = out_nb_channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = out_nb_samples;
    wanted_spec.callback = fill_audio;
    wanted_spec.userdata = audioAVCodecContext;
    if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
        printf("can't open audio.\n");
        return -1;
    }

    int index = 0;
    //FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout = av_get_default_channel_layout(audioAVCodecContext->channels);
    // Swr init
    audioSwrContext = swr_alloc();
    audioSwrContext = swr_alloc_set_opts(audioSwrContext,
                                         AV_CH_LAYOUT_STEREO,
                                         AV_SAMPLE_FMT_S16,
                                         out_sample_rate,
                                         in_channel_layout,
                                         audioAVCodecContext->sample_fmt,
                                         audioAVCodecContext->sample_rate,
                                         0,
                                         NULL);
    swr_init(audioSwrContext);

    //Play
    SDL_PauseAudio(0);

    /***
     新版本的ffmpeg用的是av_read_frame,而老版本的是av_read_packet.
     区别是av_read_packet读出的是包,它可能是半帧或多帧,不保证帧的完整性.
     av_read_frame对av_read_packet进行了封装,使读出的数据总是完整的帧.
     int av_read_frame(AVFormatContext *s, AVPacket *pkt)
     作用: 读取码流中的音频若干帧或者视频一帧,获取一个AVPacket
     如果返回0则说明读取正常
     avFormatContext: 输入的AVFormatContext
     avPacket: 输出的AVPacket

     读文件注意事项:
     av_read_frame读取视频固定是一次读取一帧,
     但是针对音频一次可以读取若干帧音频,
     因此在live555中fDurationInMicroseconds的赋值
     切勿认为一次只读取一帧音频来设置该变量值,
     而应该根据前后帧的时间戳差值来计算.

     遇到的问题:
     1.FFmpeg去连接的时候相机不在线导致avformat_open_input等函数一直死等,造成程序卡死
     2.av_read_frame的过程中相机断开连接导致读取码流一直死等
     3.ffmpeg取rtsp流时av_read_frame阻塞
     */
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        // 只取视频数据
        if (avPacket->stream_index == videoStreamIndex) {
            result = avcodec_send_packet(videoAVCodecContext, avPacket);
            switch (result) {
                case 0:
                    break;
                case AVERROR(EAGAIN):
                    printf("VIDEO AVERROR(EAGAIN)\n");
                    return -1;
                case AVERROR(EINVAL):
                    printf("VIDEO AVERROR(EINVAL)\n");
                    return -1;
                case AVERROR(ENOMEM):
                    printf("VIDEO AVERROR(ENOMEM)\n");
                    return -1;
                case AVERROR_EOF:
                    printf("VIDEO AVERROR_EOF\n");
                    return -1;
                default:
                    printf("VIDEO OTHER ERROR\n");
                    return -1;
            }
            while (avcodec_receive_frame(videoAVCodecContext, srcAVFrame) >= 0) {
                /***
                 用于转换像素
                 int sws_scale(struct SwsContext *c,
                                const uint8_t *const srcSlice[],
                                const int srcStride[],
                                int srcSliceY,
                                int srcSliceH,
                                uint8_t *const dstSlice[],
                                const int dstStride[]);
                 如果不知道什麼是stride,姑且可以先把它看成是每一列的byte數
                 srcSliceY: 註解的意思來看,是指第一列要處理的位置;這裡我是從頭處理，所以直接填0
                 将像素格式为YUV420P,分辨率为480x272的视频转换为像素格式为RGB24,分辨率为1280x720的视频
                 */
                sws_scale(videoSwsContext,
                          (const uint8_t *const *) srcAVFrame->data,
                          srcAVFrame->linesize,
                          0,
                          screen_h,
                          dstAVFrame->data,
                          dstAVFrame->linesize);

                // 下面代码的意思是把Y U V数据渲染到SDL容器上
                /***
                 设置纹理的数据
                 extern DECLSPEC int SDLCALL SDL_UpdateYUVTexture(SDL_Texture * texture,
                                                                 const SDL_Rect * rect,
                                                                 const Uint8 *Yplane, int Ypitch,
                                                                 const Uint8 *Uplane, int Upitch,
                                                                 const Uint8 *Vplane, int Vpitch)
                 参数的含义如下。
                texture：目标纹理。
                rect：更新像素的矩形区域。设置为NULL的时候更新整个区域。
                pixels：像素数据。
                pitch：一行像素数据的字节数。
                成功的话返回0，失败的话返回-1。
                 */
                SDL_UpdateYUVTexture(texture,
                                     &sdlRect,
                                     dstAVFrame->data[0], dstAVFrame->linesize[0],
                                     dstAVFrame->data[1], dstAVFrame->linesize[1],
                                     dstAVFrame->data[2], dstAVFrame->linesize[2]);
                // 先使用SDL_RenderClear()清空渲染目标
                // 实际上视频播放的时候不使用SDL_RenderClear()也是可以的，因为视频的后一帧会完全覆盖前一帧
                SDL_RenderClear(renderer);
                /***
                纹理复制给渲染目标
                参数的含义如下。
                renderer：渲染目标。
                texture：输入纹理。
                srcrect：选择输入纹理的一块矩形区域作为输入。设置为NULL的时候整个纹理作为输入。
                dstrect：选择渲染目标的一块矩形区域作为输出。设置为NULL的时候整个渲染目标作为输出。
                成功的话返回0，失败的话返回-1。
                 */
                SDL_RenderCopy(renderer, texture, NULL, &sdlRect);
                // 显示画面
                SDL_RenderPresent(renderer);

                //Delay 40ms
                SDL_Delay(40);
            }

        } /*else if (avPacket->stream_index == audioStreamIndex) {
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
                    printf("AUDIO OTHER ERROR\n");
                    return -1;
            }
            while (avcodec_receive_frame(audioAVCodecContext, srcAVFrame) >= 0) {
                // Audacity: 16bit PCM little endian stereo
                if (audioAVSampleFormat == AV_SAMPLE_FMT_S16P) {// 6
                    int16_t *ptr_l = (int16_t *) srcAVFrame->extended_data[0];
                    int16_t *ptr_r = (int16_t *) srcAVFrame->extended_data[1];
                    for (int i = 0; i < srcAVFrame->nb_samples; i++) {
                        *//*fwrite(ptr_l++, sizeof(int16_t), 1, outfile);
                        fwrite(ptr_r++, sizeof(int16_t), 1, outfile);*//*
                    }
                    //Audacity: big endian 32bit stereo start offset 7 (but has noise)
                } else if (audioAVSampleFormat == AV_SAMPLE_FMT_FLTP) {// 8
                    *//*float *ptr_l = (float *) srcAVFrame->extended_data[0];
                    float *ptr_r = (float *) srcAVFrame->extended_data[1];*//*
                    // printf("audio ptr_l = %d, ptr_r = %d\n", *ptr_l, *ptr_r);
                    // printf("audio srcAVFrame->nb_samples = %d\n", srcAVFrame->nb_samples);// 1024
                    *//*for (int i = 0; i < srcAVFrame->nb_samples; i++) {
                        *//**//*fwrite(ptr_l++, sizeof(float), 1, outfile);
                        fwrite(ptr_r++, sizeof(float), 1, outfile);*//**//*
                    }*//*

                    *//***
                     解码得到的数据类型为float 4bit，
                     而播放器播放的格式一般为S16（signed 16bit）,
                     就需要对解码得到的数据进行转换
                     int swr_convert(struct SwrContext *s,
                                     uint8_t **out,
                                     int out_count,
                                     const uint8_t **in ,
                                     int in_count)
                    *//*
                    swr_convert(audioSwrContext,
                                (uint8_t **) dstAVFrame->data,
                                MAX_AUDIO_FRAME_SIZE,
                                (const uint8_t **) srcAVFrame->data,
                                srcAVFrame->nb_samples);

                    printf("index:%5d\tavPacket->pts:%lld\tavPacket->size:%d\n", index, avPacket->pts, avPacket->size);

                    index++;

                    //Set audio buffer (PCM data)
                    audio_chunk = (Uint8 *) dstAVFrame->data;
                    //Audio buffer length
                    audio_len = out_buffer_size;
                    audio_pos = audio_chunk;
                }
            }
        }*/
        av_packet_unref(avPacket);
    }


    SDL_Quit();

    sws_freeContext(videoSwsContext);
    swr_free(&audioSwrContext);
    swr_close(audioSwrContext);
    av_frame_free(&srcAVFrame);
    av_frame_free(&dstAVFrame);
    avcodec_close(videoAVCodecContext);
    avcodec_close(audioAVCodecContext);
    avcodec_parameters_free(&videoAVCodecParameters);
    avcodec_parameters_free(&audioAVCodecParameters);
    avformat_close_input(&avFormatContext);

    return 0;
}

/***
 backup 老的api
 */
int simplest_ffmpeg_player() {
    int i, videoIndex = -1, audioIndex = -1;
    uint8_t *out_buffer;
    int result, got_picture_ptr, got_picture_ptr_audio;

//    char filePath[] = "http://192.168.0.131:8080/video/aaaaa.mp4";
//    char filePath[] = "/root/mydev/tools/apache-tomcat-9.0.0.M19/webapps/ROOT/video/aaaaa.mp4";
//    char filePath[] = "/mnt/d/Tools/apache-tomcat-8.5.23/webapps/ROOT/video/kingsman.mp4";
    char filePath[] = "/root/视频/kingsman.mp4";

    // 屏幕宽高
    int screen_w = 0, screen_h = 0;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Rect sdlRect;

    /***
     下面三个变量都是通过源文件得到的结构体,因此有关源信息可以通过下面变量得到
     */
    AVFormatContext *avFormatContext = NULL;
    // 颜色转换器
    AVCodecContext *videoAVCodecContext = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVCodec *videoAVCodecDecoder = NULL;
    AVCodec *audioAVCodecDecoder = NULL;
    AVSampleFormat audioAVSampleFormat;
    // 命名规则
    // AVCodec *videoAVCodecEncoder = NULL;
    // AVCodec *audioAVCodecEncoder = NULL;

    AVFrame *srcAVFrame = NULL, *dstAVFrame = NULL;
    // AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
    AVPacket *avPacket = NULL;
    struct SwsContext *swsContext;

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

    int nb_streams = avFormatContext->nb_streams;
    printf("avFormatContext->nb_streams = %d\n", nb_streams);// 2
    for (i = 0; i < nb_streams; i++) {
        AVMediaType avMediaType = avFormatContext->streams[i]->codec->codec_type;
        printf("avMediaType = %d\n", avMediaType);
        if (avMediaType == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            // break;
        } else if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            audioIndex = i;
        }
    }
    printf("videoIndex = %d, audioIndex = %d\n", videoIndex, audioIndex);// 0
    if (videoIndex == -1 || audioIndex == -1) {
        printf("Didn't find a video stream.\n");
        return -1;
    }
    /*if (audioIndex == -1) {
        printf("Didn't find a audio stream.\n");
        return -1;
    }*/

    // 得到AVCodecContext结构体
    videoAVCodecContext = avFormatContext->streams[videoIndex]->codec;
    audioAVCodecContext = avFormatContext->streams[audioIndex]->codec;
    screen_w = videoAVCodecContext->width;// 1280
    screen_h = videoAVCodecContext->height;// 720
    printf("screen_w = %d, screen_h = %d\n", screen_w, screen_h);

    // 得到AVCodec结构体
    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    videoAVCodecDecoder = avcodec_find_decoder(videoAVCodecContext->codec_id);
    audioAVCodecDecoder = avcodec_find_decoder(audioAVCodecContext->codec_id);
    if (videoAVCodecDecoder == NULL || audioAVCodecDecoder == NULL) {
        printf("Codec not found.\n");
        return -1;
    }
    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(videoAVCodecContext, videoAVCodecDecoder, NULL) < 0
        && avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }

    /***
     得到SwsContext结构体
     struct SwsContext *sws_getContext(int srcW, int srcH,
                                  enum AVPixelFormat srcFormat,
                                  int dstW, int dstH,
                                  enum AVPixelFormat dstFormat,
                                  int flags,
                                  SwsFilter *srcFilter,
                                  SwsFilter *dstFilter,
                                  const double *param);
     */
    reinterpret_cast<AVSampleFormat *>(swsContext = sws_getContext(screen_w, screen_h,
                                                                   videoAVCodecContext->pix_fmt,
                                                                   screen_w, screen_h,
                                                                   AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
                                                                   SWS_BICUBIC,// SWS_POINT
                                                                   NULL, NULL, NULL));
    if (swsContext == NULL) {
        printf("Cannot initialize the conversion context!\n");
        return -1;
    }

    audioAVSampleFormat = audioAVCodecContext->sample_fmt;
    srcAVFrame = av_frame_alloc();
    dstAVFrame = av_frame_alloc();
    avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    /***
     int avpicture_get_size(enum AVPixelFormat pix_fmt, int width, int height)
     */
    out_buffer = (uint8_t *) av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, // 想要转换成什么样的格式
                                                          screen_w, screen_h));
    /***
     int avpicture_fill(AVPicture *picture,
                        const uint8_t *ptr,
                        enum AVPixelFormat pix_fmt,
                        int width,
                        int height)
     */
    avpicture_fill((AVPicture *) dstAVFrame,
                   out_buffer,
                   AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
                   screen_w,
                   screen_h);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("Simplest ffmpeg player's Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_w, screen_h,
                              SDL_WINDOW_OPENGL);
    // 如果返回"0"就表示失败
    if (!window) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_IYUV,
                                SDL_TEXTUREACCESS_STREAMING,
                                screen_w,
                                screen_h);

    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = screen_w;
    sdlRect.h = screen_h;

    /***
     新版本的ffmpeg用的是av_read_frame,而老版本的是av_read_packet.
     区别是av_read_packet读出的是包,它可能是半帧或多帧,不保证帧的完整性.
     av_read_frame对av_read_packet进行了封装,使读出的数据总是完整的帧.
     int av_read_frame(AVFormatContext *s, AVPacket *pkt)
     作用: 读取码流中的音频若干帧或者视频一帧,获取一个AVPacket
     如果返回0则说明读取正常
     avFormatContext: 输入的AVFormatContext
     avPacket: 输出的AVPacket

     读文件注意事项:
     av_read_frame读取视频固定是一次读取一帧,
     但是针对音频一次可以读取若干帧音频,
     因此在live555中fDurationInMicroseconds的赋值
     切勿认为一次只读取一帧音频来设置该变量值,
     而应该根据前后帧的时间戳差值来计算.

     遇到的问题:
     1.FFmpeg去连接的时候相机不在线导致avformat_open_input等函数一直死等,造成程序卡死
     2.av_read_frame的过程中相机断开连接导致读取码流一直死等
     3.ffmpeg取rtsp流时av_read_frame阻塞
     */
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        // 只取视频数据
        if (avPacket->stream_index == videoIndex) {
            /***
             解码一帧视频的数据 相应的有编码一帧的数据avcodec_encode_video2
             int avcodec_decode_video2(AVCodecContext *avctx,
                                        AVFrame *picture,
                                        int *got_picture_ptr,
                                        const AVPacket *avpkt)
             作用: 解码一帧视频数据
             输入一个压缩编码的结构体AVPacket,输出一个解码后的结构体AVFrame
             got_picture_ptr: 该值为0表明没有图像可以解码，否则表明有图像可以解码；
             */
            result = avcodec_decode_video2(videoAVCodecContext,
                                           srcAVFrame,// 输出
                                           &got_picture_ptr,
                                           avPacket);// 输入
            if (result < 0) {
                printf("Video Decode Error.\n");
                return -1;
            } else if (result == 0) {
                printf("Video End of file.\n");
                return 0;
            }

            // printf("result = %d\n", result);
            // 前十次为0,后面一直为1
            // printf("got_picture_ptr = %d\n", got_picture_ptr);
            if (got_picture_ptr) {
                /***
                 用于转换像素
                 int sws_scale(struct SwsContext *c,
                                const uint8_t *const srcSlice[],
                                const int srcStride[],
                                int srcSliceY,
                                int srcSliceH,
                                uint8_t *const dstSlice[],
                                const int dstStride[]);
                 如果不知道什麼是stride,姑且可以先把它看成是每一列的byte數
                 srcSliceY: 註解的意思來看,是指第一列要處理的位置;這裡我是從頭處理，所以直接填0
                 将像素格式为YUV420P,分辨率为480x272的视频转换为像素格式为RGB24,分辨率为1280x720的视频
                 */
                sws_scale(swsContext,
                          (const uint8_t *const *) srcAVFrame->data,
                          srcAVFrame->linesize,
                          0,
                          screen_h,
                          dstAVFrame->data,
                          dstAVFrame->linesize);

                // 下面代码的意思是把Y U V数据渲染到SDL容器上
                SDL_UpdateYUVTexture(texture,
                                     &sdlRect,
                                     dstAVFrame->data[0], dstAVFrame->linesize[0],
                                     dstAVFrame->data[1], dstAVFrame->linesize[1],
                                     dstAVFrame->data[2], dstAVFrame->linesize[2]);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &sdlRect);
                SDL_RenderPresent(renderer);

                //Delay 40ms
                SDL_Delay(40);
            }
        } else if (avPacket->stream_index == audioIndex) {
            /***
             int avcodec_decode_audio4(AVCodecContext *avctx,
                                       AVFrame *frame,
                                       int *got_frame_ptr,
                                       const AVPacket *avpkt)
            */
            // result = avcodec_decode_audio4(audioAVCodecContext, srcAVFrame, &got_picture_ptr_audio, avPacket);
            // result = avcodec_send_packet(audioAVCodecContext, avPacket);
            printf("1 result = %d, got_picture_ptr_audio = %d\n", result, got_picture_ptr_audio);
            // result = avcodec_receive_frame(audioAVCodecContext, srcAVFrame);
            printf("2 result = %d, got_picture_ptr_audio = %d\n", result, got_picture_ptr_audio);
            if (result < 0 && result != AVERROR(EAGAIN) && result != AVERROR_EOF) {
                printf("Audio Decode Error.\n");
                return -1;
            } else if (result == 0) {
                printf("Audio End of file.\n");
                return 0;
            }


            if (got_picture_ptr_audio) {
                // Audacity: 16bit PCM little endian stereo
                if (audioAVSampleFormat == AV_SAMPLE_FMT_S16P) {
                    int16_t *ptr_l = (int16_t *) srcAVFrame->extended_data[0];
                    int16_t *ptr_r = (int16_t *) srcAVFrame->extended_data[1];
                    for (int i = 0; i < srcAVFrame->nb_samples; i++) {
                        /*fwrite(ptr_l++, sizeof(int16_t), 1, outfile);
                        fwrite(ptr_r++, sizeof(int16_t), 1, outfile);*/
                    }

                    //Audacity: big endian 32bit stereo start offset 7 (but has noise)
                } else if (audioAVSampleFormat == AV_SAMPLE_FMT_FLTP) {
                    float *ptr_l = (float *) srcAVFrame->extended_data[0];
                    float *ptr_r = (float *) srcAVFrame->extended_data[1];
                    for (int i = 0; i < srcAVFrame->nb_samples; i++) {
                        /*fwrite(ptr_l++, sizeof(float), 1, outfile);
                        fwrite(ptr_r++, sizeof(float), 1, outfile);*/
                    }
                }
            }
        }
        av_free_packet(avPacket);
    }


    SDL_Quit();

    sws_freeContext(swsContext);
    av_frame_free(&srcAVFrame);
    av_frame_free(&dstAVFrame);
    avcodec_close(videoAVCodecContext);
    avcodec_close(audioAVCodecContext);
    avformat_close_input(&avFormatContext);

    return 0;
}

#define SRCFILE "foreman_cif.yuv"
#define DSTFILE "out.yuv"

int how_to_use_sws_scale() {
    // 設定原始 YUV 的長寬
    const int in_width = 352;
    const int in_height = 288;
    // 設定目的 YUV 的長寬
    const int out_width = 640;
    const int out_height = 480;

    struct SwsContext *img_convert_ctx;
    uint8_t *inbuf[4];
    uint8_t *outbuf[4];
    const int read_size = in_width * in_height * 3 / 2;
    const int write_size = out_width * out_height * 3 / 2;
    int inlinesize[4] = {in_width, in_width / 2, in_width / 2, 0};
    int outlinesize[4] = {out_width, out_width / 2, out_width / 2, 0};

    uint8_t in[352 * 288 * 3 >> 1];
    uint8_t out[640 * 480 * 3 >> 1];

    FILE *fin = fopen(SRCFILE, "rb");
    FILE *fout = fopen(DSTFILE, "wb");
    if (fin == NULL) {
        printf("open input file %s error.\n", SRCFILE);
        return -1;
    }
    if (fout == NULL) {
        printf("open output file %s error.\n", DSTFILE);
        return -1;
    }

    inbuf[0] = (uint8_t *) malloc(in_width * in_height);
    inbuf[1] = (uint8_t *) malloc(in_width * in_height >> 2);
    inbuf[2] = (uint8_t *) malloc(in_width * in_height >> 2);
    inbuf[3] = NULL;

    outbuf[0] = (uint8_t *) malloc(out_width * out_height);
    outbuf[1] = (uint8_t *) malloc(out_width * out_height >> 2);
    outbuf[2] = (uint8_t *) malloc(out_width * out_height >> 2);
    outbuf[3] = NULL;

    // ********* Initialize software scaling *********
    // ********* sws_getContext **********************
    /*img_convert_ctx = sws_getContext(in_width, in_height,
                                     PIX_FMT_YUV420P,
                                     out_width, out_height,
                                     PIX_FMT_YUV420P,
                                     SWS_POINT,
                                     NULL, NULL, NULL);*/
    if (img_convert_ctx == NULL) {
        fprintf(stderr, "Cannot initialize the conversion context!\n");
        return -1;
    }

    fread(in, 1, read_size, fin);

    memcpy(inbuf[0], in, in_width * in_height);
    memcpy(inbuf[1], in + in_width * in_height, in_width * in_height >> 2);
    memcpy(inbuf[2], in + (in_width * in_height * 5 >> 2), in_width * in_height >> 2);

    // ********* 主要的 function ******
    // ********* sws_scale ************
    sws_scale(img_convert_ctx,
              (const uint8_t *const *) inbuf,
              inlinesize,
              0,
              in_height,
              outbuf,
              outlinesize);

    /***
     extern void *memcpy (void *__restrict __dest, const void *__restrict __src, size_t __n)
     */
    memcpy(out, outbuf[0], out_width * out_height);
    memcpy(out + out_width * out_height, outbuf[1], out_width * out_height >> 2);
    memcpy(out + (out_width * out_height * 5 >> 2), outbuf[2], out_width * out_height >> 2);

    fwrite(out, 1, write_size, fout);

    // ********* 結束的 function *******
    // ********* sws_freeContext *******
    sws_freeContext(img_convert_ctx);

    fclose(fin);
    fclose(fout);
    return 0;
}

int simplest_ffmpeg_swscale() {
    //Parameters
    FILE *src_file = fopen("sintel_480x272_yuv420p.yuv", "rb");
    const int src_w = 480, src_h = 272;
    AVPixelFormat src_pixfmt = AV_PIX_FMT_YUV420P;

    int src_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(src_pixfmt));

    FILE *dst_file = fopen("sintel_1280x720_rgb24.rgb", "wb");
    const int dst_w = 1280, dst_h = 720;
    AVPixelFormat dst_pixfmt = AV_PIX_FMT_RGB24;
    int dst_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(dst_pixfmt));

    //Structures
    uint8_t *src_data[4];
    int src_linesize[4];

    uint8_t *dst_data[4];
    int dst_linesize[4];

    int rescale_method = SWS_BICUBIC;
    struct SwsContext *img_convert_ctx;
    uint8_t *temp_buffer = (uint8_t *) malloc(src_w * src_h * src_bpp / 8);

    int frame_idx = 0;
    int ret = 0;
    ret = av_image_alloc(src_data, src_linesize, src_w, src_h, src_pixfmt, 1);
    if (ret < 0) {
        printf("Could not allocate source image\n");
        return -1;
    }
    ret = av_image_alloc(dst_data, dst_linesize, dst_w, dst_h, dst_pixfmt, 1);
    if (ret < 0) {
        printf("Could not allocate destination image\n");
        return -1;
    }
    //-----------------------------
    //Init Method 1
    img_convert_ctx = sws_alloc_context();
    //Show AVOption
    av_opt_show2(img_convert_ctx, stdout, AV_OPT_FLAG_VIDEO_PARAM, 0);
    //Set Value
    av_opt_set_int(img_convert_ctx, "sws_flags", SWS_BICUBIC | SWS_PRINT_INFO, 0);
    av_opt_set_int(img_convert_ctx, "srcw", src_w, 0);
    av_opt_set_int(img_convert_ctx, "srch", src_h, 0);
    av_opt_set_int(img_convert_ctx, "src_format", src_pixfmt, 0);
    //'0' for MPEG (Y:0-235);'1' for JPEG (Y:0-255)
    av_opt_set_int(img_convert_ctx, "src_range", 1, 0);
    av_opt_set_int(img_convert_ctx, "dstw", dst_w, 0);
    av_opt_set_int(img_convert_ctx, "dsth", dst_h, 0);
    av_opt_set_int(img_convert_ctx, "dst_format", dst_pixfmt, 0);
    av_opt_set_int(img_convert_ctx, "dst_range", 1, 0);
    sws_init_context(img_convert_ctx, NULL, NULL);

    //Init Method 2
    //img_convert_ctx = sws_getContext(src_w, src_h,src_pixfmt, dst_w, dst_h, dst_pixfmt,
    //	rescale_method, NULL, NULL, NULL);
    //-----------------------------
    /*
    //Colorspace
    ret=sws_setColorspaceDetails(img_convert_ctx,sws_getCoefficients(SWS_CS_ITU601),0,
        sws_getCoefficients(SWS_CS_ITU709),0,
         0, 1 << 16, 1 << 16);
    if (ret==-1) {
        printf( "Colorspace not support.\n");
        return -1;
    }
    */
    while (1) {
        if (fread(temp_buffer, 1, src_w * src_h * src_bpp / 8, src_file) != src_w * src_h * src_bpp / 8) {
            break;
        }

        switch (src_pixfmt) {
            case AV_PIX_FMT_GRAY8: {
                memcpy(src_data[0], temp_buffer, src_w * src_h);
                break;
            }
            case AV_PIX_FMT_YUV420P: {
                memcpy(src_data[0], temp_buffer, src_w * src_h);                    //Y
                memcpy(src_data[1], temp_buffer + src_w * src_h, src_w * src_h / 4);      //U
                memcpy(src_data[2], temp_buffer + src_w * src_h * 5 / 4, src_w * src_h / 4);  //V
                break;
            }
            case AV_PIX_FMT_YUV422P: {
                memcpy(src_data[0], temp_buffer, src_w * src_h);                    //Y
                memcpy(src_data[1], temp_buffer + src_w * src_h, src_w * src_h / 2);      //U
                memcpy(src_data[2], temp_buffer + src_w * src_h * 3 / 2, src_w * src_h / 2);  //V
                break;
            }
            case AV_PIX_FMT_YUV444P: {
                memcpy(src_data[0], temp_buffer, src_w * src_h);                    //Y
                memcpy(src_data[1], temp_buffer + src_w * src_h, src_w * src_h);        //U
                memcpy(src_data[2], temp_buffer + src_w * src_h * 2, src_w * src_h);      //V
                break;
            }
            case AV_PIX_FMT_YUYV422: {
                memcpy(src_data[0], temp_buffer, src_w * src_h * 2);                  //Packed
                break;
            }
            case AV_PIX_FMT_RGB24: {
                memcpy(src_data[0], temp_buffer, src_w * src_h * 3);                  //Packed
                break;
            }
            default: {
                printf("Not Support Input Pixel Format.\n");
                break;
            }
        }

        sws_scale(img_convert_ctx,
                  (const uint8_t *const *) src_data,
                  src_linesize,
                  0,
                  src_h,
                  dst_data,
                  dst_linesize);
        printf("Finish process frame %5d\n", frame_idx);
        frame_idx++;

        switch (dst_pixfmt) {
            case AV_PIX_FMT_GRAY8: {
                fwrite(dst_data[0], 1, dst_w * dst_h, dst_file);
                break;
            }
            case AV_PIX_FMT_YUV420P: {
                fwrite(dst_data[0], 1, dst_w * dst_h, dst_file);                   //Y
                fwrite(dst_data[1], 1, dst_w * dst_h / 4, dst_file);               //U
                fwrite(dst_data[2], 1, dst_w * dst_h / 4, dst_file);               //V
                break;
            }
            case AV_PIX_FMT_YUV422P: {
                fwrite(dst_data[0], 1, dst_w * dst_h, dst_file);                    //Y
                fwrite(dst_data[1], 1, dst_w * dst_h / 2, dst_file);                //U
                fwrite(dst_data[2], 1, dst_w * dst_h / 2, dst_file);                //V
                break;
            }
            case AV_PIX_FMT_YUV444P: {
                fwrite(dst_data[0], 1, dst_w * dst_h, dst_file);                 //Y
                fwrite(dst_data[1], 1, dst_w * dst_h, dst_file);                 //U
                fwrite(dst_data[2], 1, dst_w * dst_h, dst_file);                 //V
                break;
            }
            case AV_PIX_FMT_YUYV422: {
                fwrite(dst_data[0], 1, dst_w * dst_h * 2, dst_file);               //Packed
                break;
            }
            case AV_PIX_FMT_RGB24: {
                fwrite(dst_data[0], 1, dst_w * dst_h * 3, dst_file);               //Packed
                break;
            }
            default: {
                printf("Not Support Output Pixel Format.\n");
                break;
            }
        }
    }

    sws_freeContext(img_convert_ctx);

    free(temp_buffer);
    fclose(dst_file);
    av_freep(&src_data[0]);
    av_freep(&dst_data[0]);

    return 0;
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
        printf("encoding frame %3d (size=%5d)\n", i++, out_size);
        fwrite(outbuf, 1, out_size, f);
    }



    /* get the delayed frames */
    for (; out_size || !had_output; i++) {
        fflush(stdout);


//        out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
        had_output |= out_size;
        printf("write frame %3d (size=%5d)\n", i, out_size);
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
#define USE_SDL 1


/***
 * The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled
 * len: The length (in bytes) of the audio buffer
 * 回调函数
*/
void fill_audio(void *udata, Uint8 *stream, int len) {
    if (audio_len == 0)        /*  Only  play  if  we  have  data  left  */
        return;
    len = (len > audio_len ? audio_len : len);    /*  Mix  as  much  data  as  possible  */

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

//int playbackAudio() {
//    AVFormatContext *pFormatCtx;
//    int i, audioStream;
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
//    audioStream = -1;
//    for (i = 0; i < pFormatCtx->nb_streams; i++)
//        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
//            audioStream = i;
//            break;
//        }
//
//    if (audioStream == -1) {
//        printf("Didn't find a audio stream.\n");
//        return -1;
//    }
//
//    // Get a pointer to the codec context for the audio stream
//    pCodecCtx = pFormatCtx->streams[audioStream]->codec;
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
//        if (packet->stream_index == audioStream) {
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
//                printf("index:%5d\t pts:%lld\t packet size:%d\n", index, packet->pts, packet->size);
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
//    packet.size = buffer_size;
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

int simplest_audio_play_sdl2() {
    //Init
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    //SDL_AudioSpec
    SDL_AudioSpec wanted_spec;
    wanted_spec.freq = 44100;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = 2;
    wanted_spec.silence = 0;
    wanted_spec.samples = 1024;
    wanted_spec.callback = fill_audio;

    if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
        printf("can't open audio.\n");
        return -1;
    }

    FILE *fp = fopen("/root/mydev/tools/apache-tomcat-9.0.0.M19/webapps/ROOT/video/NocturneNo2inEflat_44.1k_s16le.pcm",
                     "rb+");
    if (fp == NULL) {
        printf("cannot open this file\n");
        return -1;
    }
    //For YUV420P
    int pcm_buffer_size = 4096;
    char *pcm_buffer = (char *) malloc(pcm_buffer_size);
    int data_count = 0;

    while (1) {
        if (fread(pcm_buffer, 1, pcm_buffer_size, fp) != pcm_buffer_size) {
            // Loop
            fseek(fp, 0, SEEK_SET);
            fread(pcm_buffer, 1, pcm_buffer_size, fp);
            data_count = 0;
        }
        printf("Now Playing %10d Bytes data.\n", data_count);
        data_count += pcm_buffer_size;
        //Set audio buffer (PCM data)
        audio_chunk = (Uint8 *) pcm_buffer;
        //Audio buffer length
        audio_len = pcm_buffer_size;
        audio_pos = audio_chunk;
        //Play
        SDL_PauseAudio(0);
        while (audio_len > 0)//Wait until finish
            SDL_Delay(1);
    }

    return 0;
}

int simplest_ffmpeg_audio_decoder() {
    int i, audioStreamIndex = -1;
    int result;

    char filePath[] = "/root/mydev/workspace_github/simplest_ffmpeg_audio_player_2.2/simplest_audio_play_sdl2/aaa.mp3";
    FILE *pFile = fopen(
            "/root/mydev/workspace_github/simplest_ffmpeg_audio_player_2.2/simplest_audio_play_sdl2/output.pcm", "wb");

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
                                (const uint8_t **) srcAVFrame->data,
                                srcAVFrame->nb_samples);*/

                    printf("index:%5d\tavPacket->pts:%lld\tavPacket->size:%d\n", index, avPacket->pts, avPacket->size);
                    //Write PCM
                    fwrite(out_buffer, 1, out_buffer_size, pFile);
                    index++;
                }
            }
        }
        av_packet_unref(avPacket);
    }

    swr_free(&audioSwrContext);
    swr_close(audioSwrContext);
    av_frame_free(&srcAVFrame);
    av_frame_free(&dstAVFrame);
    avcodec_close(audioAVCodecContext);
    avcodec_parameters_free(&audioAVCodecParameters);
    avformat_close_input(&avFormatContext);

    return 0;
}

static void decode(AVCodecContext *dec_ctx,
                   AVPacket *pkt,
                   AVFrame *frame,
                   FILE *outfile) {
    int i, ch;
    int ret, data_size;

    /* send the packet with the compressed data to the decoder */
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error submitting the packet to the decoder\n");
        exit(1);
    }

    /* read all the output frames (in general there may be any number of them */
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
        data_size = av_get_bytes_per_sample(dec_ctx->sample_fmt);
        if (data_size < 0) {
            /* This should not occur, checking just for paranoia */
            fprintf(stderr, "Failed to calculate data size\n");
            exit(1);
        }
        for (i = 0; i < frame->nb_samples; i++)
            for (ch = 0; ch < dec_ctx->channels; ch++)
                fwrite(frame->data[ch] + data_size * i, 1, data_size, outfile);
    }
}

int decode_audio() {
    const char *outfilename, *filename;
    const AVCodec *codec;
    AVCodecContext *c = NULL;
    AVCodecParserContext *parser = NULL;
    int len, ret;
    FILE *f, *outfile;
    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t data_size;
    AVPacket *pkt;
    AVFrame *decoded_frame = NULL;

    filename = "/root/mydev/workspace_github/simplest_ffmpeg_audio_player_2.2/simplest_audio_play_sdl2/aaa.mp3";
    outfilename = "/root/mydev/workspace_github/simplest_ffmpeg_audio_player_2.2/simplest_audio_play_sdl2/output.pcm";

    pkt = av_packet_alloc();

    /* find the MPEG audio decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "Parser not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate audio codec context\n");
        exit(1);
    }

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    outfile = fopen(outfilename, "wb");
    if (!outfile) {
        av_free(c);
        exit(1);
    }

    /* decode until eof */
    data = inbuf;
    data_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

    while (data_size > 0) {
        if (!decoded_frame) {
            if (!(decoded_frame = av_frame_alloc())) {
                fprintf(stderr, "Could not allocate audio frame\n");
                exit(1);
            }
        }

        ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                               data, data_size,
                               AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0) {
            fprintf(stderr, "Error while parsing\n");
            exit(1);
        }
        data += ret;
        data_size -= ret;

        if (pkt->size)
            decode(c, pkt, decoded_frame, outfile);

        if (data_size < AUDIO_REFILL_THRESH) {
            memmove(inbuf, data, data_size);
            data = inbuf;
            len = fread(data + data_size, 1,
                        AUDIO_INBUF_SIZE - data_size, f);
            if (len > 0)
                data_size += len;
        }
    }

    /* flush the decoder */
    pkt->data = NULL;
    pkt->size = 0;
    decode(c, pkt, decoded_frame, outfile);

    fclose(outfile);
    fclose(f);

    avcodec_free_context(&c);
    av_parser_close(parser);
    av_frame_free(&decoded_frame);
    av_packet_free(&pkt);

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




#endif //MYSTUDY_STUDY_FFMPEG