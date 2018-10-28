#include "LinuxSocket.h"

/***
基础知识：

    源文件：C、C++
    目标文件：经过编译器的编译生成的CPU可识别的二进制代码.
    可执行文件：目标文件与相关的库链接后生成的文件.
    预编译过程将程序中引用的头文件包含进源代码中,
并对一些宏进行替换.
    编译过程将用户可识别的语言翻译成一组处理器可识别的操作友,
生成目标文件,通常翻译成汇编语言,而汇编语言通常和机器操作码
之间是一种一对一的关系.
    链接：
    编译链接：gcc hello.c 默认生成a.out文件,不生成目标文件
（目标文件作为中间文件,在生成可执行文件后,会被删除）
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
    选项-E告诉编译器进行预编译操作,并将结果显示在计算机屏幕上：
    gcc -E string.c
    预编译生成中间文件：
    gcc -o string.i -E string.c
    编译生成汇编语言：gcc -S string.c 生成string.s
    静态库（***.a,由程序ar生成）
    静态库优点：可以在不用重新编译程序库代码的情况下,
进行程序的重新链接,这种方法节省了编译过程的时间.
静态库的另一个优势是开发者可以提供库文件给使用的人员,
不用开放源代码,这是库函数提供者经常采用的手段.
创建静态库的最基本步骤是先生成目标文件,然后使用
工具ar对目标文件进行归档.
    生成库文件：ar -rcs libstr.a string.o
    GCC链接时使用库函数和一般的obj文件的形式是一致的,如：
gcc -o test main.c libstr.a
    使用“-l库名”,库名是不包含函数库和扩展名的字符串
gcc -o test main.c -lstr（可能提示找不到库文件str）
gcc -o test main.c -L./ -lstr
    在使用-l选项时,-o选项的目的名称要在-l链接的库名称之前,
否则gcc会认为-l是生成的目标而出错.

    生成动态链接库（使用-fPIC选项或者-fpic选项）
    使用-fPIC选项或者-fpic选项的作用是使得gcc生成的代码是位置无关的.
    gcc -shared -Wl,-soname,libstr.so -o libstr.so.1 string.c
    其中选项“-soname,libstr.so”表示生成动态库时的别名是libstr.so；
    “-o libstr.so.1”选项表示是生成名字为libstr.so.1的实际动态链接库文件；
    -shared告诉编译器生成一个动态链接库.
    生成动态链接库之后一个很重要的问题就是安装,
一般情况下将生成的动态链接库复制到系统默认的动态
链接库的搜索路径下,通常有/lib、/usr/lib、/usr/local/lib,
放到之上任何一个目录下都可以.
    动态链接库的配置
    系统中的配置文件/etc/ld.so.conf是动态链接库的
搜索路径配置文件.在这个文件内,存放着可被Linux共享
的动态链接库所在的目录的名字（系统目录/lib、/usr/lib除外）,
多个目录名间以空白字符（空格、换行等）或冒号或逗号分隔.
    动态链接库管理命令
    看Linux网络编程第27页
    使用动态链接库
    在编译程序时,使用动态链接库和静态链接库是一致的,
使用“-l 库名”的方式,在生成可执行文件的时候会链接库
文件.如下面的命令将源文件main.c编译成可执行文件test,
并链接库文件libstr.a或者libstr.so：
    gcc -o test main.c -L./ -lstr
    -L指定链接动态链接库的路径,-lstr链接库函数str.
程序编译时链接动态链接库和运行时使用动态链接库的概念
是不同的,在运行时,程序链接的动态链接库需要在系统目录下
才行.有几种办法可以解决此种问题：
    1）将动态链接库的目录放到程序搜索路径中,可以将库的
路径回到环境变量LD_LIBRARY_PATH中实现.
    export LD_LIBRARY_PATH=/example/ex02: $LD_LIBRARY_PATH
    2）使用ld-Linux.so.2来加载程序,命令格式为：
    /lib/ld-Linux.so.2 --library-path 路径 程序名
    加载test程序的命令为：
    /lib/ld-Linux.so.2 --library-path /example/ex02 test
    如果系统的搜索路径下同时存在静态链接库和动态链接库,
默认情况下会链接动态链接库.如果需要强制链接静态链接库,
需要加上“-static”选项,即编译方法为：
    gcc -o testdl main.c -static -ldl
    动态加载库
    动态加载库和一般的动态链接库所不同的是,一般动态链接库
在程序启动的时候就要寻找动态库,找到库函数；而动态加载库可以
用程序的方法来控制会时候加载.动态加载库主要有函数dlopen()、
dlerror()、dlsym()和dlclose()来控制动态库的使用.
    1.打开动态库dlopen()
    void * dlopen(const char *filename, int flag);
    如
    void *phandle = dlopen("./libstr.so", RTLD_LAZY);
    2.获得函数指针dlsym()
    使用动态链接库的目的是调用其中的函数,完成特定的功能.
函数dlsym()可以获得动态链接库中指定函数的指针,然后可以使用
这个函数指针进行操作.函数dlsym()的原型如下,其中参数handle为
dlopen打开动态库后返回的句柄,参数symbol为函数的名称,返回值为
函数指针.
    void * dlsym(void *handle, char *symbol);

    gcc常用选项
    看Linux网络编程第31页

UNIX下的文件主要分为如下几种:
1.普通文件
2.字符设备文件
3.块设备文件
4.socket文件

文件系统的创建
在Linux下对磁盘进行操作的工具是fdisk.
1.系统分区情况
使用fdisk命令查看当前磁盘的情况: fdisk -l
2.建立分区
3.查看分区是否成功
4.格式化分区: mkfs.ext3 /dev/sdb1
5.挂载分区: mount /dev/sdb1 /test
6.查看分区挂载情况: df

文件描述符
在Linux下用文件描述符来表示设备文件和普通文件.
文件描述符是一个整型的数据,所有对文件的操作都通过
文件描述符实现.文件描述符是文件系统中连接用户空间
和内核空间的枢纽.当打开一个或者创建一个文件时,内核
空间创建相应的结构,并生成一个整型的变量传递给用户空间
的对应进程.进程用这个文件描述符来对文件进行操作.用户
空间的文件操作,例如读写一个文件时,将文件描述符作为参数
传送给read或write.读写函数的系统调用到达内核时,内核解析
作为文件描述符的整型变量,找出对应的设备文件,运行相应的函数,
返回用户空间结果.文件描述符的范围是0~OPEN_MAX,因此是一个
有限的资源,在使用完毕后要及时释放,通常是调用close()函数
关闭.文件描述符的值只在同一个进程中有效,即不同进程的
文件描述符,同一个值很有可能描述的不是同一个设备或者普通文件.
文件描述符为1023时还能正常打开,1024时就打不开了.
 
在Linux系统中有3个已经分配的文件描述符,即
标准输入 0 stdin
标准输出 1 stdout
标准错误 2 stderr
它们的值分别为0、1和2.可以查看/dev下的
stdin（标准输入）、stdout（标准输出）和stderr（标准错误）,
会发现分别指向了/proc/self/fd目录下的0、1、2文件.

打开创建文件open()、create()函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
open()函数打开pathname指定的文件,当函数成功时,
返回一个整型的文件描述符.这个函数正常情况下会返回一个
文件描述符的值,在出错的时候会返回-1.
参数说明:
pathname：
在通常情况下为1024个字节.
flags:
O_RDONLY：  只读,定义为0.单独使用时文件必须存在,否则打开失败.
O_WRONLY：  只写,定义为1.单独使用时文件必须存在,否则打开失败.
O_RDWR：    读写,定义为2.单独使用时文件必须存在,否则打开失败.
O_APPEND：  使每次对文件进行写操作时都追加到文件的尾端.
            单独使用时是写不进去的,需要与O_WRONLY或者O_RDWR搭配.
O_CREAT：   如果文件不存在则创建它,当使用此选项时,第三个
            参数mode需要同时设定,用来说明新文件的权限.
O_EXCL：    查看文件是否存在.如果同时指定了O_CREAT,而文件已经
            存在则会返回-1.用这种方法可以安全地打开一个文件.
O_TRUNC：    将文件长度截断为0.如果此文件存在,并且文件成功打开,
             则会将其长度截断为0.通常用作清空操作.
O_NONBLOCK： 打开文件为非阻塞方式,如果不指定此项,默认的打开
             方式为阻塞方式,即对文件的读写操作需要等待操作的返回状态.
mode：
用于表示打开文件的权限,mode的使用需要结合flags的O_CREAT一起
使用,否则是无效的.
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

在写入的时候对文件进行清空,可以使用open()函数的O_TRUNC选项,
将打开的函数修改为如下形式:
fd = open(filename, O_RDWR | O_TRUNC);

例子:
int fd = -1;
char filename[] = "test.txt";
fd = open(filename, O_RDONLY | O_CREAT, S_IRWXU);
if (-1 == fd) {
    printf("Open file %s failure, fd: %d\n", filename, fd);
} else {
    printf("Open file %s success, fd: %d\n", filename, fd);
    close(fd);
}

在Linux下如果之前没有其他文件打开过,第一个调用打开
文件成功的程序,返回的描述符为最低值,即3.
open()函数不仅可以打开一般的文件,而且可以打开设备文件.
如打开设备文件“/dev/sda1”,即磁盘的第一个分区.

创建文件的函数除了可以在打开时创建外,还可以使用create()
函数创建一个新文件,其函数原型为：
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int create(const char *pathname, mode_t mode);
create()等于一个open的缩写版本,等效于：
open(pathname, O_WRONLY|O_CREAT|O_TRUNC, mode);
create的返回值与open一样,在成功时为创建文件的描述符.

关闭文件close()函数
当函数执行成功时返回0,如果有错误发生,如文件描述符
非法,返回-1.在使用这个函数的时候,通常不检查返回值.
#include <unistd.h>
int close(int fd);
在打开文件之后,必须关闭文件.如果一个进程中没有正常关闭文件,
在进程退出时系统会自动关闭打开的文件.但是打开一个文件的时候,
系统分配的文件描述符为当前进程中最小的文件描述符的值,
这个值一般情况下是递增的,而每个进程中的文件描述符的数量是有
大小限制的.如果一个进程中频繁地打开文件而又忘记关闭文件,当
系统的文件描述符达到最大限制的时候,就会因为没有文件描述符
可以分配造成打开文件失败.

读取文件read()函数
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
read()函数从文件描述符fd对应的文件中读取count字节,
放到buf开始的缓冲区.如果count的值为0,read()函数返回0,
不进行其他操作；如果count的值大于SSIZE_MAX,结果不可预料.
在读取成功的时候,文件对应的读取位置指针,向后移动位置,
大小为成功读取的字节数.
如果read()函数执行成功,返回读取的字节数；
当返回值为-1时,读取函数有错误发生.
如果已经到达文件的末尾,返回0.
返回值的数据类型为ssize_t,这是一个可能不同于int、long
类型的数据类型,它是一个符号数,具体实现时可能定义为
int或者long.
参数buf是一个指针,它指向缓冲区地址的开始
位置,读入的数据将保存在这个缓冲区中；
参数count表示要读取的字节数,通常用这个变量来表示缓冲区的大小,
因此count的值不要超过缓冲区的大小,否则很容易造成缓冲区的溢出.
int fd = -1, i;
ssize_t size = -1;
char buf[10];
char filename[] = "test.txt";
fd = open(filename, O_RDONLY);
if (-1 == fd) {
    printf("Open file %s failure, fd: %d\n", filename, fd);
    return EXIT_FAILURE;
} else {
    printf("Open file %s success, fd: %d\n", filename, fd);
}
while (size) {
    size = read(fd, buf, 10);
    if (-1 == size) {
        close(fd);
        printf("read file error occurs\n");
        return EXIT_FAILURE;
    }
    if (size > 0) {
        printf("read %d bytes:", size);
        printf("\"");
        for (i = 0; i < size; i++) {
            printf("%c", *(buf + i));
        }
        printf("\"\n");
    } else {
        printf("read the end of file\n");
        close(fd);
    }
}

写文件write()函数
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
write()函数向文件描述符fd写入数据,数据的大小由count
指定,buf为要写入数据的指针,write()函数返回值为成功写入数据的
字节数.当操作的对象是普通文件时,写文件的位置从文件的当前
开始,操作成功后,写的位置会增加写入字节数的值.如果在打开文件
的时候指定了O_APPEND项,每次写操作之前,会将写操作的位置移到文件
的结尾处.函数操作成功会返回写入的字节数,当出错时返回-1.
出错的原因有多种,像磁盘已满,或者文件大小超出系统的设置,
例如ext2下的文件大小限制为2Gbytes等.
写操作的返回值与想写入的字节数会存在差异,与read()函数的原因类似.
写操作函数并不能保证将数据成功地写入磁盘,这在异步操作中
经常出现,write()函数通常将数据写入缓冲区,在合适的时机由系统
写入实际的设备.可以调用fsync()函数,显式将输入写入设备.
int fd = -1;
ssize_t size = -1;
char buf[] = "quick brown fox jumps over the lazy dog";
char filename[] = "test.txt";
fd = open(filename, O_WRONLY | O_APPEND);
if (-1 == fd) {
    printf("Open file %s failure, fd: %d\n", filename, fd);
    return EXIT_FAILURE;
}
printf("Open file %s success, fd: %d\n", filename, fd);
size = write(fd, "\n", strlen("\n"));
printf("write %d bytes to file %s\n", size, filename);
size = write(fd, buf, strlen(buf));
printf("write %d bytes to file %s\n", size, filename);
close(fd);

文件偏移lseek()函数
每次打开和对文件进行读写操作后,文件的偏移量都进行了更新.
当写入数据成功时,文件的偏移量要向后移动写入数据的大小.当从
文件中读出数据的时候,文件的偏移量要向后移动读出数据的大小.
文件的偏移量是一个非负整数,表示从文件的开始到当前位置的
字节数.一般情况下,对文件的读写操作都从当前的文件位移量
开始,并增加读写操作成功的字节数.当打开一个文件时,如果
没有指定O_APPEND选项,文件的位移量为0.如果指定了O_APPEND选项,
文件的偏移量与文件的长度相等,即文件的当前操作位置移到了末尾.
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
这个函数对文件描述符fd所代表的文件,按照操作模式whence和
偏移的大小offset,重新设定文件的偏移量.如果lseek()函数操作
成功,则返回新的文件偏移量的值；如果失败返回-1.由于文件的
偏移量可以为负值,判断lseek()是否操作成功时,不要使用小于0
的判断,要使用是否等于-1来判断函数失败.
offset值的含义如下：
如果whence为SEEK_SET,则offset为相对文件开始处的值,
即将该文件偏移量设为距文件开始处offset个字节.
如果whence为SEEK_CUR,则offset为相对当前位置的值,
即将该文件的偏移量设置为其当前值加offset.
如果whence为SEEK_END,则offset为相对文件结尾的值,
即将该文件的偏移量设置为文件长度加offset.
lseek()函数执行成功时返回文件的偏移量,可以用SEEK_CUR
模式下偏移0的方式获得当前的偏移量,如：
off_t cur_pos = lseek(fd, 0, SEEK_CUR);
例子:
off_t offset = -1;
offset = lseek(stdin, 0, SEEK_CUR);
if (-1 == offset) {
    printf("STDIN can't seek\n");
    return EXIT_FAILURE;
}
printf("STDIN can seek");

空洞文件的例子(见Linux网络编程104页)

获得文件状态fstat()函数
1.fstat()函数介绍
在程序设计的时候经常要用到文件的一些特性值,例如,
文件的所有者,文件的修改时间,文件的大小等.
stat()函数,fstat()函数和lstat()函数可以获得文件的状态,
其函数原型如下:
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *path, struct stat *buf);
int fstat(int filedes, struct stat *buf);
int lstat(const char *path, struct stat *buf);
函数的第一个参数是文件描述的参数,可以为文件的路径或者文件描述符；
buf为指向struct stat的指针,获得的状态从这个参数中传回.
当函数执行成功时返回0,返回值为-1表示有错误发生.
结构struct stat为一个描述文件状态的结构,定义如下:
struct stat{
    // 此文件所处设备的设备ID号
    dev_t       st_dev;
    // inode数值
    ino_t       st_ino;
    // 保护设置
    mode_t      st_mode;
    // 硬链接数
    nlink_t     st_nlink;
    // 文件所有者的ID
    uid_t       st_uid;
    // 文件所有者的组ID
    gid_t       st_gid;
    // 设备ID
    dev_t       st_rdev;
    // 以字节计的大小
    off_t       st_size;
    // 文件系统的块大小
    blksize_t   st_blksize;
    // 占用的块的数量
    blkcnt_t    st_blocks;
    // 最后访问时间
    time_t      st_atime;
    // 最后修改时间
    time_t      st_mtime;
    // 最后状态改变时间
    time_t      st_ctime;
};

2.stat()函数的例子
struct stat st;
if (-1 == stat("test.txt", &st)) {
    printf("获得文件状态失败\n");
    return EXIT_FAILURE;
}
printf("包含此文件的设备ID: %d\n", st.st_dev);
printf("此文件的节点: %d\n", st.st_ino);
printf("此文件的保护模式: %d\n", st.st_mode);
printf("此文件的硬链接数: %d\n", st.st_nlink);
printf("此文件的所有者ID: %d\n", st.st_uid);
printf("此文件的所有者的组ID: %d\n", st.st_gid);
printf("设备ID(如果此文件为特殊设备): %d\n", st.st_rdev);
printf("此文件的大小: %d\n", st.st_size);
printf("此文件的所在文件系统块大小: %d\n", st.st_blksize);
printf("此文件的占用块数量: %d\n", st.st_blocks);
printf("此文件的最后访问时间: %d\n", st.st_atime);
printf("此文件的最后修改时间: %d\n", st.st_mtime);
printf("此文件的最后状态改变时间: %d\n", st.st_ctime);

文件空间映射
mmap()函数用来将文件或者设备空间映射到内存中,
可以通过对映射后的内存空间存取来获得与存取文件一致的控制方式,
不必再使用read()函数,write()函数.映射到的内存并不占用空间,
仅仅占用一段地址空间.
1.mmap()函数介绍
#include <sys/mman.h>
void *mmap(void *start, size_t length, int prot,
            int flags, int fd, off_t offset);
将文件描述符fd对应的文件中,自offset开始的一段长length的数据
空间映射到内存中.用户可以设定映射内存的地址,
但是具体函数会映射到内存的哪个位置由返回值确定.当映射成功后,
返回映射到的内存地址.如果失败返回值为(void*)-1.
通过errno值可以获得错误方式.
mmap()函数进行地址映射的时候,用户可以指定要映射到的地址,
这个地址在参数start中指定,通常为NULL,表示由系统自己决定
映射到什么地址.而参数length表示映射数据的长度,即文件需要
遇到到内存中的数据大小.使用mmap()函数有一个限制,只能对
映射到内存的数据进行操作,即限制于开始为offset,大小为length
的区域.参数fd,代表文件的文件描述符,表示要映射到内存中的文件,
通常是open()的返回值；如果需要对文件中需要映射地址进行偏移,
则在参数offset中进行指定.
参数prot
表示映射区保护方式.保护方式prot的值是一个组合值,
可选如下的一个或者多个.这些值可以进行复合运算,其中,
PROT_EXEC表示映射区域可执行,
PROT_READ表示映射区域可读取,
PROT_WRITE表示映射区域可写入,
PROT_NONE表示映射区域不能存取.
例如PROT_WRITE | PROT_READ的方式将映射区设置为可读写.
当然prot的设置受文件打开时的选项限制,当打开文件时为只读,
由上面的写(PROT_WRITE)失效,但是读仍然有效.
参数flags
用于设定映射对象的类型,选项和是否可以对映射对象进行操作(读写等),
这个参数和open()函数中的含义类似.参数flags也是一个组合值,
下面是其可选的设置.
MAP_FIXED: 如果参数start指定了用于需要映射到的地址,而所指的地址
无法成功建立映射,则映射失败.通常不推荐使用此设置,而将start设为0,
由系统自动选取映射地址.
MAP_SHARED: 共享的映射区域,映射区域允许其他进程共享,对映射区域
写入数据将会写入到原来的文件中.
MAP_PRIVATE: 当对映射区域进行写入操作时会产生一个映射文件的复制,
即写入复制(copy on write),而读操作不会影响此复制.对此映射区的
修改不会写回原来的文件,即不会影响原来的文件的内容.
MAP_ANONYMOUS: 建立匿名映射.此时会忽略参数fd,不涉及文件,而且
映射区域无法和其他进程共享.
MAP_DENYWRITE: 对文件的写入操作将被禁止,只能通过对此映射区操作
的方式实现对文件的操作,不允许直接对文件进行操作.
MAP_LOCKED: 将映射区锁定,此区域不会被虚拟内存重置.
参数flags必须为MAP_SHARED或者MAP_PRIVATE二者之一的类型.
MAP_SHARED类型表示多个进程使用的是一个内存映射的副本,任何
一个进程都可对此映射进行修改,其他的进程对此修改是可见的.而
MAP_PRIVATE则是多个进程使用的文件内存映射,在写入操作后,
会复制一个副本给修改的进程,多个进程之间的副本是不一致的.

2.munmap()函数介绍
与mmap()函数对应的函数是munmap()函数,它的作用是取消mmap()函数
的映射关系.其函数原型如下:
#include <sys/mman.h>
int munmap(void *start, size_t length);
参数start为mmap()函数成功后的返回值,即映射的内存地址；
参数length为映射的长度.
使用mmap()函数需要遵循一定的编程模式,其模式如下:
首先使用open()函数打开一个文件,当操作成功的时候会返回一个文件描述符；
使用mmap()函数将此文件描述符所代表的文件映射到一个地址空间,如果映射
成功,会返回一个映射的地址指针；对文件的操作可以通过mmap()映射的地址
来进行,包括读数据,写数据,偏移等,与一般的指针操作相同,不过要注意不要
进行越界操作；当对文件的操作完毕后,需要使用munmap()函数将mmap()
映射的地址取消并关闭打开的文件.
fd = open(filename, flags, mode);
if(-1 == fd){
...
}
ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
...
munmap(ptr, len);
close(fd);

3.mmap()函数和munmap()函数的例子
#define FILELENGTH 80
int fd = -1;
char buf[] = "dog lazy the over jumps fox brown quick";
char *ptr = NULL;
fd = open("mmap.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
if (-1 == fd) {
    printf("Open file failure\n");
    return EXIT_FAILURE;
}
lseek(fd, FILELENGTH - 1, SEEK_SET);
write(fd, "a", 1);
ptr = (char *) mmap(NULL, FILELENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
if ((char *) -1 == ptr) {
    printf("mmap failure\n");
    close(fd);
    return EXIT_FAILURE;
}
memcpy(ptr + 16, buf, strlen(buf));
munmap(ptr, FILELENGTH);
close(fd);


程序,进程和线程
1.Linux进程间的通信和同步方式,包括管道pipe,命名管道fifo,
信号量sem,共享缓冲区shm,消息队列msg,以及信号signal.
2.线程中用到的技术有互斥,条件变量,线程信号.
3.进程的产生过程
进程的产生有多种方式,其基本过程是一致的.
首先复制其父进程的环境配置.
在内核中建立进程结构.
将此结构插入到进程列表,便于维护.
分配资源给此进程.
复制父进程的内存映射信息.
管理文件描述符和链接点.
通知父进程.
4.进程的终止方式
从main返回.
调用exit.
调用_exit.
调用abort.
由一个信号终止.
进程在终止的时候,系统会释放进程所拥有的资源,
如内存,文件符,内核结构等.
5.进程之间的通信
管道,共享内存和消息队列是最常用的方式.
管道是UNIX族中进程通信的最古老的方式,它利用内核在两个进程之间
建立通道,它的特点是与文件的操作类似,仅仅在管道的一端只读,另一端只写.
利用读写的方式在进程之间传递数据.
共享内存是将内存中的一段地址,在多个进程之间共享.多个进程利用获得
的共享内存的地址来直接对内存进行操作.
消息队列是在内核中建立一个链表,发送方按照一定的标识将数据发送到
内核中,内核将其放入量表后,等待接收方的请求.接收方发送请求后,内核按照
消息的标识,从内核中将消息从链表中摘下,传递给接收方.消息队列是一种完全
的异步操作方式.
6.进程之间的同步
Linux下进程的同步方式主要有消息,信号量等.
信号量是一个共享的表示数量的值.用于多个进程之间操作或者共享资源
的保护,它是进程之间同步的最主要方式.
7.进程和线程
进程有进程控制表PCB,系统通过PCB对进程进行高度;
线程有线程控制表TCB.
但是TCB所表示的状态比PCB要少得多.
8.进程产生的方式
fork()函数
system()函数
exec()函数
这些函数的不同在于
其环境的构造之间存在差别,其本质都是对程序运行的各种条件进行设置,
在系统之间建立一个可以运行的程序.
9.进程号
每个进程在初始化的时候,系统都分配了一个ID号,用于标识此进程.
#include <sys/types.h>
#include <unistd.h>
// typedef unsigned int pid_t
pid_t getpid(void);//当前进程ID
pid_t getppid(void);//父进程ID
ps -x | grep 父进程ID
10.进程复制fork()函数
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
成功时返回值是进程的ID号,失败返回-1.
fork()的特点是执行一次,返回两次.
在父进程和子进程中返回的是不同的值,
父进程返回的是子进程的ID号,而子进程则返回0.
例子:
pid_t pid;
//执行一次,返回两次
pid = fork();
if (pid == -1) {
    printf("进程创建失败!\n");
    return -1;
} else if (pid == 0) {
    //返回等于0的数(在这里执行的是子进程中的代码)
    printf("子进程,fork返回值: %d, 子进程ID: %d, 父进程ID: %d\n",
           pid, getpid(), getppid());
} else {
    //返回大于0的数(这个数就是当前进程创建的子进程号)
    printf("父进程,fork返回值: %d, 当前进程ID: %d, 父进程ID: %d\n",
           pid, getpid(), getppid());
}
11.进程复制system()函数
system()函数调用shell的外部命令在当前进程中开始另一个进程.
system()函数调用"/bin/sh-c command"执行特定的命令,
阻塞当前进程直到command命令执行完毕.
system()函数的原型如下:
#include <stdlib.h>
int system(const char *command);
执行system()函数时,会调用fork,execve,waitpid等函数,
其中任意一个调用失败将导致system()函数调用失败.
system()函数的返回值如下:
    1.失败返回-1.
    2.当sh不能执行时,返回127.
    3.成功返回进程状态值.
int ret;
printf("系统分配的进程号为: %d\n", getpid());
ret = system("ping www.hisense.com -c 2");
printf("返回值为: %d\n");
12.进程执行exec()函数
在使用fork()函数和system()函数的时候,系统中都会建立一个新的进程,
执行调用者的操作,而原来的进程还会存在,直到用户显式地退出；
而exec()族的函数与之前的fork()函数和system()函数不同,
exec()族函数会用新进程代替原有的进程,系统会从新的进程运行,
新的进程的PID值会与原来进程的PID值相同.
exec()族函数共有6个,其原型如下:
#include <unistd.h>
extern char **environ;
int execl(const char *path, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execlp(const char *file, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
int execvp(const char *file, char *const argv[]);
上述6个函数中,只有execve()函数是真正意义上的系统调用,
其他5个函数都是在此基础上经过包装的库函数.上述的exec()函数族
的作用是在当前系统的可执行路径中根据指定的文件名来找到合适的
可执行文件名,并用它来取代调用进程的内容,即在原来的进程内部
运行一个可执行文件.上述的可执行文件既可以是二进制文件,也可以是
可执行的脚本文件.
与fork()函数不同,exec()函数族的函数执行成功后不会返回,这是因为
执行的新程序已经占用了当前进程的空间和资源,这些资源包括代码段,
数据段和堆栈等,它们都已经被新的内容取代,而进程的ID等标识性的
信息仍然是原来的东西,即exec()函数族在原来进程的壳上运行了自己的
程序,只有程序调用失败了,系统才会返回-1.
使用exec()比较普遍的一种方法是先使用fork()函数分叉进程,然后在
新的进程中调用exec()函数,这样exec()会占用与原来一样的系统资源来运行.
Linux系统针对上述过程专门进行了优化.由于fork()的过程是
对原有系统进行复制,然后建立子进程,这些过程都比较耗费时间.
如果在fork()系统调用之后进程exec()系统调用,系统就不会进行
系统复制,而是直接使用exec指定的参数来覆盖原有的进程.上述的方法
在Linux系统上叫做"写时复制",即只有在造成系统的内容发生更改
的时候才进行进程的真正更新.
char *args[] = {"/bin/ls", NULL};
printf("系统分配的进程号是: %d\n", getpid());
if (execve("/bin/ls", args, NULL) < 0) {
    printf("创建进程出错!\n");
} else {
    printf("创建进程成功!\n");
    printf("系统新分配的进程号是: %d\n", getpid());
}
13.所有用户态进程的产生进程init
在Linux系统中,所有的进程都是有父子或者堂兄关系的,
没有哪个进程与其他进程完全独立.除了初始进程init,
系统中每个进程都有一个父进程,新的进程不是被全新地
创建,通常是从一个原有的进程进行复制或者克隆的.
Linux操作系统下的每一个父进程或者兄弟进程,
并且有自己的子进程.可以在Linux下使用命令pstree
来查看系统中运行的进程之间的关系.可以知道init进程
是所有进程的祖先,其他的进程都是由init进程直接或者
间接fork()出来的.

进程间通信和同步
在Linux下的多个进程间的通信机制叫做IPC,这是多个进程
之间进行相互沟通的一个方法.在Linux下有多种进程间通信
的方法:半双工管道,FIFO(命名管道),消息队列,信号量,
共享内存等.使用这些通信机制可以为Linux下的网络服务器
开发提供灵活而又坚固的框架.

1.半双工管道
管道是一种把两个进程之间的标准输入和标准输出连接起来的
机制.管道是一种历史悠久的进程间通信的办法,自从UNIX操作
系统诞生,管道就存在了.
由于管道仅仅是将某个进程的输出和另一个进程的输入相连接的
单向通信的办法,因此称其为"半双工".在shell中管道用"|"表示.
如"ls -l | grep *.c".把"ls -l"的输出当做"grep *.c"的输入,
管道在前一个进程中建立输出通道,在后一个进程建立输入通道,
将数据从管道的左边传输到管道的右边.
进程创建管道,每次创建两个文件描述符来操作管道.其中一个对管道
进行写操作,另一个描述符对管道进行读操作.
创建管道的函数原型为:
#include <unistd.h>
int pipe(int filedes[2]);
数组中的filedes是一个文件描述符的数组,用于保存管道返回的
两个文件描述符.数组中的第1个函数(下标为0)是为了读操作而
创建和打开的,而第2个函数(下标为1)是为了写操作而创建和打开的.
当函数执行成功时,返回0;失败时返回-1.
pipe()函数的例子
int result = -1;
int fd[2];
pid_t pid;
char string[] = "你好,管道";
char readbuffer[80];
int *write_fd = &fd[1];
int *read_fd = &fd[0];
result = pipe(fd);
if (-1 == result) {
    printf("建立管道失败\n");
    return EXIT_FAILURE;
}
pid = fork();
if (-1 == pid) {
    printf("fork进程失败\n");
    return EXIT_FAILURE;
}
if (0 == pid) {
    printf("子进程\n");
    close(*read_fd);
    result = write(*write_fd, string, strlen(string));
    printf("发送了 %d 个数据\n", result);
} else {
    printf("父进程\n");
    close(*write_fd);
    memset(readbuffer, '\0', sizeof(readbuffer));
    result = read(*read_fd, readbuffer, sizeof(readbuffer));
    printf("接收到 %d 个数据,内容为:\n%s\n", result, readbuffer);
}
管道阻塞和管道操作的原子性
当管道的写端没有关闭时,如果写请求的字节数目大于阈值PIPE_BUF,
写操作的返回值是管道中目前的数据字节数,如是请求的字节数目
不大于PIPE_BUF,则返回管道中现有数据字节数
(此时,管道中数据量小于请求的数据量);
或者返回语法的字节数
(此时,管道中数据量不小于请求的数据量).
PIPE_BUF在include/Linux/limits.h中定义,不同的内核版本
可能会有所不同.Posix.1要求PIPE_BUF至少为512字节.
管道进行写入操作的时候,当写入数据的数目小于128K时写入是
非原子的,如果把父进程中的两次写入字节数都改为128K,可以
发现:写入管道的数据量大于128K字节时,缓冲区的数据将被连续地
写入管道,直到数据全部写完为止,如果没有进程读数据,则一直阻塞.
下面的代码为一个管道读写的例子.在成功建立管道后,子进程向
管道中写入数据,父进程从管道中读出数据.子进程一次写入128K个
字节的数据,父进程每次读取10K字节的数据.当父进程没有数据可读
的时候退出.
#define K 1024
#define WRITELEN (128*K)
int result = -1;
int fd[2], nbytes = -1;
pid_t pid;
char string[WRITELEN] = "你好,管道";
char readbuffer[10 * K];
int *write_fd = &fd[1];
int *read_fd = &fd[0];
result = pipe(fd);
if (-1 == result) {
    printf("建立管道失败\n");
    return EXIT_FAILURE;
}
pid = fork();
if (-1 == pid) {
    printf("fork进程失败\n");
    return EXIT_FAILURE;
}
if (0 == pid) {
    printf("子进程\n");
    int write_size = WRITELEN;
    result = 0;
    close(*read_fd);
    while (write_size >= 0) {
        result = write(*write_fd, string, write_size);
        if (result > 0) {
            write_size -= result;
            printf("写入 %d 个数据,剩余 %d 个数据\n", result, write_size);
        } else {
            break;
        }
    }
} else {
    printf("父进程\n");
    close(*write_fd);
    while (1) {
        memset(readbuffer, '\0', sizeof(readbuffer));
        nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
        if (nbytes <= 0) {
            printf("没有数据写入了\n");
            break;
        }
        printf("接收到 %d 个数据,内容为:\n%s\n", nbytes, readbuffer);
    }
}

2.命名管道
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
在文件系统中命名管道是以设备特殊文件的形式存在的.
不同的进程可以通过命名管道共享数据.
创建FIFO
在目录/ipc下建立一个名字为namedfifo的命名管道:
mkfifo /ipc/namedfifo
FIFO操作
对命名管道FIFO来说,IO操作与普通的管道IO操作基本上是一样的,
二者之间存在着一个主要的区别.在FIFO中,必须使用一个open()
函数来显式地建立连接到管道的通道.一般来说FIFO总是处于阻塞状态.
也就是说,如果命名管道FIFO打开时设置了读权限,则读进程将一直"阻塞",
一直到其他进程打开该FIFO并且向管道中写入数据.这个阻塞动作反过来
也是成立的,如果一个进程打开一个管道写入数据,当没有进程向管道中
读取数据的时候,写管道的操作也是阻塞的,直到已经写入的数据被读出后,
才能进行写入操作.如果不希望在进行命名管道操作的时候发生阻塞,可以
在open()调用中使用O_NONBLOCK标志,以关闭默认的阻塞动作.

 3.消息队列
 消息队列是内核地址空间中的内部链表,通过Linux内核在各个进程
 之间传递内容.消息顺序地发送到消息队列,并以几种不同的方式从
 队列中获取,每个消息队列可以用IPC标识符唯一的进行标识.内核中
 的消息队列是通过IPC的标识符来区别的,不同的消息队列之间是相对
 独立的.每个消息队列中的消息,又构成一个独立的链表.
 消息缓冲区结构
 常用的结构是msgbuf结构.程序员可以以这个结构为模板定义自己的
 消息结构.在头文件<linux/msg.h>中,它的定义如下:
 struct msgbuf {
    long mtype;
    char mtext[1];
 };
 mtype:
 消息类型,以正数表示.用户可以给某个消息设定一个类型,可以在消息队列
 中正确地发送和接收自己的消息.例如.在socket编程过程中,一个服务器
 可以失道接受多个客户端的连接,可以为每个客户端设定一个消息类型,
 服务器和客户端之间的通信可以通过此消息类型来发送和接收消息,并且多个
 客户端之间通过消息类型来区分.
 mtest:
 消息数据.在构建自己的消息结构时,这个域并不一定要设为char
 或者长度为1.可以根据实际的情况进行设定,这个域能存放任意
 形式的任意数据,程序员可以重新定义msgbuf结构.例如:
 struct msgmbuf {
    long mtype;
    char mtext[10];
    long length;
 };
 上面定义的消息结构与系统模板定义的不一致,但是mtype是一致的.
 消息在通过内核在进程之间收发时,内核不对mtext域进行转换,
 任意的消息都可以发送.具体的转换工作是在应用程序之间进行的.
 但是,消息的大小,存在一个内部的限制.在Linux中,它在
 linux/msg.h中的定义如下:
 #define MSGMAX 8192
 消息总的大小不能超过8192个字节,这其中包括mtype成员,它的长度
 是4个字节(long类型).
 结构msgid_ds






#include <sys/types.h>
#include <sys/socket.h>
#include <stddef.h>
#include <sys/un.h>

TCP网络编程基础
基于TCP的Socket编程的服务器流程：
1.创建套接字（socket).
2.将套接字绑定到一个本地地址和端口上（bind）.
3.将套接字设为监听模式,准备接收客户请求（listen）.
4.等待客户请求到来；当请求到来后,接受连接请求,
  返回一个新的对应于此次连接的套接字（accept）.
5.用返回的套接字和客户端进行通信（send/recv）.
6.返回,等待另一客户的请求.
7.关闭套接字.

TCP协议是TCP/IP协议中很重要的一个协议,
由于它传输的稳定性,在很多程序中都在使用.
套接字地址结构
1.通用套接字数据结构
通用的套接字地址类型的定义如下,
它可以在不同协议族之间进行强制转换.
typedef unsigned short sa_family_t;
struct sockaddr                 // 套接字地址结构
{
    sa_family_t sa_family;      // 协议族
    char        sa_data[14];    // 协议族数据
}

2.实际使用的套接字数据结构
在网络程序设计中所使用的函数中几乎所有的套接字函数
都用这个结构作为参数.这个结构的定义如下：
// 以太网套接字地址结构
struct sockaddr_in
{
    // 无符号字符类型,结构struct sockaddr_in的长度,
    // 为16字节
    u8 sin_len;
    // 无符号字符类型,通常设置为与函数domain一致,
    // 如AF_INET
    u8 sin_family;
    // 无符号short类型,16位的端口号,网络字节序
    u16 sin_port;
    // IP地址,为32位,每8位表示IP地址的一个段,
    // 网络字节序
    struct in_addr sin_addr;
    // 未用,保留
    char sin_zero[8];
}
结构struct sockaddr_in的成员变量in_addr用于表示IP地址,
这个结构的定义如下：
struct in_addr      // IP地址结构
{
    u32 s_addr;     // 32位IP地址,网络字节序
}

3.结构sockaddr和结构sockaddr_in的关系
结构struct sockaddr和结构struct sockaddr_in
是一个同样大小的结构.
由于结构struct sockaddr和结构struct sockaddr_in
的大小是完全一致的,所以进行地址结构设置时,
通常的方法是利用
结构struct sockaddr_in进行设置,
然后强制转换为结构struct sockaddr类型.
因为这两个结构大小是完全一致的,所以进行
这样的转换不会有什么副作用.

用户层和内核层交互过程
套接字参数中有部分参数是需要用户传入的,
这些参数用来与Linux内核进行通信,如指向地址结构的指针.
通常是采用内存复制的方法进行.如bind()函数需要传入
地址结构struct sockaddr *my_addr和my_addr指向参数的长度.
1.向内核传入数据的交互过程
向内核传入数据的函数有send(),bind()等,
从内核得到数据的函数有accept(),recv()等.
bind()函数向内核中传入的参数
有套接字地址结构和结构的长度这样两个参数.
bind()函数的原型为：
int bind(
    // 套接字文件描述符
    int sockfd,
    // 套接字地址结构,指向地址结构的指针
    const struct sockaddr *my_addr,
    // 套接字地址结构的长度
    socklen_t addrlen);
调用函数bind()的时候,地址结构通过内存复制的方式
将其中的内容复制到内核,地址结构的长度通过传值的
方式传入内核,内核按照用户传入的地址结构长度来
复制套接字地址结构的内容.

2.从内核传出数据的交互过程
从内核向用户空间传递参数的过程则相反.通过地址结构的
长度和套接字地址结构指针来进行地址结构参数的传出操作.
通常是两个参数完成传出操作的功能,一个是表示地址结构
长度的参数,一个是表示套接字地址结构地址的指针.
传出过程与传入过程中的参数不同的是,表示地址结构长度
的参数在传入过程中是传值,而在传出过程中是通过值值
完成的.内核按照用户传入的地址结构长度进行套接字
地址结构数据的复制,将内核中的地址结构数据复制到
用户传入的地址结构指针中.内核付出的数据是通过
用户传进去的套接字地址结构反映出来的,需要的
数据可以从这个套接字地址结构中去拿.

TCP网络编程架构
TCP网络编程有两种模式,
服务器模式,客户端模式.
客户端程序设计模式流程与服务器端的处理模式流程类似,
二者的不同之处是客户端在套接字初始化之后可以不进行
地址绑定,而是直接连接服务器端.
客户端连接服务器的处理过程中,客户端根据用户设置的
服务器地址,端口等参数与特定的服务器程序进行通信.

服务器模式创建一个服务程序,等待客户端用户的连接,
接收到用户的连接请求后,根据用户的请求进行处理；
1.服务器端的程序设计模式
TCP连接的服务器模式的程序设计流程分为：
套接字初始化---                 socket()
套接字与端口的绑定---            bind()
设置服务器的侦听连接---          listen()
接受客户端连接---               accept()
接收和发送数据并进行数据处理---   read(),write()
关闭套接字---                  close()

客户端模式则根据目的服务器的地址和端口进行连接,
向服务器发送请求并对服务器的响应进行数据处理.
2.客户端的程序设计模式
主要分为：
套接字初始化---               socket()
连接服务器---                 connect()
读写网络数据并进行数据处理---    read(),write()
关闭套接字---                 close()

3.客户端与服务端的交互过程
客户端的连接过程,对服务器端是接收过程,在这个过程
中客户端与服务器端需要进行三次握手,建立TCP连接.
建立TCP连接之后,客户端与服务器端之间就可以进行数据的交互.
客户端与服务器端之间的数据交互是相对的过程,
客户端的读数据过程对应服务器端的写数据过程,
客户端的写数据过程对应服务器端的读数据过程.
在服务器端和客户端之间的数据交互完毕之后,
关闭套接字连接.

创建网络插口函数socket()
网络程序设计中的套接字系统调用socket()函数用来获得文件描述符.
1.socket()函数介绍
socket()函数的原型如下,如果函数调用成功,会返回一个
表示这个套接字的文件描述符,失败则返回-1.
#include <sys/types.h>
#include <sys/socket.h>
int socket(
    // 协议族为domain,用于设置网络通信的域,
    // 函数socket()根据这个参数选择通信协议的族
    int domain,
    // 协议类型
    int type,
    // 协议编号为protocol的套接字文件描述符
    int protocol);

使用socket()函数的时候需要设置上述3个参数,
例如:
第1个参数domain设置为AF_INET,
第2个参数设置为SOCK_STREAM,
第3个参数设置为0,建立一个流式套接字.
常用的一种方式:
int sock = socket(AF_INET, SOCK_STREAM, 0);

参数说明:
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
SOCK_STREAM             流式套接字.TCP连接,
                        提供序列化的,可靠的,
                        双向连接的字节流.
                        支持带外数据传输.
SOCK_DGRAM              数据包套接字.
                        支持UDP连接
                        （无连接状态的消息）.
SOCK_SEQPACKET          序列化包,提供一个序列化的,
                        可靠的,双向的基于连接的数据
                        传输通道,数据长度定常.
                        每次调用读系统调用时数据需要
                        将全部数据读出.
SOCK_RAW                RAW类型,提供原始网络协议访问.
SOCK_RDM                提供可靠的数据报文,不过可能
                        数据会有乱序.
SOCK_PACKET             专用类型,不能在通用程序中
                        使用.专用数据 包,直接从
                        设备驱动接收数据.
并不是所有的协议族都实现了这些协议类型,
如AF_INET协议族就没有实现SOCK_SEQPACKET协议类型.
主要使用SOCK_STREAM和SOCK_DGRAM.

protocol
用于指定某个协议的特定类型,即type类型中的某个类型.
通常某个协议中只有一种特定类型,这样protocol
参数仅能设置为0；但是有些协议有多种特定的类型,
就需要设置这个参数来选择特定的类型.

注意:
类型为SOCK_STREAM的套接字表示一个双向的字节流,与管道类似.
流式的套接字在进行数据收发之前必须已经连接,连接使用
connect()函数进行.一旦连接,可以使用read()或者write()
函数进行数据的传输.流式通信方式保证数据不会丢失或者重复
接收,当数据在一段时间内仍然没有接收完毕,可以将这个连接认为
已经死掉.
SOCK_DGRAM和SOCK_RAW这两种套接字可以使用函数sendto()来
发送数据,使用recvfrom()函数接收数据,recvfrom()接收来自
指定IP地址的发送方的数据.
SOCK_PACKET是一种专用的数据包,它直接从设备驱动接收数据.

socket错误
函数socket()并不总是执行成功,有可能会出现错误,
错误的产生有多种原因,可以通过errno获得.
通常情况下造成
函数socket()失败的原因是输入的参数错误造成的,
如某个协议不存在等,这时需要详细检查函数的输入参数.
由于函数的调用不一定成功,在进行程序设计的时候,
一定要检查返回值.
errno的值及含义
值                      含义
EXCCES                  没有权限建立指定domain的
                        type的socket
EAFNOSUPPORT            不支持所给的地址类型
EINVAL                  不支持此协议或者协议不可用
EMFILE                  进程文件表溢出
ENFILE                  已经达到系统允许打开的文件
                        数量,打开文件过多
ENOBUFS/ENOMEM          内存不足.socket只有到资源
                        足够或者有进程释放内存
EPROTONOSUPPORT         指定的协议type在domain中
                        不存在
其他                     ...

2.应用层函数socket()和内核函数之间的关系
用户设置套接字的参数后,函数要能够起作用,
需要与内核空间的相关系统调用交互.
应用层的socket()函数是和内核层的系统调用相对应的.

绑定一个地址端口对bind()函数
在建立套接字文件描述符成功后,需要对套接字进行
地址和端口的绑定,才能进行数据的接收和发送操作.
1.bind()函数介绍
bind()函数将长度为addlen的struct sockadd类型的
参数my_addr与sockfd绑定在一起,将sockfd绑定到
某个端口上,如果
使用connect()函数则没有绑定的必要.
绑定的函数原型如下：
#include <sys/types.h>
#include <sys/socket.h>
int bind(
    int sockfd,
    const struct sockaddr *my_addr,
    socklen_t addrlen);
参数说明:
参数一：用socket()函数创建的文件描述符.
参数二：指向一个结构为sockaddr参数的指针,
    sockaddr中包含了地址,端口和IP地址的信息.
    在进行地址绑定的时候,需要先
    将地址结构中的IP地址,端口,类型等
    结构struct sockaddr中的域进行设置后才能进行绑定,
    这样绑定后才能将套接字文件描述符与地址等结合在一起.
参数三：是参数二这个结构的长度,可以设置成
    sizeof(struct sockaddr).
    使用sizeof(struct sockaddr)来设置addlen
    是一个良好的习惯,虽然一般情况下使用AF_INET
    来设置套接字的类型和其对应的结构,
    但是不同类型的套接字有不同的地址描述符结构,
    如果对地址长度进行了强制的指定,
    可能会造成不可预料的结果.

bind()函数的返回值为
 0时表示绑定成功,
-1时表示绑定失败.
errno的错误值如下.
值                     含义
EADDRINUSE             给定地址已经使用
EBADF                  sockfd不合法
EINVAL                 sockfd已经绑定到其他地址
ENOTSOCK               sockfd是一个文件描述符,
                       不是socket描述符
EACCES                 地址被保护,用户的权限不足
EADDRNOTAVAIL          接口不存在或者绑定地址不是
                       本地 UNIX协议族,AF_UNIX
EFAULT                 my_addr指针超出用户空间
                       UNIX协议族,AF_UNIX
EINVAL                 地址长度错误,或者socket
                       不是AF_UNIX族 UNIX协议族,
                       AF_UNIX
ELOOP                  解析my_addr是符号链接过多
                       UNIX协议族,AF_UNIX
ENAMETOOLONG           my_addr过长 UNIX协议族,
                       AF_UNIX
ENOENT                 文件不存在 UNIX协议族,AF_UNIX
ENOMEM                 内存内核不足 UNIX协议族,
                       AF_UNIX
ENOTDIR                不是目录 UNIX协议族,AF_UNIX
EROFS                  socket节点应该在只读文件
                       系统上 UNIX协议族,AF_UNIX

bind()函数例子:
初始化一个AF_UNIX族中的SOCK_STREAM类型的套接字,
先使用结构struct sockaddr_un初始化my_addr,
然后进行绑定,结构struct sockaddr_un的定义为:
struct sockaddr_un {
    // 协议族,应该设置为AF_UNIX
    sa_family_t sun_family;
    // 路径名,UNIX_PATH_MAX的值为108
    char sun_path[UNIX_PATH_MAX];
};
#define MY_SOCK_PATH "/somepath"
int sfd;
// AF_UNIX对应的结构
struct sockaddr_un my_addr;
sfd = socket(AF_UNIX, SOCK_STREAM, 0);
if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
}
// 对结构体进行清零
memset(&my_addr, 0, sizeof(struct sockaddr_un));
my_addr.sun_family = AF_UNIX;
// 复制路径到地址结构
strncpy(my_addr.sun_path,
        MY_SOCK_PATH,
        sizeof(my_addr.sun_path) - 1);
if (bind(sfd,
         (struct sockaddr *) &my_addr,
         sizeof(struct sockaddr_un)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
}
// ...
close(sfd);

Linux的GCC编译器有一个特点,一个结构的最后一个成员为数组时,
这个结构可以通过最后一个成员进行扩展,可以在程序运行时第一次
调用此变量的时候动态生成结构的大小,例如上面的代码,并不会因为
struct sockaddr_un比struct sockaddr大而溢出.

下面例子是使用结构struct sockaddr_in绑定一个AF_INET族的
流协议,先将结构struct sockaddr_in的sin_famliy设置为AF_INET,
然后设置端口,接着设置一个IP地址,最后进行绑定.
#define MYPORT 3490
int sfd;
struct sockaddr_in my_addr;
sfd = socket(AF_INET, SOCK_STREAM, 0);
if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
}
// 地址结构的协议族
my_addr.sin_family = AF_INET;
// 地址结构的端口地址,MYPORT为主机字节序,需要转化为网络字节序
my_addr.sin_port = htons(MYPORT);
// IP,将字符串的IP地址转化为二进制网络字节序
my_addr.sin_addr.s_addr = inet_addr("192.168.1.150");
// 将my_addr.sin_zero置为0
bzero(&(my_addr.sin_zero), 8);
if (bind(sfd,
         (struct sockaddr *) &my_addr,
         sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
}
// ...
close(sfd);

监听本地端口listen()函数
函数listen()用来初始化服务器可连接队列,
服务器处理客户端连接请求的时候是顺序处理的,
同一时间仅能处理一个客户端连接.
当多个客户端的连接请求同时到来的时候,
服务器并不是同时处理,而是将不能处理的客户端
连接请求放到等待队列中,这个队列的
长度由listen()函数来定义.
1.listen()函数介绍
listen()函数的原型如下,其中的backlog表示等待队列的长度.
#include <sys/socket.h>
int listen(int sockfd, int backlog);
当listen()函数成功运行时,返回值为0；
当运行失败时,返回值为-1,并且设置errno值.
值                      含义
EADDRINUSE              另一个socket已经在同一端口侦听
EBADF                   参数sockfd不是合法的描述符
ENOTSOCK                参数sockfd不是代表socket的
                        文件描述符
EOPNOTSUPP              socket不支持listen操作

在接受一个连接之前,需要用listen()函数来侦听端口,
listen()函数中参数backlog表示在accept()函数处理之前
在等待队列中的客户端的长度,如果超过这个长度,
客户端返回一个EXONNREFUSED错误.
listen()函数仅对类型为SOCK_STREAM或者SOCK_SEQPACKET
的协议有效.例如,如果对一个SOCK_DGRAM的协议使用
listen(),将会出现错误errno的值为EOPNOTSUPP,
表示此socket不支持listen()操作.
大多数系统的设置为20,可以将其设置修改为5或者10,
根据系统可承受负载或者应用程序的需求来确定.
如果设置过大,那么会被设置为系统默认最大值.

2.listen()函数的例子
在成功进行socket()初始化和bind()端口之后,
设置listen()队列的长度为5.
#define MYPORT 3490
int sfd;
struct sockaddr_in my_addr;
sfd = socket(AF_INET, SOCK_STREAM, 0);
if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
}
// 地址结构的协议族
my_addr.sin_family = AF_INET;
// 地址结构的端口地址,网络字节序
my_addr.sin_port = htons(MYPORT);
// IP,将字符串的IP地址转化为网络字节序
my_addr.sin_addr.s_addr = inet_addr("192.168.1.150");
// 将my_addr.sin_zero置为0
bzero(&(my_addr.sin_zero), 8);
if (bind(sfd,
         (struct sockaddr *) &my_addr,
         sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
}
if(listen(sfd, 5)==-1){
    perror("listen");
    exit(EXIT_FAILURE);
}
// ...
close(sfd);

接受一个网络请求accept()函数
当一个客户端的连接请求到达服务器主机侦听的端口时,
此时客户端的连接会在队列中等待,直到使用服务器处理接收请求.
函数accept()成功执行后,会返回一个新的套接口文件
描述符来表示客户端的连接,客户端连接的信息可以通过这个
新描述符来获得.因此当服务器成功处理客户端的请求连接后,
会有两个文件描述符,老的文件描述符表示正在监听的socket,
新产生的文件描述符表示客户端的连接,函数send()和rece()
通过新的文件描述符进行数据收发.
1.accept()函数介绍
accept()函数的原型如下：
#include <sys/types.h>
#include <sys/socket.h>
int accept(
    int sockfd,
    struct sockaddr *addr,
    socklen_t *addrlen);
通过accept()函数可以得到成功连接客户端的IP地址,
端口和协议族等信息,这个信息是通过参数addr获得的.
当accept()返回的时候,会将客户端的信息存储在参数addr中.
参数addrlen表示参数二(addr)所指内容的长度,可以使用
sizeof(struct sockaddr_in)来获得.需要注意的
是在accept中addrlen参数是一个指针而不是结构,
accept()将这个指针传给TCP/IP协议栈.
accept()函数的返回值是新连接的客户端套接字文件描述符,
与客户端之间的通信是通过accept()返回的新套接字文件描述符
来进行的,而不是通过建立套接字时的文件描述符,
这是在程序设计的时候需要注意的地方.
如果accept()函数发生错误,accept()
会返回-1.通过errno可以得到错误值.
值                      含义
EAGAIN/EWOULDBLOCK      此socket使用了非阻塞模式,
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
ENOTSOCK                文件描述符是一个文件,
                        不是socket
EOPNOTSUPP              引用的socket不是流类型
                        SOCK_STREAM
EFAULT                  参数addr不可写
ENOBUFS/ENOMEM          内存不足
EPROTO                  协议错误
EPERM                   防火墙不允许连接

2.accept()函数的例子
#define MYPORT 3490
int sfd, client_fd;
// 用于保存网络地址长度
int addr_length;
struct sockaddr_in my_addr, client_addr;
sfd = socket(AF_INET, SOCK_STREAM, 0);
if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
}
// 地址结构的协议族
my_addr.sin_family = AF_INET;
// 地址结构的端口地址,网络字节序
my_addr.sin_port = htons(MYPORT);
// 表示任意的本地IP地址
my_addr.sin_addr.s_addr = INADDR_ANY;
// 将my_addr.sin_zero置为0
bzero(&(my_addr.sin_zero), 8);
if (bind(sfd,
         (struct sockaddr *) &my_addr,
         sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
}
if (listen(sfd, 10) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
}
addr_length = sizeof(struct sockaddr_in);
client_fd = accept(sfd, &client_addr, &addr_length);
if (client_fd == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
}
// ...
close(client_fd);
close(sfd);

连接目标网络服务器connect()函数
客户端在建立套接字之后,不需要进行地址绑定,
就可以直接连接服务器.连接服务器的函数为connect(),
此函数连接指定参数的服务器,如IP地址,端口等.
1.connect()函数介绍
#include <sys/types.h>
#include <sys/socket.h>
int connect(
    // 建立套接字时返回的套接字文件描述符,
    // 它是由系统调用socket()返回的
    int sockfd,
    // 客户端需要连接的服务器的目的端口和IP地址
    // 以及协议类型
    struct sockaddr *serv_addr,
    // serv_addr内容的大小,可以使用
    // sizeof(struct sockaddr)而获得,
    // 与bind()不同,这个参数是一个整形的变量
    // 而不是指针
    int addrlen);
connect()函数的返回值在成功时为0,
当发生错误的时候返回-1,可以查看errno获得错误的原因.
值                      含义
EACCES                  在AF_UNIX族协议中,使用路径名
                        作为标识.EACCES表示目录
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
EINPROGRESS             socket是非阻塞模式,而连接
                        不能立刻返回
EINTR                   函数被信号中断
EISCONN                 socket已经连接
ENETUNREACH             网络不可达
ENOTSOCK                文件描述符不是一个socket
ETIMEDOUT               连接超时

2.connect()函数的例子
#define DEST_IP "132.241.5.10"
#define DEST_PORT 23
int ret = 0;
int sfd;
struct sockaddr_in server;
sfd = socket(AF_INET, SOCK_STREAM, 0);
if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
}
// 地址结构的协议族
server.sin_family = AF_INET;
// 地址结构的端口地址,网络字节序
server.sin_port = htons(DEST_PORT);
// 服务器的IP地址
server.sin_addr.s_addr = htonl(DEST_IP);
// 将my_addr.sin_zero置为0
bzero(&(server.sin_zero), 8);
ret = connect(sfd, (struct sockaddr *) &server, sizeof(struct sockaddr));
if (ret == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
}
// ...
close(sfd);

写入数据函数write()函数
当服务器端在接收到一个客户端的连接后,
可以通过套接字描述符进行数据的写入操作.
对套接字进行写入的形式和过程与普通文件
的操作方式一致,内核会根据文件描述符的
值来查找所对应的属性,当为套接字的时候,
会调用相对应的内核函数.
int size;
char data[1024];
size = write(s, data, 1024);

读取数据函数read()函数
与写入数据类似,使用read()函数可以从套接字描述符中
读取数据.当然在读取数据之前,必须建立套接字并连接.
读取数据的方式如下所示,从套接字描述符s中读取1024个
字节,放入缓冲区data中,size变量的值为成功读取的
数据的大小.
int size;
char data[1024];
size = read(s, data, 1024);

关闭套接字函数close()函数
关闭socket连接可以使用close()函数实现,
函数的作用是关闭已经打开的socket连接,
内核会释放相关的资源,关闭套接字之后
就不能再使用这个套接字文件描述符进行读写操作了.

函数shutdown()可以使用更多方式来关闭连接,
允许单方向切断通信或者切断双方的通信.函数原型如下,
第一个参数s是切断通信的套接字文件描述符,
第二个参数how表示切断的方式.
#include <sys/socket.h>
int shutdown(int s, int how);
函数shutdown()用于关闭双向连接的一部分,具体的
关闭行为方式通过参数的how设置来实现.可以为如下值：
SHUT_RD:   值为0,表示切断读,
           之后不能使用此文件描述符进行读操作.
SHUT_WR:   值为1,表示切断写,
           之后不能使用此文件描述符进行写操作.
SHUT_RDWR: 值为2,表示切断读写,
           之后不能使用此文件描述符进行读写操作,
           与close()函数功能相同.
函数shutdown()如果调用成功则返回0,
如果失败则返回-1,通过errno可以获得错误的具体信息.
值                      含义
EBADF                   文件描述符不是合法的值
ENOTCONN                socket没有连接
ENOTSOCK                s是一个文件,不是socket

截取信号的例子
在Linux操作系统中当某些状况发生变化时,
系统会向相关的进程发送信号.信号的处理方式
是系统会先调用进程中注册的处理
函数 ,然后调用系统默认的响应方式,
包括终止进程,因此在系统结束进程前,
注册信号处理函数进行一些处理是一个完善程序
的必须条件.
1.信号处理
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
这个函数向信号signum注册一个void (*sighandler_t)(int)类型
的函数,函数的句柄为handler.当进程中捕捉到注册的信号时,会调用
响应函数的句柄handler.信号处理函数在处理系统默认的函数之前
会被调用.
2.信号SIGPIPE
如果正在写入套接字的时候,当读取端已经关闭时,可以得到一个
SIGPIPE信号.信号SIGPIPE会终止当前进程,因为信号系统在调用
系统默认处理方式之前会先调用用户注册的函数,所以可以通过注册
SIGPIPE信号的处理函数来获取这个信号,并进行相应的处理.
void sig_pipe(int sign){
    printf("Catch a SIGPIPE signal\n");
    // 释放资源
}
// 注册信号(放在启动客户端时的地方好了)
signal(SIGPIPE,sig_pipe);
测试过程:
在客户端连接后,退出服务器程序.当标准输入有数据的时候,客户端会
通过套接字描述符发送数据到服务器端,而服务器已经关闭,因此客户端
会收到一个SIGPIPE信号.

3.信号SIGINT
信号SIGINT通常是由Ctrl+c终止进程造成的,kill命令默认发送SIGINT
信号,用于终止进程运行向当前活动的进程发送这个信号.
void sig_int(int sign){
    printf("Catch a SIGINT signal\n");
    // 释放资源
}
// 注册信号
signal(SIGINT, sig_int);

第8章内容
服务器和客户端信息的获取
字节序
字节序是由于不同的主处理器和操作系统,对大于一个字节的变量
在内存中的存放顺序不同而产生的.
字节序通常有大端字节序和小端字节序两种分类方法.
1.字节序介绍
如一个16位的整数,它由两个字节构成,在有的系统上会将高字节
放在内存的低地址上,而有的系统上则将高字节放在内存的高地址上,
所以存在字节序的问题.
大于一个字节的变量类型的表示方法有两种:
小端字节序(Little Endian, LE):在表示变量的内存地址的
                起始地址存放低字节,高字节顺序存放.
大端字节序(Big Endian, BE):在表示变量的内存地址的
                起始地址存放高字节,低字节顺序存放.
如变量的值为0xabcd,存放的示意图见233页.
2.字节序的例子
1)字节序结构.
程序先建立一个联合类型to,用于测试字节序,成员value是short
类型变量,可以通过成员byte来访问value变量的高字节和低字节.
#include <stdio.h>
typedef union{
    // 短整形变量
    unsigned short int value;
    // 字符类型
    unsigned char byte[2];
}to;
2)变量声明:
声明一个to类型的变量typeordr,将值0xabcd赋给成员变量value.
由于在类型to中,value和byte成员共享同一块内存,所以可以通过
byte的不同来访问value的高字节和低字节.
// 一个to类型变量
to typeorder;
// typeorder变量赋值为0xabcd
typeorder.value = 0xabcd;
3)小端字节序判断.
小端字节序的检查通过判断typeorder变量的byte成员高字节和
底字节的值来进行:低字节的值为0xcd,高字节的值为0xab.
if(typeorder.byte[0] == 0xcd && typeorder.byte[1] == 0xab){
    // 低字节在前
    printf("Low endian byte order byte[0]:0x%x, byte[1]:0x%x\n",
            typeorder.byte[0], typeorder.byte[1]);
}
4)大端字节序判断.
大端字节序的检查同样通过判断typeorder变量的byte成员
高字节和低字节的值来进行:低字节的值为0xab,高字节的值为0xcd.
if(typeorder.byte[0] == 0xab && typeorder.byte[1] == 0xcd){
    // 高字节在前
    printf("High endian byte order byte[0]:0x%x, byte[1]:0x%x\n",
            typeorder.byte[0], typeorder.byte[1]);
}
5)编译运行程序.
完整代码:
typedef union {
    // 短整形变量
    unsigned short int value;
    // 字符类型
    unsigned char byte[2];
} to;
to typeorder;
typeorder.value = 0xabcd;
if (typeorder.byte[0] == 0xcd && typeorder.byte[1] == 0xab) {
    printf("低端字节序\n");
    // 低字节在前
    printf("Low endian byte order byte[0]:0x%x, byte[1]:0x%x\n",
           typeorder.byte[0], typeorder.byte[1]);
} else if (typeorder.byte[0] == 0xab && typeorder.byte[1] == 0xcd) {
    printf("高端字节序\n");
    // 高字节在前
    printf("High endian byte order byte[0]:0x%x, byte[1]:0x%x\n",
           typeorder.byte[0], typeorder.byte[1]);
}

字节序转换函数
网络字节序是指多字节变量在网络传输时的表示方法,
网络字节序采用高端字节序的表示方法.
这样小端字节序的系统通过网络传输变量的时候
需要进行字节序的转换,大端字节序的变量则不需要进行转换.
1.字节序转换转换函数介绍
#include <arpa/inet.h>
// 主机字节序到网络字节序的长整形转换
uint32_t htonl(uint32_t hostlong);
// 主机字节序到网络字节序的短整形转换
uint16_t htons(uint16_t hostlong);
// 网络字节序到主机字节序的长整形转换
uint32_t ntohl(uint32_t hostlong);
// 网络字节序到主机字节序的短整形转换
uint16_t ntohs(uint16_t hostlong);
函数的命名规则为"字节序""to""字节序""变量类型".
h: host主机字节序
n: network网络字节序
l: long型变量
s: short型变量
说明:
程序员在程序设计的时候,需要调用字节序转换函数将主机的
字节序转换为网络字节序,至于是否交换字节的顺序,则由字节序
转换函数的实现来保证.也就是说对于用户来说这种转换是透明的.
只要将需要在网络上传输的变量调用一遍此类的转换函数进行
一次转换即可,不用考虑目标系统的主机字节序方式.

一个字节序转换的例子
1.16位字节序转换结构
先定义用于16位字节序转换的结构to16,这个结构是一个
联合类型,通过value来赋值,通过byte数组来进行
字节序转换.
typedef union {
    // 短整形变量
    unsigned short int value;
    // 字符类型
    unsigned char byte[2];
} to16;
2.32位字节序转换结构
typedef union {
    // 短整形变量
    unsigned short int value;
    // 字符类型
    unsigned char byte[4];
} to32;
3.变量值打印函数showvalue()
showvalue()函数用于打印变量值,打印的方式是从变量存储空间的
第一个字节开始,按照字节进行打印.showvalue()函数有两个
输入参数,一个是变量的地址指针begin,一个是表示字长的标志flag.
参数flag的值为BITS16的时候打印16位变量的值,参数flag为
BITS32的时候打印32位变量的值.
#define BITS16 16
#define BITS32 32
void showvalue(unsigned char *begin, int flag) {
    int num = 0, i = 0;
    if (flag == BITS16) {
        num = 2;
    } else if (flag == BITS32) {
        num = 4;
    }
    for (i = 0; i < num; i++) {
        printf("%x ", *(begin + 1));
    }
    printf("\n");
}
4.主函数main()
to16 v16_orig, v16_turn1, v16_turn2;
to32 v32_orig, v32_turn1, v32_turn2;
5.16位值0xabcd的二次转换
v16_orig.value = 0xabcd;
// 第一次转换
v16_turn1.value = htons(v16_orig.value);
// 第二次转换
v16_turn2.value = htons(v16_turn1.value);
6.32位值0x12345678的二次转换
v32_orig.value = 0x12345678;
// 第一次转换
v32_turn1.value = htonl(v32_orig.value);
// 第二次转换
v32_turn2.value = htonl(v32_turn1.value);
7.结果打印
完整代码:
typedef union {
    // 短整形变量
    unsigned short int value;
    // 字符类型
    unsigned char byte[2];
} to16;
typedef union {
    // 短整形变量
    unsigned short int value;
    // 字符类型
    unsigned char byte[4];
} to32;
to16 v16_orig, v16_turn1, v16_turn2;
v16_orig.value = 0xabcd;
// 第一次转换
v16_turn1.value = htons(v16_orig.value);
// 第二次转换
v16_turn2.value = htons(v16_turn1.value);
printf("16 host to network byte order change:\n");
printf("\torig:\t");
showvalue(v16_orig.byte, BITS16);
printf("\t1 times:");
showvalue(v16_turn1.byte, BITS16);
printf("\t2 times:");
showvalue(v16_turn2.byte, BITS16);

to32 v32_orig, v32_turn1, v32_turn2;
v32_orig.value = 0x12345678;
// 第一次转换
v32_turn1.value = htonl(v32_orig.value);
// 第二次转换
v32_turn2.value = htonl(v32_turn1.value);
printf("32 host to network byte order change:\n");
printf("\torig:\t");
showvalue(v32_orig.byte, BITS32);
printf("\t1 times:");
showvalue(v32_turn1.byte, BITS32);
printf("\t2 times:");
showvalue(v32_turn2.byte, BITS32);

字符串IP地址和二进制IP地址的转换
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// 将点分四段式的IP地址转为地址结构in_addr值
int inet_aton(const char *cp, struct in_addr *inp);
// 将字符串转换为in_addr值
in_addr_t inet_addr(const char *cp);
// 字符串地址的网络部分转为in_addr值
in_addr_t inet_network(const char *cp);
// 将in_addr结构地址转为字符串
char *inet_ntoa(struct in_addr in);
// 将网络地址和主机地址合成为IP地址
struct in_addr inet_makeaddr(int net, int host);
// 获得地址的主机部分
in_addr_t inet_lnaof(struct in_addr in);
// 获得地址的网络部分
in_addr_t inet_netof(struct in_addr in);
1.inet_aton()函数
inet_aton()函数将在cp中存储的点分十进制字符串类型的
IP地址,转换为二进制的IP地址,转换后的值保存在指针inp
指向的结构struct in_addr中.当转换成功时返回值为
非0,当传入的地址非法时,返回值为0.
2.inet_addr()函数
inet_addr()函数将cp中存储的点分十进制字符串类型的IP
地址转换为二进制的IP地址,IP地址是以网络字节序表达的.
如果输入的参数非法,返回值为INADDR_NONE(通常为-1),
否则返回值为转换后的IP地址.
这个函数是函数inet_aton()的缩减版,由于值-1同时可以
理解为是合法IP地址255.255.255.255的转换结果,所以
不能使用这个函数转换IP地址255.255.255.255.
3.inet_network()函数
inet_network()函数将cp中存储的点分十进制字符类型的IP
地址,转换为二进制的IP地址,IP地址是以网络字节序表达的.
当成功时返回32位表示IP地址,失败时返回值为-1.参数cp中
的值有可能采用如下形式:
a.b.c.d:这种形式指定了IP地址的全部4个段,是一个完全的IP
地址转换,这种情况下函数inet_network()与函数inet_addr()
完全一致.
a.b.c:这种形式指定了IP地址的前3个段,a.b解释为IP地址的前
16位,c解释为后面的16位.例如172.16.888会将888解释为IP
地址的后16位.
a.b:这种形式指定了IP地址的前两个段,a为IP地址的前8位,
b解释为后面的24位.例如,172.888888会将888888解释为IP地址
的后3段.
a:当仅为一部分时,a的值直接作为IP地址,不做字节序转换.
4.inet_ntoa()函数
inet_ntoa()函数将一个参数in所表示的Internet地址结构
转换为点分十进制的4段式字符串IP地址,其形式为"a.b.c.d".
返回值为转换后的字符串指针,此内存区域为静态的,有可能会
被覆盖,因此函数并不是线程安全的.
例如,将二进制的IP地址0x1000000000000001使用函数
inet_ntoa()转换为字符串类型的结果为"127.0.0.1".
5.inet_makeaddr()函数
一个主机的IP地址分为网络地址和主机地址,inet_makeaddr()
函数将主机字节序的网络地址net和主机地址host合并成一个
网络字节序的IP地址.
下面的代码将网络地址127和主机地址1合并成一个IP地址"127.0.0.1":
unsigned long net, host;
net = 0x0000007F;
host = 0x00000001;
struct in_addr ip = inet_makeaddr(net, host);
6.inet_lnaof()
inet_lnaof()函数返回IP地址的主机部分.例如,下面的例子返回
IP地址127.0.0.1的主机部分:
const char *addr = "127.0.0.1";
unsigned long ip = inet_network(addr);
unsigned long host_id = inet_lnaof(ip);
7.inet_netof()函数
inet_netof()函数返回IP地址的网络部分.例如,下面的例子返回
IP地址127.0.0.1的网络部分.
const char *addr = "127.0.0.1";
unsigned long ip = inet_network(addr);
unsigned long network_id = inet_netof(ip);
8.结构struct in_addr
结构struct in_addr在文件<netinet/in.h>中定义.通常据说的
IP地址的二进制形式就保存在成员变量s_addr中.结构struct in_addr
的原型如下:
struct in_addr {
    // IP地址
    unsigned long int s_addr;
};
注意:
函数inet_addr(),inet_network()的返回值为-1时表示错误,
这占用了255.255.255.255的值,因此可能存在缺陷.
函数inet_ntoa()的返回值为一个指向字符串的指针,这块内存函数
inet_ntoa()每次调用都会重新覆盖,因此函数并不安全,可能存在
某种隐患.
将字符串IP地址转换为in_addr时,注意字符串中对IP地址的描述,
上述函数假设字符串中以0开始表示8进制,以0x开始表示16进制,
将按照各进制对字符串进行解析.例如IP地址192/168.000.037
最后一段的037表示的是8进制的数值,即相当于"192.168.0.31".

inet_pton()函数和inet_ntop()函数
inet_pton()函数和inet_ntop()函数是一套安全的地址转换函数.
所谓的"安全"是相对于inet_aton()函数的不可重入性来说.这两个
函数都是可以重入的,并且这些函数支持多种地址类型,
包括IPv4和IPv6.
1.inet_pton()函数
inet_pton()函数将字符串类型的IP地址转换为二进制类型.
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int inet_pton(int af, const char *src, void *dst);
参数说明:
af表示网络类型的示协议族,在IPv4下的值为AF_INET.
src表示需要转换的字符串.
dst指向转换后的结果,在IPv4下,dst指向结构struct in_addr的指针.
当函数inet_pton()的返回值为-1时,通常是用于af所指定
的协议族不支持造成的,此时errno的返回值为EAFNOSUPPORT;
当函数inet_pton()的返回值为0时,表示src指向的值不是合法的
IP地址;
当函数inet_pton()的返回值为正数时,表示转换成功.

2.inet_ntop()函数
inet_ntop()函数将二进制的网络IP地址转换为字符串.
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
const char *inet_ntop(int af, const char *src,
                    char *dst, socklen_t cnt);
参数说明:
af表示网络类型的协议族,在IPv4下的值为AF_INET.
src为需要转换的二进制IP地址,在IPv4下,
src指向一个struct in_addr结构类型的指针.
dst指向保存结果缓冲区的指针.
cnt的值是dst缓冲区的大小.
inet_ntop()函数返回一个指向dst的指针.
当发生错误时,返回NULL.当af设定的协议族不支持时,
errno设置为EAFNOSUPPORT.
当dst缓冲区大小过小的时候errno的值为ENOSPC.

地址转换函数的例子
1.初始化设置
先对程序进行初始化的必要设置.例如测试的字符串IP地址,
用户保存结果的网络地址结构和IP地址结构等参数.
2.测试函数inet_aton()
测试函数inet_aton(),将字符串IP地址192.168.1.1转换成
二进制IP地址,并将结果打印出来.
3.测试函数inet_addr()
测试函数inet_addr(),将字符串IP地址转换为二进制IP地址.
先测试192.168.1.1,再测试255.255.255.255.
4.测试函数inet_ntoa()
测试函数inet_ntoa(),先测试IP地址192.168.1.1对应的
字符串str,然后测试IP地址255.255.255.255对应的字符串
IP地址str2.两个IP地址都转换完毕后,一起打印转换后的值,
会发现str和str2的值是相同的.
5.测试函数inet_addr()
测试函数inet_addr(),将字符串IP地址转换为二进制IP地址,
使用的字符串为192.16.1.
6.测试函数inet_lnaof()
测试函数inet_lnaof(),获得本机地址.这个函数只取
四段式IP地址的前一段.
7.测试函数inet_netof()
测试函数inet_netof(),获得本机地址.这个函数只取
四段式IP地址的前三段.

完整代码:
// 第1步
struct in_addr ip, local, network;
// a.b.c.d类型的网络地址字符串
char addr1[] = "192.168.1.1";
// 二进制值为全1的IP地址对应的字符串
char addr2[] = "255.255.255.255";
// a.b.c类型的网络地址字符串
char addr3[] = "192.16.1";
char addr[16];
char *str = NULL, *str2 = NULL;
int err = 0;
// 2
err = inet_aton(addr1, &ip);
if (err != -1) {
    printf("inet_aton:string %s value is: 0x%x\n", addr1, ip.s_addr);
} else {
    printf("inet_aton:string %s error\n", addr1);
}
// 3
ip.s_addr = inet_addr(addr1);
if (err != -1) {
    printf("inet_addr:string %s value is: 0x%x\n", addr1, ip.s_addr);
} else {
    printf("inet_addr:string %s error\n", addr1);
}
ip.s_addr = inet_addr(addr2);
if (err != -1) {
    printf("inet_addr:string %s value is: 0x%x\n", addr2, ip.s_addr);
} else {
    printf("inet_addr:string %s error\n", addr2);
}
// 4
ip.s_addr = 192 << 24 | 168 << 16 | 1 << 8 | 1;
str = inet_ntoa(ip);
ip.s_addr = 255 << 24 | 255 << 16 | 255 << 8 | 255;
str2 = inet_ntoa(ip);
printf("inet_ntoa:ip: 0x%x string1 %s, pre is: %s\n", ip.s_addr, str2, str);
// 5
ip.s_addr = inet_addr(addr3);
if (err != -1) {
    printf("inet_addr:string %s value is: 0x%x\n", addr3, ip.s_addr);
} else {
    printf("inet_addr:string %s error\n", addr3);
}
str = inet_ntoa(ip);
printf("inet_ntoa:string %s ip: 0x%x\n", str, ip.s_addr);
// 6
inet_aton(addr1, &ip);
local.s_addr = htonl(ip.s_addr);
local.s_addr = inet_lnaof(ip);
str = inet_ntoa(local);
printf("inet_lnaof:string %s ip: 0x%x\n", str, local.s_addr);
// 7
network.s_addr = inet_netof(ip);
printf("inet_netof:value: 0x%x\n", network.s_addr);
输出信息"inet_ntoa:ip: 0xffffffff string1 255.255.255.255, pre is: 255.255.255.255",
表明函数inet_ntoa()在进行二进制IP地址到字符串IP地址的
转换过程中是不可重入的,这个函数转换两个不同的IP地址得到
了同一个结果.这是由于函数的实现没有考虑重入的特性,用
同一个缓冲区保存了临时结果.函数inet_addr()同样存在
不可重入的问题.此类函数在调用之后,需要立即将结果取出,
没有取出结果之前不能进行同样函数的调用.

使用函数inet_pton()和函数inet_ntop()的例子
#define ADDRLEN 16
struct in_addr ip;
char ipstr[] = "192.168.1.1";
char addr[ADDRLEN];
const char *str = NULL;
int err = 0;
err = inet_pton(AF_INET, ipstr, &ip);
if (err > 0) {
    printf("inet_pton:ip %s value is: 0x%x\n", ipstr, ip.s_addr);
}
ip.s_addr = htonl(192 << 24 | 168 << 16 | 12 << 8 | 255);
str = (const char *) inet_ntop(AF_INET,
                               (void *) &ip,
                               (char *) &addr[0],
                               ADDRLEN);
if (str) {
    printf("inet_ntop:ip 0x%x is %s\n", ip.s_addr, str);
}

套接字描述符判定函数issockettype()
套接字文件描述符从形式上与通用文件描述符没有区别,判断一个
文件描述符是否是一个套接字描述符可以通过如下的方法实现:
先调用函数fstat()获得文件描述符的模式,然后将模式的
S_IFMT部分与标识符S_IFSOCK比较,可以知道一个文件描述符
是否为套接字描述符.
int issockettype(int fd) {
    struct stat st;
    int err = fstat(fd, &st);
    if (err < 0) {
        return -1;
    }
    if ((st.st_mode & S_IFMT) == S_IFSOCK) {
        return 1;
    } else {
        return 0;
    }
}

int test() {
    int ret = issockettype(0);
    printf("value %d\n", ret);
    int s = socket(AF_INET, SOCK_STREAM, 0);
    ret = issockettype(s);
    printf("value %d\n", ret);
}

获取主机信息的函数
gethostbyname()函数和gethostbyaddr()函数都可以获得主机的信息.
gethostbyname()函数通过主机的名称获得主机的信息.
gethostbyaddr()函数通过IP地址获得主机的信息.

1.gethostbyname()函数介绍
#include <netdb.h>
extern int h_errno;
struct hostent *gethostbyname(const char *name);
这个函数的参数name是要查询的主机名,通常是DNS的域名.
如gethostbyname("www.sina.com.cn")获得主机的信息.
gethostbyname()函数的返回值是一个指向结构struct hostent
类型变量的指针,当为NULL时,表示发生错误,错误类型可以通过
errno获得,错误的类型及含义如下:
HOST_NOT_FOUND:查询的主机不可知,即查不到相关主机的信息.
NO_ADDRESS和NO_DATA:请求的名称合法但是没有合适的IP地址.
NO_RECOVERY:域名服务器不响应.
TRY_AGAIN:域名服务器当前出现临时错误,稍后再试.
struct hostent{
    // 主机的正式名称(如新浪的www.sina.com.cn)
    char *h_name;
    // 别名列表(可能有多个,所以用链表表示,链表尾部是一个NULL指针)
    char **h_aliases;
    // 主机地址类型
    // 为AF_INET时表示为IPv4的IP地址
    // 为AF_INET6时表示为IPv6的IP地址
    int h_addrtype;
    // 地址长度(以字节为单位)
    // 对于IPv4来说为4个字节
    int h_length;
    // 地址列表
    char **h_addr_list;
};
// 为了向前兼容定义的宏
#define h_addr h_addr_list[0]

2.gethostbyaddr()函数介绍
#include <netdb.h>
extern int h_errno;
struct hostent *gethostbyaddr(const void *addr, int len, int type);
gethostbyaddr()函数通过查询IP地址来获得主机的信息.
参数说明:
addr: 在IPv4的情况下指向一个struct in_addr的地址结构,用户需要查询
      主机的IP地址填入到这个参数中.
len: 表示第一个参数所指区域的大小,在IPv4情况下为
     sizeof(struct in_addr),即32位.
type: 指定需要查询主机IP地址的类型,在IPv4的情况下为AF_INET.

注意:
函数gethostbyname和gethostbyaddr是不可重入的函数,由于传出的值
为一块静态的内存地址,当另一次查询到来的时候,这块区域会被占用,所以
在使用的时候要小心.

使用主机名获取主机信息的例子
char host[] = "www.sina.com.cn";
struct hostent *ht = NULL;
ht = gethostbyname(host);
if (ht) {
    int i = 0;
    printf("host: %s\n", host);
    printf("name: %s\n", ht->h_name);
    printf("type: %s\n",
           ht->h_addrtype == AF_INET
           ?
           "AF_INET"
           :
           "AF_INET6");
    printf("length: %d\n", ht->h_length);
    for (i = 0;; i++) {
        if (ht->h_addr_list[i] != NULL) {
            // 编译不过
            // printf("IP: %s\n",
                   // inet_ntoa((unsigned int *) ht->h_addr_list[i]));
            printf("IP: %s\n", ht->h_addr_list[i]);
        } else {
            break;
        }
    }
    for (i = 0;; i++) {
        if (ht->h_aliases[i] != NULL) {
            printf("alias %d : %s\n", i, ht->h_aliases[i]);
        } else {
            break;
        }
    }
}

函数gethostbyname()不可重入的例子
char host1[] = "www.sina.com.cn";
char host2[] = "www.sohu.com";
struct hostent *ht = NULL, *ht1 = NULL, *ht2 = NULL;
ht1 = gethostbyname(host1);
ht2 = gethostbyname(host2);
int j = 0;
for (j = 0; j < 2; j++) {
    if (j == 0) {
        ht = ht1;
    } else {
        ht = ht2;
    }
    if (ht) {
        int i = 0;
        printf("host: %s\n", host1);
        printf("name: %s\n", ht->h_name);
        printf("type: %s\n",
               ht->h_addrtype == AF_INET
               ?
               "AF_INET"
               :
               "AF_INET6");
        printf("length: %d\n", ht->h_length);
        for (i = 0;; i++) {
            if (ht->h_addr_list[i] != NULL) {
                // 编译不过
                // printf("IP: %s\n",
                // inet_ntoa((unsigned int *) ht->h_addr_list[i]));
                printf("IP: %s\n", ht->h_addr_list[i]);
            } else {
                break;
            }
        }
        for (i = 0;; i++) {
            if (ht->h_aliases[i] != NULL) {
                printf("alias %d : %s\n", i, ht->h_aliases[i]);
            } else {
                break;
            }
        }
    }
}
这里的不可重入的意思是:
输出的结果都是关于www.sohu.com的信息,关于www.sina.com.cn
主机信息都已经被www.sohu.com的信息给覆盖了.因此,使用函数
gethostbyname()进行主机信息查询的时候,函数返回后,要马上
将结果取出,否则会被后面的函数调用过程覆盖.

协议名称处理函数
协议族处理函数有如下几个,可以通过协议的名称,
编号等获取协议类型.
#include <netdb.h>
// 从协议文件中读取一行
struct protoent *getprotoent(void);
// 从协议文件中找到匹配项
struct protoent *getprotobyname(const char *name);
// 按照协议类型的值获取匹配项
struct protoent *getprotobynumber(int proto);
// 设置协议文件打开状态
void setprotoent(int stayopen);
// 关闭协议文件
void endprotoent(void);
上面的函数对文件/etc/protocols中的记录进行操作,文件中记录了
协议的名称,值和别名等值.与结构struct protoent的定义一致.
结构protoent的定义如下:
struct protoent{
    // 协议的官方名称
    char *p_name;
    // 别名列表(可能有多个值)
    char **p_aliases;
    // 协议的值
    int p_proto;
};
1.函数getprotoent()从文件/etc/protocols中读取一行并且
返回一个指向struct protoent的指针,包含读取一行的协议.
需要事先打开文件/etc/protocols.
2.函数getprotobyname()按照输入的协议名称name,匹配文件
/etc/protocols中的选项,返回一个匹配项.
3.函数getprotobynumber()按照输入的协议值proto,匹配文件
/etc/protocols中的选项,返回一个匹配项.
4.函数setprotoent()打开文件/etc/protocols,当stayopen
为1的时候,在调用函数getprotobyname()或者getprotobynumber()
查询协议时,并不关闭文件.
5.函数endprotoent()关闭文件/etc/protocols.
函数getprotoent(),getprotobyname()和getprotobynumber()
在调用成功时返回一个指向结构struct protoent的指针,失败时,
返回NULL.

使用协议族函数的例子
如下的例子按照名称查询一组协议的项目,首先用setprotoent(1)
打开文件/etc/protocols,然后使用函数getprotobyname()
查询函数并显示出来,最后使用函数endprotoent()关闭文件
/etc/protocols.
1.显示协议项目函数display_protocol()
display_protocol()函数将一个给定结构protoent中的协议
名称打印出来,并判断是否有别名,将本协议所有相关的别名都打印
出来,最后打印协议的值.
void display_protocol(struct protoent *pt) {
    if (pt) {
        // 协议的官方名称
        printf("protocol name: %s\n", pt->p_name);
        if (pt->p_aliases) {
            printf("alias name:\n");
            int i = 0;
            while (pt->p_aliases[i]) {
                printf("    %s\n", pt->p_aliases[i]);
                i++;
            }
        }
        // 协议值
        printf("value: %d\n", pt->p_proto);
    }
}

int test() {
    const char *const protocol_name[] = {
            "ip",
            "icmp",
            "ggp",
            "ipencap",
            "st",
            "tcp",
            "egp",
            "igp",
            "pup",
            "udp",
            "hmp",
            "xns-idp",
            "rdp",
            "iso-tp4",
            "xtp",
            "ddp",
            "idpr-cmtp",
            "ipv6",
            "ipv6-route",
            "ipv6-frag",
            "idrp",
            "rsvp",
            "gre",
            "esp",
            "ah",
            "skip",
            "ipv6-icmp",
            "ipv6-nonxt",
            "ipv6-opts",
            "rspf",
            "vmtp",
            "eigrp",
            "ospf",
            "ax.25",
            "ipip",
            "etherip",
            "encap",
            "pim",
            "ipcomp",
            "vrrp",
            "12tp",
            "isis",
            "sctp",
            "fc",
            NULL
    };
    // 在使用函数getprotobyname()时不关闭文件/etc/protocols
    setprotoent(1);
    int i = 0;
    while (protocol_name[i] != NULL) {
        struct protoent *pt = getprotobyname(
                (const char *) &protocol_name[i][0]);
        if (pt) {
            // 显示协议项目
            display_protocol(pt);
        }
        i++;
    }
    // 关闭文件/etc/protocols
    endprotoent();
}

第9章内容
数据的IO和复用
Linux系统中的IO函数主要有
read(),write(),
readv(),writev(),
send(),recv(),
sendmsg(),recvmsg().

使用recv()函数接收数据
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int s, void *buf, size_t len, int flags);
参数说明:
s:     套接字文件描述符,调用socket()返回的.
buf:   一个指针,指向接收网络数据的缓冲区.
len:   接收缓冲区的大小,以字节为单位.
flags: 用于设置接收数据的方式.
值                   含义
MSG_DONTWAIT        非阻塞操作,立刻返回,不等待
MSG_ERRQUEUE        错误消息从套接字错误队列接收
MSG_OOB
MSG_PEEK            查看数据,不进行数据缓冲区的清空
MSG_TRUNC           返回所有的数据,即使指定的缓冲区过小
MSG_WAITALL         等待所有消息
经常使用的MSG_DONTWAIT进行接收数据的时候,不进行等待,
即使没有数据也立刻返回,即此时的套接字是非阻塞操作.
具体含义:
MSG_DONTWAIT: 这个标志将单个IO操作设为非阻塞方式,而不需要在
套接字上打开非阻塞标志,执行IO操作,然后关闭非阻塞标志.
MSG_ERRQUEUE: 该错误的传输依赖于所使用的协议.
MSG_OOB: 这个标志可以接收带外数据,而不是接收一般数据.
MSG_PEEK: 这个标志用于查看可读的数据,在recv()函数执行后,
内核不会将这些数据丢弃.
MSG_TRUNC: 在接收数据后,如果用户的缓冲区大小不足以完全
复制缓冲区中的数据,则将数据截断,仅靠复制用户缓冲区大小的
数据.其他的数据会被丢弃.
MSG_WAITALL: 这个标志告诉内核在没有读到请求的字节数之前
不使读操作返回.如果系统支持这个标志,可以去掉readn()函数
而用下面的代替:
#define readn(fd, ptr, n) recv(fd, ptr, n, MSG_WAITALL).
即使设置了MSG_WAITALL,如果发生以下情况:
a)捕获一个信号
b)连接终止
c)在套接字上发生错误
这个函数返回的字节数仍然会比请求的少.
当指定MSG_WAITALL标志时,函数会复制与用户指定的长度相等的数据.
如果内核中的当前数据不能满足要求,会一直等待直到数据足够的时候
才返回.

recv()函数从套接字s中接收数据放到缓冲区buf中,
buf的长度为len,以字节为单位,操作的方式由flags指定.
函数recv()的返回值是成功接收到的字节数,当返回值为-1时发生错误,
可以查看errno获取错误码.当另一方使用正常方式关闭
连接的时候返回值为0,如调用close()函数关闭连接.
recv()函数errno的值及含义
值                   含义
EAGAIN              套接字定义为非阻塞,而操作采用了阻塞方式,
                    或者定义的超时时间已经达到却没有接收到数据
EBADF               参数s不是合法描述符
ECONNREFUSED        远程主机不允许此操作
EFAULT              接收缓冲区指针在此进程之外
EINTR               接收到中断信号
EINVAL              传递了不合法参数
ENOTCONN            套接字s表示流式套接字,此套接字没有连接
ENOTSOCK            参数不是套接字描述符

recv()函数通常用于TCP类型的套接字,
UDP使用recvfrom()函数接收数据,
当然在数据报套接字绑定地址和端口后,
也可以使用recv()函数接收数据.
recv()函数从内核的接收缓冲区中复制到用户指定的缓冲区,
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
send()函数errno的值及含义
值                       含义
EAGAIN/EWOULDBLOCK      套接字定义为非阻塞,而操作采用了阻塞方式,
                        或者定义的超时时间已经达到却没有接收到数据
EBADF                   参数s不是合法描述符
ECONNREFUSED            远程主机不允许此操作
EFAULT                  接收缓冲区指针在此进程之外
EINTR                   在发送数据之前接收到中断信号
EINVAL                  传递了不合法参数
ENOTCONN                套接字s表示流式套接字,此套接字没有连接
ENOTSOCK                参数不是套接字描述符
ECONNRESET              连接断开
EDESTADDRREQ            套接字没有处于连接状态
ENOBUFS                 发送缓冲区已满
ENOMEM                  没有足够内存
EOPNOTSUPP              设定的发送方式flag没有实现
EPIPE                   套接字已经关闭
EACCES                  套接字不可写

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
readv()函数errno的值及含义
值                   含义
EAGAIN              套接字定义为非阻塞,而操作采用了阻塞方式,
                    或者定义的超时时间已经达到却没有接收到数据
EBADF               参数s不是合法描述符
ECONNREFUSED        远程主机不允许此操作
EFAULT              接收缓冲区指针在进程之外
EINTR               接收到中断信号
EINVAL              参数iov_len超出了ssize_t类型的范围,或者count参数
                    小于0或者大于可允许最大值
ENOTCONN            套接字s表示流式套接字,此套接字没有连接
ENOTSOCK            参数不是套接字描述符

参数vector为一个指向向量的指针,结构struct iovecd在文件
<sys/uio.h>中定义:
struct iovec {
	// 向量的缓冲区地址
	void *iov_base;
	// 向量缓冲区的大小,以字节为单位
	size_t iov_len;
};
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
writev()函数errno的值及含义
值
EAGAIN              套接字定义为非阻塞,而操作采用了阻塞方式,
                    或者定义的超时时间已经达到却没有接收到数据
EBADF               参数s不是合法描述符
ECONNREFUSED        远程主机不允许此操作
EFAULT              接收缓冲区不允许此操作
EINTR               接收到中断信号
EINVAL              参数iov_len超出了ssize_t类型的范围,或者count
                    参数小于0或者大于可允许最大值
ENOTCONN            套接字s表示流式套接字,此套接字没有连接
ENOTSOCK            参数不是套接字描述符

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

使用recvmsg()函数接收数据
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recvmsg(int s, struct msghdr *msg, int flags);
函数的返回值为成功接收到的字节数,为-1时错误发生,可以查看errno
获取错误码.当另一方使用正常方式关闭连接的时候返回值为0,如调用
close()函数关闭连接.
recvmsg()函数errno的值及含义
值                  含义
EAGAIN             套接字定义为非阻塞,而操作采用了阻塞方式,
                   或者定义的超时时间已经达到却没有接收到数据
EBADF              参数s不是合法描述符
ECONNREFUSED       远程主机不是合法描述符
EFAULT             接收缓冲区指针在此进程之外
EINTR              接收到中断信号
EINVAL             传递了不合法参数
ENOTCONN           套接字s表示流式套接字,此套接字没有连接
ENOTSOCK           参数不是套接字描述符
ENOMEM             没有足够内存

recvmsg()函数flags的值及含义(值可以采用按位或的复合值)
值
MSG_DONTWAIT       非阻塞操作,立刻返回,不等待
MSG_ERRQUEUE       错误消息从套接字错误队列接收
MSG_OOB
MSG_PEEK           查看数据,不进行数据缓冲区的清空
MSG_TRUNC          返回所有的数据,即使指定的缓冲区过小
MSG_WAITALL        函数会复制与用户指定的长度相等的数据.
                   如果内核中的当前数据不能满足要求,会一直
                   等待直到数据足够的时候才返回

地址结构msghdr
struct msghdr {
    // 可选地址
    void *msg_name;
    // 地址长度
    socklen_t msg_namelen;
    // 接收数据的数组
    struct iovec *msg_iov;
    // msg_iov中的元素数量
    size_t msg_iovlen;
    // ancillary data, see below
    void *msg_control;
    // ancillary data, see below
    socklen_t msg_controllen;
    // 接收消息的标志
    int msg_flags;
};
成员msg_name表示源地址,即为一个指向struct sockaddr的指针,
当套接字还没有连接的时候有效.
成员msg_namelen表示msg_name指向结构的长度.
成员msg_iov与函数readv()中的含义一致.
成员msg_iovlen表示msg_iov缓冲区的字节数.
成员msg_control指向缓冲区,根据msg_flags的值,会放入不同的值.
成员msg_controllen为msg_control指向缓冲区的大小.
成员msg_flags为操作的方式.
recv()函数通常用于TCP类型的套接字,UDP使用recvfrom()函数
接收数据,当然在数据报套接字绑定地址和端口后,也可以使用
recv()函数接收数据.

使用sendmsg()函数发送数据
#include <sys/uio.h>
ssize_t sendmsg(int s, const struct msghdr *msg, int flags);
函数sendmsg()可用于接收多个缓冲区数据.
函数sendmsg()向套接字描述符s中按照结构msg的设定写入数据,
其中操作方式由flags指定.
函数sendmsg()与recvmsg()相区别的地方在于sendmsg的操作方式
由flags参数设定,而recvmsg的操作方式由参数msg结构里的成员变量
msg_flags指定.

IO函数的比较
1.函数read()/write()和read()/writev()可以对所有的文件描述符使用;
  函数recv()/send(),recvfrom()/writeto()和recvmsg()/sendmsg()
  只能操作套接字描述符.
2.函数readv()/writeto()和recvmsg()/sendmsg()可以操作多个缓冲区,
  函数read()/write(),recv()/send()和recvfrom()/sendto()只能
  操作单个缓冲区.
3.函数recv()/send(),recvfrom()/sendto()和recvmsg()/sendmsg()
  具有可选标志.
4.函数recvfrom()/sendto()和recvmsg()/sendmsg()可以选择对方的IP地址.
5.函数recvmsg()/sendmsg()有可选择的控制信心,能进行高级操作.
Y标记的为具有此种属性.
名称                  任何描述符   只对套接字描述符    单个缓冲区   多个缓冲区   可选标志    可选对方地址  可选控制信息
read()/write()           Y                            Y
readv()/writev()         Y                                       Y
recv()/send()                          Y              Y                     Y
recvfrom()/writeto()                   Y              Y                     Y           Y
recvmsg()/sendmsg()                    Y                         Y          Y           Y           Y

IO模型
IO的方式有阻塞IO,非阻塞IO,IO复用,信号驱动,异步IO等.
阻塞IO模型
阻塞IO是最通用的IO类型,使用这种模型进行数据接收的时候,在数据没有到之前
程序会一直等待.例如对于函数recvfrom(),内核会一直阻塞该请求直到有数据
到来才返回.
非阻塞IO模型
当把套接字设置成非阻塞的IO,则对每次请求,内核都不会阻塞,会立即返回;
当没有数据的时候,会返回一个错误.例如对于函数recvfrom(),前几次
都没有数据返回,直到最后内核才向用户层的空间复制数据.
IO复用
使用IO复用模型可以在等待的时候加入超时的时间,当超时时间没有到达的时候
阻塞的情况一致,而当超时时间到达仍然没有数据接收到,系统会返回,不再等待.
select()函数按照一定的超时时间轮询,直到需要等待的套接字有数据到来,
利用recvfrom()函数,将数据复制到应用层.
信号驱动IO模型
信号驱动的IO在进程开始的时候注册一个信号处理的回调函数,进程继续执行,
当信号发生时,即有了IO的时间,这里即有数据到来,利用注册的回调函数将
到来的函数用recvfrom()接收到.
异步IO模型
异步IO与前面的信号驱动IO相似,其区别在于信号驱动IO当数据到来的时候,
使用信号通知注册的信号处理函数,而异步IO则在数据复制完成的时候才发送
信号通知注册的信号处理函数.

select()函数和pselect()函数
函数select()和函数pselect()用于IO复用,它们监视多个文件描述符不同.
使用select()函数可以先对需要操作的文件描述符进行查询,查看是否目标
文件描述符可以进行读,写或者错误操作,然后当文件描述符满足操作的条件
的时候才进行真正的IO操作.
select()函数简介
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
int select(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, struct timeval *timeout);
参数说明:
nfds:       一个整形的变量,它比所有文件描述符集合中的文件描述符的
            最大值大1.使用select的时候必须计算最大值的文件描述
            的值,将值通过nfds传入.
readfds:    这个文件描述符集合监视文件集中的任何文件是否有数据可读,
            当select()函数返回的时候,readfds将清除其中不可读的
            文件描述符,只留下可读的文件描述符,即可以被recv(),read()
            等进行读数据的操作.
writefds:   这个文件描述符集合监视文件集中的任何文件是否有数据可写,
            当select()函数返回的时候,readfds将清除其中的不可写
            的文件描述符,只留下可写的文件描述符,即可以被send(),
            write()函数等进行写数据的操作.
exceptfds:  这个文件集将监视文件集中的任何文件是否发生错误,其实,它可
            用于其他的用途.例如,监视带外数据OOB,带外数据使用MSG_OOB
            标志发送到套接字上.当select()函数返回的时候,readfds
            将清除其中的其他文件描述符,只留下可读OOB数据.
timeout:    设置有select()所监视的文件集合中的事件没有发生时,最长的
            等待时间,当超过此时间时,函数会返回.当超时时间为NULL时,
            表示阻塞操作,会一直等待,直到某个监视的文件集中的某个文件
            描述符符合返回条件.当timeout的值为0时,select会立即返回.
sigmask:    信号.









































*/

/////////////////////////////声明/////////////////////////////

void test_open(void);

void test_read(void);

void test_write(void);

void test_lseek(void);

int test_start_server(void);

void test_pthread(void);

void test_class(void);

void LinuxSocket::studyHard() {

    test_start_server();

}

/////////////////////////////实现/////////////////////////////

void sig_process(int signo) {
    printf("Catch a exit signal.signo: %d\n", signo);

    exit(EXIT_SUCCESS);
}

// 关闭套接字文件描述符
void close_local_server_sock_fd(int &local_server_sock_fd) {
    if (local_server_sock_fd != -1) {
        close(local_server_sock_fd);
        local_server_sock_fd = -1;
    }
}

void close_remote_client_sock_fd(int &remote_client_sock_fd) {
    if (remote_client_sock_fd != -1) {
        close(remote_client_sock_fd);
        remote_client_sock_fd = -1;
    }
}

// 下面是处理客户端消息的几种不同方式

#define DATA_BUFFER 1024

// 服务器端对客户端的处理
// sc为客户端的socket描述符
void process_client_with_read_write(int remote_client_sock_fd) {
    ssize_t size = 0;
    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        memset(buffer, 0, sizeof(buffer));
        size = read(remote_client_sock_fd, buffer, DATA_BUFFER);
        // 没有数据
        if (size == 0) {
            return;
        } else {
            // 输出读到的内容,这个函数的作用就是写给谁,这个谁就是“1”
            // “1”代表标准输出
            write(1, buffer, size);
        }
        // 构建响应字符,为接收到客户端字节的数量
        sprintf(buffer, "%d bytes altogether\n", size);
        // 发给客户端
        write(remote_client_sock_fd, buffer, strlen(buffer) + 1);
    }
}

/***
 这种方式比较好用,且便于理解
 */
void process_client_with_recv_send(int remote_client_sock_fd) {
    ssize_t recv_size = -1;
    ssize_t send_size = -1;

    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        memset(buffer, 0, sizeof(buffer));
        recv_size = recv(remote_client_sock_fd, buffer, DATA_BUFFER, 0);
        if (recv_size == -1) {
            fprintf(stderr, "server recv error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            return;
        }
        if (recv_size == 0) {
            // 跟客户端断开连接时
            printf("server没有接收到数据\n");
            close_remote_client_sock_fd(remote_client_sock_fd);
            // 不能少
            return;
        }
        // 显示到屏幕
        write(1, buffer, recv_size);

        memset(buffer, 0, sizeof(buffer));
        // 构建响应字符,为接收到客户端字节的数量
        //sprintf(buffer, "服务端已经收到客户端发送过去的 %d 个字节.\n", recv_size);
        sprintf(buffer, "服务端已经收到客户端发送过去的 %d 个字节\n", recv_size);
        // 发给客户端
        send_size = send(remote_client_sock_fd, buffer, strlen(buffer) + 1, 0);
        if (send_size == -1) {
            fprintf(stderr, "server send error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            return;
        }
        if (send_size == 0) {
            printf("server没有发送数据\n");
            /*return;*/
        }
        printf("server send size: %ld\n", send_size);
    }
}

/***
 处理过程利用3个向量来完成数据的接收和响应工作.
 先申请3个向量,每个向量的大小为10个字符.
 利用一个公共的30个字节大小的缓冲区buffer
 来初始化3个向量的地址缓冲区,将每个向量的向量长度
 设置为10.调用readv()来读取客户端的数据后,
 利用3个缓冲区构建响应信息,最后将响应信息发送给服务器端.

 不要使用下面的方法接收数据和发送数据
 */
static struct iovec *vs = NULL, *vc = NULL;

void process_client_with_readv_writev(int remote_client_sock_fd) {
    char buffer[30];
    char read_buffer[DATA_BUFFER];
    ssize_t readv_size = 0;
    struct iovec *v = (struct iovec *) malloc(3 * sizeof(struct iovec));
    if (!v) {
        perror("Not enough memory\n");
        return;
    }
    vs = v;
    v[0].iov_base = buffer;
    v[1].iov_base = buffer + 10;
    v[2].iov_base = buffer + 20;
    v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
    int i = 0;
    for (;;) {
        readv_size = readv(remote_client_sock_fd, v, 3);
        if (readv_size == -1) {
            fprintf(stderr, "server recv error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            break;
        }
        if (readv_size == 0) {
            // 跟客户端断开连接时
            printf("server没有接收到数据\n");
            close_remote_client_sock_fd(remote_client_sock_fd);
            // 不能少
            break;
        }
        // 读取到的内容还不知道怎么输出,因此暂时不要用这种方法
        // 来读取数据和发送数据
        // 下面的输出方法不好
        for (i = 0; i < 3; i++) {
            if (v[i].iov_len > 0) {
                write(1, v[i].iov_base, v[i].iov_len);
            }
        }

        sprintf((char *) v[0].iov_base, "%d ", readv_size);
        sprintf((char *) v[1].iov_base, "bytes alt");
        sprintf((char *) v[2].iov_base, "ogether\n");
        v[0].iov_len = strlen((const char *) v[0].iov_base);
        v[1].iov_len = strlen((const char *) v[1].iov_base);
        v[2].iov_len = strlen((const char *) v[2].iov_base);
        writev(remote_client_sock_fd, v, 3);
    }
    free(v);
}

void process_client_with_recvmsg_sendmsg(int remote_client_sock_fd) {
    printf("process_client_with_recvmsg_sendmsg remote_client_sock_fd: %d\n",
           remote_client_sock_fd);

    // 向量的缓冲区
    char buffer[30];
    ssize_t recvmsg_size = 0;
    // 消息结构
    struct msghdr msg;
    // 申请3个向量
    struct iovec *v =
            (struct iovec *) malloc(3 * sizeof(struct iovec));
    if (!v) {
        perror("Not enough memory\n");
        return;
    }
    // 挂接全局变量,便于释放管理
    vs = v;

    // 没有名字域
    msg.msg_name = NULL;
    // 名字域长度
    msg.msg_namelen = 0;
    // 没有控制域
    msg.msg_control = NULL;
    // 控制域长度
    msg.msg_controllen = 0;
    // 挂接向量指针
    msg.msg_iov = v;
    // 接收缓冲区长度为30
    msg.msg_iovlen = 30;
    // 无特殊操作
    msg.msg_flags = 0;
    // 每个向量分配10个字节的空间
    // 0~9
    v[0].iov_base = buffer;
    // 10~19
    v[1].iov_base = buffer + 10;
    // 20~29
    v[2].iov_base = buffer + 20;
    // 初始化长度
    v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;

    int i = 0;
    for (;;) {
        memset(msg.msg_iov[0].iov_base, 0, msg.msg_iov[0].iov_len);
        recvmsg_size = recvmsg(remote_client_sock_fd, &msg, 0);
        if (recvmsg_size == -1) {
            fprintf(stderr, "server recv error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            break;
        }
        if (recvmsg_size == 0) {
            // 跟客户端断开连接时
            printf("server没有接收到数据\n");
            close_remote_client_sock_fd(remote_client_sock_fd);
            // 不能少
            break;
        }
        printf("server接收到 %d 个字节\n", recvmsg_size);
        printf("server接收到的内容为:\n");
        /*for (i = 0; i < 3; i++) {
            if (v[i].iov_len > 0) {
                write(1, v[i].iov_base, v[i].iov_len);
            }
        }*/
        //write(1, v[0].iov_base, v[0].iov_len);
        write(1, msg.msg_iov[0].iov_base, strlen((const char *) msg.msg_iov[0].iov_base));
        printf("\n");

        // 给客户端发送数据
        // 构建响应字符,为接收到客户端字节的数量,分别放到3个缓冲区中
        /*sprintf((char *) v[0].iov_base, "%d ", recvmsg_size);
        sprintf((char *) v[1].iov_base, "bytes alt");
        sprintf((char *) v[2].iov_base, "ogether\n");
        v[0].iov_len = strlen((const char *) v[0].iov_base);
        v[1].iov_len = strlen((const char *) v[1].iov_base);
        v[2].iov_len = strlen((const char *) v[2].iov_base);
        sendmsg(remote_client_sock_fd, &msg, 0);*/
    }
}

// 下面是主程序

/***
1.bind error: Address already in use
服务器设置的端口已被占用
2.accept error: Bad file descriptor
 */
/***
 想法:
 关键函数的参数用全局静态变量代替
 */
int test_start_server(void) {
    // local_server_sock_fd为服务端的socket描述符
    // remote_client_sock为客户端的socket描述符
    int local_server_sock_fd = -1, remote_client_sock_fd = -1;
    // 服务端地址结构
    // 客户端地址结构
    struct sockaddr_in server_addr, client_addr;
    // 返回值
    int err = -1;

    // 注册信号
    signal(SIGINT, sig_process);
    signal(SIGPIPE, sig_process);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

    // 建立一个流式套接字
    local_server_sock_fd = socket(AF_INET,
                                  SOCK_STREAM,
                                  0);
    if (local_server_sock_fd == -1) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // 解决在close之后会有一个WAIT_TIME，导致bind失败的问题
    int val = -1;
    err = setsockopt(local_server_sock_fd,
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     (void *) &val,
                     sizeof(int));
    if (err == -1) {
        fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
        close_local_server_sock_fd(local_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    // 设置服务端地址
    // 清零
    /*bzero(reinterpret_cast<char *>(&server_addr),
          sizeof(server_addr));*/
    bzero(&server_addr, sizeof(server_addr));
    // 协议族
    server_addr.sin_family = AF_INET;
    // 本地地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 服务器端口
    server_addr.sin_port = htons(PORT);

    // 绑定地址结构到套接字描述符
    err = bind(local_server_sock_fd,
               (struct sockaddr *) &server_addr,
               sizeof(server_addr));
    if (err == -1) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        close_local_server_sock_fd(local_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    // 设置侦听
    err = listen(local_server_sock_fd, BACKLOG);
    if (err == -1) {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        close_local_server_sock_fd(local_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    // 主循环过程
    for (;;) {
        socklen_t addrlen = sizeof(struct sockaddr);
        // 接收客户端连接
        remote_client_sock_fd = accept(local_server_sock_fd,
                                       (struct sockaddr *) &client_addr,
                                       &addrlen);
        if (remote_client_sock_fd == -1) {
            fprintf(stderr, "accept error: %s ss: %d\n", strerror(errno), local_server_sock_fd);
            // 当客户端退出时,这里会报错,但是服务端并没有停止
            exit(EXIT_FAILURE);
        }

        printf("server local_server_sock_fd: %d\n", local_server_sock_fd);
        printf("server local_server_sock_fd: %d\n", remote_client_sock_fd);

        // 建立一个新的进程处理到来的连接
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "fork error: %s\n", strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            continue;
        }

        if (pid == 0) {
            printf("子进程\n");
            /***
             子进程关闭local_server_sock处理任务,
             使其回到TIME_WAIT状态值.
             */
            close(local_server_sock_fd);
            // process_client_with_read_write(remote_client_sock_fd);
            // process_client_with_recv_send(remote_client_sock_fd);
            // process_client_with_readv_writev(remote_client_sock_fd);
            process_client_with_recvmsg_sendmsg(remote_client_sock_fd);
        } else {
            printf("父进程.新创建的进程id: %d\n", pid);
            /***
             在父进程中关闭连接的套接字描述符,
             只是把remote_client_sock的引用数减少1,
             在子进程中还在使用remote_client_sock.
             */
            close(remote_client_sock_fd);
        }
    }
}

/////////////////////////线程/////////////////////////

// 下面是线程的内容
void *say_hello_thread(void *args) {
    sleep(3);
    printf("Hello Runoob pid: %u\n", pthread_self());
}

void test_pthread(void) {
    pthread_t p_tids[PTHREADS_NUM];
    // 定义一个属性
    pthread_attr_t attr;
    // 初始化属性值,均设为默认值
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    for (int i = 0; i < PTHREADS_NUM; ++i) {
        printf("&p_tids\[%d\] = %p\n", i, &p_tids[i]);
        int ret = pthread_create(&p_tids[i], &attr, say_hello_thread, NULL);
        if (ret != 0) {
            printf("pthread_create error: error_code = %d\n", ret);
        }
    }

    printf("pthread_join\n");

    for (int i = 0; i < PTHREADS_NUM; ++i) {
        //say_hello_thread函数里面的代码执行完了pthread_join才会返回结果
        int joinResult = pthread_join(p_tids[i], NULL);
        printf("joinResult = %d, %p\n", joinResult, &p_tids[i]);
    }

    //使用pthread_exit的话其后的代码就不会再执行了
    //pthread_exit(NULL);

    printf("Game Over\n");
}

/***
线程的知识点：

cat /usr/include/asm-generic/errno.h
man pthread_create

gcc -o test_thread test_thread.c -lpthread

linux多线程
#include <pthread.h>
Linux系统下的多线程遵循POSIX标准,叫做pthread.
编写Linux下的线程需要包含头文件pthread.h,在生成
可执行文件的时候需要链接库
libpthread.a或者libpthread.so.

                线程                  进程
标识符类型      pthread_t            pid_t
获取id         pthread_self()       getpid()
创建           pthread_create()     fork()


typedef unsigned long int pthread_t;线程标识符

extern int pthread_create __P (
    // 参数一: pthread_t指针,因此传递的参数是一个地址
    pthread_t *__thread,
    // 参数二: 线程属性
    __const pthread_attr_t *__attr,
    // 参数三: 函数指针
    void *(*__start_routine)(void *),
    // 参数四: 函数参数
    void *__arg);
参数说明:
第一个参数为指向线程标识符的指针.
第二个参数用来设置线程属性,
    为NULL时将生成默认属性的线程
第三个参数是线程运行函数的起始地址(意思就是执行线程代码的函数),
第四个参数是运行函数的参数,不需要传递参数时赋为NULL.
返回值说明:
当创建线程成功时,函数返回0,
若不为0则说明创建线程失败,常见的错误返回代码为EAGAIN和EINVAL.
前者表示系统限制创建新的线程,例如线程数目过多了;
后者表示第二个参数代表的线程属性值非法.

extern int pthread_join __P ((
     // 参数一: 线程id
     pthread_t __th,
     // 参数二:
     void **__thread_return));
第一个参数为被等待的线程标识符,即pthread_create()创建成功的值.
第二个参数为一个用户定义的指针,
    它可以用来存储被等待线程的返回值.
这个函数是一个线程阻塞的函数,
调用它的函数将一直等待到被等待的线程结束为止.
当函数返回时,被等待线程的资源被收回.

extern void pthread_exit __P ((void *__retval))
        __attribute__ ((__noreturn__));
唯一的参数是函数的返回代码,
只要pthread_join中的第二个参数thread_return不是NULL,
这个值将被传递给thread_return.
最后要说明的是,一个线程不能被多个线程等待,
否则第一个接收到信号的线程成功返回,
其余调用pthread_join的线程则返回错误代码ESRCH.

总结:
pthread_exit等各个线程退出后,进程才结束;
否则main进程强制结束了,线程可能还没反应过来,因此线程不会执行.
pthread_join后面的代码只有待线程里的代码全部执行完了才会执行.
pthread_exit后面的代码不会被执行.
pthread_join与pthread_exit只需要用一个,
一起使用也没有关系,只是pthread_exit后面不会被执行.


其他说明:
创建线程成功后,新创建的线程则运行参数三和参数四确定的函数,
原来的线程则继续运行下一行代码.

线程的属性
属性结构(/usr/include/pthread.h)
pthread_attr_t
属性值不能直接设置,须使用相关函数进行操作,
初始化的函数为
pthread_attr_init
这个函数必须在pthread_create函数之前调用.
属性对象主要包括是否绑定、是否分离、堆栈地址、堆栈大小、优先级.
默认的属性为非绑定、非分离、缺省1M的堆栈、
与父进程同样级别的优先级.

关于线程的绑定,牵涉到另外一个概念：
轻进程(LWP：Light Weight Process).
轻进程可以理解为内核线程,它位于用户层和系统层之间.
系统对线程资源的分配、对线程的控制是通过轻进程来实现的,
一个轻进程可以控制一个或多个线程.
默认状况下,启动多少轻进程、哪些轻进程来控制哪些线程是由
系统来控制的,这种状况即称为非绑定的.
绑定状况下,则顾名思义,即某个线程固定的"绑"在一个轻进程之上.
被绑定的线程具有较高的响应速度,这是因为CPU时间片的调度是
面向轻进程的,
绑定的线程可以保证在需要的时候它总有一个轻进程可用.
通过设置被绑定的轻进程的优先级和调度级可以使得
绑定的线程满足诸如实时反应之类的要求.
设置线程绑定状态的函数为
pthread_attr_setscope,
它有两个参数,
第一个是指向属性结构的指针,
第二个是绑定类型,它有两个取值：
PTHREAD_SCOPE_SYSTEM(绑定的)
PTHREAD_SCOPE_PROCESS(非绑定的)

设置线程是否分离
线程的分离状态决定一个线程以什么样的方式来终止自己.
线程的默认属性即为非分离状态,这种情况下,
原有的线程等待创建的线程结束.
只有当pthread_join()函数返回时,
创建的线程才算终止,才能释放自己占用的系统资源.
而分离线程不是这样子的,它没有被其他的线程所等待,
自己运行结束了,线程也就终止了,马上释放系统资源.
程序员应该根据自己的需要,选择适当的分离状态.
设置线程分离状态的函数为
pthread_attr_setdetachstate(
     pthread_attr_t *attr,
     int detachstate);
第二个参数可选为
PTHREAD_CREATE_DETACHED(分离线程)
PTHREAD_CREATE_JOINABLE(非分离线程)
这里要注意的一点是,如果设置一个线程为分离线程,
而这个线程运行又非常快,
它很可能在pthread_create函数返回之前就终止了,
它终止以后就可能将线程号和系统资源移交给其他的线程使用,
这样调用pthread_create的线程就得到了错误的线程号.
要避免这种情况可以采取一定的同步措施,最简单的方法之一是
可以在被创建的线程里调用pthread_cond_timewait函数,
让这个线程等待一会儿,留出足够的时间让函数pthread_create返回.
设置一段等待时间,是在多线程编程里常用的方法.
但是注意不要使用诸如wait()之类的函数,
它们是使整个进程睡眠,并不能解决线程同步的问题.
另外一个可能常用的属性是线程的优先级,
它存放在结构sched_param中.
用函数pthread_attr_getschedparam
和函数pthread_attr_setschedparam进行存放,
一般说来,我们总是先取优先级,对取得的值修改后再存放回去.

互斥锁
创建pthread_mutex_init;
销毁pthread_mutex_destroy;
加锁pthread_mutex_lock;
解锁pthread_mutex_unlock.

条件锁
创建pthread_cond_init;
销毁pthread_cond_destroy;
触发pthread_cond_signal;
广播pthread_cond_broadcast S;
等待pthread_cond_wait
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
    // 子类覆盖了父类中的fun1(int ,int ),当编译器看到这个函数时,首先在Derived的作用域内找,
    // 并且找到了,但是参数不匹配,这时候编译器并不会继续扩大搜索范围,而是报错.
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
1.析构函数如果在类中声明了,那么必须要定义,也就是至少要有空实现.
2.如果没有声明跟定义拷贝构造函数的话,那么执行Test test2(test);
这样的代码时,构造函数不会被执行,但是析构函数会被执行.
3.声明拷贝构造函数时,其参数必须是对象的引用.
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

    // 属性有了默认值,所以可以用这种方式创建对象
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
     new如果开辟内存不成功的话,会抛出异常；
     没有抛出的话,就表示一定会开辟成功.
     创建对象数组时,首先必须要求有默认的构造函数.
     */
    int *q = new int[SIZE];
    for (int j = 0; j < SIZE; ++j) {
        q[j] = j + 1;
    }
    delete[]q;

    int *q_ = new int(SIZE);
    delete q_;
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

    // 打开文件,如果文件存在则返回-1
    fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    if (fd == -1) {
        printf("File exist, reopen it\n");
        // 重新打开
        fd = open(filename, O_RDWR);
        // MyLog::i(TAG, to_string(fd), NEEDLOG);
    } else {
        // 文件不存在,成功创建并打开
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
        // 如果打开的文件即时被关闭了,那么这个文件描述符可以重复使用
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



































