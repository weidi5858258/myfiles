#include <signal.h>
#include "linuxhttp/LinuxSocket.h"
#include "signal/HandleSignal.h"

void handleSigIntSignal(int sign);

void handleSigPipeSignal(int sign);

int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    printf("The run result:\n");
    printf("---------------------------------------------------\n");
    signal(SIGINT, handleSigIntSignal);
    signal(SIGPIPE, handleSigPipeSignal);
    // test_grammar();
//    LinuxSocket linuxSocket;
//    linuxSocket.studyHard();

    pid_t pid;
    //执行一次,返回两次
    pid = fork();
    if (pid == -1) {
        printf("进程创建失败!\n");
        return -1;
    } else if (pid == 0) {
        //后执行(在这里执行的是子进程中的代码)
        printf("子进程,fork返回值: %d, 子进程ID: %d, 父进程ID: %d\n",
               pid, getpid(), getppid());
    } else {
        //先执行(成功时pid为创建的子进程号)
        printf("父进程,fork返回值: %d, 当前进程ID: %d, 父进程ID: %d\n",
               pid, getpid(), getppid());
    }

    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}