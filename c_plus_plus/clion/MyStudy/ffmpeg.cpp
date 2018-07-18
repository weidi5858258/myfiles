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
                FILE *output_YUV_file,
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

#ifdef NEED_VIDEO
#if OUTPUT_YUV420P
        fwrite(dstAVFrame->data[0], 1, srcW * srcH, output_YUV_file);        //Y
        fwrite(dstAVFrame->data[1], 1, (srcW * srcH) / 4, output_YUV_file);  //U
        fwrite(dstAVFrame->data[2], 1, (srcW * srcH) / 4, output_YUV_file);  //V
#endif
#endif

#ifdef NEED_VIDEO
#if OUTPUT_IMAGE
        // 输出多少张图片
        if (++imageCount <= 5) {
            saveImage(dstAVFrame, srcW, srcH, imageCount);
        } else {
            return 1;
        }
#endif
#endif
    }
    return 0;
}

int handleAudio(AVCodecContext *audioAVCodecContext,
                SwrContext *audioSwrContext,
                AVSampleFormat audioAVSampleFormat,
                AVPacket *avPacket,
                AVFrame *srcAVFrame,
                AVFrame *dstAVFrame,
                int audio_out_buffer_size,
                FILE *output_PCM_file) {
    int result = avcodec_send_packet(audioAVCodecContext, avPacket);
    // printf("handleAudio result = %d\n", result);
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
        /*if (audioAVSampleFormat == AV_SAMPLE_FMT_S16P) {// 6
            //Audacity: big endian 32bit stereo start offset 7 (but has noise)
        } else if (audioAVSampleFormat == AV_SAMPLE_FMT_FLTP) {// 8
        }*/
        swr_convert(audioSwrContext,
                    (uint8_t **) dstAVFrame->data,
                    MAX_AUDIO_FRAME_SIZE,
                    (const uint8_t **) srcAVFrame->data,
                    srcAVFrame->nb_samples);

        printf("avPacket->pts:%lld\tavPacket->dts:%lld\tavPacket->size:%d\n",
               avPacket->pts, avPacket->dts, avPacket->size);

#ifdef NEED_AUDIO
#if OUTPUT_PCM
        fwrite(dstAVFrame->data[0], 1, audio_out_buffer_size, output_PCM_file);
#endif
#endif
    }
    return 0;
}

/***
 只用到解码器,没有用到编码器
 不断地强化这个函数,使其代码更完善
 */
int decoder_video_frame_to_image() {
    ////////////////////////////音视频公共部分////////////////////////////
    // char filePath[] = "/root/视频/10_APITest_MKV-HEVC.mkv";
    char file_path[] = "/root/视频/01_APITest_MPEG1.mpg";
    // 格式上下结构体，,可以理解为存储数据流的文件，伴随整个生命周期
    AVFormatContext *av_format_context = NULL;

    // AVPacket结构体的作用是从内存中获取一个视频压缩帧,对于音频可能获取一个或者多个压缩帧.
    AVPacket *av_packet = NULL;

    FILE *output_YUV_file = NULL;
    FILE *output_PCM_file = NULL;

#ifdef NEED_VIDEO
#if OUTPUT_YUV420P
    output_YUV_file = fopen("/root/视频/output.yuv", "wb+");
#endif
#endif

#ifdef NEED_AUDIO
#if OUTPUT_PCM
    output_PCM_file = fopen("/root/音乐/output.pcm", "wb+");
#endif
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
    av_format_context = avformat_alloc_context();

#if RECORDING_SCREEN
    //av_dict_set(&amp;options,"framerate","5",0);
    //av_dict_set(&amp;options,"offset_x","20",0);
    //av_dict_set(&amp;options,"offset_y","40",0);
    //设置录屏参数，如录屏图像的大小、录屏帧率等
    //不设置代表全屏，但实际测试时，不设置默认大小为640x480
    /*av_dict_set(&avDictionary, "video_size", "640x480", 0);
    avInputFormat = av_find_input_format("x11grab");
    if (avformat_open_input(&av_format_context,
                            ":0.0+10,20",
                            avInputFormat,
                            &avDictionary) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }*/
#else
    // 打开文件
    if (avformat_open_input(&av_format_context, file_path, NULL, NULL)) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
#endif

    // 获取流信息
    if (avformat_find_stream_info(av_format_context, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    av_dump_format(av_format_context, 0, file_path, false);

    int video_stream_index = -1;
    int audio_stream_index = -1;
    int nb_streams = av_format_context->nb_streams;
    printf("av_format_context->nb_streams = %d\n", nb_streams);// 2
    int i;
    for (i = 0; i < nb_streams; i++) {
        AVMediaType avMediaType = av_format_context->streams[i]->codecpar->codec_type;
        if (avMediaType == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            printf("video_stream_index = %d\n", video_stream_index);
        } else if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = avMediaType;
            printf("audio_stream_index = %d\n", audio_stream_index);
        }
    }
    // 至少要有一个视频流或者音频流
    /*if (video_stream_index == -1 || audio_stream_index == -1) {
        printf("Didn't find a video or audio stream.\n");
        return -1;
    }*/

    //AVPacket代表编码后的一个包，即一帧编码为一个包
    av_packet = (AVPacket *) av_malloc(sizeof(AVPacket));

    ////////////////////////////视频部分////////////////////////////
#if NEED_VIDEO
    uint8_t *video_out_buffer;
    // 源视频宽高
    int src_video_width = 0, src_video_height = 0;
    // 颜色转换器
    AVCodecContext *video_av_codec_context = NULL;
    AVCodecParameters *video_av_codec_parameters = NULL;
    //src_av_frame保存原始帧
    //dst_av_frame转换成yuv后的帧保存在此处
    AVFrame *video_src_av_frame = NULL, *video_dst_av_frame = NULL;
    // Decoder
    AVCodec *video_decoder_av_codec = NULL;
    // Encoder
    AVCodec *video_encoder_av_codec = NULL;
    // SwsContext作为sws_scale的第一个参数,记录数据要转换的格式、大小及转换方式(用于图像像素的处理)
    struct SwsContext *video_sws_context = NULL;

    if (video_stream_index != -1) {
        video_av_codec_parameters = av_format_context->streams[video_stream_index]->codecpar;
    }

    // 视频的宽高
    src_video_width = video_av_codec_parameters->width;// 1280
    src_video_height = video_av_codec_parameters->height;// 720
    printf("src_video_width = %d, src_video_height = %d\n", src_video_width, src_video_height);

    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    // 源视频是用什么样的视频编码器,那么就找什么样的视频解码器
    video_decoder_av_codec = avcodec_find_decoder(video_av_codec_parameters->codec_id);
    if (video_decoder_av_codec == NULL) {
        printf("Video CodecDecoder not found.\n");
        return -1;
    }

    // 必须要申请内存
    video_av_codec_context = avcodec_alloc_context3(NULL);
    if (video_av_codec_context == NULL) {
        printf("videoAVCodecContext is NULL.\n");
        return -1;
    }
    if (avcodec_parameters_to_context(video_av_codec_context, video_av_codec_parameters) < 0) {
        printf("Copy videoAVCodecParameters to context fail.\n");
        return -1;
    }
    // 打开解码器 相应的有打开编码器avcodec_open2
    if (avcodec_open2(video_av_codec_context, video_decoder_av_codec, NULL) < 0) {
        printf("Could not open Video CodecDecoder.\n");
        return -1;
    }

    // 申请内存
    video_src_av_frame = av_frame_alloc();
    video_dst_av_frame = av_frame_alloc();

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
    AVPixelFormat dst_pix_format = AV_PIX_FMT_NONE;
#if OUTPUT_IMAGE
    dst_pix_format = AV_PIX_FMT_RGB24;
#else
    dst_pix_format = AV_PIX_FMT_YUV420P;
#endif
    printf("源图像的像素格式: %d\n", video_av_codec_context->pix_fmt);
    printf("目标图像的像素格式: %d\n", dst_pix_format);
    // 下面这部分代码还不确定是否需要对音频也做这样的处理,因为音频也用到了dstAVFrame
    video_sws_context = sws_getContext(src_video_width, src_video_height,
                                       video_av_codec_context->pix_fmt,
                                       src_video_width, src_video_height,
                                       dst_pix_format,
                                       SWS_BICUBIC,// SWS_POINT设定图像拉伸使用的算法
                                       NULL, NULL, NULL);
    if (video_sws_context == NULL) {
        printf("Cannot initialize the conversion context!\n");
        return -1;
    }
    int image_get_buffer_size = av_image_get_buffer_size(dst_pix_format, src_video_width, src_video_height, 1);
    printf("image_get_buffer_size = %d\n", image_get_buffer_size);// 1382400
    video_out_buffer = (uint8_t *) av_malloc(image_get_buffer_size);
    // 使用av_image_fill_arrays函数把目标帧和新申请的内存结合起来
    av_image_fill_arrays(video_dst_av_frame->data,
                         video_dst_av_frame->linesize,
                         video_out_buffer,
                         dst_pix_format,
                         src_video_width, src_video_height,
                         1);// 设置成“1”就好了
#endif

    ////////////////////////////音频部分////////////////////////////
#if NEED_AUDIO
    AVCodecContext *audio_av_codec_context = NULL;
    AVCodecParameters *audio_av_codec_parameters = NULL;
    AVFrame *audio_src_av_frame = NULL, *audio_dst_av_frame = NULL;
    // Decoder
    AVCodec *audio_decoder_av_codec = NULL;
    // Encoder
    AVCodec *audio_encoder_av_codec = NULL;
    AVSampleFormat audio_av_sample_format;
    struct SwrContext *audio_swr_context = NULL;

    if (audio_stream_index != -1) {
        audio_av_codec_parameters = av_format_context->streams[audio_stream_index]->codecpar;
    }

    // 查找解码器 相应的有查找编码器avcodec_find_encoder
    audio_decoder_av_codec = avcodec_find_decoder(audio_av_codec_parameters->codec_id);
    printf("audio_av_codec_parameters->codec_id = %d\n", audio_av_codec_parameters->codec_id);
    if (audio_decoder_av_codec == NULL) {
        printf("Audio CodecDecoder not found.\n");
        return -1;
    }

    // 必须要申请内存
    audio_av_codec_context = avcodec_alloc_context3(NULL);
    if (audio_av_codec_context == NULL) {
        printf("audioAVCodecContext is NULL.\n");
        return -1;
    }
    if (avcodec_parameters_to_context(audio_av_codec_context, audio_av_codec_parameters) < 0) {
        printf("Copy audioAVCodecParameters to context fail.\n");
        return -1;
    }
    // 打开解码器 相应的有打开编码器avcodec_open2
    // 源音频是用什么样的音频编码器,那么就找什么样的音频解码器
    if (avcodec_open2(audio_av_codec_context, audio_decoder_av_codec, NULL) < 0) {
        printf("Could not open Audio CodecDecoder.\n");
        return -1;
    }

    // 申请内存
    audio_src_av_frame = av_frame_alloc();
    audio_dst_av_frame = av_frame_alloc();

    //////////////////////////////////声音的初始化工作//////////////////////////////////

    audio_av_sample_format = audio_av_codec_context->sample_fmt;
    int audio_av_sample_rate = audio_av_codec_context->sample_rate;
    int audio_channels = audio_av_codec_context->channels;
    printf("audio_av_sample_format = %d\n", audio_av_sample_format);
    printf("audio_av_sample_rate = %d\n", audio_av_sample_rate);
    printf("audio_channels = %d\n", audio_channels);// 2(左声道,右声道)

    // AAC:1024  MP3:1152
    int out_nb_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    int out_nb_samples = audio_av_codec_context->frame_size;
    printf("audio out_nb_channels = %d\n", out_nb_channels);
    printf("audio out_nb_samples = %d\n", out_nb_samples);
    int audio_out_buffer_size = av_samples_get_buffer_size(NULL,
                                                           out_nb_channels,
                                                           out_nb_samples,
                                                           AV_SAMPLE_FMT_S16,
                                                           1);

    // FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout = av_get_default_channel_layout(audio_channels);
    // Swr init
    audio_swr_context = swr_alloc();
    audio_swr_context = swr_alloc_set_opts(audio_swr_context,
                                           AV_CH_LAYOUT_STEREO,
                                           AV_SAMPLE_FMT_S16,
                                           44100,
                                           in_channel_layout,
                                           audio_av_sample_format,
                                           audio_av_sample_rate,
                                           0,
                                           NULL);
    swr_init(audio_swr_context);
#endif

    ////////////////////////////开始解码////////////////////////////

    printf("Entry while loop start.\n");
    int result = -1;
    int imageCount = 0;
    while (av_read_frame(av_format_context, av_packet) >= 0) {
        // printf("While loop avPacket->stream_index = %d\n", av_packet->stream_index);
#if NEED_VIDEO
        if (av_packet->stream_index == video_stream_index) {// 取视频数据
            result = handleVideo(video_av_codec_context, video_sws_context,
                                 av_packet, video_src_av_frame, video_dst_av_frame,
                                 src_video_width, src_video_height,
                                 output_YUV_file,
                                 imageCount);
            // printf("While loop video result = %d\n", result);
            av_packet_unref(av_packet);
            if (result) {
                // printf("Entry while loop video break. Because result = %d\n", result);
                break;
            }
        }
#endif
#if NEED_AUDIO
        if (av_packet->stream_index == audio_stream_index) {// 取音频数据
            result = handleAudio(audio_av_codec_context, audio_swr_context, audio_av_sample_format,
                                 av_packet, audio_src_av_frame, audio_dst_av_frame,
                                 audio_out_buffer_size, output_PCM_file);
            // printf("While loop audio result = %d\n", result);
            av_packet_unref(av_packet);
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
    av_free(video_out_buffer);
    sws_freeContext(video_sws_context);
    avcodec_close(video_av_codec_context);
    av_free(video_av_codec_context);
    av_frame_free(&video_src_av_frame);
    av_frame_free(&video_dst_av_frame);
    avcodec_parameters_free(&video_av_codec_parameters);
#endif
#if NEED_AUDIO
    swr_free(&audio_swr_context);
    swr_close(audio_swr_context);
    avcodec_close(audio_av_codec_context);
    av_free(audio_av_codec_context);
    av_frame_free(&audio_src_av_frame);
    av_frame_free(&audio_dst_av_frame);
    avcodec_parameters_free(&audio_av_codec_parameters);
#endif
    avformat_close_input(&av_format_context);

#ifdef NEED_VIDEO
#if OUTPUT_YUV420P
    fclose(output_YUV_file);
#endif
#endif

#ifdef NEED_VIDEO
#if OUTPUT_YUV420P
    fclose(output_PCM_file);
#endif
#endif

    return 0;
}

int test(double) {
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