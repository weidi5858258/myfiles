#include "LinuxSocket.h"

/***
 Process finished with exit code 130 (interrupted by signal 2: SIGINT)
 */
int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    printf("The run result:\n");
    printf("---------------------------------------------------\n");
    LinuxSocket linuxSocket;
    linuxSocket.studyHard();
    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}