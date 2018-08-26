//
// Created by root on 18-7-16.
//

#ifndef MYSTUDY_FFMPEG_SAMPLE
#define MYSTUDY_FFMPEG_SAMPLE

#include "../include/MyHead.h"

static void decode_audio(AVCodecContext *audioAVCodecContext,
                         AVPacket *avPacket,
                         AVFrame *decoded_frame,
                         FILE *outfile) {
    int i, j;
    int result, data_size;

    /* send the packet with the compressed data to the decoder */
    // 正常情况下返回结果为0
    result = avcodec_send_packet(audioAVCodecContext, avPacket);
    if (result < 0) {
        fprintf(stderr, "Error submitting the packet to the decoder\n");
        exit(1);
    }

    /* read all the output frames (in general there may be any number of them */
    while (result >= 0) {
        // 正常情况情况下返回结果为0,第二次返回结果可能为-11
        // 解码后的数据放在了decoded_frame中
        result = avcodec_receive_frame(audioAVCodecContext, decoded_frame);
        // fprintf(stdout, "While loop result = %d\n", result);
        if (result == AVERROR(EAGAIN) || result == AVERROR_EOF) {
            return;
        } else if (result < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
        // 一个sample占用多少个字节
        data_size = av_get_bytes_per_sample(audioAVCodecContext->sample_fmt);
        // fprintf(stdout, "While loop data_size = %d\n", data_size);// 4
        if (data_size < 0) {
            /* This should not occur, checking just for paranoia */
            fprintf(stderr, "Failed to calculate data all_pkts_size\n");
            exit(1);
        }
        int nb_samples = decoded_frame->nb_samples;
        int channels = audioAVCodecContext->channels;
        // fprintf(stdout, "While loop nb_samples = %d\n", nb_samples);// 1152
        // fprintf(stdout, "While loop channels = %d\n", channels);// 2(左声道,右声道)
        // 一帧数据有多个sample,一个sample可能有2个声道
        for (i = 0; i < nb_samples; i++) {
            for (j = 0; j < channels; j++) {
                /***
                 刚开始写的时候,decoded_frame->data[0]与decoded_frame->data[1]指向的都是开始位置,都是起点.
                 然后decoded_frame->data[0]偏移data_size * 0个位置写data_size个字节,
                 接着decoded_frame->data[1]偏移data_size * 1个位置写data_size个字节,
                 依次类推.
                 双声道时先写左声道,再写右声道.
                 */
                fwrite(decoded_frame->data[j] + data_size * i, 1, data_size, outfile);
            }
        }
    }
}

/***
 来自官方sample的代码
 filename = "/root/mydev/workspace_github/simplest_ffmpeg_audio_player_2.2/simplest_audio_play_sdl2/aaa.mp3";
 outfilename = "/root/mydev/workspace_github/simplest_ffmpeg_audio_player_2.2/simplest_audio_play_sdl2/output.pcm";
 */
int decode_audio_main(const char *infilename, const char *outfilename) {
    AVFormatContext *avFormatContext = NULL;
    AVCodecParameters *audioAVCodecParameters = NULL;
    av_register_all();
    avFormatContext = avformat_alloc_context();
    if (avformat_open_input(&avFormatContext, infilename, NULL, NULL)) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    int audioStreamIndex = -1;
    int nb_streams = avFormatContext->nb_streams;
    printf("avFormatContext->nb_streams = %d\n", nb_streams);// 2
    int i;
    for (i = 0; i < nb_streams; i++) {
        AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
        if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = avMediaType;
            printf("audioStreamIndex = %d\n", audioStreamIndex);
        }
    }
    if (audioStreamIndex == -1) {
        printf("Didn't find a video or audio stream.\n");
        return -1;
    }

    if (audioStreamIndex != -1) {
        audioAVCodecParameters = avFormatContext->streams[audioStreamIndex]->codecpar;
    }

    printf("audioAVCodecParameters->codec_id = %d\n", audioAVCodecParameters->codec_id);// 标示特定的编解码器
    printf("audioAVCodecParameters->codec_type = %d\n", audioAVCodecParameters->codec_type);// 标示特定的编解码器
    printf("audioAVCodecParameters->channels = %d\n", audioAVCodecParameters->channels);// 声道数（音频）
    printf("audioAVCodecParameters->bit_rate = %d\n", audioAVCodecParameters->bit_rate);// 平均比特率
    printf("audioAVCodecParameters->format = %d\n", audioAVCodecParameters->format);// 像素格式/采样数据格式
    printf("audioAVCodecParameters->sample_rate = %d\n", audioAVCodecParameters->sample_rate);// 采样率（音频）
    printf("audioAVCodecParameters->sample_aspect_ratio = %d\n", audioAVCodecParameters->sample_aspect_ratio);
    printf("audioAVCodecParameters->bits_per_coded_sample = %d\n", audioAVCodecParameters->bits_per_coded_sample);
    printf("audioAVCodecParameters->bits_per_raw_sample = %d\n", audioAVCodecParameters->bits_per_raw_sample);

    const AVCodec *audioAVCodecDecoder = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVCodecParserContext *audioAVCodecParserContext = NULL;
    AVFrame *decoded_frame = NULL;
    // 存储一帧（一般情况下）压缩编码数据
    AVPacket *avPacket = NULL;
    int len, result;
    FILE *infile, *outfile;
    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data_buf;
    size_t data_buf_size;

    avPacket = av_packet_alloc();

    /* find the MPEG audio decoder */
    audioAVCodecDecoder = avcodec_find_decoder(AV_CODEC_ID_MP2);
    if (!audioAVCodecDecoder) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    audioAVCodecContext = avcodec_alloc_context3(audioAVCodecDecoder);
    if (!audioAVCodecContext) {
        fprintf(stderr, "Could not allocate audio codec context\n");
        exit(1);
    }

    audioAVCodecParserContext = av_parser_init(audioAVCodecDecoder->id);
    if (!audioAVCodecParserContext) {
        fprintf(stderr, "Parser not found\n");
        exit(1);
    }

    /* open decoder */
    if (avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    AVSampleFormat audioAVSampleFormat = audioAVCodecContext->sample_fmt;
    int sample_rate = audioAVCodecContext->sample_rate;
    int channels = audioAVCodecContext->channels;
    printf("audioAVSampleFormat = %d\n", audioAVSampleFormat);// 8
    printf("audio sample_rate = %d\n", sample_rate);// 0
    printf("audio channels = %d\n", channels);// 0

    fprintf(stdout, "AVERROR(EAGAIN) = %d\n", AVERROR(EAGAIN));// -11
    fprintf(stdout, "AVERROR(EINVAL) = %d\n", AVERROR(EINVAL));// -22
    fprintf(stdout, "AVERROR(ENOMEM) = %d\n", AVERROR(ENOMEM));// -12
    fprintf(stdout, "AVERROR_EOF     = %d\n", AVERROR_EOF);    // -541478725

    infile = fopen(infilename, "rb");
    if (!infile) {
        fprintf(stderr, "Could not open %s\n", infilename);
        exit(1);
    }
    outfile = fopen(outfilename, "wb");
    if (!outfile) {
        av_free(audioAVCodecContext);
        exit(1);
    }

    /* decode until eof */
    data_buf = inbuf;
    // 把infile中的文件内容读到data_buf指向的数组中去,读成功的话data_buf_size值等于AUDIO_INBUF_SIZE值
    data_buf_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, infile);
    // 成功时返回AUDIO_INBUF_SIZE大小
    fprintf(stdout, "data_size = %d\n", data_buf_size);

    while (data_buf_size > 0) {
        if (!decoded_frame) {
            if (!(decoded_frame = av_frame_alloc())) {
                fprintf(stderr, "Could not allocate audio frame\n");
                exit(1);
            }
        }

        /***
         int av_parser_parse2(AVCodecParserContext *s,
                                 AVCodecContext *avctx,
                                 uint8_t **poutbuf,
                                 int *poutbuf_size,
                                 const uint8_t *buf,
                                 int buf_size,
                                 int64_t pts,
                                 int64_t dts,
                                 int64_t pos)
         从data_buf数组中解析出一帧的数据(avPacket中存储的就是一帧数据量)
         调用一次av_parser_parse2函数就是解析出一帧的数据
         成功的话result大小等于avPacket->size大小
         */
        result = av_parser_parse2(audioAVCodecParserContext,
                                  audioAVCodecContext,
                                  &avPacket->data,
                                  &avPacket->size,
                                  data_buf,
                                  data_buf_size,
                                  AV_NOPTS_VALUE,
                                  AV_NOPTS_VALUE,
                                  0);
        fprintf(stderr, "result = %d\n", result);
        if (result < 0) {
            fprintf(stderr, "Error while parsing\n");
            exit(1);
        }

        data_buf += result;
        data_buf_size -= result;
        fprintf(stderr, "data_size = %d\n", data_buf_size);
        fprintf(stderr, "data_buf_size = %d\n", data_buf_size);
        fprintf(stderr, "avPacket->all_pkts_size = %d\n", avPacket->size);

        // 解析出来的一帧数据大小不为0表示avPacket中有数据
        if (avPacket->size) {
            decode_audio(audioAVCodecContext, avPacket, decoded_frame, outfile);
        }

        if (data_buf_size < AUDIO_REFILL_THRESH) {
            memmove(inbuf, data_buf, data_buf_size);
            data_buf = inbuf;
            len = fread(data_buf + data_buf_size, 1, AUDIO_INBUF_SIZE - data_buf_size, infile);
            if (len > 0) {
                data_buf_size += len;
            }
        }
    }

    /* flush the decoder */
    avPacket->data = NULL;
    avPacket->size = 0;
    decode_audio(audioAVCodecContext, avPacket, decoded_frame, outfile);

    fclose(outfile);
    fclose(infile);

    avcodec_free_context(&audioAVCodecContext);
    av_parser_close(audioAVCodecParserContext);
    av_frame_free(&decoded_frame);
    av_packet_free(&avPacket);

    return 0;
}

static void encode_audio(AVCodecContext *audioAVCodecContext,
                         AVFrame *needToEncodedAVFrame,
                         AVPacket *encodedAVPacket,
                         FILE *outputFile) {
    int result;

    /* send the frame for encoding */
    // 正常情况下返回结果为0
    result = avcodec_send_frame(audioAVCodecContext, needToEncodedAVFrame);
    // fprintf(stdout, "avcodec_send_frame_result = %d\n", result);
    if (result < 0) {
        fprintf(stderr, "Error sending the frame to the encoder\n");
        exit(1);
    }

    /* read all the available output packets (in general there may be any
     * number of them */
    while (result >= 0) {
        // 正常情况情况下返回结果为0,第二次返回结果可能为-11
        // 编码后的数据放在了avPacket中
        result = avcodec_receive_packet(audioAVCodecContext, encodedAVPacket);
        // fprintf(stdout, "avcodec_receive_packet_result = %d\n", result);
        if (result == AVERROR(EAGAIN) || result == AVERROR_EOF) {
            return;
        } else if (result < 0) {
            fprintf(stderr, "Error encoding audio frame\n");
            exit(1);
        }

        fwrite(encodedAVPacket->data, 1, encodedAVPacket->size, outputFile);
        av_packet_unref(encodedAVPacket);
    }
}

/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt) {
    const enum AVSampleFormat *p = codec->sample_fmts;

    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == sample_fmt)
            return 1;
        p++;
    }
    return 0;
}

/* just pick the highest supported samplerate */
static int select_sample_rate(const AVCodec *codec) {
    const int *p;
    int best_samplerate = 0;

    if (!codec->supported_samplerates)
        return 44100;

    p = codec->supported_samplerates;
    while (*p) {
        if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate))
            best_samplerate = *p;
        p++;
    }
    return best_samplerate;
}

/* select layout with the highest channel count */
static int select_channel_layout(const AVCodec *codec) {
    const uint64_t *p;
    uint64_t best_ch_layout = 0;
    int best_nb_channels = 0;

    if (!codec->channel_layouts)
        return AV_CH_LAYOUT_STEREO;

    p = codec->channel_layouts;
    while (*p) {
        int nb_channels = av_get_channel_layout_nb_channels(*p);

        if (nb_channels > best_nb_channels) {
            best_ch_layout = *p;
            best_nb_channels = nb_channels;
        }
        p++;
    }
    return best_ch_layout;
}

/***
 来自官方sample的代码
 */
int encode_audio_main(const char *infilename, const char *outfilename) {
    const AVCodec *audioAVCodecEncoder = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVFrame *needToEncodedAVFrame = NULL;
    AVPacket *avPacket = NULL;
    FILE *outputFile = NULL;
    uint16_t *samples = NULL;
    int i, j, k, result;
    float t, tincr;

    /* find the MP2 encoder */
    audioAVCodecEncoder = avcodec_find_encoder(AV_CODEC_ID_MP2);
    if (!audioAVCodecEncoder) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    audioAVCodecContext = avcodec_alloc_context3(audioAVCodecEncoder);
    if (!audioAVCodecContext) {
        fprintf(stderr, "Could not allocate audio codec context\n");
        exit(1);
    }
    /* put sample parameters */
    audioAVCodecContext->bit_rate = 64000;
    /* check that the encoder supports s16 pcm input */
    audioAVCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
    if (!check_sample_fmt(audioAVCodecEncoder, audioAVCodecContext->sample_fmt)) {
        fprintf(stderr, "Encoder does not support sample format %s",
                av_get_sample_fmt_name(audioAVCodecContext->sample_fmt));
        exit(1);
    }
    /* select other audio parameters supported by the encoder */
    audioAVCodecContext->sample_rate = select_sample_rate(audioAVCodecEncoder);
    audioAVCodecContext->channel_layout = select_channel_layout(audioAVCodecEncoder);
    audioAVCodecContext->channels = av_get_channel_layout_nb_channels(audioAVCodecContext->channel_layout);

    /* open it */
    if (avcodec_open2(audioAVCodecContext, audioAVCodecEncoder, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    outputFile = fopen(outfilename, "wb");
    if (!outputFile) {
        fprintf(stderr, "Could not open %s\n", outfilename);
        exit(1);
    }

    /* packet for holding encoded output */
    avPacket = av_packet_alloc();
    if (!avPacket) {
        fprintf(stderr, "could not allocate the packet\n");
        exit(1);
    }

    /* frame containing input raw audio */
    needToEncodedAVFrame = av_frame_alloc();
    if (!needToEncodedAVFrame) {
        fprintf(stderr, "Could not allocate audio frame\n");
        exit(1);
    }
    needToEncodedAVFrame->nb_samples = audioAVCodecContext->frame_size;
    needToEncodedAVFrame->format = audioAVCodecContext->sample_fmt;
    needToEncodedAVFrame->channel_layout = audioAVCodecContext->channel_layout;
    /* allocate the data buffers */
    result = av_frame_get_buffer(needToEncodedAVFrame, 0);
    if (result < 0) {
        fprintf(stderr, "Could not allocate audio data buffers\n");
        exit(1);
    }

    /* encode a single tone sound */
    t = 0;
    tincr = 2 * M_PI * 440.0 / audioAVCodecContext->sample_rate;
    for (i = 0; i < 2000; i++) {
        /* make sure the frame is writable -- makes a copy if the encoder
         * kept a reference internally */
        result = av_frame_make_writable(needToEncodedAVFrame);
        if (result < 0) {
            exit(1);
        }
        samples = (uint16_t *) needToEncodedAVFrame->data[0];

        for (j = 0; j < audioAVCodecContext->frame_size; j++) {
            samples[2 * j] = (int) (sin(t) * 10000);

            for (k = 1; k < audioAVCodecContext->channels; k++) {
                samples[2 * j + k] = samples[2 * j];
            }
            t += tincr;
        }
        encode_audio(audioAVCodecContext, needToEncodedAVFrame, avPacket, outputFile);
    }

    /* flush the encoder */
    encode_audio(audioAVCodecContext, NULL, avPacket, outputFile);

    fclose(outputFile);

    av_frame_free(&needToEncodedAVFrame);
    av_packet_free(&avPacket);
    avcodec_free_context(&audioAVCodecContext);

    return 0;
}

static void pgm_save(unsigned char *buf,
                     int wrap,
                     int xsize,
                     int ysize,
                     char *output_file_name) {
    FILE *f;
    int i;

    f = fopen(output_file_name, "w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++) {
        fwrite(buf + i * wrap, 1, xsize, f);
    }
    fclose(f);
}

static void decode_video(AVCodecContext *video_av_codec_context,
                         AVFrame *av_frame,
                         AVPacket *av_packet,
                         const char *output_file_name) {
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(video_av_codec_context, av_packet);
    fprintf(stdout, "result = %d\n", ret);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(video_av_codec_context, av_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", video_av_codec_context->frame_number);
        fflush(stdout);

        /* the picture is allocated by the decoder. no need to
           free it */
        snprintf(buf, sizeof(buf), "%s-%d", output_file_name, video_av_codec_context->frame_number);
        printf("buf: %s\n", buf);
        pgm_save(av_frame->data[0], av_frame->linesize[0],
                 av_frame->width, av_frame->height, buf);
    }
}

/***
 来自官方sample的代码
 代码执行有问题
 */
int decode_video_main(const char *input_file_name, const char *output_file_name) {
    const AVCodec *video_av_codec;
    AVCodecParserContext *video_av_codec_parser_context;
    AVCodecContext *video_av_codec_context = NULL;
    FILE *input_file;
    AVPacket *av_packet;
    AVFrame *decoded_av_frame;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data_buf;
    size_t data_buf_size;
    int result, len;

    av_packet = av_packet_alloc();
    if (!av_packet)
        exit(1);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
//    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    /* find the MPEG-1 video decoder */
    video_av_codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
    if (!video_av_codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    video_av_codec_parser_context = av_parser_init(video_av_codec->id);
    if (!video_av_codec_parser_context) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    video_av_codec_context = avcodec_alloc_context3(video_av_codec);
    if (!video_av_codec_context) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

    /* open it */
    if (avcodec_open2(video_av_codec_context, video_av_codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    input_file = fopen(input_file_name, "rb");
    if (!input_file) {
        fprintf(stderr, "Could not open %s\n", input_file_name);
        exit(1);
    }

    decoded_av_frame = av_frame_alloc();
    if (!decoded_av_frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    while (!feof(input_file)) {
        //read raw data from the input file
        data_buf_size = fread(inbuf, 1, INBUF_SIZE, input_file);
        if (!data_buf_size) {
            break;
        }

        fprintf(stdout, "data_buf_size = %d\n", data_buf_size);

        //use the parser to split the data into frames
        data_buf = inbuf;
        while (data_buf_size > 0) {
            result = av_parser_parse2(video_av_codec_parser_context, video_av_codec_context,
                                      &(av_packet->data), &(av_packet->size),
                                      data_buf, data_buf_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (result < 0) {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            fprintf(stdout, "result = %d\n", result);
            data_buf += result;
            data_buf_size -= result;
            fprintf(stdout, "data_buf_size = %d\n", data_buf_size);
            fprintf(stdout, "av_packet->all_pkts_size = %d\n", av_packet->size);

            if (av_packet->size) {
                decode_video(video_av_codec_context, decoded_av_frame, av_packet, output_file_name);
            }
        }
    }

    /* flush the decoder */
    decode_video(video_av_codec_context, decoded_av_frame, NULL, output_file_name);

    fclose(input_file);

    av_parser_close(video_av_codec_parser_context);
    avcodec_free_context(&video_av_codec_context);
    av_frame_free(&decoded_av_frame);
    av_packet_free(&av_packet);

    return 0;
}

static void encode_video(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                         FILE *outfile) {
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3" PRId64 "\n", frame->pts);

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

        printf("Write packet %3" PRId64 " (all_pkts_size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

/***
 来自官方sample的代码
 */
int encode_video_main(const char *codec_name, const char *output_file_name) {
    const AVCodec *codec;
    AVCodecContext *c = NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = {0, 0, 1, 0xb7};

    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 352;
    c->height = 288;
    /* frames per second */
    c->time_base = (AVRational) {1, 25};
    c->framerate = (AVRational) {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        // fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        fprintf(stderr, "Could not open codec: %d\n", ret);
        exit(1);
    }

    f = fopen(output_file_name, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", output_file_name);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width = c->width;
    frame->height = c->height;

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
        for (y = 0; y < c->height; y++) {
            for (x = 0; x < c->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
            }
        }

        /* Cb and Cr */
        for (y = 0; y < c->height / 2; y++) {
            for (x = 0; x < c->width / 2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }

        frame->pts = i;

        /* encode the image */
        encode_video(c, frame, pkt, f);
    }

    /* flush the encoder */
    encode_video(c, NULL, pkt, f);

    /* add sequence end code to have a real MPEG file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}

// 根据样本格式返回样本格式字符串
static int get_format_from_sample_fmt(const char **fmt,
                                      enum AVSampleFormat sample_fmt) {
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt;
        const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
            {AV_SAMPLE_FMT_U8,  "u8",    "u8"},
            {AV_SAMPLE_FMT_S16, "s16be", "s16le"},
            {AV_SAMPLE_FMT_S32, "s32be", "s32le"},
            {AV_SAMPLE_FMT_FLT, "f32be", "f32le"},
            {AV_SAMPLE_FMT_DBL, "f64be", "f64le"},
    };
    *fmt = NULL;

    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }

    fprintf(stderr,
            "Sample format %s not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return AVERROR(EINVAL);
}

/**
 * Fill dst buffer with nb_samples, generated starting from t.
 *相当于是声源 产生一个正弦波形的声波
 * dst 保存声音数据返回个调用者 nb_samples 采用的样本数 nb_channels 声音通道数，表明单次采样的样本数 t采用开始时间
 *正弦波形就是一个生源，实际中复杂的声音都是通过波形叠加成的。
 *以 sample_rate采样率，从时间t开始采样，采样通道为2，每个通道的数据相同，从频率为440HZ的波形上采样，形成声源
 */
static void fill_samples(double *dst, int nb_samples, int nb_channels, int sample_rate, double *t) {
    int i, j;
    //采样时间间隔    tincr
    double tincr = 1.0 / sample_rate, *dstp = dst;
    //正弦波y=Asin（ωx+φ）+h 最小正周期T=2π/|ω| 所以440HZ是正弦波的频率
    const double c = 2 * M_PI * 440.0;

    /* generate sin tone with 440Hz frequency and duplicated channels */
    //填充每个通道数据 采用交叉存储
    for (i = 0; i < nb_samples; i++) {
        *dstp = sin(c * (*t));
        for (j = 1; j < nb_channels; j++)
            dstp[j] = dstp[0];
        dstp += nb_channels;
        *t += tincr;
    }
}

/***
 重采样代码(官方的,比较好)
 ffplay -f s16le -channel_layout 7 -channels 3 -ar 44100 /root/音乐/dst_filename
 大致过程:
    1.
    为目标数据设置好几个参数:
    channel_layout(声道布局:单声道,双声道,环绕)
    sample_format(采样格式)
    sample_rate(采样率)
    2.
    初始化SwrContext结构体(需要设置一些参数和申请空间)
    3.
    根据channel_layout参数使用
    av_get_channel_layout_nb_channels函数
    得到nb_channels(双声道为2,环绕为3)
    4.
    使用av_rescale_rnd函数得到目标nb_samples
    5.
    使用av_samples_alloc_array_and_samples函数
    为装"源数据,目标数据"的容器申请空间
    6.
    进入循环处理:
 */
int resampling_audio(const char *dst_filename) {
    // AV_CH_LAYOUT_STEREO 声音布局立体声 	  AV_CH_LAYOUT_SURROUND 声音布局环绕立体声
    int64_t src_ch_layout = AV_CH_LAYOUT_STEREO, dst_ch_layout = AV_CH_LAYOUT_SURROUND;
    // 采样率
    int src_sample_rate = 48000, dst_sample_rate = 44100;
    // 采样格式
    //样本存储格式
    enum AVSampleFormat src_sample_fmt = AV_SAMPLE_FMT_DBL, dst_sample_fmt = AV_SAMPLE_FMT_S16;

    // 上面几个的目标参数可以先设定好

    // 声道数
    int src_nb_channels = 0, dst_nb_channels = 0;
    // 一帧音频中的采样个数，用于计算一帧数据大小
    //每次采用样本数
    int src_nb_samples = 1024, dst_nb_samples, max_dst_nb_samples;

    uint8_t **src_data = NULL, **dst_data = NULL;
    int src_linesize, dst_linesize;
    FILE *dst_file;
    int dst_bufsize;
    const char *fmt;
    //重采样上下文
    struct SwrContext *swr_context;
    double t;
    int ret;

    fprintf(stdout, "src_ch_layout = %d\n", src_ch_layout);// 3
    fprintf(stdout, "dst_ch_layout = %d\n", dst_ch_layout);// 7
    fprintf(stdout, "src_sample_fmt = %d\n", src_sample_fmt);// 4
    fprintf(stdout, "dst_sample_fmt = %d\n", dst_sample_fmt);// 1

    dst_file = fopen(dst_filename, "wb");
    if (!dst_file) {
        fprintf(stderr, "Could not open destination file %s\n", dst_filename);
        exit(1);
    }

    // 初始化SwrContext结构体
    /* create resampler context */
    swr_context = swr_alloc();
    if (!swr_context) {
        fprintf(stderr, "Could not allocate resampler context\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }
    /* set options */
    av_opt_set_int(swr_context, "in_channel_layout", src_ch_layout, 0);
    av_opt_set_int(swr_context, "out_channel_layout", dst_ch_layout, 0);

    av_opt_set_int(swr_context, "in_sample_rate", src_sample_rate, 0);
    av_opt_set_int(swr_context, "out_sample_rate", dst_sample_rate, 0);

    av_opt_set_sample_fmt(swr_context, "in_sample_fmt", src_sample_fmt, 0);
    av_opt_set_sample_fmt(swr_context, "out_sample_fmt", dst_sample_fmt, 0);
    /* initialize the resampling context */
    if ((ret = swr_init(swr_context)) < 0) {
        fprintf(stderr, "Failed to initialize the resampling context\n");
        goto end;
    }

    /* allocate source and destination samples buffers */
    //获取源通道数
    src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);
    //分配源声音所需要空间  src_linesize=	 src_nb_channels× src_nb_samples×sizeof(double)
    ret = av_samples_alloc_array_and_samples(&src_data, &src_linesize, src_nb_channels,
                                             src_nb_samples, src_sample_fmt, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate source samples\n");
        goto end;
    }

    /* compute the number of converted samples: buffering is avoided
     * ensuring that the output buffer will contain at least all the
     * converted input samples */
    //计算目标样本数  转换前后的样本数不一样  抓住一点 采样时间相等
    //src_nb_samples/src_rate=dst_nb_samples/dst_rate
    max_dst_nb_samples = dst_nb_samples =
            av_rescale_rnd(src_nb_samples, dst_sample_rate, src_sample_rate, AV_ROUND_UP);


    /* buffer is going to be directly written to a rawaudio file, no alignment */
    dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);
    // 根据dst_nb_channels,dst_nb_samples和dst_sample_fmt为dst_data申请空间
    ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
                                             dst_nb_samples, dst_sample_fmt, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate destination samples\n");
        goto end;
    }

    fprintf(stdout, "src_nb_channels = %d\n", src_nb_channels);// 2
    fprintf(stdout, "dst_nb_channels = %d\n", dst_nb_channels);// 3

    fprintf(stdout, "src_linesize = %d\n", src_linesize);// 16384
    fprintf(stdout, "src_data_size = %d\n", ret);// 16384

    fprintf(stdout, "dst_linesize = %d\n", dst_linesize);// 5760
    fprintf(stdout, "dst_data_size = %d\n", ret);// 5760

    fprintf(stdout, "src_nb_samples = %d\n", src_nb_samples);// 1024
    fprintf(stdout, "dst_nb_samples = %d\n", dst_nb_samples);// 941
    fprintf(stdout, "max_dst_nb_samples = %d\n", max_dst_nb_samples);// 941

    t = 0;
    do {
        /* generate synthetic audio */
        // 创造数据
        fill_samples((double *) src_data[0], src_nb_samples, src_nb_channels, src_sample_rate, &t);

        /* compute destination number of samples */
        // 可以理解成装目标数据的容器的空间够不够用了,不够用的话,需要重新申请空间
        //swr_get_delay(audio_swr_context, src_rate)延迟时间 源采样率为单位的样本数
        dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_context, src_sample_rate) + src_nb_samples,
                                        dst_sample_rate, src_sample_rate, AV_ROUND_UP);
        if (dst_nb_samples > max_dst_nb_samples) {
            // 先释放再申请空间(因为原来申请的空间不够使用了)
            av_freep(&dst_data[0]);
            ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels,
                                   dst_nb_samples, dst_sample_fmt, 1);
            if (ret < 0)
                break;
            max_dst_nb_samples = dst_nb_samples;
        }

        /***
         convert to destination format
         int swr_convert(struct SwrContext *s, uint8_t **out, int out_count,
                                const uint8_t **in , int in_count)
         */
        //ret 实际转换得到的样本数
        ret = swr_convert(swr_context, dst_data, dst_nb_samples,
                          (const uint8_t **) src_data, src_nb_samples);
        if (ret < 0) {
            fprintf(stderr, "Error while converting\n");
            goto end;
        }
        // 需要计算出本次转换后音频所占用的字节数,因为在写的时候要用到这个变量
        dst_bufsize = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
                                                 ret, dst_sample_fmt, 1);
        if (dst_bufsize < 0) {
            fprintf(stderr, "Could not get sample buffer all_pkts_size\n");
            goto end;
        }
        printf("t:%f in:%d out:%d dst_bufsize:%d\n", t, src_nb_samples, ret, dst_bufsize);
        // 写入数据,生成目标文件
        fwrite(dst_data[0], 1, dst_bufsize, dst_file);
    } while (t < 10);// 大约10秒

    if ((ret = get_format_from_sample_fmt(&fmt, dst_sample_fmt)) < 0)
        goto end;

    fprintf(stdout, "Resampling succeeded. Play the output file with the command:\n"
                    "ffplay -f %s -channel_layout %" PRId64 " -channels %d -ar %d %s\n",
            fmt, dst_ch_layout, dst_nb_channels, dst_sample_rate, dst_filename);

    end:
    fclose(dst_file);

    if (src_data)
        av_freep(&src_data[0]);
    av_freep(&src_data);

    if (dst_data)
        av_freep(&dst_data[0]);
    av_freep(&dst_data);

    swr_free(&swr_context);
    return ret < 0;
}


/***
 网上的代码(也是修改了官方的代码)
 */
int resampling_audio2(const char *dst_filename) {

}

int test(int) {
    return 0;
}

/*
 * ffmpeg的函数av_samples_get_buffer_size分析，对齐的意思
 *
 * 此函数只应用于音频。
 * 计算出：要把一系列的样本保存起来，需要多大的缓存。
 * sample，单个通道的单次采样所得到的样本数据。
 * planar，和yuv存储格式一样，声音也分平面（planar）和打包（packed）两种存储格式。
 * line_size，一个平面的buffer大小(packed格式，可以看成是特殊地只有一个平面)。
 * align，指定一个平面的buffer大小的对齐数，单位为字节；
 *      “0 = default”，常用于提供冗余的大小，此函数之后常常跟着的是类malloc函数。
 *      “1 = no alignment”，不对齐，其实即是按1字节对齐，也就是说，求的是音频数据的真实大小，
 *      常常用来计算出刚刚(调用此函数之前)转码出来的数据大小。
 */

/**
* Get the required buffer size for the given audio parameters.
*
* @param[out] linesize calculated linesize, may be NULL
* @param nb_channels   the number of channels
* @param nb_samples    the number of samples in a single channel
* @param sample_fmt    the sample format
* @param align         buffer size alignment (0 = default, 1 = no alignment)
* @return              required buffer size, or negative error code on failure
*/
//int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
//                               enum AVSampleFormat sample_fmt, int align) {
//    int line_size;
//    int sample_size = av_get_bytes_per_sample(sample_fmt); //获取一个样本的大小
//    int planar = av_sample_fmt_is_planar(sample_fmt); //检测是哪种存放格式
//
//    /* validate parameter ranges */
//    if (!sample_size || nb_samples <= 0 || nb_channels <= 0) //严谨地，做一下参数校验
//        return AVERROR(EINVAL);
//
//    /* auto-select alignment if not specified */
//    if (!align) {
//        if (nb_samples > INT_MAX - 31)
//            return AVERROR(EINVAL);
//        align = 1; //按1对齐0
//        nb_samples = FFALIGN(nb_samples, 32); //对齐后，一個通道里，可能比真实的样本数量多出31个。
//    }
//
//    //这里看不懂。
//    /* check for integer overflow */
//    if (nb_channels > INT_MAX / align ||
//        (int64_t) nb_channels * nb_samples > (INT_MAX - (align * nb_channels)) / sample_size)
//        return AVERROR(EINVAL);
//
//    //对齐的意义在这里。
//    line_size = planar ? FFALIGN(nb_samples * sample_size, align) :
//                FFALIGN(nb_samples * sample_size * nb_channels, align);
//    if (linesize)
//        *linesize = line_size;
//
//    //返回：能存放全部通道的所有样本数量的音频数据的buffer大小
//    return planar ? line_size * nb_channels : line_size;
//}

#endif //MYSTUDY_FFMPEG_SAMPLE