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

extern "C" {
    #include <jconfig.h>
    #include <jerror.h>
    #include <jmorecfg.h>
    #include <jpeglib.h>
    #include <turbojpeg.h>
}

#include "Test.h"

using namespace std;

#define MAX_EVENTS 500

static const char ZYGOTE_NICE_NAME[] = "zygote64";

#endif //MYSTUDY_MYHEAD_H
