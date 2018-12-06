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



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!
Someone could be eavesdropping on you right now (man-in-the-middle attack)!
It is also possible that a host key has just been changed.
The fingerprint for the ECDSA key sent by the remote host is
SHA256:cNjKthrfKoZcezU/vdermNGD0FEnTjECB0VuyGMFsgs.
Please contact your system administrator.
Add correct host key in /root/.ssh/known_hosts to get rid of this message.
Offending ECDSA key in /root/.ssh/known_hosts:1
  remove with:
  ssh-keygen -f "/root/.ssh/known_hosts" -R "[localhost]:2222"
ECDSA host key for [localhost]:2222 has changed and you have requested strict checking.
Host key verification failed.
*/

/***
 1.
 创建文件夹(picture,audio,video)
 2.
 开始抓取
 */

int test(int argc, char **argv) {
    int a = 100;
    int *p1 = &a;
    int **p2 = &p1;
/***
 总结:
  p1用 %s 打印就是字符串内容
  p1用 %p 打印就是字符串内容的地址
  p2用 %s 打印是个随机值
  p2用 %p 打印是p1自身的地址
 *p2用 %s 打印就是字符串内容
 *p2用 %p 打印就是字符串内容的地址

 *p1用 %s 打印程序会crash
**p2用 %s 打印程序会crash

下面的结果为:
test() p1  %s: Hello World
test() p1  %p: 0x41b723
test() *p2 %s: Hello World
test() *p2 %p: 0x41b723
test() &p1 %p: 0x7fff2acddaf0
test() p2  %p: 0x7fff2acddaf0
test() &p2 %p: 0x7fff2acddaf8
test() p1[0] %p: 0x48
test() *p1   %p: 0x48
test() **p2  %p: 0x48
 */
    fprintf(stdout, "test() *p1 %%d: %d\n", *p1);// 指向字符串内容
    fprintf(stdout, "test() **p2 %%d: %d\n", **p2);// 指向字符串内容
    fprintf(stdout, "test() *p1 %%p: %p\n", *p1);// 指向字符串内容的地址
    fprintf(stdout, "test() **p2 %%p: %p\n", **p2);// 指向字符串内容的地址,也就是指向p1地址处的值

    fprintf(stdout, "test() p1 %%d: %d\n", p1);// 指向字符串内容
    fprintf(stdout, "test() p1 %%p: %p\n", p1);// 指向字符串内容的地址
    fprintf(stdout, "test() *p2 %%d: %d\n", *p2);// 指向字符串内容
    fprintf(stdout, "test() *p2 %%p: %p\n", *p2);// 指向字符串内容的地址,也就是指向p1地址处的值

    fprintf(stdout, "test() &p1 %%p: %p\n", &p1);// 指向p1自身的地址,被保存到p2中了
    fprintf(stdout, "test() p2 %%p: %p\n", p2);// 指向p1自身的地址
    fprintf(stdout, "test() &p2 %%p: %p\n", &p2);// 指向p2自身的地址

    fprintf(stdout, "test() p1[0] %%p: %p\n", p1[0]);
    fprintf(stdout, "test() *p1 %%p: %p\n", *p1);
    fprintf(stdout, "test() **p2 %%p: %p\n", **p2);

    return 0;
}