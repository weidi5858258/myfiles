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
    LinuxSocket linuxSocket;
    linuxSocket.studyHard();
    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}