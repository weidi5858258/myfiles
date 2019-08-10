#include "./include/MyHead.h"
#include "decode_audio.h"

#ifdef MYSTUDY_FFMPEG_STUDYAGAIN
#include "src/StudyAgain.cpp"
#endif

/*#ifndef MYSTUDY_STUDY_FFMPEG
#define MYSTUDY_STUDY_FFMPEG

#include "src/StudyFFmpeg.cpp"

#endif //MYSTUDY_STUDY_FFMPEG

#ifndef MYSTUDY_FFMPEG
#define MYSTUDY_FFMPEG

#include "ffmpeg.cpp"

#endif //MYSTUDY_FFMPEG

#ifndef MYSTUDY_FFMPEG_SAMPLE
#define MYSTUDY_FFMPEG_SAMPLE

#include "src/ffmpeg_sample.cpp"

#endif //MYSTUDY_FFMPEG_SAMPLE

#ifndef MYSTUDY_FFMPEG_YUV_PLAYER
#define MYSTUDY_FFMPEG_YUV_PLAYER

#include "src/player.cpp"

#endif //MYSTUDY_FFMPEG_YUV_PLAYER

#ifndef MYSTUDY_FFMPEG_LEIXIAOHUA
#define MYSTUDY_FFMPEG_LEIXIAOHUA

#include "src/leixiaohua1020.cpp"

#endif //MYSTUDY_FFMPEG_LEIXIAOHUA

#ifndef MYSTUDY_FFMPEG_MUSIC_PLAYER
#define MYSTUDY_FFMPEG_MUSIC_PLAYER

#include "src/music_player.cpp"

#endif //MYSTUDY_FFMPEG_MUSIC_PLAYER*/

int decoder_video_frame_to_image();

int bgra_rgb24_bgr24_yuv420p_player_with_sdl2_pure(char *in_file_path);

int simplest_audio_play_sdl2();

int simplest_ffmpeg_audio_decoder();

int pcm2aac();

int pcm2aac(const char *in_pcm_path, const char *out_aac_path);

int crazydiode_video_devoder();

int crazydiode_audio_devoder();

int simplest_ffmpeg_audio_encoder_pure();

int decode_audio_main(const char *infilename, const char *outfilename);

int decode_audio2(const char *input_file_name, const char *output_file_name);

int encode_audio_main(const char *infilename, const char *outfilename);

int decode_video_main(const char *input_file_name, const char *output_file_name);

int encode_video_main(const char *codec_name, const char *output_file_name);

int recorderScreen();

int yuv420p_2_rgb24();

int gen_rgb24_stripe(int width, int height,
                     unsigned char r, unsigned char g, unsigned char b);

int gen_yuv420p_graybar(int width, int height, int barnum, unsigned char ymin, unsigned char ymax);

int gen_rgb24_colorbar(int width, int height);

int gen_rgb24_rgbgradient_bar(int width, int height, int barnum,
                              unsigned char src_r, unsigned char src_g, unsigned char src_b,
                              unsigned char dst_r, unsigned char dst_g, unsigned char dst_b);

int gen_yuv420p_yuvgradient_bar(int width, int height, int barnum,
                                unsigned char src_y, unsigned char src_u, unsigned char src_v,
                                unsigned char dst_y, unsigned char dst_u, unsigned char dst_v);

int rgb24_to_bmp(char *rgb24path, char *bmppath, int width, int height);

int gen_allcolor_video();

int yuv_2_h264();

int pcm2mp3(char *inPath, char *outPath);

int resampling_audio(const char *dst_filename);

int separate_media_to_yuv_and_aac();

int simplest_yuv420p_split(char *url, int w, int h, int num);

int audio_recorder(const char *out_file_path);

int alexanderAudioPlayerWithSDL();

int alexanderVideoPlayerWithSDL();

int alexander_video_player_sdl2();

int alexander_decode_video_to_yuv();

int alexander_playback_yuv();

int alexander_use_libavcodec_decode_to_yuv();

int alexander_how_to_use_sws_scale();

int alexander_how_to_use_sws_scale2();

int alexander_how_to_use_sws_scale3();

/***
 使用这个函数.
 这个函数可以改变源视频的分辨率大小和像素格式.
 像素格式现在只支持(AV_PIX_FMT_GRAY8,AV_PIX_FMT_YUV420P,AV_PIX_FMT_YUV422P,
 AV_PIX_FMT_YUV444P,AV_PIX_FMT_YUYV422,
 AV_PIX_FMT_RGB24,AV_PIX_FMT_BGR24,
 AV_PIX_FMT_BGRA)这几种之间的互转.
 */
int alexander_how_to_use_sws_scale4();

/***
 播放效果不好
 */
int alexander_playback_pcm();

/***
 播放效果还是不错的,代码比较好
 */
int alexander_music_player(const char *in_file_name);

int alexander_audio_or_video_demuxer();

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

//    simplest_ffmpeg_player();
//    simplest_ffmpeg_player2();
//    bgra_rgb24_bgr24_yuv420p_player_with_sdl2_pure("/root/视频/rgb/240_240_rgb24_haoke.rgb", 240, 240);
//    decoder_video_frame_to_image();
//    simplest_audio_play_sdl2();
//    simplest_ffmpeg_audio_decoder();
    //查看pcm,yuv文件的信息
//    crazydiode_video_devoder();
//    crazydiode_audio_devoder();
//    pcm2aac();
//    pcm2aac("/root/音乐/output.pcm", "/root/音乐/output.aac");
//    simplest_ffmpeg_audio_encoder_pure();
//    decode_audio_main("/root/音乐/GALAYoungForYou.mp3", "/root/音乐/pcm/GALAYoungForYou.pcm");
//    decode_audio2("/root/音乐/GALAYoungForYou.mp3", "/root/音乐/temp.pcm");
//    encode_audio_main("/root/音乐/txdx.pcm", "/root/音乐/txdx.mp2");
//    pcm2mp3("/root/音乐/audio.pcm", "/root/音乐/audio.mp3");
//    decode_video_main("/root/视频/sex.avi", "/root/图片/video_to_image");
//    resampling_audio("/root/音乐/resampling.mp2");
//    separate_media_to_yuv_and_aac();
//    simplest_yuv420p_split("/root/视频/tomcat_video/yuv/256_256_yuv420p.yuv", 256, 256, 1);
//    audio_recorder("/root/音乐/myrecorder.pcm");
//    alexander_video_player_sdl2();
//    alexander_decode_video_to_yuv();// OUTPUT_YUV420P 1
//    alexander_playback_yuv();
//    alexander_use_libavcodec_decode_to_yuv();
//    alexander_how_to_use_sws_scale();
//    alexander_how_to_use_sws_scale2();
//    alexander_how_to_use_sws_scale3();
//    alexander_how_to_use_sws_scale4();
//    bgra_rgb24_bgr24_yuv420p_player_with_sdl2_pure("/root/视频/rgb/240_240_bgra.rgb");
//    bgra_rgb24_bgr24_yuv420p_player_with_sdl2_pure("/root/视频/tomcat_video/yuv/256_256_yuv420p.yuv");
//    alexander_playback_pcm();

//    alexanderAudioPlayerWithSDL();
    alexanderVideoPlayerWithSDL();
//    alexander_music_player("/root/音乐/alexander_music/冷漠、云菲菲 - 伤心城市.mp3");
//    alexander_music_player("/root/音乐/KuGou/冷漠、云菲菲 - 伤心城市.mp3");

//    alexander_music_player("/root/音乐/txdx.mp3");
//    alexander_music_player("/root/视频/Prison.Break.S05E01.WEB-HR.AAC.720P.x264V3.mp4");
//    alexander_audio_or_video_demuxer();

    /*decode_audio da;
    char *name[] = {"/root/音乐/tdjm.aac", "/root/音乐/tdjm.pcm"};
    da.main_(2, name);*/

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}

/***
 注:本文中像素的采样位数一律为8bit.由于1Byte=8bit,所以一个像素的一个分量的采样值占用1Byte.
 注：本文中声音样值的采样频率一律是44100Hz,采样格式一律为16LE.
 “16”代表采样位数是16bit.由于1Byte=8bit,所以一个声道的一个采样值占用2Byte.
 “LE”代表Little Endian,代表2 Byte采样值的存储方式为高位存在高地址中.

 unsigned char相当于byte(标准C中没有byte)
                  char                           -128~127
 typedef unsigned char		uint8_t;  1个字节 8位  0~255(ffmpeg中常用到)
                  short int                      -32768~32767
 typedef unsigned short int	uint16_t; 2个字节16位  0~65535
                  int                            -2147483648~2147483647
 typedef unsigned int		uint32_t; 4个字节32位  0~4294967295
 typedef unsigned long int	uint64_t; 8个字节64位

 在ffmpeg中常用的代码:
 FILE *fp=fopen(url,"rb+");
 FILE *fp1=fopen("output_420_y.y","wb+");
 unsigned char *pic=(unsigned char *)malloc(w*h*3/2);
 fread(pic,1,w*h*3/2,fp);
 //Y
 fwrite(pic,1,w*h,fp1);
 free(pic);
 fclose(fp);
 fclose(fp1);

    // 第二种解码方式,旧的
    int got_picture_ptr = 0;
    int ret = avcodec_decode_video2(videoWrapper.father.avCodecContext,
                                videoWrapper.father.srcAVFrame,
                                &got_picture_ptr,
                                avPacket);
    if (ret < 0) {
        fprintf(stderr, "video decode error.\n");
        break;
    }
    if (got_picture_ptr) {
        sws_scale(videoWrapper.swsContext,
                  (const unsigned char *const *) videoWrapper.father.srcAVFrame->data,
                  videoWrapper.father.srcAVFrame->linesize,
                  0, videoWrapper.srcHeight,
                  videoWrapper.father.dstAVFrame->data,
                  videoWrapper.father.dstAVFrame->linesize);
    }

 关于子线程
 std::mutex lockMutex;
 std::condition_variable lockCond;
 // 唤醒线程
 std::lock_guard<std::mutex> lk(lockMutex);
 fprintf(stdout, "readData() lockCond.notify_one()\n");
 lockCond.notify_one();
 // 线程等待
 std::unique_lock<std::mutex> lk(lockMutex);
 fprintf(stdout, "handleData() lockCond.wait() start\n");
 lockCond.wait(lk,
                  [] {
                      // 如果直接返回true的话,不会等待
                      return true;
                      // 如果直接返回false的话,一直等待
                      return false;
                      // 只有在唤醒的地方创建好条件,然后在这里根据那个条件判断出是true就往下走
                      return videoWrapper.father.isHandlingForQueue1;
                  });
 fprintf(stdout, "handleData() lockCond.wait() end\n");
 lk.unlock();

 pthread_mutex_t lockMutex = PTHREAD_MUTEX_INITIALIZER;
 pthread_cond_t lockCondition = PTHREAD_COND_INITIALIZER;
 // 唤醒线程
 fprintf(stdout, "readData() pthread_cond_signal()\n");
 pthread_cond_signal(&lockCondition);
 // 线程等待
 fprintf(stdout, "handleData() pthread_cond_wait() start\n");
 pthread_cond_wait(&lockCondition, &lockMutex);
 fprintf(stdout, "handleData() pthread_cond_wait() end\n");

    // 不用SDL的线程,不然到时候不好移植
    // 创建子线程.audioRender和audioRender函数中的代码就是在子线程中执行的
    SDL_Thread *readDataThread = SDL_CreateThread(readData, NULL, NULL);
    SDL_Thread *handleDataThread = SDL_CreateThread(handleData, NULL, NULL);
    // 如果没有下面两个等待函数,那么子线程可能连执行的机会都没有
    int status = 0;
    if (readDataThread != NULL) {
        // 等待readData函数里的代码执行完后才往下走,不然一直阻塞在这里
        SDL_WaitThread(readDataThread, &status);
        printf("alexanderVideoPlayerWithSDL() readDataThread   status: %d\n", status);
        // 线程不要在这里析构
    }
    if (handleDataThread != NULL) {
        SDL_WaitThread(handleDataThread, &status);
        printf("alexanderVideoPlayerWithSDL() handleDataThread status: %d\n", status);
    }
    // 线程在最后析构
    if (readDataThread != NULL) {
        SDL_DetachThread(readDataThread);
        readDataThread = NULL;
    }
    if (handleDataThread != NULL) {
        SDL_DetachThread(handleDataThread);
        handleDataThread = NULL;
    }

 */


































































