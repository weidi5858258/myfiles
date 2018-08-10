#include "./include/MyHead.h"
#include "Method.h"

#ifndef MYSTUDY_STUDY
#define MYSTUDY_STUDY

#include "src/Study.cpp"

#endif //MYSTUDY_STUDY

#ifndef MYSTUDY_STUDY_FFMPEG
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

#endif //MYSTUDY_FFMPEG_MUSIC_PLAYER


void test();


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
//    bgra_rgb24_bgr24_yuv420p_player_with_sdl2_pure("/root/视频/rgb/240_240_rgb24_haoke.rgb",
//                                                   240, 240);
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
    simplest_yuv420p_split("/root/视频/tomcat_video/yuv/256_256_yuv420p.yuv", 256, 256, 1);
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
//    alexander_music_player("/root/音乐/tdjm.mp3");
//    alexander_music_player("/root/音乐/txdx.mp3");
//    alexander_music_player("/root/视频/Prison.Break.S05E01.WEB-HR.AAC.720P.x264V3.mp4");
//    alexander_audio_or_video_demuxer();

//    Method method;
//    method.wd_fgets();

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}

typedef struct stud {
    int num;
    int score;
    struct stud *next = NULL;
};

void testLink() {
    struct stud *head = NULL, *p = NULL;

    p = (struct stud *) malloc(sizeof(struct stud));
    scanf("%d%d", &(p->num), &(p->score));

    head = p;
    p->next = NULL;

    p = (struct stud *) malloc(sizeof(struct stud));
    scanf("%d%d", &(p->num), &(p->score));

    p->next = head;
    head = p;
}

/***
 头插法建立单链表
 */
struct stud *h_create() {
    struct stud *head = NULL, *p = NULL;
    int n;
    while (1) {
        printf("\nInput num:");
        scanf("%d", &n);
        if (n < 0) {
            break;
        }
        p = (struct stud *) malloc(sizeof(struct stud));
        p->num = n;
        printf("\nInput score:");
        scanf("%d", &(p->score));
        p->next = head;
        head = p;
    }
    return head;
}

/***
 尾插法建立单链表
 */
struct stud *r_create() {
    struct stud *head = NULL, *rear = NULL, *p = NULL;
    int n;
    while (1) {
        printf("\nInput num:");
        scanf("%d", &n);
        if (n < 0) {
            break;
        }
        p = (struct stud *) malloc(sizeof(struct stud));
        p->num = n;
        printf("\nInput score:");
        scanf("%d", &(p->score));
        if (rear == NULL) {
            head = p;
            p->next = NULL;
            rear = p;
        } else {
            p->next = rear->next;
            rear->next = p;
            rear = p;
        }
    }
    return head;
}
































































