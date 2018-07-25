//
// Created by root on 18-7-24.
//

#ifndef MYSTUDY_FFMPEG_LEIXIAOHUA
#define MYSTUDY_FFMPEG_LEIXIAOHUA

#include "../include/MyHead.h"

///////////////////////////公共变量///////////////////////////

//格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
AVFormatContext *avformat_context = NULL;

//AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
AVPacket *avpacket = NULL;
AVFrame *avframe = NULL;

int video_stream_index = -1;
int audio_stream_index = -1;
//用于线程
int thread_pause_flag = 0;
int thread_exit_flag = 0;

const char *in_file_path = "/root/视频/aaaaa.rmvb";
const char *out_file_path = NULL;

int ret = -1, got_picture_ptr = -1;

///////////////////////////video///////////////////////////

//SwsContext作为sws_scale的第一个参数,记录数据要转换的格式、大小及转换方式(用于图像像素的处理)
struct SwsContext *video_sws_context = NULL;
//源视频宽高
int src_video_width = 0, src_video_height = 0;
int playback_window_w = 0, playback_window_h = 0;
AVCodecContext *video_avcodec_context = NULL;
AVCodecParameters *video_avcodec_parameters = NULL;
//src_avframe保存原始帧 dst_avframe转换成yuv后的帧保存在此处
AVFrame *video_src_avframe = NULL, *video_dst_avframe = NULL;
//Decoder Encoder
AVCodec *video_avcodec_decoder = NULL, *video_avcodec_encoder = NULL;
// 先自己申请空间,然后为初始化video_dst_avframe而服务
unsigned char *video_out_buffer = NULL;
uint8_t *video_out_buffer2 = NULL;
int video_out_buffer_size = 0;
//像素格式
enum AVPixelFormat dst_avpixel_format = AV_PIX_FMT_YUV420P;

///////////////////////////audio///////////////////////////

struct SwrContext *audio_swr_context = NULL;
AVCodecContext *audio_avcodec_context = NULL;
AVCodecParameters *audio_avcodec_parameters = NULL;
//
AVFrame *audio_src_avframe = NULL, *audio_dst_avframe = NULL;
//Decoder Encoder
AVCodec *audio_avcodec_decoder = NULL, *audio_avcodec_encoder = NULL;
unsigned char *audio_out_buffer = NULL;
uint8_t *audio_out_buffer2 = NULL;
int audio_out_buffer_size = 0;
//采样格式
enum AVSampleFormat audio_avsample_format = AV_SAMPLE_FMT_S16;

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

void init_sdl() {
    if (src_video_width == 0 || src_video_height == 0) {
        return;
    }

    char *file_name;
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
    printf("file_name: %s\n", file_name);

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
    if (video_src_avframe != NULL) {
        av_frame_free(&video_src_avframe);
        video_src_avframe = NULL;
    }
    if (video_dst_avframe != NULL) {
        av_frame_free(&video_dst_avframe);
        video_dst_avframe = NULL;
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
    if (audio_src_avframe != NULL) {
        av_frame_free(&audio_src_avframe);
        audio_src_avframe = NULL;
    }
    if (audio_dst_avframe != NULL) {
        av_frame_free(&audio_dst_avframe);
        audio_dst_avframe = NULL;
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

    src_video_width = video_avcodec_context->width;
    src_video_height = video_avcodec_context->height;
    printf("src_video_width = %d, src_video_height = %d\n",
           src_video_width, src_video_height);

    video_src_avframe = av_frame_alloc();
    video_dst_avframe = av_frame_alloc();
    avpacket = (AVPacket *) av_malloc(sizeof(AVPacket));

    int image_get_buffer_size = av_image_get_buffer_size(dst_avpixel_format,
                                                         src_video_width,
                                                         src_video_height,
                                                         1);
    video_out_buffer = (unsigned char *) av_malloc(image_get_buffer_size);
    av_image_fill_arrays(video_dst_avframe->data, video_dst_avframe->linesize,
                         video_out_buffer,
                         dst_avpixel_format,
                         src_video_width, src_video_height,
                         1);
    video_sws_context = sws_getContext(src_video_width, src_video_height,
                                       video_avcodec_context->pix_fmt,
                                       src_video_width, src_video_height,
                                       dst_avpixel_format,
                                       SWS_BICUBIC,
                                       NULL, NULL, NULL);
    if (video_sws_context == NULL) {
        printf("%s\n", "");
        return -1;
    }

    init_sdl();
    if (sdl_thread == NULL) {
        printf("%s\n", "");
        return -1;
    }

    printf("image_get_buffer_size = %d\n", image_get_buffer_size);
    // 即使源文件像素格式与目标文件像素格式一样,也是需要调用sws_scale函数进行处理的
    printf("源文件像素格式为: %d\n", video_avcodec_context->pix_fmt);
    printf("目标文件像素格式为: %d\n", dst_avpixel_format);

    //Output Info-----------------------------
    printf("---------------- File Information ---------------\n");
    av_dump_format(avformat_context, 0, in_file_path, 0);
    printf("-------------------------------------------------\n");

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
            ret = avcodec_decode_video2(video_avcodec_context, video_src_avframe, &got_picture_ptr, avpacket);
            if (ret < 0) {
                printf("Decode Error.\n");
                return -1;
            }
            if (got_picture_ptr) {
                sws_scale(video_sws_context, (const unsigned char *const *) video_src_avframe->data,
                          video_src_avframe->linesize, 0,
                          src_video_height, video_dst_avframe->data, video_dst_avframe->linesize);

                //SDL---------------------------
                SDL_UpdateTexture(sdl_texture, NULL, video_dst_avframe->data[0], video_dst_avframe->linesize[0]);
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