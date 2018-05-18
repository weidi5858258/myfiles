//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_MYHEAD_H
#define MYSTUDY_MYHEAD_H

#include <stdio.h>
#include <iostream>
#include <string>
//#include <sys/socket.h>
//#include <sys/epoll.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <setjmp.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

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

#endif //MYSTUDY_MYHEAD_H
