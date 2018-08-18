//
// Created by root on 18-7-13.
//

/***
 保存YUV420P格式的数据，用以下代码：
 fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),1,output);
 fwrite(pFrameYUV->data[1],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
 fwrite(pFrameYUV->data[2],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
 保存RGB24格式的数据，用以下代码：
 fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height)*3,1,output);
 保存UYVY格式的数据，用以下代码：
 fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),2,output);
 在这里又有一个问题，YUV420P格式需要写入data[0]，data[1]，data[2]；而RGB24，
 UYVY格式却仅仅是写入data[0]，他们的区别到底是什么呢？经过研究发现，
 在FFMPEG中，图像原始数据包括两种：planar和packed。planar就是将几个分量分开存，
 比如YUV420中，data[0]专门存Y，data[1]专门存U，data[2]专门存V。
 而packed则是打包存，所有数据都存在data[0]中。



 */
#ifndef MYSTUDY_TEMP20180713
#define MYSTUDY_TEMP20180713

#include "include/MyHead.h"

void videoFrameToImage4(AVFrame *pFrame, int width, int height, int iFrame);

int handleVideo4(AVCodecContext *videoAVCodecContext,
                 SwsContext *videoSwsContext,
                 AVPacket *avPacket,
                 AVFrame *srcAVFrame,
                 AVFrame *dstAVFrame,
                 int screen_w, int screen_h,
                 FILE *outputYUVFile);

int handleAudio4(AVCodecContext *audioAVCodecContext,
                 SwrContext *audioSwrContext,
                 AVSampleFormat audioAVSampleFormat,
                 AVPacket *avPacket,
                 AVFrame *srcAVFrame,
                 AVFrame *dstAVFrame);

int simplest_ffmpeg_player4() {
    // 屏幕宽高
    int screen_w = 0, screen_h = 0;
    int videoStreamIndex = -1, audioStreamIndex = -1;
    uint8_t *out_buffer;
    char filePath[] = "/root/视频/10_APITest_MKV-HEVC.mkv";

    // 格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
    AVFormatContext *avFormatContext = NULL;
    // 颜色转换器
    AVCodecContext *videoAVCodecContext = NULL;
    AVCodecContext *audioAVCodecContext = NULL;
    AVCodecParameters *videoAVCodecParameters = NULL;
    AVCodecParameters *audioAVCodecParameters = NULL;
    // Decoder
    AVCodec *videoAVCodecDecoder = NULL;
    AVCodec *audioAVCodecDecoder = NULL;
    // Encoder
    AVCodec *videoAVCodecEncoder = NULL;
    AVCodec *audioAVCodecEncoder = NULL;
    AVSampleFormat audioAVSampleFormat;
    //在linux录屏，使用x11grab
    //在windows下需要gdigrab或dshow(需要安装抓屏软件：screen-capture-recorder)
    // mac下使用avfoundation，见后文
    AVInputFormat *avInputFormat = NULL;
    //Linux
    AVDictionary *avDictionary = NULL;

    //srcAVFrame保存原始帧
    //dstAVFrame转换成yuv后的帧保存在此处
    AVFrame *srcAVFrame = NULL, *dstAVFrame = NULL;
    // AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
    AVPacket *avPacket = NULL;
    //SwsContext作为sws_scale的第一个参数,记录数据要转换的格式、大小及转换方式
    struct SwsContext *videoSwsContext = NULL;
    struct SwrContext *audioSwrContext = NULL;
    FILE *outputYUVFile = NULL;

#if OUTPUT_YUV420P
    outputYUVFile = fopen("/root/视频/10_APITest_MKV-HEVC.yuv", "wb+");
#endif

    // start
    // 注册FFmpeg所有编解码器
    //注册复用器和编解码器，所有的使用ffmpeg，首先必须调用这个函数
    av_register_all();
    //用于从网络接收数据，如果不是网络接收数据，可不用（如本例可不用）
    avformat_network_init();
    //注册设备的函数，如用获取摄像头数据或音频等，需要此函数先注册
//    avdevice_register_all();
    //AVFormatContext初始化，里面设置结构体的一些默认信息
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

    // av_dump_format(avFormatContext, 0, filePath, 1);

    int nb_streams = avFormatContext->nb_streams;
    printf("avFormatContext->nb_streams = %d\n", nb_streams);// 2
    int i;
    for (i = 0; i < nb_streams; i++) {
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
    }
    printf("videoIndex = %d, audioIndex = %d\n", videoStreamIndex, audioStreamIndex);// 0
    if (videoStreamIndex == -1 || audioStreamIndex == -1) {
        printf("Didn't find a video stream.\n");
        return -1;
    }

    // 视频的宽高
    screen_w = videoAVCodecParameters->width;// 1280
    screen_h = videoAVCodecParameters->height;// 720
    printf("screen_w = %d, screen_h = %d\n", screen_w, screen_h);

    // 得到AVCodec结构体
    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    videoAVCodecDecoder = avcodec_find_decoder(videoAVCodecParameters->codec_id);
    audioAVCodecDecoder = avcodec_find_decoder(audioAVCodecParameters->codec_id);
    if (videoAVCodecDecoder == NULL || audioAVCodecDecoder == NULL) {
        printf("CodecDecoder not found.\n");
        return -1;
    }

    // 必须要申请内存
    videoAVCodecContext = avcodec_alloc_context3(NULL);
    audioAVCodecContext = avcodec_alloc_context3(NULL);
    if (videoAVCodecContext == NULL || audioAVCodecContext == NULL) {
        printf("videoAVCodecContext is NULL or audioAVCodecContext is NULL.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(videoAVCodecContext, videoAVCodecParameters) < 0) {
        printf("Copy videoAVCodecParameters to context fail.\n");
        return -1;
    }
    if (avcodec_parameters_to_context(audioAVCodecContext, audioAVCodecParameters) < 0) {
        printf("Copy audioAVCodecParameters to context fail.\n");
        return -1;
    }

    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(videoAVCodecContext, videoAVCodecDecoder, NULL) < 0
        || avcodec_open2(audioAVCodecContext, audioAVCodecDecoder, NULL) < 0) {
        printf("Could not open CodecDecoder.\n");
        return -1;
    }

    //现在我们使用avpicture_fill来把帧和我们新申请的内存来结合
    //函数的使用本质上是为已经分配的空间的结构体AVPicture挂上一段用于保存数据的空间，这个结构体中有一个指针数组data[4]，挂在这个数组里。
    reinterpret_cast<AVSampleFormat *>(videoSwsContext =
                                               sws_getContext(screen_w, screen_h,
                                                              videoAVCodecContext->pix_fmt,
                                                              screen_w, screen_h,
                                                              AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
//                                                              AV_PIX_FMT_RGB24,// 图片
                                                              SWS_BICUBIC,// SWS_POINT
                                                              NULL, NULL, NULL));
    if (videoSwsContext == NULL) {
        printf("Cannot initialize the conversion context!\n");
        return -1;
    }
    int av_image_get_buffer_size_ =
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
//            av_image_get_buffer_size(AV_PIX_FMT_RGB24,// 图片
                                     screen_w,
                                     screen_h,
                                     1);
    printf("av_image_get_buffer_size_ = %d\n", av_image_get_buffer_size_);// 1382400
    srcAVFrame = av_frame_alloc();
    dstAVFrame = av_frame_alloc();
    //AVPacket代表编码后的一个包，即一帧编码为一个包
    avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    out_buffer = (uint8_t *) av_malloc(av_image_get_buffer_size_);
    av_image_fill_arrays(dstAVFrame->data,
                         dstAVFrame->linesize,
                         out_buffer,
                         AV_PIX_FMT_YUV420P,// 想要转换成什么样的格式
//                         AV_PIX_FMT_RGB24,// 图片
                         screen_w,
                         screen_h,
                         1);

    audioAVSampleFormat = audioAVCodecContext->sample_fmt;
    printf("audioAVSampleFormat = %d\n", audioAVSampleFormat);

    // AAC:1024  MP3:1152
    int out_sample_rate = 44100;
    int out_nb_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    int out_nb_samples = audioAVCodecContext->frame_size;
    int out_buffer_size = av_samples_get_buffer_size(NULL,
                                                     out_nb_channels,
                                                     out_nb_samples,
                                                     AV_SAMPLE_FMT_S16,
                                                     1);

    // FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout = av_get_default_channel_layout(audioAVCodecContext->channels);
    // Swr init
    audioSwrContext = swr_alloc();
    audioSwrContext = swr_alloc_set_opts(audioSwrContext,
                                         AV_CH_LAYOUT_STEREO,
                                         AV_SAMPLE_FMT_S16,
                                         out_sample_rate,
                                         in_channel_layout,
                                         audioAVCodecContext->sample_fmt,
                                         audioAVCodecContext->sample_rate,
                                         0,
                                         NULL);
    swr_init(audioSwrContext);

    printf("Entry while loop start.\n");

    int result = 0;
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        // 取视频数据
        if (avPacket->stream_index == videoStreamIndex) {
            result = handleVideo4(videoAVCodecContext, videoSwsContext,
                                  avPacket, srcAVFrame, dstAVFrame,
                                  screen_w, screen_h,
                                  outputYUVFile);

            // 取音频数据
        } else if (avPacket->stream_index == audioStreamIndex) {
            result = handleAudio4(audioAVCodecContext, audioSwrContext, audioAVSampleFormat,
                                  avPacket, srcAVFrame, dstAVFrame);

        }
        av_packet_unref(avPacket);
        if (result) {
            break;
        }
    }

    printf("Entry while loop end.\n");

    av_free(out_buffer);
    sws_freeContext(videoSwsContext);
    swr_free(&audioSwrContext);
    swr_close(audioSwrContext);
    av_frame_free(&srcAVFrame);
    av_frame_free(&dstAVFrame);
    avcodec_close(videoAVCodecContext);
    avcodec_close(audioAVCodecContext);
    avcodec_parameters_free(&videoAVCodecParameters);
    avcodec_parameters_free(&audioAVCodecParameters);
    avformat_close_input(&avFormatContext);

#if OUTPUT_YUV420P
    fclose(outputYUVFile);
#endif

    return result;
}

int imageCount4 = 0;

int handleVideo4(AVCodecContext *videoAVCodecContext,
                 SwsContext *videoSwsContext,
                 AVPacket *avPacket,
                 AVFrame *srcAVFrame,
                 AVFrame *dstAVFrame,
                 int screen_w, int screen_h,
                 FILE *outputYUVFile) {
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
    // 一般情况下while循环也只执行一次.条件结果在正常情况下为0.
    while (avcodec_receive_frame(videoAVCodecContext, srcAVFrame) >= 0) {
        // 像素转换函数
        // 把源数据srcAVFrame的像素数据转换成目标数据dstAVFrame的像素数据
        // dstAVFrame已经由前面设置像素格式为AV_PIX_FMT_YUV420P
        // 直接保存的dstAVFrame数据大小是非常大的,需要再经过编码压缩
        sws_scale(videoSwsContext,
                  (const uint8_t *const *) srcAVFrame->data,
                  srcAVFrame->linesize,
                  0,
                  screen_h,
                  dstAVFrame->data,
                  dstAVFrame->linesize);

#if OUTPUT_YUV420P
        fwrite(dstAVFrame->data[0], 1, screen_w * screen_h, outputYUVFile);        //Y
        fwrite(dstAVFrame->data[1], 1, (screen_w * screen_h) / 4, outputYUVFile);  //U
        fwrite(dstAVFrame->data[2], 1, (screen_w * screen_h) / 4, outputYUVFile);  //V
#endif

        /*if (++imageCount4 <= 5) {
            videoFrameToImage4(dstAVFrame, screen_w, screen_h, imageCount4);
        } else {
            return 1;
        }*/
    }
    return 0;
}

int handleAudio4(AVCodecContext *audioAVCodecContext,
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
        }
    }
    return 0;
}

/***
把一个视频解码后保存为一张张的图片
 */
void videoFrameToImage4(AVFrame *pFrame,
                        int width,
                        int height,
                        int iFrame) {
    FILE *dstFile;
    char dstFilePath[32];
    int y;

    //打开文件
    sprintf(dstFilePath, "/root/图片/video_to_image/frame%d.ppm", iFrame);
    dstFile = fopen(dstFilePath, "wb");
    if (dstFile == NULL) {
        printf("dstFile = NULL\n");
        return;
    }
    // 写头
    fprintf(dstFile, "P6\n%d %d\n255\n", width, height);

    // 写 pixel 数据
    for (y = 0; y < height; y++) {
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], width * 3, 1, dstFile);
    }
    // 关闭文件
    fclose(dstFile);
}

static void encode(AVCodecContext *enc_ctx,
                   AVFrame *frame,
                   AVPacket *pkt,
                   FILE *outfile) {
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);

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

        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

// 需要引入独立的x264 库：先下载x264的源码，编译之后；然后configure ffmpeg时加上enable x264 的选
int yuv_2_h264() {
    const char *filename, *codec_name;
    const AVCodec *codec;
    AVCodecContext *videoAVCodecContext = NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = {0, 0, 1, 0xb7};

    filename = "/root/视频/10_APITest_MKV-HEVC.yuv";
    codec_name = "libx264";

    av_register_all();

    /* find the mpeg1video encoder */
//    codec = avcodec_find_encoder_by_name(codec_name);
    codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    videoAVCodecContext = avcodec_alloc_context3(codec);
    if (!videoAVCodecContext) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    videoAVCodecContext->bit_rate = 400000;
    /* resolution must be a multiple of two */
    videoAVCodecContext->width = 352;
    videoAVCodecContext->height = 288;
    /* frames per second */
    videoAVCodecContext->time_base = (AVRational) {1, 25};
    videoAVCodecContext->framerate = (AVRational) {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    videoAVCodecContext->gop_size = 10;
    videoAVCodecContext->max_b_frames = 1;
    videoAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(videoAVCodecContext->priv_data, "preset", "slow", 0);

    /* open it */
    ret = avcodec_open2(videoAVCodecContext, codec, NULL);
    if (ret < 0) {
        //fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }

    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = videoAVCodecContext->pix_fmt;
    frame->width = videoAVCodecContext->width;
    frame->height = videoAVCodecContext->height;

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
        for (y = 0; y < videoAVCodecContext->height; y++) {
            for (x = 0; x < videoAVCodecContext->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
            }
        }

        /* Cb and Cr */
        for (y = 0; y < videoAVCodecContext->height / 2; y++) {
            for (x = 0; x < videoAVCodecContext->width / 2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }

        frame->pts = i;

        /* encode the image */
        encode(videoAVCodecContext, frame, pkt, f);
    }

    /* flush the encoder */
    encode(videoAVCodecContext, NULL, pkt, f);

    /* add sequence end code to have a real MPEG file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_free_context(&videoAVCodecContext);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}

/***
    调用:
    //All picture's resolution is 1280x720
    //Gray Bar, from 16 to 235
    gen_yuv420p_graybar(1280,720,10,16,235);
    //Color Bar
    gen_rgb24_colorbar(1280,720);
    //10 bars, RGB changed from 255,0,0 to 0,0,255
    gen_rgb24_rgbgradient_bar(1280,720,10,255,0,0,0,0,255);
    //10 bars, RGB changed from 0,0,0 to 128,128,128
    gen_yuv420p_yuvgradient_bar(1280,720,10,0,0,0,128,128,128);
    //RGB24 to BMP
    rgb24_to_bmp("colorbar_1280x720_rgb24.rgb","colorbar_1280x720_rgb24.bmp",1280,720);
    //Red stripe
    gen_rgb24_stripe(1280,720,255,0,0);
    //Gen color video
    gen_allcolor_video();
 */

/**
 * Generate Picture contains Stripe in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param r			Red component of stripe
 * @param g			Green component of stripe
 * @param b			Blue component of stripe
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_stripe(int width,
                     int height,
                     unsigned char r,
                     unsigned char g,
                     unsigned char b) {

    unsigned char *data = NULL;
    char filename[100] = {0};
    FILE *fp = NULL;
    int i = 0, j = 0;

    //Check
    if (width <= 0 || height <= 0) {
        printf("Error: Width, Height cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }

    data = (unsigned char *) malloc(width * height * 3);

    sprintf(filename, "rgbstripe_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            if (i % 2 != 0) {
                data[(j * width + i) * 3 + 0] = r;
                data[(j * width + i) * 3 + 1] = g;
                data[(j * width + i) * 3 + 2] = b;
            } else {//White
                data[(j * width + i) * 3 + 0] = 255;
                data[(j * width + i) * 3 + 1] = 255;
                data[(j * width + i) * 3 + 2] = 255;
            }
        }
    }
    fwrite(data, width * height * 3, 1, fp);
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);
    return 0;
}


/**
 * Generate Picture contains Gray Bar changing from Black to White in YUV420P Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param ymin		the minimum value of luminance.
 * @param ymax		the maximum value of luminance.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_yuv420p_graybar(int width,
                        int height,
                        int barnum,
                        unsigned char ymin,
                        unsigned char ymax) {

    int barwidth;
    float lum_inc;
    unsigned char lum_temp;
    int uv_width, uv_height;
    FILE *fp = NULL;
    unsigned char *data_y = NULL;
    unsigned char *data_u = NULL;
    unsigned char *data_v = NULL;
    int t = 0, i = 0, j = 0;
    char filename[100] = {0};

    //Check
    if (width <= 0 || height <= 0 || barnum <= 0) {
        printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
        barnum = 10;
    }
    if (width % barnum != 0) {
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");
    }
    barwidth = width / barnum;
    lum_inc = ((float) (ymax - ymin)) / ((float) (barnum - 1));
    uv_width = width / 2;
    uv_height = height / 2;

    data_y = (unsigned char *) malloc(width * height);
    data_u = (unsigned char *) malloc(uv_width * uv_height);
    data_v = (unsigned char *) malloc(uv_width * uv_height);

    sprintf(filename, "graybar_%dx%d_yuv420p.yuv", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("Y, U, V value from picture's left to right:\n");
    for (t = 0; t < (width / barwidth); t++) {
        lum_temp = ymin + (char) (t * lum_inc);
        printf("%3d, 128, 128\n", lum_temp);
    }
    //Gen Data
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            t = i / barwidth;
            lum_temp = ymin + (char) (t * lum_inc);
            data_y[j * width + i] = lum_temp;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            data_u[j * uv_width + i] = 128;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            data_v[j * uv_width + i] = 128;
        }
    }
    fwrite(data_y, width * height, 1, fp);
    fwrite(data_u, uv_width * uv_height, 1, fp);
    fwrite(data_v, uv_width * uv_height, 1, fp);
    fclose(fp);
    free(data_y);
    free(data_u);
    free(data_v);
    printf("Finish generate %s!\n", filename);
}

/**
 * Generate Picture contains standard Color Bar in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_colorbar(int width, int height) {

    unsigned char *data = NULL;
    int barwidth;
    char filename[100] = {0};
    FILE *fp = NULL;
    int i = 0, j = 0;
    int lum;
    float r_coeff = 0.299, g_coeff = 0.587, b_coeff = 0.114;

    //Check
    if (width <= 0 || height <= 0) {
        printf("Error: Width, Height cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }
    if (width % 8 != 0)
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");

    data = (unsigned char *) malloc(width * height * 3);
    barwidth = width / 8;

    sprintf(filename, "colorbar_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    printf("Luminance (Y) component value of colors from left to right:\n");
    lum = r_coeff * 255.0 + g_coeff * 255.0 + b_coeff * 255.0;
    printf("[White]  \tR,G,B=255,255,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 255.0 + g_coeff * 255.0 + b_coeff * 0.0;
    printf("[Yellow] \tR,G,B=255,255,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 255.0 + b_coeff * 255.0;
    printf("[Cyan]   \tR,G,B=  0,255,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 255.0 + b_coeff * 0.0;
    printf("[Green]  \tR,G,B=  0,255,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 255.0 + g_coeff * 0.0 + b_coeff * 255.0;
    printf("[Magenta]\tR,G,B=255,  0,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 255.0 + g_coeff * 0.0 + b_coeff * 0.0;
    printf("[Red]    \tR,G,B=255,  0,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 0.0 + b_coeff * 255.0;
    printf("[Blue]   \tR,G,B=  0,  0,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);
    lum = r_coeff * 0.0 + g_coeff * 0.0 + b_coeff * 0.0;
    printf("[Black]  \tR,G,B=  0,  0,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
           r_coeff, g_coeff, b_coeff, lum);

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            int barnum = i / barwidth;
            switch (barnum) {
                case 0: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 255;
                    break;
                }
                case 1: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
                case 2: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 255;
                    break;
                }
                case 3: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 255;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
                case 4: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 255;
                    break;
                }
                case 5: {
                    data[(j * width + i) * 3 + 0] = 255;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
                case 6: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 255;

                    break;
                }
                case 7: {
                    data[(j * width + i) * 3 + 0] = 0;
                    data[(j * width + i) * 3 + 1] = 0;
                    data[(j * width + i) * 3 + 2] = 0;
                    break;
                }
            }

        }
    }
    fwrite(data, width * height * 3, 1, fp);
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);
}

/**
 * Generate Picture contains Color Bar Changing from source color
 * to destination color in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param src_r		Red component of source color.
 * @param src_g		Green component of source color.
 * @param src_b		Blue component of source color.
 * @param dst_r		Red component of destination color.
 * @param dst_g		Green component of destination color.
 * @param dst_b		Blue component of destination color.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_rgbgradient_bar(int width, int height, int barnum,
                              unsigned char src_r,
                              unsigned char src_g,
                              unsigned char src_b,
                              unsigned char dst_r,
                              unsigned char dst_g,
                              unsigned char dst_b) {

    unsigned char *data = NULL;
    int barwidth;
    float r_inc, g_inc, b_inc;
    unsigned char r_temp, g_temp, b_temp;
    char filename[100] = {0};
    FILE *fp = NULL;
    int t = 0, i = 0, j = 0;

    //Check
    if (width <= 0 || height <= 0 || barnum <= 0) {
        printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }
    if (width % barnum != 0)
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");


    data = (unsigned char *) malloc(width * height * 3);
    barwidth = width / barnum;
    r_inc = ((float) (dst_r - src_r)) / ((float) (barnum - 1));
    g_inc = ((float) (dst_g - src_g)) / ((float) (barnum - 1));
    b_inc = ((float) (dst_b - src_b)) / ((float) (barnum - 1));

    sprintf(filename, "rgbgradientbar_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("R, G, B value from picture's left to right:\n");
    for (t = 0; t < (width / barwidth); t++) {
        r_temp = src_r + (char) (t * r_inc);
        g_temp = src_g + (char) (t * g_inc);
        b_temp = src_b + (char) (t * b_inc);
        printf("%3d, %3d, %3d\n", r_temp, g_temp, b_temp);
    }

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            t = i / barwidth;
            r_temp = src_r + (char) (t * r_inc);
            g_temp = src_g + (char) (t * g_inc);
            b_temp = src_b + (char) (t * b_inc);
            data[(j * width + i) * 3 + 0] = r_temp;
            data[(j * width + i) * 3 + 1] = g_temp;
            data[(j * width + i) * 3 + 2] = b_temp;
        }
    }
    fwrite(data, width * height * 3, 1, fp);
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);
    return 0;
}

/**
 * Generate Picture contains Color Bar Changing from source color
 * to destination color in YUV420P Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param src_y		Luma component of source color.
 * @param src_u		U component of source color.
 * @param src_v		V component of source color.
 * @param dst_y		Luma component of destination color.
 * @param dst_u		U component of destination color.
 * @param dst_v		V component of destination color.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_yuv420p_yuvgradient_bar(int width, int height, int barnum,
                                unsigned char src_y,
                                unsigned char src_u,
                                unsigned char src_v,
                                unsigned char dst_y,
                                unsigned char dst_u,
                                unsigned char dst_v) {

    int uv_width, uv_height;
    unsigned char *data_y = NULL;
    unsigned char *data_u = NULL;
    unsigned char *data_v = NULL;
    FILE *fp = NULL;
    int barwidth, uv_barwidth;
    float y_inc, u_inc, v_inc = 0;
    unsigned char y_temp, u_temp, v_temp = 0;
    char filename[100] = {0};
    int t = 0, i = 0, j = 0;
    //Check
    if (width <= 0 || height <= 0 || barnum <= 0) {
        printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
        printf("Default Param is used.\n");
        width = 640;
        height = 480;
    }
    if (width % barnum != 0)
        printf("Warning: Width cannot be divided by Bar Number without remainder!\n");

    uv_width = width / 2;
    uv_height = height / 2;
    data_y = (unsigned char *) malloc(width * height);
    data_u = (unsigned char *) malloc(uv_width * uv_height);
    data_v = (unsigned char *) malloc(uv_width * uv_height);
    barwidth = width / barnum;
    uv_barwidth = barwidth / (width / uv_width);
    y_inc = ((float) (dst_y - src_y)) / ((float) (barnum - 1));
    u_inc = ((float) (dst_u - src_u)) / ((float) (barnum - 1));
    v_inc = ((float) (dst_v - src_v)) / ((float) (barnum - 1));

    sprintf(filename, "yuvgradientbar_%dx%d_yuv420p.yuv", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("Y, U, V value from picture's left to right:\n");
    for (t = 0; t < (width / barwidth); t++) {
        y_temp = src_y + (char) (t * y_inc);
        u_temp = src_u + (char) (t * u_inc);
        v_temp = src_v + (char) (t * v_inc);
        printf("%3d, %3d, %3d\n", y_temp, u_temp, v_temp);
    }

    //Gen Data
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            t = i / barwidth;
            y_temp = src_y + (char) (t * y_inc);
            data_y[j * width + i] = y_temp;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            t = i / uv_barwidth;
            u_temp = src_u + (char) (t * u_inc);
            data_u[j * uv_width + i] = u_temp;
        }
    }
    for (j = 0; j < uv_height; j++) {
        for (i = 0; i < uv_width; i++) {
            t = i / uv_barwidth;
            v_temp = src_v + (char) (t * v_inc);
            data_v[j * uv_width + i] = v_temp;
        }
    }
    fwrite(data_y, width * height, 1, fp);
    fwrite(data_u, uv_width * uv_height, 1, fp);
    fwrite(data_v, uv_width * uv_height, 1, fp);
    fclose(fp);
    free(data_y);
    free(data_u);
    free(data_v);
    printf("Finish generate %s!\n", filename);
    return 0;
}

/**
 * Convert RGB24 format to BMP format
 *
 * @param rgb24path		path of input RGB24 file.
 * @param bmppath		path of output BMP file
 * @param width			the width of picture.
 * @param height		the height of picture.
 * @return 0 if finished, -1 if there are errors.
 */
int rgb24_to_bmp(char *rgb24path, char *bmppath, int width, int height) {
    typedef struct {
        long imageSize;
        long blank;
        long startPosition;
    } BmpHead;

    typedef struct {
        long Length;
        long width;
        long height;
        unsigned short colorPlane;
        unsigned short bitColor;
        long zipFormat;
        long realSize;
        long xPels;
        long yPels;
        long colorUse;
        long colorImportant;
    } InfoHead;

    int i = 0, j = 0;
    BmpHead m_BMPHeader = {0};
    InfoHead m_BMPInfoHeader = {0};
    char bfType[2] = {'B', 'M'};
    int header_size = sizeof(bfType) + sizeof(BmpHead) + sizeof(InfoHead);
    unsigned char *rgb24_buffer = NULL;
    FILE *fp_rgb24 = NULL, *fp_bmp = NULL;

    if ((fp_rgb24 = fopen(rgb24path, "rb")) == NULL) {
        printf("Error: Cannot open input RGB24 file.\n");
        return -1;
    }
    if ((fp_bmp = fopen(bmppath, "wb")) == NULL) {
        printf("Error: Cannot open output BMP file.\n");
        return -1;
    }

    rgb24_buffer = (unsigned char *) malloc(width * height * 3);
    fread(rgb24_buffer, 1, width * height * 3, fp_rgb24);

    m_BMPHeader.imageSize = 3 * width * height + header_size;
    m_BMPHeader.startPosition = header_size;

    m_BMPInfoHeader.Length = sizeof(InfoHead);
    m_BMPInfoHeader.width = width;
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    m_BMPInfoHeader.height = -height;
    m_BMPInfoHeader.colorPlane = 1;
    m_BMPInfoHeader.bitColor = 24;
    m_BMPInfoHeader.realSize = 3 * width * height;

    fwrite(bfType, 1, sizeof(bfType), fp_bmp);
    fwrite(&m_BMPHeader, 1, sizeof(m_BMPHeader), fp_bmp);
    fwrite(&m_BMPInfoHeader, 1, sizeof(m_BMPInfoHeader), fp_bmp);

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            char temp = rgb24_buffer[(j * width + i) * 3 + 2];
            rgb24_buffer[(j * width + i) * 3 + 2] = rgb24_buffer[(j * width + i) * 3 + 0];
            rgb24_buffer[(j * width + i) * 3 + 0] = temp;
        }
    }
    fwrite(rgb24_buffer, 3 * width * height, 1, fp_bmp);
    fclose(fp_rgb24);
    fclose(fp_bmp);
    free(rgb24_buffer);
    printf("Finish generate %s!\n", bmppath);
    return 0;
}


/**
 * Generate a video in 256x256 and has 256 frames that contains all the colors.
 * Each color is shown in 1 pixel. They are mapped as follows:
 * In RGB24
 * R component's value is increasing with the growth of width (X-axis);
 * G component's value is increasing with the growth of height (Y-axis);
 * B component's value is increasing with the growth of frame number (Z-axis).
 * In YUV444P
 * U component's value is increasing with the growth of width (X-axis);
 * V component's value is increasing with the growth of height (Y-axis);
 * Y component's value is increasing with the growth of frame number (Z-axis).
 *
 * This function now support to draw YUV444P/RGB24 format pixel.
 *
 * @return 0 if finished, -1 if there are errors.
 */
int gen_allcolor_video() {

    unsigned char *data = NULL;
    char filename[100] = {0};
    FILE *fp = NULL;
    int width = 256, height = 256, frames = 256;
    int i = 0, j = 0, k = 0;

    //From left to right (width, X-axis),R increasing from 0 to255
    //From Top to bottom (height, Y-axis),G increasing from 0 to255
    //From 0 to 255 frames (time, Z-axis),B increasing from 0 to255
    data = (unsigned char *) malloc(width * height * 3);
    sprintf(filename, "allcolor_xr_yg_zb_%dx%d_rgb24.rgb", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }
    for (k = 0; k < frames; k++) {
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i++) {
                data[(j * width + i) * 3 + 0] = i;
                data[(j * width + i) * 3 + 1] = j;
                data[(j * width + i) * 3 + 2] = k;
            }
        }
        fwrite(data, width * height * 3, 1, fp);
        printf("Finish generate frame %d!\n", k);
    }
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);

    //From left to right (width, X-axis),U increasing from 0 to255
    //From Top to bottom (height, Y-axis),V increasing from 0 to255
    //From 0 to 255 frames (time, Z-axis),Y increasing from 0 to255
    data = (unsigned char *) malloc(width * height);
    sprintf(filename, "allcolor_xu_yv_zy_%dx%d_yuv444p.yuv", width, height);
    if ((fp = fopen(filename, "wb+")) == NULL) {
        printf("Error: Cannot create file!");
        return -1;
    }
    for (k = 0; k < frames; k++) {
        for (j = 0; j < height; j++) {//Y
            for (i = 0; i < width; i++) {
                data[j * width + i] = k;
            }
        }
        fwrite(data, width * height, 1, fp);
        for (j = 0; j < height; j++) {//U
            for (i = 0; i < width; i++) {
                data[j * width + i] = i;
            }
        }
        fwrite(data, width * height, 1, fp);
        for (j = 0; j < height; j++) {//V
            for (i = 0; i < width; i++) {
                data[j * width + i] = j;
            }
        }
        fwrite(data, width * height, 1, fp);
        printf("Finish generate frame %d!\n", k);
    }
    fclose(fp);
    free(data);
    printf("Finish generate %s!\n", filename);

    return 0;
}


#endif //MYSTUDY_TEMP20180713