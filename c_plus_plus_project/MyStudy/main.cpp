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
 * @param argc ����������һ��,��Ϊ��һ���������Ǳ���Ŀ�ִ���ļ�
 * @param argv
 * @return
Clion �����������
File -> Settings -> Default Settings -> Editor -> File Encodings:
Global Encoding: UTF-8
Project Encoding : UTF-8
Default encoding for properties files: UTF-8
Ȼ���йؼ�һ���裺�ڳ̳������½��޸�utf-8ΪGBK
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
