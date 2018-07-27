//
// Created by root on 18-7-24.
//

#ifndef MYSTUDY_FFMPEG_LEIXIAOHUA
#define MYSTUDY_FFMPEG_LEIXIAOHUA

#include "../include/MyHead.h"

///////////////////////////公共变量///////////////////////////

enum AVCodecID avcodec_id = AV_CODEC_ID_NONE;
//格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
AVFormatContext *avformat_context = NULL;

//AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
AVPacket *avpacket = NULL;
AVFrame *avframe = NULL;
//用于解析输入的数据流并把它分成一帧一帧的压缩编码数据。
//比较形象的说法就是把长长的一段连续的数据“切割”成一段段的数据。
//他的核心函数是av_parser_parse2()。
AVCodecParserContext *avcodec_parser_context = NULL;

int video_stream_index = -1;
int audio_stream_index = -1;
//用于线程
int thread_pause_flag = 0;
int thread_exit_flag = 0;

//char *in_file_path = "/root/视频/haoke.avi";
char *in_file_path = "/root/视频/yuv/240_240_yuv420p.yuv";
char *out_file_path = "/root/视频/yuv/720_576_haoke.yuv";

FILE *in_file = NULL;
FILE *out_file = NULL;

int ret = -1, got_picture_ptr = -1;

///////////////////////////video///////////////////////////

//SwsContext作为sws_scale的第一个参数,记录数据要转换的格式、大小及转换方式(用于图像像素的处理)
struct SwsContext *video_sws_context = NULL;
//源视频宽高
int src_video_width = 0, src_video_height = 0, src_video_area = 0;
int playback_window_w = 0, playback_window_h = 0;
AVCodecContext *video_avcodec_context = NULL;
AVCodecParameters *video_avcodec_parameters = NULL;
//Decoder Encoder
AVCodec *video_avcodec_decoder = NULL, *video_avcodec_encoder = NULL;
//src_avframe保存原始帧 dst_avframe转换成yuv后的帧保存在此处
AVFrame *src_video_avframe = NULL, *dst_video_avframe = NULL;
//先自己申请空间,然后为初始化video_dst_avframe而服务
unsigned char *video_out_buffer = NULL;
uint8_t *video_out_buffer2 = NULL;
int video_out_buffer_size = 0, video_frame_count = 0;
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
int audio_out_buffer_size = 0, audio_frame_count = 0;
//采样格式
enum AVSampleFormat src_avsample_format = AV_SAMPLE_FMT_NONE;
enum AVSampleFormat dst_avsample_format = AV_SAMPLE_FMT_S16;

///////////////////////////SDL2///////////////////////////

struct SDL_Rect sdl_rect;
SDL_Window *sdl_window = NULL;
SDL_Renderer *sdl_renderer = NULL;
SDL_Texture *sdl_texture = NULL;
SDL_Thread *sdl_thread = NULL;
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
    avformat_context = avformat_alloc_context();
}

/***
 先要得到src_video_width和src_video_height的值
 */
void init_sdl() {
    if (src_video_width == 0 || src_video_height == 0) {
        printf("%s", "没有设置src_video_width和src_video_height的值");
        return;
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
        return;
    }

    //SDL 2.0 Support for multiple windows
    sdl_window = SDL_CreateWindow(in_file_path,
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  src_video_width, src_video_height,
                                  SDL_WINDOW_OPENGL);
    if (sdl_window == NULL) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return;
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
    if (avformat_context != NULL) {
        avformat_close_input(&avformat_context);
        avformat_context = NULL;
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

int avformat_open_and_find() {
    if (avformat_open_input(&avformat_context, in_file_path, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(avformat_context, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }
}

void find_stream_index() {
    int i;
    int nb_streams = avformat_context->nb_streams;
    for (i = 0; i < nb_streams; i++) {
        if (avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
        } else if (avformat_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
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
        video_avcodec_context = avformat_context->streams[video_stream_index]->codec;
        video_avcodec_decoder = avcodec_find_decoder(video_avcodec_context->codec_id);
    }
    if (video_avcodec_decoder == NULL) {
        printf("Codec not found.\n");
        return -1;
    }
    if (avcodec_open2(video_avcodec_context, video_avcodec_decoder, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }
}

int open_audio_avcodec_decoder() {
    if (audio_stream_index != -1) {
        audio_avcodec_context = avformat_context->streams[audio_stream_index]->codec;
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
        printf("%s\n", "");
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
    out_file = fopen(out_file_path, "wb+");
    if (out_file == NULL) {
        printf("%s\n", "输出文件打开有问题");
        return -1;
    }
#endif
    in_file = fopen(in_file_path, "rb+");
    if (in_file == NULL) {
        printf("%s\n", "输入文件打开有问题");
        return -1;
    }
}

void av_dump_format() {
#if AV_DUMP_FORMAT
    printf("-------------File Information-------------\n");
    av_dump_format(avformat_context, 0, in_file_path, 0);
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
 无声电影
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
                if (av_read_frame(avformat_context, avpacket) < 0) {
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
    }

    close();

    return 0;
}

void *alexander_decode_video_thread(void *) {
    while (av_read_frame(avformat_context, avpacket) >= 0) {
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
    //创建线程
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
        if (strstr(pc, ".yuv") != NULL || strstr(pc, ".rgb") != NULL) {
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
            printf("avpacket->size: %6d\n", avpacket->size);
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
 播放窗口大小改变
 */
int alexander_how_to_use_sws_scale() {
    const char *srcFileName = "/root/视频/yuv/480_272_yuv420p.yuv";
    const char *dstFileName = "/root/视频/yuv/240_240_yuv420p.yuv";

    struct SwsContext *img_convert_ctx = nullptr;

    // 設定原始 YUV 的長寬
    const int in_width = 480;
    const int in_height = 272;

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

void test(char) {

}






//////////////////////////////////////////////////////////////////

const char *src_filename = "/root/视频/haoke.avi";
const char *video_dst_filename = "/root/视频/haoke.yuv";
const char *audio_dst_filename = "/root/音乐/haoke.aac";

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
    avframe = av_frame_alloc();
    if (!avframe) {
        printf("%s\n", "");
        return -1;
    }

    av_init_packet(avpacket);
    avpacket->data = NULL;
    avpacket->size = 0;

    while (av_read_frame(avformat_context, avpacket) >= 0) {
        do {
            ret = decode_packet(&got_frame);
            printf("decode packet size: %d\n", ret);
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
    avformat_close_input(&avformat_context);
    av_frame_free(&avframe);
    av_free(video_dst_data[0]);

    fclose(pOutputAudio);
    fclose(pOutputVideo);

    return 0;
}

#endif //MYSTUDY_FFMPEG_LEIXIAOHUA