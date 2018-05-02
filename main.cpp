#include <iostream>

using namespace std;

int main()
{

}

/***
C++语法
C++语言的基本数据类型
1）整数:short, int, long
2）浮点数:float, double, long double
3）字符:char
short           2       -32768~32767
unsigned short          0~65535
int             4       -2147483648~2147483647
unsigned int（或者unsigned）0~4294967295（32位系统）
long            4       -2147483648~2147483647
unsigned long           0~4294967295
float           4
double          8
long double     12
char            1       -128~127
bool            1       1，0

short int--->short
long int --->long（数后面加“l”或者“L”）
unsigned short int--->unsigned short
unsigned long int --->unsigned long

进程的终止方式：
1.从main返回
2.调用exit
3.调用_exit
4.调用abort
5.由一个信号终止
进程在终止的时候，系统会释放进程所拥有的资源，如内存、文件描述符、内核结构等。

进程之间的通信
进程之间的通信有多种方式，其中管道、共享内存和消息队列是最常用的方式。
管道是UNIX族中进程通信的最古老的方式，它利用内核在两个进程之间建立通道，它的特点是与文件的操作
类似，仅仅在管道的一端只读，另一端只写。利用读写的方式在进程之间伟弟数据。
，在多个进程之间共享。多个进程利用获得的共享内存的地址来直接对内存
进行操作。
消息则是在内核中建立一个链表，发送方按照一定的标识将数据发送到内核中，内核将其放入量表后，等待
接收方的请求。接收方发送请求后，内核按照消息的标识，从内核中将消息从链表中摘下，伟弟给接收方。
消息是一种完成的异步操作方式。

进程之间的同步
多个进程之间需要写作完成任务时，经常发生业务之间的依赖现象，从而出现了进程的同步问题。Linux
下进程的同步方式主要有消息、信号量等。
信号量是一个共享的表示数量的值。用于多个进程之间操作或者共享资源的保护，它是进程之间同步的最
主要方式。

Linux网络编程

套接字编程基础知识
1.通用套接字数据结构
通用的套接字地址类型的字义如下，
它可以在不同协议族之间进行强制轮换。
struct sockaddr                 // 套接字地址结构
{
    sa_family_t sa_family;      // 协议族
    char        sa_data[14];    // 协议族数据
}
typedef unsigned short sa_family_t;// 16字节
2.实际使用的套接字数据结构
在网络程序设计中所使用的函数中几乎所有的套接字函数都用struct sockaddr这个结构作为参数，但是使用结构struct sockaddr不方便进行设置，在以太网中，一般均采用struct sockaddr_in进行设置，这个结构的定义如下：
struct sockaddr_in             // 以太网套接字地址结构
{
    u8              sin_len;  // 结构struct sockaddr_in的长度，为16
    u8              sin_family;// 通常为AF_INET
    u16             sin_port;  // 16位的端口号，网络字节序
    struct in_addr  sin_addr;  // IP地址，为32位
    char            sin_zero[8];// 未用
}
struct in_addr  // IP地址结构
{
    u32 s_addr; // 32位IP地址，网络字节序
}
3.结构体struct sockaddr和结构体struct sockaddr_in的关系
两者都是一个同样大小的结构体。
struct sockaddr_in中的成员含义：
3-1)sin_len:无符号字符类型，表示struct sockaddr_in的长度，为16个字节
3-2)sin_family:无符号字符类型，通常设置为与socket函数的domain一致，例如AF_INET。
3-3)sin_port:无符号short类型，表示端口号，网络字节序。
3-4)sin_addr:struct in_addr类型，其成员s_addr为无符号32位数，每8位表示IP地址的一人段，网络字节序。
3-5)sin_zero[8]:char类型，保留。
由于结构体struct sockaddr和结构体struct sockaddr_in的大小是完全一致的，所以进行地址结构设置时，通常的方法是利用结构体struct sockaddr_in进行设置，然后强制转换为结构体struct sockaddr类型。因为这两个结构大小是完全一致的，所以进行这样的转换不会有什么副使用。

TCP网络编程流程
TCP网络编程架构
TCP网络编程有两种模式，一种是服务器模式，另一种是客户端模式。服务器模式创建一个服务程序，等待客户端用户的连接，接收到用户的连接请求后，根据用户的请求进行处理；客户端模式则根据目的服务器的地址和端口进行连接，向服务器发送请求并对服务器的响应进行数据处理。
1.服务器的程序设计模式
流程要主分为套接字初始化（socket()），套接字与端口的绑定（bind()），设置服务器的侦听连接（listen()），接受客户端的连接（accept()），接收和发送数据（read(),write()）并进行数据处理及处理完毕的
套接字关闭（close()）.
服务端的accept()跟客户端的connect()连接成功需要三次握手。

创建网络插口函数socket()
网络程序设计中的套接字系统调用socket()函数用来获得文件描述符。
函数原型为：
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
domain:用于设置网络通信的域，函数socket()根据这个参数选择通信协议的族。通信协议簇在文件sys/socket.h中定义。
type:用于设置套接字通信的类型。
protocol:用于指定某个协议的特定类型，即type类型中的某个类型。通常某个协议中只有一种特定类型，这样protocol参数仅能设置为0；但是有些协议有多种特定的类型，就需要设置这个参数来选择特定的类型。


绑定一个地址端口对bind()
在建立套接字文件描述符成功后，需要对套接字进行地址和端口的绑定，才能进行数据的接收和发送操作。
函数原型为：
#include <sys/types.h>
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen);
sockfd:用socket()函数创建的文件描述符。
my_addr:指向一个结构体为struct sockaddr参数的指针，包含了地址，端口和IP地址的信息。在进行地址绑定的时候，需要先将地址结构中的IP地址，端口，类型等结构体struct sockaddr中的域进行设置后才能进行绑定，这样进行绑定后才能将套接字文件描述符与地址等结合在一起。
addrlen:结构体struct sockaddr的长度，可以设置成sizeof(struct sockaddr)。
bind()函数的返回值为0时表示绑定成功，-1时表示绑定失败。
struct sockaddr_un
{
    sa_family sun_family;        // 协议族，应该设置为AF_UNIX
    char sun_path[UNIX_PATH_MAX];// 路径名，UNIX_PATH_MAX的值为108
}

bind()例子：
#define MY_SOCK_PATH "/somepath"
int main(int argc, char* argv[])
{
    int sfd;
    // AF_UNIX对应的结构体
    struct sockaddr_un addr;
    // 初始化一个AF_UNIX族的流类型socket
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 检查是否正常初始化socket
    if(sfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 将变量addr置为0
    memset(&addr, 0, sizeof(struct sockaddr_un));
    // 协议族为AF_UNIX
    addr.sun_family = AF_UNIX;
    // 复制路径到地址结构体struct sockaddr_un中
    strncpy(addr.sun_path, MY_SOCK_PATH, sizeof(addr.sun_path));

    if(bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 数据接收发送及处理过程
    ...
    // 关闭套接字文件描述符
    close(sfd);
}

Linux的GCC编译器有一个特点，一个结构的最后一个成员为数组时，这个结构可以通过最后一个成员进行扩展，可以在程序运行时第一次调用此变量的时候动态生成结构的大小，如上面的代码，并不会因为struct sockaddr_un比struct sockaddr大而溢出。

下例代码是使用结构体struct sockaddr_in绑定一个AF_INET族的流协议，先将结构struct sockaddr_in的sin_family设置为AF_INET，然后设置端口，接着设置一具IP地址，最后进行绑定。
#define MYPORT 3490
int main(int argc, char* argv[])
{
    // 套接字文件描述符变量
    int sockfd;
    // 以太网套接字地址结构
    struct sockaddr_in my_addr;
    // 初始化socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // 地址结构的协议族
    my_addr.sin_family = AF_INET;
    // 地址结构的端口地址，网络字节序
    my_addr.sin_port = htons(MYPORT);
    // IP，将字符串的IP地址转化为网络字节序
    my_addr.sin_addr.s_addr = inet_addr("192.168.1.150");

    // 将my_addr.sin_zero置为0
    bzero($(my_addr.sin_zero), 8);
    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 接收和发送数据，进行数据处理
    ...
    // 关闭套接字文件描述符
    close(sockfd);
}

上面两个例子中有两点不同，
AF_UNIX对应着结构struct sockaddr_un
AF_INET对应着结构struct sockaddr_in
不过在调用bind()函数时都要把这两个结构强制转化为struct sockaddr*。

监听本地端口listen
服务器模式中有listen()和accept()两个函数，而客户端则不需要这两个函数。函数listen()用来初始化服务器可连接队列，服务器处理客户端连接请求的时候是顺序处理的，同一时间仅能处理一个客户端连接。当多个客户端的连接请求同时到来的时候，服务器并不是同时处理，而是将不能处理的客户端连接请求放到等待队列中，这个队列的长度由listen()函数来定义。
函数原型为：
#include <sys/socket.h>
int listen(int sockfd, int backlog);
当listen()函数成功运行时，返回值为0；当运行失败时，它的返回值为-1，并且设置errno值。
在接受一个连接之前，需要用listen()函数来侦听端口，listen()函数中参数backlog的参数表示在accept()函数处理之前在等待队列中的客户端的长度，如果超过这个长度，客户端会返回一个ECONNREFUSED错误。listen()函数仅对SOCK_STREAM或者SOCK_SEQPACKET的协议有效。例如，如果对一个SOCK_DGRAM的协议使用listen()，将会出现错误errno的值为EOPNOTSUPP，表示此socket不支持listen()操作。大多数系统的设置为20，可以将其设置修改为5或者10，根据系统可承受负载或者应用程序的需求来确定。
下面是一个listen()函数的实例代码，在成功进行socket()初始化和bind()端口之后，设置listen()队列的长度为5。
#define MYPORT 3490
int main(int argc, char* argv[])
{
    // 套接字文件描述符变量
    int sockfd;
    // 以太网套接字地址结构
    struct sockaddr_in my_addr;
    // 初始化socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // 地址结构的协议族
    my_addr.sin_family = AF_INET;
    // 地址结构的端口地址，网络字节序
    my_addr.sin_port = htons(MYPORT);
    // IP，将字符串的IP地址转化为网络字节序
    my_addr.sin_addr.s_addr = inet_addr("192.168.1.150");

    // 将my_addr.sin_zero置为0
    bzero($(my_addr.sin_zero), 8);
    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 进行侦听队列长度的绑定
    if(listen(sockfd, 5) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 接收数据，发送数据和数据的处理过程
    ...
    // 关闭套接字
    close(sockfd);
}

接受一个网络请求accept()
当一个客户端的连接请求到达服务器主机侦听的端口时，此时客户端的连接会在队列中等待，直到使用服务器处理接收请求。
函数accept()成功执行后，会返回一个新的套接字文件描述符来表示客户端的连接，客户端连接的信息可以通过这个新的描述符来获得。因此当服务器成功处理客户端的请求连接后，会有两个文件描述符，老的文件描述符表示正在监听的socket，新产生的文件描述符表示客户端的连接，函数send()和recv()通过新的文件描述符进行数据收发。
函数原型为：
#include <sys/types.h>
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
sockfd:用socket()函数创建的文件描述符。
addr:
addrlen:表示addr所指内容的长度，可以使用sizeof(struct sockaddr_in)来获得。它是一个指针而不是结构，accept()将这个指针传给TCP/IP协议栈。
通过accept()函数可以得到成功连接客户端的IP地址，端口和协议族等信息，这个信息是通过参数addr获得的。
accept()函数的返回值是新连接的客户端套接字文件描述符，与客户端之间的通信是通过accept()返回的新套接字文件描述符来进行的，而不是通过建立套接字时的文件描述符，这是在程序设计的时候需要注意的地方。如果accept()函数发生错误，accept()会返回-1，通过errno可以得到错误值。
例子：
#define BACKLOG 10
int main(int argc, char* argv[])
{
    int socket_fd, client_fd;
    // 本地地址信息
    struct sockaddr_in my_addr;
    // 客户端连接的地址信息
    struct sockaddr_in client_addr;
    // int类型变量，用于保存网络地址长度量
    int addr_length;
    socket_fd = socket();
    if(socket_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 协议族为IPv4，主机字节序
    my_addr.sin_family = AF_INET;
    // 端口，短整型，网络字节序
    my_addr.sin_port = htons(MYPORT);
    // 自动IP地址获得
    my_addr.sin_addr.s_addr = INADDR_ANY;

    // 将my_addr.sin_zero置为0
    bzero($(my_addr.sin_zero), 8);
    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 进行侦听队列长度的绑定
    if(listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 地址长度
    addr_length = sizeof(struct sockaddr_in);
    // 等待客户端连接，地址在client_addr中
    client_fd = accept(socket_fd, &client_addr, &add_length);
    if(client_fd == -1)
    {
        perror("accept");
        exit();
    }

    ...
    // 关闭客户端连接
    close(client_fd);
    ...
    // 关闭服务端连接
    close(socket_fd);
}

连接目标网络服务器connect()
客户端在建立套接字之后，不需要进行地址的绑定，就可以直接连接服务器。连接服务器的函数为connect()，此函数连接指定参数的服务器，例如IP地址，端口等。
函数原型为：
#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd, struct sockaddr* serv_addr, int addrlen);
sockfd：建立套接字时返回的套接字文件描述符，它是由系统调用socket()返回的。
serv_addr:是一个指向数据结构struct sockaddr的指针，其中包括客户端需要连接的服务器的目的端口和IP地址以及协议类型。
addrlen：表示数据结构struct sockaddr内容的大小，可以使用sizeof(struct sockaddr)获得。
connect()函数的返回值在成功时为0，当发生错误的时候返回-1，可以查看errno获得错误的原因。
例子：
#define DEST_IP "132.241.5.10"
#define DEST_PORT 23
int main(int argc, char* argv[])
{
    int ret = 0;
    int sockfd;
    struct sockaddr_in server;
    // 初始化一个IPv4族的流式连接
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(DEST_PORT);
    // 服务器的IP地址
    server.sin_addr.s_addr = htonl(DEST_IP);
    // 保留字段置0
    bzero(&(server.sin_zero), 8);

    ret = connect(sockfd, (struct sockaddr*)&server, sizeof(sockaddr));
    ...
    close(sockfd);
}

写入数据函数write()
当服务端在接收到一个客户端的连接后，可以通过套接字描述符进行数据的写入操作。对套接字进行写入的形式和过程与普通文件的操作方式一致，内核会根据文件描述符的值来查找所对应的属性，当为套接字的时候，会调用对应的内核函数。
下面是一个向套接字文件描述符中写入数据的例子，将缓冲区data的数据全部写入套接字文件描述符s中，返回值为成功写入的数据长度。
int size;
char data[1024];
size = write(s, data, 1024);

读取数据函数read()
读取数据的方式如下所示，从套接字描述符s中读取1024个字节，放入缓冲区data中，size变量的值为成功读取的数据大小。
int size;
char data[1024];
aize = read(s, data, 1024);

关闭套接字函数close()
关闭socket连接可以使用close()函数实现，函数的作用是关闭已经打开的socket连接，内核会释放相关的资源，关闭套接字之后就不能再使用这个套接字文件描述符进行读写操作了。
函数shutdown()可以使用更多方式来关闭连接，允许单方向切断通信或者切断双方的通信。
函数原型为：
#include <sys/socket.h>
int shutdown(int s, int how);
s:切断通信的套接字文件描述符。
how:表示切断的方式
函数shutdown()用于关闭双向连接的一部分，具体的关闭行为方式通过参数的how设置来实现。可以为如下值：
SHUT_RD:值为0，表示切断读，之后不能使用此文件描述符进行读操作。
SHUT_WR:值为1，表示切断写，之后不能使用此文件描述符进行写操作。
SHUT_REWR:值为2，表示切断读写，之后不能使用此文件描述符进行读写操作，与close()函数功能相同。
函数shutdown()如果调用成功则返回0，如果失败则返回-1，通过errno可以获得错误的具体信息。
  
截取信号
在Linux操作系统中当某些状况发生变化时，系统会向相关的进程发送信号。信号的处理方式是系统会先调用进程中注册的处理函数，然后调用系统的默认响应方式，包括终止进程，因此在系统结束进程前，注册信号处理函数进行一些处理是一个完善程序的必须条件。
信号处理
信号是发生某件事情时的一个通知，有时候也将称其为中断。信号将事件发送给相关的进程，相关进程可以对信号进行捕捉并处理。信号的捕捉由系统自动完成，信号处理函数的注册通过函数signal()完成。
函数原型为：
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
这个函数向信号signum注册一个void (*sighandler_t)(int)类型的函数，函数的句柄为handler。当进程中捕捉到注册的信号时，会调用响应函数的句柄handler。信号处理函数在处理系统默认的函数之前会被调用。
信号SIGPIPE
如果正在写入套接字的时候，当读取端已经关闭时，可以得到一个SIGPIPE信号。信号SIGPIPE会终止当前进程，因为信号系统在调用系统默认处理方式之前会先调用用户注册的函数，所以可以通过注册SIGPIPE信号的处理函数来获取这个信号，并进行的处理。
如当服务端已经关闭，而客户端试图向套接字写入数据的时候会产生一个SIGPIPE信号，此时将造成程序的非正常退出。可以使用signal()注册一个处理函数，释放资源，进行一些善后工作。下面的例子将处理函数sig_pipe()挂接到信号SIGPIPE上。
void sig_pipe(int sign)
{
    printf("Catch a SIGPIPE signal\n");
    // 释放资源
}
signal(SIGPIPE, sig_pipe);

信号SIGINT
信号SIGINT通常是由Ctrl+C终止进程造成的，与Ctrl+C一致，kill命令默认发送SIGINT信号，用于终止进程运行向当前活动的进程发送这个信号。
void sig_int(int sign)
{
    printf("Catch a SIGINT signal\n");
    // 释放资源
}
signal(SIGINT, sig_pipe);

服务器和客户端信息的获取
字节序
字节序是由于不同的主处理器和操作系统，对不大于一个字节的变量在内存中的存放顺序不同而产生的。如2个字节的short int和4个字的int    类型变量都有字节序的问题。字节序通常有大端字节序和小端字节序两种分类方法。
大端字节序和小端字节序
字节序是由于CPU和OS对多字节变量的内存存储顺序不同而产生的。
大于一个字节的变量类型的表示方法有两种：
小端字节序（）：在表示变量的内存地址的起始地址存放低字节，高字节顺序存放；
大端字节序（）：在表示变量的内存地址的起始地址存放高字节，低字节顺序存放。

什么情况下会出现字节序的问题，如果出现了要怎样处理后才能得到正确的结果，如果不处理又会出现什么样的结果？

下面的一段代码用于检查变量在内存中的表示方法，确定系统中的字节序为大端字节序还是小端字节序。
（1）字节序结构。程序先建立一个联合类型to，用于测试字节序，成员value是short类型变量，可以通过成员byte来访问value变量的高字节和低字节。
#include <stdio.h>
typedef union
{
    unsigned short int value;
    unsigned char byte[2];
}to;
（2）变量声明。声明一个to类型的变量typeordr，将值0xabcd赋给成员变量value。由于在类型to中，value和byte成员共享同一块内存，所以可以byte的不同成员来访问value的高字节和低字节。
int main(int argc, char* argv[])
{
    to typeorder;
    typeorder.value = 0xabcd;
}
（3）小端字节判断。小端字节序的检查通过判断typeorder变量的byte成员高字节和低字节的值来进行：低字节的值为0xcd,高字节的值为0xab。
// 小端字节序检查
if(typeorder.byte[0] == 0xcd && typeorder.byte[1] == 0xab)
{
    printf("Low endian byte order byte[0]:0x%x, byte[1]:0x%x\n",typeorder.byte[0], typeorder.byte[1]);
}
（4）大端字节序判断。大端字节序的检查同样通过判断typeorder变量的byte成员高字节和低字节的值来进行：低字节的值为0xab,高字节的值为0xcd。
// 大端字节序检查
if(typeorder.byte[0] == 0xab && typeorder.byte[1] == 0xcd)
{
    printf("High endian byte order byte[0]:0x%x, byte[1]:0x%x\n",typeorder.byte[0], typeorder.byte[1]);
}

字节序转换函数
网络字节序是指多字节变量在网络传输时的表示方法，网络字节序采用大端字节序的表示方法。这样小端字节序的系统通过网络传输变量的时候需要进行字节序的转换，大端字节序的变量则不需要进行轮换。
#include <arpa/inet.h>
// 主机字节序到网络字节序的长整型转换
uint32_t htonl(uint32_t hostlong);
// 主机字节序到网络字节序的短整型转换
uint16_t htons(uint16_t hostshort);
// 网络字节序到主机字节序的长整型转换
uint32_t ntohl(uint32_t netlong);
// 网络字节序到主机字节序的短整型转换
uint16_t ntohs(uint16_t netshort);
函数传入的变量为需要转换的变量，返回值为转换后的数值。
函数的命名规则为“字节序”“to”“字节序”“变量类型”。在上述函数中，h表示host，即主机字节序n表示network，即网络字节序；l表示long类型变量，s表示short类型变量。函数htonl()的含义为“主机字节序”转换为“网络字节序”，操作的变量为“long类型变量”。
两个对short类型进行转换的函数为htons()和ntohs(),两个对long类型变量进行转换的函数为htonl()和ntohl()。

下面的例子是对16位数值和32位数值进行字节序转换，每种类型的数值进行再次转换，最后打印结果。
1.16位字节序转换结构
先定义用于16位字节序转换的结构to16，这个结构是一个联合类型，通过value来赋值，通过byte数组来进行字节序转换。
#include <stdio.h>
typedef union
{
    unsigned short int value;
    unsigned char byte[2];
}to16;
2.32位字节序转换结构
typedef union
{
    unsigned long int value;
    unsigned char byte[4];
}to32;
3.变量值打印函数showvalue()
showvalue()函数用于打印变量值，打印的方式是从变量存储空间的第一个字节开始，按照字节进行打印。showvalue()函数有两个输入参数，一个是变量的地址指针begin,一个是表示字长的标志flag。参数flag的值为BITS16的时候打印16位变量的值，参数flag为BITS32的时候打印32位变量的值。
#define BITS16 16
#define BITS32 32
void showvalue(unsigned char* begin, int flag)
{
    int num = 0, i = 0;
    if(flag == BITS16)
    {
        num = 2;
    }
    else if(flag == BITS32)
    {
        num = 4;
    }

    for(i = 0; i < num; ++i)
    {
        printf("%x ", *(begin+i));
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    to16 v16_orig, v16_turn1, v16_turn2;
    to32 v32_orig, v32_turn1, v32_turn2;

    v16_orig.value = 0xabcd;
    v16_turn1.value = htons(v16_orig.value);
    v16_turn2.value = htons(v16_trun1.value);

    v32_orig.value = 0x12345678;
    v32_turn1.value = htonl(v32_orig.value);
    v32_turn2.value = htonl(v32_turn1.value);

    printf("16 host to network byte order change:\n");
    printf("\torig:\t");
    showvalue(v16_orig.byte, BITS16);
    printf("\t1 tiems:\t");
    showvalue(v16_turn1.byte, BITS16);
    printf("\t2 tiems:\t");
    showvalue(v16_turn2.byte, BITS16);

    printf("32 host to network byte order change:\n");
    printf("\torig:\t");
    showvalue(v32_orig.byte, BITS32);
    printf("\t1 tiems:\t");
    showvalue(v32_turn1.byte, BITS32);
    printf("\t2 tiems:\t");
    showvalue(v32_turn2.byte, BITS32);

    return 0;
}

字符串IP地址和二进制IP地址的转换
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// 将点分四段式的IP地址转为地址结构in_addr值
int inet_aton(const char* cp, struct in_addr* inp);
// 将字符串转换为in_addr值
in_addr_t inet_addr(const char* cp);
// 将字符串地址的网络部分转为in_addr类型
in_addr_t inet_network(const char* cp);
// 将in_addr结构地址转为字符串
char* inet_ntoa(struct in_addr in);
// 将网络地址和主机地址合成为IP地址
struct in_addr inet_makeaddr(int net, int host);
// 获得地址的主机部分
in_addr_t inet_lnaof(struct in_addr in);
// 获得地址的网络部分
in_addr_t inet_netof(struct in_addr in);

1.inet_aton()函数
inet_aton()函数将在cp中存储的点分十进制字符串的IP地址，转换为二进制的IP地址，转换后的值保存在指针inp指向的结构structure in_addr中。当转换成功时返回值为非0，当传入的地址非法时，返回值为0。
2.inet_addr()函数 
inet_addr()函数将cp中存储的点分十进制字符串类型的IP地址转换为二进制的IP地址，IP地址是以网络字节序表达的。如果输入的参数非法，返回值为INADDR_NONE(通常为-1)，否则返回值为转换后的IP地址。
这个函数是函数inet_aton()的缩减版，由于值-1(1111111111111111)同时可以理解为是合法IP地址255.255.255.255的转换结果，所以不能使用这个函数转换IP地址255.255.255.255。
3.inet_network()函数
4.inet_ntoa()函数
5.inet_makeaddr()函数
6.inet_lnaof()函数
7.inet_netof()函数
8.结构struct in_addr
结构struct in_addr在文件<netinet/in.h>中定义，结构in_addr额一个unsigned long int类型的成员变量s_addr。通常所说的IP地址的二进制形式就保存在成员变量s_addr中。
struct in_addr
{
    unsigned long int s_addr;// IP地址
}
函数inet_addr()，inet_network()的返回值为-1时表示错误，这占用了255.255.255.255的值，因此可能存在缺陷。函数inet_ntoa()的返回值为一个指向字符串的，这块内存每次调用函数inet_ntoa()后都会覆盖，因此函数并不安全，可能存在某种隐患。

函数inet_pton()和函数inet_ntop()是一套安全的协议无关的地址转换函数。所谓的“安全”是相对于inet_aton()函数的不可重入性来说。这两个函数都是可以重入的，并且这引函数支持多种地址类型，包括IPv4和IPv6。

函数inet_pton()将字符串类型的IP地址转换为二进制类型，函数原型为：
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int inet_pton(int af, const char* src, void* dst);
af:表示网络类型的协议族，在IPv4下的值为AF_INET。
src:表示需要转换的字符串。
dst:指向转换后的结果，在IPv4下，dst指向结构struct in_addr的指针。
当函数inet_pton()的返回值为-1时，通常是用于af所指定的协议族不支持造成的，此时errno的返回值为EAFNOSUPPORT；当函数的返回值为0时，表示src指向的值不是合法的IP地址；当函数的返回值为正值时，表示转换成功。

函数inet_ntop()函数将二进制的网络IP地址转换为字符串，函数原型为：
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
const char* inet_ntop(int af, const void* src, char* dst, socklen_t cnt);
af:表示网络类型的协议族，在IPv4下的值为AF_INET。
src:为需要转换的二进制IP地址，在IPv4下，src指向一个struct in_addr结构类型的指针。
dst:指向保存结果缓冲区的指针。
cnt:dst缓冲区的大小。
函数inet_ntop()返回一个指向dst的指针。当发生错误时，返回NULL。当af设定的协议族不支持时，errno
设置为EAFNOSUPPRORT；当dst缓冲区大小过小的时候，errno的值为ENOSPC。

套接字描述符判定函数issockettype()
套接字文件描述符从形式上与通用文件描述符没有区别，判断一个文件描述符是否是一个套接字描述符可以通过如下的方法实现：先调用函数fstat()获得文件描述符的模式，然后将模式的S_IFMT部分与标识符S_IFSOCK比较，可以知道一个文件描述符是否为套接字描述符。
函数issockettype()先获得描述符的状态，保存在变量st中，将st的成员st_mode与S_IFMT进行“与”运算后获取文件描述符的模式。判断上述值是否与S_IFSOCK相等，可以知道文件描述符是否为套接字描述符。
int issockettype(int fd)
{
    struct stat st;
    // 获得文件的状态
    int err = fstat(fd, &st);
    if(err < 0)
    {
        return -1;
    }

    // 比较是否套接字描述
    if((st.st_mode & S_IFMT) == S_IFSOCK)
    {
        // 是套接字描述符
        return 1;
    }
    else
    {
        // 不是套接字描述符
        return 0;
    }
}

int main(int argc, char* argv[])
{
    // 查询标准输入是否为套接字描述符
    int ret = issockettype(0);
    printf("value %d\n", ret);

    // 建立套接字描述符
    int s = socket(AF_INET, SOCK_STREAM, 0);
    // 查询是否为套接字描述符
    ret = issockettype(s);
    printf("value %d\n", ret);

    return 0;
}
输出结果表明标准输入不是套接字描述符，而建立的SOCK_STREAM类型的套接字是套接字描述符，通过上述方法可以正确判定一个文件描述符是否是套接字文件描述符。

IP地址与域名之间的相互转换
在实际的使用中，经常有只知道主机的域名而不知道主机名对应的IP地址的情况，而socket的API均为IP地址，所以如何进行主机域名和IP地址之间的转换是十分必要的。 
获取主机信息的函数
gethostbyname()函数和gethostbyaddr()函数都可以获得主机的信息。gethostbyname()函数通过主机的名称获得主机的信息，gethostbyaddr()函数通过IP地址获得主机的信息。

1.gethostbyname()函数的原型为：
#include <netdb.h>
extern int h_errno;
struct hostent* gethostbyname(const char* name);
根据主机名获取主机的信息，如www.sina.com.cn,使用gethostbyname("wwww.sina.com.cn")可以获得主机的信息。这个函数的参数name是要查询的主机名，通常是DNS的域名。
gethostbyname()函数的返回值是一个指向结构struct hostent类型变量的指针，当为NULL时，表示发生错误，错误类型可以通过errno获得，错误的类型及含义如下：
HOST_NOT_FOUND:查询的主机不可知，即查不到相关主机的信息。
NO_ADDRESS和NO_DATA:请求的名称合法但是没有合适的IP地址。
NO_RECOVERY:域名服务器不响应。
TRY_AGAIN:域名服务器当前出现临时性错误，稍后再试。
struct hostent
{
    char*   h_name;         // 主机的正式名称
    char**  h_aliases;      // 别名列表（主机名0主机名1...NULL）
    int     h_addrtype;     // 主机地址类型（当为AF_INET时表示为IPv4）
    int     h_length;       // 地址长度（主机地址长度L，以字节为单位）
    char**  h_addr_list;    // 地址列表（主机地址0主机地址1...NULL）
}
成员h_name是主机的官方名称，如新浪的www.sina.com.cn。
成员h_sliases是主机的别名，别名可能有多个，所以用一个链表表示，链表的尾部是一个NULL指针。
成员h_addrtype是主机的地址类型，AF_INET表示IPv4的IP地址，AF_INET6表示IPv6的IP地址。
成员h_length是IP地址的长度，对于IPv4来说为4，即4个字节。
成员h_addr_list是主机的IP地址的链表，每个都为h_length长，链表的尾部是一个NULL指针。

2.gethostbyaddr()函数的原型为：
#include <netdb.h>
#include <sys/socket.h>
struct hostent* gethostbyaddr(const void* addr, int len, int type);
addr:在IPv4的情况下指向一个struct in_addr的地址结构，用户需要查询主机的IP地址填入到这个参数中。
len:表示第一个参数所指区域的大小，在IPv4情况下为sizeof(struct in_addr),即32位。
type:指定需要查询主机IP地址的类型，在IPv4的情况下为AF_INET。
其返回什玫错误代码含义与函数gethostbyname()相同。

gethostbyname()函数和gethostbyaddr()都是不可重入的函数，由于传出的值为一块静态的内存地址，当另一次查询到来的时候，这块区域会被占用，所以在使用的时候要小心。

协议名称处理函数
协议族处理函数有如下几个，可以通过协议的名称，编号等获取协议类型。
#include <netdb.h>
struct protoent* getprotoent(void);// 从协议文件中读取一行
struct protoent* getprotobyname(const char* name);// 从协议文件中找到匹配项
struct protoent* getprotobynumber(int proto);// 按照协议类型的值获取匹配项
void setprotoent(int stayopen);// 设置协议文件打开状态
void endprotoent(void);// 关闭协议文件
上面的函数对文件/etc/protocols中的记录进行操作，文件中记录了协议的名称，值和别名等值。与结构struct protoent的定义一致。
struct protoent
{
    char*   p_name;     // 协议的官方名称 
    char**  p_aliases;  // 别名列表（协议别名0协议别名1...NULL）
    int     p_proto;    // 协议的值
}
成员p_nameo为指向协议名称的指针。 
成员p_aliases是指向别名列表的指针，协议的别名是一个字符串。 
成员p_proto是协议的值。
函数getprotobynumber()执照输入的协议值proto，匹配文件/etc/protocols中的选项，返回一个匹配项。
函数setprotoent()打开文件/etc/protocols，当stayopen为1时，在调用函数getprotobyname()或者函数getprotobynumber()查询协议时，并不关闭文件。
函数endprotoent()关闭文件/etc/protocols。
函数getprotoent(),getprotobyname()和getprotobynumber()在调用成功时返回一个指向结构struct protoent的指针，失败时，返回NULL。

#include <sys/types.h>
#include <sys/stat.h>
const char *filePath = "/data/vendor/recognition/put_image.dat";
chmod(filePath, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH);// 774

继承：public private protected
继承后的子类，调用sizeof()函数后容量会变大的，因为父类的容量也要加上。
自身的私有与继承的私有还是有区别的。
protect的属性不能被对象所访问，但是可以让子类访问。
通过对象只能访问公有的属性或者函数。
父类的公有函数被子类保护继承后，用子类的对象不能再调用父类的公有函数了，因为这时父类的公有函数变成了保护，原来父类保护的函数被保护继承后还是保护，私有的还是私有。

static_cast：对于隐式类型可以转化的，即可用此类型
dynamic_cast：
reinterpret_cast：对于双方都不能相互转化的，即可用此类型
const_cast：只能应用于指针和引用

float a = 5.6;
int b = 5;
b = static_cast<int>(a);
a = static_cast<float>(b);

void* p;
int* q;
p = q;
q = p;// error
q = static_cast<int*>(p);

int x = 10;
int y = 3;
float z = x / y;
float z = static_cast<float>(x) / y;

#include <stdlib.h>
char* pc = static_cast<char*>(malloc(100));

char* p;
int* q;
p = reinterpret_cast<char*>(q);

int a[5] = {1,2,3,4,5};
int* p = (short*)((int)a + 1);// error
int* p = reinterpret_cast<int*>((reinterpret_cast<int>(a)+1));

void func(int& v){
    
}
const int a = 20;
func(const_cast<int&>(a));

函数模版
// T表示未确定的模板参数类型
template <class T>
void printArray(T* array, int size){
    ...
}

类模版
// T是一个模板参数
template <class T>
class Stack{
public:
    Stack(int s = 10);
    ~Stack();

    int push(const T&);
    int pop(T&);
    int isEmpty() const;
    int isFull() const;
    void printOn(ostream& output);
private:
    int size;
    int top;
    T* stackPtr;
}


echo "y" | apt-get install gnupg && 
echo "y" | apt-get install flex && 
echo "y" | apt-get install bison && 
echo "y" | apt-get install gperf && 
echo "y" | apt-get install build-essential && 
echo "y" | apt-get install zip && 
echo "y" | apt-get install curl && 
echo "y" | apt-get install libc6-dev && 
echo "y" | apt-get install x11proto-core-dev && 
echo "y" | apt-get install libgl1-mesa-dev && 
echo "y" | apt-get install g++-multilib && 
echo "y" | apt-get install mingw32 && 
echo "y" | apt-get install tofrodos && 
echo "y" | apt-get install python-markdown && 
echo "y" | apt-get install libxml2-utils && 
echo "y" | apt-get install xsltproc && 
echo "y" | apt-get install dpkg-dev && 
echo "y" | apt-get install libx11-dev:i386 && 
echo "y" | apt-get install zlib1g-dev:i386 && 
echo "y" | apt-get install libncurses5-dev:i386 && 
echo "y" | apt-get install libreadline6-dev:i386 && echo "0" || echo "1"
echo "y" | apt-get upgrade && echo "y" | apt-get dist upgrade
echo "y" | apt-get install libx11-dev && 
echo "y" | apt-get install zlib1g-dev && 
echo "y" | apt-get install libncurses5-dev && 
echo "y" | apt-get install libreadline6-dev && echo "0" || echo "1"


echo "y" | apt-get install ubuntu-desktop && 
echo "y" | apt-get install unity && 
echo "y" | apt-get install ccsm && echo "0" || echo "1"

0001    1
0010    2
0011    3
0100    4
0101    5
0110    6
0111    7
1000    8
1001    9
1010    A
1011    B
1100    C
1101    D
1110    E
1111    F

virtual status_t addService(const String16& name, const sp<IBinder>& service,  bool allowIsolated) {  
    Parcel data, reply;  
    data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());  
    data.writeString16(name);  
    data.writeStrongBinder(service);  
    data.writeInt32(allowIsolated ? 1 : 0);  
    status_t err = remote()->transact(ADD_SERVICE_TRANSACTION, data, &reply);  
    return err == NO_ERROR ? reply.readExceptionCode() : err;  
}
从代码知，实例化了两个Parcel栈上对象data和reply，data主要是传送给servicemanager程序，而reply是从servicemanager返回的结果。
Data写入的值有
InterfaceToken : “android.os.IServiceManager” 
String16 : “media.player”
StrongBinder : MediaPlayerService 
Int32 ：0
通过remote()->transact(ADD_SERVICE_TRANSACTION, data, &reply);
BpBinder(0)把Parcel data发送到ServiceManager

引用的基础知识
C++函数中参数的传递方式是传值。在函数域中为参数重新分配内存，而把实参的数值传递到新分配的内存中。它的优点是有效避免函数的副作用。这个优点恰恰却是某些方面的缺点。
引用不是定义一个新的变量，而是给一个已经定义的变量重新起一个别名，也就是C++系统不为引用类型分配内存空间。引用主要用于函数之间的数据传递。定义的格式为：类型& 引用变量名 = 已定义过的变量名；
如：double number; double& newnum = number;

引用作为参数的形参：

引用作为函数的返回值：
一般函数返回值时，要生成一个临时变量作为返回值的拷贝，而用引用作为返回值时，不生成值的拷贝。
如果在一个函数中局部生成一个对象，当这个函数的返回值是对象时，能得到正确的答案，当这个函数的返回值是对象的引用时，就不能得到正确的答案了，因为对象引用使用了这个局部对象的地址，而这个局部对象出了这个函数后就析构了，因此调用这个函数处就得不到正确的结果了，所以在这种情况下要返回对象，不能是对象的引用。

返回值为引用的函数作为左值：


对数组只能引用数组元素，不能引用数组（数组名本身为地址）
不能定义引用的引用（引用也是地址），所以当函数的参数为引用时，引用不能作为实参。

结普通变量的引用
int a = 10;
int& b = a;
对指针的引用
int a = 10;
int* p = &a;
int* &q = p;// 对指针p的引用，p的地址跟q的地址是一样的
对数组的引用
int ar[3] = {1, 2, 3};
int (&br)[3] = ar;// br是数组ar的别名，br的地址跟ar的地址是一样的
对常量的引用
const int x = 100;
int& y = x;// error
const int& y = x;// ok

int n = 20;
const int& m = n;// ok

const引用：引用在内部存放的是被引用对象的地址，不可寻址的值是不能引用的；当引用作为形参时，实参也不能使用不可寻址的值，更不可能进行类型转换（如：实数转换为整数）。但是const引用不同，它是只读的，为了绝对保证不会发生误改，编译器实现const引用时，生成一具临时对象，引用实际上指向该临时对象，但是用户不能访问它。所以const引用可以实现不可寻址的值（包括字面常量）的引用，如：
const double dval = 1024;
int& ri = dval;// error
const int& ri = dval;是正确的，编译器将其转换为：
const double dval = 1024;
int temp = dval;
const int& ri = temp;// ri的地址与dval的地址是不一样的，ri的地址跟temp的地址是一样的
因有临时对象，引用和类型转换都实现了。当const引用作为形参时，实参也能使用不可寻址的值，并能进行类型转换。
临时变量一般来说具备常量的性质（这点很重要），因此不能用一个可变的引用去指向这个临时变量，这在语法上是不允许的。

拷贝构造函数的基础知识
同一个类的对象在内存中有完全相同的结构，如果作为一个整体进行复制或称拷贝是完全可行的。这个拷贝过程只需要拷贝数据成员，而函数成员是共用的（只有一份拷贝）。在建立对象时可用同一类的另一个对象来初始化该对象，这时所用的构造函数称为拷贝构造函数（Copy Constructor）。
CGoods(const CGoods& good){
    strcpy(name, good.name);
    price = good.price;
    amount = good.amount;
    total_value = good.total_value;
}
使用：
CGoods good1;
...
CGoods good2(good1);或者CGoods good2 = good1;（初始化。）// 都会调用拷贝构造函数（两者完全等价）
CGoods good2;
good2 = good1;（赋值。）// 不会再调用拷贝构造函数，会调用赋值语句

C++四少（系统会默认生成）
构造函数
拷贝构造函数
赋值语句
析构函数
CGoods* operator&(){
    return this;
}
const CGoods* operator&() const {
    return this;
}

对象初始化调用的是拷贝构造函数，对象赋值调用的是赋值语句。

拷贝构造函数的参数必须采用---引用。如果一个真实的类对象作为参数传递到拷贝构造函数，会引起无穷递归。
系统会自动提供，称为缺省的按成员语义支持的拷贝构造函数，每个类成员被依次拷贝，亦称为缺省的按成员初始化。按成员作拷贝是通过依次拷贝每个数据成员实现的，而不是对整个类对象按位拷贝。赋值运算符“=”称缺省的按成员拷贝赋值操作符，同类对象之间可以用“=”直接拷贝。
通常按成员语义支持已经足够。但在某些情况下，它对类与对象的安全性和处理的正确性还不够，这时就要求类的设计者提供特殊的拷贝构造函数和拷贝赋值操作符的定义。
拷贝构造函数还在另外两个方面使用：
1.当函数的形参是类的对象，调用函数时，进行形参与实参结合时使用。这时要在内存新建立一个局部对象，并把实参拷贝到新的对象中。
2.当函数的返回值是类的对象时，函数执行完成返回调用者时使用。理由也是要建立一具临时对象中，再返回调用者。因为局部对象在离开建立它的函数时就消亡了，不可能在返回调用函数后继续生存，所以在处理这种情况时，编译系统会在调用函数的表达式中创建一个无名临时对象，该临时对象的生存周期只在函数调用处的表达式中。所谓return对象，实际上是调用拷贝构造函数把该对象的值拷入临时对象。如果返回的是变量，处理过程类似，只是不调用构造函数。

赋值语句
// CGoods& operator(CGoods* const this, const CGoods& goods){}
CGoods& operator=(const CGoods& goods){
    if(this != &goods){
        data = goods.data;
    }
    // 把返回值也设为引用，是为了赋值时可以连续赋值
    return *this;
}
// 使用这种会调用一下拷贝构造函数，因此一般不使用，会使用上面方式
void operator=(CGoods goods){
    
}

CGoods goods1(10);
CGoods goods2,goods3;
// operator=(&goods2, goods1);
// goods2.operator=(goods1);
goods2 = goods1;
// goods3.operator=(goods2.operator=(goods1));
goods3 = goods2 = goods1;

int a, b, c;
a = b = c = 1;

函数的调用优化
1.
CGoods func(CGoods goods){
    int data = goods.getData();
    CGoods goods2(data);
    return goods2;
}
使用：CGoods goods1(100); CGoods goods2; goods2 = func(goods1);
2.
CGoods& func(CGoods goods){
    int data = goods.getData();
    return CGoods(data);// 创建无名临时对象的语法
}
使用：CGoods goods1(100); CGoods goods2; goods2 = func(goods1);
3.
CGoods func(CGoods& goods){
    int data = goods.getData();
    return CGoods(data);// 创建无名临时对象的语法
}
使用：CGoods goods1(100); CGoods goods2; goods2 = func(goods1);
4.(最优)
CGoods func(CGoods& goods){
    int data = goods.getData();
    return CGoods(data);// 创建无名临时对象的语法
}
使用：CGoods goods1(100); CGoods goods2 = func(goods1);

深拷贝与浅拷贝
// C++默认会生成
CGoods* operator&(){
    return this;
}
// CGoods* pc = goods3.operator&();
使用：CGoods goods3; CGoods* pc = &goods3;

// C++默认会生成
const CGoods* operator&() const {
    return this;
}
使用：const CGoods goods4; const CGoods* pc = &goods4;

深拷贝：不仅把值拷贝过来，还要自己重新创建内存空间用于存放这个值。

class String{
public:
    String(const char* str = ""){
        if(*str == NULL){
            data = (char*)malloc(sizeof(char));
            data[0] = '\0';
        } else {
            data = (char*)malloc(sizeof(char) * strlen(str) + 1);
            strcpy(data, str)
        }
    }
    String(const String& s){
        data = (char*)malloc(sizeof(char) * (strlen(s.data) + 1));
    }
    // 分四步走
    // 1.if(this != $s)
    // 2.free(data);
    // 3.data = (char*)malloc(sizeof(char) * (strlen(s.data) + 1));
    // 4.return *this;
    String& oprator=(const String& s){
        if(this != $s){
            free(data);
            data = NULL;
            // 这里需要作处理，就是防止内存不够，申请失败
            data = 
            (char*)malloc(sizeof(char) * (strlen(s.data) + 1));
            strcpy(data, s.data);
        }
        return *this;
    }
    ~String(){
        free(data);
        data = NULL;
    }
private:
    char* data;
};
void main(){
    char* str = "Hello";
    String s1(str);
    String s2(s1);
    String s3("World");
    s3 = s2;
}

运算符的重载
运算符重载实际是一种特殊的函数重载，必须定义一个函数，并告诉
C++编译器，当遇到该重载的运算符时调用眦函数。这个函数叫做运算符
重载函数，通常为类的成员函数。
定义运算符重载函数的一般格式：
返回值类型 类名::operator重载的运算符（参数表）{...}
因函数名的特殊性，C++编译器可以将这类函数识别出来。
使用引用类型变量作为运算符重载函数的参数，可以提高复数类型运算的
效率：
Complex Complex::operator+(const Complex& comp){
    return Complex(real + comp.real, image + comp.image);
}
这里采用Complex对象的引用而不是对象本身，调用时不再重新分配内存
建立一个复制的对象，函数效率会更高。而在引用形式参数类型说明前
加const关键字，表示被引用的实参是不可改变的，如程序员不当心在
函数体中重新赋值了被引用的实参，C++编译器会认为出错。
重载的运算符“+=”标准算法是：
Complex& Complex::operator+=(Complex& comp){
    real += comp.real;
    image += comp.image;
    return *this;
}
使用：c1 += c2; c1.operator+=(c2);
小结：
1.运算符重载函数的函数名必须为关键字operator加一个合法的运算符。
在调用该函数时，将右操作数作为函数的实参。
2.当用类的成员函数实现运算符的重载时，运算符重载函数的参数
（当为双目运算符时）为一个或（当为单目运算符时）没有。运算符的
左操作数一定是对象，因为重载的运算符是该对象的成员函数，而
右操作数是该函数的参数。
3.单目运算符“++”和“--”存在前置与后置的问题。
前置“++”格式为：
返回类型 类名::operator++(){...}
后置“++”格式为：
返回类型 类名::operator++(int){...}
后置“++”中的参数int仅用作区分，并无实际意义，可以给一个变量名，
也可以不给变量名。
Int& operator++(){
    m_i++;
    return *this;
}
Int operator++(int){
    Int temp(m_i);
    m_i++;
    return temp;
}
4.C++中只有极少数的运算符不允许重载
?:       三目条件运算符
.和.*    成员操作符
::       作用域操作符
sizeof   类型字长操作符

ostream& operator<<(ostream& out, const Complex& comp){
    out<<...<<endl;
    return out;
}

static方法与const方法
一个常规的成员函数声明描述了三件在逻辑上相互不同的事情
1.该函数能访问类声明的私有部分
2.该函数位于类的作用域之中
3.该函数必须经由一个对象去激活（有一个this指针）
通过将函数声明为static，可以让它只有前两种性质，通过将一个函数声明
为友元可以使它具有第一种性质。
由关键字static修饰说明的类成员，成为静态类成员
(static class member)。虽然使用static修饰说明，但与函数中的
静态变量有明显差异。类的静态成员为其所有对象共享，不管有多少对象，
静态成员只有一份存于公用内存中。
// void list(Test* const this){...}
// 由于const修饰的是this,而this代表的是对象的地址，对象地址不能变，可是对象可以改变，
// 因此对象的属性也可以改变
void list(){data = 0;}
// void list(const Test* const this){...}
// 由于const修饰的是*this,而*this代表的是对象，对象不能变，刚对象的属性也就不能变
void list() const{data = 0;// error}

动态内存分配
堆区（自由存储区）
int *p = (int*)malloc(sizeof(int) * 10);
if(p == NULL){
    exit(1);
}
for(int i=0;i<10;++i){
    p[i] = i + 1;
}

int *q = new int[10];
for(int j=0;j<10;++j){
    q[j] = j + 1;
}
delete []q;
q = NULL;

int *q = new int(10);// 申请空间并初始化
delete q;

在C++中，申请和释放堆中分配的存储空间，分别使用new和delete的两个
运算符来完成，其使用的格式为：
指针变量名 = new 类型名（初始化式）；
delete 指针变量名；
new运算符返回的是一个指向所分配类型变量（对象）的指针。对所创建的
变量或对象，都是通过该指针来间接操作的，而动态创建的对象本身没有
名字。

new operator:
1.operator new开辟空间
2.constructor调用构造函数
delete operator:
1.~constructor调用析构函数
2.operator delete回收空间

void* operator new(size_t size){
    void* p = malloc(size);
    return p;
}
void operator delete(void* p){
    free(p);
}

new:
String* ps = (String*)operator new(sizeof(String));
new(ps)String("Hello");
delete:
ps->~String();
operator delete(ps);

void* operator new(size_t size){
    void* p = malloc(size);
    return p;
}
void operator delete(void*p){
    free(p);
}
void* operator new[](size_t size){
    void* p = malloc(size);
    return p;
}
void operator delete[](void* p){
    free(p);
}

继承访问属性细说
公有继承
私有继承
保护继承
1.不管是什么继承，都会把父类的私有属性也给继承下来，只是不能访问
而已。
#include <iostream>
using namespace std;

class Base{
public:
    Base() : x(0){}
    ~Base();

    void showBase(){}

private:
    int x;
};

class Child : public Base{
public:
    Child() : y(0){}
    ~Child();

    void showChild(){}

private:
    int y;
};

class Child : private Base{
public:
    Child() : y(0){}
    ~Child();

    void showChild(){}

private:
    int y;
};

class Child : protected Base{
public:
    Child() : y(0){}
    ~Child();

    void showChild(){}

private:
    int y;
};

类的派生与继承
派生类的构造函数与析构函数

class D : public B1, public B2{};
D d;
d.B1::n = 10;
d.B2::n = 20;

#include <iostream>
using namespace std;

// 虚拟继承
// 虚基类
class B0{
    public:
        B0() : m(0){}
    public:
        int m;
}

class B1 : virtual public B0{
    public:
        B1() : n(0){}
    public:
        int n;
};

class B2 : virtual public B0{
    public:
        B2() : n(0){}
    public:
        int n;
};

class D : public B1, public B2{
    public:
        D() : x(0){}
    private:
        int x;
};

void main(){
    D d;// 构造B1, B2, D
    d.n = 10;// error(二义性)
    d.B1::n = 10;// ok
    d.B2::n = 20;// ok
    d.m = 100;// error
    // m的地址是一样的
    d.B1::m = 200;
    d.B2::m = 300;
    d.m = 100;// ok
}

在子类中只要覆写了父类中的一个同名函数，那么父类中的所有这个同名
函数（参数不同，函数名相同）都将被隐藏。

派生类与基类：
在任何需要基类对象的地方都可以用公有派生类的对象来代替，这条规则
称为赋值兼容规则。它包括以下情况：
1.派生类的对象可以赋值给基类的对象，这时是把派生类对象中从对应基类中继承来的隐藏对象赋值给基类对象。反过来不行，因为派生类的新成员
无值可赋。
2.可以将一个派生类的对象的地址赋给其基类的指针变量，但只能通过
这个指针访问派生类中由基类继承来的隐藏对象，不能访问派生类中的
新成员。同样也不能反过来做。
3.派生类对象可以初始化基类的引用。引用是别名，但这个别名只能包含派生类对象中的由基类继承来的隐藏对象。

重点结论：
1.在父类中的函数如果加了virtual关键字，子类又重写了这个
函数（三同：返回值相同，函数名相同，参数相同），
那么当父类的指针（或者引用）指向子类时，这个指针调用这个
函数就会去调用子类中的重写的那个函数；如果不加virtual关键字，
那么调用的是父类的函数。
2.如果是父类的对象调用函数（这个函数被子类重写，
并且加入virtual），并且是子类的对象赋给父类的对象，
结果调用的永远是父类的函数，不会
是子类的。

虚函数是一个类的成员函数，定义格式为：
virtual 返回类型 函数名（参数表）；
关键字virtual指明该成员函数为虚函数。virtual仅用于定义中，如虚函数在类外定义，不可加virtual。
当某一个类的一个类成员函数被定义为虚函数，则由该类派生出来的所有
派生类中，该函数始终保持虚函数的特性。
虚只用作函数的声明，不能用作函数的实现。除非声明和实现一起写。
当在派生类中重新定义虚函数时，不必加关键字virtual。但重新定义时
不仅要同名，而且它的参数和返回类型全部与基类中的虚函数一样，否则
编译时出错。



#include <iostream>
using namespace std;

class Base{
    public:
        virtual void show(){
            cout<<"This is Base show()."<<endl;
        }
};

class D : public Base{
    public:
        void show(){
            cout<<"This is D show()."<<endl;
        }
        void print(){
            cout<<"This is D print()."<<endl;
        }
};

void main(){
    D d;
    Base b = d;
    b.show();// 调用的永远是父类的函数
    Base* pb = &d;
    pb->show();// ok 调用的是子类函数
    pb->print();// error
    Base &bf = d;
    bf.show();// ok 调用的是子类函数
}


Linux 只是系统核心（kernel），通常跟 GNU 整合才成为完整的操作系统，这样的系统称为 GNU/Linux 系统“发行套件”，知名的有 Debian、Ubuntu、Redhat、Fedora、Suse、Arch、SteamOS 等，此外也有基底是 Linux，上层是别的元件，例如 Google 的 Andorid、Chromebook 等。TechNews科技新报界面JMedia联盟成员

// 有纯虚函数的类就叫做“抽象类”。
// 有子类继承这个“抽象类”时，必须全部实现纯虚函数，不然子类还是
// 一个“抽象类”，不能实例化对象。
class A{
    public:
        // 纯虚函数
        virtual void eat() = 0;
        virtual void sleep() = 0;
        // 纯虚函数也可以有实现体，不过这实现体永远执行不了
        virtual void run() = 0 {
            cout<<""<<endl;
        }
};
定义纯虚函数必须注意：
1.
定义纯虚函数时，不能定义虚函数的实现部分（不过语法上是可以的）。
即使是函数体为空也不可以，函数体为空就可以执行，只是什么也不做就
返回。而纯虚函数不能调用。
2.
“=0”表明程序员将不定义该函数，函数声明是为派生类保留一个位置。
“=0”本质上是将指向函数体的指针定为NULL。
3.
在派生类中必须有重新定义的纯虚函数的函数体，这样的派生类才能
用来定义对象。

流类库和输入输出

wget -d wwww.baidu.com // 下载百度的网页资源。-d表示打印信息。
得到的内容如下：
DEBUG output created by Wget 1.17.1 on linux-gnu.

Reading HSTS entries from /root/.wget-hsts
URI encoding = ‘UTF-8’
--2017-07-15 09:50:09--  http://www.baidu.com/
Resolving www.baidu.com (www.baidu.com)... 119.75.213.61, 119.75.216.20
Caching www.baidu.com => 119.75.213.61 119.75.216.20
Connecting to www.baidu.com (www.baidu.com)|119.75.213.61|:80... connected.
Created socket 3.
Releasing 0x00007fffe24ed7a0 (new refcount 1).

---request begin--- （请求）
GET / HTTP/1.1 （版本号）
User-Agent: Wget/1.17.1 (linux-gnu) （我的客户端）
Accept: \*\/\* 
（可接收的文件类型。这里使用了转义字符，不然注释到这里就结束了）
Accept-Encoding: identity
Host: www.baidu.com （请求的地址）
Connection: Keep-Alive （保持连接）
（这里有两个回车换行，表示报头结束了）
---request end---
HTTP request sent, awaiting response...
---response begin--- （响应）
HTTP/1.1 200 OK （200表示请求已经成功）
Server: bfe/1.0.8.18
Date: Sat, 15 Jul 2017 01:50:13 GMT
Content-Type: text/html
Content-Length: 2381
Last-Modified: Mon, 23 Jan 2017 13:27:36 GMT
Connection: Keep-Alive
ETag: "588604c8-94d"
Cache-Control: private, no-cache, no-store, proxy-revalidate, no-transform
Pragma: no-cache
Set-Cookie: BDORZ=27315; max-age=86400; domain=.baidu.com; path=/
Accept-Ranges: bytes

---response end---
200 OK
cdm: 2 3 4 5 6 7 8
Stored cookie baidu.com -1 (ANY) / <permanent> <insecure> [expiry 2017-07-16 09:50:09] BDORZ 27315
Registered socket 3 for persistent reuse.
Length: 2381 (2.3K) [text/html]
Saving to: ‘index.html’

index.html              100%[============================>]   2.33K  --.-KB/s    in 0s

2017-07-15 09:50:09 (78.3 MB/s) - ‘index.html’ saved [2381/2381]

Saving HSTS entries to /root/.wget-hsts
网页内容结束。

tcpdump -x host 192.168.3.108 and tcp port 2001

Apache
HTTP SERVER PROJECT
1.Apache软件基金会开发
2.最流行的Web服务器端软件
3.号称“Linux/Unix平台杀手级应用”
4.可以运行在三大主流操作系统平台上
5.高效、稳定、功能强大并且可通过简单的API扩展
    负载均衡
    并发限制
    SSL支持
    图像处理
    压缩
    LDAP认证
    带宽限制
    CGI V8 引擎
    日志监控
6.开源

项目目标三要素：时间、成本、质量

单位时间内系统处理的客户请求的数量（常用单位有TPS）


Linux配置IP地址的方法：
1.ifconfig命令临时配置IP地址
2.setup工具永久配置IP地址
3.修改网络配置文件
4.图形界面配置IP地址

ifconfig eth0 192.168.0.200 netmask 255.255.255.0
ifconfig eth0:0 192.168.0.100
ifconfig eth0:0 down

Linux网络配置文件：
/etc/sysconfig/network-scripts/ifcfg-eth0

规范化：
初始化：
char* prot_override = NULL;

#include <getopt.h>
getopt()和getopt_long()的函数说明
getopt是一个专门设计用来减轻命令行参数处理负担的库函数，它可以在
全局结构中记录命令参数，以便以后随时在整个程序中使用，即getopt被
用来解析命令行选项参数，就不用自己写代码处理argv参数了。
(1) main()中的两个参数。声明main（）函数有两种形式：
int main( int argc, char *argv[] ); 
int main( int argc, char **argv );
现在前者用的较多。当 C 运行时库的程序启动代码调用 main()
时，已经对命令行进行了处理。argc 参数包含参数的计数值，
而 argv 包含指向这些参数的指针数组。对于 C 运行时库，
arguments 是程序的名称，程序名后的任何内容都应该使用空格
加以分隔。
例如，如果使用参数 -v bar www.ibm.com 运行一个名为 foo 程序，
您的 argc 将设置为 4，argv 的设置情况则是：

argv[0] - foo

argv[1] - -v 

argv[2] - bar 

argv[3] - www.ibm.com

struct 结构类型名{
    数据类型 成员名1;
    数据类型 成员名2;
    数据类型 成员名3;
    ... 
}; 

关于结构类型的几点说明：
1.结构定义的位置，可以在函数内部，也可以在函数外部。在函数内部
定义的结构，只能在函数内部使用；在函数外部定义的结构，其有效范围
是从定义处开始，直到它所在的源程序文件结束。
2.大括号后要以分号“;”结束。
3.结构类型定义是一种数据类型的格式定义，只给出这种数据类型的构成
情况，并不涉及内存的分配。
4.结构类型中的成员类型，既可以是基本数据类型，也允许是另一个已经
定义的结构类型。
5.结构中成员的类型不能是被描述的结构本身。如
struct invalid{
    int n;
    struct invalid iv;// error
};
这种描述引起了无穷嵌套，既不合理也不可能在计算机里实现。若成员
类型是描述的结构本身的指针是合法的。
6.数据类型相同的项，既可逐个、逐行分别定义，也可合并成一行定义。
7.成员名可以和程序中的变量名相同，两者代表不同的对象。如
int x;
int y;
struct point{
    int x;
    int y;
};

结构变量的定义
用户自己定义的结构类型，与系统定义的基本类型(int,char,float等)
一样，仅仅是一种数据类型，没有为各个成员分配内存单元。只有定义
了结构变量后才分配内存单元，并能对结构变量进行操作。
1.间接定义法
struct 结构类型名 结构变量名;
在定义变量之后，编译系统会适时地为结构变量分配一组连续的内存空间。
sizeof(struct stud) 或 sizeof(student)
2.直接定义法
在定义结构类型的同时，定义结构变量。其定义形式为：
struct 结构类型名{
  ...  
}结构变量表; 
如
struct stud{
    int num;
    char name[20];
    float score[3];
    struct date birthday;
}student1, student2;
也可以定成
struct {
    int num;
    char name[20];
    float score[3];
    struct date birthday;
}student1, student2;
必须注意：
2-1.结构类型与结构变量是两个不同的概念，其区别如同int类型与int型
变量的区别一样。
2-2.用间接定义法定义结构变量时，必须给出“struct 结构类型名”这一
完整的结构类型。
2-3.每种定义方式的末尾都有一个分号“;”结束。

结构变量的使用
结构变量的使用主要是通过结构变量成员的操作来实现的。
1.结构变量成员的引用
引用结构变量成员的方式为：
结构变量名.成员名
其中的“.”是成员运算符，它是最高优先级的运算符之一。
#include <stdio.h>
struct date{
  int year, month, day;  
};
struct stud{
    int num;
    char name[20];
    float score[3];
    struct date birthday;
};
main(){
    struct stud student;
    student.num = 12;
    strcpy(student.name, "Li Ming");
    student.score[0] = 85;
    student.score[1] = 90;
    student.score[2] = 88;
    student.birthday.year = 1970;
    student.birthday.month = 12;
    student.birthday.day = 17;
    printf("Num: %d\n", student.num);
    // student.name是字符数组名，本身代表地址
    printf("Name: %s\n", student.name);
    printf("Score: %d %d %d\n", student.score[0], 
        student.score[1], student.score[2]);
    printf("Birthday: %d-%d-%d\n", student.birthday.year,
        student.birthday.month, student.birthday.day);
}
2.结构变量赋值
结构变量赋值是通过结构变量的成员逐个赋值来完成的。
注意：只能对最低一级成员赋值。
对结构变量的成员可以像普通变量样进行各种运算（根据其类型决定
可以进行的运算）。
3.结构变量的输入和输出
若结构变量的值不是赋值获得，而是程序运行时由键盘输入，则结构变量的
输入只能对最低一级的成员进行。
scanf("%d%s", %student.num, student.name);
scanf("%d%d%d", &student.score[0], %student.score[1], 
    &student.score[2]);
scanf("%d%d%d", &student.birthday.year, &student.birthday.month,
    &student.birthday.day);
注意：不能引用结构变量的地址整体读入结构变量的值。如
scanf("%d%s%c%f%d%d%d", &student);// error
&student就结构变量student的首地址，结构变量的地址主要用于作函数
参数，传递结构的地址。
4.结构变量之间的赋值
当两个结构变量的类型相同时，它们互相可以进行整体赋值。
student2 = student1;
执行此语句后，student2中各成员的值与student1中各成员的值完全一致。
也就是说，相同类型的结构变量的赋值实际上是成员之间一一对应的赋值。
注意：结构变量之间除了整体赋值外，其他的整体运算都不允许。如
student++;// error
student1 == student2;// error
5.结构变量的初始化
struct stud student = {12, "Li Ming", 85, 90, 88, {1970, 12, 17}};
注意：
5-1.初值的顺序和数据类型，应与结构变量中相应成员的顺序和类型相一致
5-2.必须在结构变量定义时直接初始化。
// error
strucct stud student;
student = {12, "Li Ming", 85, 90, 88, {1970, 12, 17}};

结构与数组
具有相同类型的结构变量本身也可以构成数组，称为结构数组。
结构数组的元素是结构类型。结构数组既具有结构属性又含有数组属性，
定义结构数组实质上是定义一具数据库，其每个数组元素是一条记录。
结构数组的定义形式如下：
struct 结构类型名 结构数组名[元素个数];
#include <stdio.h>
#define MAX 3
struct stud{
    int num;
    char name[20];
    char sex;
    int score[3];
};
main(){
    struct stud students[MAX];
    int i, j;
    for(i = 0;i < MAX; i++){
        printf("Input the student No %d:\n", i+1);
        scanf("%d", &students[i].num);// 遇到空格就输入
        getchar();
        gets(students[i].name);// 可以输入空格
        student[i].sex = getchar();
        for(j = 0;j < MAX; j++)
            scanf("%d", &students[i].score[j]);
    }

    printf("\nNum Name Sex S1 S2 S3 ");
    for(i=0;i<MAX;i++){
        printf("\n%d%s%c ", students[i].num, students[i].name,
            students[i].sex);
        for(j=0;j<MAX;j++){
            printf("%d ", students[i].score[j]);
        }
    }
}
结构数组直接定义：
struct stud{
    int num;
    char name[10];
    char sex;
} students[MAX]; 
结构数组的初始化
初始化的格式为：
struct 结构类型名 结构数组名[n] = {{初值表1}, {初值表1}, ...};
结构数组在定义时可初始化为：
struct stud students[3] = {
    {10, "A", 'm', 78, 85, 90},
    {11, "B", 'f', 78, 85, 90},
    {12, "C", 'm', 78, 85, 90}
};
每个元素占用字节数相同，为sizeof(student[0])个字节，共占用
sizeof(students)个字节。数组名students为结构数组在内存中的首地址。
注意：结构数组也一定要在定义时就拌初始化。
struct stud{
    int num;
    char name[10];
    char sex;
    int score[3];
} students[] = {
    {10, "A", 'm', 78, 85, 90},
    {11, "B", 'f', 78, 85, 90},
    {12, "C", 'm', 78, 85, 90}
};

结构与指针
结构成员可以是指针类型的变量
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct stud{
    int num;
    char name[10];
    char sex;
    int score[3];
    char* addr;
};
main(){
    int i;
    struct stud student[3];
    char s[20];
    printf("\nEnter student's num name sex score(3) addr:");
    for(i=0;i<3;i++){
        scanf("%d", &student[i].num);
        getchar();
        gets(student[i].name);
        student[i].sex = getchar();
        scanf("%d%d%d", &student[i].score[0], 
            &student[i].score[1], &student[i].score[2]);
        getchar();
        gets(s);
        student[i].addr = (char*)malloc(strlen(s)+1);
        strcpy(student[i].addr, s);
    }
    for(i=0;i<3;i++){
        printf("\n%d%s%c", student[i].num, 
            student[i].name, student[i].sex);
        printf("\t%d%d%d", student[i].score[0],
            student[i].score[1], student[i].score[2]);
        printf("\t%s", student[i].addr);
    }
}

// 主要用在链表结构中
struct valid{
    int n;
    struct valid* pv;
};// ok

指向结构变量的指针
结构变量在内存中的起始地址称为结构变量的地址。
1.指向结构变量的指针
结构类型定义之后，就可以指向该结构类型的指针变量，其定义形式为：
struct 结构类型名* 指针变量名;
struct stud student , *p;
结构变量student定义后，便为其分配了结构类型的内存单元。而结构指针
变量p定义后，也为p变量分配了内存单元，用来存放一个结构变量存储空间
的起始地址。但此时的p未指向属于struct stud类型的任何变量，当然也不
能由p指针对结构变量做任何的操作。
下面使用赋值语句：
p = &student;
则p指向结构变量student，也就是student在内存单元的首地址赋值给了
指针变量p。此时可用结构指针p来引用结构中的成员。其效果等价于直接
通过结构变量的引用。引用方式有两种：
a. (*p).成员名
b. p->成员名
注意：因成员运算符“.”的优先级高于间接访问运算符“*”，故前一种方式中
的括号不能省略。“->”称为指向结构成员运算符。
#include<stdio.h>
struct stud{
    int num;
    char name[20];
    char sex;
    int score[3];
};
main(){
    struct stud student = {22, "A", 'm', 78, 85, 90};
    struct stud* p;
    p = &student;
    printf("\nNum Name Sex S1 S2 S3 ");
    printf("\n %d %s %c %d %d %d ", p->num, p->name, p->sex,
        p->score[0], p->score[1], p->score[2]);
    printf("\n %d %s %c %d %d %d ", (*p).num, (*p).name, 
        (*p).sex, (*p).score[0], (*p).score[1], (*p).score[2]);
}

2.指向结构数组的指针
当把结构数组的首地址赋值给同类型的结构指针时，这个指针就是指向
结构数组的指针。
使用指向结构数组的指针来访问结构数组。
#include <stdio.h>
struct stud{
    int num;
    char name[20];
    char sex;
    int score[3];
};
main(){
    struct stud students[] = {
        {10, "A", 'm', 78, 85, 90},
        {11, "B", 'f', 78, 85, 90},
        {12, "C", 'm', 78, 85, 90}
    };
    struct stud *p;
    printf("\nNum Name Sex S1 S2 S3 ");
    for(p=students; p<students+3;p++){
        printf("\n %d %s %c %d %d %d ", p->num, p->name,
            p->sex, p->score[0], p->score[1], p->score[2]);
    }
}
几点说明：
2-1.通过结构指针来操作结构数组时，一般把数组名作为指针的初值。
2-2.如果指针变量p已指向某结构数组，则p自增1（如p++）后使p指向结构数组的下一个元素，而不是当前元素的下一个成员。

结构与函数
1.基本数据类型作为函数形参
当函数的形参是基本数据类型时，相同类型的结构变量成员可作为函数调用
时的实参。
#include <stdio.h>
struct fraction{
    int fx;
    int fm;
};
int max_div(int m, int n);// 函数的原型说明
main(){
    int maxd;
    struct fraction num;
    num.fz = 24;
    num.fm = 36;
    maxd = max_div(num.fz, num.fm);
    printf("\n24/36 single fraction is: %d/%d", num.fz/maxd,
        num.fm/maxd);
}
int max_div(int m, int n){
    int r;
    r = m % n;
    while(r != 0){
        m = n;
        n = r;
        r = m % n;
    }
    return n;
}
2.结构变量和结构指针作为函数的形参
2-1.函数的形参是结构变量
当函数的形参是结构变量时，主调函数中的实参必须是同类型的结构变量。
类似于普通变量，函数在调用过程中，实参和形参对应不同的内存单元，
只能实现值传递。
#include <stdio.h>
#define MAX 3
struct stud{
    int num;
    char name[20];
    char sex;
    int score[3];
};
float aver(struct stud st);
main(){
    struct stud students[MAX];
    int i, j;
    for(i=0;i<MAX;i++){
        printf("\nInput the student No %d:\n", i+1);
        scanf("%d", &students[i].num);
        getchar();
        gets(students[i].name);
        students[i].sex = getchar();
        for(j=0;j<MAX;j++)
            scanf("%d", &studentss[i].score[j]);
    }
    printf("\nNum Name Sex S1 S2 S3 Aver ");
    for(i=0;i<MAX;i++){
        printf("\n %d %s %c ", students[i].num, students[i].name,
            students[i].sex);
        for(j=0;j<MAX;j++){
            printf("%d", students[i].score[j]);
        }
        printf("%4.1f", aver(student[i]));
    }
}
float aver(struct stud st){
    int i;
    float average = 0.0;
    for(i=0;i<MAX;i++){
        average += st.score[i];
    }
    average /= 3;
    return average;
}
2-2.函数的形参是结构指针
当函数的形参是结构指针时，用指向结构变量（或数组）的指针做实参，
函数在调用过程中，把结构变量（或数组）的地址传递给形参，故实参和
形参所指向的是同一组内存单元，实现的是地址传递。
#include <stdio.h>
#define MAX 3
struct stud{
    int num;
    char name[20];
    char sex;
    int score[3];
    float ave;
};
void aver(struct stud *st);
main(){
    struct stud students[MAX];
    int i, j;
    for(i=0;i<MAX;i++){
        printf("\nInput the student No %d:\n", i+1);
        scanf("%d", &students[i].num);
        getchar();
        gets(students[i].name);
        students[i].sex = getchar();
        for(j=0;j<MAX;j++)
            scanf("%d", &studentss[i].score[j]);
    }
    aver(students);
    printf("\nNum Name Sex S1 S2 S3 Aver ");
    for(i=0;i<MAX;i++){
        printf("\n %d %s %c ", students[i].num, students[i].name,
            students[i].sex);
        for(j=0;j<MAX;j++){
            printf("%d", students[i].score[j]);
        }
        printf("%4.1f", students[i].ave);
    }
}
void aver(struct stud *st){
    int i;
    float average = 0.0;
    for(i=0;i<MAX;i++){
        average += st->score[i];
    }
    st->ave = average / MAX;
}
2-3.结构变量作函数的返回值

联合
结构与联合之间的异同在于：
1.结构与联合老是由多个成员分量组成的一个整体。
2.结构与联合在定义、说明和使用上十分相似。
3.结构：多个成员分量分别占用不同的存储空间构成一个整体；
    成员分量之间是相互独立的，所进行的各种操作互不影响。
4.联合：多个成员分量共同占用同一存储空间；成员分量之间是
相互联系的，所进行的操作也是相互依赖的。
联合的定义和引用
定义：
union 联合类型名{
    数据类型 成员名1;
    数据类型 成员名2;
    ...
};
union u_type{// 定义联合类型
    char ch;
    int i;
    long li;
} u1, u2, *pu;// 说明联合类型的变量和指针变量
联合变量的定义与结构变量的定义类似。
union u_type u1, u2, *pu;
pu = &u1;
联合变量占用的内存空间等于所需字节数最多的那个成员的长度，而不是
各成员长度之和。
三个成员ch、i和li共享内存单元，则它们在内存中的起始地址也相同。
&u1 == &u1.ch == &u1.i == &u1.li
共用变量的引用---与结构变量一样，也只能逐个引用共用变量的成员。
联合变量名.成员名
联合指针变量名->成员名
#include "bios.h"
main(){
    union u_type{
        int i;
        char ch[2];
    }key;
    key.i = bioskey(0);
    printf("\n ASCII = %x, Scancode = %x", key.ch[0], key.i);
}
联合变量只能以第一个成员的身份进行初始化。
任何时候只能有一个成员使用共享内存；如执行u1.i = 1, u1,ch = 'c',
u1.li = 32888后，u1.li才是有效的成员。
联合可以出现在结构类型定义中，结构也可以出现在联合类型定义中。

用typedef重定义类型名
typedef定义的一般形式为：
typedef 原类型名 新类型名;
为了增加程序的可读性，可把整型类型名用typedef定义为：
typedef int INTEGER;
使用：INTEGER a, b;
定义一种新数据类型，专用于某种类型的变量，使程序更清晰明了
定义一.typedef unsigned int size_t;
定义size_t数据类型，专用于内存字节数。
定义二.
typedef struct stu{
    char name[20];
    int age;
    char sex;
} STU, *P_STU;
定义STU表示stu的结构类型，然后可用STU来说明结构变量：
 STU student;
 STU *p;
 STU students[10];
 而P_STU表示指向结构的指针类型。定义结构指针变量可用以下方式：
 P_STU q; // 结构指针变量q
 此时在q前不能再加指针定义符星号(*)。
定义三.
typedef char NAME[20];
表示NAME是字符数组类型，数组长度为20.然后可用NAME说明变量，如
NAME a1, a2, a3;

1 Byte = 8 Bits
1 KB = 1024 Bytes
1 MB = 1024 KB
1 GB = 1024 MB
字 word 
字节 byte 
位 bit 
字长是指字的长度
1字=2字节(1 word = 2 byte) 
1字节=8位(1 byte = 8bit) 
一个字的字长为16 
一个字节的字长是8
bps 是 bits per second 的简称。一般数据机及网络通讯的传输速率
都是以「bps」为单位。如56Kbps、100.0Mbps 等等。 
Bps即是Byte per second 的简称。而电脑一般都以Bps显示速度，
如1Mbps 大约等同 128 KBps。
以前所谓的56kb的modem换算过来56kbps除以8也就是7kbyte，
所以真正从网上下载文件存在硬盘上的速度也就是每秒7kbyte。 
也就是说与传输速度有关的b一般指的是bit。 
与容量有关的b一般指的是byte。
usb2.0 480mbps=60mb/s的传输速率还只是理论值，它还要受到系统环境
的制约（cpu、硬盘和内存等），其实际读、取写入硬盘的速度约
在11～16mb/s。但这也比usb1.1的12mbps(1.5m/s)快了近10倍。
usb2.0标准接口传输速率。许多人都将“480mbps”误解为480兆/秒。其实，这是错误的，事实上“480mbps”应为“480兆比特/秒”或“480兆位/秒”，它等于“60兆字节/秒”，大家看到差距了吧。
这要从bit和byte说起：bit和byte同译为"比特"，都是数据量度单位，
bit=“比特”或“位”。 
byte=字节即1byte=8bits,两者换算是1：8的关系。 
mbps=mega bits per second(兆位/秒)是速率单位，所以正确的说法应该是说usb2.0的传输速度是
480兆位/秒,即480mbps。 
mb=mega bytes(兆比、兆字节)是量单位，
1mb/s（兆字节/秒）=8mbps（兆位/秒）。
我们所说的硬盘容量是40gb、80gb、100gb，这里的b指是的byte也就是
“字节”。 
1 kb = 1024 bytes =2^10 bytes 
1 mb = 1024 kb = 2^20 bytes 
1 gb = 1024 mb = 2^30 bytes

列表list
使用双向链表管理元素
List的元素可以是任意类型T，但必须具备赋值和拷贝能力
List不支持随机存取，不提供下标操作符
在任何位置上指向元素的插入和移除效率高

















































*/