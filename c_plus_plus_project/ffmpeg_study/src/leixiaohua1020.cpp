//
// Created by root on 18-7-24.
//

#ifndef MYSTUDY_FFMPEG_LEIXIAOHUA
#define MYSTUDY_FFMPEG_LEIXIAOHUA

#include "../include/MyHead.h"

/***
 frame->format为 FLTP类型(每个sample是float类型的)
 获取pcm音频帧每个声道的sample采样点个数
 frame->nb_samples=2048(每个声道2048个采样点)
 ffmpeg音频帧的物理存放方式含义:
 1.
 每个采样点数据的物理存储类型:
 大端小端方式
 无符号/有符号
 数据位数(8位,16位等)
 数据类型(整形，浮点类型等)
 2.
 不同声道的同一采样点是否单独存放:
 第一种:多个声道数据交错存放(packed类型,不带字符P)
 对于 packed音频(左右声道打包存放), 只有一个数据指针(相当于一个声道)。
 所有声道的数据交错排放在frame->data[0](即frame->extended_data[0])地址处
 所有声道的数据长度为linesize[0](单位:字节)
 第二种:每个声道数据单独存放(planar类型，带字符P)
 对于 planar音频(左右声道分开存放)，每个声道有自己的数据存放位置。
 声道0的起始地址为 frame->data[0](或frame->extended_data[0])
 声道1的起始地址为 frame->data[1](或frame->extended_data[1])
 声道i的起始地址为 frame->data[i](或frame->extended_data[i])
 每个声道的数据长度为linesize[0](单位:字节)
 两者之间的联系:
    所有声道的数据都是存放在 frame->data[0]开始的一段连续空间中
    如果是 packed类型，同一采样点的不同声道数据放到一起，然后存储下一个采样点
    如果是 planar类型，同一声道的所有采样点数据放到一起，然后存放下一个声道
 判断是否是planar类型:
 av_sample_fmt_is_planar(sample_fmt)


 */

///////////////////////////公共变量///////////////////////////

/***
 有关Context的指针可以理解成"管家","秘书".
 如AVFormatContext就好比音视频Format的管家.其保存着有关音视频的Format信息.
 Format管家:        AVFormatContext
 Codec管家:         AVCodecContext
 Codec分析器管家:    AVCodecParserContext
 对于视频有个Sws管家: SwsContext
 对于音视有个Swr管家: SwrContext
 */
enum AVCodecID avcodec_id = AV_CODEC_ID_NONE;
//格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
// AVFormatContext相当于Android的MediaExtractor,保存了音视频的Format信息(MediaFormat)
AVFormatContext *src_avformat_context = NULL;
AVFormatContext *dst_avformat_context = NULL;

//AVPacket结构体的作用是从内存中获取一个视频压缩帧,
//对于音频可能获取一个或者多个压缩帧.
AVPacket *avpacket = NULL;
//用于解析输入的数据流并把它分成一帧一帧的压缩编码数据。
AVFrame *avframe = NULL;
//比较形象的说法就是把长长的一段连续的数据“切割”成一段段的数据。
//他的核心函数是av_parser_parse2()。
AVCodecParserContext *avcodec_parser_context = NULL;

int video_stream_index = -1;
int audio_stream_index = -1;
//用于线程
int thread_pause_flag = 0;
int thread_exit_flag = 0;

char *in_file_path = "/root/视频/tomcat_video/game_of_thrones_5_01.mp4";
//char *in_file_path = "/root/视频/tomcat_video/yuv/256_256_yuv420p_y.y";
//char *in_file_path = "/root/视频/yuv/240_240_rgb24_haoke.yuv";
//char *out_file_path = "/root/视频/rgb/720_480_rgb24.rgb";
char *out_file_path = "/root/视频/tomcat_video/yuv/haoke.yuv";

FILE *in_file = NULL;
FILE *out_file = NULL;

int ret = -1, got_picture_ptr = -1;

///////////////////////////video///////////////////////////

//SwsContext作为sws_scale的第一个参数,记录数据要转换的格式、大小及转换方式(用于图像像素的处理)
struct SwsContext *video_sws_context = NULL;
//源视频宽高
int src_video_width = 0, src_video_height = 0;
size_t src_video_area = 0;
int dst_video_width = 0, dst_video_height = 0;
size_t dst_video_area = 0;
int playback_window_w = 0, playback_window_h = 0;
// 编解码器的管家(通过这个管家才能找到编解码器,而且本身也保存了好多的信息)
AVCodecContext *video_avcodec_context = NULL;
AVCodecParameters *video_avcodec_parameters = NULL;
// Decoder解码器 Encoder编码器
AVCodec *video_avcodec_decoder = NULL, *video_avcodec_encoder = NULL;
// src_avframe保存原始帧 dst_avframe转换成yuv后的帧保存在此处
AVFrame *src_video_avframe = NULL, *dst_video_avframe = NULL;
//先自己申请空间,然后为初始化video_dst_avframe而服务
unsigned char *video_out_buffer = NULL;
uint8_t *video_out_buffer2 = NULL;
size_t video_out_buffer_size = 0;
int video_frame_count = 0;
//使用到sws_scale函数时需要定义这些变量
uint8_t *src_video_data[4] = {NULL}, *dst_video_data[4] = {NULL};
int src_video_linesize[4] = {0}, dst_video_linesize[4] = {0};
//像素格式
enum AVPixelFormat src_avpixel_format = AV_PIX_FMT_NONE;
enum AVPixelFormat dst_avpixel_format = AV_PIX_FMT_YUV420P;

///////////////////////////audio///////////////////////////

struct SwrContext *audio_swr_context = NULL;
AVCodecContext *audio_avcodec_context = NULL;
AVCodecParameters *audio_avcodec_parameters = NULL;
//
AVFrame *src_audio_avframe = NULL, *dst_audio_avframe = NULL;
//Decoder Encoder
AVCodec *audio_avcodec_decoder = NULL, *audio_avcodec_encoder = NULL;
unsigned char *audio_out_buffer = NULL;
uint8_t *audio_out_buffer2 = NULL;
size_t audio_out_buffer_size = 0;
int audio_frame_count = 0;
//
uint8_t *src_audio_data[4] = {NULL}, *dst_audio_data[4] = {NULL};
int src_audio_linesize[4] = {0}, dst_audio_linesize[4] = {0};

///////////////////////////音频重采样使用///////////////////////////

//声道布局
uint64_t src_ch_layout = AV_CH_LAYOUT_STEREO, dst_ch_layout = AV_CH_LAYOUT_STEREO;
//采样率
int src_sample_rate = 0, dst_sample_rate = 0;
//采样格式(一个采样点占用的字节数)
enum AVSampleFormat src_avsample_format = AV_SAMPLE_FMT_NONE, dst_avsample_format = AV_SAMPLE_FMT_S16;
//声道数(一般使用双声道)
int src_nb_channels = 0, dst_nb_channels = 0;
//一个声道采用的sample采样点个数(aac = 1024, mp3 = 1152)
int src_nb_samples = 0, dst_nb_samples = 0, max_dst_nb_samples = 0;

///////////////////////////SDL2///////////////////////////

struct SDL_Rect sdl_rect;
SDL_Window *sdl_window = NULL;
SDL_Renderer *sdl_renderer = NULL;
SDL_Texture *sdl_texture = NULL;
SDL_Thread *sdl_thread = NULL;
struct SDL_AudioSpec *sdl_audio_spec = NULL;
SDL_Event sdl_event;

////////////////////////////////////////////////////////////////////////////////////

void close();

int alexander_refresh_video_thread(void *);

void init_av() {
    // 注册复用器和编解码器，所有的使用ffmpeg，首先必须调用这个函数
    av_register_all();
    // 用于从网络接收数据，如果不是网络接收数据，可不用（如本例可不用）
    avformat_network_init();
    // 注册设备的函数，如用获取摄像头数据或音频等，需要此函数先注册
    // avdevice_register_all();
    // AVFormatContext初始化，里面设置结构体的一些默认信息
    // 相当于Java中创建对象
    src_avformat_context = avformat_alloc_context();
}

/***
 先要得到src_video_width和src_video_height的值
 */
int init_sdl() {
    if (src_video_width == 0 || src_video_height == 0) {
        printf("%s", "没有设置src_video_width和src_video_height的值");
        return -1;
    }

    //得到文件名
    /*char *file_name = NULL;
    char temp_file_path[1024];
    strncpy(temp_file_path, in_file_path, strlen(in_file_path));
    strncpy(temp_file_path, temp_file_path + 1, strlen(temp_file_path));
    char *pchar = strtok(temp_file_path, "/");
    while (pchar != NULL) {
        file_name = pchar;
        pchar = strtok(NULL, "/");
        if (pchar == NULL) {
            break;
        }
    }
    printf("file_name: %s\n", file_name);*/

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    //SDL 2.0 Support for multiple windows
    sdl_window = SDL_CreateWindow(in_file_path,
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  src_video_width, src_video_height,
                                  SDL_WINDOW_OPENGL);
    if (sdl_window == NULL) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    sdl_texture = SDL_CreateTexture(sdl_renderer,
                                    SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                    src_video_width, src_video_height);
    sdl_rect.x = 0;
    sdl_rect.y = 0;
    sdl_rect.w = src_video_width;
    sdl_rect.h = src_video_height;

    sdl_thread = SDL_CreateThread(alexander_refresh_video_thread, NULL, NULL);
}

void close() {
    // 清理工作
    if (sdl_window != NULL) {
        SDL_Quit();
    }
    if (sdl_thread != NULL) {
        SDL_DetachThread(sdl_thread);
        sdl_thread = NULL;
    }
    if (sdl_audio_spec != NULL) {
        SDL_CloseAudio();
        sdl_audio_spec = NULL;
    }
    //video
    if (video_out_buffer != NULL) {
        av_free(video_out_buffer);
        video_out_buffer = NULL;
    }
    if (video_out_buffer2 != NULL) {
        av_free(video_out_buffer2);
        video_out_buffer2 = NULL;
    }
    if (src_video_data[0] != NULL) {
        av_freep(&src_video_data[0]);
        src_video_data[0] = NULL;
    }
    if (dst_video_data[0] != NULL) {
        av_freep(&dst_video_data[0]);
        dst_video_data[0] = NULL;
    }
    if (video_sws_context != NULL) {
        sws_freeContext(video_sws_context);
        video_sws_context = NULL;
    }
    if (src_video_avframe != NULL) {
        av_frame_free(&src_video_avframe);
        src_video_avframe = NULL;
    }
    if (dst_video_avframe != NULL) {
        av_frame_free(&dst_video_avframe);
        dst_video_avframe = NULL;
    }
    if (video_avcodec_parameters != NULL) {
        avcodec_parameters_free(&video_avcodec_parameters);
        video_avcodec_parameters = NULL;
    }
    if (video_avcodec_context != NULL) {
        avcodec_close(video_avcodec_context);
        av_free(video_avcodec_context);
        video_avcodec_context = NULL;
    }
    //audio
    if (audio_out_buffer != NULL) {
        av_free(audio_out_buffer);
        audio_out_buffer = NULL;
    }
    if (audio_out_buffer2 != NULL) {
        av_free(audio_out_buffer2);
        audio_out_buffer2 = NULL;
    }
    if (audio_swr_context != NULL) {
        swr_close(audio_swr_context);
        swr_free(&audio_swr_context);
        audio_swr_context = NULL;
    }
    if (src_audio_avframe != NULL) {
        av_frame_free(&src_audio_avframe);
        src_audio_avframe = NULL;
    }
    if (dst_audio_avframe != NULL) {
        av_frame_free(&dst_audio_avframe);
        dst_audio_avframe = NULL;
    }
    if (src_audio_data[0] != NULL) {
        av_freep(&src_audio_data[0]);
        src_audio_data[0] = NULL;
    }
    if (dst_audio_data[0] != NULL) {
        av_freep(&dst_audio_data[0]);
        dst_audio_data[0] = NULL;
    }
    if (audio_avcodec_parameters != NULL) {
        avcodec_parameters_free(&audio_avcodec_parameters);
        audio_avcodec_parameters = NULL;
    }
    if (audio_avcodec_context != NULL) {
        avcodec_close(audio_avcodec_context);
        av_free(audio_avcodec_context);
        audio_avcodec_context = NULL;
    }

    if (avframe != NULL) {
        av_frame_free(&avframe);
        avframe = NULL;
    }
    if (avpacket != NULL) {
        av_free(&avpacket);
        avpacket = NULL;
    }
    if (src_avformat_context != NULL) {
        avformat_close_input(&src_avformat_context);
        src_avformat_context = NULL;
    }
    if (in_file != NULL) {
        fclose(in_file);
        in_file = NULL;
    }
    if (out_file != NULL) {
        fclose(out_file);
        out_file = NULL;
    }
}

/***
 src_avformat_context相当于初始化操作
 */
int avformat_open_and_find() {
    if (avformat_open_input(&src_avformat_context, in_file_path, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(src_avformat_context, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }
}

/***
 找到音视频流的轨道
 */
void find_stream_index() {
    int i;
    int nb_streams = src_avformat_context->nb_streams;
    for (i = 0; i < nb_streams; i++) {
        AVMediaType codec_type = src_avformat_context->streams[i]->codec->codec_type;
        if (codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
        } else if (codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
        }
    }
    if (video_stream_index == -1) {
        printf("Didn't find a video stream.\n");
    } else {
        printf("video_stream_index = %d\n", video_stream_index);
    }
    if (audio_stream_index == -1) {
        printf("Didn't find a audio stream.\n");
    } else {
        printf("audio_stream_index = %d\n", audio_stream_index);
    }
}

int open_video_avcodec_decoder() {
    if (video_stream_index != -1) {
        video_avcodec_context = src_avformat_context->streams[video_stream_index]->codec;
        // 找到编码器
        video_avcodec_decoder = avcodec_find_decoder(video_avcodec_context->codec_id);
    }
    if (video_avcodec_decoder == NULL) {
        printf("Codec not found.\n");
        return -1;
    }
    // 打开编码器
    if (avcodec_open2(video_avcodec_context, video_avcodec_decoder, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }
}

int open_audio_avcodec_decoder() {
    if (audio_stream_index != -1) {
        audio_avcodec_context = src_avformat_context->streams[audio_stream_index]->codec;
        audio_avcodec_decoder = avcodec_find_decoder(audio_avcodec_context->codec_id);
    }
    if (audio_avcodec_decoder == NULL) {
        printf("Codec not found.\n");
        return -1;
    }
    if (avcodec_open2(audio_avcodec_context, audio_avcodec_decoder, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }
}

int create_video_sws_context() {
    src_video_width = video_avcodec_context->width;
    src_video_height = video_avcodec_context->height;
    src_video_area = src_video_width * src_video_height;
    src_avpixel_format = video_avcodec_context->pix_fmt;
    // 为要操作的视频帧先申请空间
    src_video_avframe = av_frame_alloc();
    dst_video_avframe = av_frame_alloc();
    avpacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(avpacket);

    int image_get_buffer_size = av_image_get_buffer_size(dst_avpixel_format,
                                                         src_video_width,
                                                         src_video_height,
                                                         1);
    video_out_buffer = (unsigned char *) av_malloc(image_get_buffer_size);
    av_image_fill_arrays(dst_video_avframe->data, dst_video_avframe->linesize,
                         video_out_buffer,
                         dst_avpixel_format,
                         src_video_width, src_video_height,
                         1);
    video_sws_context = sws_getContext(src_video_width, src_video_height, src_avpixel_format,
                                       src_video_width, src_video_height, dst_avpixel_format,
                                       SWS_BICUBIC,//flags
                                       NULL, NULL, NULL);
    if (video_sws_context == NULL) {
        printf("%s\n", "video_sws_context is NULL");
        return -1;
    }

    printf("src_video_width = %d, src_video_height = %d\n",
           src_video_width, src_video_height);
    printf("image_get_buffer_size = %d\n", image_get_buffer_size);
    //即使源文件像素格式与目标文件像素格式一样,也是需要调用sws_scale函数进行处理的
    printf("src_avpixel_format = %d\n", src_avpixel_format);
    printf("dst_avpixel_format = %d\n", dst_avpixel_format);
}

int open_files() {
#if OUTPUT_YUV420P
    char temp_out_file_path[1024];
    char str[10];
    memset(temp_out_file_path, 0, strlen(temp_out_file_path));
    strcat(temp_out_file_path, "/");

    char *file_name;
    char temp_file_path[1024];
    strncpy(temp_file_path, out_file_path, strlen(out_file_path));
    strncpy(temp_file_path, temp_file_path + 1, strlen(temp_file_path));
    char *pchar = strtok(temp_file_path, "/");
    while (pchar != NULL) {
        file_name = pchar;
        pchar = strtok(NULL, "/");
        if (pchar == NULL) {
            break;
        }
        strcat(temp_out_file_path, file_name);
        strcat(temp_out_file_path, "/");
    }
    sprintf(str, "%d", src_video_width);
    strcat(temp_out_file_path, str);
    strcat(temp_out_file_path, "_");
    sprintf(str, "%d", src_video_height);
    strcat(temp_out_file_path, str);
    strcat(temp_out_file_path, "_");
    strcat(temp_out_file_path, "yuv420p");
    strcat(temp_out_file_path, "_");
    strcat(temp_out_file_path, file_name);
    printf("out_file_path: %s\n", temp_out_file_path);

    out_file = fopen(temp_out_file_path, "wb+");
    if (out_file == NULL) {
        printf("%s\n", "输出文件打开有问题");
        return -1;
    }
#else
    if (out_file_path != NULL) {
        out_file = fopen(out_file_path, "wb+");
        if (out_file == NULL) {
            printf("%s\n", "输出文件打开有问题");
            return -1;
        }
    }
#endif
    if (in_file_path != NULL) {
        in_file = fopen(in_file_path, "rb+");
        if (in_file == NULL) {
            printf("%s\n", "输入文件打开有问题");
            return -1;
        }
    }
}

void av_dump_format() {
#if AV_DUMP_FORMAT
    printf("-------------File Information-------------\n");
    av_dump_format(src_avformat_context, 0, in_file_path, 0);
    printf("------------------------------------------\n");
#endif
}

//线程
int alexander_refresh_video_thread(void *opaque) {
    thread_exit_flag = 0;
    thread_pause_flag = 0;
    while (!thread_exit_flag) {
        if (!thread_pause_flag) {
            SDL_Event event;
            event.type = REFRESH_EVENT;
            SDL_PushEvent(&event);
        }
        SDL_Delay(40);
    }
    thread_exit_flag = 0;
    thread_pause_flag = 0;
    //Break
    SDL_Event event;
    event.type = BREAK_EVENT;
    SDL_PushEvent(&event);
    printf("SDL_PushEvent BREAK_EVENT\n");

    return 0;
}

/***
 播放流畅,代码比较好
 无声电影

 流程:
 1.注册跟编解码器相关的东西
 2.对AVFormatContext指针进行初始化操作
 3.找到音视频流的轨道
 4.找到并打开视频解码器
 */
int alexander_video_player_sdl2() {
    init_av();

    if (avformat_open_and_find() < 0) {
        return -1;
    }

    find_stream_index();

    if (open_video_avcodec_decoder() < 0) {
        return -1;
    }

    if (create_video_sws_context() < 0) {
        printf("%s\n", "");
        return -1;
    }

    init_sdl();
    if (sdl_thread == NULL) {
        printf("%s\n", "");
        return -1;
    }

    av_dump_format();

    for (;;) {
        //Wait
        SDL_WaitEvent(&sdl_event);
        if (sdl_event.type == REFRESH_EVENT) {
            while (1) {
                if (av_read_frame(src_avformat_context, avpacket) < 0) {
                    thread_exit_flag = 1;
                }

                if (avpacket->stream_index == video_stream_index) {
                    break;
                }
            }
            ret = avcodec_decode_video2(video_avcodec_context, src_video_avframe, &got_picture_ptr, avpacket);
            if (ret < 0) {
                printf("Decode Error.\n");
                return -1;
            }
            if (got_picture_ptr) {
                sws_scale(video_sws_context, (const unsigned char *const *) src_video_avframe->data,
                          src_video_avframe->linesize, 0,
                          src_video_height, dst_video_avframe->data, dst_video_avframe->linesize);

                //SDL---------------------------
                SDL_UpdateTexture(sdl_texture, NULL, dst_video_avframe->data[0], dst_video_avframe->linesize[0]);
                SDL_RenderClear(sdl_renderer);
                //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
                SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
                SDL_RenderPresent(sdl_renderer);
                //SDL End-----------------------
            }
            av_free_packet(avpacket);
        } else if (sdl_event.type == SDL_KEYDOWN) {
            //Pause
            if (sdl_event.key.keysym.sym == SDLK_SPACE) {
                thread_pause_flag = !thread_pause_flag;
            }
        } else if (sdl_event.type == SDL_QUIT) {
            thread_exit_flag = 1;
        } else if (sdl_event.type == BREAK_EVENT) {
            break;
        }
    }// for(;;) end

    close();

    return 0;
}

void *alexander_decode_video_thread(void *) {
    while (av_read_frame(src_avformat_context, avpacket) >= 0) {
        if (avpacket->stream_index == video_stream_index) {
            ret = avcodec_decode_video2(video_avcodec_context, src_video_avframe, &got_picture_ptr, avpacket);
            if (ret < 0) {
                printf("Decode Error.\n");
                break;
            }
            if (got_picture_ptr) {
                //像素格式转换,改变大小(放大或缩小)
                sws_scale(video_sws_context,
                          (const unsigned char *const *) src_video_avframe, src_video_avframe->linesize,
                          0, src_video_height, dst_video_avframe->data, dst_video_avframe->linesize);

                src_video_area = src_video_width * src_video_height;
                //Y
                fwrite(dst_video_avframe->data[0], 1, src_video_area, out_file);
                //U
                fwrite(dst_video_avframe->data[1], 1, src_video_area / 4, out_file);
                //V
                fwrite(dst_video_avframe->data[2], 1, src_video_area / 4, out_file);

                ++video_frame_count;
            }
        }
        av_free_packet(avpacket);
    }

    //flush decoder
    //FIX: Flush Frames remained in Codec
    while (1) {
        ret = avcodec_decode_video2(video_avcodec_context, src_video_avframe, &got_picture_ptr, avpacket);
        if (ret < 0) {
            break;
        }
        if (!got_picture_ptr) {
            break;
        }
        sws_scale(video_sws_context,
                  (const unsigned char *const *) src_video_avframe, src_video_avframe->linesize,
                  0, src_video_height, dst_video_avframe->data, dst_video_avframe->linesize);

        src_video_area = src_video_width * src_video_height;
        //Y
        fwrite(dst_video_avframe->data[0], 1, src_video_area, out_file);
        //U
        fwrite(dst_video_avframe->data[1], 1, src_video_area / 4, out_file);
        //V
        fwrite(dst_video_avframe->data[2], 1, src_video_area / 4, out_file);

        ++video_frame_count;
    }
    printf("Succeed to decode %d frame.\n", video_frame_count);
}

int alexander_decode_video_to_yuv() {
    init_av();

    if (avformat_open_and_find() < 0) {
        return -1;
    }

    find_stream_index();

    if (open_video_avcodec_decoder() < 0) {
        return -1;
    }

    if (create_video_sws_context() < 0) {
        printf("%s\n", "");
        return -1;
    }

    if (open_files() < 0) {
        return -1;
    }

    av_dump_format();

    pthread_t decode_video_thread;
    //创建线程(使用线程进行解码)
    if (pthread_create(&decode_video_thread, NULL, alexander_decode_video_thread, NULL) == -1) {
        puts("Fail to create pthread decode_video_thread.\n");
        return -1;
    }
    //等待线程结束
    void *result;
    if (pthread_join(decode_video_thread, &result) == -1) {
        puts("fail to recollect t0");
        return -1;
    }

    close();

    return 0;
}

int alexander_playback_yuv() {
    char *pc = NULL;
    char *temp_file_path = NULL;
    char temp_path[strlen(in_file_path)];
    memset(temp_path, 0, strlen(in_file_path));
    strncpy(temp_path, in_file_path + 1, strlen(in_file_path) - 1);
    printf("temp_path: %s\n", temp_path);
    pc = strtok(temp_path, "/");
    while (pc != NULL) {
        if (strstr(pc, ".yuv") != NULL
            || strstr(pc, ".rgb") != NULL
            || strstr(pc, ".y") != NULL) {
            temp_file_path = pc;
            printf("in_file_name: %s\n", temp_file_path);
            break;
        }
        pc = strtok(NULL, "/");
    }
    pc = strtok(temp_file_path, "_");
    int i = 0;
    while (pc != NULL) {
        if (i == 0) {
            src_video_width = atoi(pc);
        } else if (i == 1) {
            src_video_height = atoi(pc);
        } else {
            break;
        }
        i++;
        pc = strtok(NULL, "_");
    }
    printf("src_video_width: %d, src_video_height: %d\n", src_video_width, src_video_height);

    init_sdl();
    if (sdl_thread == NULL) {
        printf("%s\n", "sdl_thread is NULL.");
        return -1;
    }

    if (open_files() < 0) {
        return -1;
    }

    const int bpp = 12;
    unsigned char video_out_buffer[src_video_width * src_video_height * bpp / 8];
    while (1) {
        SDL_WaitEvent(&sdl_event);
        if (sdl_event.type == REFRESH_EVENT) {
            if (fread(video_out_buffer, 1, src_video_width * src_video_height * bpp / 8, in_file)
                != src_video_width * src_video_height * bpp / 8) {
                // Loop
                fseek(in_file, 0, SEEK_SET);
                fread(video_out_buffer, 1, src_video_width * src_video_height * bpp / 8, in_file);
            }

            SDL_UpdateTexture(sdl_texture, NULL, video_out_buffer, src_video_width);

            //FIX: If window is resize
            sdl_rect.x = 0;
            sdl_rect.y = 0;
            sdl_rect.w = src_video_width;
            sdl_rect.h = src_video_height;

            SDL_RenderClear(sdl_renderer);
            SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, &sdl_rect);
            SDL_RenderPresent(sdl_renderer);
        } else if (sdl_event.type == SDL_WINDOWEVENT) {
            //If Resize
            SDL_GetWindowSize(sdl_window, &src_video_width, &src_video_height);
        } else if (sdl_event.type == SDL_QUIT) {
            thread_exit_flag = 1;
        } else if (sdl_event.type == BREAK_EVENT) {
            break;
        }
    }// while end

    close();

    return 0;
}

/***
 只使用到libavcodec库
 一个“纯净”的解码器，理论上说只需要使用libavcodec就足够了，并不需要使用libavformat。
 一个“纯净”的解码器，它仅仅通过调用libavcodec将H.264/HEVC等格式的压缩视频码流解码成为YUV数据。
 此解码器的输入必须是只包含视频编码数据“裸流”（例如H.264、HEVC码流文件）,
 而不能是包含封装格式的媒体数据（例如AVI、MKV、MP4）。
 */
int alexander_use_libavcodec_decode_to_yuv() {
    //error
//    memset(in_file_path, 0, strlen(in_file_path));
//    memset(out_file_path, 0, strlen(out_file_path));

#define TEST_HEVC 0
#define TEST_H264 0

#if TEST_HEVC
    //HEVC
    avcodec_id = AV_CODEC_ID_HEVC;
    in_file_path = "/root/视频/bigbuckbunny_480x272.hevc";
#elif TEST_H264
    //H.264
    avcodec_id = AV_CODEC_ID_H264;
    in_file_path = "/root/视频/bigbuckbunny_480x272.h264";
#else
    //MPEG2
    avcodec_id = AV_CODEC_ID_MPEG2VIDEO;
    in_file_path = "/root/视频/bigbuckbunny_480x272.m2v";
#endif

    out_file_path = "/root/视频/yuv/480_272_bigbuckbunny.yuv";

    uint8_t out_buffer[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE] = {0};
    int first_time = 1;

    avcodec_register_all();
    //解码特定的格式(根据格式找到解码器,大多数视频的解码器是能找到的,编码器就比较少了)
    video_avcodec_decoder = avcodec_find_decoder(avcodec_id);
    if (!video_avcodec_decoder) {
        printf("%s\n", "video_avcodec_decoder is NULL.");
        return -1;
    }
    video_avcodec_context = avcodec_alloc_context3(video_avcodec_decoder);
    if (!video_avcodec_context) {
        printf("%s\n", "video_avcodec_context is NULL.");
        return -1;
    }
    if (avcodec_open2(video_avcodec_context, video_avcodec_decoder, NULL) < 0) {
        printf("%s\n", "avcodec_open2 failure");
        return -1;
    }

    avcodec_parser_context = av_parser_init(avcodec_id);
    if (!avcodec_parser_context) {
        printf("%s\n", "avcodec_parser_context is NULL.");
        return -1;
    }

    if (open_files() < 0) {
        return -1;
    }

    avframe = av_frame_alloc();
    avpacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(avpacket);

    while (1) {
        video_out_buffer_size = fread(out_buffer, 1, INBUF_SIZE, in_file);
        printf("video_out_buffer_size = %d\n", video_out_buffer_size);
        if (!video_out_buffer_size) {
            break;
        }
        video_out_buffer2 = out_buffer;
        while (video_out_buffer_size > 0) {
            /***
             使用AVCodecParser从输入的数据流中分离出一帧一帧的压缩编码数据。
             其中poutbuf指向解析后输出的压缩编码数据帧，buf指向输入的压缩编码数据。
             如果函数执行完后输出数据为空（poutbuf_size为0），则代表解析还没有完成，
             还需要再次调用av_parser_parse2()解析一部分数据才可以得到解析后的数据帧。
             当函数执行完后输出数据不为空的时候，代表解析完成，可以将poutbuf中的这帧数据取出来做后续处理。
             */
            int len = av_parser_parse2(avcodec_parser_context, video_avcodec_context,
                                       &(avpacket->data), &(avpacket->size),
                                       video_out_buffer2, video_out_buffer_size,
                                       AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

            printf("len: %6d\n", len);
            printf("avpacket->all_pkts_size: %6d\n", avpacket->size);
            video_out_buffer2 += len;
            video_out_buffer_size -= len;
            if (!avpacket->size) {
                //继续解析
                continue;
            }
            switch (avcodec_parser_context->pict_type) {
                case AV_PICTURE_TYPE_I:
                    printf("Type:\tI帧\t");
                    break;
                case AV_PICTURE_TYPE_P:
                    printf("Type:\tP帧\t");
                    break;
                case AV_PICTURE_TYPE_B:
                    printf("Type:\tB帧\t");
                    break;
                default:
                    printf("Type:Other\t");
                    break;
            }
            printf("Number:\t%d\n", avcodec_parser_context->output_picture_number);

            ret = avcodec_decode_video2(video_avcodec_context, avframe, &got_picture_ptr, avpacket);
            if (ret < 0) {
                printf("Decode Error.\n");
                return ret;
            }
            if (got_picture_ptr) {
                if (first_time) {
                    //解码一帧成功后才能得到某些信息
                    //纯净”的解码器中，可以通过avcodec_decode_video2()获得这些信息。
                    //因此我们只有在成功解码第一帧之后，才能通过读取相应的字段获取到这些信息。
                    printf("\nCodec Full Name: %s\n", video_avcodec_context->codec->long_name);
                    printf("width: %d\nheight: %d\n\n", video_avcodec_context->width, video_avcodec_context->height);
                    first_time = 0;
                }

                //Y U V
                int i;
                for (i = 0; i < avframe->height; i++) {
                    fwrite(avframe->data[0] + avframe->linesize[0] * i, 1, avframe->width, out_file);
                }
                for (i = 0; i < avframe->height / 2; i++) {
                    fwrite(avframe->data[1] + avframe->linesize[1] * i, 1, avframe->width / 2, out_file);
                }
                for (i = 0; i < avframe->height / 2; i++) {
                    fwrite(avframe->data[2] + avframe->linesize[2] * i, 1, avframe->width / 2, out_file);
                }

                video_frame_count++;
            }
        }//while end
    }//while end

    //Flush Decoder
    avpacket->data = NULL;
    avpacket->size = 0;
    while (1) {
        ret = avcodec_decode_video2(video_avcodec_context, avframe, &got_picture_ptr, avpacket);
        if (ret < 0) {
            printf("Decode Error.\n");
            return ret;
        }
        if (!got_picture_ptr) {
            break;
        }

        //Y, U, V
        for (int i = 0; i < avframe->height; i++) {
            fwrite(avframe->data[0] + avframe->linesize[0] * i, 1, avframe->width, out_file);
        }
        for (int i = 0; i < avframe->height / 2; i++) {
            fwrite(avframe->data[1] + avframe->linesize[1] * i, 1, avframe->width / 2, out_file);
        }
        for (int i = 0; i < avframe->height / 2; i++) {
            fwrite(avframe->data[2] + avframe->linesize[2] * i, 1, avframe->width / 2, out_file);
        }

        video_frame_count++;
    }//while end

    printf("Decoder: Succeed to decode %d frame.\n", video_frame_count);

    close();

    return 0;
}

/***
 网上的代码
 播放窗口大小改变
 总结:

 */
int alexander_how_to_use_sws_scale() {
    const char *srcFileName = "/root/视频/yuv/720_576_yuv420p_haoke.yuv";
    const char *dstFileName = "/root/视频/yuv/240_240_rgb24_haoke.yuv";

    struct SwsContext *img_convert_ctx = nullptr;

    // 設定原始 YUV 的長寬
    const int in_width = 720;
    const int in_height = 576;

    // 設定目的 YUV 的長寬
    const int out_width = 240;
    const int out_height = 240;

    const int read_size = in_width * in_height * 3 / 2;
    const int write_size = out_width * out_height * 3 / 2;

    //用于保存yuv的各个分量的数据
    uint8_t *inbuf[4];
    uint8_t *outbuf[4];

    //不知道为什么要这样定义
    int inlinesize[4] = {in_width, in_width / 2, in_width / 2, 0};
    int outlinesize[4] = {out_width, out_width / 2, out_width / 2, 0};

    uint8_t *ptr_src_yuv_buf = nullptr;
    uint8_t *ptr_dst_yuv_buf = nullptr;
    ptr_src_yuv_buf = new uint8_t[read_size];
    ptr_dst_yuv_buf = new uint8_t[write_size];

    FILE *fin = fopen(srcFileName, "rb");
    FILE *fout = fopen(dstFileName, "wb");

    if (fin == NULL) {
        fprintf(stderr, "open input file %s error.\n", srcFileName);
        return -1;
    }

    if (fout == NULL) {
        fprintf(stderr, "open output file %s error.\n", dstFileName);
        return -1;
    }

    //保存多少数据就申请多少空间
    inbuf[0] = (uint8_t *) malloc(in_width * in_height);
    inbuf[1] = (uint8_t *) malloc(in_width * in_height >> 2);//in_width * in_height / 4
    inbuf[2] = (uint8_t *) malloc(in_width * in_height >> 2);
    inbuf[3] = NULL;

    outbuf[0] = (uint8_t *) malloc(out_width * out_height);
    //向右移动2位,就是除以4(2的2次方)；向右移动3位,就是除以8(2的3次方)；
    //向左移动2位,就是乘以4(2的2次方)；向左移动3位,就是乘以8(2的3次方)；
    outbuf[1] = (uint8_t *) malloc(out_width * out_height >> 2);
    outbuf[2] = (uint8_t *) malloc(out_width * out_height >> 2);
    outbuf[3] = NULL;

    // ********* Initialize software scaling *********
    // ********* sws_getContext **********************
    img_convert_ctx = sws_getContext(in_width, in_height, AV_PIX_FMT_YUV420P,
                                     out_width, out_height, AV_PIX_FMT_YUV420P,
                                     SWS_POINT,
                                     nullptr, nullptr, nullptr);
    if (img_convert_ctx == NULL) {
        fprintf(stderr, "Cannot initialize the conversion context!\n");
        return -1;
    }

    int32_t in_y_size = in_width * in_height;
    int32_t out_y_size = out_width * out_height;

    bool bExit = false;
    while (!bExit) {
        //
        if ((fread(ptr_src_yuv_buf, 1, read_size, fin) < 0) || (feof(fin))) {
            bExit = true;
            break;
        }

        memcpy(inbuf[0], ptr_src_yuv_buf, in_y_size);
        memcpy(inbuf[1], ptr_src_yuv_buf + in_y_size, in_y_size / 4);
        memcpy(inbuf[2], ptr_src_yuv_buf + in_y_size * 5 / 4, in_y_size / 4);

        // ********* 主要的 function ******
        // ********* sws_scale ************
        sws_scale(img_convert_ctx, inbuf, inlinesize,
                  0, in_height, outbuf, outlinesize);

        memcpy(ptr_dst_yuv_buf, outbuf[0], out_y_size);
        memcpy(ptr_dst_yuv_buf + out_y_size, outbuf[1], out_y_size >> 2);
        memcpy(ptr_dst_yuv_buf + (out_y_size * 5 >> 2), outbuf[2], out_y_size >> 2);

        fwrite(ptr_dst_yuv_buf, 1, write_size, fout);
    }
    // ********* 結束的 function *******
    // ********* sws_freeContext *******
    sws_freeContext(img_convert_ctx);

    fclose(fin);
    fclose(fout);

    delete[] ptr_src_yuv_buf;
    ptr_src_yuv_buf = nullptr;

    delete[] ptr_dst_yuv_buf;
    ptr_dst_yuv_buf = nullptr;

    return 0;
}

static void fill_yuv_image(uint8_t *data[4], int linesize[4],
                           int width, int height, int frame_index) {
    int x, y;

    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            data[0][y * linesize[0] + x] = x + y + frame_index * 3;

    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            data[1][y * linesize[1] + x] = 128 + y + frame_index * 2;
            data[2][y * linesize[2] + x] = 64 + x + frame_index * 5;
        }
    }
}

/***
 官方的sample
 总结:
 1.
 要使用到sws_scale函数,那么先需要定义下面这些变量:
 源视频宽高,像素格式
 目标视频宽高,像素格式
 uint8_t *src_data[4];源视频数据
 int src_linesize[4]源视频的行数
 uint8_t *dst_data[4];目标视频数据
 int dst_linesize[4];目标视频的行数
 int dst_bufsize;存放目标视频数据申请的空间大小
 */
int alexander_how_to_use_sws_scale2() {
    uint8_t *src_data[4], *dst_data[4];
    int src_linesize[4], dst_linesize[4];
    src_video_width = 1440;
    src_video_height = 960;
    src_avpixel_format = AV_PIX_FMT_YUV420P;
    dst_avpixel_format = AV_PIX_FMT_RGB24;
    const char *dst_size = NULL;
    int dst_bufsize;
    int i, ret;

    dst_size = "720x480";

    if (av_parse_video_size(&dst_video_width, &dst_video_height, dst_size) < 0) {
        fprintf(stderr,
                "Invalid all_pkts_size '%s', must be in the form WxH or a valid all_pkts_size abbreviation\n",
                dst_size);
        exit(1);
    }

    out_file = fopen(out_file_path, "wb");
    if (!out_file) {
        fprintf(stderr, "Could not open destination file %s\n", out_file_path);
        exit(1);
    }

    /* create scaling context */
    video_sws_context = sws_getContext(src_video_width, src_video_height, src_avpixel_format,
                                       dst_video_width, dst_video_height, dst_avpixel_format,
                                       SWS_BILINEAR,
                                       NULL, NULL, NULL);
    if (!video_sws_context) {
        fprintf(stderr,
                "Impossible to create scale context for the conversion "
                "fmt:%s s:%dx%d -> fmt:%s s:%dx%d\n",
                av_get_pix_fmt_name(src_avpixel_format), src_video_width, src_video_height,
                av_get_pix_fmt_name(dst_avpixel_format), dst_video_width, dst_video_height);
        ret = AVERROR(EINVAL);
        return -1;;
    }

    /* allocate source and destination image buffers */
    //申请空间
    ret = av_image_alloc(src_data, src_linesize,
                         src_video_width, src_video_height,
                         src_avpixel_format, 16);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate source image\n");
        return -1;;
    }

    /* buffer is going to be written to rawvideo file, no alignment */
    //申请空间
    ret = av_image_alloc(dst_data, dst_linesize,
                         dst_video_width, dst_video_height,
                         dst_avpixel_format, 1);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
        return -1;;
    }
    dst_bufsize = ret;

    for (i = 0; i < 100; i++) {
        /* generate synthetic video */
        fill_yuv_image(src_data, src_linesize, src_video_width, src_video_height, i);

        /* convert to destination format */
        /***
         int sws_scale(struct SwsContext *c,
                         const uint8_t *const srcSlice[], const int srcStride[],
                         int srcSliceY, int srcSliceH,
                          uint8_t *const dst[], const int dstStride[])
         */
        sws_scale(video_sws_context,
                  (const uint8_t *const *) src_data, src_linesize,
                  0, src_video_height,
                  dst_data, dst_linesize);

        /* write scaled image to file */
        fwrite(dst_data[0], 1, dst_bufsize, out_file);
    }

    fprintf(stderr, "Scaling succeeded. Play the output file with the command:\n"
                    "ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
            av_get_pix_fmt_name(dst_avpixel_format), dst_video_width, dst_video_height, out_file_path);

    end:
    fclose(out_file);
    av_freep(&src_data[0]);
    av_freep(&dst_data[0]);
    sws_freeContext(video_sws_context);
    return ret < 0;
}

/***
 向右移动2位,就是除以4(2的2次方)；向右移动3位,就是除以8(2的3次方)；
 向左移动2位,就是乘以4(2的2次方)；向左移动3位,就是乘以8(2的3次方)；
 这个函数是把AV_PIX_FMT_YUV420P转化为AV_PIX_FMT_RGB24,
 而存储AV_PIX_FMT_YUV420P格式的数据是Y,U,V三个平面的数据,
 存储AV_PIX_FMT_RGB24格式的数据只有一个平面.
 */
int alexander_how_to_use_sws_scale3() {
    const char *srcFileName = "/root/视频/yuv/720_576_yuv420p_haoke.yuv";
    const char *dstFileName = "/root/视频/rgb/240_240_rgb24_haoke.rgb";

    src_video_width = 720, src_video_height = 576, src_avpixel_format = AV_PIX_FMT_YUV420P;
    dst_video_width = 240, dst_video_height = 240, dst_avpixel_format = AV_PIX_FMT_RGB24;//AV_PIX_FMT_RGB24
    uint8_t *src_data[4] = {NULL};
    //保存多少数据就申请多少空间
    src_data[0] = (uint8_t *) malloc(src_video_width * src_video_height);
    src_data[1] = (uint8_t *) malloc(src_video_width * src_video_height >> 2);//in_width * in_height / 4
    src_data[2] = (uint8_t *) malloc(src_video_width * src_video_height >> 2);
    src_data[3] = NULL;
    int src_linesize[4] = {src_video_width, src_video_width / 2, src_video_width / 2, 0};

    uint8_t *dst_data[4] = {NULL};
    dst_data[0] = (uint8_t *) malloc(dst_video_width * dst_video_height);
    dst_data[1] = (uint8_t *) malloc(dst_video_width * dst_video_height >> 2);
    dst_data[2] = (uint8_t *) malloc(dst_video_width * dst_video_height >> 2);
    dst_data[3] = NULL;
    int dst_linesize[4] = {dst_video_width, dst_video_width / 2, dst_video_width / 2, 0};

    int dst_bufsize;
    int ret;
    uint8_t *ptr_src_yuv_buf = nullptr;
    uint8_t *ptr_dst_yuv_buf = nullptr;
    const int32_t in_y_size = src_video_width * src_video_height;
    const int32_t out_y_size = dst_video_width * dst_video_height;
    const int read_size = in_y_size * 3 / 2;
    const int write_size = out_y_size * 3 / 2;
    ptr_src_yuv_buf = new uint8_t[read_size];
    ptr_dst_yuv_buf = new uint8_t[write_size];

    if (av_parse_video_size(&dst_video_width, &dst_video_height, "240x240") < 0) {
        return -1;
    }

    in_file = fopen(srcFileName, "rb+");
    if (!in_file) {
        fprintf(stderr, "Could not open destination file %s\n", srcFileName);
        exit(1);
    }

    out_file = fopen(dstFileName, "wb+");
    if (!out_file) {
        fprintf(stderr, "Could not open destination file %s\n", dstFileName);
        exit(1);
    }

    video_sws_context = sws_getContext(src_video_width, src_video_height, src_avpixel_format,
                                       dst_video_width, dst_video_height, dst_avpixel_format,
                                       SWS_BILINEAR,
                                       NULL, NULL, NULL);
    if (!video_sws_context) {
        fprintf(stderr,
                "Impossible to create scale context for the conversion "
                "fmt:%s s:%dx%d -> fmt:%s s:%dx%d\n",
                av_get_pix_fmt_name(src_avpixel_format), src_video_width, src_video_height,
                av_get_pix_fmt_name(dst_avpixel_format), dst_video_width, dst_video_height);
        ret = AVERROR(EINVAL);
        return -1;
    }

    ret = av_image_alloc(dst_data, dst_linesize,
                         dst_video_width, dst_video_height,
                         dst_avpixel_format, 1);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
        return -1;
    }
    dst_bufsize = ret;

    bool bExit = false;
    while (!bExit) {
        //
        if ((fread(ptr_src_yuv_buf, 1, read_size, in_file) < 0) || (feof(in_file))) {
            bExit = true;
            break;
        }

        memcpy(src_data[0], ptr_src_yuv_buf, in_y_size);
        memcpy(src_data[1], ptr_src_yuv_buf + in_y_size, in_y_size / 4);
        memcpy(src_data[2], ptr_src_yuv_buf + in_y_size * 5 / 4, in_y_size / 4);

        // ********* 主要的 function ******
        // ********* sws_scale ************
        sws_scale(video_sws_context, src_data, src_linesize,
                  0, src_video_height, dst_data, dst_linesize);

        /*memcpy(ptr_dst_yuv_buf, dst_data[0], out_y_size);
        memcpy(ptr_dst_yuv_buf + out_y_size, dst_data[1], out_y_size >> 2);
        memcpy(ptr_dst_yuv_buf + (out_y_size * 5 >> 2), dst_data[2], out_y_size >> 2);

        fwrite(ptr_dst_yuv_buf, 1, write_size, out_file);*/

        fwrite(dst_data[0], 1, dst_bufsize, out_file);
    }

    fclose(in_file);
    fclose(out_file);
    delete[] ptr_src_yuv_buf;
    ptr_src_yuv_buf = nullptr;
    delete[] ptr_dst_yuv_buf;
    ptr_dst_yuv_buf = nullptr;
    av_freep(&src_data[0]);
    av_freep(&dst_data[0]);
    sws_freeContext(video_sws_context);

    return 0;
}

/***
 使用这个函数.
 这个函数可以改变源视频的分辨率大小和像素格式.
 像素格式现在只支持
 (
 AV_PIX_FMT_GRAY8,
 AV_PIX_FMT_YUV420P,
 AV_PIX_FMT_YUV422P,
 AV_PIX_FMT_YUV444P,
 AV_PIX_FMT_YUYV422,
 AV_PIX_FMT_RGB24,
 AV_PIX_FMT_BGR24,
 AV_PIX_FMT_BGRA
 )
 这几种之间的互转.
 */
int alexander_how_to_use_sws_scale4() {
    in_file_path = "/root/视频/yuv/720_576_yuv420p.yuv";
    out_file_path = "/root/视频/rgb/240_240_bgra.rgb";

    if (open_files() < 0) {
        return -1;
    }

    src_video_width = 720, src_video_height = 576, src_avpixel_format = AV_PIX_FMT_YUV420P;
    dst_video_width = 240, dst_video_height = 240, dst_avpixel_format = AV_PIX_FMT_BGRA;//AV_PIX_FMT_RGB24
    //这个参数很重要
    int src_video_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(src_avpixel_format));
    printf("src_video_bpp = %d\n", src_video_bpp);
    src_video_area = src_video_width * src_video_height;
    dst_video_area = dst_video_width * dst_video_height;
    //中转缓存需要的空间大小算法
    video_out_buffer_size = src_video_area * src_video_bpp / 8;
    //中转缓存
    video_out_buffer2 = (uint8_t *) malloc(video_out_buffer_size);

    //申请空间
    ret = av_image_alloc(src_video_data, src_video_linesize,
                         src_video_width, src_video_height, src_avpixel_format, 1);
    if (ret < 0) {
        printf("Could not allocate source image\n");
        return -1;
    }
    ret = av_image_alloc(dst_video_data, dst_video_linesize,
                         dst_video_width, dst_video_height, dst_avpixel_format, 1);
    if (ret < 0) {
        printf("Could not allocate destination image\n");
        return -1;
    }

    /***
    //Init Method 1
    video_sws_context = sws_alloc_context();
    //Show AVOption
    av_opt_show2(video_sws_context, NULL, AV_OPT_FLAG_VIDEO_PARAM, 0);
    //Set Value
    av_opt_set_int(video_sws_context, "sws_flags", SWS_BICUBIC | SWS_PRINT_INFO, 0);
    av_opt_set_int(video_sws_context, "srcw", src_video_width, 0);
    av_opt_set_int(video_sws_context, "srch", src_video_height, 0);
    av_opt_set_int(video_sws_context, "src_format", src_avpixel_format, 0);
    //'0' for MPEG (Y:0-235);'1' for JPEG (Y:0-255)
    av_opt_set_int(video_sws_context, "src_range", 1, 0);
    av_opt_set_int(video_sws_context, "dstw", dst_video_width, 0);
    av_opt_set_int(video_sws_context, "dsth", dst_video_height, 0);
    av_opt_set_int(video_sws_context, "dst_format", dst_avpixel_format, 0);
    av_opt_set_int(video_sws_context, "dst_range", 1, 0);
    sws_init_context(video_sws_context, NULL, NULL);*/

    //Init Method 2
    video_sws_context = sws_getContext(src_video_width, src_video_height, src_avpixel_format,
                                       dst_video_width, dst_video_height, dst_avpixel_format,
                                       SWS_BICUBIC,
                                       NULL, NULL, NULL);
    //Colorspace
    ret = sws_setColorspaceDetails(video_sws_context,
                                   sws_getCoefficients(SWS_CS_ITU601), 0,
                                   sws_getCoefficients(SWS_CS_ITU709), 0,
                                   0, 1 << 16, 1 << 16);
    if (ret == -1) {
        printf("Colorspace not support.\n");
        return -1;
    }

    while (1) {
        if (fread(video_out_buffer2, 1, video_out_buffer_size, in_file) != video_out_buffer_size) {
            break;
        }

        /***
         下面这几种常用的像素格式存储数据的算法是需要掌握的

         数据的流向: video_out_buffer2 ---> src_video_data
         */
        switch (src_avpixel_format) {
            case AV_PIX_FMT_GRAY8: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area);
                break;
            }
            case AV_PIX_FMT_YUV420P: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area);                               //Y
                memcpy(src_video_data[1], video_out_buffer2 + src_video_area, src_video_area / 4);          //U
                memcpy(src_video_data[2], video_out_buffer2 + src_video_area * 5 / 4, src_video_area / 4);  //V
                break;
            }
            case AV_PIX_FMT_YUV422P: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area);                               //Y
                memcpy(src_video_data[1], video_out_buffer2 + src_video_area, src_video_area / 2);          //U
                memcpy(src_video_data[2], video_out_buffer2 + src_video_area * 3 / 2, src_video_area / 2);  //V
                break;
            }
            case AV_PIX_FMT_YUV444P: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area);                           //Y
                memcpy(src_video_data[1], video_out_buffer2 + src_video_area, src_video_area);          //U
                memcpy(src_video_data[2], video_out_buffer2 + src_video_area * 2, src_video_area);      //V
                break;
            }
            case AV_PIX_FMT_YUYV422: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area * 2);                  //Packed
                break;
            }
            case AV_PIX_FMT_RGB24: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area * 3);                  //Packed
                break;
            }
            case AV_PIX_FMT_BGR24: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area * 3);                  //Packed
                break;
            }
            case AV_PIX_FMT_BGRA: {
                memcpy(src_video_data[0], video_out_buffer2, src_video_area * 4);                  //Packed
                break;
            }
            default: {
                printf("Not Support Input Pixel Format.\n");
                break;
            }
        }

        /***
         数据的流向: src_video_data ---> dst_video_data
         */
        sws_scale(video_sws_context,
                  src_video_data, src_video_linesize,
                  0, src_video_height,
                  dst_video_data, dst_video_linesize);
        video_frame_count++;

        /***
         数据的流向: dst_video_data ---> out_file
         */
        switch (dst_avpixel_format) {
            case AV_PIX_FMT_GRAY8: {
                fwrite(dst_video_data[0], 1, dst_video_area, out_file);
                break;
            }
            case AV_PIX_FMT_YUV420P: {
                fwrite(dst_video_data[0], 1, dst_video_area, out_file);                     //Y
                fwrite(dst_video_data[1], 1, dst_video_area / 4, out_file);                 //U
                fwrite(dst_video_data[2], 1, dst_video_area / 4, out_file);                 //V
                break;
            }
            case AV_PIX_FMT_YUV422P: {
                fwrite(dst_video_data[0], 1, dst_video_area, out_file);                     //Y
                fwrite(dst_video_data[1], 1, dst_video_area / 2, out_file);                 //U
                fwrite(dst_video_data[2], 1, dst_video_area / 2, out_file);                 //V
                break;
            }
            case AV_PIX_FMT_YUV444P: {
                fwrite(dst_video_data[0], 1, dst_video_area, out_file);                     //Y
                fwrite(dst_video_data[1], 1, dst_video_area, out_file);                     //U
                fwrite(dst_video_data[2], 1, dst_video_area, out_file);                     //V
                break;
            }
            case AV_PIX_FMT_YUYV422: {
                fwrite(dst_video_data[0], 1, dst_video_area * 2, out_file);                 //Packed
                break;
            }
            case AV_PIX_FMT_RGB24: {
                fwrite(dst_video_data[0], 1, dst_video_area * 3, out_file);                 //Packed
                break;
            }
            case AV_PIX_FMT_BGR24: {
                fwrite(dst_video_data[0], 1, dst_video_area * 3, out_file);                 //Packed
                break;
            }
            case AV_PIX_FMT_BGRA: {
                fwrite(dst_video_data[0], 1, dst_video_area * 4, out_file);                 //Packed
                break;
            }
            default: {
                printf("Not Support Output Pixel Format.\n");
                break;
            }
        }
    }

    printf("Finish process frame %5d\n", video_frame_count);

    close();

    return 0;
}


//Use SDL
#define PLAYBACK_AUDIO_WITH_SDL2 1

//Buffer:
//|-----------|-------------|
//chunk-------pos---len-----|
static unsigned char *audio_chunk;
static unsigned int audio_len;
static unsigned char *audio_pos;

/***
 * Audio Callback
 * The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled
 * len: The length (in bytes) of the audio buffer
 */
void alexander_fill_audio2(void *udata, unsigned char *stream, int len) {
    //SDL 2.0
    SDL_memset(stream, 0, len);
    /*  Only  play  if  we  have  data  left  */
    if (audio_len == 0) {
        return;
    }
    len = (len > audio_len ? audio_len : len);   /*  Mix  as  much  data  as  possible  */

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

/***
 播放效果不好
 */
int alexander_playback_pcm() {
    SDL_AudioSpec sdl_audio_spec_;
    sdl_audio_spec = &sdl_audio_spec_;

    in_file_path = "/root/音乐/tdjm.mp3";

#if OUTPUT_PCM
    out_file = fopen("/root/音乐/pcm/01_xxx_24kHz_32kbps_Stereo.pcm", "wb+");
#endif

    init_av();

    if (avformat_open_and_find() < 0) {
        return -1;
    }

    find_stream_index();

    if (open_audio_avcodec_decoder() < 0) {
        return -1;
    }

    // Dump valid information onto standard error
    //av_dump_format(src_avformat_context, 0, in_file_path, false);

    //Out Audio Param
    //FIX:Some Codec's Context Information is missing
    int64_t in_ch_layout = av_get_default_channel_layout(audio_avcodec_context->channels);
    int64_t out_ch_layout = AV_CH_LAYOUT_STEREO;

    enum AVSampleFormat in_sample_fmt = audio_avcodec_context->sample_fmt;
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    printf("in_sample_fmt = %d\n", in_sample_fmt);
    printf("out_sample_fmt = %d\n", out_sample_fmt);

    int in_sample_rate = audio_avcodec_context->sample_rate;
    int out_sample_rate = 44100;

    //nb_samples: AAC-1024 MP3-1152
    int out_nb_samples = audio_avcodec_context->frame_size;
    printf("out_nb_samples = %d\n", out_nb_samples);
    int out_nb_channels = av_get_channel_layout_nb_channels(out_ch_layout);
    printf("out_nb_channels = %d\n", out_nb_channels);
    //Out Buffer Size
    audio_out_buffer_size = av_samples_get_buffer_size(NULL, out_nb_channels, out_nb_samples, out_sample_fmt, 1);
    printf("audio_out_buffer_size = %d\n", audio_out_buffer_size);

    //Swr
    audio_swr_context = swr_alloc();
    audio_swr_context = swr_alloc_set_opts(audio_swr_context,
                                           out_ch_layout, out_sample_fmt, out_sample_rate,
                                           in_ch_layout, in_sample_fmt, in_sample_rate,
                                           0, NULL);
    swr_init(audio_swr_context);

    avpacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(avpacket);
    avframe = av_frame_alloc();
    audio_out_buffer2 = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

#if PLAYBACK_AUDIO_WITH_SDL2
    //Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    //SDL_AudioSpec
    sdl_audio_spec->freq = out_sample_rate;
    sdl_audio_spec->format = AUDIO_S16SYS;
    sdl_audio_spec->channels = out_nb_channels;
    sdl_audio_spec->silence = 0;
    sdl_audio_spec->samples = out_nb_samples;
    sdl_audio_spec->userdata = audio_avcodec_context;
    sdl_audio_spec->callback = alexander_fill_audio2;
    if (SDL_OpenAudio(sdl_audio_spec, NULL) < 0) {
        printf("can't open audio.\n");
        return -1;
    }
    //Play
    SDL_PauseAudio(0);
#endif

    while (av_read_frame(src_avformat_context, avpacket) >= 0) {
        if (avpacket->stream_index == audio_stream_index) {
            ret = avcodec_decode_audio4(audio_avcodec_context, avframe, &got_picture_ptr, avpacket);
            if (ret < 0) {
                printf("ret = %d\n", ret);
                printf("Error in decoding audio frame.\n");
                break;
            }
            if (got_picture_ptr > 0) {
                //avframe为解码后的数据
                swr_convert(audio_swr_context, &audio_out_buffer2, MAX_AUDIO_FRAME_SIZE,
                            (const uint8_t **) avframe->data, avframe->nb_samples);
                //printf("index:%5d\t pts:%lld\t packet all_pkts_size:%d\n", index, avpacket->pts, avpacket->all_pkts_size);

#if OUTPUT_PCM
                //Write PCM
                fwrite(audio_out_buffer2, 1, audio_out_buffer_size, out_file);
#endif
                audio_frame_count++;
            }

#if PLAYBACK_AUDIO_WITH_SDL2
            //Wait until finish
            while (audio_len > 0) {
                SDL_Delay(1);
            }

            //Set audio buffer (PCM data)
            audio_chunk = (uint8_t *) audio_out_buffer2;
            //Audio buffer length
            audio_len = audio_out_buffer_size;
            audio_pos = audio_chunk;
#endif
        }
        av_free_packet(avpacket);
    }//while end

    close();

    return 0;
}

/*int alexander_how_to_use_swr_convert() {
    AVFrame *frame, *frame_out;
    AVPacket pkt_in, pkt_out;
    int ret;

    av_register_all();
    avfilter_register_all();

    //input
    if (avformat_open_input(&src_avformat_context, in_file_path, NULL, NULL) < 0) {
        printf("can not open input file context");
        return -1;;
    }

    if (avformat_find_stream_info(src_avformat_context, NULL) < 0) {
        printf("can not find input stream info!\n");
        return -1;;
    }

    //output
    avformat_alloc_output_context2(&dst_avformat_context, NULL, NULL, out_file_path);
    if (!dst_avformat_context) {
        printf("can not alloc output context!\n");
        return -1;;
    }

    //open decoder & new out stream & open encoder
    for (int i = 0; i < src_avformat_context->nb_streams; i++) {
        if (src_avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            //open decoder
            if (0 > avcodec_open2(src_avformat_context->streams[i]->codec,
                                  avcodec_find_decoder(src_avformat_context->streams[i]->codec->codec_id), NULL)) {
                printf("can not find or open decoder!\n");
                return -1;;
            }
            audio_stream_index = i;

            //new stream
            out_stream = avformat_new_stream(dst_avformat_context, NULL);
            if (!out_stream) {
                printf("can not new stream for output!\n");
                return -1;;
            }
            //set codec context param
            //use default audio encoder
            out_stream->codec->codec = avcodec_find_encoder(dst_avformat_context->oformat->audio_codec);
            //use the input audio encoder
            //
            out_stream->codec->codec = avcodec_find_encoder(src_avformat_context->streams[i]->codec->codec_id);
            out_stream->codec->sample_rate = src_avformat_context->streams[i]->codec->sample_rate;
            out_stream->codec->channel_layout = src_avformat_context->streams[i]->codec->channel_layout;
            out_stream->codec->channels = av_get_channel_layout_nb_channels(out_stream->codec->channel_layout);

            // take first format from list of supported formats
            out_stream->codec->sample_fmt = out_stream->codec->codec->sample_fmts[0];
            AVRational time_base = {1, out_stream->codec->sample_rate};
            out_stream->codec->time_base = time_base;

            //open encoder
            if (!out_stream->codec->codec) {
                printf("can not find the encoder!\n");
                return -1;;
            }
            if ((avcodec_open2(out_stream->codec, out_stream->codec->codec, NULL)) < 0) {
                printf("can not open the encoder\n");
                return -1;;
            }
            if (dst_avformat_context->oformat->flags & AVFMT_GLOBALHEADER)
                out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            break;
        }
    }
    //dump input info
    av_dump_format(src_avformat_context, 0, in_file_path, 0);
    //dump output info
    av_dump_format(dst_avformat_context, 0, out_file_path, 1);
    if (-1 == audio_stream_index) {
        printf("found no audio stream in input file!\n");
        return -1;;
    }
    if (!(dst_avformat_context->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&dst_avformat_context->pb, out_file_path, AVIO_FLAG_WRITE) < 0) {
            printf("can not open output file handle!\n");
            return -1;;
        }
    }

    if (avformat_write_header(dst_avformat_context, NULL) < 0) {
        printf("can not write the header of the output file!\n");
        return -1;;
    }
    //
    initSwr();
    int got_frame;
    int i = 0;

    for (int i = 0;; i++) {
        pkt_in.data = NULL;
        pkt_in.all_pkts_size = 0;
        got_frame = -1;
        got_picture_ptr = -1;
        if (av_read_frame(src_avformat_context, &pkt_in) < 0) {
            break;
        }

        if (pkt_in.stream_index != audio_stream_index) {
            continue;
        }

        frame = av_frame_alloc();

        if ((ret = avcodec_decode_audio4(src_avformat_context->streams[audio_stream_index]->codec, frame, &got_frame,
                                         &pkt_in)) < 0) {
            av_frame_free(&frame);
            printf("can not decoder a frame");
            break;
        }

        av_free_packet(&pkt_in);

        if (got_frame) {
            frame->pts = av_frame_get_best_effort_timestamp(frame);
            frame_out = av_frame_alloc();
            if (0 != TransSample(frame, frame_out)) {
                printf("can not swr the audio data!\n");
                break;
            }

            av_init_packet(&pkt_out);
            int ret = avcodec_encode_audio2(dst_avformat_context->streams[0]->codec, &pkt_out, frame_out,
                                            &got_picture_ptr);

            av_free(frame_out->data[0]);
            av_frame_free(&frame_out);

            if (got_picture_ptr) {
                pkt_out.stream_index = out_stream->index;
                pkt_out.dts = av_rescale_q_rnd(pkt_out.dts,
                                               dst_avformat_context->streams[out_stream->index]->codec->time_base,
                                               dst_avformat_context->streams[out_stream->index]->time_base,
                                               (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                pkt_out.pts = av_rescale_q_rnd(pkt_out.pts,
                                               dst_avformat_context->streams[out_stream->index]->codec->time_base,
                                               dst_avformat_context->streams[out_stream->index]->time_base,
                                               (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                pkt_out.duration = av_rescale_q(pkt_out.duration,
                                                dst_avformat_context->streams[out_stream->index]->codec->time_base,
                                                dst_avformat_context->streams[out_stream->index]->time_base);

                av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");

                *//* mux encoded frame *//*
                ret = av_interleaved_write_frame(dst_avformat_context, &pkt_out);
                av_free_packet(&pkt_out);

                if (ret < 0) {
                    printf("write a frame failed!\n");
                    break;
                }

                printf("success write a frame:index %d\n", audio_frame_count++);
            }
        }
    }

    ret = flush_encoder2(dst_avformat_context, out_stream->index);
    if (ret < 0) {
        printf("Flushing encoder failed");
        return -1;
    }

    //write file trailer
    av_write_trailer(dst_avformat_context);

    //clean
    avcodec_close(out_stream->codec);
    avcodec_close(src_avformat_context->streams[audio_stream_index]->codec);

    end:
    avformat_close_input(&src_avformat_context);
    if (dst_avformat_context && !(dst_avformat_context->oformat->flags & AVFMT_NOFILE)) {
        avio_close(dst_avformat_context->pb);
    }
    avformat_free_context(dst_avformat_context);
    getchar();
    return 0;
}*/

int alexander_save_yuv_to_char() {
    int i = 0, j = 0, k = 0;
    int Zoom_Width = 0, Zoom_Height = 0, Zoom_pix_fmt = AV_PIX_FMT_NONE;
    if (video_avcodec_context->codec_type == AVMEDIA_TYPE_VIDEO) {
        int new_videosize = avpacket->size;
        int video_decode_size = avpicture_get_size(video_avcodec_context->pix_fmt, Zoom_Width, Zoom_Height);
        //最大分配的空间，能满足yuv的各种格式
        uint8_t *video_decode_buffer = (uint8_t *) calloc(1, video_decode_size * 3 * sizeof(char));

        // Decode video frame
        avcodec_decode_video2(video_avcodec_context, avframe, &got_picture_ptr, avpacket);
        if (got_picture_ptr) {
            //如果是yuv420p的
            if (video_avcodec_context->pix_fmt == AV_PIX_FMT_YUV420P) {
                for (i = 0; i < video_avcodec_context->height; i++) {
                    memcpy(video_decode_buffer + video_avcodec_context->width * i,
                           avframe->data[0] + avframe->linesize[0] * i,
                           video_avcodec_context->width);
                }
                for (j = 0; j < video_avcodec_context->height / 2; j++) {
                    memcpy(video_decode_buffer + video_avcodec_context->width * i +
                           video_avcodec_context->width / 2 * j,
                           avframe->data[1] + avframe->linesize[1] * j,
                           video_avcodec_context->width / 2);
                }
                for (k = 0; k < video_avcodec_context->height / 2; k++) {
                    memcpy(video_decode_buffer + video_avcodec_context->width * i +
                           video_avcodec_context->width / 2 * j +
                           video_avcodec_context->width / 2 * k,
                           avframe->data[2] + avframe->linesize[2] * k,
                           video_avcodec_context->width / 2);
                }
                //如果是yuv422p的
            } else if (video_avcodec_context->pix_fmt == AV_PIX_FMT_YUV422P) {
                for (i = 0; i < video_avcodec_context->height; i++) {
                    memcpy(video_decode_buffer + video_avcodec_context->width * i,
                           avframe->data[0] + avframe->linesize[0] * i,
                           video_avcodec_context->width);
                }
                for (j = 0; j < video_avcodec_context->height; j++) {
                    memcpy(video_decode_buffer + video_avcodec_context->width * i +
                           video_avcodec_context->width / 2 * j,
                           avframe->data[1] + avframe->linesize[1] * j,
                           video_avcodec_context->width / 2);
                }
                for (k = 0; k < video_avcodec_context->height; k++) {
                    memcpy(video_decode_buffer + video_avcodec_context->width * i +
                           video_avcodec_context->width / 2 * j +
                           video_avcodec_context->width / 2 * k,
                           avframe->data[2] + avframe->linesize[2] * k,
                           video_avcodec_context->width / 2);
                }
            } else {
                //可扩展
            }
            video_decode_size = avpicture_get_size(video_avcodec_context->pix_fmt,
                                                   video_avcodec_context->width,
                                                   video_avcodec_context->height);
            new_videosize = video_decode_size;

            //缩放或格式转换
            if (video_avcodec_context->width != Zoom_Width
                || video_avcodec_context->height != Zoom_Height
                || video_avcodec_context->pix_fmt != Zoom_pix_fmt) {
                /*new_videosize = VideoScaleYuvZoom(Is_flip, video_avcodec_context->width,
                                                  video_avcodec_context->height,
                                                  (int) video_avcodec_context->pix_fmt,
                                                  Zoom_Width, Zoom_Height, Zoom_pix_fmt,
                                                  video_decode_buffer);*/
            }
            /*//这里可以取出数据
            frame_info->stream_idx = pstream_info->stream_idx;
            //转化成毫秒
            //frame_info->pts = avframe->pkt_pts * 1000 * av_q2d(pstream_info->stream->time_base);
            frame_info->pts = avframe->pkt_pts;
            frame_info->timebase_den = pstream_info->stream->time_base.den;
            frame_info->timebase_num = pstream_info->stream->time_base.num;
            frame_info->bufsize = new_videosize;
            memcpy(frame_info->buf, video_decode_buffer, new_videosize);*/
        } else {
            //缓存
            /*frame_info->stream_idx = pstream_info->stream_idx;
            frame_info->pts = 0;
            frame_info->timebase_den = 0;
            frame_info->timebase_num = 0;
            frame_info->bufsize = 0;
            memset(frame_info->buf, 0, MAX_FRAME_SIZE);*/
        }
        if (video_decode_buffer) {
            free(video_decode_buffer);
            video_decode_buffer = NULL;
        }
        video_decode_size = 0;
    }
}

int yuv422p() {
//    AVFormatContext *src_avformat_context;
//    int i, video_stream_index;
//    AVCodecContext *video_avcodec_context;
//    AVCodec *pCodec;
//    AVFrame *pFrame;
//    AVFrame *pFrameYUV;
//    AVPacket packet;
//    int frameFinished;
//    int numBytes;
//    uint8_t *video_out_buffer2;
//    SDL_Rect rect;
//    static int sws_flags = SWS_BICUBIC;
//    struct SwsContext *video_sws_context;
//    AVPicture pict;
//    //  argc = 2;
//    char argv[100] = "d:\\temp\\VIDEO720576.264";
//    // argv[1] = "d:\\temp\\ff.mpg";
//
//    // /*注册所有可用的格式和编解码器*/
//    av_register_all();
//
//    // Open video file
//    /*以输入方式打开一个媒体文件,也即源文件,codecs并没有打开,只读取了文件的头信息*/
//    if (avformat_open_input(&src_avformat_context, argv, NULL, NULL) != 0)
//        return -1; // Couldn't open file
//
//    // Retrieve stream information
///*通过读取媒体文件的中的包来获取媒体文件中的流信息,对于没有头信息的文件如(mpeg)是非常有用的,
//// 该函数通常重算类似mpeg-2帧模式的真实帧率,该函数并未改变逻辑文件的position.
//*/
//    if (avformat_find_stream_info(src_avformat_context, NULL) < 0)
//        return -1; // Couldn't find stream information
//
//    // Find the first video stream
//    video_stream_index = -1;
//    printf("%d\n", src_avformat_context->nb_streams);
//    getchar();
//    for (i = 0; i < src_avformat_context->nb_streams; i++)
//        if (src_avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_index < 0) {
//            video_stream_index = i;
//            break;
//        }
//    if (video_stream_index == -1)
//        return -1; // Didn't find a video stream
//
//    // Get a pointer to the codec context for the video stream
//    video_avcodec_context = src_avformat_context->streams[video_stream_index]->codec;
//
//    // Find the decoder for the video stream
//    pCodec = avcodec_find_decoder(video_avcodec_context->codec_id);
//    /*通过code ID查找一个已经注册的音视频编码器,查找编码器之前,必须先调用av_register_all注册所有支持的编码器
//    音视频编码器保存在一个链表中,查找过程中,函数从头到尾遍历链表,通过比较编码器的ID来查找
//    */
//    if (pCodec == NULL) {
//        fprintf(stderr, "Unsupported codec!\n");
//        return -1; // Codec not found
//    }
//    // Open codec
////使用给定的AVCodec初始化AVCodecContext
//
//
//    if (avcodec_open2(video_avcodec_context, pCodec, NULL) < 0)
//        return -1; // Could not open codec
//
//
//    //printf("name %s\n",pCodec->name);
//    //getchar();
//
//
//    // Allocate video frame
//    pFrame = av_frame_alloc();
//
//    // Allocate an AVFrame structure
//    pFrameYUV = av_frame_alloc();
//    if (pFrameYUV == NULL)
//        return -1;
//
//    // Determine required video_out_buffer2 all_pkts_size and allocate video_out_buffer2
//    numBytes = avpicture_get_size(AV_PIX_FMT_YUV422P, video_avcodec_context->width,
//                                  video_avcodec_context->height);
//    video_out_buffer2 = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
//
//    // Assign appropriate parts of video_out_buffer2 to image planes in pFrameRGB
//    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
//    // of AVPicture
//    avpicture_fill((AVPicture *) pFrameYUV, video_out_buffer2, AV_PIX_FMT_YUV422P,
//                   video_avcodec_context->width, video_avcodec_context->height);
//
//    // Read frames and save first five frames to disk
//    i = 0;
//    ////////////////////////////////////////////////////////////////
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        fprintf(stderr, "can not initialize SDL:%s\n", SDL_GetError());
//        exit(1);
//    }
//    atexit(SDL_Quit);
//    SDL_Surface *screen;
//    screen = SDL_SetVideoMode(720, 576, 24, SDL_SWSURFACE | SDL_ANYFORMAT);
//    if (screen == NULL) {
//        exit(2);
//    }
//    SDL_Overlay *image;
//
//
//    image = SDL_CreateYUVOverlay(video_avcodec_context->width,
//                                 video_avcodec_context->height,
//                                 SDL_YUY2_OVERLAY,
//                                 screen);
//
//
//    if (image == NULL) {
//        //fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
//        exit(1);
//    }
//    //////////////////////////////////////////////////////////////////
//    while (av_read_frame(src_avformat_context, &packet) >= 0) {
//        // Is this a packet from the video stream?
//        if (packet.stream_index == video_stream_index) {
//            // Decode video frame
//            avcodec_decode_video(video_avcodec_context, pFrame, &frameFinished,
//                                 packet.data, packet.all_pkts_size);
//
//            // Did we get a video frame?
//            if (frameFinished) {
//                // Convert the image from its native format to RGB
//                SDL_LockYUVOverlay(image);
//
//
//// pFrameYUV->data[0] = image->pixels[0];
//// pFrameYUV->data[1] = image->pixels[2];
//// pFrameYUV->data[2] = image->pixels[1];
////
//// pFrameYUV->linesize[0] = image->pitches[0];
//// pFrameYUV->linesize[1] = image->pitches[2];
//// pFrameYUV->linesize[2] = image->pitches[1];
//
//
//                video_sws_context = sws_getContext(video_avcodec_context->width,
//                                                   video_avcodec_context->height,
//                                                   video_avcodec_context->pix_fmt,
//                                                   video_avcodec_context->width,
//                                                   video_avcodec_context->height,
//                                                   PIX_FMT_YUV422,
//                                                   sws_flags, NULL, NULL, NULL);
//                sws_scale(video_sws_context, pFrame->data, pFrame->linesize, 0, video_avcodec_context->height,
//                          pFrameYUV->data,
//                          pFrameYUV->linesize);
//                sws_freeContext(video_sws_context);
//
//                memcpy(image->pixels[0], pFrameYUV->data[0], 720 * 576 * 2);//拷贝数据yuv422
//
//                SDL_UnlockYUVOverlay(image);
//
//                rect.x = 0;
//                rect.y = 0;
//                rect.w = video_avcodec_context->width;
//                rect.h = video_avcodec_context->height;
//                SDL_DisplayYUVOverlay(image, &rect);
//            }
//        }
//
//        // Free the packet that was allocated by av_read_frame
//        av_free_packet(&packet);
//    }
//
//    // Free the RGB image
//    av_free(video_out_buffer2);
//    av_free(pFrameYUV);
//
//    // Free the YUV frame
//    av_free(pFrame);
//
//    // Close the codec
//    avcodec_close(video_avcodec_context);
}

/***
FIX: H.264 in some container format (FLV, MP4, MKV etc.) need
"h264_mp4toannexb" bitstream filter (BSF)
  *Add SPS,PPS in front of IDR frame
  *Add start code ("0,0,0,1") in front of NALU
H.264 in some container (MPEG2TS) don't need this BSF.
*/
int alexander_audio_or_video_demuxer() {
    //'1': Use H.264 Bitstream Filter
#define USE_H264BSF 0
    AVOutputFormat *ofmt_a = NULL, *ofmt_v = NULL;
    //（Input AVFormatContext and Output AVFormatContext）
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx_a = NULL, *ofmt_ctx_v = NULL;
    AVPacket pkt;
    int ret, i;
    int videoindex = -1, audioindex = -1;
    int frame_index = 0;

    const char *in_filename = "/root/视频/tomcat_video/Silent_Movie_Short_321_AC4_h264_MP4_25fps.mp4";//Input file URL
    //char *in_filename  = "cuc_ieschool.mkv";
    const char *out_filename_v = "/root/视频/tomcat_video/Silent_Movie_Short_321_AC4_h264_MP4_25fps.h264";//Output file URL
    //char *out_filename_a = "cuc_ieschool.mp3";
    const char *out_filename_a = "/root/视频/tomcat_video/Silent_Movie_Short_321_AC4_h264_MP4_25fps.aac";

    av_register_all();
    //Input
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        printf("Could not open input file.");
        goto end;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        printf("Failed to retrieve input stream information");
        goto end;
    }

    //Output
    avformat_alloc_output_context2(&ofmt_ctx_v, NULL, NULL, out_filename_v);
    if (!ofmt_ctx_v) {
        printf("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    ofmt_v = ofmt_ctx_v->oformat;

    avformat_alloc_output_context2(&ofmt_ctx_a, NULL, NULL, out_filename_a);
    if (!ofmt_ctx_a) {
        printf("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    ofmt_a = ofmt_ctx_a->oformat;

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        //Create output AVStream according to input AVStream
        AVFormatContext *ofmt_ctx;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVStream *out_stream = NULL;

        if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            out_stream = avformat_new_stream(ofmt_ctx_v, in_stream->codec->codec);
            ofmt_ctx = ofmt_ctx_v;
        } else if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioindex = i;
            out_stream = avformat_new_stream(ofmt_ctx_a, in_stream->codec->codec);
            ofmt_ctx = ofmt_ctx_a;
        } else {
            break;
        }

        if (!out_stream) {
            printf("Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }
        //Copy the settings of AVCodecContext
        if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) {
            printf("Failed to copy context from input to output stream codec context\n");
            goto end;
        }
        out_stream->codec->codec_tag = 0;

        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    //Dump Format------------------
    printf("\n==============Input Video=============\n");
    av_dump_format(ifmt_ctx, 0, in_filename, 0);
    printf("\n==============Output Video============\n");
    av_dump_format(ofmt_ctx_v, 0, out_filename_v, 1);
    printf("\n==============Output Audio============\n");
    av_dump_format(ofmt_ctx_a, 0, out_filename_a, 1);
    printf("\n======================================\n");
    //Open output file
    if (!(ofmt_v->flags & AVFMT_NOFILE)) {
        if (avio_open(&ofmt_ctx_v->pb, out_filename_v, AVIO_FLAG_WRITE) < 0) {
            printf("Could not open output file '%s'", out_filename_v);
            goto end;
        }
    }

    if (!(ofmt_a->flags & AVFMT_NOFILE)) {
        if (avio_open(&ofmt_ctx_a->pb, out_filename_a, AVIO_FLAG_WRITE) < 0) {
            printf("Could not open output file '%s'", out_filename_a);
            goto end;
        }
    }

    //Write file header
    if (avformat_write_header(ofmt_ctx_v, NULL) < 0) {
        printf("Error occurred when opening video output file\n");
        goto end;
    }
    if (avformat_write_header(ofmt_ctx_a, NULL) < 0) {
        printf("Error occurred when opening audio output file\n");
        goto end;
    }

#if USE_H264BSF
    AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
#endif

    while (1) {
        AVFormatContext *ofmt_ctx;
        AVStream *in_stream, *out_stream;
        //Get an AVPacket
        if (av_read_frame(ifmt_ctx, &pkt) < 0)
            break;
        in_stream = ifmt_ctx->streams[pkt.stream_index];


        if (pkt.stream_index == videoindex) {
            out_stream = ofmt_ctx_v->streams[0];
            ofmt_ctx = ofmt_ctx_v;
            printf("Write Video Packet. size:%d\tpts:%lld\n", pkt.size, pkt.pts);
#if USE_H264BSF
            av_bitstream_filter_filter(h264bsfc, in_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif
        } else if (pkt.stream_index == audioindex) {
            out_stream = ofmt_ctx_a->streams[0];
            ofmt_ctx = ofmt_ctx_a;
            printf("Write Audio Packet. size:%d\tpts:%lld\n", pkt.size, pkt.pts);
        } else {
            continue;
        }


        //Convert PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        pkt.stream_index = 0;
        //Write
        if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) {
            printf("Error muxing packet\n");
            break;
        }
        //printf("Write %8d frames to output file\n",frame_index);
        av_free_packet(&pkt);
        frame_index++;
    }

#if USE_H264BSF
    av_bitstream_filter_close(h264bsfc);
#endif

    //Write file trailer
    av_write_trailer(ofmt_ctx_a);
    av_write_trailer(ofmt_ctx_v);
    end:
    avformat_close_input(&ifmt_ctx);
    /* close output */
    if (ofmt_ctx_a && !(ofmt_a->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx_a->pb);

    if (ofmt_ctx_v && !(ofmt_v->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx_v->pb);

    avformat_free_context(ofmt_ctx_a);
    avformat_free_context(ofmt_ctx_v);


    if (ret < 0 && ret != AVERROR_EOF) {
        printf("Error occurred.\n");
        return -1;
    }
    return 0;
}

int alexander_get_y_frame_from_yuv420p() {
    //读取文件test_yuv420p_320x180.yuv
    FILE *fp_yuv = fopen("test_yuv420p_320x180.yuv", "rb");
    //写入文件frame.yuv
    FILE *fp_frame = fopen("frame_320x180_out.yuv", "wb");
    //开辟内存读取test_yuv420p_320x180.yuv文件的第一帧的亮度数据（Y）
    char *buffer_y = (char *) malloc(sizeof(char) * 320 * 180);
    char *buffer_u = (char *) malloc(sizeof(char) * 320 * 180 / 4);
    char *buffer_v = (char *) malloc(sizeof(char) * 320 * 180 / 4);

    //读取函数，将test_yuv420p_320x180.yuv的第一帧存入buff指向的内存
    for (int i = 0; i < 30; i++) {
        fread(buffer_y, 320 * 180, 1, fp_yuv);
        fread(buffer_u, 320 * 180 / 4, 1, fp_yuv);
        fread(buffer_v, 320 * 180 / 4, 1, fp_yuv);
        //不要色度的话,就是把u,v方向上的值设为128就行了
        memset(buffer_u, 128, 320 * 180 / 4);
        memset(buffer_v, 128, 320 * 180 / 4);

        fwrite(buffer_y, 320 * 180, 1, fp_frame);
        fwrite(buffer_u, 320 * 180 / 4, 1, fp_frame);
        fwrite(buffer_v, 320 * 180 / 4, 1, fp_frame);
    }
    //fread(buff,320*180,1,fp_yuv);
    //将buff指向的内存写入frame.yuv

    //fprintf演示
    //FILE* fp_demo=fopen("demo.txt","wb");
    //char hello[20]="GuangDianGong";
    //fprintf(fp_demo,"Hello World,%s",hello);
    //fclose(fp_demo);

    free(buffer_y);
    free(buffer_u);
    free(buffer_v);
    fclose(fp_yuv);
    fclose(fp_frame);

    return 0;
}

void test(char) {

}






//////////////////////////////////////////////////////////////////

const char *src_filename = "/root/视频/haoke.avi";
const char *video_out_filename = "/root/视频/haoke.yuv";
const char *audio_out_filename = "/root/音乐/haoke.aac";

FILE *pOutputAudio = NULL;
FILE *pOutputVideo = NULL;


AVStream *avstream = NULL;
AVCodec *avcodec = NULL;
AVCodecContext *avcodec_context = NULL;

//分别定义音视频流文件
AVStream *video_avstream = NULL, *audio_avstream = NULL;

//enum AVPixelFormat avpixel_format;

unsigned char *video_dst_data[4];
int video_dst_linesize[4];
int video_dst_bufsize;

int decode_packet(int *got_frame) {
    int ret = 0;
    *got_frame = 0;
    if (avpacket->stream_index == video_avstream->index) {
        ret = avcodec_decode_video2(video_avcodec_context, avframe, got_frame, avpacket);
        if (ret < 0) {
            printf("%s\n", "");
            return -1;
        }
        if (*got_frame) {
            printf("%s\n", "");
            av_image_copy(video_dst_data, video_dst_linesize,
                          (const unsigned char **) avframe->data, avframe->linesize,
                          dst_avpixel_format, src_video_width, src_video_height);
            fwrite(video_dst_data[0], 1, video_dst_bufsize, pOutputVideo);
        }
    } else if (avpacket->stream_index == audio_avstream->index) {
        ret = avcodec_decode_audio4(audio_avcodec_context, avframe, got_frame, avpacket);
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
                    avframe->nb_samples * av_get_bytes_per_sample((enum AVSampleFormat) avframe->format);
            fwrite(avframe->extended_data[0], 1, unpadded_linesize, pOutputAudio);
        }
    }

    return FFMIN(ret, avpacket->size);
}

static int open_codec_context(enum AVMediaType type) {
    int stream_index = 0;
    /*
    根据type找到对应的音视频流或者字母流，返回的是对应文件流的index
    */
    stream_index = av_find_best_stream(src_avformat_context, type, -1, -1, NULL, 0);
    if (stream_index < 0) {
        printf("%s\n", "");
        return -1;
    } else {
        avstream = src_avformat_context->streams[stream_index];
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
    if (avformat_open_input(&src_avformat_context, src_filename, NULL, NULL) < 0) {
        printf("%s\n", "");
        return -1;
    }
    /*
    读取一部分视音频数据并且获得一些相关的信息,主要给每个媒体流的AVStream结构体赋值
    */
    if (avformat_find_stream_info(src_avformat_context, NULL) < 0) {
        printf("%s\n", "");
        return -1;
    }

    //=====打开音频和视频流=====
    if (open_codec_context(AVMEDIA_TYPE_VIDEO) >= 0) {
        video_avstream = avstream;
        video_avcodec_context = avcodec_context;

        src_video_width = video_avcodec_context->width;
        src_video_height = video_avcodec_context->height;
        dst_avpixel_format = video_avcodec_context->pix_fmt;

        /*
        分配像素的存储空间
        该函数的四个参数分别表示AVFrame结构中的缓存指针、各个颜色分量的宽度、图像分辨率（宽、高）、像素格式和内存对其的大小。
        该函数会返回分配的内存的大小。其实就是对video_dst_data,video_dst_linesize内存进行分配
        */
        ret = av_image_alloc(video_dst_data, video_dst_linesize,
                             src_video_width, src_video_height, dst_avpixel_format, 1);
        if (ret < 0) {
            printf("%s\n", "");
            return -1;
        } else {
            video_dst_bufsize = ret;
        }
        if (!(pOutputVideo = fopen(video_out_filename, "wb"))) {
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
        if (!(pOutputAudio = fopen(audio_out_filename, "wb"))) {
            printf("%s\n", "");
            return -1;
        }
    }

    //=====打印输入输出格式信息====
    av_dump_format(src_avformat_context, 0, src_filename, 0);

    //=====读取和处理音视频文件=====
    avframe = av_frame_alloc();
    if (!avframe) {
        printf("%s\n", "");
        return -1;
    }

    av_init_packet(avpacket);
    avpacket->data = NULL;
    avpacket->size = 0;

    while (av_read_frame(src_avformat_context, avpacket) >= 0) {
        do {
            ret = decode_packet(&got_frame);
            printf("decode packet all_pkts_size: %d\n", ret);
            avpacket->data += ret;
            avpacket->size -= ret;
        } while (avpacket->size > 0);
    }

    /*
    将编码器缓存中的数据解码完
    */
    avpacket->data = NULL;
    avpacket->size = 0;
    do {
        ret = decode_packet(&got_frame);
        avpacket->data += ret;
        avpacket->size -= ret;
    } while (got_frame);

    //=====释放资源=====
    avcodec_close(video_avcodec_context);
    avcodec_close(audio_avcodec_context);
    avformat_close_input(&src_avformat_context);
    av_frame_free(&avframe);
    av_free(video_dst_data[0]);

    fclose(pOutputAudio);
    fclose(pOutputVideo);

    return 0;
}

#endif //MYSTUDY_FFMPEG_LEIXIAOHUA