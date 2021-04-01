//
// Created by weidi5858258 on 2017/12/30.
//

#ifndef GRAMMAR_LINUXSOCKET_H
#define GRAMMAR_LINUXSOCKET_H

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <string.h>
#include <strings.h>
// 多线程相关操作头文件，可移植众多平台
#include <pthread.h>
#include <sched.h>
#include <iostream>
#include <linux/in.h>

//开启大文件宏开关
#define _FILE_OFFSET_BITS 64

// 侦听端口地址
//#define PORT 8888
#define PORT 41824

using namespace std;

class LinuxSocket {
public:
    void studyHard();
};


#endif //GRAMMAR_LINUXSOCKET_H
