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
//#include <opencv2/opencv.hpp>

// ffmpeg and SDL
extern "C" {// 不能少
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include "libswresample/swresample.h"
// libswscale是一个主要用于处理图片像素数据的类库.可以完成图片像素格式的转换,图片的拉伸等工作.
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/pixfmt.h>
#include <libavutil/fifo.h>
#include <libavutil/opt.h>
#include <libavutil/mem.h>

#include <SDL2/SDL.h>
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

using namespace std;

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
#define OUTPUT_YUV420P 0
#define OUTPUT_IMAGE 1
/***
 现在不能单独使用音频,必须先初始化视频,此后才能处理音频.
 因为在初始化视频时需要初始化dstAVFrame,dstAVFrame在
 音频中也要用到.
*/
#define NEED_VIDEO 1
#define NEED_AUDIO 1

#include "Test.h"




#endif //MYSTUDY_MYHEAD_H
