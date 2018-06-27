

/***
isalnum（测试字符是否为英文或数字）
头文件：#include <ctype.h>
相关函数：isalpha, isdigit, islower, isupper
函数定义：int isalnum(int c)
函数说明：检查参数c是否为英文字母或阿拉伯数字，
在标准c中相当于使用（isalpha(c) || isdigit(c)）做测试。
返回值：若参数c为字母或数字，则返回TRUE，否则返回NULL(0)。
附加说明：此为宏定义，非真正函数。
范例：找出str字符串中为英文字母或数字的字符
*/
#include <ctype.h>
void main(void){
    char str[] = "123c@#FDsP[e?";
    int i;
    for(i=0;str[i]!=0;i++){
        if(isalnum(str[i])){
            printf("%c is an alphanumeric character\n", str[i]);
        }
    }
}

/***
isalpha（测试字符是否为英文字母）
相关函数：isalnum, islower, isupper
头文件：#include <ctype.h>
函数定义：int isalpha(int c)
函数说明：检查参数c是否为英文字母，在标准c中相当于使用（isupper(c) || islower(c)）做测试。
返回值：若参数c为英文字母，则返回TRUE，否则返回NULL(0)。
附加说明：此为宏定义，非真正函数。
范例：找出str字符串中为英文字母的字符
*/
#include <ctype.h>
void main(void){
    char str[] = "123c@#FDsP[e?";
    int i;
    for(i=0;str[i]!=0;i++){
        if(isalpha(str[i])){
            printf("%c is an alphanumeric character\n", str[i]);
        }
    }
}

/***
isascii（测试字符是否为ASCII码字符）
相关函数：iscntrl
头文件：#include <ctype.h>
函数定义：int isascii(int c)
函数说明：检查参数c是否为ASCII码字符，也就是判断c的范围是否在0到127之间。
返回值：若参数c为ASCII码字符，则返回TRUE，否则返回NULL(0)。
附加说明：此为宏定义，非真正函数。
范例：找出int i是否具有对映的ASCII码字符
*/
#include <ctype.h>
void main(void){
    int i;
    for(i=125;i<130;i++){
        printf("%d is an ascii character:%c\n", i, i);
    }else{
        printf("%d is not ascii character\n", i);
    }
}

/***
iscntrl（测试字符是否为ASCII码的控制字符）
相关函数：isascii
头文件：#include <ctype.h>
函数定义：int iscntrl(int c)
函数说明：检查参数c是否为ASCII控制码，也就是判断c的范围是否在0到30之间。
返回值：若参数c为ASCII控制码，则返回TRUE，否则返回NULL(0)。
附加说明：此为宏定义，非真正函数。
范例：找出int i是否具有对映的ASCII码字符
*/

/***
isdigit（测试字符是否为ASCII码的控制字符）
相关函数：isxdigit
头文件：#include <ctype.h>
函数定义：int isdigit(int c)
函数说明：检查参数c是否为阿拉伯数字0到9。
返回值：若参数c为阿拉伯数字，则返回TRUE，否则返回NULL(0)。
附加说明：此为宏定义，非真正函数。
范例：找出str字符中为阿拉伯数字的字符
*/
#include <cytpe.h>
void main(void){
    char str[] = "123c@#FDsP[e?";
    int i;
    for(i=0;str[i]!=0;i++){
        if(isdigit(str[i])){
            printf("%c is an digit character\n", str[i]);
        }
    }
}
































