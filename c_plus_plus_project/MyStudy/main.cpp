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

int test(int argc, char **argv);

/***
@param argc 参数至少有一个,因为第一个参数就是本身的可执行文件
@param argv
@return
Clion 输出乱码问题
File -> Settings -> Default Settings -> Editor -> File Encodings:
Global Encoding: UTF-8
Project Encoding : UTF-8
Default encoding for properties files: UTF-8
然后还有关键一步骤：在程程序左下角修改utf-8为GBK
 */
//int main(int argc, char *argv[]) {
int main(int argc, char **argv) {
    printf("\n");
    printf("argc = %d\n", argc);
    for (int j = 0; j < argc; j++) {
        printf("argv[%d]: %s\n", j, argv[j]);
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

//    HandleAndroidString handleAndroidString;
//    handleAndroidString.doSomething("/root/temp_dir/merge_string");
//    handleAndroidString.doSomething("/root/temp_dir/merge_string/temp_cache");

    test(argc, argv);

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}

/***
类型	输出	例子
d或i	带符号十进制整形	392
u	无符号十进制整形	7235
o	无符号八进制数	610
x	无符号十六进制整形	7fa
X	无符号十六进制整形（大写）	7FA
f	十进制浮点数，小写	392.65
F	十进制浮点数，大写	392.65
e	科学计数法（尾数/指数），小写	3.9265e+2
E	科学计数法（尾数/指数），大写	3.9265E+2
g	使用最短的表示: %e 或 %f	392.65
G	使用最短的表示: %E 或 %F	392.65
a	十六进制浮点数，小写	-0xc.90fep-2
A	十六进制浮点数，大写	-0XC.90FEP-2
c	字符	a
s	字符串	sample
p	指针地址	b8000000
n	不打印。相应的参数必须是指向带符号整形的指针，到目前为止写入的字符数将被存储在指向的位置。
%	跟在一个%字符后面的另一个%将会写入一个%	%




*/

/***
 1.
 创建文件夹(picture,audio,video)
 2.
 开始抓取
 */

int test(int argc, char **argv) {


    return 0;
}