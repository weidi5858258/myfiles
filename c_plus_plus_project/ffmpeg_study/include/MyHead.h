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
#include <inttypes.h>
#include <iostream>
#include <string>
#include <string.h>
// 定义了一些常用类型的最小值,最大值
#include <limits.h>
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
#include <sys/mount.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
//#include <opencv2/opencv.hpp>

// STL
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <pthread.h>

#include <mutex>
#include <condition_variable>

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

#include <SDL2/SDL.h>
//mp3 encode
#include <lame.h>
#include <cstddef>
//录音
#include <alsa/asoundlib.h>
#include <GL/glut.h>

//    #include <jconfig.h>
//    #include <jerror.h>
//    #include <jmorecfg.h>
//    #include <jpeglib.h>
//    #include <turbojpeg.h>

};// extern "C" end

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

//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)
//Break
#define BREAK_EVENT    (SDL_USEREVENT + 2)

#define ALSA_PCM_NEW_HW_PARAMS_API

#define AV_DUMP_FORMAT 1
#define SWR_CH_MAX 1024
#define PI 3.14159265

using namespace std;



#endif //MYSTUDY_MYHEAD_H
