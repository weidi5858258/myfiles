#ifdef WIN32

#include "MyHead.h"
#include "Test.h"
#include "Method.h"
#include "HandleAndroidString.h"

#else

#include "./include/MyHead.h"
#include "./include/Test.h"
#include "./include/Method.h"
#include "HandleAndroidString.h"

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

    FILE *fp = nullptr;
    char c;
    fp = fopen("file.txt", "w");
    c = fgetc(fp);
    if (ferror(fp)) {
        printf("1读取文件: file.txt时发生错误\n");
    }
    clearerr(fp);
    if (ferror(fp)) {
        printf("2读取文件: file.txt时发生错误\n");
    }
    fclose(fp);


    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}
/***





*/