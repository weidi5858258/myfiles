#include <signal.h>
#include "linuxhttp/LinuxSocket.h"
#include "signal/HandleSignal.h"

void handleSigIntSignal(int sign);

void handleSigPipeSignal(int sign);

int howToCreateChildProcess();

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



    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}

int howToCreateChildProcess() {
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
    } else {
        //先执行(成功时pid为创建的子进程号)
        //在这里返回的值就是被创建的子进程号
        printf("fork返回值: %d, 当前进程ID: %d, 父进程ID: %d\n",
               pid, getpid(), getppid());
    }
}
