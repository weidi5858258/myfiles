//
// Created by root on 18-7-12.
//

#ifndef MYSTUDY_FFMPEG
#define MYSTUDY_FFMPEG

#include "../include/MyHead.h"

#define RECORDING_SCREEN 0
#define OUTPUT_YUV420P 0

void videoFrameToImage(AVFrame *pFrame, int width, int height, int iFrame);

int handleVideo(AVCodecContext *videoAVCodecContext,
                SwsContext *videoSwsContext,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame,
                int screen_w, int screen_h,
                FILE *outputYUVFile);

int handleAudio(AVCodecContext *audioAVCodecContext,
                SwrContext *audioSwrContext,
                AVSampleFormat audioAVSampleFormat,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame);

int simplest_ffmpeg_player3() {
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
    outputYUVFile = fopen("/root/视频/output.yuv", "wb+");
#endif

    // start
    // 注册FFmpeg所有编解码器
    //注册复用器和编解码器，所有的使用ffmpeg，首先必须调用这个函数
    av_register_all();
    //用于从网络接收数据，如果不是网络接收数据，可不用（如本例可不用）
    avformat_network_init();
    //注册设备的函数，如用获取摄像头数据或音频等，需要此函数先注册
    // avdevice_register_all();
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

    printf("Entry while loop.\n");

    int result = 0;
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        // 取视频数据
        if (avPacket->stream_index == videoStreamIndex) {
            result = handleVideo(videoAVCodecContext, videoSwsContext,
                                 avPacket, srcAVFrame, dstAVFrame,
                                 screen_w, screen_h,
                                 outputYUVFile);

            // 取音频数据
        } else if (avPacket->stream_index == audioStreamIndex) {
            result = handleAudio(audioAVCodecContext, audioSwrContext, audioAVSampleFormat,
                                 avPacket, srcAVFrame, dstAVFrame);

        }
        if (result) {
            return result;
        }
        av_packet_unref(avPacket);
    }

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

    return 0;
}

int imageCount = 0;

int handleVideo(AVCodecContext *videoAVCodecContext,
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
    // 一般情况下while循环也只执行一次
    while (avcodec_receive_frame(videoAVCodecContext, srcAVFrame) >= 0) {
        //转换，把源数据pFrame转换成pFrameYUV，pFrameYUV由前面设置格式为yuv420P
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

        if (++imageCount <= 500) {
            videoFrameToImage(dstAVFrame, screen_w, screen_h, imageCount);
        } else {
            return 1;
        }
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
        }
    }
    return 0;
}

/***
把一个视频解码后保存为一张张的图片
 */
void videoFrameToImage(AVFrame *pFrame, int width, int height, int iFrame) {
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