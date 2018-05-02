//
// Created by weidi5858258 on 2017/12/30.
//

#ifndef GRAMMAR_LINUXSOCKET_H
#define GRAMMAR_LINUXSOCKET_H

#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <string.h>
// 多线程相关操作头文件，可移植众多平台
#include <pthread.h>
#include <mutex>
#include <iostream>
#include <functional>
// #include <condition_variable>
// 动态加载库
#include <dlfcn.h>
extern "C" {

}

#include <stdarg.h>

#include "../mylog/MyLog.h"

#define PORT 8888
#define BACKLOG 2

using namespace std;

#define TAG "LinuxSocket"
#define NEEDLOG true

class LinuxSocket {
public:
    void studyHard();
};


#endif //GRAMMAR_LINUXSOCKET_H
