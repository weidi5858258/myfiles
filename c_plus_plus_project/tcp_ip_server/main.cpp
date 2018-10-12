#include <signal.h>
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

int test() {
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

}
