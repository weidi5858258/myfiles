#ifdef WIN32

#include "MyHead.h"
#include "Test.h"
#include "Method.h"
#include "HandleAndroidString.h"

#endif
#ifdef linux

#include "./include/MyHead.h"
#include "./include/Test.h"
#include "./include/Method.h"
#include "./include/HandleAndroidString.h"

#endif

#ifndef MYSTUDY_STUDY
#define MYSTUDY_STUDY

#include "src/Study.cpp"

#endif //MYSTUDY_STUDY


void test();

class A {
public:
    virtual int get() {
        return 0;
    }
};

class B : public A {
public:
    virtual int get() {
        return 1;
    }
};

class C : public B {
public:
    int get() {
        return 2;
    }
};

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

    HandleAndroidString handleAndroidString;
    handleAndroidString.doSomething("/root/temp_dir/merge_string");

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

typedef struct stud {
    int num;
    int score;
    struct stud *next = NULL;
};

void testLink() {
    struct stud *head = NULL, *p = NULL;

    p = (struct stud *) malloc(sizeof(struct stud));
    scanf("%d%d", &(p->num), &(p->score));

    head = p;
    p->next = NULL;

    p = (struct stud *) malloc(sizeof(struct stud));
    scanf("%d%d", &(p->num), &(p->score));

    p->next = head;
    head = p;
}

/***
 头插法建立单链表
 */
struct stud *h_create() {
    struct stud *head = NULL, *p = NULL;
    int n;
    while (1) {
        printf("\nInput num:");
        scanf("%d", &n);
        if (n < 0) {
            break;
        }
        p = (struct stud *) malloc(sizeof(struct stud));
        p->num = n;
        printf("\nInput score:");
        scanf("%d", &(p->score));
        p->next = head;
        head = p;
    }
    return head;
}

/***
 尾插法建立单链表
 */
struct stud *r_create() {
    struct stud *head = NULL, *rear = NULL, *p = NULL;
    int n;
    while (1) {
        printf("\nInput num:");
        scanf("%d", &n);
        if (n < 0) {
            break;
        }
        p = (struct stud *) malloc(sizeof(struct stud));
        p->num = n;
        printf("\nInput score:");
        scanf("%d", &(p->score));
        if (rear == NULL) {
            head = p;
            p->next = NULL;
            rear = p;
        } else {
            p->next = rear->next;
            rear->next = p;
            rear = p;
        }
    }
    return head;
}
































































