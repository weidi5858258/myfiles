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
