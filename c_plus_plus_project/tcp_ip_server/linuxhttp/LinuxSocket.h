//
// Created by weidi5858258 on 2017/12/30.
//

#ifndef GRAMMAR_LINUXSOCKET_H
#define GRAMMAR_LINUXSOCKET_H

#include <stdio.h>
#include <stdlib.h>


#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/ioctl.h>          //ioctl结构

#include <arpa/inet.h>
#include <net/if.h>             //ifreq结构
#include <netinet/in.h>         //in_addr结构
//#include <netinet/tcp.h> // 与#include <linux/tcp.h>中的struct tcphdr有重复定义
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <stddef.h>


#include <iostream>

#include <string>
#include <string.h>
#include <strings.h>
#include <thread>
// 多线程相关操作头文件，可移植众多平台
#include <pthread.h>

#include <linux/in.h>
//#include <linux/un.h> // struct sockaddr_un重复定义
#include <linux/if_ether.h>     //ethhdr结构
#include <linux/ip.h>           //iphdr结构
#include <linux/tcp.h>          //tcphdr结构
#include <linux/udp.h>          //udphdr结构



#include "../sqlite/SQLiteOpenHelper.h"
/***

*/

//开启大文件支持宏开关
#define _FILE_OFFSET_BITS 64

#define PORT 8888
#define BACKLOG 2

#define PTHREADS_NUM 5

using namespace std;

class LinuxSocket {
public:
    void studyHard();
};

#endif //GRAMMAR_LINUXSOCKET_H
