//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_MYHEAD_H
#define MYSTUDY_MYHEAD_H

////////////////////////////////start////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <wchar.h>
#include <limits.h>// 定义了一些常用类型的最小值,最大值
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <libgen.h>
#include <inttypes.h>
#include <math.h>
#include <dirent.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <pthread.h>
#include <time.h>
#include <netdb.h>



//下面三个头文件使用open函数时用到
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
////////////////////////////////STL start////////////////////////////////
//迭代器
#include <iterator>
//函数对象
#include <functional>
//顺序容器
#include <vector>
#include <deque>
#include <list>
#include <stack>
//关联容器
#include <set>
#include <map>
//容器适配器
#include <stack>
#include <queue>
////////////////////////////////STL end////////////////////////////////

////////////////////////////////boost start////////////////////////////////
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <boost/random.hpp>
#include <boost/range.hpp>
#include <boost/limits.hpp>
#include <boost/iterator.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
////////////////////////////////boost end////////////////////////////////

//windows头文件
#ifdef WIN32

#include <glut.h>
#include <io.h>
// 按键监控
#include <conio.h>

//ubuntu头文件
#else

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstddef>
//录音
#include <alsa/asoundlib.h>
#include <sys/mount.h>
#include <GL/glut.h>

#endif


#define PI 3.14159265

using namespace std;

#endif //MYSTUDY_MYHEAD_H

////////////////////////////////end////////////////////////////////

#ifdef WIN32
#endif
#ifdef linux
#endif

#ifdef WIN32
#else
#endif