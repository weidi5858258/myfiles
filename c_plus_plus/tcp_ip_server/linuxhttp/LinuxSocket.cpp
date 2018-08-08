#include "LinuxSocket.h"

void test_open(void);

void test_read(void);

void test_write(void);

void test_lseek(void);

void test_create_server_socket(void);

void test_pthread(void);

void test_class(void);

void test_sqlite3(void);

void LinuxSocket::studyHard() {

    test_class();

}

void test_open(void) {
    int fd = -1;
    char filename[] = "test.txt";
    // char filename[] = "/dev/sda1";
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        printf("Open file failure\n");
    } else {
        printf("Open file success 1\n");
    }

    close(fd);

    // 打开文件，如果文件存在则返回-1
    fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    if (fd == -1) {
        printf("File exist, reopen it\n");
        // 重新打开
        fd = open(filename, O_RDWR);
        // MyLog::i(TAG, to_string(fd), NEEDLOG);
    } else {
        // 文件不存在，成功创建并打开
        printf("Open file success 2\n");
    }

    close(fd);

    int j = 0;
    for (j = 0; fd >= 0; ++j) {
        fd = open(filename, O_RDONLY);
        sleep(1);
        if (fd > 0) {
            printf("%d\n", fd);
        } else {
            printf("error, can't open file\n");
            // exit(0);
        }
        // 如果打开的文件即时被关闭了，那么这个文件描述符可以重复使用
        close(fd);
    }
}

void test_read(void) {
    int fd = -1, i;
    ssize_t size = -1;
    char buf[10];
    char filename[] = "test.txt";

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Open file %s failure, fd:%d\n", filename, fd);
    } else {
        printf("Open file %s success, fd:%d\n", filename, fd);
    }
    while (size) {
        size = read(fd, buf, 10);
        // 读取数据时出错
        if (size == -1) {
            printf("read file error occurs\n");
            break;
        } else {
            if (size > 0) {
                printf("read %d bytes: ", size);
                // printf("\"");
                for (i = 0; i < size; ++i) {
                    printf("%c", *(buf + i));
                }
                printf("\n");
            } else {
                printf("read %d bytes\n", size);
                printf("reach the end of file\n");
            }
        }
    }
    close(fd);
}

void test_write(void) {
    int fd = -1, i;
    ssize_t size = -1;
    int input = 0;
    char buf[] = "quick brown fox jumps over the lazy dog 我是谁 ";
    char filename[] = "test.txt";
    fd = open(filename, O_RDWR | O_TRUNC);
    if (fd == -1) {
        printf("Open file %s failure, fd: %d\n", filename, fd);
    } else {
        printf("Open file %s success, fd: %d\n", filename, fd);
    }
    size = write(fd, buf, strlen(buf));
    printf("write %d bytes to file %s\n", size, filename);
    close(fd);
}

void test_lseek(void) {
    off_t offset = -1;
    // 第一个参数0代表标准输入
    offset = lseek(0, 0, SEEK_CUR);
    if (offset == -1) {
        printf("STDIN can't seek\n");
    } else {
        printf("STDIN can seek\n");
    }

    int fd = -1, i;
    ssize_t size = -1;
    char buf1[] = "01234567";
    char buf2[] = "ABCDEFGH";
    char filename[] = "test.txt";
    int len = 8;
    fd = open(filename, O_RDWR | O_TRUNC, S_IRWXU);
    if (fd == -1) {
        printf("Open file %s failure, fd: %d\n", filename, fd);
        return;
    }
    size = write(fd, buf1, len);
    if (size != len) {
        printf("write %d bytes to file %s\n", size, filename);
        return;
    }
    offset = lseek(fd, 32, SEEK_SET);
    if (offset == -1) {
        return;
    }
    size = write(fd, buf2, len);
    if (size != len) {
        printf("write %d bytes to file %s\n", size, filename);
        return;
    }
    close(fd);
}

void process_conn_server(int);

void test_create_server_socket(void) {
    // ss为服务器端的socket描述符，sc为客户端的socket描述符
    int ss, sc;
    // 服务器端地址结构
    struct sockaddr_in server_addr;
    // 客户端地址结构
    struct sockaddr_in client_addr;
    // 返回值
    int err;
    // 分叉的进行ID
    pid_t pid;
    // 建立一个流式套接字
    ss = socket(AF_INET, SOCK_STREAM, 0);
    // 出错
    if (ss < 0) {
        printf("socket error\n");
        return;
    }

    // 设置服务器端地址
    // 清零
    bzero(reinterpret_cast<char *>(&server_addr), sizeof(server_addr));
    // 协议族
    server_addr.sin_family = AF_INET;
    // 本地地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 服务器端口
    server_addr.sin_port = htons(PORT);

    // 绑定地址结构到套接字描述符
    err = bind(ss, (struct sockaddr *) &server_addr, sizeof(server_addr));
    // 出错
    if (err < 0) {
        printf("bind error\n");
        return;
    }

    // 设置侦听
    err = listen(ss, BACKLOG);
    // 出错
    if (err < 0) {
        printf("listen error\n");
        return;
    }

    // 主循环过程
    for (;;) {
        socklen_t addrlen = sizeof(struct sockaddr);
        // 接收客户端连接
        sc = accept(ss, (struct sockaddr *) &client_addr, &addrlen);
        // 出错
        if (sc < 0) {
            continue;
        } else {
            printf("server sc: %p\n", sc);
        }
        // 建立一个新的进程处理到来的连接
        pid = fork();
        if (pid == 0) {
            // 子进程
            close(ss);
            process_conn_server(sc);
        } else {
            // 父进程
            close(sc);
        }
    }
}

// 服务器端对客户端的处理
void process_conn_server(int sc) {
    ssize_t size = 0;
    // 数据的缓冲区
    char buffer[1024];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        size = read(sc, buffer, 1024);
        // 没有数据
        if (size == 0) {
            return;
        } else {
            // 输出读到的内容，这个函数的作用就是写给谁，这个谁就是“1”
            // “1”代表标准输出
            write(1, buffer, size);
        }
        // 构建响应字符，为接收到客户端字节的数量
        sprintf(buffer, "%d bytes altogether\n", size);
        // 发给客户端
        write(sc, buffer, strlen(buffer) + 1);
    }
}

/***
基础知识：

    源文件：C、C++
    目标文件：经过编译器的编译生成的CPU可识别的二进制代码。
    可执行文件：目标文件与相关的库链接后生成的文件。
    预编译过程将程序中引用的头文件包含进源代码中，
并对一些宏进行替换。
    编译过程将用户可识别的语言翻译成一组处理器可识别的操作友，
生成目标文件，通常翻译成汇编语言，而汇编语言通常和机器操作码
之间是一种一对一的关系。
    链接：
    编译链接：gcc hello.c 默认生成a.out文件，不生成目标文件
（目标文件作为中间文件，在生成可执行文件后，会被删除）
    运行： ./a.out
    生成指定的可执行文件名：gcc -o test hello.c
    运行： ./test
    生成目标文件：gcc -c hello.c 生成目标文件hello.o
    生成指定的目标文件：gcc -c -o test.o hello.c
    生成多目标文件：gcc -c file1.c file2.c file3.c
    多文件编译：gcc -o test string.c main.c
    运行： ./test
    分步进行：
    1、gcc -c string.c main.c
    2、gcc -o test string.o main.o
    选项-E告诉编译器进行预编译操作，并将结果显示在计算机屏幕上：
    gcc -E string.c
    预编译生成中间文件：
    gcc -o string.i -E string.c
    编译生成汇编语言：gcc -S string.c 生成string.s
    静态库（***.a，由程序ar生成）
    静态库优点：可以在不用重新编译程序库代码的情况下，
进行程序的重新链接，这种方法节省了编译过程的时间。
静态库的另一个优势是开发者可以提供库文件给使用的人员，
不用开放源代码，这是库函数提供者经常采用的手段。
创建静态库的最基本步骤是先生成目标文件，然后使用
工具ar对目标文件进行归档。
    生成库文件：ar -rcs libstr.a string.o
    GCC链接时使用库函数和一般的obj文件的形式是一致的，如：
gcc -o test main.c libstr.a
    使用“-l库名”，库名是不包含函数库和扩展名的字符串
gcc -o test main.c -lstr（可能提示找不到库文件str）
gcc -o test main.c -L./ -lstr
    在使用-l选项时，-o选项的目的名称要在-l链接的库名称之前，
否则gcc会认为-l是生成的目标而出错。

    生成动态链接库（使用-fPIC选项或者-fpic选项）
    使用-fPIC选项或者-fpic选项的作用是使得gcc生成的代码是位置无关的。
    gcc -shared -Wl,-soname,libstr.so -o libstr.so.1 string.c
    其中选项“-soname,libstr.so”表示生成动态库时的别名是libstr.so；
    “-o libstr.so.1”选项表示是生成名字为libstr.so.1的实际动态链接库文件；
    -shared告诉编译器生成一个动态链接库。
    生成动态链接库之后一个很重要的问题就是安装，
一般情况下将生成的动态链接库复制到系统默认的动态
链接库的搜索路径下，通常有/lib、/usr/lib、/usr/local/lib，
放到之上任何一个目录下都可以。
    动态链接库的配置
    系统中的配置文件/etc/ld.so.conf是动态链接库的
搜索路径配置文件。在这个文件内，存放着可被Linux共享
的动态链接库所在的目录的名字（系统目录/lib、/usr/lib除外），
多个目录名间以空白字符（空格、换行等）或冒号或逗号分隔。
    动态链接库管理命令
    看Linux网络编程第27页
    使用动态链接库
    在编译程序时，使用动态链接库和静态链接库是一致的，
使用“-l 库名”的方式，在生成可执行文件的时候会链接库
文件。如下面的命令将源文件main.c编译成可执行文件test，
并链接库文件libstr.a或者libstr.so：
    gcc -o test main.c -L./ -lstr
    -L指定链接动态链接库的路径，-lstr链接库函数str。
程序编译时链接动态链接库和运行时使用动态链接库的概念
是不同的，在运行时，程序链接的动态链接库需要在系统目录下
才行。有几种办法可以解决此种问题：
    1）将动态链接库的目录放到程序搜索路径中，可以将库的
路径回到环境变量LD_LIBRARY_PATH中实现。
    export LD_LIBRARY_PATH=/example/ex02: $LD_LIBRARY_PATH
    2）使用ld-Linux.so.2来加载程序，命令格式为：
    /lib/ld-Linux.so.2 --library-path 路径 程序名
    加载test程序的命令为：
    /lib/ld-Linux.so.2 --library-path /example/ex02 test
    如果系统的搜索路径下同时存在静态链接库和动态链接库，
默认情况下会链接动态链接库。如果需要强制链接静态链接库，
需要加上“-static”选项，即编译方法为：
    gcc -o testdl main.c -static -ldl
    动态加载库
    动态加载库和一般的动态链接库所不同的是，一般动态链接库
在程序启动的时候就要寻找动态库，找到库函数；而动态加载库可以
用程序的方法来控制会时候加载。动态加载库主要有函数dlopen()、
dlerror()、dlsym()和dlclose()来控制动态库的使用。
    1.打开动态库dlopen()
    void * dlopen(const char *filename, int flag);
    如
    void *phandle = dlopen("./libstr.so", RTLD_LAZY);
    2.获得函数指针dlsym()
    使用动态链接库的目的是调用其中的函数，完成特定的功能。
函数dlsym()可以获得动态链接库中指定函数的指针，然后可以使用
这个函数指针进行操作。函数dlsym()的原型如下，其中参数handle为
dlopen打开动态库后返回的句柄，参数symbol为函数的名称，返回值为
函数指针。
    void * dlsym(void *handle, char *symbol);

    gcc常用选项
    看Linux网络编程第31页

    文件描述符
    在Linux下用文件描述符来表示设备文件和普通文件。
文件描述符是一个整型的数据，所有对文件的操作都通过
文件描述符实现。文件描述符是文件系统中连接用户空间
和内核空间的枢纽。当打开一个或者创建一个文件时，内核
空间创建相应的结构，并生成一个整型的变量传递给用户空间
的对应进程。进程用这个文件描述符来对文件进行操作。用户
空间的文件操作，例如读写一个文件时，将文件描述符作为参数
传送给read或write。读写函数的系统调用到达内核时，内核解析
作为文件描述符的整型变量，找出对应的设备文件，运行相应的函数，
返回用户空间结果。文件描述符的范围是0~OPEN_MAX，因此是一个
有限的资源，在使用完毕后要及时释放，通常是调用close()函数
关闭。文件描述符的值公在同一个进程中有效，即不同进程的
文件描述符，同一个值很有可能描述的不是同一个设备或者普通文件。
在Linux系统中有3个已经分配的文件描述符，即标准输入、
标准输出和标准错误，它们的值分别为0、1和2。可以查看/dev下的
stdin（标准输入）、stdout（标准输出）和stderr（标准错误），
会发现分别指向了/proc/self/fd目录下的0、1、2文件。

    打开创建文件open()、create()函数
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    int open(const char *pathname, int flags);
    int open(const char *pathname, int flags, mode_t mode);
    open()函数打开pathname指定的文件，当函数成功时，
返回一个整型的文件描述符。这个函数正常情况下会返回一个
文件描述符的值，在出错的时候会返回-1。
pathname：
在通常情况下为1024个字节。
flags:
O_RDONLY：只读，定义为0。
O_WRONLY：只写，定义为1。
O_RDWR：读写，定义为2。
O_APPEND：使每次对文件进行写操作时都追加到文件的尾端。
O_CREAT：如果文件不存在则创建它，当使用此选项时，第三个
参数mode需要同时设定，用来说明新文件的权限。
O_EXCL：查看文件是否存在。如果同时指定了O_CREAT，而文件已经
存在则会返回-1。用这种方法可以安全地打开一个文件。
O_TRUNC：将文件长度截断为0.如果此文件存在，并且文件成功打开，
则会将其长度截断为0。通常用作清空操作。
O_NONBLOCK：打开文件为非阻塞方式，如果不指定此项，默认的打开
方式为阻塞方式，即对文件的读写操作需要等待操作的返回状态。
mode：
用于表示打开文件的权限，mode的使用性友结合flags的O_CREAT一起
使用，否则是无效的。
S_IRWXU 00700 用户（文件所有者）有读写和执行的权限
S_IRUSR 00400 用户对文件有读权限
S_IWUSR 00200 写
S_IXUSR 00100 执行
S_IRWXG 00070 组用户（文件所有者）有读写和执行的权限
S_IRGRP 00040 读
S_IWGRP 00020 写
S_IXGRP 00010 执行
S_IRWXO 00007 其他用户（文件所有者）有读写和执行的权限
S_IROTH 00004 读着
S_IWOTH 00002 写
S_IXOTH 00001 执行
在Linux下如果之前没有其他文件打开过，第一个调用打开
文件成功的程序，返回的描述符为最低值，即3.
open()函数不仅可以打开一般的文件，而且可以打开设备文件。
如打开设备文件“/dev/sda1”，即磁盘的第一个分区。
创建文件的函数除了可以在打开时创建外，还可以使用create()
函数创建一个新文件，其函数原型为：
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    int create(const char *pathname, mode_t mode);
create()等于一个open的缩写版本，等效于：
    open(pathname, O_WRONLY|O_CREAT|O_TRUNC, mode);
create的返回值与open一样，在成功时为创建文件的描述符。

    关闭文件close()函数
    当函数执行成功时返回0，如果有错误发生，如文件描述符
非法，返回-1。在使用这个函数的时候，通常不检查返回值。
    #include <unistd.h>
    int close(int fd);
    在打开文件之后，必须关闭文件。如果一个进程中没有正常关闭文件，
在进程退出时系统会自动关闭打开的文件。但是打开一个文件的时候，
系统分配的文件描述符为当前进程中最小的文件描述符的值，
这个值一般情况下是递增的，而每个进程中的文件描述符的数量是有
大小限制的。如果一个进程中频繁地打开文件而又忘记关闭文件，当
系统的文件描述符达到最大限制的时候，就会因为没有文件描述符
可以分配造成打开文件失败。

    读取文件read()函数
    #include <unistd.h>
    ssize_t read(int fd, void *buf, size_t count);
    read()函数从文件描述符fd对应的文件中读取count字节，
放到buf开始的缓冲区。如果count的值为0，read()函数返回0，
不进行其他操作；如果count的值大于SSIZE_MAX，结果不可预料。
在读取成功的时候，文件对应的读取位置指针，向后移动位置，
大小为成功读取的字节数。
如果read()函数执行成功，返回读取的字节数；当返回值为-1时，
读取函数有错误发生。如果已经到达文件的末尾，返回0。
返回值的数据类型为ssize_t，这是一个可能不同于int、long
类型的数据类型，它是一个符号数，具体实现时可能定义为
int或者long。参数buf是一个指针，它指向缓冲区地址的开始
位置，读入的数据将保存在这个缓冲区中；参数count表示要
读取的字节数，通常用这个变量来表示缓冲区的大小，因此count
的值不要超过缓冲区的大小，否则很容易造成缓冲区的溢出。

    写文件write()函数
    #include <unistd.h>
    ssize_t write(int fd, const void *buf, size_t count);
    write()函数向文件描述符fd写入数据，数据的大小由count
指定，buf为要写入数据的指针，write()函数返回值为成功写入数据的
字节数。当操作的对象是普通文件时，写文件的位置从文件的当前
开始，操作成功后，写的位置会增加写入字节数的值。如果在打开文件
的时候指定了O_APPEND项，每次写操作之前，会将写操作的位置移到文件
的结尾处。函数操作成功会返回写入的字节数，当出错时返回-1。
写操作函数并不能保证将数据成功地写入磁盘，这在异步操作中
经常出现，write()函数通常将数据写入缓冲区，在合适的时机由系统
写入实际的设备。可以调用fsync()函数，显式将输入写入设备。

    文件偏移lseek()函数
    每次打开和对文件进行读写操作后，文件的偏移量都进行了更新。
当写入数据成功时，文件的偏移量要向后移动写入数据的大小。当从
文件中读出数据的时候，文件的偏移量要向后移动读出数据的大小。
文件的偏移量是一个非负整数，表示从文件的开始到当前位置的
字节数。一般情况下，对文件的读写操作都从当前的文件位移量
开始，并增加读写操作成功的字节数。当打开一个文件时，如果
没有指定O_APPEND选项，文件的位移量为0。如果指定了O_APPEND选项，
文件的偏移量与文件的长度相等，即文件的当前操作位置移到了末尾。
    #include <sys/types.h>
    #include <unistd.h>
    off_t lseek(int fd, off_t offset, int whence);
    这个函数对文件描述符fd所代表的文件，按照操作模式whence和
偏移的大小offset，重新设定文件的偏移量。如果lseek()函数操作
成功，则返回新的文件偏移量的值；如果失败返回-1。由于文件的
偏移量可以为负值，判断lseek()是否操作成功时，不要使用小于0
的判断，要使用是否等于-1来判断函数失败。
    offset值的含义如下：
    如果whence为SEEK_SET，则offset为相对文件开始处的值，
即将该文件偏移量设为距文件开始处offset个字节。
    如果whence为SEEK_CUR，则offset为相对当前位置的值，
即将该文件的偏移量设置为其当前值加offset。
    如果whence为SEEK_END，则offset为相对文件结尾的值，
即将该文件的偏移量设置为文件长度加offset。
    lseek()函数执行成功时返回文件的偏移量，可以用SEEK_CUR
模式下偏移0的方式获得当前的偏移量，如：
    off_t cur_pos = lseek(fd, 0, SEEK_CUR);

    TCP网络编程基础
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

    TCP协议是TCP/IP协议中很重要的一个协议，
由于它传输的稳定性，在很多程序中都在使用。
套接字地址结构
    1.通用套接字数据结构
    通用的套接字地址类型的定义如下，
它可以在不同协议族之间进行强制转换。
typedef unsigned short sa_family_t;
struct sockaddr                 // 套接字地址结构
{
    sa_family_t sa_family;      // 协议族
    char        sa_data[14];    // 协议族数据
}

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
地址结构struct sockaddr *my_addr和
my_addr指向参数的长度。
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
方式传入内核，内核按照用户传入的地址结构长度来
复制套接字地址结构的内容。

    2.内核传出数据的交互过程
    从内核向用户空间传递参数的过程则相反。通过地址结构的
长度和套接字地址结构指针来进行地址结构参数的传出操作。
通常是两个参数完成传出操作的功能，一个是表示地址结构
长度的参数，一个是表示套接字地址结构地址的指针。
传出过程与传入过程中的参数不同的是，表示地址结构长度
的参数在传入过程中是传值，而在传出过程中是通过值值
完成的。内核按照用户传入的地址结构长度进行套接字
地址结构数据的复制，将内核中的地址结构数据复制到
用户传入的地址结构指针中。内核付出的数据是通过
用户传进去的套接字地址结构反映出来的，需要的
数据可以从这个套接字地址结构中去拿。

    TCP网络编程架构
    TCP网络编程有两种模式，
一种是服务器模式，
另一种是客户端模式。
服务器模式创建一个服务程序，等待客户端用户的连接，
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
接收和发送数据并进行数据处理---read(),write()
关闭套接字---close()

    2.客户端的程序设计模式
    主要分为：
套接字初始化---socket()
连接服务器---connect()
读写网络数据并进行数据处理---read(),write()
关闭套接字---close()
客户端程序设计模式流程与服务器端的处理模式流程类似，
二者的不同之处是客户端在套接字初始化之后可以不进行
地址绑定，而是
直接连接服务器端。
客户端连接服务器的处理过程中，客户端根据用户设置的
服务器地址，端口等参数与特定的服务器程序进行通信。

    3.客户端与服务端的交互过程
    客户端的连接过程，对服务器端是接收过程，在这个过程
中客户端与服务器端需要进行三次握手，建立TCP连接。
建立TCP连接之后，客户端
与服务器端之间就可以进行数据的交互。
客户端与服务器端之间的数据交互是相对的过程，
客户端的读数据过程对应服务器端的写数据过程，
客户端的写数据过程对应服务器端的读数据过程。
在服务器端和客户端之间的数据交互完毕之后，
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


数据的IO和复用
Linux系统 中的IO函数主要有
read(),write(),recv(),send(),
recvmsg(),sendmsg(),readv(),writev().

使用recv()函数接收数据
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int s, void *buf, size_t len, int flags);
recv()函数从套接字s中接收数据放到缓冲区buf中,buf的长度
为len,以字节为单位,操作的方式由flags指定.


函数recv()的返回值是成功接收到的字节数,当返回值为-1时发生错误,
可以查看errno获取错误码.

recv()函数通常用于TCP类型的套接字,UDP使用recvfrom()函数接收
数据,当然在数据报套接字绑定地址和端口后,也可以使用recv()函数
接收数据.recv()函数从内核的接收缓冲区中复制到用户指定的缓冲区,
当内核中的数据比指定的缓冲区小时,一般情况下(没有采用
MSG_WAITALL标志)会复制缓冲区中的所有数据到用户缓冲区,并返回
数据的长度.当内核接收缓冲区中的数据比用户指定的多时,会将用户
指定长度len的接收缓冲区中的数据复制到用户指定地址,其余的数据需要
下次调用接收函数的时候再复制,内核在复制用户指定的数据之后,会
销毁已经复制完毕的数据,并进行调整.

使用send()函数发送数据
#include <sys/types.h>
#include <sys/socket.h>
ssize_t send(int s, const void *buf, size_t len, int flags);
send()函数将缓冲区buf中大小为len的数据,通过套接字文件描述符按照
flags指定的方式发送出去.其中的参数含义与recv中的含义一致,它的返回值
是成功发送的字节数.由于用户缓冲区buf中的数据在通过send()函数进行
发送的时候,并不一定能够全部发送出去,所以要检查send()函数的返回值,
按照与计划发送的字节长度len是否相等来判断如何进行下一步操作.
当send()函数的返回值小于len时,表明缓冲区中仍然有部分数据没有
成功发送,这时需要重新发送剩余部分的数据.通常的剩余数据发送方法
是对原来buf中的数据位置进行偏移,偏移的大小为已发送成功的字节数.
函数send()发生错误时返回-1,这时可以查看errno获取错误码,
当另一方使用正常方式关闭连接时返回值为0.如调用close()函数关闭连接.
函数send()只能用于套接字处于连接状态的描述符,之前必须用connect()
函数或者其他函数进行连接.对于send()函数和write()函数之间的差别
是表示发送方式的flag,当flag为0时,send()函数和write()函数完全一致.
而且send(s,buf,len,flags)与sendto(s,buf,len,flags,NULL,0)是等价的.

使用readv()函数接收数据
#include <sys/uio.h>
ssize_t readv(int s, const struct iovec *vector, int count);
readv()函数可用于接收多个缓冲区数据.readv()函数从套接字
描述符s中读取count块数据放到缓冲区向量vector中.
readv()函数的返回值为成功接收到的字节数,当为-1时错误发生,
可以查看errno获取错误码.
参数vector为一个指向向量的指针,结构struct iovecd在文件
<sys/uio.h>中定义:
struct iovec{
	void *iov_base;
	size_t iov_len;
}
在调用readv()函数的时候必须指定iovec的iov_base的长度,
将值放到成员iov_len中.参数vector指向一块结构vector的
内存,大小为count指定.结构vector的成员变量iov_base指向
内存空间,iov_len表示内存的长度.

使用writev()函数发送数据
#include <sys/uio.h>
ssize_t writev(int fd, const struct iovec *vector, int count);
writev()函数可用于接收多个缓冲区数据.writev()函数向套接字
描述符s中写入到向量vector中保存的count块数据.writev()函数的返回值为
成功发送的字节数,当为-1时错误发生,可以查看errno获取错误码.
参数vector为一个指向向量的指针,结构struct iovec在文件<sys/uio.h>
中定义:
struct iovec {
	// 向量的缓冲区地址
	void *iov_base;
	// 向量缓冲区的大小,以字节为单位
	size_t iov_len;
};
在调用writev()函数的时候必须指定iovec的iov_base的长度,将值放到成员
iov_len中.参数vector指向一块结构vector的内存,大小为count指定.结构
vector的成员变量iov_base指向内存空间,iov_len表示内存的长度.




*/

char *getStr() {
//    char *str = "Hello World";
    char str[] = "Hello World";
    return str;
}

void test_sqlite3(void) {
//    SQLiteOpenHelper mySQLiteOpenHelper("/root/temp/tv.db");
//    mySQLiteOpenHelper.onCreate();
//
//    char *str = NULL;
//    str = getStr();
//    fprintf(stdout, "%s\n", str);
//
//    int num;
//    int *p = &num;
//    printf("%d\n", sizeof(p));
//    printf("%p\n", p);
}

// 下面是线程的内容
void *say_hello(void *args) {
    sleep(3);
    printf("Hello Runoob\n");
}

void test_pthread(void) {
    pthread_t p_tids[PTHREADS_NUM];
    // 定义一个属性
    pthread_attr_t attr;
    // 初始化属性值，均设为默认值
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    for (int i = 0; i < PTHREADS_NUM; ++i) {
        // printf("p_tids\[%d\] = %d\n", i, p_tids[i]);
        printf("&p_tids\[%d\] = %p\n", i, &p_tids[i]);
        int ret = pthread_create(&p_tids[i], &attr, say_hello, NULL);
        if (ret != 0) {
            printf("pthread_create error: error_code = %d\n", ret);
        }
    }

    printf("pthread_join\n");

    for (int i = 0; i < PTHREADS_NUM; ++i) {
        // 其后面的代码会被执行
        int r = 10;
        int joinResult = pthread_join(p_tids[i], NULL);
        printf("joinResult = %d\n", joinResult);
    }

    // pthread_exit(NULL);

    printf("Game Over\n");
}
/***
线程的知识点：

 typedef unsigned long int pthread_t;线程标识符

extern int pthread_create __P ((
     // 参数一: pthread_t指针,因此传递的参数是一个地址
     pthread_t *__thread,
     // 参数二: 线程属性
     __const pthread_attr_t *__attr,
     // 参数三: 函数指针
     void *(*__start_routine) (void *)),
     // 参数四: 函数参数
     void *__arg);
第一个参数为指向线程标识符的指针，
第二个参数用来设置线程属性，第二个参数为NULL时将生成默认属性的线程
第三个参数是线程运行函数的起始地址，
最后一个参数是运行函数的参数，不需要传递参数时赋为NULL.
当创建线程成功时，函数返回0，
若不为0则说明创建线程失败，常见的错误返回代码为EAGAIN和EINVAL。
前者表示系统限制创建新的线程，例如线程数目过多了；
后者表示第二个参数代表的线程属性值非法。
创建线程成功后，新创建的线程则运行参数三和参数四确定的函数，原来的线程则继续运行下一行代码。
属性结构为pthread_attr_t，它同样在头文件/usr/include/pthread.h中定义。
属性值不能直接设置，须使用相关函数进行操作，
初始化的函数为pthread_attr_init，这个函数必须在pthread_create函数之前调用。
属性对象主要包括是否绑定、是否分离、堆栈地址、堆栈大小、优先级。
默认的属性为非绑定、非分离、缺省1M的堆栈、与父进程同样级别的优先级。
　　关于线程的绑定，牵涉到另外一个概念：轻进程（LWP：Light Weight Process）。
轻进程可以理解为内核线程，它位于用户层和系统层之间。
系统对线程资源的分配、对线程的控制是通过轻进程来实现的，一个轻进程可以控制一个或多个线程。
默认状况下，启动多少轻进程、哪些轻进程来控制哪些线程是由系统来控制的，这种状况即称为非绑定的。
绑定状况下，则顾名思义，即某个线程固定的"绑"在一个轻进程之上。
被绑定的线程具有较高的响应速度，这是因为CPU时间片的调度是面向轻进程的，
绑定的线程可以保证在需要的时候它总有一个轻进程可用。
通过设置被绑定的轻进程的优先级和调度级可以使得绑定的线程满足诸如实时反应之类的要求。
　　设置线程绑定状态的函数为pthread_attr_setscope，
它有两个参数，第一个是指向属性结构的指针，
第二个是绑定类型，它有两个取值：
PTHREAD_SCOPE_SYSTEM（绑定的）和PTHREAD_SCOPE_PROCESS（非绑定的）。

    设置线程是否分离
    线程的分离状态决定一个线程以什么样的方式来终止自己。
线程的默认属性即为非分离状态，这种情况下，原有的线程等待创建的线程结束。
只有当pthread_join（）函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。
而分离线程不是这样子的，它没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。
程序员应该根据自己的需要，选择适当的分离状态。
设置线程分离状态的函数为
pthread_attr_setdetachstate（
     pthread_attr_t *attr,
     int detachstate）;
第二个参数可选为PTHREAD_CREATE_DETACHED（分离线程）和 PTHREAD _CREATE_JOINABLE（非分离线程）。
这里要注意的一点是，如果设置一个线程为分离线程，而这个线程运行又非常快，
它很可能在pthread_create函数返回之前就终止了，
它终止以后就可能将线程号和系统资源移交给其他的线程使用，
这样调用pthread_create的线程就得到了错误的线程号。
要避免这种情况可以采取一定的同步措施，最简单的方法之一是
可以在被创建的线程里调用pthread_cond_timewait函数，
让这个线程等待一会儿，留出足够的时间让函数pthread_create返回。
设置一段等待时间，是在多线程编程里常用的方法。
但是注意不要使用诸如wait（）之类的函数，
它们是使整个进程睡眠，并不能解决线程同步的问题。
　　 另外一个可能常用的属性是线程的优先级，它存放在结构sched_param中。
用函数pthread_attr_getschedparam和函数pthread_attr_setschedparam进行存放，
一般说来，我们总是先取优先级，对取得的值修改后再存放回去。

函数pthread_join用来等待一个线程的结束。函数原型为：
extern int pthread_join __P ((
     // 参数一: 线程id
     pthread_t __th,
     // 参数二:
     void **__thread_return));
第一个参数为被等待的线程标识符，
第二个参数为一个用户定义的指针，它可以用来存储被等待线程的返回值。
这个函数是一个线程阻塞的函数，调用它的函数将一直等待到被等待的线程结束为止，当函数返回时，被等待线程的资源被收回。

extern void pthread_exit __P ((void *__retval)) __attribute__ ((__noreturn__));
唯一的参数是函数的返回代码，只要pthread_join中的第二个参数thread_return不是NULL，
这个值将被传递给thread_return。
最后要说明的是，一个线程不能被多个线程等待，
否则第一个接收到信号的线程成功返回，其余调用pthread_join的线程则返回错误代码ESRCH。

 获取当前线程id

　　函数原型：pthread_t pthread_self(void);

　　·互斥锁

　　创建pthread_mutex_init；销毁pthread_mutex_destroy；加锁pthread_mutex_lock；解锁pthread_mutex_unlock。

　　·条件锁

　　创建pthread_cond_init；销毁pthread_cond_destroy；触发pthread_cond_signal；广播pthread_cond_broadcast S；等待pthread_cond_wait

pthread_exit等各个线程退出后，进程才结束;
否则main进程强制结束了，线程可能还没反应过来,因此线程不会执行
pthread_join后面的代码只有待线程里的代码全部执行完了才会执行
pthread_exit后面的代码不会被执行
pthread_join与pthread_exit只需要用一个
一起使用也没有关系,只是pthread_exit后面不会被执行
 */

// 下面是继承方面的一点语法内容
/*class Base {
public:
    void fun1(int, int);

    virtual void fun2();

    virtual void fun2(int);
};

class Derived : public Base {
    // 让Base class中的所有fun1均在Derived作用域
    using Base::fun1;
    using Base::fun2;
public:
    void fun1();

    virtual void fun2();
};

void test_class(void) {
    Derived derived;
    int x;
    int y;
    // right
    derived.fun1();
    // error
    // 子类覆盖了父类中的fun1(int ,int ),当编译器看到这个函数时，首先在Derived的作用域内找，
    // 并且找到了，但是参数不匹配，这时候编译器并不会继续扩大搜索范围，而是报错。
    // 加了using Base::fun1;后就可以了
//    derived.fun1(x, y);
    // right
    derived.fun2();
    // error
    // 加了using Base::fun2;后就可以了
//    derived.fun2(x);
}*/

/***
只要在子类中定义了一个跟父类相同名称的函数,不管参数怎样,
那么,子类创建对象后调用子类已有的方法没有问题,但是调用父类的
相同名称的方法(参数不同,但是子类没有这个方法)就不行,
除非加了using Base::方法名;

    构造函数
    析构函数
    拷贝构造函数
    赋值语句
1.析构函数如果在类中声明了，那么必须要定义，也就是至少要有空实现。
2.如果没有声明跟定义拷贝构造函数的话，那么执行Test test2(test);
这样的代码时，构造函数不会被执行，但是析构函数会被执行。
3.声明拷贝构造函数时，其参数必须是对象的引用。
4.Test test3 = test2;// 调用Test::Test(const Test &test)
5.
 Test test3;// 调用Test::Test(int d = 0)
 test3 = test2;// 赋值语句
6.赋值语句在类内实现时
  Test& operator=(const Test &test){...}
  赋值语句在类外实现时
  Test& Test::operator=(const Test &test){...}
*/

class Test {
private:
    int data;
public:
//    Test();

    Test(int);

    Test(const Test &test);

    Test &operator=(const Test &test);

    ~Test();
};

//Test::Test() {
//    printf("Test(): \t%p\n", this);
//}

Test::Test(int d = 0) : data(d) {
    printf("Test(int): \t%p\n", this);
}

Test::Test(const Test &test) {
    this->data = test.data;
    printf("Test(&): \t%p\n", this);
}

Test &Test::operator=(const Test &test) {
    printf("Test(=): \t%p\n", this);
    if (this != &test) {
        this->data = test.data;
    }
    return *this;
}

Test::~Test() {
    printf("~Test(): \t%p\n", this);
}

struct {
    char *name;
    int age;
    char sex;
} XiaoMing;
#define SIZE 10

void test_class(void) {
    Test test(10);

    Test test2(test);

    // 属性有了默认值，所以可以用这种方式创建对象
    Test test3;
    test3 = test2;

    // 不建议使用从字符串常量到‘char*’的转换
    // char *me = "I am a student.";
    char me[] = "I am a student.";
    printf("sizeof(...): \t%d\n", sizeof(me));// 16
    printf("strlen(...): \t%d\n", strlen(me));// 15
    printf("sizeof(...): \t%d\n", sizeof("XiaoMing"));// 9
    printf("strlen(...): \t%d\n", strlen("XiaoMing"));// 8

    /***
     开辟了10个可以放int值的空间
     */
    int *p = (int *) malloc(sizeof(int) * SIZE);
    if (p == NULL) {
        exit(1);
    }
    for (int i = 0; i < 10; ++i) {
        p[i] = i + 1;
    }
    free(p);

    int *p_ = (int *) malloc(sizeof(int));
    *p_ = 10;
    free(p_);

    /***
     new如果开辟内存不成功的话，会抛出异常；
     没有抛出的话，就表示一定会开辟成功。
     创建对象数组时，首先必须要求有默认的构造函数。
     */
    int *q = new int[SIZE];
    for (int j = 0; j < 10; ++j) {
        q[j] = j + 1;
    }
    delete[]q;

    int *q_ = new int(10);
    delete q_;
}















































