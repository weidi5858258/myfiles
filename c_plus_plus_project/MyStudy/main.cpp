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

    FILE *fp = nullptr;
    char c;
    fp = fopen("file.txt", "w");
    c = fgetc(fp);
    if (ferror(fp)) {
        printf("1��ȡ�ļ�: file.txtʱ��������\n");
    }
    clearerr(fp);
    if (ferror(fp)) {
        printf("2��ȡ�ļ�: file.txtʱ��������\n");
    }
    fclose(fp);


    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}
/***





*/