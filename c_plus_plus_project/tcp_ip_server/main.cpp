#include "linuxhttp/LinuxSocket.h"
#include "signal/HandleSignal.h"

void handleSigIntSignal(int sign);

void handleSigPipeSignal(int sign);

int howToCreateChildProcess();

int test();

int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("At %d parameter: %s\n", (i + 1), argv[i]);
    }
    printf("The run result:\n");
    printf("---------------------------------------------------\n");

//    signal(SIGINT, handleSigIntSignal);
//    signal(SIGPIPE, handleSigPipeSignal);

//    howToCreateChildProcess();

//    LinuxSocket linuxSocket;
//    linuxSocket.studyHard();

    test();

    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}

int test() {



}
// http://pic104.nipic.com/file/20160715/6171480_185807154956_2.jpg

// 不要删
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

/***
子线程
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

第三种使用线程方法:
用lambda表达式.
auto testThread = [] {
    cout << "我是子线程,现在正在执行任务..." << endl;
};
thread childThread(testThread);
if (childThread.joinable()) {
    childThread.join();
}


 */
