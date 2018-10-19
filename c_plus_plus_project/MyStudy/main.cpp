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
int main2(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    for (int j = 0; j < argc; j++) {
        printf("%s\n", argv[j]);
    }
    printf("The run result:\n");
    printf("------------------------------------------\n");

//    FILE *fp;
//    fpos_t position;
//    fp = fopen("file.txt", "w+");
//    fgetpos(fp, &position);
//    fprintf(stdout, "position = %d\n", position);
//    fputs("HelloWorld", fp);
//    fgetpos(fp, &position);
//    fprintf(stdout, "position = %d\n", position);
//    fsetpos(fp, &position);
//    fprintf(stdout, "position = %d\n", position);
//    fputs("这将覆盖之前的内容", fp);
//    fgetpos(fp, &position);
//    fprintf(stdout, "position = %d\n", position);
//    fclose(fp);

    HandleAndroidString handleAndroidString;
//    handleAndroidString.doSomething("/root/temp_dir/merge_string");
    handleAndroidString.doSomething("/root/temp_dir/merge_string/temp_cache");

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}
/***





*/