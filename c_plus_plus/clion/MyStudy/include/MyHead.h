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
//#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
//#include <libavfilter/avfilter.h>
//#include <libavfilter/buffersink.h>
//#include <libavfilter/buffersrc.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libavutil/pixfmt.h>
#include <libavutil/fifo.h>
#include <libavutil/opt.h>

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

#define MAX_EVENTS 500

static const char ZYGOTE_NICE_NAME[] = "zygote64";

#include "Test.h"


#endif //MYSTUDY_MYHEAD_H
