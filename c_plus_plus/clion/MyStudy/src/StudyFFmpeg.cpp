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

void fill_audio(void *udata, Uint8 *stream, int len);

void print_file_audio_info(AVFormatContext *avformat_context, AVCodecContext *audio_avcodec_context,
                           int in_sample_fmt, int audio_stream_index) {
    ///////////////////////////////输出音频信息///////////////////////////////

    //输出音频信息
    printf("\n");
    printf("Complete name：         \t%s\n", avformat_context->filename);
    printf("Format long name：     \t%s\n", avformat_context->iformat->long_name);
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
    printf("Stream size：         \t%d\n", avformat_context->nb_streams);
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
    AVInputFormat *pInputFormat = avformat_context->iformat;
    while (pInputFormat) {
        printf("%s ", pInputFormat->name);
        pInputFormat = pInputFormat->next;
    }*/
    // 下面的内容还不知道怎么得到
//    printf("File size： %ld\n", );
//    printf("Format version： %s\n", );
//    printf("Format profile：%s\n", );
//    printf("Bit rate mode： %d\n", );
//    printf("Compression mode： %d\n", );
//    printf("Encoding settings： %ld\n", );
}

/***
 能够播放视频,但是一会儿工夫画面就成黑白了,
 这个原因是不是视频在主线程中播放导致的?
 */
int simplest_ffmpeg_player2() {
    int i, videoStreamIndex = -1, audioStreamIndex = -1;
    uint8_t *out_buffer;
    int result, got_picture_ptr, got_picture_ptr_audio;

//    char filePath[] = "http://192.168.0.131:8080/video/aaaaa.mp4";
//    char filePath[] = "/root/mydev/tools/apache-tomcat-9.0.0.M19/webapps/ROOT/video/aaaaa.mp4";
    char filePath[] = "/root/视频/haoke.avi";

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

/***
 尖叫声
 */
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

    FILE *fp = fopen("/root/音乐/GALAYoungForYou.pcm", "rb+");
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

int yuv420p_2_rgb24() {
    //Parameters
    FILE *src_file = fopen("/root/视频/sintel_480x272_yuv420p.yuv", "rb");
    const int src_w = 480, src_h = 272;
    AVPixelFormat src_pixfmt = AV_PIX_FMT_YUV420P;
    int src_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(src_pixfmt));
    printf("源视频每个像素占用的比特数:   %d\n", src_bpp);

    FILE *dst_file = fopen("/root/视频/sintel_1280x720_rgb24.rgb", "wb");
    const int dst_w = 1280, dst_h = 720;
    AVPixelFormat dst_pixfmt = AV_PIX_FMT_RGB24;
    int dst_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(dst_pixfmt));
    printf("目标视频每个像素占用的比特数: %d\n", dst_bpp);

    //Structures
    uint8_t *src_data[4];
    int src_linesize[4];

    uint8_t *dst_data[4];
    int dst_linesize[4];

    struct SwsContext *videoSwsContext;
    uint8_t *temp_buffer = (uint8_t *) malloc(src_w * src_h * src_bpp / 8);

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

    printf("SwsContext init start.\n");
    // Init Method 1 方法一是不成功的
    videoSwsContext = sws_alloc_context();
    // Show AVOption 这一步出错,不能使用stdout
    // av_opt_show2(img_convert_ctx, stdout, AV_OPT_FLAG_VIDEO_PARAM, 0);
    // 打印出一些信息
    av_opt_show2(videoSwsContext, NULL, AV_OPT_FLAG_VIDEO_PARAM, 0);
    // Set Value
    av_opt_set_int(videoSwsContext, "sws_flags", SWS_BICUBIC | SWS_PRINT_INFO, 0);
    av_opt_set_int(videoSwsContext, "srcw", src_w, 0);
    av_opt_set_int(videoSwsContext, "srch", src_h, 0);
    av_opt_set_int(videoSwsContext, "src_format", src_pixfmt, 0);
    // '0' for MPEG (Y:0-235);'1' for JPEG (Y:0-255)
    av_opt_set_int(videoSwsContext, "src_range", 1, 0);
    av_opt_set_int(videoSwsContext, "dstw", dst_w, 0);
    av_opt_set_int(videoSwsContext, "dsth", dst_h, 0);
    av_opt_set_int(videoSwsContext, "dst_format", dst_pixfmt, 0);
    av_opt_set_int(videoSwsContext, "dst_range", 1, 0);
    sws_init_context(videoSwsContext, NULL, NULL);
    printf("SwsContext init end.\n");

    /*printf("SwsContext init start.\n");
    //Init Method 2 能够正常运行
    img_convert_ctx = sws_getContext(src_w, src_h, src_pixfmt,
                                     dst_w, dst_h, dst_pixfmt,
                                     SWS_BICUBIC,
                                     NULL, NULL, NULL);
    //Colorspace
    ret = sws_setColorspaceDetails(img_convert_ctx, sws_getCoefficients(SWS_CS_ITU601), 0,
                                   sws_getCoefficients(SWS_CS_ITU709), 0,
                                   0, 1 << 16, 1 << 16);
    if (ret == -1) {
        printf("Colorspace not support.\n");
        return -1;
    }
    printf("SwsContext init end.\n");*/

    int frame_idx = 0;
    while (1) {
        if (fread(temp_buffer, 1, src_w * src_h * src_bpp / 8, src_file) != src_w * src_h * src_bpp / 8) {
            printf("fread(...) failure.\n");
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

        sws_scale(videoSwsContext, (const uint8_t *const *) src_data, src_linesize, 0, src_h, dst_data, dst_linesize);
        printf("Finish process frame %5d\n", frame_idx);
        frame_idx++;

        switch (dst_pixfmt) {
            case AV_PIX_FMT_GRAY8: {
                fwrite(dst_data[0], 1, dst_w * dst_h, dst_file);
                break;
            }
            case AV_PIX_FMT_YUV420P: {
                fwrite(dst_data[0], 1, dst_w * dst_h, dst_file);                 //Y
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

    sws_freeContext(videoSwsContext);

    free(temp_buffer);
    fclose(dst_file);
    av_freep(&src_data[0]);
    av_freep(&dst_data[0]);

    return 0;
}

static void encode(AVCodecContext *enc_ctx,
                   AVFrame *frame,
                   AVPacket *pkt,
                   FILE *outfile) {
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);

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

        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
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
        printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
        /* mux encoded frame */
        ret = av_write_frame(fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }
    return ret;
}

/***
 pcm ---> aac
 不成功
 */
int simplest_ffmpeg_audio_encoder() {
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

    in_file = fopen("/root/音乐/tdjm.pcm", "rb");

    av_register_all();

    //Method 1.
//    pFormatCtx = avformat_alloc_context();
//    fmt = av_guess_format(NULL, out_file, NULL);
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
    pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);
    pCodecCtx->bit_rate = 64000;

    //Show some information
    av_dump_format(pFormatCtx, 0, out_file, 1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
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
            printf("Succeed to encode 1 frame! \tsize:%5d\n", pkt.size);
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
        printf("Succeed to encode frame: %5d\tsize:%5d\n", ++endocedFrameCount, (*encodedAVPacket).size);
        fwrite((*encodedAVPacket).data, 1, (*encodedAVPacket).size, outputFile);
        av_packet_unref(encodedAVPacket);
    }
    return 0;
}

/***
 来自雷神的代码
 使用这个方法进行编码
 只能对pcm文件进行编码
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

    char filename_in[] = "/root/音乐/tdjm.pcm";

    char filename_out[] = "/root/音乐/temp.mp2";

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
    audioAVCodecContext->codec_id = codec_id;
    audioAVCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;
    // 比特率降低后,音频体积也会降低
    audioAVCodecContext->bit_rate = 64000;// 64kb/s
    audioAVCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
    // 下面几个参数的设置参照官方sample
    audioAVCodecContext->sample_rate = 44100;// 44100Hz
    audioAVCodecContext->channel_layout = AV_CH_LAYOUT_STEREO;// 立体声
    // 双声道
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
        avPacket.size = 0;*/
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
    lame_global_flags *gfp;
    int ret_code;
    FILE *infp;
    FILE *outfp;
    short *unencoded_pcm_buffer;
    int input_samples;
    unsigned char *encoded_mp3_buffer;
    int mp3_bytes;

    gfp = lame_init();
    if (gfp == NULL) {
        printf("lame_init failed\n");
        status = -1;
        goto exit;
    }

    // 设置被输入编码器的原始数据的采样率
//    lame_set_in_samplerate(gfp,8000);
    // 设置最终mp3编码输出的声音的采样率，如果不设置则和输入采样率一样
//    lame_set_out_samplerate(gfp, 44100);
    // 设置被输入编码器的原始数据的声道数
//    lame_set_num_channels(gfp,1);
    // 设置比特率控制模式，默认是CBR，但是通常我们都会设置VBR。参数是枚举，vbr_off代表CBR，vbr_abr代表ABR（因为ABR不常见，所以本文不对ABR做讲解）vbr_mtrh代表VBR
//    lame_set_VBR(gfp,vbr_off);
    // 设置CBR的比特率，只有在CBR模式下才生效
//    lame_set_brate(gfp,8);
    // 设置最终mp3编码输出的声道模式，如果不设置则和输入声道数一样。参数是枚举，STEREO代表双声道，MONO代表单声道
//    lame_set_mode(gfp,3);
//    lame_set_quality(gfp,2); /* 2 = high 5 = medium 7 = low */

    ret_code = lame_init_params(gfp);
    if (ret_code < 0) {
        printf("lame_init_params returned %d\n", ret_code);
        status = -1;
        goto close_lame;
    }

    infp = fopen(inPath, "rb");
    outfp = fopen(outPath, "wb");

    unencoded_pcm_buffer = (short *) malloc(INBUF_SIZE * 2);
    encoded_mp3_buffer = (unsigned char *) malloc(MP3BUFSIZE);

    do {
        input_samples = fread(unencoded_pcm_buffer, 2, INBUF_SIZE, infp);
        printf("input_samples is %d.\n", input_samples);
        //fprintf(stderr, "input_samples is %d./n", input_samples);
        /***
         编码PCM数据
            lame_encode_buffer或lame_encode_buffer_interleaved：将PCM数据送入编码器，获取编码出的mp3数据。这些数据写入文件就是mp3文件。
            其中lame_encode_buffer输入的参数中是双声道的数据分别输入的，lame_encode_buffer_interleaved输入的参数中双声道数据是交错在一起输入的。具体使用哪个需要看采集到的数据是哪种格式的，不过现在的设备采集到的数据大部分都是双声道数据是交错在一起。
            单声道输入只能使用lame_encode_buffer，把单声道数据当成左声道数据传入，右声道传NULL即可。
            调用这两个函数时需要传入一块内存来获取编码器出的数据，这块内存的大小lame给出了一种建议的计算方式：采样率/20+7200。
         */
        mp3_bytes = lame_encode_buffer_interleaved(gfp,
                                                   unencoded_pcm_buffer,
                                                   input_samples / 2,
                                                   encoded_mp3_buffer,
                                                   MP3BUFSIZE);
        fprintf(stdout, "mp3_bytes is %d\n", mp3_bytes);
        if (mp3_bytes < 0) {
            printf("lame_encode_buffer_interleaved returned %d\n", mp3_bytes);
            status = -1;
            goto free_buffers;
        } else if (mp3_bytes > 0) {
            fwrite(encoded_mp3_buffer, 1, mp3_bytes, outfp);
        }
    } while (input_samples == INBUF_SIZE);

    mp3_bytes = lame_encode_flush(gfp, encoded_mp3_buffer, sizeof(encoded_mp3_buffer));
    if (mp3_bytes > 0) {
        printf("writing %d mp3 bytes\n", mp3_bytes);
        fwrite(encoded_mp3_buffer, 1, mp3_bytes, outfp);
    }

    free_buffers:
    free(encoded_mp3_buffer);
    free(unencoded_pcm_buffer);

    fclose(outfp);
    fclose(infp);
    close_lame:
    lame_close(gfp);

    exit:
    return status;
}

/***
 * 实现将音频的三大要素：声道，样本，采样率变更的demo
 * 例如双声道变成单声道，44100->48000，float->s16等
 * @param audio_dec_ctx
 * @param pAudioDecodeFrame
 * @param out_sample_fmt
 * @param out_channels
 * @param out_sample_rate
 * @param out_buf
 * @return
 */
int AudioResampling(AVCodecContext *audio_dec_ctx, AVFrame *pAudioDecodeFrame,
                    int out_sample_fmt, int out_channels, int out_sample_rate, uint8_t *out_buf) {
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

    src_ch_layout = (audio_dec_ctx->channel_layout &&
                     audio_dec_ctx->channels ==
                     av_get_channel_layout_nb_channels(audio_dec_ctx->channel_layout)) ?
                    audio_dec_ctx->channel_layout :
                    av_get_default_channel_layout(audio_dec_ctx->channels);

    if (out_channels == 1) {
        dst_ch_layout = AV_CH_LAYOUT_MONO;
    } else if (out_channels == 2) {
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
    av_opt_set_int(swr_ctx, "in_sample_rate", audio_dec_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", audio_dec_ctx->sample_fmt, 0);

    av_opt_set_int(swr_ctx, "out_channel_layout", dst_ch_layout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", out_sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", (AVSampleFormat) out_sample_fmt, 0);
    swr_init(swr_ctx);

    max_dst_nb_samples = dst_nb_samples =
            av_rescale_rnd(src_nb_samples, out_sample_rate, audio_dec_ctx->sample_rate, AV_ROUND_UP);
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


    dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, audio_dec_ctx->sample_rate) +
                                    src_nb_samples, out_sample_rate, audio_dec_ctx->sample_rate, AV_ROUND_UP);
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

    data_size = av_samples_get_buffer_size(NULL, audio_dec_ctx->channels,
                                           pAudioDecodeFrame->nb_samples,
                                           audio_dec_ctx->sample_fmt, 1);
    if (data_size <= 0) {
        printf("av_samples_get_buffer_size error \n");
        return -1;
    }
    resampled_data_size = data_size;

    if (swr_ctx) {
        ret = swr_convert(swr_ctx, dst_data, dst_nb_samples,
                          (const uint8_t **) pAudioDecodeFrame->data, pAudioDecodeFrame->nb_samples);
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
        printf("swr_ctx null error \n");
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
    const char *output_file_name_path = "/root/视频/output.yuv";
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
    const char in_file_path[] = "/root/音乐/漂洋过海来看你.mp3";
    const char out_file_path[] = "/root/音乐/output.pcm";
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

//    av_dump_format(avformat_context, 0, in_file_path, false);

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

int get_file_infomation() {

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
    FILE *fp1 = fopen("/root/视频/output_420_y.y", "wb+");
    FILE *fp2 = fopen("/root/视频/output_420_u.y", "wb+");
    FILE *fp3 = fopen("/root/视频/output_420_v.y", "wb+");

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

//////////////////////////////////////////////////////////////////

const char *src_filename = "/root/视频/haoke.avi";
const char *video_dst_filename = "/root/视频/haoke.yuv";
const char *audio_dst_filename = "/root/音乐/haoke.aac";

FILE *pOutputAudio = NULL;
FILE *pOutputVideo = NULL;

AVFormatContext *avformat_context = NULL;

AVStream *avstream = NULL;
AVCodec *avcodec = NULL;
AVCodecContext *avcodec_context = NULL;

//分别定义音视频流文件
AVStream *video_avstream = NULL, *audio_avstream = NULL;
AVCodecContext *video_avcodec_context = NULL, *audio_avcodec_context = NULL;

int frame_width = 0, frame_height = 0;
enum AVPixelFormat pix_fmt;

unsigned char *video_dst_data[4];
int video_dst_linesize[4];
int video_dst_bufsize;

AVFrame *frame = NULL;
AVPacket packet;

int decode_packet(int *got_frame) {
    int ret = 0;
    *got_frame = 0;
    if (packet.stream_index == video_avstream->index) {
        ret = avcodec_decode_video2(video_avcodec_context, frame, got_frame, &packet);
        if (ret < 0) {
            printf("%s\n", "");
            return -1;
        }
        if (*got_frame) {
            printf("%s\n", "");
            av_image_copy(video_dst_data, video_dst_linesize,
                          (const unsigned char **) frame->data, frame->linesize,
                          pix_fmt, frame_width, frame_height);
            fwrite(video_dst_data[0], 1, video_dst_bufsize, pOutputVideo);
        }
    } else if (packet.stream_index == audio_avstream->index) {
        ret = avcodec_decode_audio4(audio_avcodec_context, frame, got_frame, &packet);
        if (ret < 0) {
            printf("%s\n", "");
            return -1;
        }

        /*
        av_get_bytes_per_sample((enum AVSampleFormat)frame->format)返回的是采样位数16bits or 8 bits，此函数进行了右移
        操作，即除以8来获得字节数
        nb_samples指每个声道每一帧所包含的采样个数
        */
        if (*got_frame) {
            size_t unpadded_linesize =
                    frame->nb_samples * av_get_bytes_per_sample((enum AVSampleFormat) frame->format);
            fwrite(frame->extended_data[0], 1, unpadded_linesize, pOutputAudio);
        }
    }

    return FFMIN(ret, packet.size);
}

static int open_codec_context(enum AVMediaType type) {
    int stream_index = 0;
    /*
    根据type找到对应的音视频流或者字母流，返回的是对应文件流的index
    */
    stream_index = av_find_best_stream(avformat_context, type, -1, -1, NULL, 0);
    if (stream_index < 0) {
        printf("%s\n", "");
        return -1;
    } else {
        avstream = avformat_context->streams[stream_index];
        avcodec_context = avstream->codec;
        avcodec = avcodec_find_decoder(avcodec_context->codec_id);
        if (!avcodec) {
            printf("%s\n", "");
            return -1;
        }

        /*
        用于初始化一个视音频编解码器的AVCodecContext
        */
        if (stream_index = avcodec_open2(avcodec_context, avcodec, NULL) < 0) {
            printf("%s\n", "");
            return -1;
        }
    }
    return stream_index;
}

int separate_media_to_yuv_and_aac() {
    int ret, got_frame = 0;

    //=====FFmpeg相关初始化=====
    av_register_all();
    /*
    打开待处理的音视频文件，探测读取文件格式信息保存到ftm_ctx中，
    同时读取多媒体文件信息，根据视音频流创建AVstream
    */
    if (avformat_open_input(&avformat_context, src_filename, NULL, NULL) < 0) {
        printf("%s\n", "");
        return -1;
    }
    /*
    读取一部分视音频数据并且获得一些相关的信息,主要给每个媒体流的AVStream结构体赋值
    */
    if (avformat_find_stream_info(avformat_context, NULL) < 0) {
        printf("%s\n", "");
        return -1;
    }

    //=====打开音频和视频流=====
    if (open_codec_context(AVMEDIA_TYPE_VIDEO) >= 0) {
        video_avstream = avstream;
        video_avcodec_context = avcodec_context;

        frame_width = video_avcodec_context->width;
        frame_height = video_avcodec_context->height;
        pix_fmt = video_avcodec_context->pix_fmt;

        /*
        分配像素的存储空间
        该函数的四个参数分别表示AVFrame结构中的缓存指针、各个颜色分量的宽度、图像分辨率（宽、高）、像素格式和内存对其的大小。
        该函数会返回分配的内存的大小。其实就是对video_dst_data,video_dst_linesize内存进行分配
        */
        ret = av_image_alloc(video_dst_data, video_dst_linesize,
                             frame_width, frame_height, pix_fmt, 1);
        if (ret < 0) {
            printf("%s\n", "");
            return -1;
        } else {
            video_dst_bufsize = ret;
        }
        if (!(pOutputVideo = fopen(video_dst_filename, "wb"))) {
            printf("%s\n", "");
            return -1;
        }
    }

    /*
    查找对应type流，并初始化设置解码器上下文
    */
    if (open_codec_context(AVMEDIA_TYPE_AUDIO) >= 0) {
        audio_avstream = avstream;
        audio_avcodec_context = avcodec_context;
        if (!(pOutputAudio = fopen(audio_dst_filename, "wb"))) {
            printf("%s\n", "");
            return -1;
        }
    }

    //=====打印输入输出格式信息====
    av_dump_format(avformat_context, 0, src_filename, 0);

    //=====读取和处理音视频文件=====
    frame = av_frame_alloc();
    if (!frame) {
        printf("%s\n", "");
        return -1;
    }

    av_init_packet(&packet);
    packet.data = NULL;
    packet.size = 0;

    while (av_read_frame(avformat_context, &packet) >= 0) {
        do {
            ret = decode_packet(&got_frame);
            printf("decode packet size: %d\n", ret);
            packet.data += ret;
            packet.size -= ret;
        } while (packet.size > 0);
    }

    /*
    将编码器缓存中的数据解码完
    */
    packet.data = NULL;
    packet.size = 0;
    do {
        ret = decode_packet(&got_frame);
        packet.data += ret;
        packet.size -= ret;
    } while (got_frame);

    //=====释放资源=====
    avcodec_close(video_avcodec_context);
    avcodec_close(audio_avcodec_context);
    avformat_close_input(&avformat_context);
    av_frame_free(&frame);
    av_free(video_dst_data[0]);

    fclose(pOutputAudio);
    fclose(pOutputVideo);

    return 0;
}

//////////////////////////////////////////////////////////////////

int test(float) {
    return 0;
}


#endif //MYSTUDY_STUDY_FFMPEG