//
// Created by weidi5858258 on 2018/1/1.
//

#include "Method.h"

/***
 double sin(double x);
 */
void Method::wd_sin(void) {
    double param, result;
    param = 30.0;
    result = sin(param * PI / 180);
    printf("The sine of %f degrees is %f.\n", param, result);
}

/***
 double cos(double x);
 */
void Method::wd_cos(void) {
    double param, result;
    param = 60.0;
    result = cos(param * PI / 180.0);
    printf("The cosine of %f degrees is %f.\n", param, result);
}

/***
 double tan(double x);
 */
void Method::wd_tan(void) {
    double param, result;
    param = 45.0;
    result = tan(param * PI / 180.0);
    printf("The tangent of %f degrees is %f.\n", param, result);
}

/***
 int fgetc(FILE* stream);
 */
void Method::wd_fgetc(void) {
    FILE *pFile;
    int c;
    int temp;
    int n = 0;
    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "r");
    if (pFile == NULL) {
        // Error opening file: No such file or directory文件不存在时
        perror("Error opening file");
    } else {
        do {
            c = fgetc(pFile);
            printf("%c", c);
            if (c == '$') {
                n++;
            } else if (c == EOF) {
                printf(" The last second character is : %c\n", temp);
                printf(" The last character is : %c\n", c);
            }
            temp = c;
        } while (c != EOF);
        fclose(pFile);
        printf("The file contains %d dollar sign characters ($).\n", n);
    }
}

/***
 char* fgets(char* str, int num, FILE* stream);
 num这个数不能大于定义字符数组的大于，可以等于或者小于
 遇到“回车”就停止读取，也就是说最多读取一行的内容
 */
void Method::wd_fgets(void) {
    FILE *pFile;
    char mystring[1000];

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "r");
    if (pFile == NULL) {
        perror("Error opening file");
    } else {
//        if(fgets(mystring, 1000, pFile) != NULL){
//            puts(mystring);
//        }
        while (fgets(mystring, 1000, pFile) != NULL) {
            puts(mystring);
        }
        fclose(pFile);
    }
}

/***
 int fputc(int character, FILE* stream);
 把字符存到stream流代表的文件中
 文件打开时必须要有“写”的权限
 */
void Method::wd_fputc(void) {
    FILE *pFile;
    char c;

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "w");
    if (pFile != NULL) {
        for (c = 'A'; c <= 'Z'; c++) {
            fputc(c, pFile);
        }
        fclose(pFile);
    }
}

/***
 int fputs(const char* str, FILE* stream);
 把字符串str存到stream流代表的文件中
 文件打开时必须要有“写”的权限
 */
void Method::wd_fputs(void) {
    FILE *pFile;
    char sentence[256];
//    char* sentence;// error

    printf("Enter sentence to append: ");
    // 从输入设备输入字符，最多接收256个字节
    fgets(sentence, 256, stdin);
    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "a");
    fputs(sentence, pFile);
    fclose(pFile);
}

/***
 int getc(FILE* stream);
 */
void Method::wd_getc(void) {
    FILE *pFile;
    int c;
    int n = 0;
    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "r");
    if (pFile == NULL) {
        perror("Error opening file");
    } else {
        do {
            c = getc(pFile);
            if (c == '$') {
                n++;
            }
        } while (c != EOF);
        fclose(pFile);
        printf("File contains %d $.\n", n);
    }
}

/***
 int getchar ( void );
 */
void Method::wd_getchar(void) {
    int c;
    puts("Enter text. Include a dot('.') in a sentence to exit: ");
    do {
        // 输入时可以一大串的进行，输出时是一个字符一个字符进行的
        c = getchar();
        printf("%c\n", c);
        // putchar(c);
    } while (c != '.');
}

/***
 char* gets ( char* str );
 这个运行不了
 */
void Method::wd_gets(void) {
    char string[256];
    printf("Insert your full address: ");
    // warning: unsafe (see fgets instead)
    // gets(string);
    printf("Your address is: %s\n", string);
}

/***
 int putc(int character, FILE* stream);
 */
void Method::wd_putc(void) {
    FILE *pFile;
    char c;

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "wt");
    for (c = 'A'; c <= 'Z'; c++) {
        // 一个字符一个字符存进文件流中
        putc(c, pFile);
    }
    fclose(pFile);
}

/***
 int putchar(int character);
 */
void Method::wd_putchar(void) {
    char c;
    for (c = 'A'; c <= 'Z'; c++) {
        putchar(c);
    }
}

/***
 int puts(const char* str);
 */
void Method::wd_puts(void) {
    char string[] = "Hello World!";
    puts(string);
}

/***
 int ungetc(int character, FILE* stream);
 */
void Method::wd_ungetc(void) {
    FILE *pFile;
    int c;
    char buffer[256];

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "rt");
    if (pFile == NULL) {
        perror("Error opening file");
    } else {
        while (!feof(pFile)) {
            c = getc(pFile);
            if (c == EOF) {
                break;
            }
            if (c == '#') {
                ungetc('@', pFile);
            } else {
                ungetc(c, pFile);
            }
            if (fgets(buffer, 255, pFile) != NULL) {
                fputs(buffer, stdout);
            } else {
                break;
            }
        }
    }
}

/***
 总结：
    1.fgetc,fgets,fputc,fputs都是相对于文件流进行的操作，
    前两个是从文件流中读取内容，后两个是把内容写进文件流中去。
    2.读文件必须要有“r”的属性，写文件必须要有“w”的属性。
    3.操作fgets和fputs这两个函数时必须定义一个字符数组，不能是字符指针。
    4.用fgets函数从文件流中读取数据时，读取的长度（第二个参数值）不能
    超过定义的字符数组长度。
    5.getc与fgetc的参数和返回值都相同，用法也相同。
    6.getchar这个函数可以接收一大串的字符，包括空格，遇到回车就截断
    输入。输入时是一个字符一个字符进行输出的。
    7.putchar这个函数是一个字符一个字符往输出设备进行输出。
    8.putc这个函数是一个字符一个字符往文件流中输入。
    9.puts这个函数是整个字符串往输出设备进行输出。
 */

/***
 int fopen(const char* filename, const char* mode);
 mode:
 "r"  :  可读操作。文件必须先存在。文件不存在时，报没有这样的文件的异常。
 "w"  :  可写操作。文件不存在就创建新文件，若存在，则清空原内容。
 "a"  :  追加操作。文件不存在就创建新文件，若存在，则在原来的内容后面追加新的内容。
 "r+" :  可读可写。文件必须先存在。
 "w+" :  可读可写。
 "a+" :  可读可写。
 */
void Method::wd_fopen(void) {
    FILE *pFile;
    pFile = fopen("myfile.txt", "w");
    if (pFile != NULL) {
        fputs("fopen example ... ", pFile);
        fclose(pFile);
    }
}

/***
 int fclose(FILE* stream);
 */
void Method::wd_fclose(void) {
    FILE *pFile;
    pFile = fopen("myfile.txt", "wt");
    fprintf(pFile, "fclose example");
    fclose(pFile);
}

/***
 int fflush(FILE* stream);
 */
void Method::wd_fflush(void) {
    char mybuffer[80];
    FILE *pFile;
    pFile = fopen("myfile.txt", "r+");
    if (pFile == NULL) {
        perror("Error opening file");
    } else {
        fputs("test", pFile);
        fflush(pFile);
        fgets(mybuffer, 80, pFile);
        puts(mybuffer);
        fclose(pFile);
    }
}

/***
 void* memcpy(void* destination, const void* source, size_t num);
 */
void Method::wd_memcpy(void) {
    char myname[] = "Pierre de fermat";
    printf("%zu\n", strlen(myname));
    printf("%d\n", sizeof(myname));
    // strlen(myname)不包括结束符，sizeof (myname)包括结束符
    // memcpy(person.name, myname, strlen(myname)+1);
    memcpy(person.name, myname, sizeof(myname));
    person.age = 46;
    printf("person: %s, %d\n", person.name, person.age);
    memcpy(&person_copy, &person, sizeof(person));
    printf("person_copy: %s, %d\n",
           person_copy.name, person_copy.age);
}

/***
 void* memmove(void* destination, const void* source, size_t num);
 */
void Method::wd_memmove(void) {
    char str[] = "memmove can be very useful......";
    memmove(str + 20, str + 15, 11);
    puts(str);
}

/***
 const void* memchr(const void* ptr, int value, size_t num);
       void* memchr(      void* ptr, int value, size_t num);
 */
void Method::wd_memchr(void) {
    char *pch;
    char str[] = "Example string";
    pch = (char *) memchr(str, 'p', strlen(str));

    if (pch != NULL) {
        printf("'p' found at position %d.\n", pch - str + 1);
    } else {
        printf("'p' not found.\n");
    }
}

/***
 int memcmp(const void* ptr1, const void* ptr2, size_t num);
 Compares the first num bytes of the block of memory pointed by ptr1 to the first num bytes pointed by ptr2, returning zero if they all match or a value different form zero representing which is greater if they do not.
 Notice that, unlike strcmp, the function does not stop comparing after finding a null character.
 <0 the first byte that not match in both memory blocks has a lower value in ptr1 than in ptr2(if evaluated as unsigned char values)
 0 the contents of both memory blocks are equal
 >0 the first byte that does not match in both memory blocks has a greater value in ptr1 than in ptr2(if evaluated as unsigned char values)
 */
void Method::wd_memcmp(void) {
    char buffer1[] = "DWgaOtP12df0";
    char buffer2[] = "DWGAOTP12DF0";
    int n;
    n = memcmp(buffer1, buffer2, sizeof(buffer1));

    if (n > 0) {
        printf("'%s' is greater than '%s'.\n", buffer1, buffer2);
    } else if (n < 0) {
        printf("'%s' is less than '%s'.\n", buffer1, buffer2);
    } else {
        printf("'%s' is the same as '%s'.\n", buffer1, buffer2);
    }
}

/***
 void* memset(void* ptr, int value, size_t num);
 Sets the first num bytes of the block of memory pointed by ptr to  the specified value(interpreted as an unsigned char)
 */
void Method::wd_memset(void) {
    char str[] = "almost every programmer should know memset!";
    memset(str, '-', 6);
    puts(str);
}

/***
 char* strcat(char* destination, const char* source);
 */
void Method::wd_strcat(void) {
    char str[80];
    strcpy(str, "these ");
    strcpy(str, "strings ");
    strcpy(str, "are ");
    strcpy(str, "concatenated.");
    puts(str);
}

/***
 const char* strchr(const char* str, int character);
       char* strchr(      char* str, int character);
 */
void Method::wd_strchr(void) {
    char str[] = "This is a sample string.";
    char *pch;
    printf("Looking for the 's' character in \'%s\'...\n", str);
    // 在str这个字符串中找到字符‘s’后就返回找到后字符串
    pch = strchr(str, 's');

    if (pch == 0) {
        return;
    }

    // str: This is a sample string.
    // pch: s is a sample string.
    puts(pch);

    while (pch != NULL) {
        printf("pch = %d\n", pch);
        printf("str = %d\n", str);
        printf("found at %d\n", pch - str + 1);
        pch = strchr(pch + 1, 's');
    }
}

/***
 int strcmp(const char* str1, const char* str2);
 字符串比较，str1跟str2字母大小写都相同时返回0
 */
void Method::wd_strcmp(void) {
    char key[] = "apple";
    char buffer[80];

    do {
        printf("Guess my favorite fruit?\n");
        fflush(stdout);
        scanf("%79s", buffer);
    } while (strcmp(key, buffer) != 0);

    puts("Correct answer!");
}

/***
 int strcoll(const char* str1, const char* str2);
 */
void Method::wd_strcoll(void) {
    char key[] = "apple";
    char buffer[80];
    int result;

    do {
        printf("Guess my favorite fruit?\n");
        fflush(stdout);
        scanf("%79s", buffer);
        result = strcoll(key, buffer);
        printf("result = %d\n", result);
    } while (result != 0);

    puts("Correct answer!");
}

/***
 char* strcpy(char* destination, const char* source);
 */
void Method::wd_strcpy(void) {
    char str1[] = "Sample string";
    char str2[40];
    char str3[40];
    strcpy(str2, str1);
    strcpy(str3, "copy successful");
    printf("str1: %s\nstr2: %s\nstr3: %s\n", str1, str2, str3);
}

/***
 size_t strcspn(const char* str1, const char* str2);
 str1中的第一个字符在str2中出现，那么返回这个字符的位置，
 从“0”开始，如果没有出现，那么用str1中的第二个字符查找是否在str2
 中出现，依次类推。
 */
void Method::wd_strcspn(void) {
    char str[] = "fc9ba73";
    char keys[] = "d1234567890";
    int i;
    i = strcspn(str, keys);
    printf("The first number in str is at position %d.\n", i + 1);
}

/***
 char* strerror(int errnum);
 如果某些操作发生错误，那么系统会把这个错误放进errno这个变量中，
 然后再调用此函数得到具体错误原因
 */
void Method::wd_strerror(void) {
    FILE *pFile = 0;
    pFile = fopen("unexist.ent", "r");

    if (pFile == NULL) {
        printf("Error opening file unexist.ent: %s\n", strerror(errno));
    }
}

/***
 size_t strlen(const char* str);
 计算字符串长度
 */
void Method::wd_strlen(void) {
    char szInput[256];
    printf("Enter a sentence: ");
    // gets(szInput);
    printf("The sentence entered is %u characters long.\n", (unsigned) strlen(szInput));
    char *myTest = "abcdefg";
    // 7
    printf("The myTest is %u characters long.\n", (unsigned) strlen(myTest));
    // 8
    printf("The myTest is %u characters long.\n", sizeof(myTest));
}

/***
 char* strncat(char* destination, const char* source, size_t num);
 destination从source那里复制num个字节，接到自己的末尾
 */
void Method::wd_strncat(void) {
    char str1[20];
    char str2[20];
    strcpy(str1, "To be ");
    puts(str1);// To be
    strcpy(str2, "or not to be");
    puts(str2);// or not to be
    strncat(str1, str2, 8);
    puts(str1);// To be or not t
    puts(str2);// or not to be
}

/***
 在我的计算机上，3个字节相当于一个汉字（包括中文输入法下的标点符号）。
 如果没有3个字节的空间，那么是个什么字符也不知道。
 */
void Method::wd_strncat2(void) {
    char str1[20];
    char str2[20];
    strcpy(str1, "我叫王力伟。");
    puts(str1);//
    strcpy(str2, "！你叫什么名字？");
    puts(str2);//
    strncat(str1, str2, 7);
    puts(str1);//
    puts(str2);//
}

/***
 int strncmp(const char* str1, const char* str2, size_t num);
 str1跟str2进行比较，如果两个字符串的前num个字节完全相同，那么返回0.
 */
void Method::wd_strncmp(void) {
    char str[][5] = {"R2D2", "R3PO", "R2A6", "C2H6"};
    int n;
    puts("Looking for R2 astromech droids...");

    for (n = 0; n < 4; ++n) {
        if (strncmp(str[n], "R2xx", 2) == 0) {
            printf("found %s\n", str[n]);
        }
    }
}

/***
 char* strncpy(char* destination, const char* source, size_t num);
 str1字符串的长度是18,加上结束符是19.
 如果str2的数组长度是18,那么也能把str1这个
 字符串复制过来,只是没有了结束符,到时如果
 需要进行某种操作,要用到判断的时候就没法做了.
 */
void Method::wd_strncpy(void) {
    char str1[] = "To be or not to be";
    char str2[18];
    char str3[40];
    size_t length = sizeof(str2);
    printf("%d\n", length);
    str2[18] = '\0';
    length = sizeof(str2);
    printf("%d\n", length);
    strncpy(str2, str1, length);
    puts(str1);
    puts(str2);
    strncpy(str3, str2, 5);
    str3[5] = '\0';
    puts(str1);
    puts(str2);
    puts(str3);
}

/***
 const char* strpbrk(const char* str1, const char* str2);
       char* strpbrk(      char* str1, const char* str2);
 */
void Method::wd_strpbrk(void) {
    char str[] = "This is a sample string";
    char key[] = "aeiou";
    char *pch;
    printf("Vowels in '%s': ", str);
    pch = strpbrk(str, key);

    while (pch != NULL) {
        printf("%c", *pch);
        pch = strpbrk(pch + 1, key);
    }

    printf("\n");
}

/***
 wint_t fgetwc(FILE* stream);
 */
void Method::wd_fgetwc(void) {
    FILE *pFile;
    wint_t wc;
    int n = 0;
    pFile = fopen("myfile.txt", "r");
    if (pFile != NULL) {
        do {
            wc = fgetwc(pFile);
            wprintf(L"%d", wc);
            if (wc == L'$') {
                n++;
            }
        } while (wc != WEOF);
        fclose(pFile);
        wprintf(L"The file contains %d dollar sign characters ($).\n", n);
    }
}

/***

 */
void Method::wd_abort(void) {
    FILE *pFile;
    pFile = fopen("myfile.txt", "r");
    if (pFile == NULL) {
        fputs("error opening file\n", stderr);
        abort();
    }

    fclose(pFile);
}

/***

 */
void Method::wd_abs(void) {
    int m, n;
    m = abs(-11);
    n = abs(23);
    printf("m = %d\n", m);
    printf("n = %d\n", n);
}

/******************************************/

void fnExit1(void) {
    puts("Exit function 1.");
}

void fnExit2(void) {
    puts("Exit function 2.");
}

/***

 */
void Method::wd_atexit(void) {
    atexit(fnExit1);
    atexit(fnExit2);
    puts("Main function.");
}

/******************************************/

/***

 */
void Method::wd_atof(void) {
    double m, n;
    double pi = 3.1415926535;
    char buffer[256];
    printf("Enter degrees: ");
    fgets(buffer, 256, stdin);
    m = atof(buffer);
    n = sin(n * pi / 180);
    printf("The sine of %f degrees is %f\n", n, m);
}

/***

 */
void Method::wd_atoi(void) {
    int i;
    char buffer[256];
    fgets(buffer, 256, stdin);
    i = atoi(buffer);
    printf("The value entered is %d. Its double is %d.\n", i, i * 2);
}

/***

 */
void Method::wd_atol(void) {
    long int li;
    char buffer[256];
    printf("Enter a long number: ");
    fgets(buffer, 256, stdin);
    li = atol(buffer);
    printf("The value entered is %ld. Its double is %ld.\n", li, li * 2);
}

/***

 */
void Method::wd_atoll(void) {
    long long int lli;
    char buffer[256];
    printf("Enter a long number: ");
    fgets(buffer, 256, stdin);
    lli = atoll(buffer);
    printf("The value entered is %lld. Its double is %lld.\n", lli, lli * 2);
}

/******************************************/

void fnQExit(void) {
    puts("Quick exit function.");
}

/***

 */
void Method::wd_at_quick_exit(void) {
    // 没有这两个函数
    // at_quick_exit(fnQExit);
    puts("Main function: Beginning");
    // quick_exit(EXIT_SUCCESS);
    puts("Main function: End");
}

/******************************************/

/******************************************/

int compareints(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

/***
void* bsearch (const void* key,
               const void* base,
               size_t num,
               size_t size,
               int (*compar)(const void*,const void*));
key
Pointer to the object that serves as key for the search,
type-casted to a void*.

base
Pointer to the first object of the array where the
 search is performed, type-casted to a void*.

num
Number of elements in the array pointed to by base.
size_t is an unsigned integral type.

size
Size in bytes of each element in the array.
size_t is an unsigned integral type.

compar
Pointer to a function that compares two elements.
This function is called repeatedly by bsearch to
compare key against individual elements in base.
It shall follow the following prototype:
int compar (const void* pkey, const void* pelem);
 */
void Method::wd_bsearch(void) {
    int values[] = {50, 20, 60, 40, 10, 30};
    int *pItem;
    int key = 40;
    qsort(values, 6, sizeof(int), compareints);
    pItem = (int *) bsearch(&key, values, 6, sizeof(int), compareints);
    if (pItem != NULL) {
        printf("%d is in the array.\n", *pItem);
    } else {
        printf("%d is not in the array.\n", key);
    }

    char *pChItem;
    char strvalues[][20] = {"some", "example", "strings", "here"};
    char keys[20] = "exmaple";
    qsort(strvalues, 4, 20, (int (*)(const void *, const void *)) strcmp);
    pChItem = (char *) bsearch(keys, strvalues, 4, 20, (int (*)(const void *, const void *)) strcmp);
    if (pChItem != NULL) {
        printf("%s is in the array.\n", pChItem);
    } else {
        printf("%s is not in the array.\n", key);
    }
}

/******************************************/

/***
void* calloc (size_t num, size_t size);
num
Number of elements to allocate.

size
Size of each element.
size_t is an unsigned integral type.
 */
void Method::wd_calloc(void) {
    int i, n;
    int *pData;
    printf("Amount of numbers to be entered: ");
    scanf("%d", &i);
    pData = (int *) calloc(i, sizeof(int));
    if (pData == NULL) {
        exit(1);
    }
    for (n = 0; n < i; ++n) {
        printf("Enter number #%d: ", n + 1);
        scanf("%d", &pData[n]);
    }
    printf("You have entered: ");
    for (n = 0; n < i; ++n) {
        printf("%d", pData[n]);
    }
    free(pData);
}

/***
div_t div (int numer, int denom);
numer
Numerator.

denom
Denominator.
 */
void Method::wd_div(void) {
    div_t divresult;
    divresult = div(38, 5);
    // divresult.quot 商
    // divresult.rem 余数
    printf("38 div 5 = > %d, remainder %d.\n", divresult.quot, divresult.rem);
}

/***
void exit (int status);
status
Status code.
If this is 0 or EXIT_SUCCESS, it indicates success.
If it is EXIT_FAILURE, it indicates failure.
 */
void Method::wd_exit(void) {
    FILE *pFile;
    pFile = fopen("myfile.txt", "r");
    if (pFile == NULL) {
        printf("Error opening file.");
        exit(EXIT_FAILURE);
    } else {
        printf("Success opening file.");
        fclose(pFile);
        exit(EXIT_SUCCESS);
    }
}

/***
void free (void* ptr);
ptr
Pointer to a memory block previously allocated with malloc, calloc or realloc.
 */
void Method::wd_free(void) {
    int *buffer1, *buffer2, *buffer3;
    buffer1 = (int *) malloc(100 * sizeof(int));
    buffer2 = (int *) calloc(100, sizeof(int));
    buffer3 = (int *) realloc(buffer2, 500 * sizeof(int));
    free(buffer1);
    free(buffer3);
}

/***
char* getenv (const char* name);
name
C-string containing the name of the requested variable.
Depending on the platform, this may either be case sensitive or not.
 */
void Method::wd_getenv(void) {
    char *pPath;
    pPath = getenv("PATH");
    if (pPath != NULL) {
        printf("The current path is: %s", pPath);
    }
}

/***
long int labs (long int n);
n
Integral value.
 */
void Method::wd_labs(void) {
    long int m, n;
    m = labs(-100000L);
    n = labs(65537L);
    printf("m = %ld\n", m);
    printf("n = %ld\n", n);
}

/***
ldiv_t ldiv (long int numer, long int denom);
numer
Numerator.

denom
Denominator.
 */
void Method::wd_ldiv(void) {
    ldiv_t ldivresult;
    ldivresult = ldiv(1000000L, 132L);
    // ldivresult.quot 商
    // ldivresult.rem 余数
    printf("1000000 div 132 => %ld, remainder %ld.\n", ldivresult.quot, ldivresult.rem);
}

/***
long long int llabs (long long int n);
n
Integral value.
 */
void Method::wd_llabs(void) {
    long long int m, n;
    m = llabs(-100000000LL);
    n = llabs(31558149LL);
    printf("m = %lld\n", m);
    printf("n = %lld\n", n);
}

/***
lldiv_t lldiv (long long int numer, long long int denom);
numer
Numerator.

denom
Denominator.
 */
void Method::wd_lldiv(void) {
    lldiv_t lldivresult;
    lldivresult = lldiv(31558149LL, 3600LL);
    // lldivresult.quot 商
    // lldivresult.rem 余数
    printf("Earth orbit: %lld hours and %lld seconds.\n", lldivresult.quot, lldivresult.rem);
}

/***
void* malloc (size_t size);
size
Size of the memory block, in bytes.
size_t is an unsigned integral type.
 */
void Method::wd_malloc(void) {
    int i, n;
    char *buffer;
    printf("How long do you want the string?");
    scanf("%d", &i);
    buffer = (char *) malloc(i + 1);
    if (buffer == NULL) {
        exit(1);
    }
    for (n = 0; n < i; ++n) {
        buffer[n] = rand() % 26 + 'a';
    }
    buffer[i] = '\0';
    printf("Random string: %s\n", buffer);
    free(buffer);
}

void printBuffer(const char *pt, size_t max) {
    int length;
    wchar_t dest;
    mblen(NULL, 0);
    mbtowc(NULL, NULL, 0);
    while (max > 0) {
        length = mblen(pt, max);
        if (length < 1) {
            break;
        }
        mbtowc(&dest, pt, length);
        printf("[%lc", dest);
        pt += length;
        max -= length;
    }
}

/***
int mblen (const char* pmb, size_t max);
pmb
Pointer to the first byte of a multibyte character.
Alternativelly, the function may be called with a null pointer,
in which case the function resets its internal shift state to
the initial value and returns whether multibyte characters use
a state-dependent encoding.

max
Maximum number of bytes of pmb to consider for the multibyte character.
No more than MB_CUR_MAX characters are examined in any case.
size_t is an unsigned integral type.
 */
void Method::wd_mblen(void) {
    const char str[] = "test string";
    printBuffer(str, sizeof(str));
}

/***
size_t mbstowcs (wchar_t* dest, const char* src, size_t max);
dest
Pointer to an array of wchar_t elements long enough to contain
the resulting sequence (at most, max wide characters).

src
C-string with the multibyte characters to be interpreted.
The multibyte sequence shall begin in the initial shift state.

max
Maximum number of wchar_t characters to write to dest.
size_t is an unsigned integral type.

Return Value
The number of wide characters written to dest,
not including the eventual terminating null character.
If an invalid multibyte character is encountered,
a value of (size_t)-1 is returned.
Notice that size_t is an unsigned integral type,
and thus none of the values possibly returned is less than zero.
 */
void Method::wd_mbstowcs(void) {

}

void printBuffer2(const char *pt, size_t max) {
    int length;
    wchar_t dest;
    mbtowc(NULL, NULL, 0);
    while (max > 0) {
        length = mbtowc(&dest, pt, max);
        if (length < 1) {
            break;
        }
        printf("[%lc", dest);
        pt += length;
        max -= length;
    }
}

/***
int mbtowc (wchar_t* pwc, const char* pmb, size_t max);
pwc
Pointer to an object of type wchar_t.
Alternativelly, this argument can be a null pointer,
in which case the function does not store the wchar_t translation,
but still returns the length in bytes of the multibyte character.

pmb
Pointer to the first byte of a multibyte character.
Alternativelly, this argument can be a null pointer,
in which case the function resets its internal shift
state to the initial value and returns whether multibyte
characters have a state-dependent encoding.

max
Maximum number of bytes of pmb to consider for the multibyte character.
No more than MB_CUR_MAX characters are examined in any case.
size_t is an unsigned integral type.
 */
void wd_mbtowc(void) {
    const char str[] = "mbtowc example";
    printBuffer2(str, sizeof(str));
}

int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

/***
void qsort (void* base,
            size_t num,
            size_t size,
            int (*compar)(const void*,const void*));
base
Pointer to the first object of the array to be sorted, converted to a void*.

num
Number of elements in the array pointed to by base.
size_t is an unsigned integral type.

size
Size in bytes of each element in the array.
size_t is an unsigned integral type.

compar
Pointer to a function that compares two elements.
This function is called repeatedly by qsort to compare two elements.
It shall follow the following prototype:
int compar (const void* p1, const void* p2);
 */
void Method::wd_qsort(void) {
    int values[] = {40, 10, 100, 90, 20, 25};
    int n;
    qsort(values, 6, sizeof(int), compare);
    for (n = 0; n < 6; n++) {
        printf("%d ", values[n]);
    }
}

/***
_Noreturn void quick_exit (int status);
status
Status code.
If this is 0 or EXIT_SUCCESS, it indicates success.
If it is EXIT_FAILURE, it indicates failure.
 */
void Method::wd_quick_exit(void) {
    // at_quick_exit(fnQExit);
    puts("Main function: Beginning");
    // quick_exit(EXIT_SUCCESS);
    puts("Main function: End");  // never executed
}

/***
int rand (void);
Returns a pseudo-random integral number in the range between 0 and RAND_MAX.
RAND_MAX is a constant defined in <cstdlib>.
v1 = rand() % 100;         // v1 in the range 0 to 99
v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
 */
void Method::wd_rand(void) {
    int iSecret, iGuess;

    /* initialize random seed: */
    // srand(time(NULL));

    /* generate secret number between 1 and 10: */
    iSecret = rand() % 10 + 1;

    do {
        printf("Guess the number (1 to 10): ");
        scanf("%d", &iGuess);
        if (iSecret < iGuess) {
            puts("The secret number is lower");
        } else if (iSecret > iGuess) {
            puts("The secret number is higher");
        }
    } while (iSecret != iGuess);

    puts("Congratulations!");
}

/***
void* realloc (void* ptr, size_t size);
ptr
Pointer to a memory block previously allocated with malloc, calloc or realloc.
Alternatively, this can be a null pointer,
in which case a new block is allocated (as if malloc was called).

size
New size for the memory block, in bytes.
size_t is an unsigned integral type.
 */
void Method::wd_realloc(void) {
    int input, n;
    int count = 0;
    int *numbers = NULL;
    int *more_numbers = NULL;

    do {
        printf("Enter an integer value (0 to end): ");
        scanf("%d", &input);
        ++count;

        more_numbers = (int *) realloc(numbers, count * sizeof(int));

        if (more_numbers != NULL) {
            numbers = more_numbers;
            numbers[count - 1] = input;
        } else {
            free(numbers);
            puts("Error (re)allocating memory");
            exit(1);
        }
    } while (input != 0);

    printf("Numbers entered: ");
    for (n = 0; n < count; n++) printf("%d ", numbers[n]);
    free(numbers);
}

/***
void srand (unsigned int seed);
seed
An integer value to be used as seed by the
pseudo-random number generator algorithm.
 */
void Method::wd_srand(void) {
    printf("First number: %d\n", rand() % 100);
    // srand(time(NULL));
    printf("Random number: %d\n", rand() % 100);
    srand(1);
    printf("Again the first number: %d\n", rand() % 100);
}

/***
double strtod (const char* str, char** endptr);
str
C-string beginning with the representation of a floating-point number.

endptr
Reference to an already allocated object of type char*,
whose value is set by the function to the next character
in str after the numerical value.
This parameter can also be a null pointer,
in which case it is not used.
 */
void Method::wd_strtod(void) {
    char szOrbits[] = "365.24 29.53";
    char *pEnd;
    double d1, d2;
    d1 = strtod(szOrbits, &pEnd);
    d2 = strtod(pEnd, NULL);
    printf("The moon completes %.2f orbits per Earth year.\n", d1 / d2);
}

/***
float strtof (const char* str, char** endptr);
str
C-string beginning with the representation of a floating-point number.

endptr
Reference to an already allocated object of type char*,
whose value is set by the function to the next character in str after the numerical value.
This parameter can also be a null pointer, in which case it is not used.
 */
void Method::wd_strtof(void) {
    char szOrbits[] = "686.97 365.24";
    char *pEnd;
    float f1, f2;
    f1 = strtof(szOrbits, &pEnd);
    f2 = strtof(pEnd, NULL);
    printf("One martian year takes %.2f Earth years.\n", f1 / f2);
}

/***
long int strtol (const char* str, char** endptr, int base);
str
C-string beginning with the representation of an integral number.

endptr
Reference to an object of type char*, whose value is set by the
function to the next character in str after the numerical value.
This parameter can also be a null pointer, in which case it is not used.

base
Numerical base (radix) that determines the valid characters and their interpretation.
If this is 0, the base used is determined by the format in the sequence (see above).
 */
void Method::wd_strtol(void) {
    char szNumbers[] = "2001 60c0c0 -1101110100110100100000 0x6fffff";
    char *pEnd;
    long int li1, li2, li3, li4;
    li1 = strtol(szNumbers, &pEnd, 10);
    li2 = strtol(pEnd, &pEnd, 16);
    li3 = strtol(pEnd, &pEnd, 2);
    li4 = strtol(pEnd, NULL, 0);
    printf("The decimal equivalents are: %ld, %ld, %ld and %ld.\n", li1, li2, li3, li4);
}

/***
long double strtold (const char* str, char** endptr);
str
C string beginning with the representation of a floating-point number.

endptr
Reference to an already allocated object of type char*, whose value is
set by the function to the next character in str after the numerical value.
This parameter can also be a null pointer, in which case it is not used.
 */
void Method::wd_strtold(void) {
    char szOrbits[] = "90613.305 365.24";
    char *pEnd;
    long double f1, f2;
    f1 = strtold(szOrbits, &pEnd);
    f2 = strtold(pEnd, NULL);
    printf("Pluto takes %.2Lf years to complete an orbit.\n", f1 / f2);
}

/***
long long int strtoll (const char* str, char** endptr, int base);
str
C-string beginning with the representation of an integral number.

endptr
Reference to an object of type char*, whose value is set by the
function to the next character in str after the numerical value.
This parameter can also be a null pointer, in which case it is not used.

base
Numerical base (radix) that determines the valid characters and their interpretation.
If this is 0, the base used is determined by the format in the sequence (see strtol for details).
 */
void Method::wd_strtoll(void) {
    char szNumbers[] = "1856892505 17b00a12b -01100011010110000010001101100 0x6fffff";
    char *pEnd;
    long long int lli1, lli2, lli3, lli4;
    lli1 = strtoll(szNumbers, &pEnd, 10);
    lli2 = strtoll(pEnd, &pEnd, 16);
    lli3 = strtoll(pEnd, &pEnd, 2);
    lli4 = strtoll(pEnd, NULL, 0);
    printf("The decimal equivalents are: %lld, %lld, %lld and %lld.\n", lli1, lli2, lli3, lli4);
}

/***
unsigned long int strtoul (const char* str, char** endptr, int base);
str
C-string containing the representation of an integral number.

endptr
Reference to an object of type char*, whose value is set by the
function to the next character in str after the numerical value.
This parameter can also be a null pointer, in which case it is not used.

base
Numerical base (radix) that determines the valid characters and their interpretation.
If this is 0, the base used is determined by the format in the sequence (see strtol for details).
 */
void Method::wd_strtoul(void) {
    char buffer[256];
    unsigned long ul;
    printf("Enter an unsigned number: ");
    fgets(buffer, 256, stdin);
    ul = strtoul(buffer, NULL, 0);
    printf("Value entered: %lu. Its double: %lu\n", ul, ul * 2);
}

/***
unsigned long long int strtoull (const char* str, char** endptr, int base);
str
C-string beginning with the representation of an integral number.

endptr
Reference to an object of type char*, whose value is set by the
function to the next character in str after the numerical value.
This parameter can also be a null pointer, in which case it is not used.

base
Numerical base (radix) that determines the valid characters and their interpretation.
If this is 0, the base used is determined by the format in the sequence (see strtol for details).
 */
void Method::wd_strtoull(void) {
    char szNumbers[] = "250068492 7b06af00 1100011011110101010001100000 0x6fffff";
    char *pEnd;
    unsigned long long int ulli1, ulli2, ulli3, ulli4;
    ulli1 = strtoull(szNumbers, &pEnd, 10);
    ulli2 = strtoull(pEnd, &pEnd, 16);
    ulli3 = strtoull(pEnd, &pEnd, 2);
    ulli4 = strtoull(pEnd, NULL, 0);
    printf("The decimal equivalents are: %llu, %llu, %llu and %llu.\n", ulli1, ulli2, ulli3, ulli4);
}

/***
int system (const char* command);
command
C-string containing the system command to be executed.
Or, alternatively, a null pointer, to check for a command processor.
 */
void Method::wd_system(void) {
    int i;
    printf("Checking if processor is available...");
    if (system(NULL)) puts("Ok");
    else exit(EXIT_FAILURE);
    printf("Executing command DIR...\n");
    i = system("dir");
    printf("The value returned was: %d.\n", i);
}

/***
size_t wcstombs (char* dest, const wchar_t* src, size_t max);
dest
Pointer to an array of char elements long enough to
contain the resulting sequence (at most, max bytes).

src
C wide string to be translated.

max
Maximum number of bytes to be written to dest.
size_t is an unsigned integral type.
 */
void Method::wd_wcstombs(void) {
    const wchar_t str[] = L"wcstombs example";
    char buffer[32];
    int ret;
    printf("wchar_t string: %ls \n", str);
    ret = wcstombs(buffer, str, sizeof(buffer));
    if (ret == 32) buffer[31] = '\0';
    if (ret) {
        printf("multibyte string: %s \n", buffer);
    }
}

/***
int wctomb (char* pmb, wchar_t wc);
pmb
Pointer to an array large enough to hold a multibyte sequence.
The maximum length of a multibyte sequence for a character in
the current locale is MB_CUR_MAX bytes.
Alternativelly, the function may be called with a null pointer,
in which case the function resets its internal shift state to
the initial value and returns whether multibyte sequences use a state-dependent encoding.

wc
Wide character of type wchar_t.
 */
void Method::wd_wctomb(void) {
    const wchar_t str[] = L"wctomb example";
    const wchar_t *pt;
    char buffer[MB_CUR_MAX];
    int i, length;
    pt = str;
    while (*pt) {
        length = wctomb(buffer, *pt);
        if (length < 1) break;
        for (i = 0; i < length; ++i) printf("[%c]", buffer[i]);
        ++pt;
    }
}

/***
void _Exit (int status);
status
Status code.
If this is 0 or EXIT_SUCCESS, it indicates success.
If it is EXIT_FAILURE, it indicates failure.
 */
void Method::wd__Exit(void) {
    FILE *pFile;
    pFile = fopen("myfile.txt", "r");
    if (pFile == NULL) {
        printf("Error opening file");
        _Exit(EXIT_FAILURE);
    } else {
        /* file operations here */
        printf("Success opening file");
        fclose(pFile);
        _Exit(EXIT_SUCCESS);
    }
}
