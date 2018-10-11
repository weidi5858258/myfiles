//
// Created by weidi5858258 on 2017/12/30.
//

#ifndef GRAMMAR_LINUXSOCKET_H
#define GRAMMAR_LINUXSOCKET_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string>
#include <string.h>
// 多线程相关操作头文件，可移植众多平台
#include <pthread.h>
#include <sched.h>
#include <iostream>
#include <linux/in.h>
/***

*/
#include <netdb.h>

#include "../sqlite/SQLiteOpenHelper.h"
/***

*/
#define PORT 8888
#define BACKLOG 2

#define PTHREADS_NUM 5

using namespace std;

class LinuxSocket {
public:
    void studyHard();
};

#endif //GRAMMAR_LINUXSOCKET_H
