//
// Created by root on 18-7-22.
//

#ifndef MYSTUDY_FFMPEG_YUV_PLAYER
#define MYSTUDY_FFMPEG_YUV_PLAYER

#include "../include/MyHead.h"

//Convert RGB24/BGR24 to RGB32/BGR32
//And change Endian if needed
void convert_24to32(unsigned char *image_in, unsigned char *image_out, int pixel_w, int pixel_h) {
    for (int i = 0; i < pixel_h; i++)
        for (int j = 0; j < pixel_w; j++) {
            //Big Endian or Small Endian?
            //"ARGB" order:high bit -> low bit.
            //ARGB Format Big Endian (low address save high MSB, here is A) in memory : A|R|G|B
            //ARGB Format Little Endian (low address save low MSB, here is B) in memory : B|G|R|A
            if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                //Little Endian (x86): R|G|B --> B|G|R|A
                image_out[(i * pixel_w + j) * 4 + 0] = image_in[(i * pixel_w + j) * 3 + 2];
                image_out[(i * pixel_w + j) * 4 + 1] = image_in[(i * pixel_w + j) * 3 + 1];
                image_out[(i * pixel_w + j) * 4 + 2] = image_in[(i * pixel_w + j) * 3];
                image_out[(i * pixel_w + j) * 4 + 3] = '0';
            } else {
                //Big Endian: R|G|B --> A|R|G|B
                image_out[(i * pixel_w + j) * 4] = '0';
                memcpy(image_out + (i * pixel_w + j) * 4 + 1, image_in + (i * pixel_w + j) * 3, 3);
            }
        }
}

int thread_exit = 0;

int refresh_video(void *opaque) {
    thread_exit = 0;
    while (!thread_exit) {
        SDL_Event event;
        event.type = REFRESH_EVENT;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }
    thread_exit = 0;
    //Break
    SDL_Event event;
    event.type = BREAK_EVENT;
    SDL_PushEvent(&event);
    printf("SDL_PushEvent BREAK_EVENT\n");

    return 0;
}

/***
 文件名命名规则:
 320_180_bgra.rgb       AV_PIX_FMT_BGRA
 640_480_rgb24.rgb      AV_PIX_FMT_RGB24
 320_180_bgr24.rgb      AV_PIX_FMT_BGR24
 480_272_yuv420p.yuv    AV_PIX_FMT_YUV420P
 */
int bgra_rgb24_bgr24_yuv420p_player_with_sdl2_pure(char *in_file_path) {
    if (in_file_path == NULL) {
        printf("%s\n", "");
        return -1;
    }

    printf("in_file_path: %s\n", in_file_path);
    FILE *in_file = fopen(in_file_path, "rb+");
    if (in_file == NULL) {
        printf("cannot open this file\n");
        return -1;
    }

    int pixel_w = 0, pixel_h = 0;
    char *pc = NULL;
    char temp_path[strlen(in_file_path)];
    strncpy(temp_path, in_file_path + 1, strlen(in_file_path));

    pc = strtok(temp_path, "/");
    while (pc != NULL) {
        if (strstr(pc, ".yuv") != NULL
            || strstr(pc, ".rgb") != NULL
            || strstr(pc, ".y") != NULL) {
            in_file_path = pc;
            printf("in_file_name: %s\n", in_file_path);
            break;
        }
        pc = strtok(NULL, "/");
    }
    pc = strtok(in_file_path, "_");
    int i = 0;
    while (pc != NULL) {
        if (i == 0) {
            pixel_w = atoi(pc);
        } else if (i == 1) {
            pixel_h = atoi(pc);
        } else {
            break;
        }
        i++;
        pc = strtok(NULL, "_");
    }
    printf("pixel_w: %d, pixel_h: %d\n", pixel_w, pixel_h);
    if (pixel_w == 0 || pixel_h == 0) {
        printf("%s", "");
        return -1;
    }

//set '1' to choose a type of file to play
#define LOAD_BGRA    0
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_YUV420P 1
//下面两个还没有验证
#define LOAD_YUV422P 1
#define LOAD_YUV444P 1

    //Bit per Pixel
    int src_video_bpp = 0;
    Uint32 sdl_pix_fmt = 0;
#if LOAD_BGRA
    //Note: ARGB8888 in "Little Endian" system stores as B|G|R|A
    src_video_bpp = 32;
    sdl_pix_fmt = SDL_PIXELFORMAT_ARGB8888;
#elif LOAD_RGB24
    src_video_bpp = 24;
    sdl_pix_fmt= SDL_PIXELFORMAT_RGB888;
#elif LOAD_BGR24
    src_video_bpp = 24;
    sdl_pix_fmt = SDL_PIXELFORMAT_BGR888;
#elif LOAD_YUV420P
    src_video_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(AV_PIX_FMT_YUV420P));
    //YV12: Y + V + U  (3 planes)
    //IYUV: Y + U + V  (3 planes)
    sdl_pix_fmt = SDL_PIXELFORMAT_IYUV;
#elif LOAD_YUV422P
    src_video_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(AV_PIX_FMT_YUV422P));
    sdl_pix_fmt = SDL_PIXELFORMAT_IYUV;
#elif LOAD_YUV444P
    src_video_bpp = av_get_bits_per_pixel(av_pix_fmt_desc_get(AV_PIX_FMT_YUV444P));
    sdl_pix_fmt = SDL_PIXELFORMAT_IYUV;
#endif

    printf("src_video_bpp = %d\n", src_video_bpp);
    const int data_buffer_size = pixel_w * pixel_h * src_video_bpp / 8;
    //总的倍数除以8就是byte数
    unsigned char data_buffer[data_buffer_size];
    //BPP=32
    unsigned char data_buffer_convert[pixel_w * pixel_h * 4];

    SDL_Window *sdl_window = NULL;
    SDL_Renderer *sdl_renderer = NULL;
    SDL_Texture *sdl_texture = NULL;
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    //SDL 2.0 Support for multiple windows
    sdl_window = SDL_CreateWindow(temp_path,
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  pixel_w, pixel_h,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!sdl_window) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);

    sdl_texture = SDL_CreateTexture(sdl_renderer,
                                    sdl_pix_fmt,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    pixel_w, pixel_h);
    //SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video, NULL, NULL);
    SDL_CreateThread(refresh_video, NULL, NULL);

    SDL_Event event;
    SDL_Rect sdl_rect;
    while (1) {
        //Wait
        SDL_WaitEvent(&event);
        if (event.type == REFRESH_EVENT) {
            if (fread(data_buffer, 1, data_buffer_size, in_file) != data_buffer_size) {
                //Loop
                fseek(in_file, 0, SEEK_SET);
                fread(data_buffer, 1, data_buffer_size, in_file);
            }

#if LOAD_BGRA
            //We don't need to change Endian
            //Because input BGRA pixel data(B|G|R|A) is same as ARGB8888 in Little Endian (B|G|R|A)
            SDL_UpdateTexture(sdl_texture, NULL, data_buffer, pixel_w * 4);
#elif LOAD_RGB24 | LOAD_BGR24
            //为什么要从24位转化到32位
            //change 24 bit to 32 bit and in Windows we need to change Endian
            convert_24to32(data_buffer, data_buffer_convert, pixel_w, pixel_h);
            SDL_UpdateTexture(sdl_texture, NULL, data_buffer_convert, pixel_w * 4);
#elif LOAD_YUV420P
            SDL_UpdateTexture(sdl_texture, NULL, data_buffer, pixel_w);
#elif LOAD_YUV422P
            SDL_UpdateTexture(sdl_texture, NULL, data_buffer, pixel_w);
#elif LOAD_YUV444P
            //convert_24to32(data_buffer, data_buffer_convert, pixel_w, pixel_h);
            SDL_UpdateTexture(sdl_texture, NULL, data_buffer, pixel_w);
#endif

            //FIX: If window is resize
            sdl_rect.x = 0;
            sdl_rect.y = 0;
            sdl_rect.w = pixel_w;
            sdl_rect.h = pixel_h;

            SDL_RenderClear(sdl_renderer);
            SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, &sdl_rect);
            SDL_RenderPresent(sdl_renderer);
        } else if (event.type == SDL_WINDOWEVENT) {
            //printf("SDL_WINDOWEVENT\n");
            //If Resize
            SDL_GetWindowSize(sdl_window, &pixel_w, &pixel_h);
        } else if (event.type == SDL_QUIT) {
            printf("SDL_QUIT\n");
            thread_exit = 1;
        } else if (event.type == BREAK_EVENT) {
            printf("BREAK_EVENT\n");
            break;
        }
    }// while end

    SDL_Quit();

    return 0;
}


#endif //MYSTUDY_FFMPEG_YUV_PLAYER
