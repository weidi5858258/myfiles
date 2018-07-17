//
// Created by root on 18-7-12.
//

#ifndef MYSTUDY_FFMPEG
#define MYSTUDY_FFMPEG

#include "include/MyHead.h"

#define RECORDING_SCREEN 0

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
        if (++imageCount <= 500) {
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
 只用到解码器,没有用到编码器
 */
int decoder_video_frame_to_image() {
    ////////////////////////////音视频公共部分////////////////////////////
    // char filePath[] = "/root/视频/10_APITest_MKV-HEVC.mkv";
    char filePath[] = "/root/视频/haoke.avi";
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





/***
 来自网上的代码
 使用这个方法解码成的pcm文件再编码成mp3文件后,声音变了,这种声音蛮好听的
 */
int decode_audio2(const char *input_file_name, const char *output_file_name) {
    //1.注册组件
    av_register_all();
    //封装格式上下文
    AVFormatContext *av_format_context = avformat_alloc_context();

    //2.打开输入音频文件
    if (avformat_open_input(&av_format_context, input_file_name, NULL, NULL) != 0) {
        printf("%s\n", "打开输入音频文件失败");
        return -1;
    }
    //3.获取音频信息
    if (avformat_find_stream_info(av_format_context, NULL) < 0) {
        printf("%s\n", "获取音频信息失败");
        return -1;
    }

    //音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
    int audio_stream_index = -1;
    int i;
    for (i = 0; i < av_format_context->nb_streams; i++) {
        //根据类型判断是否是音频流
        if (av_format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    //4.获取解码器
    //根据索引拿到对应的流,根据流拿到解码器上下文
    AVCodecContext *audio_av_codec_context = av_format_context->streams[audio_stream_index]->codec;
    //再根据上下文拿到编解码id，通过该id拿到解码器
    AVCodec *audio_av_codec = avcodec_find_decoder(audio_av_codec_context->codec_id);
    if (audio_av_codec == NULL) {
        printf("%s\n", "无法解码");
        return -1;
    }
    //5.打开解码器
    if (avcodec_open2(audio_av_codec_context, audio_av_codec, NULL) < 0) {
        printf("%s\n", "编码器无法打开");
        return -1;
    }
    //编码数据
    AVPacket *av_packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    //解压缩数据
    AVFrame *frame = av_frame_alloc();

    //frame->16bit 44100 PCM 统一音频采样格式与采样率
    SwrContext *swr_context = swr_alloc();
    //重采样设置选项-----------------------------------------------------------start
    //输入的采样格式
    enum AVSampleFormat in_sample_fmt = audio_av_codec_context->sample_fmt;
    //输出的采样格式 16bit PCM
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入的采样率
    int in_sample_rate = audio_av_codec_context->sample_rate;
    //输出的采样率
    int out_sample_rate = 44100;
    //输入的声道布局
    uint64_t in_ch_layout = audio_av_codec_context->channel_layout;
    //输出的声道布局
    uint64_t out_ch_layout = AV_CH_LAYOUT_MONO;

    swr_alloc_set_opts(swr_context, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt,
                       in_sample_rate, 0, NULL);
    swr_init(swr_context);
    //重采样设置选项-----------------------------------------------------------end
    //获取输出的声道个数
    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
    //存储pcm数据
    uint8_t *out_buffer = (uint8_t *) av_malloc(2 * 44100);
    FILE *fp_pcm = fopen(output_file_name, "wb");
    int ret, got_frame, framecount = 0;
    //6.一帧一帧读取压缩的音频数据AVPacket
    while (av_read_frame(av_format_context, av_packet) >= 0) {
        if (av_packet->stream_index == audio_stream_index) {
            //解码AVPacket->AVFrame
            ret = avcodec_decode_audio4(audio_av_codec_context, frame, &got_frame, av_packet);
            if (ret < 0) {
                printf("%s\n", "解码完成");
            }
            //非0，正在解码
            if (got_frame) {
                printf("解码%d帧\n", framecount++);
                swr_convert(swr_context, &out_buffer, 2 * 44100, (const uint8_t **)frame->data, frame->nb_samples);
                //获取sample的size
                int out_buffer_size = av_samples_get_buffer_size(NULL, out_channel_nb,
                                                                 frame->nb_samples,
                                                                 out_sample_fmt, 1);
                //写入文件进行测试
                fwrite(out_buffer, 1, out_buffer_size, fp_pcm);
            }
        }
        av_free_packet(av_packet);
    }


    fclose(fp_pcm);
    av_frame_free(&frame);
    av_free(out_buffer);
    swr_free(&swr_context);
    avcodec_close(audio_av_codec_context);
    avformat_close_input(&av_format_context);
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