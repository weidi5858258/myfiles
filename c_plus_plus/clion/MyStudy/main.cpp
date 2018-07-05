#include "include/MyHead.h"
#include "src/B0.cpp"
#include "src/Pointer.cpp"

void test();

int simplest_ffmpeg_player();

int simplest_ffmpeg_player2();

int simplest_ffmpeg_player_sdl2();

void fill_audio(void *udata, Uint8 *stream, int len);

/***
 * @param argc 参数至少有一个,因为第一个参数就是本身的可执行文件
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    int j = 0;
    for (j = 0; j < argc; j++) {
        printf("%s\n", argv[j]);
    }
    printf("The run result:\n");
    printf("------------------------------------------\n");

    simplest_ffmpeg_player2();
//    simplest_ffmpeg_player_sdl2();

    // test();
    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}

/***
 要测试的代码放在main函数中,测试后放到这个函数中
 */
void test() {
    Test t;
    cout << t.getData() << endl;
    Test t2(10);
    Test t3(t2);
    Test t4 = t2;

    C co;
    // error
    // c.B0::b0 = 100;
    co.B1::b0 = 100;
    co.B2::b0 = 200;
    cout << co.B1::m << endl;
    cout << co.B2::m << endl;
    cout << co.c << endl;
    co.print();
    B1 *pb = &co;
    pb->print();

    // 指针
    Pointer pt;
    pt.point();

    char name[] = "ueventc";
    cout << strcmp(basename(name), "ueventd") << endl;
    size_t key_len = strlen("PATH");
    cout << "key_len = " << key_len << endl;// 4
    cout << "key_len = " << sizeof("PATH") << endl;// 5

    char ch = '!';
    printf("%d = %c.\n", ch, ch);
    int ich = 127;
    printf("%d = %c.\n", ich, ich);

    int tmp[5] = {1, 2, 3, 4, 5};
    int *p = tmp;
    cout << sizeof(tmp) << endl;// tmp数组占用空间大小
    cout << sizeof(tmp) / sizeof(int) << endl;// tmp数组元素个数

    cout << tmp << endl;
    cout << &tmp[0] << endl;
    cout << p << endl;

    cout << *tmp << endl;
    // cout << *(tmp++) << endl;// error
    cout << *(&tmp[0]) << endl;
    int j;
    for (j = 0; j < 5; ++j) {
        cout << *(p++) << endl;
    }

    cout << sizeof(char) << endl;// 1
    cout << sizeof(wchar_t) << endl;// 4
    cout << sizeof(int) << endl;// 4
    cout << sizeof(short) << endl;// 2
    cout << sizeof(float) << endl;// 4
    cout << sizeof(double) << endl;// 8
    cout << sizeof(long) << endl;// 8
    cout << sizeof(long long) << endl;// 8
    cout << sizeof(long long int) << endl;// 8
    cout << sizeof(long double) << endl;// 16

    cout << endl;

    cout << sizeof(signed char) << endl;// 1
    cout << sizeof(unsigned char) << endl;// 1
    cout << sizeof(unsigned wchar_t) << endl;// 4
    cout << sizeof(unsigned int) << endl;// 4
    cout << sizeof(unsigned short) << endl;// 2
    cout << sizeof(unsigned long) << endl;// 8
    cout << sizeof(unsigned long long) << endl;// 8
    cout << sizeof(unsigned long long int) << endl;// 8

    cout << endl;

    cout << CHAR_MIN << endl;
    cout << CHAR_MAX << endl;
    cout << INT_MIN << endl;
    cout << INT_MAX << endl;
    cout << LONG_MIN << endl;
    cout << LONG_MAX << endl;

    // char与signed char取值范围都是 -128 到 127(有符号位)
    // unsigned char取值范围是 0 到 255
    // 如果定义的变量的值在此范围内,那么输出的值就是当前值
    // 正常边界,正常输出
    char a = -128;
    signed char b = 127;
    unsigned char c = 0;
    unsigned char d = 255;
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);// -128 127 0
    printf("a=%c, b=%c, c=%c, d=%d\n", a, b, c, d);//
    // 超出边界,反向输出
    a = -129;
    b = 128;
    c = -1;
    d = 256;
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);// 127 -128 255 0

    cout << endl;

    FILE *fp;
    size_t size = 0;
    char *ar;
    // 二进制方式打开文件
    fp = fopen("src/B0.cpp", "rb");
    if (NULL == fp) {
        printf("Error:Open input.c file fail!\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    // 求得文件的大小
    size = ftell(fp);
    rewind(fp);
    // 申请一块能装下整个文件的空间
    ar = (char *) malloc(sizeof(char) * size);
    // 读文件
    // fread(ar, 1, size, fp);//每次读一个，共读size次
    fread(ar, size, 1, fp);
    // 内容的首地址
    printf("%p\n", ar);
    // 输出文件内容
    printf("%s\n", ar);
    fclose(fp);
    free(ar);
    ar = NULL;

    cout << endl;

    const char *path = "/data";
    printf("path = %s\n", path);

    const unsigned char image[] = "abcd\0efg";
    //  const unsigned char image[] = "";
    const unsigned char *pImage = image;
    size = sizeof(pImage);// 包括结尾隐藏的\0
    cout << size << endl;// 8 固定长度,用于存放指针地址的
    size = sizeof(image);
    cout << size << endl;// 9
    for (int i = 0; i < size; ++i) {
        // 这个例子说明了可以用pImage[i]这种方式一个字符一个字符的输出
        printf("%c\n", pImage[i]);
        if (pImage[i] == 0) {
            printf("遇到\\0换行\n");
        }
    }

    double tmpArray[] = {1, 2, 3, 4, 5};
    cout << sizeof(tmpArray) << endl;
    for (int i = 0; i < 5; i++) {
        printf("%f\t%f\t%f\t%p\t%p\n",
               tmpArray[i], *(&tmpArray[i]), *(tmpArray + i),
               &tmpArray[i], (tmpArray + i));
    }

    // 下面的代码在这个工具中无法编译通过
    /*// 在栈上开辟一维数组
    int *pOne = (int[]){0};
    // 在栈上开辟二维数组
    int ab[3][4];
    int (*pTwo)[4] = (int[][4]){0};
    // 在栈上开辟三维数组
    int (*pThree)[3][4] = (int[][3][4]){0};*/

    // 下面代码使用SDL2简单显示一个窗口
    /*SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(
                "SDL Tutorial",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                640,
                480,
                SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        SDL_Delay(5000);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();*/

    /*SDL_Window *window = NULL;
    SDL_Renderer *renderer = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
            "hello",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(10000);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();*/
}

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

int simplest_ffmpeg_player2() {
    int i, videoStreamIndex = -1, audioStreamIndex = -1;
    uint8_t *out_buffer;
    int result, got_picture_ptr, got_picture_ptr_audio;

//    char filePath[] = "http://192.168.0.131:8080/video/aaaaa.mp4";
//    char filePath[] = "/root/mydev/tools/apache-tomcat-9.0.0.M19/webapps/ROOT/video/aaaaa.mp4";
    char filePath[] = "/mnt/d/Tools/apache-tomcat-8.5.23/webapps/ROOT/video/kingsman.mp4";

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

    printf("SDL init\n");

    // SDL init
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

        } else if (avPacket->stream_index == audioStreamIndex) {
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
                    /*float *ptr_l = (float *) srcAVFrame->extended_data[0];
                    float *ptr_r = (float *) srcAVFrame->extended_data[1];*/
                    // printf("audio ptr_l = %d, ptr_r = %d\n", *ptr_l, *ptr_r);
                    // printf("audio srcAVFrame->nb_samples = %d\n", srcAVFrame->nb_samples);// 1024
                    /*for (int i = 0; i < srcAVFrame->nb_samples; i++) {
                        *//*fwrite(ptr_l++, sizeof(float), 1, outfile);
                        fwrite(ptr_r++, sizeof(float), 1, outfile);*//*
                    }*/

                    /***
                     int swr_convert(struct SwrContext *s,
                                     uint8_t **out,
                                     int out_count,
                                     const uint8_t **in ,
                                     int in_count)
                    */
                    swr_convert(audioSwrContext,
                                (uint8_t **) dstAVFrame->data,
                                MAX_AUDIO_FRAME_SIZE,
                                (const uint8_t **) srcAVFrame->data,
                                srcAVFrame->nb_samples);

                    // printf("index:%5d\t pts:%lld\t packet size:%d\n", index, packet->pts, packet->size);

                    index++;

                    //Set audio buffer (PCM data)
                    audio_chunk = (Uint8 *) dstAVFrame->data;
                    //Audio buffer length
                    audio_len = out_buffer_size;
                    audio_pos = audio_chunk;
                }
            }
        }
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
    char filePath[] = "/mnt/d/Tools/apache-tomcat-8.5.23/webapps/ROOT/video/kingsman.mp4";

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
 用typedef重定义类型名
 typedef 原类型名 新类型名;
 则以后可以用这个"新类型名"来代替"原类型名"进行使用了.
 注意:用typedef类型重定义只是给原类型取一个别名,没有创造新的类型.
 typedef unsigned int size_t;// 用于内存字节计算
 typedef char NAME[20];
 NAME a1,a2,a3;
 等效于
 char a1[20],a2[20],a3[20];

 typedef与#define的区别
 用"#define"一般用于表示常量的比较多.
 定义方式不同:typedef是用后面的名称来表示前面的类型,#define是用前面的名称来表示后面的内容(数据类型,常量)
 用typedef能实现的,用#define有可能出错.如:
 #define INTP int*;
 INTP p,q;其等价于int* p,q;// 实际只有p是指向整型的指针变量,而q仅仅是整型变量.
 typedef int* INTP;
 INTP p,q;// p,q都是指向整型的指针变量.

 C++中函数的高级用法
 内联函数: 内联函数的声明和定义必须放在一个文件里,
 inline关键字是建议C++编译器将该函数设定为内联函数,
 但没有决定权;当函数里面有循环语句,复杂的条件语句,递归等时,
 则C++编译器无视inline关键字,将该函数作为普通函数处理.
 函数重载: 满足函数重载的条件是 有相同的函数名,不同的形参列表;与函数的返回值无关.
 递归函数: 自己调用自己的函数,注意必须要有退出函数的条件判断.





























“C++中的四少”：
构造函数
拷贝构造函数
赋值函数
析构函数
用户自己不重写上面的函数时，系统会自动生成这样的函数

关于继承的一些结论:
1.
当父类与子类有相同的函数(包括返回值,函数名,参数列名都相同),
父类的指针指向子类对象的地址,然后用这个指向调用这个函数时,
如果父类的这个函数没有用virtual修饰,那么调用的是父类的函数;
否则调用的是子类的函数.
2.
父类中有多个函数重载,可是子类却只有一个函数覆写了父类的函数,
那么当子类对象调用父类的重载函数时,只有自己的类中有的函数才可以调用,
其他的父类的重载函数不能调用,在子类中已经被同名隐藏了.
3.
3-1.
B1,B2才是普通继承B0,然后C顺次继承B1,B2,则创建C对象时,依次为B0,B1,B0,B2,C
3-2.
B1普通继承B0,B2使用virtual继承B0,然后C顺次继承B1,B2,则创建C对象时,依次为B0,B0,B1,B2,C
3-3.
B1普通继承B0,B2使用virtual继承B0,然后C顺次继承B1,virtual B2,则创建C对象时,依次为B0,B2,B0,B1,C

写成员函数时，有返回值的先把成员函数写在最前面，然后
才是“类名::成员函数”

引用与拷贝构造函数
5.4.1 引用
C++函数中参数的传递方式是传值。在函数域中为参数重新分配
内存，而把实参的数值传递到新分配的内存中。它的优点是
有效避免函数的副作用（意思就是不改变原来实参的值）。
如果要求改变实参的值（或者实参是一个复杂的对象），重新
分配内存会引起程序执行效率的降低，因此在C++中有一咱新的
导出型数据类型---引用（又称为别名）。
void swap(int &x, int &y){
    int tmp = x;
    x = y;
    y = tmp;
}
swap函数的形参被定义为引用类型后，就不再为x和y重新分配
内存，其使用的是实参的地址。
如果swap函数的形参被定义为具体数据类型，那么会为x和y
重新分配内存。
int a = 10;
int b = 20;
调用方式为：swap(a, b);
引用不是定义一个新的变量，而是给一个已经定义的变量重新起
一个别名，也就是C++系统不为引用类型的变量分配内存空间。
引用主要用于函数之间的数据传递。定义的格式为：
类型 &引用变量名 = 已定义过的变量名；
引用变量的地址是第一次被赋值时的那个变量地址，以后不会
再改变。

引用的使用：
1.
引用作为函数的形参（拷贝构造函数中就是使用了引用）
2.
引用作为函数的返回值
一般函数返回值时，要生成一个临时变量作为返回值的拷贝，
而用引用作为返回值时，不生成值的拷贝。
3.
返回值为引用的函数作为左值

注意：
1.
对数组只能引用数组元素，不能引用数组（数组名本身就是个
地址）。
2.
不能定义引用的引用（引用也是一个地址），所以当函数的参数
为引用时，引用不能作为实参。
3.
不能定义空引用。如int &a;// 这样是编译不通过的。
正确的用法为：
int a = 10;
int &b = a;
b的地址就是a的地址，以后即使被其他变量重新赋值其地址也
不会再改变，还是原来的a的地址。
下面的例子就能说明这个结论
int a = 10;
int b = 20;
fun(a, b);
int &c = a;
cout<<"a = "<<a<<", b = "<<b<<", c = "<<c<<endl;
cout<<"a: "<<&a<<", b: "<<&b<<", c: "<<&c<<endl;
c = b;
cout<<"a = "<<a<<", b = "<<b<<", c = "<<c<<endl;
cout<<"a: "<<&a<<", b: "<<&b<<", c: "<<&c<<endl;
结果为：
a = 10, b = 20, c = 10
a: 0xff914050, b: 0xff91404c, c: 0xff914050
a = 20, b = 20, c = 20
a: 0xff914050, b: 0xff91404c, c: 0xff914050

下面是指针的引用：
int a = 10;
int *P = &a;
int *q = p; // 这样的话p和q指向不同的地址
int *&q = p;// 这样的话p和q指向相同的地址（指针的引用）
下面是数组的引用：
int ar[5] = {1,2,3,4,5};
int (&br)[5] = ar;// 这样的话ar和br指向相同的地址
int &br[5] = ar;  // 这样的话是编译不通过的
下面是常量的引用：
const int x = 100;
int &y = x;// 这样的话是编译不通过的
const int &y = x;// 这样的话是正确的

int n = 20;
const int &m = n;// 这样的话是正确的
m不能再被赋值，但是n可以被重新赋值，赋值后，m和n的值相同。


const引用：
引用在内部存放的是被引用对象的地址，不可寻址的值是不能
引用的；当引用作为形参时，实参也不能使用不可寻址的值，
更不能进行类型转换（如：实数转换为整数）。
但是const引用不同，它是只读的，为了绝对保证不会发生误改，
编译器实现const引用时，生成一个临时对象，引用实际上指向
该临时对象，但用户不能访问它。所以const引用可以实现
不可寻址的值（包括字面常量）的引用，例如：
const double dval = 1024;
const int &ri = dval;
这是正确的，编译器将其转换为：
const double dval = 1024;
int temp = dval;
const int &ri = temp;
因有临时对象，引用和类型转换都实现了。
因为dval和ri的类型不一样，因此两者的地址也是不一样的，
这是因为有临时变量产生的缘故，ri的地址就是临时变量的
地址。
当const引用作为形参时，实参也能使用不可寻址的值，并能
进行类型转换。
从上面可以得到这样一个结果，类型不一样时，必须借助一个
临时变量才能完成转换，只是这个临时变量我们不能使用而已。
const double dval = 12.34;
int &ri = dval;
这样的话是编译不通过的。
因此这说明了临时变量具备常量的性质，是一个常量。

5.4.2 拷贝构造函数
如果把一个真实的类对象作为参数传递到拷贝构造函数中去，那么
会引起无穷递归。
同一个类的对象在内存中有完全相同的结构，如果作为一个
整体进行复制或称拷贝是完全可行的。这个拷贝过程只需要
拷贝数据成员，而函数成员是共用的（只有一份拷贝）。
在建立对象时可用同一类的另一个对象来初始化该对象，这时
所用的构造函数称为拷贝构造函数。
有CGoods类，其拷贝构造函数为：
CGoods(const CGoods &cgd){
    strcpy(name, cgd.name);
    price = cgd.price;
    amount = cgd.amount;
    totalValue = cgd.totalValue;
}

class Test{
public:
    Test(int d = 0):data(d){
        cout<<"Create Test(int) Object: "<<this<<endl;
    }
    Test(const Test &t){
        cout<<"Copy Create Test(Test) Object: "<<this<<endl;
    }
    ~Test(){
        cout<<"Free Test Object: "<<this<<endl;
    }
private:
    int data;
};
使用：
Test t(10);
Test t2(t);完全等价于Test t2 = t;
Test t3 = t;// 初始化（因为t3这个变量还不存在），拷贝构造函数
Test t4;// 需要重写Test()函数，不然没法使用
t4 = t;// 赋值函数（因为t4这个变量已经存在）

拷贝构造函数在三种情况下的使用：
1.
用一个对象初始化另一个对象时，前提必须是初始化。
2.
当函数的形参是类的对象（就是一个普通的对象），调用函数时，
进行形参与实参结合时使用（实际就是用一个对象初始化另一个对象）。
这时要在内存新建立一个局部对象，并把实参拷贝到新的对象中。
3.
当函数的返回值是类对象，函数执行完成后返回调用者时使用。
理由也是要建立一个临时对象，再返回调用者。
因为局部对象在离开建立它的函数时就消亡了，不可能在返回
调用函数后继续生存，所以在处理这种情况时，编译系统会在
调用函数的表达式中创建一个无名临时对象，该临时对象的
生存周期只在函数调用处的表达式中。所谓return对象，
实际上是调用拷贝构造函数把该对象的值拷入临时对象。如果
返回的是变量，处理过程类似，只是不调用构造函数。
int getData(int x, int y){
     int sum = x + y;
     return sum;
 }
 在getData函数中返回的值不是sum地址处的值，情况是这样的，
 在返回时，会生成一个临时变量，然后sum的值赋给这个临时变量，
 因为sum的作用域只能在getData函数中，出了函数就消亡了；当getData
 函数的结果赋给调用者的某个值后，这个生成的临时变量也就消亡了。
 这个临时变量是我们不可见的。

 5.4.3 赋值函数（赋值语句）
 Test t2;先实例化一个对象
 t2 = t;然后这样才是赋值，才会调用到下面的函数（t2必须事先已经定义过了）
实际上是这样的调用形式：t2.operator=(t)，
然后又会转化为：operator=(&t2, t);

 Test &Test::operator=(Test * const this, const Test &t)
 Test &Test::operator=(const Test &t) {
    std::cout << "Call operator=(Test &): " << this << std::endl;
    if (this != &t) {
        this->data = t.data;
    }
    return *this;
    this代表地址，那么*this就是地址处的值，这个值就是当前对象。
}
上面operator函数有三个重点：
 1.
 形参为：const Test &t
 以引用方式接收对象，就不会再发生拷贝构造这样的步骤，也就是不会再
 产生无名临时对象。
 2.
 if (this != &t)
 3.
 返回值类型为：Test &
 返回值类型设为Test&的目的是为了能够“连等赋值”。
 如：
 Test t(10);
 Test t2, t3;
 t3 = t2 = t;// t3.operator=(t2.operator=(t))

用“引用”类型作为返回值的类型的原则：
如果return的对象（如return *this）不受当前
函数的影响（如operator函数），
那么返回值可以使用“引用”而不会出现问题；
如果return的对象的作用域只在这个函数中时，
那么使用“引用”时就会出现问题而得不到正确的结果，
此时的返回值类型应该是普通对象，因为这样才能在函数
内部产生一个无名临时对象，然后才能把这个临时对象返回。
使用“引用”的好处是效率和空间的提升。

5.4.4 函数的调用优化
1.
return Test(10);
// 创建无名临时对象的语法
2.
Test temp(10);
return temp;
// 这样会创建一个无名临时对象，然后出了函数就把temp给free掉，
// 也就是说这样的操作创建了两个对象
3.
// 返回值必须是Test，而不能是Test &（不然得到不正确结果）。
Test fun(Test &t){
    int value = t.getData();
    // 创建无名临时对象的语法
    return Test(value);
}
使用：
Test t(100);
1.
Test t2 = fun(t);
// 最优状态，此时会把fun函数中创建的无名临时对象直接赋给t2对象，
// 而不会再另外产生一个t2对象。也就是说这样的初始化操作
// 不会再调用拷贝构造函数（这是编译器优化的）。
// 像Test t2 = t;这样是会调用拷贝构造函数的。
2.
Test t2;
t2 = fun(t);

上面的例子的前提是自己重写了“operator=”函数。

有下面这样一种现象（前提是自己没有重写“operator=”函数）：
返回值是Test &，而不是Test（如果是Test也是可以的）。
Test& fun(Test &t){
    int value = t.getData();
    // 创建无名临时对象的语法
    return Test(value);
}
然后这样使用：
Test t(100);
Test t2 = fun(t);// 这样就能得到正确的结果

5.4.5 深拷贝与线拷贝
主要内容在于“四少”中什么时候哪些函数需要重写，哪些不需要重写。
int a = 100;
我们能够这样操作“&a”，对a这个变量取地址，这是因为编译器已经帮
我们实现了“&”取地址这样的操作，但是我们没法重写这个操作，因为
这是普通类型的变量；如果是一个对象，那么我们可以重写“&”这样的
操作，这就是运算符重载。
Test t;
Test *pt = &t;// Test *pt = t.operator&();
因此需要重写下面函数：
第5个函数
Test* operator&(){
    return this;
}
如果不重写某个类的上面的函数，那么编译器也会自动生成这样的函数，
只不过我们不能额外的干些其他的事了，比如执行到这个函数时打印一些
日志。
const Test t;
const Test *pt = &t;// 去年const编译是不通过的
此时const封锁的是*pt，因此不能再用*pt去指向其他的对象。
还有加了const后，是不会调用到上面重写的函数，需要重写下面的函数：
第6个函数
const Test* operator&() const{
    return this;
}
到现在为止，如果我们自己不重写任何的函数，那么编译器也会自动生成
有关的函数能够让代码正常运行。如果要重写，那么我们有6个函数可以
重写，就是“四少”，普通对象的取地址函数和const对象的取地址函数。
一个人不重写这6个函数，但是进行相关的操作时是能够正常执行的。
如果要操作其他的功能，那么必须要重写相关的函数后才能执行。
如输出对象的运算符重载。
深拷贝：不但拷贝指针的指向，还创建出一个空间用于存放内容
浅拷贝：只拷贝指针的指向
char* data;
data = (char*)malloc(sizeof(char) + (strlen(str) + 1));
free(data);
申请空间之前，先判断原来的变量有没有指向，如果有指向，那么先要
释放掉，然后才是重新申请空间。
重写赋值函数时一般有四个步骤：
1.
判断是不是给自己赋值，不是的情况下才操作if(this != &T){...}
2.
释放原有空间
3.
申请新的空间，然后进行成员变量的赋值(可能有异常，如申请不成功)
4.
return *this;

5.4.6 运算符重载
运算符的重载实际是一种特殊的函数重载，必须定义一个函数，并告诉C++
编译器，当遇到该重载的运算符时调用此函数。这个函数叫做运算符重载
函数，通常为类的成员函数。
定义运算符重载函数的一般格式：
返回值类型 类名::operator重载的运算符（参数表）{...}
operator是关键字，它与重载的运算符一起构成函数名。
因函数名的特殊性，C++编译器可以将这类函数识别出来。
细解运算符重载：
复数类+的重载：
Complex Complex::operator+(const Complex &c){
    return Complex(real+c.real, imag+c.imag);
}
这里采用Complex对象的引用而不是对象本身，调用时不再重新分配内存
建立一个复制的对象，函数效率会更高。而在引用形式参数类型说明前
加const关键字，表示被引用的实参是不可改变的，防止程序员不小心在
函数体中重新赋值了被引用的实参，如果这样做了那么C++编译器会认为
出错。
当用类的成员函数实现运算符的重载时，运算符重载函数的参数（当为
双目运算符时）为一个或（当为单目运算符时）没有。运算符的左操作数
一定是对象，因为重载的运算符是该对象的成员函数，而右操作数是该
函数的参数。
单目运算符“++”和“--”存在前置与后置的问题。
前置“++”格式为：
返回类型 类名::operator++(){...}
后置“++”格式为：
返回类型 类名::operator++(int){...}
后置“++”中的参数int公用途区分，并无实际意义，可以给一个变量名，
也可以不给变量名。
Int& operator++(){
    m_i++;
    return *this;
}
Int operator++(int){
    Int temp(m_i);
    m_i++;
    return temp;
}
C++不允许重载的运算符有：
1.
?:
2.
. .*
3.
::
4.
sizeof

5.4.7 友元
一个常规的成员函数声明描述了三件在逻辑上相互不同的事情：
1.该函数能访问类声明的私有部分
2.该函数位于类的作用域中
3.该函数必须经由一个对象去激活（有一个this指针）
通过将函数声明为static，可以让它只有前两种性质
通过将一个函数声明为友元可以使它只具有第一咱性质

成员函数的形参中第一个参数才是this指针，如
void fun(Test * const this),只是这个this指针不需要程序员去关心。
然后调用函数是这样的：
Test t(100);
t.fun();// 转化为：fun(&t);
class Test{
// 友元函数在类中声明
friend void show(Test &);
};
// 友元函数在实现
void show(Test &t){...}

友元函数注意点：
1.
友元函数不是类的成员函数，在函数体中访问对象的成员，必须用对象名
加运算符“.”加对象成员名。但是友元函数可以访问类中的所有成员。
一般函数只能访问类中的公有成员。
2.
友元函数不受类中的访问权限关键字限制，可以把它放在类的公有、私有、
保护部分，但结果都一样。
3.
某类的友元函数的作用域并非该类作用域。如果该友元函数是另一类的
成员函数，则其作用域为另一类的作用域，否则与一般函数相同。
友元类：
整个类可以是另一个类的友元。友元类的每个成员函数都是另一个类的
友元函数，都可以访问另一个类中的保护或私有数据成员。
定义方法如下：
class A{
// 声明B为A的友元类
friend class B;
}
下面几个都是声明为Complex类的友元函数：
Complex operator+(int i, const Complex &c);
Complex operator+(const Complex &c, int i);
Complex comp(100,200);
Complex comp2 = 10 + comp;
Complex comp3 = comp + 10;
// 这样操作之后就可以直接输出一个对象了cout<<comp<<endl;
ostream& operator<<(ostream &out, const Complex &c);
istream& operator>>(istream &in, Complex &c);

也可以定义为Complex类的成员函数进行输出
ostream& operator<<(ostream &out){
    cout<<"("<<m_real<<", "<<m_imag<<")"<<endl;
    return out;
}
不过需要这样调用：comp<<cout<<endl;


5.4.8 static方法与const方法

模板是标准C++实现代码复用的有力工具，特别是有关数据结构的算法。
5.4.9 函数模板
1.
定义模板：
template<typename Type>
Type max(Type a, Type b){
    cout<<typeid(Type).name()<<endl;
    return a > b ? a : b;
}
使用：
max(1, (int)2.3);
max((double)1, 2.3);
max<int>(1, 2.3);
2.
定义模板：
template<typename Type1, typename Type2>
Type1 max(Type1 a, Type2 b){
     return a > b ? a : b;
}

5.4.10 类模板
例子：
// 声明类模板
template<typename Type>
class List;

template<typename Type>
class ListNode {
    friend class List<Type>;

public:
    ListNode() :
            data(Type()), next(NULL) {}

    ListNode(Type d, ListNode<Type> *n = NULL) :
            data(d), next(n) {}

    ~ListNode() {}

private:
    Type data;
    ListNode<Type> *next;
};

template<typename Type>
class List {
public:
    List();

    bool push_back(Type x);

private:
    ListNode<Type> *first;
    ListNode<Type> *last;
    size_t size;
};

template<typename Type>
List<Type>::List() {
    first = last = (ListNode<Type> *) malloc(sizeof(ListNode<Type>));
    last->next = NULL;
    size = 0;
}

template<typename Type>
bool List<Type>::push_back(Type x) {
    ListNode<Type> *s = (ListNode<Type> *) malloc(sizeof(ListNode<Type>));
    if (s == NULL) {
        return false;
    }
    s->data = x;
    s->next = NULL;

    last->data = s;
    last = s;
    size++;
    return true;
}

void main() {
    List<int> intList;
    for (int i = 1; i <= 10; ++i) {
        intList.push_back(i);
    }
}

派生类与基类
在任何需要基类对象的地方都可以用公有派生类的对象来代替,
这条规则称为赋值兼容规则.它包括以下情况:
1.
派生类的对象可以赋值给基类的对象,这时是把派生类对象中
从对应基类中继承来的隐藏对象赋值给基类对象.反过来赋值就
不行,因为派生类的新成员无值可赋.
2.
可以将一个派生类的对象的地址赋给基类的指针变量,但只能通过
这个指针访问类中由基类继承来的隐藏对象,不能访问派生类中的
新成员.同样也不能反过来做.
3.
派生类对象可以初始化基类的引用.引用是别名,但这个别名只能包含
派生类对象中的由基类继承来的隐藏对象.

基类指针或引用访问子类中的方法:
把子类的地址赋给基类的指针或者把子类赋给基类的引用,
此时用这个基类的指针或者引用去访问基类与子类都有的方法时,
结果访问的是基类中的方法,不会是子类中的方法;如果想要访问
子类中的方法,那么需要把基类中的方法加上virtual.
基类对象访问子类中的方法:
把子类对象赋给基类对象后,用基类对象访问子类中的方法时,
访问的还是基类中的方法.(前提是基类与子类都有相同的方法)

达到多态的条件:
1.
有继承关系
2.
有相同的方法名,返回值类型,参数列表
3.
基类方法加上virtual
4.
子类对象赋给基类的指针或者基类的引用

动态内存开辟new_delete
堆内存的分配与释放
堆对象与构造函数
深拷贝与浅拷贝
int *p = (int*)malloc(sizeof(int) * SIZE);
if(p == NULL) exit(1);
free(p);
int *q = new int[10];
delete []q;

#include<list>
#include<stack>
容器分为三大类：
顺序容器
vector（参量）                 从后面快速插入与删除，直接访问任何元素
deque（双端队列）              从前面或后面快速插入与删除，直接访问任何元素
list（列表）                   从任何地方快速插入与删除，双链表
关联容器
set（集合）                    快速查找，不允许重复值
multiset（多重集合）           快速查找，允许重复值
map（映射）                    一对一映射，基于关键字快速查找，不允许重复值
multimap（多重映射）           一对多映射，基于关键字快速查找，允许重复值
容器适配器
stack（栈）                    后进先出
queue（队列）                  先进先出
priority queue（优先级队列）   最高优先级元素问题第一个出列



字符串
字符串
char占用一个字节的空间
char man[4] = {'J', 'a', 'c', 'k'};
然而该char型数组man却不是一个字符串，因为它没有的结束
标志---“\0”.只有我们为其添加“\0”这个结束标志后，这个char
型数组man才能转化为字符串。如：
char man[4] = {'J', 'a', 'c', 'k', '\0'};
“\0”是一个空字符标志，它的ASCII码为0，C++有好多处理字符串的函数，
它们都以“\0”为结束标志，也就是以空字符为结束标志，如cin和cout，
它们也是以空字符为结束标志，它们在碰到空字符后将会停止输入或输出。
char man[12];
cin>>man;如果没有下面的代码，那么遇到空格就结束输入
cin.get(man, 12);遇到空格也会存入，并输出到屏幕上
cin.get的结束标志是“\n”,也就是换行，因此语句遇到空格不会结束，而是
把空格也看做一个字符，同时在输入结束后，也就是用户输入一个回车后，
cin.get自动为当前接受输入的数组添加字符串结束标志“\n”，因此它实际
保存的有效字符是它的第2个参数减1，即11个字符。
同样，输出也是这样，假如我们在一行字符串的中间存放了0，
那么0之后（包括0）的内容都不会输出。
数字0跟字符'\0'的效果是一样的。
空字符的ASCII码为0，编译器会根据ASCII码来寻找字符，ASCII码为0的字符
是NULL，也就是空字符，cout遇到空字符就会停止输出。
char man[12] = {"Hello world"};
用双引号包括起来的字符串隐含了字符串结束标志“\0”，因此不用手动去添加它。
也可以省略定义字符串的长度，而把这个任务交给编译器去完成。
如：char man[] = {"Hello world"};

strlen函数与sizeof函数的区别
strlen函数返回的是字符串结束标志“\0”之前的字符串长度，而不是数组长度；
而sizeof函数返回的是数组的长度，也就是该数组共有多少个元素。
char man[12] = {"Hello world"};
strlen(man): 11
sizeof(man): 12

字符串是以空字符结尾的字符数组.
char型字符串
char占用一个字节的空间.
char man[4] = {'J','a','c','k'};
然而该char型数组man却不是一个字符串,
因为它没有字符串的结束标志---"\0".
只有为其添加"\0"这个结束标志后,这个char型数组man才转化为字符串.如
char man[4] = {'J','a','c','k', '\0'};
"\0"

string型字符串的赋值
char型字符串无法直接赋值.如:
char ch1[] = "give me";
char ch2[] = "a cup";
ch1 = ch2;
这样是错误的,不能将一个数组名直接赋给另一个数组名.
可以用strcpy拷贝函数实现赋值的目的.
char ch1[] = "give me";
char ch2[] = "a cup";
strcpy(ch1, ch2);
必须保证ch1的空间足够大,以便能够容纳ch2的字符串.
strcpy会将ch2中的所有字符,包括结束标志"\0"一起复制到ch1中去.
第一种方式赋值
#include <string>
string str1 = "what's your name";
string str2 = "my name is Jack";
str1 = str2;
output: str1 = my name is Jack
第二种方式赋值
string str1 = " gh ";
string str2 = " abcdef ";
str1.assign(str2, 3, 1);
output: d

string型字符串的合并
对两个char型字符串进行合并要用到strcat函数,strcat函数
将第2个字符串合并到第1个字符串中,因此第1个字符串必须保证能容纳两个字符串的长度.
char ch1[] = "what's your name";
char ch2[] = "my name is Jack";
strcat(ch1, ch2);
运行后就会立即崩溃,这是因为ch1的空间不够造成的.
string str1 = "what's your name";
string str2 = "my name is Jack";
str1 = str1 + str2;

string型字符串的部分合并
char ch1[10] = "ab";
char ch2[] = "abcdef";
strncat(ch1, ch2, 3);
output: ch1 = ababc

string str1 = "ab";
string str2 = "abcdef";
str1.append(str2, 2, 3);
output: str1 = abcde

string型字符串的替换
char ch1[10] = "gh";
char ch2[] = "abcdef";
strncpy(ch1, ch2, 3);
output: ch1 = abc

string str1 = "gh";
string str2 = "abcdef";
str1.replace(0, 1, str2, 4, 2);
output: str1 = efh

basic_string &replace(size_type index, size_type num, const basic_string &str);
从本字符串的index开始,用str中的所有字符替换本字符串中的num个字符.
basic_string &replace(size_type index, size_type num, const char *str);
从本字符串的index开始,用str中的所有字符替换本字符串中的num个字符.
basic_string &replace(size_type index1, size_type num1, const basic_string &str, size_type index2, size_type num2);
用str中的num2个字符(从index2开始)替换本字符串中的字符,从index1开始,最多有num1个字符.
basic_string &replace(size_type index, size_type num1, const char *str, size_type num2);
用str中的num2个字符本字符串中的字符,从index开始,共num1个字符.
basic_string &replace(size_type index, size_type num1, size_type num2, char ch);
用num2个ch字符替换本字符串中的字符,从index开始,fugai num1个字符.
basic_string &replace(iterator start, iterator end, const basic_string &str);
用str中的字符替换本字符串中的字符,迭代器start和end指示范围.
basic_string &replace(iterator start, iterator end, const char *str);
用str中的字符替换本字符串中的字符,迭代器start和end指示范围.
basic_string &replace(iterator start, iterator end, const char *str, size_type num);
用str中的num个字符替换本字符串中的内容,迭代器start和end指示范围.
basic_string &replace(iterator start, iterator end, size_type num, char ch);
用num个ch字符替换本字符串中的内容,迭代器start和end指示范围.

string型字符串的复制
char ch1[15] = "abcdefghijklmn";
char ch2[] = "1234567890";
memmove(ch1, ch2, 10);
output: ch1 = 1234567890klmn

从string型字符中复制到char型字符串的copy函数
string str = "abcd";
char ch[] = "1234";
int n = str.copy(ch, 4, 0);
output: n = 4, ch = abcd

string型字符串的插入
str.insert(index1, str1, index2, num);
string str1 = "12789";
string str2 = "3456";
str1.insert(2, str2, 0, 4);
output: str1 = 123456789

string型字符串的删除
string s("give me");
// 第1个参数表示从哪个位置开始删除，第2个参数表示删除几个
s.erase(2, 2);
output: s = gi me
// 表示从哪个位置开始删除
s.erase(2);
output: s = gi
// 参数默认值为0，则从0开始往后的字符全部删除
s.erase();
output: s = 没有内容

string型字符串的查找
char ch1[15];
char *p, c = 'w';
strcpy(ch1, "hello world");
p = strchr(ch1, c);

string str1("hello world");
// “w”表示要搜索的字符，0表示从str1的第1个字符开始搜索
int f = str1.find("w", 0);
if(f != string::npos){
    cout<<f<<endl;
}else{
    cout<<"not find"<<endl;
}
npos是string类提供的一个常量，用来表示不存在的位置，
许多平台中npos的值都不一样。因此它的值由平台决定，
一般是-1，不管什么平台，npos的值都不能用作数组的下标。
find_first_not_of(): 查找第一个与value中的某值不相等的字符
find_first_of(): 查找第一个与value中的某值相等的字符
find_last_of(): 查找最后一个与value中的某值相等的字符
find_last_not_of(): 查找最后一个与value中的所有值都不相等的字符
rfind(): 查找最后一个与value相等的字符（逆向查找）

string型字符串的比较
string s1 = "155";
string s2 = "52";
char c[] = "34";
int i,j,k,l,m,n;
// 将s1与s2比较，返回0为相等，返回1为s1大于s2，返回-1为s1小于s2
i = s1.compare(s2);
// 将char型字符串c与s2比较
j = s2.compare(c);
// 取s1前两个字符与s2比较，参数1表示位置，参数2表示取的个数
k = s1.compare(0, 2, s2);
// 
l = s1.compare(1, 1, s2, 0, 1);
// 
m = s1.compare(1, 1, c, 0, 1);
// 
n = s1.compare(1, 1, c, 1);

string型字符串是否为空
string str = "";
if(str.empty()){
    cout<<"str is empty"<<endl;
}

字符串的使用
swap()交换两个字符串的内容
交换两个char型字符串的函数swab()，只能交换字符串中的相信的
偶数字节和奇数字节的位置。
char ch1[15] = "ofru";
char ch2[15] = "";
swab(ch1, ch2, strlen(ch1));
output: ch1 = ofru ch2 = four

string str1 = "give me";
string str2 = "a cup";
str1.swap(str2);
output: str1 = a cup str2 = give me

将string型字符串转为char型字符串
string类的成员函数c_str()可以返回一个指向char型字符串的const指针,
该指针指向调用该函数的string型字符串对象.
string str = "hello world";
const char *ch;
ch = str.c_str();
output: ch = hello world

char型字符串与函数
char型字符串就是C风格的字符串,它由一串字符组成,结尾为字符串结束
标志"\0".字符串名是第1个字符的地址,因此我们将字符串名作为参数
传递到函数中时,其实就是将地址传递到函数中去.
int get(const char p[])
int get(const char *p){
    int count = 0;
    while(*p){
        count++;
        p++;
    }
    return count;
}
char ch[15] = "hello world";
char *p = "very well";
int a = get(ch);
int b = get(p);
output: a = 10 b = 9

字符串有3种表达方式,如下:
1. char c[12] = "study";// char型数组
2. "study"              // 未命名数组
3. char *p = "study";   // 指向char型数组的指针
由于这3种表达方式才是以指针的方式来保存字符串中第1个字符的
内存地址,因此都可作为参数传递到函数中去.

函数如何返回字符串
char *get(char *str){
    char *p = new char(strlen(str) + 1);
    strcpy(p, str);
    return p;
}
char c[10];
char *ch;
cin>>c;
ch = get(c);
cout<<ch<<endl;
delete []ch;
ch = get("Jack");
cout<<ch<<endl;
delete []ch;
char *ch2 = "Mike";
ch = get(ch2);
cout<<ch<<endl;
delete []ch;

结构体
结构体是C时代的产物,到了C++之后结构体有了巨大的变化,可以增加
函数,可以设置成员的公有,私有和保护属性,可以从别的类继承,也
可以被别的类继承,可以有虚函数.唯一与类不同之后是结构体定义中
默认情况下的成员是public,而类定义中默认情况下的成员是private.
struct A{
public:// 有没有是一样的
    int get(){return x;}  
    void set(int i){x = i;}
private:
    int x;
};
使用:
A a;
set(9);
cout<<a.get()<<endl;

struct people{
  double weight;
  double tall;
  int age;
  char *name;
  char *native;
  bool sex;  
};
prople Jack={
  180.5,
  179.3,
  34,
  "Jack",
  "济南",
  1
};
cout<<Jack.name<<endl;
cout<<Jack.native<<endl;
cout<<Jack.tall<<endl;
cout<<Jack.weight<<endl;
cout<<Jack.age<<endl;

结构体中使用string
struct people{
  people(double t_weight, double t_tall, int t_age, 
         string t_name, char *t_native, bool t_sex);
  double weight;
  double tall;
  int age;
  string name;
  char *native;
  bool sex;
};
people::people(double t_weight, double t_tall, int t_age, 
         string t_name, char *t_native, bool t_sex){
  weight = t_weight;
  tall = t_tall;
  age = t_age;
  name = t_name;
  native = t_native;
  sex = t_sex;
};
people Jack(180.5, 179.3, 34, "Jack", "济南", 1);
由于是调用构造函数对Jack中的数据成员进行初始化,因此Jack后面没有
加赋值符(=),并且用括号替代了双引号.

结构体的赋值
既然结构是一种数据类型,那么我们就可以像对变量那样进行赋值操作,
不过前提是两个结构必须都是同类型的.
struct people{
  double weight;
  double tall;  
};
people Mike = {185.2, 178.1};
people Jack = {65.4, 165.2};
Mike = Jack;
output: 两者输出是一样的

结构体与函数
struct time{
  int hour;
  int minute;  
};
const int perhour = 60;
time sum(time t1, time t2){
    time total;
    total.minute = (t1.minute + t2.minute) % perhour;
    total.hour = t1.hour + t2.hour + (t1.minute + t2.minute)/perhour;
    return total;
}
void show(time t){
    cout<<t.hour<<":"<<t.minute<<endl;
}
time one = {8, 15};
time two = {6, 55};
time day = sum(one, two);
show(day);
time day3 = {9, 35};

time &sum(time t1, time t2){
    time *total = new time;
    total.minute = (t1.minute + t2.minute) % perhour;
    total.hour = t1.hour + t2.hour + (t1.minute + t2.minute)/perhour;
    return *total;
}
time *day = &sum(one, two);
show(*day);
time day2 = {9, 35};
time *p1 = &sum(*day, day2);
show(*p1);
delete day;
delete p1;

结构体与string
void show(const string p){
    cout<<p<<endl;
}
string str = "hello world";
show(str);

void show(const string &p){
    cout<<p<<endl;
}
show(str);

string数组与函数
假如要传递多个字符串,那么可以声明一个string对象数组,然后
将数组传递函数中.
void show(const string str[], int n){
    for(int i=0;i<n;i++){
        cout<<i+1<<":"<<str[i]<<endl;
    }
}
const int length = 5;
string str[length];
for(int i=0;i<length;i++){
    cout<<i+1<<":";
    cin>>str[i];
}
show(str, length);

string str[5];
char str[][5];
string对象数组与二维char型数组差不多.不同的是,string对象数组有
自动调节数组大小的功能,而二维char型数组却没有,因此使用二维char
型数组时,必须将数组定义得足够大,以避免数组越界.

流的使用
把控制数据流动的cin和cout叫做输入输出流.凡是跟输入输出流
有关系的类叫做流类,凡是实现流类功能的成员函数全部合并在一个
库中,该库叫做流类库.
重载输出运算符<<
class A{
  public:
    A(int x, int y)  {rx = x; ry = y}
  public:
    int rx;
    int ry;
};
ostream &operator<<(ostream &s, const A &a){
    s<<a.rx;// A的成员变量必须定义为public
    s<<a.ry;
    return s;
}

友元的方式重载输出运算符
class A{
  public:
    A(int x, int y)  {rx = x; ry = y;}
    friend ostream &operator<<(ostream &s, const A &a){
        s<<a.rx;
        s<<a.ry;
        return s;
    }
  private:
    int rx;
    int ry;
};
A a(3, 4), b(5, 6);
cout<<a<<b<<endl;

重载输入运算符>>
class A{
  public:
    A(int i)  {x = i;}
    friend istream &operator>>(istream &in, A &a){
        in>>a.x;
        return in;
    }
    friend ostream &operator<<(ostream &o, const A &a){
        o<<a.x;
        return o;
    }
  private:
    int x;
};
A a(3), b(4);
cin>>a>>b;
cout<<a<<b;

异常和错误处理
try{
    wrong();
}






















































































*/































































