#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <linux/in.h>
//#include <netinet/in.h>

using namespace std;

#define PORT 8888
#define BACKLOG 2

// 对某个套接字的处理
void process_conn_server(int s)
{
    ssize_t size = 0;
    char buffer[1024];

    for(;;)
    {
        // 从套接字中读取数据放到缓冲区buffer中
        size = read(s, buffer, 1024);
        if(size == 0)
        {
            return;
        }

        // 构建响应字符，为接收到客户端字节的数量
        sprintf(
            buffer, 
            "%d bytes altogether\n", 
            (int)size);
        // 发给客户端
        write(s, buffer, strlen(buffer) + 1);
    }
}

void create_socket_server()
{
    int ss, sc;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int err;
    pid_t pid;

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0)
    {
        cout<<"socket error"<<endl;
        return;
    }
    else
    {
        cout<<"socket success"<<endl;
    }

    // 清零
    bzero(&server_addr, sizeof(server_addr));
    // 协议族
    server_addr.sin_family = AF_INET;
    // 本地地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 服务器端口
    server_addr.sin_port = htons(PORT);

    err = bind(
        ss, 
        (struct sockaddr *)&server_addr, 
        sizeof(server_addr));
    if(err < 0)
    {
        cout<<"bind error"<<endl;
        return;
    }
    else
    {
        cout<<"bind success"<<endl;
    }

    err = listen(ss, BACKLOG);
    if(err < 0)
    {
        cout<<"listen error"<<endl;
        return;
    }
    else
    {
        cout<<"listen success"<<endl;
    }

    for(;;)
    {
        socklen_t addrlen = sizeof(struct sockaddr);
        sc = accept(
            ss, 
            (struct sockaddr *)&client_addr, 
            &addrlen);
        if(sc < 0)
        {
            continue;
        }
        else
        {
            cout<<"sc = "<<sc<<endl;
        }

        // 建立一个新进程处理到来的连接
        // 分叉进程
        pid = fork();
        cout<<"pid = "<<pid<<endl;
        if(pid == 0)
        {
            // 当前进程创建子进程成功
            // 在子进程中关闭服务器的侦听
            close(ss);
            // 处理连接
            process_conn_server(sc);
        }
        else if(pid > 0)
        {
            // 在父进程中关闭客户端的连接
            close(sc);
        }
        else
        {
            // 创建子进程失败
        }
    }
}

void process_conn_client(int& s)
{
    ssize_t size = 0;
    char buffer[1024];

    for(;;)
    {
        size = read(0, buffer, 1024);
        // 读到数据
        if(size > 0)
        {
            // 发送给服务器
            write(s, buffer, size);
            // 从服务器读取数据
            size = read(s, buffer, 1024);
            // 写到标准输出
            write(1, buffer, size);
        }
    }
    /***
     使用read()和write()函数时，文件描述符0表示标准
     输入，1表示标准输出，
     可以直接对这些文件描述符进行操作，
     例如读和写。
     */
}

void create_socket_client(const char *msg)
{
    int s;
    int err;
    struct sockaddr_in server_addr;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0)
    {
        cout<<"socket error"<<endl;
        return;
    }
    else
    {
        cout<<"socket success"<<endl;
    }

    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 将用户输入的字符串类型的IP地址转为整型
    inet_pton(AF_INET, msg, &server_addr.sin_addr);

    connect(
        s,
        (struct sockaddr *)&server_addr, 
        sizeof(struct sockaddr));
    process_conn_client(s);
    close(s);
}

void test_pid_fork()
{
    pid_t ppid = getppid();
    pid_t pid = getpid();
    // A进程运行了这个程序（B进程），
    // 那么得到的就是A进程的pid号
    cout<<"ppid = "<<ppid<<endl;
    // 程序（B进程）的pid号
    cout<<"pid = "<<pid<<endl;

    pid_t forkid = fork();
    /***
    正常情况下，fork()会返回两次结果，
    第一次大于0，说明是在B进程中运行；
    第二次等于0，说明创建子进程成功。
    */
    cout<<"forkid = "<<forkid<<endl;
    if(forkid>0)
    {
        // 下面代码还是在B进程中运行的
        pid = getpid();
        cout<<"pid1 = "<<pid<<endl;
    }
    else if(forkid<0)
    {
        // 创建进程失败
        pid = getpid();
        cout<<"pid2 = "<<pid<<endl;
    }
    else
    {
        // 下面代码就是创建子进程成功后，
        // 在子进程中运行的。
        pid = getpid();
        cout<<"pid3 = "<<pid<<endl;
    }
}

/***
#include "sys/type.h"
#include "sys/stat.h"
#include "fcntl.h"
int open(const char* pathname, int flags);
int open(
const char* pathname, 
int flags, 
mode_t mode);
当函数成功时，返回一个整型的文件描述符；
当函数出错时，返回-1.
pathname通常情况下为1024个字节。
flsgs用于设置文件打开后允许的操作方式，
O_RDONLY（0）只读
O_WRONLY（1）只写
O_RDWR（2）读写
O_APPEND追加
O_CREAT如果文件不存在则创建它，当使用此选项时，第三个
参数mode需要同时设定，用来说明新文件的权限。
O_EXCL查看文件是否存在。如果同时指定了O_CREAT，而文件
已经存在，会返回错误。
用这种方法可以安全地打开一个文件。
O_ERUNC将文件长度截断为0.如果此文件存在，并且文件成功
打开，则会将长度截断为0.

*/

void test_socket()
{
//    socket_server_test();

//    char *msg = "hello world";
    // create_socket_client("hello world");
    // cout<<"test_socket()"<<endl;
    test_pid_fork();
    

}

/***
基于TCP的Socket编程的服务器流程：
1.创建套接字（socket)。
2.将套接字绑定到一个本地地址和端口上（bind）。
3.将套接字设为监听模式，准备接收客户请求（listen）。
4.等待客户请求到来；当请求到来后，接受连接请求，
返回一个新的对应于此次连接的套接字（accept）。
5.用返回的套接字和客户端进行通信（send/recv）。
6.返回，等待另一客户的请求。
7.关闭套接字。

ps -ax | grep xinetd

TCP网络编程基础
TCP协议是TCP/IP协议中很重要的一个协议，
由于它传输的稳定性，在很多程序中都在使用。
套接字地址结构
1.通用套接字数据结构
通用的套接字地址类型的定义如下，
它可以在不同协议族之间进行强制转换。
struct sockaddr                 // 套接字地址结构
{
    sa_family_t sa_family;      // 协议族
    char        sa_data[14];    // 协议族数据
}
typedef unsigned short sa_family_t;

2.实际使用的套接字数据结构
在网络程序设计中所使用的函数中几乎所有的套接字函数
都用这个结构作为参数。这个结构的定义如下：
// 以太网套接字地址结构
struct sockaddr_in              
{
    // 无符号字符类型，结构struct sockaddr_in的长度，
    // 为16字节
    u8 sin_len;
    // 无符号字符类型，通常设置为与函数domain一致，
    // 如AF_INET
    u8 sin_family;              
    // 无符号short类型，16位的端口号，网络字节序
    u16 sin_port;               
    // IP地址，为32位，每8位表示IP地址的一个段，
    // 网络字节序
    struct in_addr sin_addr;    
    // 未用，保留
    char sin_zero[8];           
}
结构struct sockaddr_in的成员变量in_addr用于表示IP地址，
这个结构的定义如下：
struct in_addr      // IP地址结构
{
    u32 s_addr;     // 32位IP地址，网络字节序
}

3.结构sockaddr和结构sockaddr_in的关系
结构struct sockaddr和结构struct sockaddr_in
是一个同样大小的结构。
由于结构struct sockaddr和结构struct sockaddr_in
的大小是完全一致的，所以进行地址结构设置时，
通常的方法是利用
结构struct sockaddr_in进行设置，
然后强制转换为结构struct sockaddr类型。
因为这两个结构大小是完全一致的，所以进行
这样的转换不会有什么副作用。

用户层和内核层交互过程
套接字参数中有部分参数是需要用户传入的，
这些参数用来与Linux内核进行通信，如指向地址结构的指针。
通常是采用内存复制的
方法进行。如bind()函数需要传入
地址结构struct sockaddr *my_addr和my_addr指向参数的
长度。
1.向内核传入数据的交互过程
向内核传入数据的函数有send(),bind()等，
从内核得到数据的函数有accept(),recv()等。
bind()函数向内核中传入的参数
有套接字地址结构和结构的长度这样两个参数。
bind()函数的原型为：
int bind(
    // 套接字文件描述符
    int sockfd,                         
    // 套接字地址结构，指向地址结构的指针
    const struct sockaddr *my_addr,     
    // 套接字地址结构的长度
    socklen_t addrlen);                 
调用函数bind()的时候，地址结构通过内存复制的方式
将其中的内容复制到内核，地址结构的长度通过传值的
方式传入内核，
内核按照用户传入的地址结构长度来复制套接字地址结构
的内容。

2.内核传出数据的交互过程
从内核向用户空间传递参数的过程则相反。通过地址结构的
长度和套接字地址结构指针来进行地址结构参数的传出操作。
通常是两个
参数完成传出操作的功能，一个是表示地址结构长度的参数，
一个是表示套接字地址结构地址的指针。
传出过程与传入过程中的参数不同的是，表示地址结构长度
的参数在传入过程中是传值，而在传出过程中是通过值值
完成的。内核按照
用户传入的地址结构长度进行套接字地址结构数据的复制，
将内核中的地址结构数据复制到用户传入的地址结构指针中。
内核付出的数据是通过用户传进去的套接字地址结构反映
出来的，需要的数据可以从这个套接字地址结构中去拿。

TCP网络编程架构
TCP网络编程有两种模式，一种是服务器模式，另一种是
客户端模式。服务器模式创建一个服务程序，等待客户端
用户的连接，
接收到用户的连接请求后，根据用户的请求进行处理；
客户端模式则根据目的服务器的地址和端口进行连接，
向服务器发送请求
并对服务器的响应进行数据处理。
1.服务器端的程序设计模式
TCP连接的服务器模式的程序设计流程分为：
套接字初始化---socket()
套接字与端口的绑定---bind()
设置服务器的侦听连接---listen()
接受客户端连接---accept()
接收和发送数据(read(),write())并进行数据处理入
处理完毕的套接字关闭---close()

2.客户端的程序设计模式
主要分为：
套接字初始化---socket()
连接服务器---connect()
读写网络数据(read(),write())并进行数据处理和最后的
套接字关闭(close())过程
客户端程序设计模式流程与服务器端的处理模式流程类似，
二者的不同之处是客户端在套接字初始化之后可以不进行
地址绑定，而是
直接连接服务器端。
客户端连接服务器的处理过程中，客户端根据用户设置的
服务器地址，端口等参数与特定的服务器程序进行通信。

3.客户端与服务端的交互过程
客户端的连接过程，对服务器端是接收过程，在这个过程
中客户端与服务端进行三次握手，建立TCP连接。
建立TCP连接之后，客户端
与服务端之间可以进行数据的交互。
客户端与服务端之间的数据交互是相对的过程，
客户端的读数据过程对应了服务端的写数据过程，
客户端的写数据过程对应服务端的
读数据过程。
在服务端和客户端之间的数据交互完毕之后，
关闭套接字连接。

创建网络插口函数socket()
网络程序设计中的套接字系统调用socket()函数用来获得
文件描述符。
1.socket()函数介绍
socket()函数的原型如下，如果函数调用成功，会返回一个
表示这个套接字的文件描述符，失败则返回-1.
#include <sys/types.h>
#include <sys/socket.h>
int socket(
    // 协议族为domain,用于设置网络通信的域，
    // 函数socket()根据这个参数选择通信协议的族
    int domain,         
    // 协议类型
    int type,           
    // 协议编号为protocol的套接字文件描述符
    int protocol);      

domain的值及含义
      值                      含义
PF_UNIX,PF_LOCAL        本地通信
PF_INET,AF_INET         IPv4 Internet协议
PF_INET6                IPv6 Internet协议
PF_IPX                  IPX-Novell协议
PF_NETLINK              内核用户界面设备
PF_X25                  ITU-T X.25 / ISO-8208协议
PF_AX25                 Amateur radio AX.25协议
PF_ATMPVC               原始ATM PVC访问
PF_APPLETALK            Appletalk
PF_PACKET               底层包访问

type的值及含义
      值                      含义
SOCK_STREAM             流式套接字。TCP连接，
                        提供序列化的，可靠的，
                        双向连接的字节流。
                        支持带外数据传输。
SOCK_DGRAM              数据包套接字。
                        支持UDP连接
                        （无连接状态的消息）。
SOCK_SEQPACKET          序列化包，提供一个序列化的，
                        可靠的，双向的基于连接的数据
                        传输通道，数据长度定常。
                        每次调用读系统调用时数据需要
                        将全部数据读出。
SOCK_RAM                RAW类型，提供原始网络协议访问。
SOCK_RDM                提供可靠的数据报文，不过可能
                        数据会有乱序。
SOCK_PACKET             专用类型，不能在通用程序中
                        使用。专用数据 包，直接从
                        设备驱动接收数据。

并不是所有的协议族都实现了这些协议类型，
如AF_INET协议族就没有实现SOCK_SEQPACKET协议类型。

参数三protocol用于指定某个协议的特定类型，
即type类型中的某个类型。通常某个协议中只有
一种特定类型，这样protocol
参数仅能设置为0；但是有些协议有多种特定的类型，
就需要设置这个参数来选择特定的类型。

函数socket()并不总是执行成功，有可能会出现错误，
错误的产生有多种原因，可以通过errno获得。通常
情况下造成
函数socket()失败的原因是输入的参数错误造成的，
如某个协议不存在等，这时需要详细检查函数的输入参数。
由于函数的调用不一定成功，在进行程序设计的时候，
一定要检查返回值。
errno的值及含义
      值                      含义
EXCCES                  没有权限建立指定domain的
                        type的socket
EAFNOSUPPORT            不支持所给的地址类型
EINVAL                  不支持此协议或者协议不可用
EMFILE                  进程文件表溢出
ENFILE                  已经达到系统允许打开的文件
                        数量，打开文件过多
ENOBUFS/ENOMEM          内存不足。socket只有到资源
                        足够或者有进程释放内存
EPROTONOSUPPORT         指定的协议type在domain中
                        不存在
其他

使用socket()函数的时候需要设置上述3个参数，
如将socket()函数的第1个参数domain设置为AF_INET，
第2个参数设置为
SOCK_STREAM，第3个参数设置为0,建立一个流式套接字。
int sock = socket(AF_INET, SOCK_STREAM, 0);

2.应用层函数socket()和内核函数之间的关系
用户设置套接字的参数后，函数要能够起作用，
需要与内核空间的相关系统调用交互。
应用层的socket()函数是和内核层的系统调用相对应的。

绑定一个地址端口对bind()
在建立套接字文件描述符成功后，需要对套接字进行
地址和端口的绑定，才能进行数据的接收和发送操作。
bind()函数介绍
bind()函数将长度为addlen的struct sockadd类型的
参数my_addr与sockfd绑定在一起，将sockfd绑定到
某个端口上，如果
使用connect()函数则没有绑定的必要。
绑定的函数原型如下：
#include <sys/types.h>
#include <sys/socket.h>
int bind(
int sockfd, 
const struct sockaddr *my_addr, 
socklen_t addrlen);
参数一：用socket()函数创建的文件描述符。
参数二：指向一个结构为sockaddr参数的指针，
sockaddr中包含了地址，端口和IP地址的信息。
在进行地址绑定的时候，需要先
将地址结构中的IP地址，端口，类型等
结构struct sockaddr中的域进行设置后才能进行绑定，
这样绑定后才能将套接字文件描述符
与地址等结合在一起。
参数三：是参数二这个结构的长度，可以设置成
sizeof(struct sockaddr)。使用sizeof(struct sockaddr)
来设置addlen
是一具良好的习惯，虽然一般情况下使用AF_INET
来设置套接字的类型和其对应的结构，但是不同类型的
套接字有不同的地址
描述符结构，如果对地址长度进行了强制的指定，
可能会造成不可预料的结果。
bind()函数的返回值为0时表示绑定成功，
-1时表示绑定失败，errno的错误值如下。
      值                      含义
EADDRINUSE             给定地址已经使用
EBADF                  sockfd不合法
EINVAL                 sockfd已经绑定到其他地址
ENOTSOCK               sockfd是一个文件描述符，
                       不是socket描述符
EACCES                 地址被保护，用户的权限不足
EADDRNOTAVAIL          接口不存在或者绑定地址不是
                       本地 UNIX协议族，AF_UNIX
EFAULT                 my_addr指针超出用户空间 
                       UNIX协议族，AF_UNIX
EINVAL                 地址长度错误，或者socket
                       不是AF_UNIX族 UNIX协议族，
                       AF_UNIX
ELOOP                  解析my_addr是符号链接过多 
                       UNIX协议族，AF_UNIX
ENAMETOOLONG           my_addr过长 UNIX协议族，
                       AF_UNIX
ENOENT                 文件不存在 UNIX协议族，AF_UNIX
ENOMEM                 内存内核不足 UNIX协议族，
                       AF_UNIX
ENOTDIR                不是目录 UNIX协议族，AF_UNIX
EROFS                  socket节点应该在只读文件
                       系统上 UNIX协议族，AF_UNIX

监听本地端口listen
函数listen()用来初始化服务器可连接队列，
服务器处理客户端连接请求的时候是顺序处理的，
同一时间仅能处理一个客户端连接。
当多个客户端的连接请求同时到来的时候，
服务器并不是同时处理，而是将不能处理的客户端
连接请求放到等待队列中，这个队列的
长度由listen()函数来定义。
1.listen()函数介绍
listen()函数的原型如下，其中的backlog表示等待队列的
长度。
#include <sys/socket.h>
int listen(int sockfd, int backlog);
当listen()函数成功运行时，返回值为0；当运行失败时，
返回值为-1,并且设置errno值。
      值                      含义
EADDRINUSE              另一个socket已经在同一端口侦听
EBADF                   参数sockfd不是合法的描述符
ENOTSOCK                参数sockfd不是代表socket的
                        文件描述符
EOPNOTSUPP              socket不支持listen操作
在接受一个连接之前，需要用listen()函数来侦听端口，
listen()函数中参数backlog的参数表示在accept()函数
处理之前
在等待队列中的客户端的长度，如果超过这个长度，
客户端返回一个EXONNREFUSED错误。
listen()函数仅对类型为SOCK_STREAM或者SOCK_SEQPACKET
的协议有效。例如，如果对一个SOCK_DGRAM的协议使用
listen()，将会出现错误errno的值为EOPNOTSUPP，
表示此socket不支持listen()操作。大多数系统的
设置为20,可以
将其设置修改为5或者10,根据系统可承受负载或者
应用程序的需求来确定。

接受一个网络请求accept()
当一个客户端的连接请求到达服务器主机侦听的端口时，
此时客户端的连接会在队列中等待，直到使用服务器
处理接收请求。
函数accept()成功执行后，会返回一个新的套接口文件
描述符来表示客户端的连接，客户端连接的信息可以
通过这个新描述符
来获得。因此当服务器成功处理客户端的请求连接后，
会有两个文件描述符，老的文件描述符表示正在监听的
socket，新产生的
文件描述符表示客户端的连接，函数send()和rece()
通过新的文件描述符进行数据收发。
1.accept()函数介绍
accept()函数的原型如下：
#include <sys/types.h>
#include <sys/socket.h>
int accept(
int sockfd, 
struct sockaddr *addr, 
socklen_t *addrlen);
通过accept()函数可以得到成功连接客户端的IP地址，
端口和协议族等信息，这个信息是通过参数addr获得的。
当accept()
返回的时候，会将客户端的信息存储在参数addr中。
参数addrlen表示参数二(addr)所指内容的长度，可以使用
sizeof(struct sockaddr_in)来获得。需要注意的
是在accept中addrlen参数是一个指针而不是结构，
accept()将这个
指针传给TCP/IP协议栈。
accept()函数的返回值是新连接的客户端套接字文件描述符，
与客户端之间的通信是通过accept()返回的新套接字
文件描述符
来进行的，而不是通过建立套接字时的文件描述符，
这是在程序设计的时候需要注意的地方。
如果accept()函数发生错误，accept()
会返回-1.通过errno可以得到错误值。
      值                      含义
EAGAIN/EWOULDBLOCK      此socket使用了非阻塞模式，
                        当前情况下没有可接受的连接
EBADF                   描述符非法
ECONNABORTED            连接取消
EINTR                   信号在合法连接到来之前
                        打断了accept的系统调用
EINVAL                  socket没有侦听连接或者地址
                        长度不合法
EMFILE                  每个进程允许打开的文件描述符
                        数量最大值已经到达
EMFILE                  达到系统允许打开文件的总数量
ENOTSOCK                文件描述符是一个文件，
                        不是socket
EOPNOTSUPP              引用的socket不是流类型
                        SOCK_STREAM
EFAULT                  参数addr不可写
ENOBUFS/ENOMEM          内存不足
EPROTO                  协议错误
EPERM                   防火墙不允许连接

连接目标网络服务器connect()
客户端在建立套接字之后，不需要进行地址绑定，
就可以直接连接服务器。连接服务器的函数为connect()，
此函数连接指定参数的
服务器，如IP地址，端口等。
1.connect()函数介绍
#include <sys/types.h>
#include <sys/socket.h>
int connect(
    // 建立套接字时返回的套接字文件描述符，
    // 它是由系统调用socket()返回的
    int sockfd,                     
    // 客户端需要连接的服务器的目的端口和IP地址
    // 以及协议类型
    struct sockaddr *serv_addr,     
    // serv_addr内容的大小，可以使用
    // sizeof(struct sockaddr)而获得，
    // 与bind()不同，这个参数是一个整形的变量
    // 而不是指针
    int addrlen);                   
connect()函数的返回值在成功时为0,
当发生错误的时候返回-1,可以查看errno获得错误的原因。
      值                      含义
EACCES                  在AF_UNIX族协议中，使用路径名
                        作为标识。EACCES表示目录
                        不可写或者不可访问
EACCES/EPERM            用户没有设置广播标志而连接
                        广播地址或者连接请求被防火墙
                        限制
EADDRINUSE              本地地址已经在使用
EAFNOSUPPORT            参数serv_addr的域sa_family
                        不正确
EAGAIN                  本地端口不足
EALREADY                socket是非阻塞类型并且前面的
                        连接没有返回
EBADF                   文件描述符不是合法的值
ECONNREFUSED            连接的主机地址没有侦听
EFAULT                  socket结构地址超出用户空间
EINPROGRESS             socket是非阻塞模式，而连接
                        不能立刻返回
EINTR                   函数被信号中断
EISCONN                 socket已经连接
ENETUNREACH             网络不可达
ENOTSOCK                文件描述符不是一个socket
ETIMEDOUT               连接超时

写入数据函数write()
当服务器端在接收到一个客户端的连接后，
可以通过套接字描述符进行数据的写入操作。
对套接字进行写入的形式和过程与普通文件
的操作方式一致，内核会根据文件描述符的
值来查找所对应的属性，当为套接字的时候，
会调用相对应的内核函数。
int size;
char data[1024];
size = write(s, data, 1024);

读取数据函数read()
与写入数据类似，使用read()函数可以从套接字描述符中
读取数据。当然在读取数据之前，必须建立套接字并连接。
读取数据的方式如下所示，从套接字描述符s中读取1024个
字节，放入缓冲区data中，size变量的值为成功读取的
数据的大小。
int size;
char data[1024];
size = read(s, data, 1024);

关闭套接字函数close()
关闭socket连接可以使用close()函数实现，
函数的作用是关闭已经打开的socket连接，
内核会释放相关的资源，关闭套接字之后
就不能再使用这个套接字文件描述符进行读写操作了。

函数shutdown()可以使用更多方式来关闭连接，
允许单方向切断通信或者切断双方的通信。函数原型如下，
第一个参数s是切断通信
的套接字文件描述符，第二个参数how表示切断的方式。
#include <sys/socket.h>
int shutdown(int s, int how);
函数shutdown()用于关闭双向连接的一部分，具体的
关闭行为方式通过参数的how设置来实现。可以为如下值：
SHUT_RD: 值为0,表示切断读，
之后不能使用此文件描述符进行读操作。
SHUT_WR: 值为1,表示切断写，
之后不能使用此文件描述符进行写操作。
SHUT_RDWR: 值为2,表示切断读写，
之后不能使用此文件描述符进行读写操作，
与close()函数功能相同。
函数shutdown()如果调用成功则返回0,
如果失败则返回-1，通过errno可以获得错误的具体信息。
      值                      含义
EBADF                   文件描述符不是合法的值
ENOTCONN                socket没有连接
ENOTSOCK                s是一个文件，不是socket

截取信号的例子
在Linux操作系统中当某些状况发生变化时，
系统会向相关的进程发送信号。信号的处理方式
是系统会先调用进程中注册的处理
函数 ，然后调用系统默认的响应方式，
包括终止进程，因此在系统结束进程前，
注册信号处理函数进行一些处理是一个完善程序
的必须条件。












































 */
