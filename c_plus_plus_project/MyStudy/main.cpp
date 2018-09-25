#ifdef WIN32

#include "MyHead.h"
#include "Test.h"
#include "Method.h"
#include "HandleAndroidString.h"

#else

#include "./include/MyHead.h"
#include "./include/Test.h"
#include "./include/Method.h"
#include "./include/HandleAndroidString.h"

#endif

/***
 * @param argc 参数至少有一个,因为第一个参数就是本身的可执行文件
 * @param argv
 * @return
Clion 输出乱码问题
File -> Settings -> Default Settings -> Editor -> File Encodings:
Global Encoding: UTF-8
Project Encoding : UTF-8
Default encoding for properties files: UTF-8
然后还有关键一步骤：在程程序左下角修改utf-8为GBK
 */
int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    for (int j = 0; j < argc; j++) {
        printf("%s\n", argv[j]);
    }
    printf("The run result:\n");
    printf("------------------------------------------\n");

//    HandleAndroidString handleAndroidString;
//    handleAndroidString.doSomething("/root/temp_dir/merge_string");

    if (argc >= 2) {
        string TEMP_CACHE = argv[1];
        HandleAndroidString handleAndroidString;
//        handleAndroidString.doSomething(TEMP_CACHE);
        handleAndroidString.doSomething("/root/temp_dir/merge_string");
    }

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}
