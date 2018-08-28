#include "src/platform.h"

#ifdef UBUNTU_SYSTEM
#include "./include/MyHead.h"
#include "./include/Method.h"
#else

#include "MyHead.h"
#include "Method.h"

#endif

#ifndef MYSTUDY_STUDY
#define MYSTUDY_STUDY

#include "src/Study.cpp"

#endif //MYSTUDY_STUDY


void test();


/***
 * @param argc 参数至少有一个,因为第一个参数就是本身的可执行文件
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    int j = 0;
    for (j = 0; j < argc; j++) {
        printf("%s\n", argv[j]);
    }
    printf("The run result:\n");
    printf("------------------------------------------\n");

    vector<string> tVector;
    tVector.push_back("a");
    tVector.push_back("b");
    tVector.push_back("a");
    tVector.push_back("c");

    char ch1[100] = "what't your name? ";
    char ch2[] = "my name is Jack.";
    memmove(ch1, ch2, 10);
    cout << ch1 << endl;

    string str1 = "what't your name? ";
    string str2 = "my name is Jack.";
    str1.erase(8, 2);
    cout << str1 << endl;
    str1.erase(5);
    cout << str1 << endl;
    str1.erase();
    cout << str1 << endl;
    //whmy nt't your name?


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
































































