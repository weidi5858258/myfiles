//
// Created by root on 18-7-12.
//

#ifndef MYSTUDY_FFMPEG
#define MYSTUDY_FFMPEG

#include "include/MyHead.h"

#define RECORDING_SCREEN 0

void saveImage(AVFrame *dstAVFrame, int width, int height, int iFrame);

int handleVideo(AVCodecContext *videoAVCodecContext,
                SwsContext *videoSwsContext,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame,
                int srcW, int srcH,
                FILE *outputYUVFile,
                int &imageCount);

int handleAudio(AVCodecContext *audioAVCodecContext,
                SwrContext *audioSwrContext,
                AVSampleFormat audioAVSampleFormat,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame);

/***
 只用到解码器,没有用到编码器
 */
int decoderVideoFrameToImage() {
    ////////////////////////////音视频公共部分////////////////////////////
    // char filePath[] = "/root/视频/10_APITest_MKV-HEVC.mkv";
    char filePath[] = "/media/root/windows-d/Tools/apache-tomcat-8.5.23/webapps/ROOT/video/xunnu.mp4";
    // 格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
    AVFormatContext *avFormatContext = NULL;
    //srcAVFrame保存原始帧
    //dstAVFrame转换成yuv后的帧保存在此处
    AVFrame *srcAVFrame = NULL, *dstAVFrame = NULL;
    // AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
    AVPacket *avPacket = NULL;
    uint8_t *out_buffer;
    FILE *outputYUVFile = NULL;

#if OUTPUT_YUV420P
    outputYUVFile = fopen("/root/视频/output.yuv", "wb+");
#endif

    //在linux录屏，使用x11grab
    //在windows下需要gdigrab或dshow(需要安装抓屏软件：screen-capture-recorder)
    // mac下使用avfoundation，见后文
    AVInputFormat *avInputFormat = NULL;
    //Linux
    AVDictionary *avDictionary = NULL;

    // start
    // 注册复用器和编解码器，所有的使用ffmpeg，首先必须调用这个函数
    av_register_all();
    // 用于从网络接收数据，如果不是网络接收数据，可不用（如本例可不用）
    avformat_network_init();
    // 注册设备的函数，如用获取摄像头数据或音频等，需要此函数先注册
    // avdevice_register_all();
    // AVFormatContext初始化，里面设置结构体的一些默认信息
    avFormatContext = avformat_alloc_context();

#if RECORDING_SCREEN
    //av_dict_set(&amp;options,"framerate","5",0);
    //av_dict_set(&amp;options,"offset_x","20",0);
    //av_dict_set(&amp;options,"offset_y","40",0);
    //设置录屏参数，如录屏图像的大小、录屏帧率等
    //不设置代表全屏，但实际测试时，不设置默认大小为640x480
    /*av_dict_set(&avDictionary, "video_size", "640x480", 0);
    avInputFormat = av_find_input_format("x11grab");
    if (avformat_open_input(&avFormatContext,
                            ":0.0+10,20",
                            avInputFormat,
                            &avDictionary) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }*/
#else
    // 打开文件
    if (avformat_open_input(&avFormatContext, filePath, NULL, NULL)) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
#endif

    //寻找到获取的流
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    // 申请内存
    srcAVFrame = av_frame_alloc();
    dstAVFrame = av_frame_alloc();
    //AVPacket代表编码后的一个包，即一帧编码为一个包
    avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));

    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
    int nb_streams = avFormatContext->nb_streams;
    printf("avFormatContext->nb_streams = %d\n", nb_streams);// 2
    int i;
    for (i = 0; i < nb_streams; i++) {
        AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
        if (avMediaType == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            printf("videoStreamIndex = %d\n", videoStreamIndex);
        } else if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = avMediaType;
            printf("audioStreamIndex = %d\n", audioStreamIndex);
        }
    }
    // 至少要有一个视频流或者音频流
    if (videoStreamIndex == -1 || audioStreamIndex == -1) {
        printf("Didn't find a video or audio stream.\n");
        return -1;
    }

    ////////////////////////////视频部分////////////////////////////
#if NEED_VIDEO
    // 源视频宽高
    int scrW = 0, scrH = 0;
    // 颜色转换器
    AVCodecContext *videoAVCodecContext = NULL;
    AVCodecParameters *videoAVCodecParameters = NULL;
    // Decoder
    AVCodec *videoAVCodecDecoder = NULL;
    // Encoder
    AVCodec *videoAVCodecEncoder = NULL;
    // SwsContext作为sws_scale的第一个参数,记录数据要转换的格式、大小及转换方式(用于图像像素的处理)
    struct SwsContext *videoSwsContext = NULL;

    if (videoStreamIndex != -1) {
        videoAVCodecParameters = avFormatContext->streams[videoStreamIndex]->codecpar;
    }

    // 视频的宽高
    scrW = videoAVCodecParameters->width;// 1280
    scrH = videoAVCodecParameters->height;// 720
    printf("screen_w = %d, screen_h = %d\n", scrW, scrH);

    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    videoAVCodecDecoder = avcodec_find_decoder(videoAVCodecParameters->codec_id);
    if (videoAVCodecDecoder == NULL) {
        printf("Video CodecDecoder not found.\n");
        return -1;
    }

    // 必须要申请内存
    videoAVCodecContext = avcodec_alloc_context3(NULL);
    if (videoAVCodecContext == NULL) {
        printf("videoAVCodecContext is NULL.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(videoAVCodecContext, videoAVCodecParameters) < 0) {
        printf("Copy videoAVCodecParameters to context fail.\n");
        return -1;
    }

    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(videoAVCodecContext, videoAVCodecDecoder, NULL) < 0) {
        printf("Could not open Video CodecDecoder.\n");
        return -1;
    }

    /***
     AVPixelFormat像素格式:想要转换成什么样的格式(下面三个地方需要设置)
     AV_PIX_FMT_RGB24把视频保存成一张张图片时需要设置的格式
     FFmpeg有一个专门用于描述像素格式的结构体AVPixFmtDescriptor.该结构体的定义位于libavutil\pixdesc.h
     const AVPixFmtDescriptor *av_pix_fmt_desc_get(enum AVPixelFormat pix_fmt);
     通过下面的函数可以获得指定像素格式每个像素占用的比特数（Bit Per Pixel）
     int av_get_bits_per_pixel(const AVPixFmtDescriptor *pixdesc);

     视频像素转化三部曲(如从YUV420P的像素格式转化成RGB24的像素格式)：
     sws_getContext()：初始化一个SwsContext
     sws_scale()：处理图像数据,用于转换像素
     sws_freeContext()：释放一个SwsContext

     想要转化成的视频像素格式
     */
    AVPixelFormat dstFormat = AV_PIX_FMT_NONE;
#if OUTPUT_IMAGE
    dstFormat = AV_PIX_FMT_RGB24;
#else
    dstFormat = AV_PIX_FMT_YUV420P;
#endif
    printf("源图像的像素格式：   %d\n", videoAVCodecContext->pix_fmt);
    printf("目标图像的像素格式： %d\n", dstFormat);
    // 下面这部分代码还不确定是否需要对音频也做这样的处理,因为音频也用到了dstAVFrame
    videoSwsContext = sws_getContext(scrW, scrH,
                                     videoAVCodecContext->pix_fmt,
                                     scrW, scrH,
                                     dstFormat,
                                     SWS_BICUBIC,// SWS_POINT设定图像拉伸使用的算法
                                     NULL, NULL, NULL);
    if (videoSwsContext == NULL) {
        printf("Cannot initialize the conversion context!\n");
        return -1;
    }
    int av_image_get_buffer_size_ = av_image_get_buffer_size(dstFormat, scrW, scrH, 1);
    printf("av_image_get_buffer_size_ = %d\n", av_image_get_buffer_size_);// 1382400
    out_buffer = (uint8_t *) av_malloc(av_image_get_buffer_size_);
    // 使用av_image_fill_arrays函数把目标帧和新申请的内存结合起来
    av_image_fill_arrays(dstAVFrame->data,
                         dstAVFrame->linesize,
                         out_buffer,
                         dstFormat,
                         scrW, scrH,
                         1);// 设置成“1”就好了
#endif

    ////////////////////////////音频部分////////////////////////////
#if NEED_AUDIO
    AVCodecContext *audioAVCodecContext = NULL;
    AVCodecParameters *audioAVCodecParameters = NULL;
    // Decoder
    AVCodec *audioAVCodecDecoder = NULL;
    // Encoder
    AVCodec *audioAVCodecEncoder = NULL;
    AVSampleFormat audioAVSampleFormat;
    struct SwrContext *audioSwrContext = NULL;

    if (audioStreamIndex != -1) {
        audioAVCodecParameters = avFormatContext->streams[audioStreamIndex]->codecpar;
    }

    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    audioAVCodecDecoder = avcodec_find_decoder(audioAVCodecParameters->codec_id);
    if (audioAVCodecDecoder == NULL) {
        printf("Audio CodecDecoder not found.\n");
        return -1;
    }

    // 必须要申请内存
    audioAVCodecContext = avcodec_alloc_context3(NULL);
    if (audioAVCodecContext == NULL) {
        printf("audioAVCodecContext is NULL.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(audioAVCodecContext, audioAVCodecParameters) < 0) {
        printf("Copy audioAVCodecParameters to context fail.\n");
        return -1;
    }

    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
        printf("Could not open Audio CodecDecoder.\n");
        return -1;
    }

    //////////////////////////////////声音的初始化工作//////////////////////////////////

    audioAVSampleFormat = audioAVCodecContext->sample_fmt;
    int sample_rate = audioAVCodecContext->sample_rate;
    int channels = audioAVCodecContext->channels;
    printf("audioAVSampleFormat = %d\n", audioAVSampleFormat);
    printf("audio sample_rate = %d\n", sample_rate);
    printf("audio channels = %d\n", channels);// 2(左声道,右声道)

    // AAC:1024  MP3:1152
    int out_nb_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    int out_nb_samples = audioAVCodecContext->frame_size;
    printf("audio out_nb_channels = %d\n", out_nb_channels);
    printf("audio out_nb_samples = %d\n", out_nb_samples);
    int out_buffer_size = av_samples_get_buffer_size(NULL,
                                                     out_nb_channels,
                                                     out_nb_samples,
                                                     AV_SAMPLE_FMT_S16,
                                                     1);

    // FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout = av_get_default_channel_layout(channels);
    // Swr init
    audioSwrContext = swr_alloc();
    audioSwrContext = swr_alloc_set_opts(audioSwrContext,
                                         AV_CH_LAYOUT_STEREO,
                                         AV_SAMPLE_FMT_S16,
                                         44100,
                                         in_channel_layout,
                                         audioAVSampleFormat,
                                         sample_rate,
                                         0,
                                         NULL);
    swr_init(audioSwrContext);
#endif

    printf("Entry while loop start.\n");
    int result = -1;
    int imageCount = 0;
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        // printf("While loop avPacket->stream_index = %d\n", avPacket->stream_index);
#if NEED_VIDEO
        if (avPacket->stream_index == videoStreamIndex) {// 取视频数据
            result = handleVideo(videoAVCodecContext, videoSwsContext,
                                 avPacket, srcAVFrame, dstAVFrame,
                                 scrW, scrH,
                                 outputYUVFile,
                                 imageCount);
            // printf("While loop video result = %d\n", result);
            av_packet_unref(avPacket);
            if (result) {
                // printf("Entry while loop video break. Because result = %d\n", result);
                break;
            }
        }
#endif
#if NEED_AUDIO
        if (avPacket->stream_index == audioStreamIndex) {// 取音频数据
            result = handleAudio(audioAVCodecContext, audioSwrContext, audioAVSampleFormat,
                                 avPacket, srcAVFrame, dstAVFrame);
            // printf("While loop audio result = %d\n", result);
            av_packet_unref(avPacket);
            if (result) {
                // printf("Entry while loop audio break. Because result = %d\n", result);
                break;
            }
        }
#endif
    }
    printf("Entry while loop end.\n");

    // 清理工作
#if NEED_VIDEO
    av_free(out_buffer);
    sws_freeContext(videoSwsContext);
    avcodec_close(videoAVCodecContext);
    av_free(videoAVCodecContext);
    avcodec_parameters_free(&videoAVCodecParameters);
#endif
#if NEED_AUDIO
    swr_free(&audioSwrContext);
    swr_close(audioSwrContext);
    avcodec_close(audioAVCodecContext);
    av_free(audioAVCodecContext);
    avcodec_parameters_free(&audioAVCodecParameters);
#endif
    av_frame_free(&srcAVFrame);
    av_frame_free(&dstAVFrame);
    avformat_close_input(&avFormatContext);

#if OUTPUT_YUV420P
    fclose(outputYUVFile);
#endif

    return 0;
}

int handleVideo(AVCodecContext *videoAVCodecContext,
                SwsContext *videoSwsContext,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame,
                int srcW, int srcH,
                FILE *outputYUVFile,
                int &imageCount) {
    int result = avcodec_send_packet(videoAVCodecContext, avPacket);
    // printf("video result = %d\n", result);
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
    // 一般情况下while循环也只执行一次
    while (avcodec_receive_frame(videoAVCodecContext, srcAVFrame) >= 0) {
        //转换，把源数据srcAVFrame转换成dstAVFrame，dstAVFrame由前面设置格式为AV_PIX_FMT_YUV420P
        sws_scale(videoSwsContext,
                  (const uint8_t *const *) srcAVFrame->data,
                  srcAVFrame->linesize,
                  0,
                  srcH,
                  dstAVFrame->data,
                  dstAVFrame->linesize);

#if OUTPUT_YUV420P
        fwrite(dstAVFrame->data[0], 1, screen_w * screen_h, outputYUVFile);        //Y
        fwrite(dstAVFrame->data[1], 1, (screen_w * screen_h) / 4, outputYUVFile);  //U
        fwrite(dstAVFrame->data[2], 1, (screen_w * screen_h) / 4, outputYUVFile);  //V
#endif

#if OUTPUT_IMAGE
        // 输出多少张图片
        if (++imageCount <= 5) {
            saveImage(dstAVFrame, srcW, srcH, imageCount);
        } else {
            return 1;
        }
#endif
    }
    return 0;
}

int handleAudio(AVCodecContext *audioAVCodecContext,
                SwrContext *audioSwrContext,
                AVSampleFormat audioAVSampleFormat,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame) {
    int result = avcodec_send_packet(audioAVCodecContext, avPacket);
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
    // 一般情况下while循环也只执行一次
    while (avcodec_receive_frame(audioAVCodecContext, srcAVFrame) >= 0) {
        // Audacity: 16bit PCM little endian stereo
        if (audioAVSampleFormat == AV_SAMPLE_FMT_S16P) {// 6

            //Audacity: big endian 32bit stereo start offset 7 (but has noise)
        } else if (audioAVSampleFormat == AV_SAMPLE_FMT_FLTP) {// 8
            swr_convert(audioSwrContext,
                        (uint8_t **) dstAVFrame->data,
                        MAX_AUDIO_FRAME_SIZE,
                        (const uint8_t **) srcAVFrame->data,
                        srcAVFrame->nb_samples);

            /*printf("index:%5d\tavPacket->pts:%lld\tavPacket->size:%d\n",
                   index, avPacket->pts, avPacket->size);*/
            printf("avPacket->pts:%lld\tavPacket->dts:%lld\tavPacket->size:%d\n",
                   avPacket->pts, avPacket->dts, avPacket->size);
        }
    }
    return 0;
}

/***
把一个视频解码后保存为一张张的图片
 */
void saveImage(AVFrame *dstAVFrame, int width, int height, int iFrame) {
    FILE *dstFile;
    char dstFilePath[32];
    int y;

    sprintf(dstFilePath, "/root/图片/video_to_image/frame%d.ppm", iFrame);
    // 打开文件
    dstFile = fopen(dstFilePath, "wb");
    if (dstFile == NULL) {
        printf("dstFile = NULL\n");
        return;
    }
    // 写头
    fprintf(dstFile, "P6\n%d %d\n255\n", width, height);

    // 写pixel数据
    for (y = 0; y < height; y++) {
        fwrite(dstAVFrame->data[0] + y * dstAVFrame->linesize[0], width * 3, 1, dstFile);
    }
    // 关闭文件
    fclose(dstFile);
}

static void decode(AVCodecContext *audioAVCodecContext,
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
        // 一个sample多少个字节
        data_size = av_get_bytes_per_sample(audioAVCodecContext->sample_fmt);
        // fprintf(stdout, "While loop data_size = %d\n", data_size);// 4
        if (data_size < 0) {
            /* This should not occur, checking just for paranoia */
            fprintf(stderr, "Failed to calculate data size\n");
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

int decode_audio(const char *infilename, const char *outfilename) {
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
    fprintf(stderr, "data_size = %d\n", data_buf_size);

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
        fprintf(stderr, "avPacket->size = %d\n", avPacket->size);

        // 解析出来的一帧数据大小不为0表示avPacket中有数据
        if (avPacket->size) {
            decode(audioAVCodecContext, avPacket, decoded_frame, outfile);
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
    decode(audioAVCodecContext, avPacket, decoded_frame, outfile);

    fclose(outfile);
    fclose(infile);

    avcodec_free_context(&audioAVCodecContext);
    av_parser_close(audioAVCodecParserContext);
    av_frame_free(&decoded_frame);
    av_packet_free(&avPacket);

    return 0;
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

static void encode(AVCodecContext *audioAVCodecContext,
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

int encode_audio(const char *infilename, const char *outfilename) {
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
        encode(audioAVCodecContext, needToEncodedAVFrame, avPacket, outputFile);
    }

    /* flush the encoder */
    encode(audioAVCodecContext, NULL, avPacket, outputFile);

    fclose(outputFile);

    av_frame_free(&needToEncodedAVFrame);
    av_packet_free(&avPacket);
    avcodec_free_context(&audioAVCodecContext);

    return 0;
}

/***
AVFormatContext *avFormatContext = NULL;
// 颜色转换器
AVCodecContext *videoAVCodecContext = NULL;
AVCodecContext *audioAVCodecContext = NULL;
AVCodecParameters *videoAVCodecParameters = NULL;
AVCodecParameters *audioAVCodecParameters = NULL;
// decoder
AVCodec *videoAVCodecDecoder = NULL;
AVCodec *audioAVCodecDecoder = NULL;
// encoder
AVCodec *videoAVCodecEncoder = NULL;
AVCodec *audioAVCodecEncoder = NULL;
// audio
AVSampleFormat audioAVSampleFormat;

AVFrame *srcAVFrame = NULL, *dstAVFrame = NULL;
// AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
AVPacket *avPacket = NULL;
struct SwsContext *videoSwsContext;
struct SwrContext *audioSwrContext;

avFormatContext = avformat_alloc_context();

int nb_streams = avFormatContext->nb_streams;
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

videoAVCodecDecoder = avcodec_find_decoder(videoAVCodecParameters->codec_id);
audioAVCodecDecoder = avcodec_find_decoder(audioAVCodecParameters->codec_id);

videoAVCodecContext = avcodec_alloc_context3(NULL);
audioAVCodecContext = avcodec_alloc_context3(NULL);
if (avcodec_parameters_to_context(videoAVCodecContext, videoAVCodecParameters) < 0) {
    printf("copy videoAVCodecParameters to context fail\n");
    return -1;
}
if (avcodec_parameters_to_context(audioAVCodecContext, audioAVCodecParameters) < 0) {
    printf("copy audioAVCodecParameters to context fail\n");
    return -1;
}

if (avcodec_open2(videoAVCodecContext, videoAVCodecDecoder, NULL) < 0
    || avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
    printf("Could not open codec.\n");
    return -1;
}

srcAVFrame = av_frame_alloc();
dstAVFrame = av_frame_alloc();
avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
out_buffer = (uint8_t *) av_malloc(av_image_get_buffer_size_);

audioAVSampleFormat = audioAVCodecContext->sample_fmt;






















































































*/

#endif //MYSTUDY_FFMPEG