#include "linuxhttp/LinuxSocket.h"
#include "signal/HandleSignal.h"

void handleSigIntSignal(int sign);

void handleSigPipeSignal(int sign);

int howToCreateChildProcess();

// http://pic104.nipic.com/file/20160715/6171480_185807154956_2.jpg

// 不要删(怎样创建子进程)
int howToCreateChildProcess() {
    int x = 10;
    pid_t pid;
    //执行一次,返回两次
    pid = fork();
    if (pid == -1) {
        printf("进程创建失败!\n");
        return -1;
    } else if (pid == 0) {
        //后执行(在这里执行的是子进程中的代码)
        printf("fork返回值: %d, 子进程ID: %d, 父进程ID: %d\n",
               pid, getpid(), getppid());
        printf("x2 = %d\n", x);
    } else {
        //先执行(成功时pid为创建的子进程号)
        //在这里返回的值就是被创建的子进程号
        printf("fork返回值: %d, 当前进程ID: %d, 父进程ID: %d\n",
               pid, getpid(), getppid());
        x = 20;
        printf("x1 = %d\n", x);
    }
}

/////////////////////////线程/////////////////////////

#define PTHREADS_NUM 5

// 下面是线程的内容
void *say_hello_thread(void *args) {
    sleep(3);
    printf("Hello Runoob pid: %u\n", pthread_self());
}

void test_pthread(void) {
    pthread_t p_tids[PTHREADS_NUM];
    // 定义线程属性
    pthread_attr_t thread_attr;
    // 初始化属性值,均设为默认值
    pthread_attr_init(&thread_attr);
    pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
    sched_param param;
    pthread_attr_getschedparam(&thread_attr, &param);
    param.sched_priority = 20;
    pthread_attr_setschedparam(&thread_attr, &param);
    for (int i = 0; i < PTHREADS_NUM; ++i) {
        printf("&p_tids\[%d\] = %p\n", i, &p_tids[i]);
        // 创建线程
        int ret = pthread_create(&p_tids[i], &thread_attr, say_hello_thread, NULL);
        if (ret != 0) {
            printf("pthread_create error: error_code = %d\n", ret);
        }
    }

    printf("pthread_join\n");

    for (int i = 0; i < PTHREADS_NUM; ++i) {
        // 线程等待
        //say_hello_thread函数里面的代码执行完了pthread_join才会返回结果
        int joinResult = pthread_join(p_tids[i], NULL);
        printf("joinResult = %d, %p\n", joinResult, &p_tids[i]);
    }

    //使用pthread_exit的话其后的代码就不会再执行了
    //pthread_exit(NULL);

    printf("Game Over\n");
}

/***
两种:
一个是#include <thread>
一个是#include <pthread.h>

子线程的使用
#include <thread>
线程类thread
主要方法:
join()
detach()
joinable()
使用说明:
join()让主线程等待子线程的完成
detach()让主线程与子线程分离,子线程就成了后台线程.
如果主线程退出了,子线程就不再执行.
joinable()条件判断.
返回true时,线程对象可以join()或者detach();
返回false时,线程对象不能join()或者detach().
如果调用了join()后就不能再调用join()或者detach(),
如果调用了detach()后也不能再调用join()或者detach().
因此最好的做法是调用join()或者detach()之前,
先调用joinable()方法判断一下,返回true时再调用.

第一种使用线程方法:
1.
void testThread() {
    cout << "我是子线程,现在正在执行任务..." << endl;
}
2.
thread childThread(testThread);
if (childThread.joinable()) {
    childThread.join();
}

第二种使用线程方法:
用lambda表达式.
auto testThread = [] {
    cout << "我是子线程,现在正在执行任务..." << endl;
};
thread childThread(testThread);
if (childThread.joinable()) {
    childThread.join();
}

第三种使用线程方法:
1.创建类,void operator()() {...}这个方法少不了
class MyThread {
public:
    // 如果没有这个方法,那么使用MyThread类创建的对象就不是可调用对象.
    // 如果不是可调用对象,那么这个类就是普通类,不能作为线程对象的参数.
    void operator()() {
        cout << "我是子线程,现在正在执行任务..." << endl;
    }
};
2.
MyThread myThread;
// thread类对象参数必须是可调用对象.
thread childThread(myThread);
if (childThread.joinable()) {
    childThread.join();
}


 线程的知识点:

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
属性对象主要包括是否绑定,是否分离,堆栈地址,堆栈大小,优先级.
默认的属性为非绑定,非分离,缺省1M的堆栈,
与父进程同样级别的优先级.

关于线程的绑定,牵涉到另外一个概念:
轻进程(LWP:Light Weight Process).
轻进程可以理解为内核线程,它位于用户层和系统层之间.
系统对线程资源的分配,对线程的控制是通过轻进程来实现的,
一个轻进程可以控制一个或多个线程.
默认状况下,启动多少轻进程,哪些轻进程来控制哪些线程是由
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
第二个是绑定类型,它有两个取值:
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
     new如果开辟内存不成功的话,会抛出异常;
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

int test() {

}

int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("At %d parameter: %s\n", (i + 1), argv[i]);
    }
    printf("The run result:\n");
    printf("---------------------------------------------------\n");

    signal(SIGINT, handleSigIntSignal);
    signal(SIGPIPE, handleSigPipeSignal);

//    howToCreateChildProcess();

//    LinuxSocket linuxSocket;
//    linuxSocket.studyHard();

    test();

    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}