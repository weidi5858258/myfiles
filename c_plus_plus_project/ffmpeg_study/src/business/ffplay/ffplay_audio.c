/*
 * Copyright (c) 2003 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * simple media player based on the FFmpeg libraries
 */

#include "ffplay.h"

#include <SDL.h>
#include <SDL_thread.h>

/* Minimum SDL audio buffer size, in samples. */
#define SDL_AUDIO_MIN_BUFFER_SIZE 512
/* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30
/* Step size for volume control in dB */
#define SDL_VOLUME_STEP (0.75)
#define FF_QUIT_EVENT   (SDL_USEREVENT + 2)

//static int screen_left = SDL_WINDOWPOS_CENTERED;
//static int screen_top = SDL_WINDOWPOS_CENTERED;
//static SDL_Window *window;
//static SDL_Renderer *renderer;
//static SDL_RendererInfo renderer_info = {0};
static SDL_AudioDeviceID audio_device_id;
//static SDL_Texture *vis_texture;
//static SDL_Texture *sub_texture;
//static SDL_Texture *vid_texture;

static const struct TextureFormatEntry {
    enum AVPixelFormat format;
    int texture_fmt;
} sdl_texture_format_map[] = {
        {AV_PIX_FMT_RGB8,           SDL_PIXELFORMAT_RGB332},
        {AV_PIX_FMT_RGB444,         SDL_PIXELFORMAT_RGB444},
        {AV_PIX_FMT_RGB555,         SDL_PIXELFORMAT_RGB555},
        {AV_PIX_FMT_BGR555,         SDL_PIXELFORMAT_BGR555},
        {AV_PIX_FMT_RGB565,         SDL_PIXELFORMAT_RGB565},
        {AV_PIX_FMT_BGR565,         SDL_PIXELFORMAT_BGR565},
        {AV_PIX_FMT_RGB24,          SDL_PIXELFORMAT_RGB24},
        {AV_PIX_FMT_BGR24,          SDL_PIXELFORMAT_BGR24},
        {AV_PIX_FMT_0RGB32,         SDL_PIXELFORMAT_RGB888},
        {AV_PIX_FMT_0BGR32,         SDL_PIXELFORMAT_BGR888},
        {AV_PIX_FMT_NE(RGB0, 0BGR), SDL_PIXELFORMAT_RGBX8888},
        {AV_PIX_FMT_NE(BGR0, 0RGB), SDL_PIXELFORMAT_BGRX8888},
        {AV_PIX_FMT_RGB32,          SDL_PIXELFORMAT_ARGB8888},
        {AV_PIX_FMT_RGB32_1,        SDL_PIXELFORMAT_RGBA8888},
        {AV_PIX_FMT_BGR32,          SDL_PIXELFORMAT_ABGR8888},
        {AV_PIX_FMT_BGR32_1,        SDL_PIXELFORMAT_BGRA8888},
        {AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_IYUV},
        {AV_PIX_FMT_YUYV422,        SDL_PIXELFORMAT_YUY2},
        {AV_PIX_FMT_UYVY422,        SDL_PIXELFORMAT_UYVY},
        {AV_PIX_FMT_NONE,           SDL_PIXELFORMAT_UNKNOWN},
};

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
static int decoder_start(Decoder *d, int (*fn)(void *), const char *thread_name, void *arg) {
    packet_queue_start(d->queue);
    int ret = pthread_create(&d->decoder_thread, NULL, fn, arg);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "decoder_start() pthread_create(): %s\n", strerror(ret));
        return AVERROR(ENOMEM);
    }
    return 0;
}

/***
 *
 * @param d
 * @param frame      解码后的帧.Subtitle时为NULL
 * @param avSubtitle 音视频时为NULL
 * @return
 */
static int decoder_decode_frame(Decoder *d, AVFrame *frame, AVSubtitle *avSubtitle) {
    int ret = AVERROR(EAGAIN);

    for (;;) {
        AVPacket pkt;

        if (d->queue->serial == d->pkt_serial) {
            do {
                if (d->queue->abort_request)
                    return -1;

                switch (d->avctx->codec_type) {
                    case AVMEDIA_TYPE_VIDEO:
                        ret = avcodec_receive_frame(d->avctx, frame);
                        if (ret >= 0) {
                            if (decoder_reorder_pts == -1) {
                                frame->pts = frame->best_effort_timestamp;
                            } else if (!decoder_reorder_pts) {
                                frame->pts = frame->pkt_dts;
                            }
                        }
                        break;
                    case AVMEDIA_TYPE_AUDIO:
                        ret = avcodec_receive_frame(d->avctx, frame);
                        if (ret >= 0) {
                            AVRational tb = (AVRational) {1, frame->sample_rate};
                            if (frame->pts != AV_NOPTS_VALUE)
                                frame->pts = av_rescale_q(frame->pts, d->avctx->pkt_timebase, tb);
                            else if (d->next_pts != AV_NOPTS_VALUE)
                                frame->pts = av_rescale_q(d->next_pts, d->next_pts_tb, tb);
                            if (frame->pts != AV_NOPTS_VALUE) {
                                d->next_pts = frame->pts + frame->nb_samples;
                                d->next_pts_tb = tb;
                            }
                        }
                        break;
                }
                if (ret == AVERROR_EOF) {
                    d->finished = d->pkt_serial;
                    avcodec_flush_buffers(d->avctx);
                    return 0;
                }
                if (ret >= 0)
                    return 1;
            } while (ret != AVERROR(EAGAIN));
        }

        do {
            if (d->queue->nb_packets == 0) {
                //printf("decoder_decode_frame() pthread_cond_signal\n");
                pthread_cond_signal(d->empty_queue_cond);
            }
            if (d->packet_pending) {
                av_packet_move_ref(&pkt, &d->pkt);
                d->packet_pending = 0;
            } else {
                if (packet_queue_get(d->queue, &pkt, 1, &d->pkt_serial) < 0)
                    return -1;
            }
        } while (d->queue->serial != d->pkt_serial);

        if (pkt.data == flush_pkt.data) {
            avcodec_flush_buffers(d->avctx);
            d->finished = 0;
            d->next_pts = d->start_pts;
            d->next_pts_tb = d->start_pts_tb;
        } else {
            if (d->avctx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
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
            } else {
                if (avcodec_send_packet(d->avctx, &pkt) == AVERROR(EAGAIN)) {
                    av_log(d->avctx, AV_LOG_ERROR,
                           "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
                    d->packet_pending = 1;
                    av_packet_move_ref(&d->pkt, &pkt);
                }
            }
            av_packet_unref(&pkt);
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

static void calculate_display_rect(SDL_Rect *rect,
                                   int scr_xleft, int scr_ytop, int scr_width, int scr_height,
                                   int pic_width, int pic_height, AVRational pic_sar) {
    AVRational aspect_ratio = pic_sar;
    int64_t width, height, x, y;

    if (av_cmp_q(aspect_ratio, av_make_q(0, 1)) <= 0)
        aspect_ratio = av_make_q(1, 1);

    aspect_ratio = av_mul_q(aspect_ratio, av_make_q(pic_width, pic_height));

    /* XXX: we suppose the screen has a 1.0 pixel ratio */
    height = scr_height;
    width = av_rescale(height, aspect_ratio.num, aspect_ratio.den) & ~1;
    if (width > scr_width) {
        width = scr_width;
        height = av_rescale(width, aspect_ratio.den, aspect_ratio.num) & ~1;
    }
    x = (scr_width - width) / 2;
    y = (scr_height - height) / 2;
    rect->x = scr_xleft + x;
    rect->y = scr_ytop + y;
    rect->w = FFMAX((int) width, 1);
    rect->h = FFMAX((int) height, 1);
}

static void stream_component_close(VideoState *is, int stream_index) {
    AVFormatContext *ic = is->avFormatContext;
    AVCodecParameters *codecpar;

    if (stream_index < 0 || stream_index >= ic->nb_streams)
        return;
    codecpar = ic->streams[stream_index]->codecpar;

    switch (codecpar->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            decoder_abort(&is->audDecoder, &is->sampFQ);
            SDL_CloseAudioDevice(audio_device_id);
            decoder_destroy(&is->audDecoder);
            swr_free(&is->swr_ctx);
            av_freep(&is->audio_buf1);
            is->audio_buf1_size = 0;
            is->audio_buf = NULL;

            if (is->rdft) {
                av_rdft_end(is->rdft);
                av_freep(&is->rdft_data);
                is->rdft = NULL;
                is->rdft_bits = 0;
            }
            break;
        case AVMEDIA_TYPE_VIDEO:
            decoder_abort(&is->vidDecoder, &is->pictFQ);
            decoder_destroy(&is->vidDecoder);
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            decoder_abort(&is->subDecoder, &is->subpFQ);
            decoder_destroy(&is->subDecoder);
            break;
        default:
            break;
    }

    ic->streams[stream_index]->discard = AVDISCARD_ALL;
    switch (codecpar->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            is->audio_st = NULL;
            is->audio_stream = -1;
            break;
        case AVMEDIA_TYPE_VIDEO:
            is->video_st = NULL;
            is->video_stream = -1;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            is->subtitle_st = NULL;
            is->subtitle_stream = -1;
            break;
        default:
            break;
    }
}

static void stream_close(VideoState *is) {
    printf("stream_close() start\n");
    /* XXX: use a special url_shutdown call to abort parse cleanly */
    // 为了退出读线程
    is->abort_request = 1;
    // 等到读线程退出
    pthread_join(is->read_thread, NULL);

    /* close each stream */
    if (is->audio_stream >= 0)
        stream_component_close(is, is->audio_stream);
    if (is->video_stream >= 0)
        stream_component_close(is, is->video_stream);
    if (is->subtitle_stream >= 0)
        stream_component_close(is, is->subtitle_stream);

    avformat_close_input(&is->avFormatContext);

    packet_queue_destroy(&is->videoPQ);
    packet_queue_destroy(&is->audioPQ);
    packet_queue_destroy(&is->subtitlePQ);

    /* free all pictures */
    frame_queue_destory(&is->pictFQ);
    frame_queue_destory(&is->sampFQ);
    frame_queue_destory(&is->subpFQ);
    pthread_mutex_destroy(&is->continue_read_thread_mutex);
    pthread_cond_destroy(&is->continue_read_thread_cond);
    sws_freeContext(is->img_convert_ctx);
    sws_freeContext(is->sub_convert_ctx);
    av_free(is->filename);
    av_free(is);
    printf("stream_close() end\n");
}

static void do_exit(VideoState *is) {
    printf("do_exit() start\n");
    if (is) {
        stream_close(is);
    }
    uninit_opts();
#if CONFIG_AVFILTER
    av_freep(&vfilters_list);
#endif
    avformat_network_deinit();
    if (show_status) {
        printf("\n");
    }
    SDL_Quit();
    av_log(NULL, AV_LOG_QUIET, "%s", "SDL_Quit()");
    printf("do_exit() end\n");
    exit(0);
}

static void sigterm_handler(int sig) {
    printf("sigterm_handler() sig: %d\n", sig);
    exit(123);
}

static void set_default_window_size(int width, int height, AVRational sar) {
    SDL_Rect rect;
    int max_width = screen_width ? screen_width : INT_MAX;
    int max_height = screen_height ? screen_height : INT_MAX;
    if (max_width == INT_MAX && max_height == INT_MAX)
        max_height = height;
    calculate_display_rect(&rect, 0, 0, max_width, max_height, width, height, sar);
    default_width = rect.w;
    default_height = rect.h;
}

static void check_external_clock_speed(VideoState *is) {
    if (is->video_stream >= 0 && is->videoPQ.nb_packets <= EXTERNAL_CLOCK_MIN_FRAMES ||
        is->audio_stream >= 0 && is->audioPQ.nb_packets <= EXTERNAL_CLOCK_MIN_FRAMES) {
        set_clock_speed(&is->extClock, FFMAX(EXTERNAL_CLOCK_SPEED_MIN, is->extClock.speed - EXTERNAL_CLOCK_SPEED_STEP));
    } else if ((is->video_stream < 0 || is->videoPQ.nb_packets > EXTERNAL_CLOCK_MAX_FRAMES) &&
               (is->audio_stream < 0 || is->audioPQ.nb_packets > EXTERNAL_CLOCK_MAX_FRAMES)) {
        set_clock_speed(&is->extClock, FFMIN(EXTERNAL_CLOCK_SPEED_MAX, is->extClock.speed + EXTERNAL_CLOCK_SPEED_STEP));
    } else {
        double speed = is->extClock.speed;
        if (speed != 1.0) {
            set_clock_speed(&is->extClock, speed + EXTERNAL_CLOCK_SPEED_STEP * (1.0 - speed) / fabs(1.0 - speed));
        }
    }
}

/* seek in the stream */
static void stream_seek(VideoState *is, int64_t pos, int64_t rel, int seek_by_bytes) {
    if (!is->seek_req) {
        is->seek_pos = pos;
        is->seek_rel = rel;
        is->seek_flags &= ~AVSEEK_FLAG_BYTE;
        if (seek_by_bytes)
            is->seek_flags |= AVSEEK_FLAG_BYTE;
        is->seek_req = 1;
        printf("stream_seek() pthread_cond_signal\n");
        pthread_cond_signal(&is->continue_read_thread_cond);
    }
}

/* pause or resume the video */
static void stream_toggle_pause(VideoState *is) {
    if (is->paused) {
        is->frame_timer += av_gettime_relative() / 1000000.0 - is->vidClock.last_updated;
        if (is->read_pause_return != AVERROR(ENOSYS)) {
            is->vidClock.paused = 0;
        }
        set_clock(&is->vidClock, get_clock(&is->vidClock), is->vidClock.serial);
    }
    set_clock(&is->extClock, get_clock(&is->extClock), is->extClock.serial);
    is->paused = is->audClock.paused = is->vidClock.paused = is->extClock.paused = !is->paused;
}

static void toggle_pause(VideoState *is) {
    stream_toggle_pause(is);
    is->step = 0;
}

static void toggle_mute(VideoState *is) {
    is->muted = !is->muted;
}

static void update_volume(VideoState *is, int sign, double step) {
    double volume_level = is->audio_volume ? (20 * log(is->audio_volume / (double) SDL_MIX_MAXVOLUME) / log(10))
                                           : -1000.0;
    int new_volume = lrint(SDL_MIX_MAXVOLUME * pow(10.0, (volume_level + sign * step) / 20.0));
    is->audio_volume = av_clip(is->audio_volume == new_volume ? (is->audio_volume + sign) : new_volume, 0,
                               SDL_MIX_MAXVOLUME);
}

static void step_to_next_frame(VideoState *is) {
    /* if the stream is paused unpause it, then step */
    if (is->paused)
        stream_toggle_pause(is);
    is->step = 1;
}

static double compute_target_delay(double delay, VideoState *is) {
    double sync_threshold, diff = 0;

    /* update delay to follow master synchronisation source */
    if (get_master_sync_type(is) != AV_SYNC_VIDEO_MASTER) {
        /* if video is slave, we try to correct big delays by
           duplicating or deleting a frame */
        diff = get_clock(&is->vidClock) - get_master_clock(is);

        /* skip or repeat frame. We take into account the
           delay to compute the threshold. I still don't know
           if it is the best guess */
        sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, delay));
        if (!isnan(diff) && fabs(diff) < is->max_frame_duration) {
            if (diff <= -sync_threshold)
                delay = FFMAX(0, delay + diff);
            else if (diff >= sync_threshold && delay > AV_SYNC_FRAMEDUP_THRESHOLD)
                delay = delay + diff;
            else if (diff >= sync_threshold)
                delay = 2 * delay;
        }
    }

    av_log(NULL, AV_LOG_TRACE, "video: delay=%0.3f A-V=%f\n",
           delay, -diff);

    return delay;
}

static double vp_duration(VideoState *is, Frame *vp, Frame *nextvp) {
    if (vp->serial == nextvp->serial) {
        double duration = nextvp->pts - vp->pts;
        if (isnan(duration) || duration <= 0 || duration > is->max_frame_duration)
            return vp->duration;
        else
            return duration;
    } else {
        return 0.0;
    }
}

#if CONFIG_AVFILTER

static int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
                                 AVFilterContext *source_ctx, AVFilterContext *sink_ctx) {
    int ret, i;
    int nb_filters = graph->nb_filters;
    AVFilterInOut *outputs = NULL, *inputs = NULL;

    if (filtergraph) {
        outputs = avfilter_inout_alloc();
        inputs = avfilter_inout_alloc();
        if (!outputs || !inputs) {
            ret = AVERROR(ENOMEM);
            goto fail;
        }

        outputs->name = av_strdup("in");
        outputs->filter_ctx = source_ctx;
        outputs->pad_idx = 0;
        outputs->next = NULL;

        inputs->name = av_strdup("out");
        inputs->filter_ctx = sink_ctx;
        inputs->pad_idx = 0;
        inputs->next = NULL;

        if ((ret = avfilter_graph_parse_ptr(graph, filtergraph, &inputs, &outputs, NULL)) < 0)
            goto fail;
    } else {
        if ((ret = avfilter_link(source_ctx, 0, sink_ctx, 0)) < 0)
            goto fail;
    }

    /* Reorder the filters to ensure that inputs of the custom filters are merged first */
    for (i = 0; i < graph->nb_filters - nb_filters; i++)
        FFSWAP(AVFilterContext*, graph->filters[i], graph->filters[i + nb_filters]);

    ret = avfilter_graph_config(graph, NULL);
    fail:
    avfilter_inout_free(&outputs);
    avfilter_inout_free(&inputs);
    return ret;
}

static int configure_audio_filters(VideoState *is, const char *afilters, int force_output_format) {
    static const enum AVSampleFormat sample_fmts[] = {AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE};
    int sample_rates[2] = {0, -1};
    int64_t channel_layouts[2] = {0, -1};
    int channels[2] = {0, -1};
    AVFilterContext *filt_asrc = NULL, *filt_asink = NULL;
    char aresample_swr_opts[512] = "";
    AVDictionaryEntry *e = NULL;
    char asrc_args[256];
    int ret;

    avfilter_graph_free(&is->agraph);
    if (!(is->agraph = avfilter_graph_alloc()))
        return AVERROR(ENOMEM);
    is->agraph->nb_threads = filter_nbthreads;

    while ((e = av_dict_get(swr_opts, "", e, AV_DICT_IGNORE_SUFFIX)))
        av_strlcatf(aresample_swr_opts, sizeof(aresample_swr_opts), "%s=%s:", e->key, e->value);
    if (strlen(aresample_swr_opts))
        aresample_swr_opts[strlen(aresample_swr_opts) - 1] = '\0';
    av_opt_set(is->agraph, "aresample_swr_opts", aresample_swr_opts, 0);

    ret = snprintf(asrc_args, sizeof(asrc_args),
                   "sample_rate=%d:sample_fmt=%s:channels=%d:time_base=%d/%d",
                   is->audio_filter_src.sample_rate, av_get_sample_fmt_name(is->audio_filter_src.sample_fmt),
                   is->audio_filter_src.channels,
                   1, is->audio_filter_src.sample_rate);
    if (is->audio_filter_src.channel_layout)
        snprintf(asrc_args + ret, sizeof(asrc_args) - ret,
                 ":channel_layout=0x%"
                         PRIx64, is->audio_filter_src.channel_layout);

    ret = avfilter_graph_create_filter(&filt_asrc,
                                       avfilter_get_by_name("abuffer"), "ffplay_abuffer",
                                       asrc_args, NULL, is->agraph);
    if (ret < 0)
        goto end;


    ret = avfilter_graph_create_filter(&filt_asink,
                                       avfilter_get_by_name("abuffersink"), "ffplay_abuffersink",
                                       NULL, NULL, is->agraph);
    if (ret < 0)
        goto end;

    if ((ret = av_opt_set_int_list(filt_asink, "sample_fmts", sample_fmts, AV_SAMPLE_FMT_NONE,
                                   AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;
    if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 1, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;

    if (force_output_format) {
        channel_layouts[0] = is->audio_tgt.channel_layout;
        channels[0] = is->audio_tgt.channels;
        sample_rates[0] = is->audio_tgt.sample_rate;
        if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 0, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_layouts", channel_layouts, -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_counts", channels, -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "sample_rates", sample_rates, -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
    }

    if ((ret = configure_filtergraph(is->agraph, afilters, filt_asrc, filt_asink)) < 0)
        goto end;

    is->in_audio_filter = filt_asrc;
    is->out_audio_filter = filt_asink;

    end:
    if (ret < 0)
        avfilter_graph_free(&is->agraph);
    return ret;
}

#endif  /* CONFIG_AVFILTER */

static int audio_thread(void *arg) {
    VideoState *is = arg;
    Frame *af;
    AVRational avRational;
    int got_frame = 0, ret = 0;

#if CONFIG_AVFILTER
    int last_serial = -1;
    int reconfigure;
    int64_t dec_channel_layout;
#endif

    AVFrame *decodedAVFrame = av_frame_alloc();
    if (!decodedAVFrame) {
        return AVERROR(ENOMEM);
    }

    do {
        if ((got_frame = decoder_decode_frame(&is->audDecoder, decodedAVFrame, NULL)) < 0) {
            goto the_end;
        }

        if (got_frame) {
            avRational = (AVRational) {1, decodedAVFrame->sample_rate};

#if CONFIG_AVFILTER
            dec_channel_layout = get_valid_channel_layout(decodedAVFrame->channel_layout, decodedAVFrame->channels);
            reconfigure =
                    cmp_audio_fmts(is->audio_filter_src.sample_fmt, is->audio_filter_src.channels,
                                   decodedAVFrame->format, decodedAVFrame->channels)
                    || is->audio_filter_src.channel_layout != dec_channel_layout
                    || is->audio_filter_src.sample_rate != decodedAVFrame->sample_rate
                    || is->audDecoder.pkt_serial != last_serial;
            if (reconfigure) {
                char buf1[1024], buf2[1024];
                av_get_channel_layout_string(buf1, sizeof(buf1), -1, is->audio_filter_src.channel_layout);
                av_get_channel_layout_string(buf2, sizeof(buf2), -1, dec_channel_layout);
                av_log(NULL, AV_LOG_DEBUG,
                       "Audio frame changed from rate:%d ch:%d fmt:%s layout:%s serial:%d to rate:%d ch:%d fmt:%s layout:%s serial:%d\n",
                       is->audio_filter_src.sample_rate, is->audio_filter_src.channels,
                       av_get_sample_fmt_name(is->audio_filter_src.sample_fmt), buf1, last_serial,
                       decodedAVFrame->sample_rate, decodedAVFrame->channels,
                       av_get_sample_fmt_name(decodedAVFrame->format), buf2,
                       is->audDecoder.pkt_serial);

                is->audio_filter_src.sample_fmt = decodedAVFrame->format;
                is->audio_filter_src.channels = decodedAVFrame->channels;
                is->audio_filter_src.channel_layout = dec_channel_layout;
                is->audio_filter_src.sample_rate = decodedAVFrame->sample_rate;
                last_serial = is->audDecoder.pkt_serial;

                if ((ret = configure_audio_filters(is, afilters, 1)) < 0)
                    goto the_end;
            }

            if ((ret = av_buffersrc_add_frame(is->in_audio_filter, decodedAVFrame)) < 0)
                goto the_end;

            while ((ret = av_buffersink_get_frame_flags(is->out_audio_filter, decodedAVFrame, 0)) >= 0) {
                avRational = av_buffersink_get_time_base(is->out_audio_filter);
#endif
                // 从sampQ中取出一个Frame指针
                if (!(af = frame_queue_peek_writable(&is->sampFQ))) {
                    goto the_end;
                }
                af->pts = (decodedAVFrame->pts == AV_NOPTS_VALUE) ? NAN : decodedAVFrame->pts * av_q2d(avRational);
                af->pos = decodedAVFrame->pkt_pos;
                af->serial = is->audDecoder.pkt_serial;
                // 当前帧包含的(单个声道)采样数/采样率就是当前帧的播放时长
                af->duration = av_q2d(
                        (AVRational) {decodedAVFrame->nb_samples, decodedAVFrame->sample_rate});
                // 将frame数据拷入af->frame,af->frame指向音频frame队列尾部
                av_frame_move_ref(af->frame, decodedAVFrame);
                // 更新音频frame队列大小及写指针
                frame_queue_push(&is->sampFQ);
                //printf("audio_thread() frame_queue_push af->pts: %lf\n", af->pts);
#if CONFIG_AVFILTER
                if (is->audioPQ.serial != is->audDecoder.pkt_serial) {
                    break;
                }
            }
            if (ret == AVERROR_EOF) {
                is->audDecoder.finished = is->audDecoder.pkt_serial;
            }
#endif
        }// if (got_frame)
    } while (ret >= 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);

    the_end:
#if CONFIG_AVFILTER
    avfilter_graph_free(&is->agraph);
#endif
    av_frame_free(&decodedAVFrame);
    return ret;
}

/* copy samples for viewing in editor window */
static void update_sample_display(VideoState *is, short *samples, int samples_size) {
    int size, len;

    size = samples_size / sizeof(short);
    while (size > 0) {
        len = SAMPLE_ARRAY_SIZE - is->sample_array_index;
        if (len > size)
            len = size;
        memcpy(is->sample_array + is->sample_array_index, samples, len * sizeof(short));
        samples += len;
        is->sample_array_index += len;
        if (is->sample_array_index >= SAMPLE_ARRAY_SIZE)
            is->sample_array_index = 0;
        size -= len;
    }
}

/* return the wanted number of samples to get better sync if sync_type is video
 * or external master clock */
static int synchronize_audio(VideoState *is, int nb_samples) {
    int wanted_nb_samples = nb_samples;

    /* if not master, then we try to remove or add samples to correct the clock */
    if (get_master_sync_type(is) != AV_SYNC_AUDIO_MASTER) {
        double diff, avg_diff;
        int min_nb_samples, max_nb_samples;

        diff = get_clock(&is->audClock) - get_master_clock(is);

        if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD) {
            is->audio_diff_cum = diff + is->audio_diff_avg_coef * is->audio_diff_cum;
            if (is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
                /* not enough measures to have a correct estimate */
                is->audio_diff_avg_count++;
            } else {
                /* estimate the A-V difference */
                avg_diff = is->audio_diff_cum * (1.0 - is->audio_diff_avg_coef);

                if (fabs(avg_diff) >= is->audio_diff_threshold) {
                    wanted_nb_samples = nb_samples + (int) (diff * is->audio_src.sample_rate);
                    min_nb_samples = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100));
                    max_nb_samples = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100));
                    wanted_nb_samples = av_clip(wanted_nb_samples, min_nb_samples, max_nb_samples);
                }
                av_log(NULL, AV_LOG_TRACE, "diff=%f adiff=%f sample_diff=%d apts=%0.3f %f\n",
                       diff, avg_diff, wanted_nb_samples - nb_samples,
                       is->audio_clock, is->audio_diff_threshold);
            }
        } else {
            /* too big difference : may be initial PTS errors, so
               reset A-V filter */
            is->audio_diff_avg_count = 0;
            is->audio_diff_cum = 0;
        }
    }

    return wanted_nb_samples;
}

/**
 * Decode one audio frame and return its uncompressed size.
 *
 * The processed audio frame is decoded, converted if required, and
 * stored in is->audio_buf, with size in bytes given by the return
 * value.
 */
static int audio_decode_frame(VideoState *is) {
    //printf("audio_decode_frame() start\n");
    if (is->paused) {
        return -1;
    }

    int data_size, resampled_data_size, wanted_nb_samples;
    int64_t dec_channel_layout;
    Frame *af;// 从FrameQueue中取出一帧数据

    do {
#if defined(_WIN32)
        while (frame_queue_nb_remaining(&is->sampq) == 0) {
            if ((av_gettime_relative() - audio_callback_time) > 1000000LL * is->audio_hw_buf_size / is->audio_tgt.bytes_per_sec / 2)
                return -1;
            av_usleep (1000);
        }
#endif
        if (!(af = frame_queue_peek_readable(&is->sampFQ))) {
            return -1;
        }
        frame_queue_next(&is->sampFQ);
        //printf("audio_decode_frame() frame_queue_next af->pts: %lf\n", af->pts);
    } while (af->serial != is->audioPQ.serial);

    data_size = av_samples_get_buffer_size(NULL, af->frame->channels,
                                           af->frame->nb_samples, af->frame->format, 1);

    dec_channel_layout =
            (af->frame->channel_layout &&
             af->frame->channels == av_get_channel_layout_nb_channels(af->frame->channel_layout)) ?
            af->frame->channel_layout : av_get_default_channel_layout(af->frame->channels);
    wanted_nb_samples = synchronize_audio(is, af->frame->nb_samples);

    if (af->frame->format != is->audio_src.sample_fmt ||
        dec_channel_layout != is->audio_src.channel_layout ||
        af->frame->sample_rate != is->audio_src.sample_rate ||
        (wanted_nb_samples != af->frame->nb_samples && !is->swr_ctx)) {
        printf("audio_decode_frame() swr_free(&is->swr_ctx)\n");
        swr_free(&is->swr_ctx);
        is->swr_ctx = swr_alloc_set_opts(NULL,
                                         is->audio_tgt.channel_layout, is->audio_tgt.sample_fmt,
                                         is->audio_tgt.sample_rate,
                                         dec_channel_layout, af->frame->format, af->frame->sample_rate,
                                         0, NULL);
        if (!is->swr_ctx || swr_init(is->swr_ctx) < 0) {
            av_log(NULL, AV_LOG_ERROR,
                   "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                   af->frame->sample_rate, av_get_sample_fmt_name(af->frame->format), af->frame->channels,
                   is->audio_tgt.sample_rate, av_get_sample_fmt_name(is->audio_tgt.sample_fmt), is->audio_tgt.channels);
            swr_free(&is->swr_ctx);
            return -1;
        }
        is->audio_src.sample_rate = af->frame->sample_rate;
        is->audio_src.channels = af->frame->channels;
        is->audio_src.channel_layout = dec_channel_layout;
        is->audio_src.sample_fmt = af->frame->format;
    }

    if (is->swr_ctx) {
        //printf("audio_decode_frame() is->swr_ctx is not NULL\n");
        const uint8_t **in = (const uint8_t **) af->frame->extended_data;
        uint8_t **out = &is->audio_buf1;
        int out_count = (int64_t) wanted_nb_samples * is->audio_tgt.sample_rate / af->frame->sample_rate + 256;
        int out_size = av_samples_get_buffer_size(NULL, is->audio_tgt.channels, out_count, is->audio_tgt.sample_fmt, 0);
        int len2;
        if (out_size < 0) {
            av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
            return -1;
        }
        if (wanted_nb_samples != af->frame->nb_samples) {
            if (swr_set_compensation(is->swr_ctx,
                                     (wanted_nb_samples - af->frame->nb_samples) * is->audio_tgt.sample_rate /
                                     af->frame->sample_rate,
                                     wanted_nb_samples * is->audio_tgt.sample_rate / af->frame->sample_rate) < 0) {
                av_log(NULL, AV_LOG_ERROR, "swr_set_compensation() failed\n");
                return -1;
            }
        }
        av_fast_malloc(&is->audio_buf1, &is->audio_buf1_size, out_size);
        if (!is->audio_buf1)
            return AVERROR(ENOMEM);
        len2 = swr_convert(is->swr_ctx, out, out_count, in, af->frame->nb_samples);
        if (len2 < 0) {
            av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
            return -1;
        }
        if (len2 == out_count) {
            av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
            if (swr_init(is->swr_ctx) < 0)
                swr_free(&is->swr_ctx);
        }
        is->audio_buf = is->audio_buf1;
        resampled_data_size = len2 * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.sample_fmt);
    } else {
        //printf("audio_decode_frame() is->swr_ctx is NULL\n");
        is->audio_buf = af->frame->data[0];
        resampled_data_size = data_size;
    }

    av_unused double audio_clock0;
    audio_clock0 = is->audio_clock;
    /* update the audio clock with the pts */
    if (!isnan(af->pts)) {
        is->audio_clock = af->pts + (double) af->frame->nb_samples / af->frame->sample_rate;
        // audio_decode_frame() serial: 1, audio_clock: 0.069660
        // audio_decode_frame() serial: 1, audio_clock: 0.092721
        // printf("audio_decode_frame() serial: %d, audio_clock: %lf\n", af->serial, is->audio_clock);
    } else {
        is->audio_clock = NAN;
    }
    is->audio_clock_serial = af->serial;

#ifdef DEBUG
    {
        static double last_clock;
        printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
               is->audio_clock - last_clock,
               is->audio_clock, audio_clock0);
        last_clock = is->audio_clock;
    }
#endif

    //printf("audio_decode_frame() end\n");
    return resampled_data_size;
}

/***
此函数被SDL按需调用，此函数不在用户主线程中，因此数据需要保护
@param[in]  opaque 用户在注册回调函数时指定的参数
@param[out] stream 音频数据缓冲区地址，将解码后的音频数据填入此缓冲区
@param[out] len    音频数据缓冲区大小，单位字节
回调函数返回后，stream指向的音频缓冲区将变为无效
双声道采样点的顺序为LRLRLR
 */
/* prepare a new audio buffer */
static void sdl_audio_callback(void *opaque, Uint8 *stream, int len) {
    VideoState *is = opaque;
    int audio_size, len1;

    audio_callback_time = av_gettime_relative();

    while (len > 0) {
        if (is->audio_buf_index >= is->audio_buf_size) {
            audio_size = audio_decode_frame(is);
            if (audio_size < 0) {
                /* if error, just output silence */
                is->audio_buf = NULL;
                is->audio_buf_size = SDL_AUDIO_MIN_BUFFER_SIZE / is->audio_tgt.frame_size * is->audio_tgt.frame_size;
            } else {
                if (is->show_mode != SHOW_MODE_VIDEO) {
                    update_sample_display(is, (int16_t *) is->audio_buf, audio_size);
                }
                is->audio_buf_size = audio_size;
            }
            is->audio_buf_index = 0;
        }
        len1 = is->audio_buf_size - is->audio_buf_index;
        if (len1 > len) {
            len1 = len;
        }
        if (!is->muted && is->audio_buf && is->audio_volume == SDL_MIX_MAXVOLUME) {
            memcpy(stream, (uint8_t *) is->audio_buf + is->audio_buf_index, len1);
        } else {
            memset(stream, 0, len1);
            if (!is->muted && is->audio_buf) {
                SDL_MixAudioFormat(stream, (uint8_t *) is->audio_buf + is->audio_buf_index,
                                   AUDIO_S16SYS, len1, is->audio_volume);
            }
        }
        len -= len1;
        stream += len1;
        is->audio_buf_index += len1;
    } // while (len > 0) end

    is->audio_write_buf_size = is->audio_buf_size - is->audio_buf_index;
    /* Let's assume the audio driver that is used by SDL has two periods. */
    if (!isnan(is->audio_clock)) {
        // is->audio_clock的值在audio_decode_frame方法中被赋值
        double pts = is->audio_clock -
                     (double) (2 * is->audio_hw_buf_size + is->audio_write_buf_size) / is->audio_tgt.bytes_per_sec;
        set_clock_at(&is->audClock,
                     pts,
                     is->audio_clock_serial,
                     audio_callback_time / 1000000.0);
        sync_clock_to_slave(&is->extClock, &is->audClock);
        //printf("sdl_audio_callback() pts: %lf, serial: %d\n", pts, is->audio_clock_serial);
    }
}

static int audio_open(void *videoState,
                      int64_t wanted_channel_layout,
                      int wanted_nb_channels,
                      int wanted_sample_rate,
                      struct AudioParams *audio_hw_params) {
    printf("audio_open() start\n");
    SDL_AudioSpec wanted_spec, spec;
    const char *env;
    static const int next_sample_rates[] = {0, 44100, 48000, 96000, 192000};
    static const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
    int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;

    env = SDL_getenv("SDL_AUDIO_CHANNELS");
    printf("audio_open() env: %s\n", env);
    if (env) {
        wanted_nb_channels = atoi(env);
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
    }
    if (!wanted_channel_layout
        || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
    }
    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
    wanted_spec.channels = wanted_nb_channels;
    wanted_spec.freq = wanted_sample_rate;
    if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
        av_log(NULL, AV_LOG_ERROR, "Invalid sample rate or channel count!\n");
        return -1;
    }
    while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_spec.freq) {
        next_sample_rate_idx--;
    }
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE,
                                2 << av_log2(wanted_spec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
    wanted_spec.callback = sdl_audio_callback;
    wanted_spec.userdata = videoState;
    while (!(audio_device_id = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec,
                                                   SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
                                                   SDL_AUDIO_ALLOW_CHANNELS_CHANGE))) {
        av_log(NULL, AV_LOG_WARNING, "SDL_OpenAudio (%d channels, %d Hz): %s\n",
               wanted_spec.channels, wanted_spec.freq, SDL_GetError());
        wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
        if (!wanted_spec.channels) {
            wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
            wanted_spec.channels = wanted_nb_channels;
            if (!wanted_spec.freq) {
                av_log(NULL, AV_LOG_ERROR,
                       "No more combinations to try, audio open failed\n");
                return -1;
            }
        }
        wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
    }
    if (spec.format != AUDIO_S16SYS) {
        av_log(NULL, AV_LOG_ERROR,
               "SDL advised audio format %d is not supported!\n", spec.format);
        return -1;
    }
    if (spec.channels != wanted_spec.channels) {
        wanted_channel_layout = av_get_default_channel_layout(spec.channels);
        if (!wanted_channel_layout) {
            av_log(NULL, AV_LOG_ERROR,
                   "SDL advised channel count %d is not supported!\n", spec.channels);
            return -1;
        }
    }

    audio_hw_params->sample_fmt = AV_SAMPLE_FMT_S16;
    audio_hw_params->sample_rate = spec.freq;
    audio_hw_params->channel_layout = wanted_channel_layout;
    audio_hw_params->channels = spec.channels;
    audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, audio_hw_params->channels, 1,
                                                             audio_hw_params->sample_fmt,
                                                             1);
    audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL, audio_hw_params->channels,
                                                                audio_hw_params->sample_rate,
                                                                audio_hw_params->sample_fmt, 1);
    if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
        return -1;
    }
    printf("audio_open() end\n");
    return spec.size;
}

/* open a given stream. Return 0 if OK */
static int stream_component_open(VideoState *is, int stream_index) {
    if (stream_index < 0 || stream_index >= is->avFormatContext->nb_streams)
        return -1;

    const char *forced_codec_name = NULL;
    AVFormatContext *formatContext = is->avFormatContext;
    AVCodec *deCodec = NULL;
    AVDictionary *dictionary = NULL;
    AVDictionaryEntry *dictionaryEntry = NULL;
    int ret = 0;
    int stream_lowres = lowres;

    AVCodecContext *codecContext = NULL;
    codecContext = avcodec_alloc_context3(NULL);
    if (!codecContext)
        return AVERROR(ENOMEM);

    ret = avcodec_parameters_to_context(codecContext, formatContext->streams[stream_index]->codecpar);
    if (ret < 0)
        goto fail;

    codecContext->pkt_timebase = formatContext->streams[stream_index]->time_base;
    switch (codecContext->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            is->last_audio_stream = stream_index;
            forced_codec_name = audio_codec_name;
            break;
        case AVMEDIA_TYPE_VIDEO:
            is->last_video_stream = stream_index;
            forced_codec_name = video_codec_name;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            is->last_subtitle_stream = stream_index;
            forced_codec_name = subtitle_codec_name;
            break;
    }
    deCodec = avcodec_find_decoder(codecContext->codec_id);
    if (forced_codec_name)
        deCodec = avcodec_find_decoder_by_name(forced_codec_name);
    if (!deCodec) {
        if (forced_codec_name)
            av_log(NULL, AV_LOG_WARNING,
                   "No codec could be found with name '%s'\n", forced_codec_name);
        else
            av_log(NULL, AV_LOG_WARNING,
                   "No decoder could be found for codec %s\n", avcodec_get_name(codecContext->codec_id));
        ret = AVERROR(EINVAL);
        goto fail;
    }

    codecContext->codec_id = deCodec->id;
    if (stream_lowres > deCodec->max_lowres) {
        av_log(codecContext, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
               deCodec->max_lowres);
        stream_lowres = deCodec->max_lowres;
    }
    codecContext->lowres = stream_lowres;

    if (fast)
        codecContext->flags2 |= AV_CODEC_FLAG2_FAST;

    dictionary = filter_codec_opts(codec_opts, codecContext->codec_id, formatContext,
                                   formatContext->streams[stream_index], deCodec);
    if (!av_dict_get(dictionary, "threads", NULL, 0))
        av_dict_set(&dictionary, "threads", "auto", 0);
    if (stream_lowres)
        av_dict_set_int(&dictionary, "lowres", stream_lowres, 0);
    if (codecContext->codec_type == AVMEDIA_TYPE_VIDEO || codecContext->codec_type == AVMEDIA_TYPE_AUDIO)
        av_dict_set(&dictionary, "refcounted_frames", "1", 0);
    if ((ret = avcodec_open2(codecContext, deCodec, &dictionary)) < 0) {
        goto fail;
    }
    if ((dictionaryEntry = av_dict_get(dictionary, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", dictionaryEntry->key);
        ret = AVERROR_OPTION_NOT_FOUND;
        goto fail;
    }

    is->is_eof = 0;
    formatContext->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    switch (codecContext->codec_type) {
        case AVMEDIA_TYPE_AUDIO: {
            int64_t channel_layout;
            int sample_rate, nb_channels;

#if CONFIG_AVFILTER
            {
                AVFilterContext *filterContext;
                is->audio_filter_src.sample_rate = codecContext->sample_rate;
                is->audio_filter_src.channels = codecContext->channels;
                is->audio_filter_src.channel_layout = get_valid_channel_layout(
                        codecContext->channel_layout, codecContext->channels);
                is->audio_filter_src.sample_fmt = codecContext->sample_fmt;
                if ((ret = configure_audio_filters(is, afilters, 0)) < 0)
                    goto fail;

                filterContext = is->out_audio_filter;
                /***
                 sample_rate,nb_channels,channel_layout
                 这三个值为什么要这样去得到?
                 这样filterContext是怎么来的变得重要了.
                 is->out_audio_filter是从configure_audio_filters方法里被赋值的.
                 */
                sample_rate = av_buffersink_get_sample_rate(filterContext);
                nb_channels = av_buffersink_get_channels(filterContext);
                channel_layout = av_buffersink_get_channel_layout(filterContext);
                printf("stream_component_open() sample_rate: %d, nb_channels: %d, channel_layout: %"PRIu64"\n",
                       codecContext->sample_rate, codecContext->channels, codecContext->channel_layout);
                printf("stream_component_open() sample_rate: %d, nb_channels: %d, channel_layout: %"PRIu64"\n",
                       sample_rate, nb_channels, channel_layout);
            }
#else
            sample_rate = codecContext->sample_rate;
            nb_channels = codecContext->channels;
            channel_layout = codecContext->channel_layout;
#endif

            /* prepare audio output */
            if ((ret = audio_open(is, channel_layout, nb_channels, sample_rate, &is->audio_tgt)) < 0) {
                goto fail;
            }

            is->audio_hw_buf_size = ret;
            is->audio_src = is->audio_tgt;
            is->audio_buf_size = 0;
            is->audio_buf_index = 0;
            /* init averaging filter */
            is->audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
            is->audio_diff_avg_count = 0;
            /* since we do not have a precise anough audio FIFO fullness,
               we correct audio sync only if larger than this threshold */
            is->audio_diff_threshold = (double) (is->audio_hw_buf_size) / is->audio_tgt.bytes_per_sec;

            is->audio_stream = stream_index;
            is->audio_st = formatContext->streams[stream_index];

            decoder_init(&is->audDecoder, codecContext, &is->audioPQ, &is->continue_read_thread_cond);
            if ((is->avFormatContext->iformat->flags & (AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH | AVFMT_NO_BYTE_SEEK)) &&
                !is->avFormatContext->iformat->read_seek) {
                is->audDecoder.start_pts = is->audio_st->start_time;
                is->audDecoder.start_pts_tb = is->audio_st->time_base;
            }
            if ((ret = decoder_start(&is->audDecoder, audio_thread, "audio_decoder", is)) < 0) {
                goto out;
            }
            SDL_PauseAudioDevice(audio_device_id, 0);
            printf("stream_component_open() SDL_PauseAudioDevice\n");
            break;
        }
        default:
            break;
    }

    goto out;

    fail:
    avcodec_free_context(&codecContext);
    out:
    av_dict_free(&dictionary);

    return ret;
}

static int decode_interrupt_cb(void *ctx) {
    VideoState *is = ctx;
    return is->abort_request;
}

static int stream_has_enough_packets(AVStream *st, int stream_id, PacketQueue *queue) {
    return stream_id < 0 ||
           queue->abort_request ||
           (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
           queue->nb_packets > MIN_FRAMES && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0);
}

static int is_realtime(AVFormatContext *s) {
    if (!strcmp(s->iformat->name, "rtp")
        || !strcmp(s->iformat->name, "rtsp")
        || !strcmp(s->iformat->name, "sdp")) {
        return 1;
    }
    if (s->pb
        && (!strncmp(s->url, "rtp:", 4) || !strncmp(s->url, "udp:", 4))) {
        return 1;
    }
    return 0;
}

/* this thread gets the stream from the disk or the network */
static int read_thread(void *arg) {
    printf("read_thread() start\n");
    VideoState *is = arg;
    AVPacket pkt1, *pkt = &pkt1;
    int i, err, ret, pkt_in_play_range = 0, scan_all_pmts_set = 0;
    int64_t pkt_ts, stream_start_time;

    // st_index[0] AVMEDIA_TYPE_VIDEO
    // st_index[1] AVMEDIA_TYPE_AUDIO
    // st_index[2] AVMEDIA_TYPE_SUBTITLE
    int stream_index[AVMEDIA_TYPE_NB];
    memset(stream_index, -1, sizeof(stream_index));
    is->last_video_stream = is->video_stream = -1;
    is->last_audio_stream = is->audio_stream = -1;
    is->last_subtitle_stream = is->subtitle_stream = -1;
    is->is_eof = 0;

    AVFormatContext *avFormatContext = NULL;
    avFormatContext = avformat_alloc_context();
    if (!avFormatContext) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    // 中断回调机制。为底层I/O层提供一个处理接口，比如中止IO操作。
    avFormatContext->interrupt_callback.callback = decode_interrupt_cb;
    avFormatContext->interrupt_callback.opaque = is;
    if (!av_dict_get(format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
        av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
        scan_all_pmts_set = 1;
    }
    err = avformat_open_input(&avFormatContext, is->filename, is->iformat, &format_opts);
    if (err < 0) {
        print_error(is->filename, err);
        ret = -1;
        goto fail;
    }
    if (scan_all_pmts_set) {
        av_dict_set(&format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);
    }

    AVDictionaryEntry *dictionaryEntry = NULL;
    if ((dictionaryEntry = av_dict_get(format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", dictionaryEntry->key);
        ret = AVERROR_OPTION_NOT_FOUND;
        goto fail;
    }
    is->avFormatContext = avFormatContext;

    if (genpts) {
        avFormatContext->flags |= AVFMT_FLAG_GENPTS;
    }

    av_format_inject_global_side_data(avFormatContext);

    if (find_stream_info) {
        AVDictionary **options = setup_find_stream_info_opts(avFormatContext, codec_opts);
        // 流个数(视频流,音频流,字幕流)
        int orig_nb_streams = avFormatContext->nb_streams;

        err = avformat_find_stream_info(avFormatContext, options);

        for (i = 0; i < orig_nb_streams; i++)
            av_dict_free(&options[i]);
        av_freep(&options);

        if (err < 0) {
            av_log(NULL, AV_LOG_WARNING, "%s: could not find codec parameters\n", is->filename);
            ret = -1;
            goto fail;
        }
    }

    if (avFormatContext->pb) {
        // FIXME hack, ffplay maybe should not use avio_feof() to test for the end
        avFormatContext->pb->eof_reached = 0;
    }

    if (seek_by_bytes < 0) {
        seek_by_bytes =
                !!(avFormatContext->iformat->flags & AVFMT_TS_DISCONT) && strcmp("ogg", avFormatContext->iformat->name);
    }

    is->max_frame_duration = (avFormatContext->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

    /*if (!window_title && (dictionaryEntry = av_dict_get(avFormatContext->metadata, "title", NULL, 0)))
        window_title = av_asprintf("%s - %s", dictionaryEntry->value, input_filename);*/

    /* if seeking requested, we execute it */
    if (start_time != AV_NOPTS_VALUE) {
        int64_t timestamp;

        timestamp = start_time;
        /* add the stream start time */
        if (avFormatContext->start_time != AV_NOPTS_VALUE)
            timestamp += avFormatContext->start_time;
        ret = avformat_seek_file(avFormatContext, -1, INT64_MIN, timestamp, INT64_MAX, 0);
        if (ret < 0) {
            av_log(NULL, AV_LOG_WARNING, "%s: could not seek to position %0.3f\n",
                   is->filename, (double) timestamp / AV_TIME_BASE);
        }
    }

    is->realtime = is_realtime(avFormatContext);
    printf("read_thread() realtime: %d\n", is->realtime);

    if (show_status) {
        av_dump_format(avFormatContext, 0, is->filename, 0);
    }

    for (i = 0; i < avFormatContext->nb_streams; i++) {
        AVStream *st = avFormatContext->streams[i];
        enum AVMediaType type = st->codecpar->codec_type;
        st->discard = AVDISCARD_ALL;
        if (type >= 0 && wanted_stream_spec[type] && stream_index[type] == -1)
            if (avformat_match_stream_specifier(avFormatContext, st, wanted_stream_spec[type]) > 0)
                stream_index[type] = i;
    }
    for (i = 0; i < AVMEDIA_TYPE_NB; i++) {
        if (wanted_stream_spec[i] && stream_index[i] == -1) {
            av_log(NULL, AV_LOG_ERROR, "Stream specifier %s does not match any %s stream\n",
                   wanted_stream_spec[i],
                   av_get_media_type_string(i));
            stream_index[i] = INT_MAX;
        }
    }

    if (!audio_disable)
        stream_index[AVMEDIA_TYPE_AUDIO] =
                av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO,
                                    stream_index[AVMEDIA_TYPE_AUDIO],
                                    stream_index[AVMEDIA_TYPE_VIDEO],
                                    NULL, 0);
    printf("read_thread() video: %d, audio: %d, subtitle: %d\n",
           stream_index[AVMEDIA_TYPE_VIDEO], stream_index[AVMEDIA_TYPE_AUDIO], stream_index[AVMEDIA_TYPE_SUBTITLE]);

    // stream_component_open里面到时会启动不同的线程
    if (stream_index[AVMEDIA_TYPE_AUDIO] >= 0) {
        stream_component_open(is, stream_index[AVMEDIA_TYPE_AUDIO]);
    }

    ret = -1;
    is->show_mode = show_mode;
    if (is->show_mode == SHOW_MODE_NONE) {
        is->show_mode = ret >= 0 ? SHOW_MODE_VIDEO : SHOW_MODE_RDFT;
    }
    printf("read_thread() show_mode: %d\n", is->show_mode);

    if (is->audio_stream < 0) {
        av_log(NULL, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n", is->filename);
        ret = -1;
        goto fail;
    }

    if (infinite_buffer < 0 && is->realtime)
        infinite_buffer = 1;

    printf("read_thread() infinite_buffer: %d, start_time: %"PRIu64", duration: %"PRIu64"\n",
           infinite_buffer, start_time, duration);

    for (;;) {
        if (is->abort_request) {
            printf("read_thread() for(;;) break\n");
            break;
        }
        if (is->paused != is->last_paused) {
            is->last_paused = is->paused;
            if (is->paused)
                is->read_pause_return = av_read_pause(avFormatContext);
            else
                av_read_play(avFormatContext);
        }
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
        if (is->paused &&
            (!strcmp(avFormatContext->iformat->name, "rtsp") ||
             (avFormatContext->pb && !strncmp(input_filename, "mmsh:", 5)))) {
            /* wait 10 ms to avoid trying to get another packet */
            /* XXX: horrible */
            SDL_Delay(10);
            continue;
        }
#endif
        if (is->seek_req) {
            int64_t seek_target = is->seek_pos;
            int64_t seek_min = is->seek_rel > 0 ? seek_target - is->seek_rel + 2 : INT64_MIN;
            int64_t seek_max = is->seek_rel < 0 ? seek_target - is->seek_rel - 2 : INT64_MAX;
            // FIXME the +-2 is due to rounding being not done in the correct direction in generation
            //      of the seek_pos/seek_rel variables

            ret = avformat_seek_file(is->avFormatContext, -1, seek_min, seek_target, seek_max, is->seek_flags);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR,
                       "%s: error while seeking\n", is->avFormatContext->url);
            } else {
                if (is->audio_stream >= 0) {
                    packet_queue_flush(&is->audioPQ);
                    packet_queue_put(&is->audioPQ, &flush_pkt);
                }
                if (is->seek_flags & AVSEEK_FLAG_BYTE) {
                    set_clock(&is->extClock, NAN, 0);
                } else {
                    set_clock(&is->extClock, seek_target / (double) AV_TIME_BASE, 0);
                }
            }
            is->seek_req = 0;
            is->queue_attachments_req = 1;
            is->is_eof = 0;
            if (is->paused)
                step_to_next_frame(is);
        }
        if (is->queue_attachments_req) {
            if (is->video_st && is->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                AVPacket copy = {0};
                if ((ret = av_packet_ref(&copy, &is->video_st->attached_pic)) < 0)
                    goto fail;
                packet_queue_put(&is->videoPQ, &copy);
                packet_queue_put_nullpacket(&is->videoPQ, is->video_stream);
            }
            is->queue_attachments_req = 0;
        }
        /* if the queue are full, no need to read more */
        if (infinite_buffer < 1 &&
            (is->audioPQ.size + is->videoPQ.size + is->subtitlePQ.size > MAX_QUEUE_SIZE
             || (stream_has_enough_packets(is->audio_st, is->audio_stream, &is->audioPQ)))) {
            /* wait 10 ms */
            struct timespec timeToWait;
            struct timeval now;
            gettimeofday(&now, NULL);
            timeToWait.tv_sec = now.tv_sec + 5;
            timeToWait.tv_nsec = (now.tv_usec + 1000UL * 10) * 1000UL;// 10ms
            pthread_mutex_lock(&is->continue_read_thread_mutex);
            //printf("read_thread() SDL_CondWaitTimeout\n");
            pthread_cond_timedwait(&is->continue_read_thread_cond, &is->continue_read_thread_mutex, &timeToWait);
            pthread_mutex_unlock(&is->continue_read_thread_mutex);
            continue;
        }
        if (!is->paused
            && (!is->audio_st
            || (is->audDecoder.finished == is->audioPQ.serial && frame_queue_nb_remaining(&is->sampFQ) == 0))) {
            if (loop != 1 && (!loop || --loop)) {
                stream_seek(is, start_time != AV_NOPTS_VALUE ? start_time : 0, 0, 0);
            } else if (autoexit) {
                ret = AVERROR_EOF;
                goto fail;
            }
        }

        // 读取AVPacket
        ret = av_read_frame(avFormatContext, pkt);
        if (ret < 0) {
            if ((ret == AVERROR_EOF || avio_feof(avFormatContext->pb))
                && !is->is_eof) {
                printf("read_thread() AVERROR_EOF: %d, ret: %d\n", AVERROR_EOF, ret);
                if (is->audio_stream >= 0)
                    packet_queue_put_nullpacket(&is->audioPQ, is->audio_stream);
                is->is_eof = 1;
            }
            if (avFormatContext->pb && avFormatContext->pb->error) {
                printf("read_thread() error: %d\n", avFormatContext->pb->error);
                break;
            }

            // 文件已经读完了,为什么还要这样操作?不退出去难道是为了seek?
            /* wait 10 ms */
            struct timespec timeToWait;
            struct timeval now;
            gettimeofday(&now, NULL);
            timeToWait.tv_sec = now.tv_sec + 5;
            timeToWait.tv_nsec = (now.tv_usec + 1000UL * 10) * 1000UL;// 10ms
            pthread_mutex_lock(&is->continue_read_thread_mutex);
            //printf("read_thread() SDL_CondWaitTimeout ret < 0\n");
            pthread_cond_timedwait(&is->continue_read_thread_cond, &is->continue_read_thread_mutex, &timeToWait);
            pthread_mutex_unlock(&is->continue_read_thread_mutex);
            continue;
        }

        is->is_eof = 0;
        /* check if packet is in play range specified by user, then queue, otherwise discard */
        stream_start_time = avFormatContext->streams[pkt->stream_index]->start_time;
        pkt_ts = pkt->pts == AV_NOPTS_VALUE ? pkt->dts : pkt->pts;
        pkt_in_play_range = duration == AV_NOPTS_VALUE ||
                            (pkt_ts - (stream_start_time != AV_NOPTS_VALUE ? stream_start_time : 0)) *
                            av_q2d(avFormatContext->streams[pkt->stream_index]->time_base) -
                            (double) (start_time != AV_NOPTS_VALUE ? start_time : 0) / 1000000
                            <= ((double) duration / 1000000);

        if (pkt->stream_index == is->audio_stream
            && pkt_in_play_range) {
            // 音频帧
            packet_queue_put(&is->audioPQ, pkt);
        } else {
            av_packet_unref(pkt);
        }
    }// for (;;) end

    ret = 0;
    fail:
    if (avFormatContext && !is->avFormatContext)
        avformat_close_input(&avFormatContext);

    if (ret != 0) {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        event.user.data1 = is;
        SDL_PushEvent(&event);
    }

    printf("read_thread() end\n");
    return 0;
}

static VideoState *stream_open(const char *filename, AVInputFormat *iformat) {
    printf("stream_open() start\n");

    VideoState *videoState;
    videoState = av_mallocz(sizeof(VideoState));
    if (!videoState) {
        return NULL;
    }

    // 复制字符串(有可能为NULL)
    videoState->filename = av_strdup(filename);
    if (!videoState->filename) {
        goto fail;
    }
    videoState->iformat = iformat;
    videoState->xleft = 0;
    videoState->ytop = 0;

    if (packet_queue_init(&videoState->audioPQ) < 0) {
        goto fail;
    }
    if (frame_queue_init(&videoState->sampFQ, &videoState->audioPQ, SAMPLE_QUEUE_SIZE, 1) < 0) {
        goto fail;
    }

    pthread_mutex_init(&videoState->continue_read_thread_mutex, NULL);
    pthread_cond_init(&videoState->continue_read_thread_cond, NULL);

    init_clock(&videoState->audClock, &videoState->audioPQ.serial);
    init_clock(&videoState->extClock, &videoState->extClock.serial);

    videoState->audio_clock_serial = -1;
    videoState->av_sync_type = av_sync_type;
    videoState->muted = 0;// 是否静音 否
    if (startup_volume < 0)
        av_log(NULL, AV_LOG_WARNING, "-volume=%d < 0, setting to 0\n", startup_volume);
    if (startup_volume > 100)
        av_log(NULL, AV_LOG_WARNING, "-volume=%d > 100, setting to 100\n", startup_volume);
    startup_volume = av_clip(startup_volume, 0, 100);
    startup_volume = av_clip(SDL_MIX_MAXVOLUME * startup_volume / 100, 0, SDL_MIX_MAXVOLUME);
    printf("stream_open() audio_volume: %d\n", startup_volume);// 128
    videoState->audio_volume = startup_volume;

    // 创建读线程
    int ret = pthread_create(&videoState->read_thread, NULL, read_thread, videoState);
    if (ret != 0) {
        av_log(NULL, AV_LOG_FATAL, "pthread_create(): %s\n", strerror(ret));
        fail:
        stream_close(videoState);
        return NULL;
    }

    printf("stream_open() end\n");
    return videoState;
}

static void stream_cycle_channel(VideoState *is, int codec_type) {
    AVFormatContext *ic = is->avFormatContext;
    int start_index, stream_index;
    int old_index;
    AVStream *st;
    AVProgram *p = NULL;
    int nb_streams = is->avFormatContext->nb_streams;

    if (codec_type == AVMEDIA_TYPE_VIDEO) {
        start_index = is->last_video_stream;
        old_index = is->video_stream;
    } else if (codec_type == AVMEDIA_TYPE_AUDIO) {
        start_index = is->last_audio_stream;
        old_index = is->audio_stream;
    } else {
        start_index = is->last_subtitle_stream;
        old_index = is->subtitle_stream;
    }
    stream_index = start_index;

    if (codec_type != AVMEDIA_TYPE_VIDEO && is->video_stream != -1) {
        p = av_find_program_from_stream(ic, NULL, is->video_stream);
        if (p) {
            nb_streams = p->nb_stream_indexes;
            for (start_index = 0; start_index < nb_streams; start_index++)
                if (p->stream_index[start_index] == stream_index)
                    break;
            if (start_index == nb_streams)
                start_index = -1;
            stream_index = start_index;
        }
    }

    for (;;) {
        if (++stream_index >= nb_streams) {
            if (codec_type == AVMEDIA_TYPE_SUBTITLE) {
                stream_index = -1;
                is->last_subtitle_stream = -1;
                goto the_end;
            }
            if (start_index == -1)
                return;
            stream_index = 0;
        }
        if (stream_index == start_index)
            return;
        st = is->avFormatContext->streams[p ? p->stream_index[stream_index] : stream_index];
        if (st->codecpar->codec_type == codec_type) {
            /* check that parameters are OK */
            switch (codec_type) {
                case AVMEDIA_TYPE_AUDIO:
                    if (st->codecpar->sample_rate != 0 &&
                        st->codecpar->channels != 0)
                        goto the_end;
                    break;
                case AVMEDIA_TYPE_VIDEO:
                case AVMEDIA_TYPE_SUBTITLE:
                    goto the_end;
                default:
                    break;
            }
        }
    }
    the_end:
    if (p && stream_index != -1)
        stream_index = p->stream_index[stream_index];
    av_log(NULL, AV_LOG_INFO, "Switch %s stream from #%d to #%d\n",
           av_get_media_type_string(codec_type),
           old_index,
           stream_index);

    stream_component_close(is, old_index);
    stream_component_open(is, stream_index);
}

static void toggle_audio_display(VideoState *is) {
    int next = is->show_mode;
    do {
        next = (next + 1) % SHOW_MODE_NB;
    } while (next != is->show_mode &&
             (next == SHOW_MODE_VIDEO && !is->video_st || next != SHOW_MODE_VIDEO && !is->audio_st));
    if (is->show_mode != next) {
        is->force_refresh = 1;
        is->show_mode = next;
    }
}

static void refresh_loop_wait_event(VideoState *videoState, SDL_Event *event) {
    //printf("event_loop() refresh_loop_wait_event start\n");
    // 这个参数搞懂就达到同步了
    double remaining_time = 0.0;
    SDL_PumpEvents();
    while (!SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)) {
        if (!cursor_hidden && av_gettime_relative() - cursor_last_shown > CURSOR_HIDE_DELAY) {
            SDL_ShowCursor(0);
            cursor_hidden = 1;
        }
        // 立即显示当前帧或延时remaining_time后再显示
        if (remaining_time > 0.0) {
            int64_t remaining_time_sleep = (int64_t) (remaining_time * 1000000.0);
            //printf("event_loop() remaining_time_sleep: %ld\n", (long) remaining_time_sleep);
            // 在这里睡觉了(单位: 微秒)
            av_usleep(remaining_time_sleep);
        }
        remaining_time = REFRESH_RATE;
        if (videoState->show_mode != SHOW_MODE_NONE
            && (!videoState->paused || videoState->force_refresh)) {
            //video_refresh(videoState, &remaining_time);
        }
        SDL_PumpEvents();
    }
    //printf("event_loop() refresh_loop_wait_event remaining_time: %lf\n", remaining_time);
}

static void seek_chapter(VideoState *is, int incr) {
    int64_t pos = get_master_clock(is) * AV_TIME_BASE;
    int i;

    if (!is->avFormatContext->nb_chapters)
        return;

    /* find the current chapter */
    for (i = 0; i < is->avFormatContext->nb_chapters; i++) {
        AVChapter *ch = is->avFormatContext->chapters[i];
        if (av_compare_ts(pos, AV_TIME_BASE_Q, ch->start, ch->time_base) < 0) {
            i--;
            break;
        }
    }

    i += incr;
    i = FFMAX(i, 0);
    if (i >= is->avFormatContext->nb_chapters)
        return;

    av_log(NULL, AV_LOG_VERBOSE, "Seeking to chapter %d.\n", i);
    stream_seek(is, av_rescale_q(is->avFormatContext->chapters[i]->start, is->avFormatContext->chapters[i]->time_base,
                                 AV_TIME_BASE_Q), 0, 0);
}

/* handle an event sent by the GUI */
static void event_loop(VideoState *cur_stream) {
    printf("event_loop() start\n");
    double incr, pos, frac;
    // incr: 0.000000, pos: 0.000000, frac: 0.000000
    printf("event_loop() incr: %lf, pos: %lf, frac: %lf\n", incr, pos, frac);

    SDL_Event event;
    for (;;) {
        double x;
        // SDL event队列为空,则在while循环中播放视频帧.否则从队列头部取一个event,退出当前函数,然后处理该event.
        refresh_loop_wait_event(cur_stream, &event);
        //printf("event_loop() event.type: %d\n", event.type);
        switch (event.type) {
            case SDL_KEYDOWN:
                //printf("event_loop() SDL_KEYDOWN\n");
                if (exit_on_keydown || event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) {
                    do_exit(cur_stream);
                    break;
                }
                // If we don't yet have a window, skip all key events, because read_thread might still be initializing...
                if (!cur_stream->width)
                    continue;
                switch (event.key.keysym.sym) {
                    case SDLK_f:
                        //toggle_full_screen(cur_stream);
                        cur_stream->force_refresh = 1;
                        break;
                    case SDLK_p:
                    case SDLK_SPACE:
                        toggle_pause(cur_stream);
                        break;
                    case SDLK_m:
                        toggle_mute(cur_stream);
                        break;
                    case SDLK_KP_MULTIPLY:
                    case SDLK_0:
                        update_volume(cur_stream, 1, SDL_VOLUME_STEP);
                        break;
                    case SDLK_KP_DIVIDE:
                    case SDLK_9:
                        update_volume(cur_stream, -1, SDL_VOLUME_STEP);
                        break;
                    case SDLK_s: // S: Step to next frame
                        step_to_next_frame(cur_stream);
                        break;
                    case SDLK_a:
                        stream_cycle_channel(cur_stream, AVMEDIA_TYPE_AUDIO);
                        break;
                    case SDLK_v:
                        stream_cycle_channel(cur_stream, AVMEDIA_TYPE_VIDEO);
                        break;
                    case SDLK_c:
                        stream_cycle_channel(cur_stream, AVMEDIA_TYPE_VIDEO);
                        stream_cycle_channel(cur_stream, AVMEDIA_TYPE_AUDIO);
                        stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
                        break;
                    case SDLK_t:
                        stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
                        break;
                    case SDLK_w:
#if CONFIG_AVFILTER
                        if (cur_stream->show_mode == SHOW_MODE_VIDEO && cur_stream->vfilter_idx < nb_vfilters - 1) {
                            if (++cur_stream->vfilter_idx >= nb_vfilters)
                                cur_stream->vfilter_idx = 0;
                        } else {
                            cur_stream->vfilter_idx = 0;
                            toggle_audio_display(cur_stream);
                        }
#else
                        toggle_audio_display(cur_stream);
#endif
                        break;
                    case SDLK_PAGEUP:
                        if (cur_stream->avFormatContext->nb_chapters <= 1) {
                            incr = 600.0;
                            goto do_seek;
                        }
                        seek_chapter(cur_stream, 1);
                        break;
                    case SDLK_PAGEDOWN:
                        if (cur_stream->avFormatContext->nb_chapters <= 1) {
                            incr = -600.0;
                            goto do_seek;
                        }
                        seek_chapter(cur_stream, -1);
                        break;
                    case SDLK_LEFT:
                        incr = seek_interval ? -seek_interval : -10.0;
                        goto do_seek;
                    case SDLK_RIGHT:
                        incr = seek_interval ? seek_interval : 10.0;
                        goto do_seek;
                    case SDLK_UP:
                        incr = 60.0;
                        goto do_seek;
                    case SDLK_DOWN:
                        incr = -60.0;
                    do_seek:
                        if (seek_by_bytes) {
                            pos = -1;
                            if (pos < 0 && cur_stream->video_stream >= 0)
                                pos = frame_queue_last_pos(&cur_stream->pictFQ);
                            if (pos < 0 && cur_stream->audio_stream >= 0)
                                pos = frame_queue_last_pos(&cur_stream->sampFQ);
                            if (pos < 0)
                                pos = avio_tell(cur_stream->avFormatContext->pb);
                            if (cur_stream->avFormatContext->bit_rate)
                                incr *= cur_stream->avFormatContext->bit_rate / 8.0;
                            else
                                incr *= 180000.0;
                            pos += incr;
                            stream_seek(cur_stream, pos, incr, 1);
                        } else {
                            pos = get_master_clock(cur_stream);
                            if (isnan(pos))
                                pos = (double) cur_stream->seek_pos / AV_TIME_BASE;
                            pos += incr;
                            if (cur_stream->avFormatContext->start_time != AV_NOPTS_VALUE &&
                                pos < cur_stream->avFormatContext->start_time / (double) AV_TIME_BASE)
                                pos = cur_stream->avFormatContext->start_time / (double) AV_TIME_BASE;
                            stream_seek(cur_stream, (int64_t) (pos * AV_TIME_BASE), (int64_t) (incr * AV_TIME_BASE), 0);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                //printf("event_loop() SDL_MOUSEBUTTONDOWN\n");
                if (exit_on_mousedown) {
                    do_exit(cur_stream);
                    break;
                }
                if (event.button.button == SDL_BUTTON_LEFT) {
                    static int64_t last_mouse_left_click = 0;
                    if (av_gettime_relative() - last_mouse_left_click <= 500000) {
                        //toggle_full_screen(cur_stream);
                        cur_stream->force_refresh = 1;
                        last_mouse_left_click = 0;
                    } else {
                        last_mouse_left_click = av_gettime_relative();
                    }
                }
            case SDL_MOUSEMOTION://
                //printf("event_loop() SDL_MOUSEMOTION\n");
                if (cursor_hidden) {
                    SDL_ShowCursor(1);
                    cursor_hidden = 0;
                }
                cursor_last_shown = av_gettime_relative();
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button != SDL_BUTTON_RIGHT)
                        break;
                    x = event.button.x;
                } else {
                    if (!(event.motion.state & SDL_BUTTON_RMASK))
                        break;
                    x = event.motion.x;
                }
                if (seek_by_bytes || cur_stream->avFormatContext->duration <= 0) {
                    uint64_t size = avio_size(cur_stream->avFormatContext->pb);
                    stream_seek(cur_stream, size * x / cur_stream->width, 0, 1);
                } else {
                    int64_t ts;
                    int ns, hh, mm, ss;
                    int tns, thh, tmm, tss;
                    tns = cur_stream->avFormatContext->duration / 1000000LL;
                    thh = tns / 3600;
                    tmm = (tns % 3600) / 60;
                    tss = (tns % 60);
                    frac = x / cur_stream->width;
                    ns = frac * tns;
                    hh = ns / 3600;
                    mm = (ns % 3600) / 60;
                    ss = (ns % 60);
                    av_log(NULL, AV_LOG_INFO,
                           "Seek to %2.0f%% (%2d:%02d:%02d) of total duration (%2d:%02d:%02d)       \n", frac * 100,
                           hh, mm, ss, thh, tmm, tss);
                    ts = frac * cur_stream->avFormatContext->duration;
                    if (cur_stream->avFormatContext->start_time != AV_NOPTS_VALUE)
                        ts += cur_stream->avFormatContext->start_time;
                    stream_seek(cur_stream, ts, 0, 0);
                }
                break;
            case SDL_WINDOWEVENT://
                //printf("event_loop() SDL_WINDOWEVENT\n");
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        screen_width = cur_stream->width = event.window.data1;
                        screen_height = cur_stream->height = event.window.data2;
                        printf("event_loop() SDL_WINDOWEVENT screen_width : %d\n", screen_width);
                        printf("event_loop() SDL_WINDOWEVENT screen_height: %d\n", screen_height);
                    case SDL_WINDOWEVENT_EXPOSED:
                        printf("event_loop() SDL_WINDOWEVENT SDL_WINDOWEVENT_EXPOSED\n");
                        cur_stream->force_refresh = 1;
                }
                break;
            case SDL_QUIT:
                // 点击窗口"关闭"按钮退出
                printf("event_loop() SDL_QUIT\n");
            case FF_QUIT_EVENT:
                printf("event_loop() FF_QUIT_EVENT\n");
                do_exit(cur_stream);
                break;
            default:
                break;
        }
    }// for(...) end

    printf("event_loop() end\n");
}

static int dummy;

static const OptionDef options[] = {
        CMDUTILS_COMMON_OPTIONS
        {"x",
         HAS_ARG,
         {.func_arg = opt_width},
         "force displayed width",
         "width"},
        {"y", HAS_ARG, {.func_arg = opt_height}, "force displayed height", "height"},
        {"s", HAS_ARG | OPT_VIDEO, {.func_arg = opt_frame_size}, "set frame size (WxH or abbreviation)", "size"},
        {"fs", OPT_BOOL, {&is_full_screen}, "force full screen"},
        {"an", OPT_BOOL, {&audio_disable}, "disable audio"},
        {"vn", OPT_BOOL, {&video_disable}, "disable video"},
        {"sn", OPT_BOOL, {&subtitle_disable}, "disable subtitling"},
        {"ast", OPT_STRING | HAS_ARG | OPT_EXPERT, {&wanted_stream_spec[AVMEDIA_TYPE_AUDIO]},
         "select desired audio stream", "stream_specifier"},
        {"vst", OPT_STRING | HAS_ARG | OPT_EXPERT, {&wanted_stream_spec[AVMEDIA_TYPE_VIDEO]},
         "select desired video stream", "stream_specifier"},
        {"sst", OPT_STRING | HAS_ARG | OPT_EXPERT, {&wanted_stream_spec[AVMEDIA_TYPE_SUBTITLE]},
         "select desired subtitle stream", "stream_specifier"},
        {"ss", HAS_ARG, {.func_arg = opt_seek}, "seek to a given position in seconds", "pos"},
        {"t", HAS_ARG, {.func_arg = opt_duration}, "play  \"duration\" seconds of audio/video", "duration"},
        {"bytes", OPT_INT | HAS_ARG, {&seek_by_bytes}, "seek by bytes 0=off 1=on -1=auto", "val"},
        {"seek_interval", OPT_FLOAT | HAS_ARG, {&seek_interval}, "set seek interval for left/right keys, in seconds",
         "seconds"},
        {"nodisp", OPT_BOOL, {&display_disable}, "disable graphical display"},
        {"noborder", OPT_BOOL, {&borderless}, "borderless window"},
        {"alwaysontop", OPT_BOOL, {&alwaysontop}, "window always on top"},
        {"volume", OPT_INT | HAS_ARG, {&startup_volume}, "set startup volume 0=min 100=max", "volume"},
        {"f", HAS_ARG, {.func_arg = opt_format}, "force format", "fmt"},
        {"pix_fmt", HAS_ARG | OPT_EXPERT | OPT_VIDEO, {.func_arg = opt_frame_pix_fmt}, "set pixel format", "format"},
        {"stats", OPT_BOOL | OPT_EXPERT, {&show_status}, "show status", ""},
        {"fast", OPT_BOOL | OPT_EXPERT, {&fast}, "non spec compliant optimizations", ""},
        {"genpts", OPT_BOOL | OPT_EXPERT, {&genpts}, "generate pts", ""},
        {"drp", OPT_INT | HAS_ARG | OPT_EXPERT, {&decoder_reorder_pts}, "let decoder reorder pts 0=off 1=on -1=auto",
         ""},
        {"lowres", OPT_INT | HAS_ARG | OPT_EXPERT, {&lowres}, "", ""},
        {"sync", HAS_ARG | OPT_EXPERT, {.func_arg = opt_sync},
         "set audio-video sync. type (type=audio/video/ext)",
         "type"},
        {"autoexit", OPT_BOOL | OPT_EXPERT, {&autoexit}, "exit at the end", ""},
        {"exitonkeydown", OPT_BOOL | OPT_EXPERT, {&exit_on_keydown}, "exit on key down", ""},
        {"exitonmousedown", OPT_BOOL | OPT_EXPERT, {&exit_on_mousedown}, "exit on mouse down", ""},
        {"loop", OPT_INT | HAS_ARG | OPT_EXPERT, {&loop}, "set number of times the playback shall be looped",
         "loop count"},
        {"framedrop", OPT_BOOL | OPT_EXPERT, {&framedrop}, "drop frames when cpu is too slow", ""},
        {"infbuf", OPT_BOOL | OPT_EXPERT, {&infinite_buffer},
         "don't limit the input buffer size (useful with realtime streams)", ""},
        {"window_title", OPT_STRING | HAS_ARG, {&window_title}, "set window title", "window title"},
#if CONFIG_AVFILTER
        {"vf", OPT_EXPERT | HAS_ARG, {.func_arg = opt_add_vfilter}, "set video filters", "filter_graph"},
        {"af", OPT_STRING | HAS_ARG, {&afilters}, "set audio filters", "filter_graph"},
#endif
        {"rdftspeed", OPT_INT | HAS_ARG | OPT_AUDIO | OPT_EXPERT, {&rdftspeed}, "rdft speed", "msecs"},
        {"showmode", HAS_ARG, {.func_arg = opt_show_mode}, "select show mode (0 = video, 1 = waves, 2 = RDFT)", "mode"},
        {"default", HAS_ARG | OPT_AUDIO | OPT_VIDEO | OPT_EXPERT, {.func_arg = opt_default}, "generic catch all option",
         ""},
        {"i", OPT_BOOL, {&dummy}, "read specified file", "input_file"},
        {"codec", HAS_ARG, {.func_arg = opt_codec}, "force decoder", "decoder_name"},
        {"acodec", HAS_ARG | OPT_STRING | OPT_EXPERT, {&audio_codec_name}, "force audio decoder", "decoder_name"},
        {"scodec", HAS_ARG | OPT_STRING | OPT_EXPERT, {&subtitle_codec_name}, "force subtitle decoder", "decoder_name"},
        {"vcodec", HAS_ARG | OPT_STRING | OPT_EXPERT, {&video_codec_name}, "force video decoder", "decoder_name"},
        {"autorotate", OPT_BOOL, {&autorotate}, "automatically rotate video", ""},
        {"find_stream_info", OPT_BOOL | OPT_INPUT | OPT_EXPERT, {&find_stream_info},
         "read and decode the streams to fill missing information with heuristics"},
        {"filter_threads", HAS_ARG | OPT_INT | OPT_EXPERT, {&filter_nbthreads}, "number of filter threads per graph"},
        {NULL,},
};

static void show_usage(void) {
    av_log(NULL, AV_LOG_INFO, "Simple media player\n");
    av_log(NULL, AV_LOG_INFO, "usage: %s [options] input_file\n", program_name);
    av_log(NULL, AV_LOG_INFO, "\n");
}

void show_help_default(const char *opt, const char *arg) {
    av_log_set_callback(log_callback_help);
    show_usage();
    show_help_options(options, "Main options:", 0, OPT_EXPERT, 0);
    show_help_options(options, "Advanced options:", OPT_EXPERT, 0, 0);
    printf("\n");
    show_help_children(avcodec_get_class(), AV_OPT_FLAG_DECODING_PARAM);
    show_help_children(avformat_get_class(), AV_OPT_FLAG_DECODING_PARAM);
#if !CONFIG_AVFILTER
    show_help_children(sws_get_class(), AV_OPT_FLAG_ENCODING_PARAM);
#else
    show_help_children(avfilter_get_class(), AV_OPT_FLAG_FILTERING_PARAM);
#endif
    printf("\nWhile playing:\n"
           "q, ESC              quit\n"
           "f                   toggle full screen\n"
           "p, SPC              pause\n"
           "m                   toggle mute\n"
           "9, 0                decrease and increase volume respectively\n"
           "/, *                decrease and increase volume respectively\n"
           "a                   cycle audio channel in the current program\n"
           "v                   cycle video channel\n"
           "t                   cycle subtitle channel in the current program\n"
           "c                   cycle program\n"
           "w                   cycle video filters or show modes\n"
           "s                   activate frame-step mode\n"
           "left/right          seek backward/forward 10 seconds or to custom interval if -seek_interval is set\n"
           "down/up             seek backward/forward 1 minute\n"
           "page down/page up   seek backward/forward 10 minutes\n"
           "right mouse click   seek to percentage in file corresponding to fraction of width\n"
           "left double-click   toggle full screen\n"
    );
}

/***
 read_thread(void *arg)
	stream_component_open(VideoState *is, int stream_index)
	stream_component_open(is, st_index[AVMEDIA_TYPE_AUDIO])
	stream_component_open(is, st_index[AVMEDIA_TYPE_VIDEO])
	stream_component_open(is, st_index[AVMEDIA_TYPE_SUBTITLE])
		decoder_start(Decoder *d,
					int (*fn)(void *),
					const char *thread_name,
					void *arg)
		decoder_start(&is->auddec, audio_thread, "audio_decoder", is)
		decoder_start(&is->viddec, video_thread, "video_decoder", is)
		decoder_start(&is->subdec, subtitle_thread, "subtitle_decoder", is)
上面的流程是:
read_thread是一个线程,
在这个线程中通过调用stream_component_open方法,stream_component_open方法调用
decoder_start方法,在decoder_start方法中再开启音频,视频,字幕(如果有的话)三个
线程.

 选择外部时钟作为主时钟
 ffplay ~/Videos/Sintel.2010.720p.mkv -sync ext

 在播放过程中如何取得到当前的播放时间
 VideoState->video_st->codec->reordered_opaque ＊ av_q2d(VideoState->video_st->time_base)
 */
/* Called from the main */
int main(int argc, char **argv) {
    printf("main() start\n");
    // N-96855-ga439acee3f
    printf("main() version: %s\n", av_version_info());

    av_init_packet(&flush_pkt);
    flush_pkt.data = (uint8_t *) &flush_pkt;

    init_dynload();
    //av_log_set_flags(AV_LOG_SKIP_REPEATED);
    av_log_set_flags(AV_LOG_QUIET);
    parse_loglevel(argc, argv, options);

    /* register all codecs, demux and protocols */
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avformat_network_init();

    init_opts();

    signal(SIGINT, sigterm_handler); /* Interrupt (ANSI).    */
    signal(SIGTERM, sigterm_handler); /* Termination (ANSI).  */

    show_banner(argc, argv, options);

    parse_options(NULL, argc, argv, options, opt_input_file);

    input_filename = "/Users/alexander/Music/music/谁在意我留下的泪.mp3";
    if (!input_filename) {
        show_usage();
        av_log(NULL, AV_LOG_FATAL, "An input file must be specified\n");
        av_log(NULL, AV_LOG_FATAL,
               "Use -h to get full help or, even better, run 'man %s'\n", program_name);
        exit(1);
    }
    // 全路径名称
    printf("main() input_filename : %s\n", input_filename);
    printf("main() audio_disable  : %d\n", audio_disable);// 0
    int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if (audio_disable) {
        flags &= ~SDL_INIT_AUDIO;
    } else {
        /* Try to work around an occasional ALSA buffer underflow issue when the
         * period size is NPOT due to ALSA resampling by forcing the buffer size. */
        if (!SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE")) {
            SDL_setenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE", "1", 1);
        }
    }
    if (SDL_Init(flags)) {
        av_log(NULL, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        av_log(NULL, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
        exit(1);
    }
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    if (!display_disable) {
        int flags = SDL_WINDOW_HIDDEN;
        printf("main() alwaysontop    : %d\n", alwaysontop);// 0
        if (alwaysontop) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
            flags |= SDL_WINDOW_ALWAYS_ON_TOP;
#else
            av_log(NULL, AV_LOG_WARNING, "Your SDL version doesn't support SDL_WINDOW_ALWAYS_ON_TOP. Feature will be inactive.\n");
#endif
        }
        printf("main() borderless     : %d\n", borderless);// 0
        if (borderless) {
            flags |= SDL_WINDOW_BORDERLESS;
        } else {
            flags |= SDL_WINDOW_RESIZABLE;
        }
    }

    VideoState *videoState;
    videoState = stream_open(input_filename, file_iformat);
    if (!videoState) {
        // videoState is NULL
        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
    }

    event_loop(videoState);

    /* never returns */
    printf("main() end\n");

    return 0;
}

void test() {

}
