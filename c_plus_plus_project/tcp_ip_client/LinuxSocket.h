//
// Created by weidi5858258 on 2017/12/30.
//

#ifndef GRAMMAR_LINUXSOCKET_H
#define GRAMMAR_LINUXSOCKET_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <signal.h>
// 多线程相关操作头文件，可移植众多平台
#include <pthread.h>
#include <iostream>
#include <linux/in.h>
#include <arpa/inet.h>

// 侦听端口地址
#define PORT 8888

using namespace std;

class LinuxSocket {
public:
    void studyHard();
};


#endif //GRAMMAR_LINUXSOCKET_H
