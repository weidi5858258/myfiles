// #include "wd_string.h"
// #include "wd_opencv.h"
// #include "socket.h"
#include "wd_grammar.h"
#include "mylog/MyLog.h"
#include "method/Method.h"
#include "linuxhttp/LinuxSocket.h"

int main(int argc, char *argv[]) {
    // set log
    MyLog::setNeedLog(true);
    MyLog::setSeparator(" -----> ");

    printf("\n");
    printf("argc = %d\n", argc);
    printf("The run result:\n");
    printf("---------------------------------------------------\n");
    // test_grammar();
    LinuxSocket linuxSocket;
    linuxSocket.studyHard();
    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}