//
// Created by alexander on 2020/3/2.
//

#ifndef FFMPEG_STUDY_FFPLAY_H
#define FFMPEG_STUDY_FFPLAY_H

#include "cmdutils.h"
#include "config.h"
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <secure/_stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/avstring.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

#if CONFIG_AVFILTER

# include "libavfilter/avfilter.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"

#endif

#ifdef __cplusplus
}
#endif

#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 25
#define EXTERNAL_CLOCK_MIN_FRAMES 2
#define EXTERNAL_CLOCK_MAX_FRAMES 10

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0 // 10秒

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* external clock speed adjustment constants for realtime sources based on buffer fullness */
#define EXTERNAL_CLOCK_SPEED_MIN  0.900
#define EXTERNAL_CLOCK_SPEED_MAX  1.010
#define EXTERNAL_CLOCK_SPEED_STEP 0.001

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* polls for possible required screen refresh at least this often, should be less than 1/fps */
#define REFRESH_RATE 0.01

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
#define SAMPLE_ARRAY_SIZE (8 * 65536)

#define CURSOR_HIDE_DELAY 1000000

#define USE_ONEPASS_SUBTITLE_RENDER 1

#define PICTURE_QUEUE_SIZE 3
#define SAMPLE_QUEUE_SIZE 9
#define SUBPICTURE_QUEUE_SIZE 16
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))

typedef struct AudioParams {
    // 采样率
    int sample_rate;
    // 声道数
    int channels;
    // 声道布局
    int64_t channel_layout;
    enum AVSampleFormat sample_fmt;
    int frame_size;
    int bytes_per_sec;
} AudioParams;

typedef struct Clock {
    // 初始化时为NAN
    double pts;           /* clock base */
    double pts_drift;     /* clock base minus time at which we updated the clock */
    // 初始化时为av_gettime_relative() / 1000000.0;
    double last_updated;
    // 初始化时为1.0(以外部时钟作为基准时钟时才设置速度)
    double speed;
    // 初始化时为-1
    int serial;           /* clock is based on a packet with this serial */
    // 初始化时为0,需要暂停时为1
    int paused;
    // 指向当前数据包队列序列的指针，用于过时的时钟检测
    // 就是PacketQueue的serial值
    int *queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
} Clock;

/* Common struct for handling all types of decoded data and allocated render buffers. */
typedef struct Frame {
    // 解码帧
    AVFrame *frame;
    // 字幕
    AVSubtitle sub;
    // 就是Decoder的pkt_serial值
    int serial;
    // 帧的显示时间戳
    double pts;           /* presentation timestamp for the frame */
    // 帧显示时长
    double duration;      /* estimated duration of the frame */
    // 文件中的位置
    int64_t pkt_pos;          /* byte position of the frame in the input file */
    int width;
    int height;
    // 格式
    int format;
    // 额外参数
    AVRational sample_aspect_ratio;
    // 上载
    int uploaded;
    // 反转
    int flip_v;
    // 为了看流程设置的参数,没有实际意义
    double relative_time;
} Frame;
// 一个Node(节点)
// 原名:MyAVPacketList
typedef struct AVPacketNode {
    AVPacket pkt;
    struct AVPacketNode *next;
    // 就是PacketQueue的serial值
    int serial;
} AVPacketNode;

/***
PacketQueue:serial(在packet_queue_put_private()时加1操作)
影响
AVPacketNode:serial(packet_queue_put_private()时由PacketQueue:serial直接赋值)
Clock:queue_serial(init_clock()时传递指针,指向了PacketQueue:serial)
Decoder:pkt_serial(packet_queue_get()时传递指针,指向了AVPacketNode:serial)
Frame:serial(由Decoder:pkt_serial赋值)
 */
// 保存AVPacket(音频,视频,字幕各一个Queue)
typedef struct PacketQueue {
    AVPacketNode *first_pkt, *last_pkt;
    // 队列中有多少个AVPacket
    int nb_packets;
    // 队列中所有AVPacket的大小总和
    int size;
    /***
     如果从头到尾不seek,那么其值一直为1;每seek一次值加1.
     此值影响其他所有有关的serial值.
     因为每次seek后,队列里有seek前的pkt和seek后的pkt,
     此后要处理的是seek后的pkt,需要把seek前的pkt区别开.
     */
    int serial;
    int abort_request;
    // 队列中所有AVPacket的时长总和
    int64_t duration;
    pthread_mutex_t pMutex;// "同步"作用
    pthread_cond_t pCond;  // "暂停"作用
} PacketQueue;

// 保存AVFrame(音频,视频,字幕各一个Queue)
typedef struct FrameQueue {
    // 指向VideoState中的PacketQueue
    PacketQueue *pktQ;
    // 视频放3 个元素
    // 音频放9 个元素
    // 字幕放16个元素
    Frame queue[FRAME_QUEUE_SIZE];
    int read_index;
    int write_index;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    pthread_mutex_t pMutex;// "同步"作用
    pthread_cond_t pCond;  // "暂停"作用
} FrameQueue;

// 音频,视频,字幕各有一个
typedef struct Decoder {
    AVPacket pkt;
    PacketQueue *queue;
    // 视频,音频,字幕各不同
    AVCodecContext *avctx;
    // 就是AVPacketNode的serial值
    int pkt_serial;
    int finished;
    // 是否有包在等待
    int packet_pending;
    // 开始的时间戳
    int64_t start_pts;
    // 开始的额外参数
    AVRational start_pts_tb;
    // 下一帧时间戳
    int64_t next_pts;
    // 下一帧的额外参数
    AVRational next_pts_tb;
    pthread_t decoder_thread;
    pthread_cond_t *empty_queue_cond;
} Decoder;

enum {
    AV_SYNC_AUDIO_MASTER,   /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

enum ShowMode {
    SHOW_MODE_NONE = -1,
    SHOW_MODE_VIDEO = 0,
    SHOW_MODE_WAVES,
    SHOW_MODE_RDFT,
    SHOW_MODE_NB
};
static enum ShowMode show_mode = SHOW_MODE_NONE;

typedef struct VideoState {
    pthread_t read_thread;
    //SDL_Thread *read_tid;// 读线程id
    AVInputFormat *iformat;
    int abort_request;
    int force_refresh;
    int paused;
    int last_paused;
    int queue_attachments_req;
    int seek_req;// 需要seek时为1,否则为0
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
    int read_pause_return;
    AVFormatContext *avFormatContext;
    int realtime;

    // 同步方式(以什么为基准进行同步,默认以音频为基准进行同步)
    int av_sync_type;
    Clock audClock;
    Clock vidClock;
    Clock extClock;

    PacketQueue videoPQ;
    PacketQueue audioPQ;
    PacketQueue subtitlePQ;

    FrameQueue pictFQ;
    FrameQueue sampFQ;
    FrameQueue subpFQ;

    Decoder audDecoder;
    Decoder vidDecoder;
    Decoder subDecoder;

    // audio
    int audio_stream;
    int audio_clock_serial;
    int audio_diff_avg_count;
    double audio_clock;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    AVStream *audio_st;
    uint8_t *audio_buf;
    uint8_t *audio_buf1;
    unsigned int audio_buf_size; /* in bytes */
    unsigned int audio_buf1_size;
    int audio_buf_index; /* in bytes */
    int audio_hw_buf_size;
    int audio_write_buf_size;
    int audio_volume;// 音量
    int muted;// 静音
    struct AudioParams audio_src;
#if CONFIG_AVFILTER
    struct AudioParams audio_filter_src;
#endif
    struct AudioParams audio_tgt;
    struct SwrContext *swr_ctx;
    int frame_drops_early;
    int frame_drops_late;

    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;
    RDFTContext *rdft;
    int rdft_bits;
    FFTSample *rdft_data;
    int xpos;
    double last_vis_time;
    // subtitle
    int subtitle_stream;
    AVStream *subtitle_st;
    // video
    double frame_timer;
    double frame_last_returned_time;
    double frame_last_filter_delay;
    int video_stream;
    AVStream *video_st;
    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    struct SwsContext *img_convert_ctx;
    struct SwsContext *sub_convert_ctx;
    int is_eof;

    char *filename;
    int width, height, xleft, ytop;
    int step;

#if CONFIG_AVFILTER
    int vfilter_idx;
    AVFilterContext *in_video_filter;   // the first filter in the video chain
    AVFilterContext *out_video_filter;  // the last filter in the video chain
    AVFilterContext *in_audio_filter;   // the first filter in the audio chain
    AVFilterContext *out_audio_filter;  // the last filter in the audio chain
    AVFilterGraph *agraph;              // audio filter graph
#endif

    int last_audio_stream, last_video_stream, last_subtitle_stream;

    //SDL_cond *continue_read_thread;
    pthread_mutex_t continue_read_thread_mutex;
    pthread_cond_t continue_read_thread_cond;
    enum ShowMode show_mode;
} VideoState;

const char program_name[] = "ffplay";
const int program_birth_year = 2003;
static unsigned sws_flags = SWS_BICUBIC;

/* options specified by the user */
static AVInputFormat *file_iformat;
static const char *input_filename;
static const char *window_title;
static int default_width = 640;
static int default_height = 480;
static int screen_width = 0;
static int screen_height = 0;
static int audio_disable;
static int video_disable;
static int subtitle_disable;
static const char *wanted_stream_spec[AVMEDIA_TYPE_NB] = {0};
static int seek_by_bytes = -1;
static float seek_interval = 10;
static int display_disable;
static int borderless;
static int alwaysontop;
static int startup_volume = 100;
static int show_status = 0;// 打印某些日志
static int av_sync_type = AV_SYNC_AUDIO_MASTER;
static int64_t start_time = AV_NOPTS_VALUE;
static int64_t duration = AV_NOPTS_VALUE;
static int fast = 0;
static int genpts = 0;
static int lowres = 0;
static int decoder_reorder_pts = -1;
static int autoexit;
static int exit_on_keydown;
static int exit_on_mousedown;
static int loop = 1;
static int framedrop = -1;
static int infinite_buffer = -1;
// 根据name找到AVCodec(解码器)
static const char *audio_codec_name;
static const char *video_codec_name;
static const char *subtitle_codec_name;
double rdftspeed = 0.02;
static int64_t cursor_last_shown;
static int cursor_hidden = 0;
#if CONFIG_AVFILTER
static const char **vfilters_list = NULL;
static int nb_vfilters = 0;
static char *afilters = NULL;
#endif
static int autorotate = 1;
static int find_stream_info = 1;
static int filter_nbthreads = 0;

/* current context */
static int is_full_screen;
static int64_t audio_callback_time;

static AVPacket flush_pkt;

#if CONFIG_AVFILTER

static int opt_add_vfilter(void *optctx, const char *opt, const char *arg) {
    //GROW_ARRAY(vfilters_list, nb_vfilters);
    vfilters_list = (const char **) grow_array(
            vfilters_list, sizeof(*vfilters_list), &nb_vfilters, nb_vfilters + 1);
    vfilters_list[nb_vfilters - 1] = arg;
    return 0;
}

#endif

static inline
int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
                   enum AVSampleFormat fmt2, int64_t channel_count2) {
    /* If channel count == 1, planar and non-planar formats are the same */
    if (channel_count1 == 1 && channel_count2 == 1)
        return av_get_packed_sample_fmt(fmt1) != av_get_packed_sample_fmt(fmt2);
    else
        return channel_count1 != channel_count2 || fmt1 != fmt2;
}

static inline
int64_t get_valid_channel_layout(int64_t channel_layout, int channels) {
    if (channel_layout && av_get_channel_layout_nb_channels(channel_layout) == channels)
        return channel_layout;
    else
        return 0;
}

static int opt_frame_size(void *optctx, const char *opt, const char *arg) {
    av_log(NULL, AV_LOG_WARNING, "Option -s is deprecated, use -video_size.\n");
    return opt_default(NULL, "video_size", arg);
}

static int opt_width(void *optctx, const char *opt, const char *arg) {
    screen_width = parse_number_or_die(opt, arg, OPT_INT64, 1, INT_MAX);
    return 0;
}

static int opt_height(void *optctx, const char *opt, const char *arg) {
    screen_height = parse_number_or_die(opt, arg, OPT_INT64, 1, INT_MAX);
    return 0;
}

static int opt_format(void *optctx, const char *opt, const char *arg) {
    printf("opt_format()\n");
    file_iformat = av_find_input_format(arg);
    if (!file_iformat) {
        av_log(NULL, AV_LOG_FATAL, "Unknown input format: %s\n", arg);
        return AVERROR(EINVAL);
    }
    return 0;
}

static int opt_frame_pix_fmt(void *optctx, const char *opt, const char *arg) {
    av_log(NULL, AV_LOG_WARNING, "Option -pix_fmt is deprecated, use -pixel_format.\n");
    return opt_default(NULL, "pixel_format", arg);
}

static int opt_sync(void *optctx, const char *opt, const char *arg) {
    if (!strcmp(arg, "audio"))
        av_sync_type = AV_SYNC_AUDIO_MASTER;
    else if (!strcmp(arg, "video"))
        av_sync_type = AV_SYNC_VIDEO_MASTER;
    else if (!strcmp(arg, "ext"))
        av_sync_type = AV_SYNC_EXTERNAL_CLOCK;
    else {
        av_log(NULL, AV_LOG_ERROR, "Unknown value for %s: %s\n", opt, arg);
        exit(1);
    }
    return 0;
}

static int opt_seek(void *optctx, const char *opt, const char *arg) {
    start_time = parse_time_or_die(opt, arg, 1);
    return 0;
}

static int opt_duration(void *optctx, const char *opt, const char *arg) {
    duration = parse_time_or_die(opt, arg, 1);
    return 0;
}

static int opt_show_mode(void *optctx, const char *opt, const char *arg) {
    int cmp = strcmp(arg, "video");
    if (!cmp) {
        show_mode = SHOW_MODE_VIDEO;
    } else {
        cmp = strcmp(arg, "waves");
        if (!cmp) {
            show_mode = SHOW_MODE_WAVES;
        } else {
            cmp = strcmp(arg, "rdft");
            if (!cmp) {
                show_mode = SHOW_MODE_RDFT;
            } else {
                show_mode = (enum ShowMode) parse_number_or_die(opt, arg, OPT_INT, 0, SHOW_MODE_NB - 1);
            }
        }
    }
    /*show_mode = !strcmp(arg, "video") ? SHOW_MODE_VIDEO :
                !strcmp(arg, "waves") ? SHOW_MODE_WAVES :
                !strcmp(arg, "rdft") ? SHOW_MODE_RDFT :
                parse_number_or_die(opt, arg, OPT_INT, 0, SHOW_MODE_NB - 1);*/
    return 0;
}

static void opt_input_file(void *optctx, const char *filename) {
    if (input_filename) {
        av_log(NULL, AV_LOG_FATAL,
               "Argument '%s' provided as input filename, but '%s' was already specified.\n",
               filename, input_filename);
        exit(1);
    }
    if (!strcmp(filename, "-"))
        filename = "pipe:";
    input_filename = filename;
}

static int opt_codec(void *optctx, const char *opt, const char *arg) {
    const char *spec = strchr(opt, ':');
    if (!spec) {
        av_log(NULL, AV_LOG_ERROR,
               "No media specifier was specified in '%s' in option '%s'\n",
               arg, opt);
        return AVERROR(EINVAL);
    }
    spec++;
    switch (spec[0]) {
        case 'a' :
            audio_codec_name = arg;
            break;
        case 's' :
            subtitle_codec_name = arg;
            break;
        case 'v' :
            video_codec_name = arg;
            break;
        default:
            av_log(NULL, AV_LOG_ERROR,
                   "Invalid media specifier '%s' in option '%s'\n", spec, opt);
            return AVERROR(EINVAL);
    }
    return 0;
}

static double get_clock(Clock *c) {
    //printf("get_clock() serial: %d, queue_serial: %d\n", c->serial, *(c->queue_serial));
    if (*(c->queue_serial) != c->serial) {
        return NAN;
    }
    if (c->paused) {
        return c->pts;
    } else {
        double time = av_gettime_relative() / 1000000.0;
        return c->pts_drift + time - (time - c->last_updated) * (1.0 - c->speed);
    }
}

static int get_master_sync_type(VideoState *is) {
    if (is->av_sync_type == AV_SYNC_VIDEO_MASTER) {
        if (is->video_st)
            return AV_SYNC_VIDEO_MASTER;
        else
            return AV_SYNC_AUDIO_MASTER;
    } else if (is->av_sync_type == AV_SYNC_AUDIO_MASTER) {
        if (is->audio_st)
            return AV_SYNC_AUDIO_MASTER;
        else
            return AV_SYNC_EXTERNAL_CLOCK;
    } else {
        return AV_SYNC_EXTERNAL_CLOCK;
    }
}

/* get the current master clock value */
static double get_master_clock(VideoState *is) {
    double val;
    switch (get_master_sync_type(is)) {
        case AV_SYNC_VIDEO_MASTER:
            val = get_clock(&is->vidClock);
            break;
        case AV_SYNC_AUDIO_MASTER:
            val = get_clock(&is->audClock);
            break;
        default:
            val = get_clock(&is->extClock);
            break;
    }
    return val;
}

static void set_clock_at(Clock *c, double pts, int serial, double time) {
    c->pts = pts;
    c->serial = serial;
    c->last_updated = time;
    c->pts_drift = c->pts - time;
}

static void set_clock(Clock *c, double pts, int serial) {
    double time = av_gettime_relative() / 1000000.0;
    set_clock_at(c, pts, serial, time);
}

static void set_clock_speed(Clock *c, double speed) {
    set_clock(c, get_clock(c), c->serial);
    c->speed = speed;
    printf("set_clock_speed() speed: %lf\n", speed);
}

static void init_clock(Clock *c, int *queue_serial) {
    c->queue_serial = queue_serial;
    c->speed = 1.0;
    c->paused = 0;
    set_clock(c, NAN, -1);
}

/***
1、外部时钟pts非法，从属时钟（音频/视频）的pts有效时更新。
2、外部时钟pts与从属时钟的时间差值超过AV_NOSYNC_THRESHOLD（10秒），则对外部时钟进行更新。
 */
static void sync_clock_to_slave(Clock *c, Clock *slave) {
    double clock = get_clock(c);
    double slave_clock = get_clock(slave);
    /* 仅当下列条件满足时，才会更新外部时钟 */
    if (!isnan(slave_clock)
        && (isnan(clock) || fabs(clock - slave_clock) > AV_NOSYNC_THRESHOLD)) {
        set_clock(c, slave_clock, slave->serial);
    }
}

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial) {
    AVPacketNode *node;
    int ret;

    pthread_mutex_lock(&q->pMutex);

    for (;;) {
        if (q->abort_request) {
            ret = -1;
            break;
        }

        node = q->first_pkt;
        if (node) {
            q->first_pkt = node->next;
            if (!q->first_pkt)
                q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= node->pkt.size + sizeof(*node);
            q->duration -= node->pkt.duration;
            *pkt = node->pkt;
            if (serial)
                *serial = node->serial;
            av_free(node);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            pthread_cond_wait(&q->pCond, &q->pMutex);
        }
    }
    pthread_mutex_unlock(&q->pMutex);
    return ret;
}

static int packet_queue_put_private(PacketQueue *q, AVPacket *pkt) {
    AVPacketNode *node;

    if (q->abort_request)
        return -1;

    node = (AVPacketNode *) av_malloc(sizeof(AVPacketNode));
    if (!node)
        return -1;

    node->pkt = *pkt;
    node->next = NULL;
    // PacketQueue中的第一个node的pkt是flush_pkt
    if (pkt == &flush_pkt) {
        q->serial++;
        printf("packet_queue_put_private() flush_pkt q->serial: %d\n", q->serial);
    }
    node->serial = q->serial;

    if (!q->last_pkt)
        q->first_pkt = node;
    else
        q->last_pkt->next = node;
    q->last_pkt = node;
    q->nb_packets++;
    q->size += node->pkt.size + sizeof(*node);
    q->duration += node->pkt.duration;
    /* XXX: should duplicate packet data in DV case */
    pthread_cond_signal(&q->pCond);
    return 0;
}

static int packet_queue_put(PacketQueue *q, AVPacket *pkt) {
    int ret;
    pthread_mutex_lock(&q->pMutex);
    ret = packet_queue_put_private(q, pkt);
    pthread_mutex_unlock(&q->pMutex);
    if (pkt != &flush_pkt && ret < 0) {
        av_packet_unref(pkt);
    }
    return ret;
}

/* packet queue handling */
static int packet_queue_init(PacketQueue *q) {
    memset(q, 0, sizeof(PacketQueue));
    pthread_mutex_init(&q->pMutex, NULL);
    pthread_cond_init(&q->pCond, NULL);
    q->abort_request = 1;
    return 0;
}

// 队列清空
static void packet_queue_flush(PacketQueue *q) {
    AVPacketNode *pkt, *pkt1;
    pthread_mutex_lock(&q->pMutex);
    for (pkt = q->first_pkt; pkt; pkt = pkt1) {
        pkt1 = pkt->next;
        av_packet_unref(&pkt->pkt);
        av_freep(&pkt);
    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
    q->duration = 0;
    pthread_mutex_unlock(&q->pMutex);
}

static void packet_queue_destroy(PacketQueue *q) {
    packet_queue_flush(q);
    pthread_mutex_destroy(&q->pMutex);
    pthread_cond_destroy(&q->pCond);
}

static void packet_queue_abort(PacketQueue *q) {
    pthread_mutex_lock(&q->pMutex);
    q->abort_request = 1;
    pthread_cond_signal(&q->pCond);
    pthread_mutex_unlock(&q->pMutex);
}

static int packet_queue_put_nullpacket(PacketQueue *q, int stream_index) {
    AVPacket pkt1, *pkt = &pkt1;
    av_init_packet(pkt);
    pkt->data = NULL;
    pkt->size = 0;
    pkt->stream_index = stream_index;
    return packet_queue_put(q, pkt);
}

// 视频,音频,字幕都会调一次
static void packet_queue_start(PacketQueue *q) {
    printf("packet_queue_start() start\n");
    pthread_mutex_lock(&q->pMutex);
    q->abort_request = 0;
    packet_queue_put_private(q, &flush_pkt);
    pthread_mutex_unlock(&q->pMutex);
    printf("packet_queue_start() end\n");
}

static void frame_queue_unref_item(Frame *vp) {
    av_frame_unref(vp->frame);
    avsubtitle_free(&vp->sub);
}

static int frame_queue_init(FrameQueue *f, PacketQueue *pktq, int max_size, int keep_last) {
    memset(f, 0, sizeof(FrameQueue));
    pthread_mutex_init(&f->pMutex, NULL);
    pthread_cond_init(&f->pCond, NULL);
    f->pktQ = pktq;
    f->max_size = FFMIN(max_size, FRAME_QUEUE_SIZE);
    f->keep_last = !!keep_last;
    printf("frame_queue_init() max_size: %d, keep_last: %d\n", f->max_size, f->keep_last);
    int i;
    for (i = 0; i < f->max_size; i++) {
        // 为每一个AVFrame指针申请空间
        if (!(f->queue[i].frame = av_frame_alloc())) {
            return AVERROR(ENOMEM);
        }
    }
    return 0;
}

static void frame_queue_destory(FrameQueue *f) {
    int i;
    for (i = 0; i < f->max_size; i++) {
        Frame *vp = &f->queue[i];
        frame_queue_unref_item(vp);
        av_frame_free(&vp->frame);
    }
    pthread_mutex_destroy(&f->pMutex);
    pthread_cond_destroy(&f->pCond);
}

static void frame_queue_signal(FrameQueue *f) {
    pthread_mutex_lock(&f->pMutex);
    pthread_cond_signal(&f->pCond);
    pthread_mutex_unlock(&f->pMutex);
}

static Frame *frame_queue_peek(FrameQueue *f) {
    return &f->queue[(f->read_index + f->rindex_shown) % f->max_size];
}

static Frame *frame_queue_peek_next(FrameQueue *f) {
    return &f->queue[(f->read_index + f->rindex_shown + 1) % f->max_size];
}

// 指向最后一次使用过的Frame
static Frame *frame_queue_peek_last(FrameQueue *f) {
    return &f->queue[f->read_index];
}

static Frame *frame_queue_peek_writable(FrameQueue *f) {
    /* wait until we have space to put a new frame */
    pthread_mutex_lock(&f->pMutex);
    while (f->size >= f->max_size && !f->pktQ->abort_request) {
        pthread_cond_wait(&f->pCond, &f->pMutex);
    }
    pthread_mutex_unlock(&f->pMutex);

    if (f->pktQ->abort_request) {
        return NULL;
    }

    return &f->queue[f->write_index];
}

static Frame *frame_queue_peek_readable(FrameQueue *f) {
    /* wait until we have a readable a new frame */
    pthread_mutex_lock(&f->pMutex);
    while (f->size - f->rindex_shown <= 0 && !f->pktQ->abort_request) {
        pthread_cond_wait(&f->pCond, &f->pMutex);
    }
    pthread_mutex_unlock(&f->pMutex);

    if (f->pktQ->abort_request) {
        return NULL;
    }

    return &f->queue[(f->read_index + f->rindex_shown) % f->max_size];
}

static void frame_queue_push(FrameQueue *f) {
    if (++f->write_index == f->max_size) {
        f->write_index = 0;
    }
    pthread_mutex_lock(&f->pMutex);
    f->size++;
    pthread_cond_signal(&f->pCond);
    pthread_mutex_unlock(&f->pMutex);
}

static void frame_queue_next(FrameQueue *f) {
    if (f->keep_last && !f->rindex_shown) {
        f->rindex_shown = 1;
        return;
    }
    frame_queue_unref_item(&f->queue[f->read_index]);
    if (++f->read_index == f->max_size) {
        f->read_index = 0;
    }
    pthread_mutex_lock(&f->pMutex);
    f->size--;
    pthread_cond_signal(&f->pCond);
    pthread_mutex_unlock(&f->pMutex);
}

/* return the number of undisplayed frames in the queue */
static int frame_queue_nb_remaining(FrameQueue *f) {
    return f->size - f->rindex_shown;
}

/* return last shown position */
static int64_t frame_queue_last_pos(FrameQueue *f) {
    Frame *fp = &f->queue[f->read_index];
    if (f->rindex_shown && fp->serial == f->pktQ->serial)
        return fp->pkt_pos;
    else
        return -1;
}

static void decoder_init(Decoder *d,
                         AVCodecContext *avctx,
                         PacketQueue *queue,
                         pthread_cond_t *empty_queue_cond) {
    memset(d, 0, sizeof(Decoder));
    d->avctx = avctx;
    d->queue = queue;
    d->empty_queue_cond = empty_queue_cond;
    d->start_pts = AV_NOPTS_VALUE;
    d->pkt_serial = -1;
}

// 创建音频,视频,字幕三个不同的线程
static int decoder_start(Decoder *d, void *(*fn)(void *), const char *thread_name, void *arg) {
    packet_queue_start(d->queue);
    int ret = pthread_create(&d->decoder_thread, NULL, fn, arg);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "decoder_start() pthread_create(): %s\n", strerror(ret));
        return AVERROR(ENOMEM);
    }
    return 0;
}

/***
 * decodedAVFrame->pts会被修改,这个很关键
 * @param d
 * @param decodedAVFrame 解码后的帧(Subtitle时为NULL)
 * @param avSubtitle     音视频时为NULL
 * @return
 */
static int decoder_decode_frame(Decoder *d, AVFrame *decodedAVFrame, AVSubtitle *avSubtitle) {
    int ret = AVERROR(EAGAIN);

    for (;;) {
        AVPacket pkt;
        // avcodec_receive_frame
        if (d->queue->serial == d->pkt_serial) {
            do {
                if (d->queue->abort_request) {
                    if (d->avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                        printf("decoder_decode_frame() abort_request return\n");
                    }
                    return -1;
                }
                switch (d->avctx->codec_type) {
                    case AVMEDIA_TYPE_VIDEO:
                        ret = avcodec_receive_frame(d->avctx, decodedAVFrame);
                        if (ret >= 0) {
                            if (decoder_reorder_pts == -1) {
                                decodedAVFrame->pts = decodedAVFrame->best_effort_timestamp;
                                //printf("decoder_decode_frame() pts1: %ld\n", decodedAVFrame->pts);
                            } else if (!decoder_reorder_pts) {
                                decodedAVFrame->pts = decodedAVFrame->pkt_dts;
                                printf("decoder_decode_frame() pts2: %ld\n", decodedAVFrame->pts);
                            }
                        }
                        break;
                    case AVMEDIA_TYPE_AUDIO:
                        ret = avcodec_receive_frame(d->avctx, decodedAVFrame);
                        if (ret >= 0) {
                            AVRational tb = (AVRational) {1, decodedAVFrame->sample_rate};
                            if (decodedAVFrame->pts != AV_NOPTS_VALUE) {
                                decodedAVFrame->pts = av_rescale_q(decodedAVFrame->pts, d->avctx->pkt_timebase, tb);
                            } else if (d->next_pts != AV_NOPTS_VALUE) {
                                decodedAVFrame->pts = av_rescale_q(d->next_pts, d->next_pts_tb, tb);
                            }
                            if (decodedAVFrame->pts != AV_NOPTS_VALUE) {
                                d->next_pts = decodedAVFrame->pts + decodedAVFrame->nb_samples;
                                d->next_pts_tb = tb;
                            }
                        }
                        /*printf("decoder_decode_frame() avcodec_receive_frame AV_NOPTS_VALUE: %ld\n",
                               (long) AV_NOPTS_VALUE);
                        printf("decoder_decode_frame() avcodec_receive_frame pts: %ld, next_pts: %ld\n",
                               (long) decodedAVFrame->pts, (long) d->next_pts);*/
                        break;
                }
                if (ret == AVERROR_EOF) {// AVERROR_EOF: -541478725
                    d->finished = d->pkt_serial;
                    avcodec_flush_buffers(d->avctx);
                    return 0;
                }
                if (ret >= 0) {
                    return 1;
                }
            } while (ret != AVERROR(EAGAIN));// AVERROR(EAGAIN): -35
        }
        // 从队列中取出一个AVPacket
        do {
            if (d->queue->nb_packets == 0) {
                //printf("decoder_decode_frame() pthread_cond_signal\n");
                pthread_cond_signal(d->empty_queue_cond);
            }
            if (!d->packet_pending) {
                ret = packet_queue_get(d->queue, &pkt, 1, &d->pkt_serial);
                // 上面代码执行后,d->pkt_serial为1
                /*if (d->avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                    printf("decoder_decode_frame() packet_queue_get ret     : %d\n", ret);
                }*/
                if (ret < 0) {
                    return -1;
                }
            } else {
                av_packet_move_ref(&pkt, &d->pkt);
                d->packet_pending = 0;
            }
            if (d->queue->serial == d->pkt_serial) {
                break;
            }
            av_packet_unref(&pkt);
        } while (1);
        // avcodec_send_packet
        if (pkt.data != flush_pkt.data) {
            if (d->avctx->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                ret = avcodec_send_packet(d->avctx, &pkt);
                /*if (d->avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                    printf("decoder_decode_frame() avcodec_send_packet   ret: %d\n", ret);
                }*/
                if (ret == AVERROR(EAGAIN)) {
                    av_log(d->avctx, AV_LOG_ERROR,
                           "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
                    d->packet_pending = 1;
                    av_packet_move_ref(&d->pkt, &pkt);
                }
            } else {
                // 字幕时
                printf("decoder_decode_frame() AVMEDIA_TYPE_SUBTITLE\n");
                int got_frame = 0;
                ret = avcodec_decode_subtitle2(d->avctx, avSubtitle, &got_frame, &pkt);
                if (ret < 0) {
                    ret = AVERROR(EAGAIN);
                } else {
                    if (got_frame && !pkt.data) {
                        d->packet_pending = 1;
                        av_packet_move_ref(&d->pkt, &pkt);
                    }
                    ret = got_frame ? 0 : (pkt.data ? AVERROR(EAGAIN) : AVERROR_EOF);
                }
            }
            av_packet_unref(&pkt);
        } else {
            // 队列中取出第一个AVPacket时
            avcodec_flush_buffers(d->avctx);
            d->finished = 0;
            d->next_pts = d->start_pts;// -9223372036854775808
            d->next_pts_tb = d->start_pts_tb;
            /*if (d->avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                printf("decoder_decode_frame() start_pts : %ld\n", (long) d->start_pts);
            }*/
        }
    }// for (;;) end
}

static void decoder_destroy(Decoder *d) {
    av_packet_unref(&d->pkt);
    avcodec_free_context(&d->avctx);
}

static void decoder_abort(Decoder *d, FrameQueue *fq) {
    packet_queue_abort(d->queue);
    frame_queue_signal(fq);
    pthread_join(d->decoder_thread, NULL);
    packet_queue_flush(d->queue);
}


#endif //FFMPEG_STUDY_FFPLAY_H
