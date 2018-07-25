//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_MYHEAD_H
#define MYSTUDY_MYHEAD_H

//#include <sys/socket.h>
//#include <sys/epoll.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <limits.h>// 定义了一些常用类型的最小值,最大值
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <libgen.h>
#include <cstddef>
#include <inttypes.h>
//#include <opencv2/opencv.hpp>

// ffmpeg and SDL
extern "C" {// 不能少
#include <libavcodec/avcodec.h>
// 摄像头所用
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include "libswresample/swresample.h"
// libswscale是一个主要用于处理图片像素数据的类库.可以完成图片像素格式的转换,图片的拉伸等工作.
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/channel_layout.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
// 这里是做分片时候重采样编码音频用的
#include <libavutil/audio_fifo.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/pixfmt.h>
#include <libavutil/fifo.h>
#include <libavutil/opt.h>
#include <libavutil/mem.h>
#include <libavutil/error.h>
#include <libavutil/time.h>

#include <lame.h>

//录音
#include <alsa/asoundlib.h>

#include <SDL2/SDL.h>

#include <GL/glut.h>
};

//extern "C" {
//    #include <jconfig.h>
//    #include <jerror.h>
//    #include <jmorecfg.h>
//    #include <jpeglib.h>
//    #include <turbojpeg.h>
//}

// STL
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

#define INBUF_SIZE 4096
// 采样率/20+7200
#define MP3BUFSIZE (int) (1.25 * INBUF_SIZE) + 7200
/***
 现在不能单独使用音频,必须先初始化视频,此后才能处理音频.
 因为在初始化视频时需要初始化dstAVFrame,dstAVFrame在
 音频中也要用到.
*/
#define NEED_VIDEO 1
#define NEED_AUDIO 1
#define OUTPUT_YUV420P 0
#define OUTPUT_PCM 1
#define OUTPUT_IMAGE 0

#define NEED_TO_DECODER 1

//set '1' to choose a type of file to play
#define LOAD_BGRA    0
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_YUV420P 1

//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)
//Break
#define BREAK_EVENT    (SDL_USEREVENT + 2)

#define ALSA_PCM_NEW_HW_PARAMS_API

#define AV_DUMP_FORMAT 1

using namespace std;

#include "Test.h"




#endif //MYSTUDY_MYHEAD_H
