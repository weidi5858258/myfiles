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
#include "./include/FormatCode.hpp"
#include "./include/TestRegularExpression.hpp"
#include "./include/leak_detector_c.hpp"
#include "./include/TestString.hpp"
#include "./include/TestClass.hpp"
//#include "./include/thread_pool_active.hpp"
//#include "./include/leak_detector_c.h"

#endif

int test(int argc, char **argv);

int basicKnowledge(int argc, char **argv);

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
    // 有多少个参数
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

//    argc = 3;
//    argv[1] = "/root/temp_dir/format_code/main.cpp";
//    argv[2] = "/root/temp_dir/format_code/main_temp.cpp";
//    FormatCode *formatCode = new FormatCode;
//    formatCode->start(argc, argv);
//    formatCode->release();
//    delete formatCode;

//    TestRegularExpression testRegularExpression;
//    testRegularExpression.test();

    // 智能指针
//    alexander::TestString testString;
//    testString.testSmartPointer();

    //test(argc, argv);
    basicKnowledge(argc, argv);

    /*{
        alexander::Son son(10);
    }*/

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}

int basicKnowledge(int argc, char **argv) {
    /***
     通过这个例子,只要知道能这样操作就行了.即:
     我们一般要操作的是内容,因此使用p1或者*p2以%s输出时,得到的就是内容(限于char类型).
     */
    char *p1 = "Hello World";
    char **p2 = &p1;

    // Hello World
    fprintf(stdout, "basicKnowledge() p1    %%s: %s\n", p1);// 指向字符串内容(使用的时候不要带上*)
    // Hello World
    fprintf(stdout, "basicKnowledge() *p2   %%s: %s\n", *p2);// 指向字符串内容
    // Hello World
    fprintf(stdout, "basicKnowledge() p2[0] %%s: %s\n", p2[0]);// 指向字符串内容
    // H(只能以%c输出单个字符,以%s输出就会出错)
    fprintf(stdout, "basicKnowledge() p2[0] %%c: %c\n", p1[0]);// 指向字符串内容
    // 0x436dee
    fprintf(stdout, "basicKnowledge() p1    %%p: %p\n", p1);// 指向字符串内容的地址
    // 0x436dee
    fprintf(stdout, "basicKnowledge() *p2   %%p: %p\n", *p2);// 指向字符串内容的地址,也就是指向p1地址处的值

    // 0x7ffe793a41b0
    fprintf(stdout, "basicKnowledge() &p1   %%p: %p\n", &p1);// 指向p1自身的地址,被保存到p2中了
    // 0x7ffe793a41b0
    fprintf(stdout, "basicKnowledge() p2    %%p: %p\n", p2);// 指向p1自身的地址
    // 0x7ffe793a41b8
    fprintf(stdout, "basicKnowledge() &p2   %%p: %p\n", &p2);// 指向p2自身的地址

    // 0x48
    fprintf(stdout, "basicKnowledge() p1[0] %%p: %p\n", p1[0]);
    // 0x48
    fprintf(stdout, "basicKnowledge() *p1   %%p: %p\n", *p1);
    // 0x48
    fprintf(stdout, "basicKnowledge() **p2  %%p: %p\n", **p2);

    int a = 100;
    int *p3 = &a;
    int **p4 = &p3;
    // 100
    fprintf(stdout, "basicKnowledge() *p1   %%d: %d\n", *p3);// 指向int内容
    // 100
    fprintf(stdout, "basicKnowledge() **p2  %%d: %d\n", **p4);// 指向int内容

    // 下面是定义字符串的几种方式及得到字符串的长度
    const char *string1 = "Hello World";
    int len = strlen(string1);
    printf("string1 strlen: %d\n", len);// 11
    size_t size = sizeof(string1);
    printf("string1 sizeof: %d\n", size);// 8

    // 最好以这种方式定义一个字符串
    const char string2[] = "Hello World";
    len = strlen(string2);
    printf("string2 strlen: %d\n", len);// 11
    size = sizeof(string2);
    printf("string2 sizeof: %d\n", size);// 12(包括了'\0'这个字符)

    const char string3[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    len = strlen(string3);
    printf("string3 strlen: %d\n", len);// 11
    size = sizeof(string3);
    printf("string3 sizeof: %d\n", size);// 11(大括号中有多少个字符结果就是多少)

    const char string4[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\0'};
    len = strlen(string4);
    printf("string4 strlen: %d\n", len);// 11(遇到'\0'就结束计数)
    size = sizeof(string4);
    printf("string4 sizeof: %d\n", size);// 12(大括号中有多少个字符结果就是多少)

    const char string5[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\0', 'a'};
    len = strlen(string5);
    printf("string5 strlen: %d\n", len);// 11(遇到'\0'就结束计数)
    size = sizeof(string5);
    printf("string5 sizeof: %d\n", size);// 13(大括号中有多少个字符结果就是多少)

    cout << endl;
    short n_short_min = SHRT_MIN;
    short n_short_max = SHRT_MAX;
    int n_int_min = INT_MIN;
    int n_int_max = INT_MAX;
    int n_int8_min = INT8_MIN;
    int n_int8_max = INT8_MAX;
    int n_int16_min = INT16_MIN;
    int n_int16_max = INT16_MAX;
    int n_int32_min = INT32_MIN;
    int n_int32_max = INT32_MAX;
    int n_int64_min = INT64_MIN;
    int n_int64_max = INT64_MAX;
    long n_long_min = LONG_MIN;
    long n_long_max = LONG_MAX;
    long long n_llong_min = LLONG_MIN;
    long long n_llong_max = LLONG_MAX;
    cout << "short  max is " << sizeof n_short_max << " bytes." << endl;
    cout << "int    max is " << sizeof(int) << " bytes." << endl;
    cout << "int8   max is " << sizeof n_int8_max << " bytes." << endl;
    cout << "int16  max is " << sizeof n_int16_max << " bytes." << endl;
    cout << "int32  max is " << sizeof n_int32_max << " bytes." << endl;
    cout << "int64  max is " << sizeof n_int64_max << " bytes." << endl;
    cout << "long   max is " << sizeof n_long_max << " bytes." << endl;
    cout << "long long max is " << sizeof n_llong_max << " bytes." << endl;
    cout << endl;

    cout << "short min: " << n_short_min << endl;
    cout << "short max: " << n_short_max << endl;
    cout << "int min  : " << n_int_min << endl;
    cout << "int max  : " << n_int_max << endl;
    cout << "int8 min : " << n_int8_min << endl;
    cout << "int8 max : " << n_int8_max << endl;
    cout << "int16 min: " << n_int16_min << endl;
    cout << "int16 max: " << n_int16_max << endl;
    cout << "int32 min: " << n_int32_min << endl;
    cout << "int32 max: " << n_int32_max << endl;
    cout << "int64 min: " << n_int64_min << endl;
    cout << "int64 max: " << n_int64_max << endl;
    cout << "long min : " << n_long_min << endl;
    cout << "long max : " << n_long_max << endl;
    cout << "long long min: " << n_llong_min << endl;
    cout << "long long max: " << n_llong_max << endl;

    cout << "Bits per byte = " << CHAR_BIT << endl;
}

int test(int argc, char **argv) {
#define alloca __builtin_alloca
#define alloca_array(type, size) ((type *) alloca ((size) * sizeof (type)))
    char **url, **t;
    // 申请存储两个字符串的空间,由二级指针保存这个空间首地址
    url = alloca_array (char *, 2);
    url[0] = "https://";
    // 最后一个元素弄一个标记,这样在for循环中可以起到判断作用
    url[1] = NULL;
    // *t不为NULL就循环
    for (t = url; *t; t++) {
        fprintf(stdout, "test() url: %s\n", *t);
    }

#ifdef WIN32
    fprintf(stdout, "  WIN32\n");
#elif BOOST_OS_LINUX
    fprintf(stdout, "  BOOST_OS_LINUX\n");
#else
    fprintf(stdout, "  UNKNOW\n");
#endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

/***
程序中表示个数的时候从1开始数
程序中表示位置偏移的时候从0开始数

类型	输出	例子
d或i 带符号十进制整形	392
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

/*void king_counter(void *arg) {
    int index = *(int*)arg;
    printf("index : %d, selfid : %lu\n", index, pthread_self());
    free(arg);
    usleep(1);
}
 nThreadPool *pool = ntyThreadPoolCreate(10, 20, 15, NULL);

    int i = 0;
    for (i = 0;i < KING_COUNTER_SIZE;i ++) {
        int *index = (int*)malloc(sizeof(int));

        memset(index, 0, sizeof(int));
        memcpy(index, &i, sizeof(int));

        ntyThreadPoolQueue(pool, king_counter, index);

    }

    getchar();
    printf("You are very good !!!!\n");
#define KING_COUNTER_SIZE 1000*/