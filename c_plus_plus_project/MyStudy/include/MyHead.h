//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_MYHEAD_H
#define MYSTUDY_MYHEAD_H

//#define UBUNTU_SYSTEM

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <iostream>
#include <string>
#include <string.h>
#include <limits.h>// 定义了一些常用类型的最小值,最大值
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <libgen.h>
#include <inttypes.h>
#include <math.h>
//下面三个头文件使用open函数时用到
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
//#include <opencv2/opencv.hpp>
//STL
//顺序容器
#include <vector>
#include <deque>
#include <list>
//关联容器
#include <set>
#include <map>
//容器适配器
#include <stack>
#include <queue>

#include <iterator>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <pthread.h>

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
#include <libavutil/avassert.h>
#include <libavutil/avstring.h>
#include <libavutil/frame.h>
#include <libavutil/hwcontext.h>
#include <libavutil/parseutils.h>
#include <libavutil/pixdesc.h>
#include <libavutil/pixfmt.h>
#include <libavutil/fifo.h>
#include <libavutil/log.h>
#include <libavutil/opt.h>
#include <libavutil/mem.h>
#include <libavutil/error.h>
#include <libavutil/time.h>


#ifdef UBUNTU_SYSTEM
    #include <sys/socket.h>
    #include <sys/epoll.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    //undefined reference to `WinMain@16'
    #include <SDL2/SDL.h>
    #include <lame.h>
    #include <cstddef>
    //录音
    #include <alsa/asoundlib.h>
    #include <sys/mount.h>
    #include <GL/glut.h>
#else
    #include <glut.h>
#endif

#ifdef UBUNTU_SYSTEM
#else
#endif

//    #include <jconfig.h>
//    #include <jerror.h>
//    #include <jmorecfg.h>
//    #include <jpeglib.h>
//    #include <turbojpeg.h>

};

#define PI 3.14159265

using namespace std;

#include "Test.h"


#endif //MYSTUDY_MYHEAD_H
