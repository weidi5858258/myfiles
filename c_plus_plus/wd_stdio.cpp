#include <stdio.h>

/***
 int fgetc(FILE* stream);
 */
void wd_fgetc(void){
    FILE* pFile;
    int c;
    int temp;
    int n = 0;
    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "r");
    if(pFile == NULL){
        // Error opening file: No such file or directory文件不存在时
        perror("Error opening file");
    }else{
        do{
            c = fgetc(pFile);
            printf("%c", c);
            if(c == '$'){
                n++;
            }else if(c == EOF){
                printf(" The last second character is : %c\n", temp);
                printf(" The last character is : %c\n", c);
            }
            temp = c;
        }while(c != EOF);
        fclose(pFile);
        printf("The file contains %d dollar sign characters ($).\n", n);
    }
}

/***
 char* fgets(char* str, int num, FILE* stream);
 num这个数不能大于定义字符数组的大于，可以等于或者小于
 遇到“回车”就停止读取，也就是说最多读取一行的内容
 */
void wd_fgets(void){
    FILE* pFile;
    char mystring[1000];

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "r");
    if(pFile == NULL){
        perror("Error opening file");
    }else{
//        if(fgets(mystring, 1000, pFile) != NULL){
//            puts(mystring);
//        }
        while(fgets(mystring, 1000, pFile) != NULL){
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
void wd_fputc(void){
    FILE* pFile;
    char c;

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "w");
    if(pFile != NULL){
        for(c='A';c<='Z';c++){
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
void wd_fputs(void){
    FILE* pFile;
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
void wd_getc(void){
    FILE* pFile;
    int c;
    int n = 0;
    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "r");
    if(pFile == NULL){
        perror("Error opening file");
    }else{
        do{
            c = getc(pFile);
            if(c == '$'){
                n++;
            }
        }while(c != EOF);
        fclose(pFile);
        printf("File contains %d $.\n", n);
    }
}

/***
 int getchar(void);
 */
void wd_getchar(void){
    int c;
    puts("Enter text. Include a dot('.') in a sentence to exit: ");
    do{
        // 输入时可以一大串的进行，输出时是一个字符一个字符进行的
        c = getchar();
        printf("%c\n", c);
        // putchar(c);
    }while(c != '.');
}

/***
 char* gets(char* str);
 这个运行不了
 */
void wd_gets(void){
    char string[256];
    printf("Insert your full address: ");
    // warning: unsafe (see fgets instead)
    // gets(string);
    printf("Your address is: %s\n", string);
}

/***
 int putc(int character, FILE* stream);
 */
void wd_putc(void){
    FILE* pFile;
    char c;

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "wt");
    for(c='A';c<='Z';c++){
        // 一个字符一个字符存进文件流中
        putc(c, pFile);
    }
    fclose(pFile);
}

/***
 int putchar(int character);
 */
void wd_putchar(void){
    char c;
    for(c='A';c<='Z';c++){
        putchar(c);
    }
    putchar('\n');
}

/***
 int puts(const char* str);
 */
void wd_puts(void){
    char string[] = "Hello World!";
    puts(string);
}

/***
 int ungetc(int character, FILE* stream);
 */
void wd_ungetc(void){
    FILE* pFile;
    int c;
    char buffer[256];

    pFile = fopen("D:\\workspace_c\\test_dir\\test_file.txt", "rt");
    if(pFile == NULL){
        perror("Error opening file");
    }else{
        while(!feof(pFile)){
            c = getc(pFile);
            if(c == EOF){
                break;
            }
            if(c == '#'){
                ungetc('@', pFile);
            }else{
                ungetc(c, pFile);
            }
            if(fgets(buffer, 255, pFile) != NULL){
                fputs(buffer, stdout);
            }else{
                break;
            }
        }
    }
}

/***
 总结：
    1.
    fgetc,fgets,fputc,fputs都是相对于文件流进行的操作，
    前两个是从文件流中读取内容，后两个是把内容写进文件流中去。
    2.
    读文件必须要有“r”的属性，写文件必须要有“w”的属性。
    3.
    操作fgets和fputs这两个函数时必须定义一个字符数组，不能是字符指针。
    4.
    用fgets函数从文件流中读取数据时，读取的长度（第二个参数值）不能
    超过定义的字符数组长度。
    5.
    getc与fgetc的参数和返回值都相同，用法也相同。
    6.
    getchar这个函数可以接收一大串的字符，包括空格，
    遇到回车就截断输入。输入时是一个字符一个字符进行输入的。
    7.
    putchar这个函数是一个字符一个字符往输出设备进行输出。
    8.
    putc这个函数是一个字符一个字符往文件流中输入。
    9.
    puts这个函数是整个字符串往输出设备进行输出。

    char content[1024];
    fgets(content, 1024, stdin);
    fputs(content, stdout);

    int     fgetc(FILE* stream);
    int     fputc(int character, FILE* stream);

    使用这两个方法就可以了
    char*   fgets(char* str, int num, FILE* stream);
    int     fputs(const char* str, FILE* stream);
    
    int     getc(FILE* stream);
    int     putc(int character, FILE* stream);

    int     getchar(void);
    int     putchar(int character);

    char*   gets(char* str);(废弃不用,使用fgets代替,stream换成stdin就可以了)
    int     puts(const char* str);
    int     ungetc(int character, FILE* stream);
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
void wd_fopen(void){
    FILE* pFile;
    pFile = fopen("myfile.txt", "w");
    if(pFile != NULL){
        fputs("fopen example ... ", pFile);
        fclose(pFile);
    }
}

/***
 int fclose(FILE* stream);
 */
void wd_fclose(void){
    FILE* pFile;
    pFile = fopen("myfile.txt","wt");
    fprintf(pFile, "fclose example");
    fclose(pFile);
}

/***
 int fflush(FILE* stream);
 */
void wd_fflush(void){
    char mybuffer[80];
    FILE* pFile;
    pFile = fopen("myfile.txt", "r+");
    if(pFile == NULL){
        perror("Error opening file");
    }else{
        fputs("test", pFile);
        fflush(pFile);
        fgets(mybuffer, 80, pFile);
        puts(mybuffer);
        fclose(pFile);
    }
}

















