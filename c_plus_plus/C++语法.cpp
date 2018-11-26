C++基础知识

///////////////////////////零碎知识点///////////////////////////

静态变量(static)
外部变量(extern)
寄存器变量
自动存储变量

            具体写法                  简写形式
有符号短整型	short int           ---> short
有符号长整型	long int            ---> long(数值后面加"l"或者"L")
无符号短整型	unsigned short int  ---> unsigned short
无符号长整型	unsigned long int   ---> unsigned long
有符号整型    signed int          ---> int
无符号整型		 unsigned int        ---> unsigned

C++语言的基本数据类型
1)布尔型:
    bool
2)字符型:
    char
  双字符型:
    双字节型(wchar_t)
    宽字符型
3)整型:
    无符号型(unsigned int)
    短整型(short int)
    长整型(long int)
4)浮点型:
    单精度浮点型(float)
    双精度浮点型(double)
    长双精度浮点型(long double)

类型                         占用字节数       取值范围
bool                            1           true or false

char                            1           -128~127(256个字符)
wchar_t 			                                 256个字符

short int                       2           -32768~32767
unsigned short int                          0~65535

int                             4           -2147483648~2147483647
long int                        4           -2147483648~2147483647
unsigned int(或者unsigned)                 0~4294967295(32位系统)
unsigned long int                           0~4294967295

float                           4
double                          8
long double                     12

								 char									-128~127
typedef unsigned char				uint8_t;	0~255
typedef unsigned short int	uint16_t;	0~65535
								 int									-32768~32767
typedef unsigned int				uint32_t;	0~65535
			  [signed] long [int]						-2147483648~2147483647
typedef unsigned long int		uint64_t;	0~4294967295

size_t和int
  size_t是一些C/C++标准在stddef.h中定义的.
  这个类型足以用来表示对象的大小.size_t的真实类型与操作系统有关.
在32位架构中被普遍定义为：
typedef   unsigned int size_t;
而在64位架构中被定义为：
typedef  unsigned long size_t;
	size_t在32位架构上是4字节,在64位架构上是8字节,
	在不同架构上进行编译时需要注意这个问题.
	而int在不同架构下都是4字节,与size_t不同;
	且int为带符号数,size_t为无符号数.

ssize_t
ssize_t是有符号整型,在32位机器上等同与int,在64位机器上等同与long int.

size_t和ssize_t作用
	 size_t一般用来表示一种计数,比如有多少东西被拷贝等.
	 例如：sizeof操作符的结果类型是size_t,
	 该类型保证能容纳实现所建立的最大对象的字节大小. 
	 它的意义大致是"适于计量内存中可容纳的数据项目个数的无符号整数类型".
	 所以,它在数组下标和内存管理函数之类的地方广泛使用.
	 而ssize_t这个数据类型用来表示可以被执行读写操作的数据块的大小.
	 它和size_t类似,但必需是signed.意即：它表示的是signed size_t类型的.

进程的终止方式：
1.从main返回
2.调用exit
3.调用_exit
4.调用abort
5.由一个信号终止
进程在终止的时候,系统会释放进程所拥有的资源,如内存、文件描述符、内核结构等.

进程之间的通信
进程之间的通信有多种方式,其中管道、共享内存和消息队列是最常用的方式.
1.
管道
是UNIX族中进程通信的最古老的方式,它利用内核在两个进程之间建立通道,
它的特点是与文件的操作类似,仅仅在管道的一端只读,另一端只写.
利用读写的方式在进程之间传递数据.
2.
共享内存
在多个进程之间共享,多个进程利用获得的共享内存的地址来直接对内存进行操作.
3.
消息
是在内核中建立一个链表,发送方按照一定的标识将数据发送到内核中,
内核将其放入量表后,等待接收方的请求.接收方发送请求后,
内核按照消息的标识,从内核中将消息从链表中摘下,传递给接收方.
消息是一种完全的异步操作方式.

进程之间的同步
多个进程之间需要写作完成任务时,经常发生业务之间的依赖现象,
从而出现了进程的同步问题.Linux下进程的同步方式主要有消息、信号量等.
信号量是一个共享的表示数量的值.用于多个进程之间操作或者共享资源的保护,
它是进程之间同步的最主要方式.

函数名: getenv
功  能: 从环境中取字符串,获取环境变量的值
头文件: stdlib.h
用  法: char *getenv(char *envvar);
函数说明:
getenv()用来取得参数envvar环境变量的内容.
参数envvar为环境变量的名称,如果该变量存在
则会返回指向该内容的指针.环境变量的格式为
envvar=value.
getenv函数的返回值存储在一个全局二维数组里,
当你再次使用getenv函数时不用担心会覆盖上次的调用结果.
返回值: 执行成功则返回指向该内容的指针,
找不到符合的环境变量名称则返回NULL.
如果变量存在但无关联值,它将运行成功并返回一个空字符串,
即该字符的第一个字节是null.

类型不一样进行赋值时,中间必须要借助一个临时变量.
如果这个临时变量产生不了,那么这个赋值操作就会失败.
产生的中间变量的类型为赋值操作的左值的类型,
然后把右值当作这个临时变量的初始值从而生成一个中间变量,
最后把这个中间变量赋给左值.

产生随机数
#include <cstdlib>
void srand(unsigned seed)
ctime头文件中的time函数
srand(time(NULL))
chrono头文件中的std::chrono::system_clock
srand(std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()));



dynamic_cast<son>(father)
typeid()



int a = 100;
void *p = (void *) (&a);
// b78f8bf0
printf("%x\n", &a);
// b78f8bf0
printf("%x\n", p);
// b78f8bf0
printf("%x\n", (int*)p);
int b;
b = *(int *) p;
b = *((int *) p);
cout << "b = " << b << endl;






运算符*被称为间接引用运算符,当使用*时,就读取它后面变量中所保存的地址处的值.
概念:
指针地址:            指针自身的地址.
指针保存的地址:       指针保存的地址.
指针地址的值:         指针保存的地址处的值.

char *p1 = "Hello World";
char **p2 = &p1;
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

int a = 100;
int *p1 = &a;
int **p2 = &p1;
fprintf(stdout, "test() *p1 %%d: %d\n", *p1);// 指向int内容
fprintf(stdout, "test() **p2 %%d: %d\n", **p2);// 指向int内容
*/
// 下面4句代码的输出只针对于char类型,像int类型就不是这样子输出
fprintf(stdout, "test() p1 %%s: %s\n", p1);// 指向字符串内容
fprintf(stdout, "test() p1 %%p: %p\n", p1);// 指向字符串内容的地址
fprintf(stdout, "test() *p2 %%s: %s\n", *p2);// 指向字符串内容
fprintf(stdout, "test() *p2 %%p: %p\n", *p2);// 指向字符串内容的地址,也就是指向p1地址处的值

fprintf(stdout, "test() &p1 %%p: %p\n", &p1);// 指向p1自身的地址,被保存到p2中了
fprintf(stdout, "test() p2 %%p: %p\n", p2);// 指向p1自身的地址
fprintf(stdout, "test() &p2 %%p: %p\n", &p2);// 指向p2自身的地址

fprintf(stdout, "test() p1[0] %%p: %p\n", p1[0]);
fprintf(stdout, "test() *p1 %%p: %p\n", *p1);
fprintf(stdout, "test() **p2 %%p: %p\n", **p2);



#include<stdio.h>
#include<limits.h> 整数的极限
#include<float.h>  浮点数的极限
#include<math.h>   数学
#include<stdlib.h>
#include<Windows.h>

int a1 = 10;十进制
int a2 = 010;八进制
int a3 = 0x10;十六进制
int a4 = 101u;无符号整数
int a5 = 102l;长整数
int a6 = 103ll;长长整数
int a7 = 0101u;


short int nummax = SHRT_MAX;  32767
short int nummin = SHRT_MIN; -32768
%d sizeof(short int); 2个字节
标准写法是short int, 不过int可以省略,但是最好不要省略

unsigned short nummax = USHRT_MAX; 65535
unsigned short nummin = USHRT_MIN; 0
USHRT_MAX加1后就变成0
unsigned int的极大值要大于int,但是unsigned int不能表示负数,因为最小为0

int intmax = INT_MAX;  2147483647
int intmin = INT_MIN; -2147483648
long int longmax = LONG_MAX;  2147483647
long int longmin = LONG_MIN; -2147483648
标准写法是long int, 不过int可以省略,但是最好不要省略
sizeof(int) 4个字节
sizeof(long int) 4个字节
int与long int在32位及以上的机器是等价的,都占用4个字节
在16位机器上,int与short int是等价的,都占用2个字节

LONG_MAX;   2147483647L
LONG_MIN;  -2147483647L-1
ULONG_MAX;  0xffffffffUL
LLONG_MAX;  9223372036854775807I64
LLONG_MIN; -9223372036854775807I64-1
ULLONG_MAX; 0xffffffffffffffffui64
long long phone = 18565603244; 存储QQ, phone
printf("%lld", phone);
unsigned long long max = ULLONG_MAX; 存储身份证号
printf("%llu", max); 18446744073709551615

float f1 = 10.5;
printf("%f", f);
32bits or 64bits are same.
sizeof(10.5);  8个字节 实数双精度常量 sizeof(double)
sizeof(10.5f); 4个字节 实数单精度常量 sizeof(float)
long double >= double指的是内存空间(取决于编译器)

int num = 3 / 5;
printf("%d", num); 0
num = 3.2;
printf("%d", num); 3 赋值号会自动转换类型,printf不会

float f2 = 19e8;
printf("%f", f2); 1900000000.000000
f2 = 19.3456e-25; e 10,指数只能是整数
3.14        double 8个字节 32bits 有效数字6~7位
3.14f 3.14F float  4个字节 64bits 有效数字15~16位

== 成立是1,不成立是0

float f3 = 1.0000000000001; 有效数字6~7位
float f4 = 1.0000000000000005;
priintf("%d", f3 == f4); 1

%.50f 保留小数点后50位
printf("%f, %f", FLT_MAX, FLT_MIN);
printf("%f, %f", DBL_MAX, DBL_MIN);

double S = sqrt(p*(p-a)*(p-b)*(p-c));



char ch = 'A'; 1个字节(字符常量)
wchar_t wch = L'字'; 宽字符
putchar(ch);
printf("%c", ch);
sizeof(ch);  1个字节
sizeof('A'); 4个字节(为了兼容和拓展宽字符,一般情况下占4个字节)
sizeof(wch); 2个字节

system("color 4f");
system("title weidi");
system("pause");

printf("%d", sizeof(""));  1个字节
printf("%d", sizeof("A")); 2个字节(字符串/0结束,等于每个字符,尾部加上'\0')
printf("%d", sizeof("节")); 2个字节,再加结束符1个

字符串不能作用于单个字符
1
'1'
0    %d 48 %c 0
'0'  %d 0  %c 空字符
'\0' %d 0  %c 空字符

int num = 1;
char ch = '1';
printf("%d, %d", sizeof(num), sizeof(ch)); 4 1 内存大小不一样
printf("%d", ch); 屏幕上输出的这个字符对应的整数,相当于求ASCCII码,ASCCII码就是整数
printf("%c", ch); 打印字符,是个字符1,不是数字1
printf("%d", num); 打印数字1
printf("%c", num); 打印数字1对应的字符
char ch1 = '\0';
char ch2 = '0';
char ch3 = 0; ch1与ch3是等价的
int num = 0;
printf("%d,%d,%d", sizeof(ch1), sizeof(ch2), sizeof(num));
printf("\n%d", ch1); '\0'的编号是0
printf("\n[%c]", ch1); '\0'空字符
printf("\n%d", ch2); 48
printf("\n%c", ch2); 0  求字符本身
printf("\n%d", ch3);
printf("\n%c", ch3);

getchar(); 等待
putchar(ch);

char str[5] = {'c','a','3','e','@'};
system(str);//windows下打开计算器
printf("%s", str);
char str[100] = {0};
sprintf(str,"color %c%c" ,'5','e');
system(str);
char str[100] = {0};
sprintf(str, "title %s的杰作", "weidi");
system(str);

int a;
&a; ok
&&a; error

const int a = 10;
int *p = 100;
printf("a = %d\n", a);

const int a; int const a; // same
int *const p;
const int *const p;

const int a; // c can this
const int a = 100; // c++ must this

char *p = malloc(100); // c can this
char *p = malloc(100); // c++ cann't this
char *p = (char*)malloc(100);

#include<iostream>
std::cout<<"...\t";
std::cout<<"...\n";
std::cout<<std::endl;
"endl"与"\n"的区别是前者不仅有后者的功能,还有调用输出流的flush的功能.
std::是个命名空间标识符,C++标准库中的函数或者对象都是在命名空间std中定义的,
所以我们要使用的标准库中的函数或者对象都要用std来限定.
使用标准库文件iostream时,要写上std;使用非标准库文件iostream.h时,则不用写.
如果在使用标准库文件iostream时,不喜欢重复地使用std,
我们可以使用一种类似通告的形式来告诉编译器我们将使用标准库函数cout和endl.
在方法中这样使用:
using std::cout;
using std::endl;
cout<<"...\t";
cout<<endl;
另外一种方式是(也在方法中使用):
using namespace std;
iostream.h是C语言格式的,iostream是C++语言格式的

在程序中使用函数时,必须先声明它然后再定义,
声明的目的是告诉编译器即将要定义的函数的名字是什么,
返回值的类型是什么以及参数是什么.
而定义则是告诉编译器这个函数的功能是什么.
假如不声明,那么该函数就不能被其他函数调用.
通常我们把函数声明叫做函数原型,而把函数定义叫函数实现.

面向对象: 抽象,封装,继承,多态

//内联函数:把这个函数的所有代码复制到调用处,这样程序执行时不用来回跳转
inline int func(int);
在不知道具体怎么操作时,最好先不要使用内联函数.
等完成程序的大部分功能时,再对符合要求的函数进行内联操作.
将类中的函数声明和函数定义合并在一起时,该函数会自动成为内联函数.
拷贝构造函数实例
Student(const Student&);
Student stu;
Student stu2 = stu;
Student stu3(stu);





在C++中如果使用到某个字符串需要转义时,
那么可以这样使用:
string str(R"(某个需要转义的字符串)");
像上面这样操作后,就不需要再转义了.
当输出str这个字符串时,里面有转义的东西也会
原样输出(如换行).如果不按照上面这种写法,
自己对字符串进行转义,那么输出这个字符串时会按照
转义的命令输出.

在64位的win上,如果字符串中有括号,那么这个左括号与
前面的字符之间要有空格.

int a[3][4];
下面的代码在有些编译上才能通过,在clion编译器上通不过.
// 栈上开辟一维数组
int *p = (int[]){0};
// 栈上开辟二维数组
// 4是必须的,因为一行有多少列必须要明确
int(*px)[4]=(int[][4]){0};
// 栈上开辟三维数组
// 3和4是必须的,列和面必须要明确
int(*py)[3][4]=(int[][3][4]){0};
// 分块数据模型相比连续数据模型有什么好处

C 错误处理
errno、perror() 和 strerror()
例:
打开一个不存在的文件
FILE *pf;
pf = fopen("unexist.txt", "rb");
if (!pf) {
    fprintf(stderr, "错误号: %d\n", errno);
    perror("通过perror输出错误");
    // 如果要用到输出错误信息的操作,那么用这句代码好了
    fprintf(stderr, "打开文件错误: %s\n", strerror(errno));
} else {
    fclose(pf);
}
输出如下:
错误号: 2
通过perror输出错误: No such file or directory
打开文件错误: No such file or directory

程序退出状态
通常情况下,程序成功执行完一个操作正常退出的时候会带有值EXIT_SUCCESS.
在这里,EXIT_SUCCESS 是宏,它被定义为 0.
如果程序中存在一种错误情况,当您退出程序时,
会带有状态值EXIT_FAILURE,被定义为 -1.
exit(EXIT_FAILURE);
exit(EXIT_SUCCESS);

C 强制类型转换
int sum = 17, count = 5;
double mean;
mean = (double) sum / count;
printf("Value of mean : %f\n", mean );
输出如下:
Value of mean : 3.400000

整数提升
int  i = 17;
char c = 'c'; /* ascii 值是 99 */
int sum;
sum = i + c;
printf("Value of sum : %d\n", sum );
输出如下:
Value of sum : 116
在执行实际加法运算时,把 'c' 的值转换为对应的 ascii 值.

int ---> unsigned int ---> long ---> unsigned long --->
long long ---> unsigned long long --->
float ---> double ---> long double

函数指针
函数指针变量的声明：
// 声明一个指向同样参数、返回值的函数指针类型
typedef int (*fun_ptr)(int,int);
int max(int x, int y)
{
    return x > y ? x : y;
}
/* p 是函数指针 */
int (* p)(int, int) = & max; // &可以省略
int a, b, c, d;
printf("请输入三个数字:");
scanf("%d %d %d", & a, & b, & c);
/* 与直接调用函数等价,d = max(max(a, b), c) */
d = p(p(a, b), c);
printf("最大的数字是: %d\n", d);

回调函数
// 回调函数
void populate_array(int *array,
                    size_t arraySize,
                    int (*getNextValue)(void))
{
    for (size_t i=0; i<arraySize; i++)
        array[i] = getNextValue();
}
// 获取随机值
int getNextRandomValue(void)
{
    return rand();
}
int myarray[10];
populate_array(myarray, 10, getNextRandomValue);
for(int i = 0; i < 10; i++) {
    printf("%d ", myarray[i]);
}
输出如下:
16807 282475249 1622650073 984943658 1144108930
470211272 101027544 1457850878 1458777923 2007237709

/***
 数组是可以在内存中连续存储多个元素的结构
 数组中的所有元素必须属于相同的数据类型
 对一个大小为N,类型为short的数组,其占据的内存大小为:
 N * sizeof(short) = N * 2
 如果说第1个元素在内存中的地址为p,那么第M个元素(M不大于N)
 在内存中的地址可表示为:
 p + (M - 1) * sizeof(short)
 */
// a是数组名,代表内存的首地址
// 数组没有被初始化时,里面存的都是垃圾值
// 只要初始化一个值,其他的值就被初始化为同一个值
int a[5] = {0};
printf("%p\n", a);
printf("%p\n", &a[0]);
printf("%d\n", *(&a[0]));
// 取数组大小(4 * 5)
printf("%d\n", sizeof(a));
for (int i = 0; i < 5; i++) {
    printf("%d, %p, %p, %d\n", a[i], &a[i], a + i, *(a + i));
}

///////////////////////////面向对象///////////////////////////

虚函数的作用:
使得通过基类类型的指针,可以使属于不同派生类的不同对象产生不同的行为.
否则基类指针访问派生类对象时访问到的总是基类的同名成员.

一个函数被说明为虚函数,在派生类中覆盖了该函数,那么该函数也是个
虚函数,不过也可以把它说明为虚函数,这样看起来更好懂些.

静态联编：编译时、运行时
动态联编：联编工作是在编译时确定的
在不使用virtual之前,C++对重载的函数使用静态联编,而使用了virtual以后,
C++则对该函数进行动态联编.
将一个调用函数者联结上正确的被调用函数,这一过程叫做函数联编,一般简称为联编.
在未加virtual说明时,该函数是静态联编,即被调函数和调用函数的关系以及它们的
内存地址在编译时都已经确立好,运行时不再发生变化.这样的好处是速度快.

在编译时的静态联编
在运行时的静态联编
在编译时的动态联编
在运行时的动态联编

在继承中,只有当使用指针或者引用的方式来调用虚函数时,虚函数才能发挥多态性的作用.
只有被说明为虚函数的那个成员函数才具有多态性.
被继承的虚函数仍然是虚函数.

在虚函数中使用成员名限定
在虚函数中使用成员名限定强行解除动态联编.
C c;
A *p = &c;
cout << p->get() << endl;
//成员名限定会强制使用静态联编来调用类A的成员函数
cout << p->A::get() << endl;

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
int main() {
	C c;
	A* p = &c;
	// 2
	cout<<p->get()<<endl;
	// 0
	cout<<p->A::get()<<endl;
	// error: ‘B’ is not a base of ‘A’(编译通不过)
  // cout << p->B::get() << endl;
  B *p2 = &c;
  cout << p2->B::get() << endl;
  总结:
  如果想要父类指针指向的函数最终调用的是子类函数,
  那么父类中的函数需要加上virtual关键字.
  如果不加,那么调用的是父类自身的函数.
}

一个派生类对象在创建时会首先调用基类的构造函数,
然后调用该类的构造函数,一般情况下,在使用虚函数时,
我们都会将派生类对象传递给指向基类的指针,那么假如
指向派生类对象的指针删除时会发生什么情况呢？如果
析构函数是虚函数,那么就会进行正确的操作,它会先调用
派生类的析构函数,由于一般情况下任何类的析构函数都可
声明为虚析构函数,当指针被删除时,系统会获得对象运行
时的类型并调用正确的析构函数.
但是要注意三点：
1.由于析构函数不允许有参数,因此它不可能实现重载,
    那么一个类就只能有一个虚析构函数.
2.只要基类的析构函数被说明为虚函数,那么派生类的析构函数无论说明与否,
    都自然成为虚函数.
3.在C++中虚构造函数是不存在的,因此也无法声明.

// 观察者模式
class Observer;
class Subject {
public:
    virtual ~Subject = 0;
    virtual void registerObserver(Observer*) = 0;
    virtual void removeObserver(Observer*) = 0;
    virtual void notifyObserver() const = 0;
};
// 这个析构函数不能少,语法这样规定的.
Subject::~Subject() {

}

class WeatherData : public Subject {

public:
    ~WeatherData(){}

    // 注意:是const_iterator,而不是iterator
    void notifyObserver const {
        for(list<Observer*>::const_iterator it = m_observers.begin();
            it != m_observers.end();
            ++it){
            Observer *obj = *it;
            obj->update(...);
        }
    }
};



///////////////////////////模板///////////////////////////

模板部分
templeate<typename Type>
typeid(Type).name()

用于方法
template<typename T> 
T sum(T a, T b);
用于类
template<typename T1, typename T2>
class Point
{
private:
	T1 x;
	T2 y;
public:
	Point(T1 _x, T2 _y): x(_x), y(_y){}
	T1 getX();
	void setX(T1 x);
	T2 getY();
	void setY(T2 y);
};
举例子:
template<class T>
swap(T &rx, T &ry) {
    T temp = rx;
    rx = ry;
    ry = temp;
}

///////////////////////////字符串///////////////////////////

美帝 窄字符
天朝 宽字符

#include <string>
#include <string.h>
using namespace std;

下列码表含有全部128个ASCII十进制、八进制、十六进制及字符编码.
十进制 	八进制 	十六进制 	字符
0 		  0 		  00 	    NUL(空)
1 	    1 	    01 	    SOH(标题开始)
2 	    2 	    02 	    STX(正文开始)
3 	    3 	    03 	    ETX(正文结束)
4 	    4 	    04 	    EOT(传送结束)
5 	    5 	    05 	    ENQ(询问)
6 	    6 	    06 	    ACK(确认)
7 	    7 	    07 	    BEL(响铃)
8 	    10 	    08 	    BS(退格)
9 	    11 	    09 	    HT(横向制表)
10 	    12 	    0a 	    LF(换行)
11 	    13 	    0b 	    VT(纵向制表)
12 	    14 	    0c 	    FF(换页)
13 	    15 	    0d 	    CR(回车)
14 	    16 	    0e 	    SO(移出)
15 	    17 	    0f 	    SI(移入)
16 	    20 	    10 	    DLE(退出数据链)
17 	    21 	    11 	    DC1(设备控制1)
18 	    22 	    12 	    DC2(设备控制2)
19 	    23 	    13 	    DC3(设备控制3)
20 	    24 	    14 	    DC4(设备控制4)
21 	    25 	    15 	    NAK(反确认)
22 	    26 	    16 	    SYN(同步空闲)
23 	    27 	    17 	    ETB(传输块结束)
24 	    30 	    18 	    CAN(取消)
25 	    31 	    19 	    EM(媒介结束)
26 	    32 	    1a 	    SUB(替换)
27 	    33 	    1b 	    ESC(退出)
28 	34 	1c 	FS(文件分隔符)
29 	35 	1d 	GS(组分隔符)
30 	36 	1e 	RS(记录分隔符)
31 	37 	1f 	US(单元分隔符)
32 	40 	20 	(空格)
33 	41 	21 	!
//34 	42 	22 	"
35 	43 	23 	#
36 	44 	24 	$
37 	45 	25 	%
38 	46 	26 	&
//39 	47 	27 	'
40 	50 	28 	(
41 	51 	29 	)
42 	52 	2a 	*
43 	53 	2b 	+
44 	54 	2c 	,
45 	55 	2d 	-
46 	56 	2e 	.
47 	57 	2f 	/
48 	60 	30 	0
49 	61 	31 	1
50 	62 	32 	2
51 	63 	33 	3
52 	64 	34 	4
53 	65 	35 	5
54 	66 	36 	6
55 	67 	37 	7
56 	70 	38 	8
57 	71 	39 	9
58 	72 	3a 	:
59 	73 	3b 	;
60 	74 	3c 	<
61 	75 	3d 	=
62 	76 	3e 	>
63 	77 	3f 	?
64 	100 	40 	@
65 	101 	41 	A
66 	102 	42 	B
67 	103 	43 	C
68 	104 	44 	D
69 	105 	45 	E
70 	106 	46 	F
71 	107 	47 	G
72 	110 	48 	H
73 	111 	49 	I
74 	112 	4a 	J
75 	113 	4b 	K
76 	114 	4c 	L
77 	115 	4d 	M
78 	116 	4e 	N
79 	117 	4f 	O
80 	120 	50 	P
81 	121 	51 	Q
82 	122 	52 	R
83 	123 	53 	S
84 	124 	54 	T
85 	125 	55 	U
86 	126 	56 	V
87 	127 	57 	W
88 	130 	58 	X
89 	131 	59 	Y
90 	132 	5a 	Z
91 	133 	5b 	[
92 	134 	5c 	\
93 	135 	5d 	]
94 	136 	5e 	^
95 	137 	5f 	_
96 	140 	60 	`
97 	141 	61 	a
98 	142 	62 	b
99 	143 	63 	c
100 	144 	64 	d
101 	145 	65 	e
102 	146 	66 	f
103 	147 	67 	g
104 	150 	68 	h
105 	151 	69 	i
106 	152 	6a 	j
107 	153 	6b 	k
108 	154 	6c 	l
109 	155 	6d 	m
110 	156 	6e 	n
111 	157 	6f 	o
112 	160 	70 	p
113 	161 	71 	q
114 	162 	72 	r
115 	163 	73 	s
116 	164 	74 	t
117 	165 	75 	u
118 	166 	76 	v
119 	167 	77 	w
120 	170 	78 	x
121 	171 	79 	y
122 	172 	7a 	z
123 	173 	7b 	{
124 	174 	7c 	|
125 	175 	7d 	}
126 	176 	7e 	~
127 	177 	7f 	DEL(删除)

string型字符串的赋值(直接进行赋值操作)
string型字符串的合并
string型字符串的部分合并
string型字符串的替换
string型字符串的复制
string型字符串的插入
string型字符串的删除
string型字符串的查找
string型字符串的比较
判断string型字符串是否为空(empty())

' ' == 32
空格的ASCII码为32,而空字符的ASCII码为0.
cin遇到不可见字符和空字符就停止输入(不可见字符指的
是制表符和空格),而cout则只是遇到空字符才停止输出.
这就量为什么将空字符作为字符串结束标志的原因,
cin和cout遇到空字符都会结束输入和输出操作.
cout遇到空字符就停止输出.因此其后面的内容不再输出.
char man[] = {'a', 0, 'b'};//b不会输出
char man[] = {'a', 32, 'b'};
输出a b后,后面还有乱码.这是因为man没有结束标志,
因此不是字符串,是一个普通的数组,当输出这个数组的
时候,发生了越界,所以就输出了乱码.因此需要加入一个
字符串的结束标志来防止越界操作.

cin.get(str, 1000)能接收空格.
char name[] = {'a','b',32,'c',0,'\0'};
后面要自己添加上'\0',这样字符数组才能转化成字符串.
char name[] = {"abc efg"};
后面不需要自己再添加'\0'.
strlen(name):不包括不可见字符(不可见：结束符)
sizeof(name):包括不可见字符
strcmp(ch1, ch2)
不能把一个数组名赋给另一个数组名
strcpy(ch1, ch2)把ch2赋给ch1
strcpy函数会将ch2中的所有字符,包括结束标志'\0'一块复制到ch1中去
str1.assign(str2, 3, 1)
strcat(ch1, ch2)
下面几个函数都是计算字符串的可见长度
strlen(str1.c_str())
str1.size()
str1.length()

strncat(ch1, ch2, 5)
str1.append(str2, 5, 3)

string a("c++");
string b(a);
string c(4, 'a');
cout<<a<<endl;//c++
cout<<b<<endl;//c++
cout<<c<<endl;//aaaa
string s1;
string s2(s1);
cout<<s1<<endl;//空行
cout<<s2<<endl;//空行

字符串是以空字符结尾的字符数组.
"\0"是一个空字符(NULL)标志,它的ASCII码为0.
哪些是空字符呢:
1.
空格

char man[12];
cin>>man;
cin.get(man, 12);
cin.get的结束标志是"\n",也就是换行,因此语句遇到空格不会结束,
而是把空格也看做一个字符.
cout遇到空字符(不带单引号的0或者带单引号的\0)就会停止输出.

//在初始化数组大小时,如果小于12,那么会报error
char man[12] = {"hello world"};
//hello world
cout<<man<<endl;
//11可见字符长度,不包括\0
cout<<"strlen(man): "<<strlen(man)<<endl;
//12数组定义多大就是多大,如果没有定义,那么结果为可见字符长度加上\0
cout<<"sizeof(man): "<<sizeof(man)<<endl;
man[5]='\0';
//hello
cout<<man<<endl;
//5
cout<<"strlen(man): "<<strlen(man)<<endl;
//12
cout<<"sizeof(man): "<<sizeof(man)<<endl;

char型字符串是C语言风格的字符串,它是用数组来保存字符串的.
在C++风格中,就是string字符串.
使用:
using std::string声明一次
std::string在各个地方都这样使用

字符数组与字符对象的不同点:
1.
string str = "string";
char ch[] = "char";
if (str == "string") {
    //成立
}
if (ch == "char") {
    //不成立(正确做法是需要对字符数组中的每个元素进行比较)
}
if (strcmp(ch, "pig") == 0) {
    //成立
}

string型字符串的赋值(直接赋值或者使用assign函数)
char型字符串无法直接赋值,如:
char ch1[] = "give me";
char ch2[] = "a cup";
ch1 = ch2;//error
不能将一个数组名直接赋给另一个数组名.
但是可以使用strcpy函数来实现赋值的目的.如:
char ch1[100] = "what ";
char ch2[] = "my name is Jack";
cout<<sizeof(ch1)<<endl;
cout<<sizeof(ch2)<<endl;
strcpy(ch1, ch2);
cout<<ch1<<endl;
cout<<ch2<<endl;
cout<<sizeof(ch1)<<endl;
cout<<sizeof(ch2)<<endl;
总结:
只要ch1的大小足够放得下ch2的字符个数,那么程序没有问题.
但是如果ch1的大小小于ch2的字符个数,虽然程序也执行了,
而且没有报错,但是程序会卡住一会儿,然后程序返回时又是一个非正常值,
因此需要保证ch1的大小要大于等于ch2的字符个数.
strcpy会将ch2中的所有字符,包括结束标志"\0"一起复制到ch1中去.

string类具有自动调节字符串大小的功能.
string str1 = " gh ";
string str2 = "q abcdef ";
str1.assign(str2, 9, 1);
cout<<str1<<endl;
str2有9个字符,不包括"\0".
9表示从str2中第9个位置(从0开始),取出1个字符赋值给str1,
因此str1的结果是个空格.
assign函数中的第2个参数不能超出str2的可见字符个数,不能编译出错.

string型字符串的合并(直接使用加法运算)
对两个char型字符串进行合并要用到strcat函数,strcat函数将第2个字符串合并
到第1个字符串,因此第1个字符串必须保证能容纳两个字符串的长度.
char ch1[] = "what't your name? ";
char ch2[] = "my name is Jack.";
strcat(ch1, ch2);
//what't your name? my name is Jack.
cout << ch1 << endl;
string型字符串的合并实例:
string str1 = "what't your name? ";
string str2 = "my name is Jack.";
str1 = str1 + str2;
cout << str1 << endl;
cout<<str1.size()<<endl;
cout<<str1.length()<<endl;
string对象调用size()和length()的结果是相同的,都是计算不包括"\0"的可见字符.
length()是早期版本的string类中的成员,而size()则是以后添加进行的,为的是兼容STL.
C++的字符串合并比C语言的字符串合并更方便有效,而且不易产生错误.

string型字符串的部分合并(使用append函数)
C语言提供了strncat函数来实现对char型字符串的部分合并,或者说部分复制.
char ch1[100] = "what't your name? ";
char ch2[] = "my name is Jack.";
strncat(ch1, ch2, 4);
//what't your name? my n
cout << ch1 << endl;
4表示从ch2中取前4个字符,然后接到ch1中去.
string类append成员函数的使用实例:
string str1 = "what't your name? ";
string str2 = "my name is Jack.";
str1.append(str2, 4, 2);
//what't your name? am
cout << str1 << endl;
4表示从str2的第4个位置(从0开始)开始,取2个字符接到str1中去.

string型字符串的替换(使用replace函数)
如果想用一个char型字符串中的几个字符替换掉另一个char型字符串,
可以使用C库中的strncpy函数.
char ch1[100] = "what't your name? ";
char ch2[] = "my name is Jack.";
strncpy(ch1, ch2, 4);
//my n't your name?
cout << ch1 << endl;
4表示从ch2中取前4个字符,把ch1中的前4个字符替换掉,其他的不变.
string类自带了一个替换函数replace,该函数可以被重载,它的参数有多种.

string型字符串的复制
char型字符串的复制.如:
char ch1[100] = "what't your name? ";
char ch2[] = "my name is Jack.";
memmove(ch1, ch2, 10);
//my name isr name?
cout << ch1 << endl;
效果跟strncpy函数一样.
string型字符串复制到char型字符串的copy函数.
string str1 = "what't your name? ";
char ch1[] = "12345";
int n = str1.copy(ch1, 4, 1);
//hat'5
cout << ch1 << endl;
4表示从str1的字符串中+复制4个字符,1表示从第1个位置(从0开始)开始,
然后把ch1的前4个字符覆盖掉.

string型字符串的插入(使用insert函数)
string str1 = "what't your name? ";
string str2 = "my name is Jack.";
str1.insert(2, str2, 0, 4);
//whmy nat't your name?
cout << str1 << endl;
从str2的第0个位置开始取4个字符,然后插入到str1中第2个位置的后面.

string型字符串的删除(使用erase函数)
string类的成员函数erase()可以根据指定的字符串中的位置,删除字符串中的字符,
同时返回指向下一个字符的this指针.
string str1 = "what't your name? ";
str1.erase(8, 2);
//what't yr name?(把第8个字符的后面2个字符给删除掉)
cout << str1 << endl;
str1.erase(5);
//what'(把第5个字符后面的所有字符给删除掉)
cout << str1 << endl;
str1.erase();
//把剩下的全部给删除掉了,因为位置默认从0开始
cout << str1 << endl;

string型字符串的查找(使用find函数)
C方式:
char ch1[15];
char *p, c = 'w';
strcpy(ch1, "hello world");
p = strchr(ch1, c);
if (p) {
    //字符w位于第6
    cout << "字符" << c << "位于第" << p - ch1 << endl;
} else {
    cout << "没有找到" << endl;
}
C++方式:
string str1("hello world");
int f = str1.find("w", 0);
if (f != string::npos) {
    //在第6个字符
    cout << "在第" << f << "个字符" << endl;
} else {
    cout << "没有找到" << endl;
}
find函数:第1个参数为要搜索的字符w,第2个参数0表示从字符串str1中的第1个字符
开始搜索.找到的话会返回该字符在str1中的位置.假如有两个w,返回第1个w的位置,
没找到的话,返回npos,也就是-1.不同的平台中可能不一样.
find_first_not_of():查找第一个与value中的某值不相等的字符
find_first_of():查找第一个与value中的某值相等的字符
find_last_of():查找最后一个与value中的某值相等的字符
find_last_not_of():查找最后一个与value中的所有值都不相等的字符
rfind():查找最后一个与value相等的字符(逆向查找)
例:"hello world" 查找'w'字符,在第6个位置(从0开始)
如果这个__pos小于等于6,那么结果返回都是6,因为从__pos位置开始查找都能找到;
如果这个__pos大于6,由于源字符串中在第6个位置后没有'w'字符了,
因此找不到就返回string::npos.

__n是针对__s而言的,就是匹配__s中前几个字符(字符是连续的).
如果这几个字符在源字符串中第一次出现了,那么就返回这个位置;
如果找不到就返回string::npos.
当__n的值大于__s中的字符个数时,返回string::npos.
当__n的值等于__s中的字符个数或者不传时,那么就是需要完全匹配.
当__n的值为0时,表示任何一个字符都匹配,因此返回的是第0个位置.
str.find(char __c, size_type __pos);
str.find(const char *__s, size_type __pos);
str.find(const char *__s, size_type __pos, size_type __n);
str.find(const basic_string &__str, size_type __pos);
例:
string testStr = "gfdvjfjbjv#vgkbznfbvcl#v#vfb#ivkngxyz";
string::size_type index = testStr.find("#v#vf", 0, 4);//22

__n是针对__s而言的,只要匹配__s中前几个字符中的某一个字符就行了.
如果这几个字符中的某一个字符在源字符串中第一次出现了,那么就返回这个位置;
如果找不到就返回string::npos.
当__n的值大于__s中的字符个数时,返回string::npos.
当__n的值等于__s中的字符个数或者不传时,那么就是需要完全匹配.
当__n的值为0时,表示任何一个字符都匹配,因此返回的是第0个位置.
str.find_first_of(char __c, size_type __pos);
str.find_first_of(const char *__s, size_type __pos);
str.find_first_of(const char *__s, size_type __pos, size_type __n);
str.find_first_of(const basic_string &__str, size_type __pos);
例:
string testStr = "gfdvjfjbjv#vgkbznfbvcl#v#vfb#ivkngxyz";
string::size_type index = testStr.find_first_of("#v#vf", 0, 4);//3

__n的值好像没有范围,传0,小于等于,大于__s字符串的个数时都没关系.
只要在源字符串中找到跟__s字符串中任何一个字符不同这样一个字符,
那么就返回一个字符的位置,如果源字符串中的各个字符都是__s中的
某个字符,那么就是查找不到,返回string::npos.
str.find_first_not_of(char __c, size_type __pos);
__pos从哪个位置开始查找(一般设为0),查找与__c内容不相同的第1个字符的位置,
如果有那么一个位置则返回的就是这个位置,如果没有返回string::npos.
str.find_first_not_of(const char *__s, size_type __pos);
str.find_first_not_of(const char *__s, size_type __pos, size_type __n);
str.find_first_not_of(const basic_string &__str, size_type __pos);
例:
string testStr = "gfdvjfjbjv#vgkbznfbvcl#v#vfb#ivkngxyz";
string::size_type index = testStr.find_first_of("#v#vf", 0, 4);//0

str.find_last_of(char __c, size_type __pos);
str.find_last_of(const char *__s, size_type __pos);
str.find_last_of(const char *__s, size_type __pos, size_type __n);
str.find_last_of(const basic_string &__str, size_type __pos);
例:
string testStr = "gfdvjfjbjv#vgkbznfbvcl#v#vfb#ivkngxyz";
index = testStr.find_last_of("#v#vf", testStr.npos, 4);//30

str.find_last_not_of(char __c, size_type __pos);
str.find_last_not_of(const char *__s, size_type __pos);
str.find_last_not_of(const char *__s, size_type __pos, size_type __n);
str.find_last_not_of(const basic_string &__str, size_type __pos);
例:
string testStr = "gfdvjfjbjv#vgkbznfbvcl#v#vfb#ivkngxyz";
index = testStr.find_last_not_of("#v#vf", testStr.npos, 4);//36

str.rfind(char __c, size_type __pos);
str.rfind(const char *__s, size_type __pos);
str.rfind(const char *__s, size_type __pos, size_type __n);
str.rfind(const basic_string &__str, size_type __pos);
例:
string testStr = "gfdvjfjbjv#vgkbznfbvcl#v#vfb#ivkngxyz";
index = testStr.rfind("#v", testStr.npos);//24

string型字符串的比较
string s1 = "155";
string s2 = "52";
char c[] = "34";
int i, o, k, l, m, n;
i = s1.compare(s2);
o = s2.compare(c);
k = s1.compare(0, 2, s2);
l = s1.compare(1, 1, s2, 0, 1);
m = s1.compare(1, 1, c, 0, 1);
n = s1.compare(1, 1, c, 1);
cout << s1 << " : " << s2 << " = " << i << endl;
cout << s2 << " : " << c << " = " << o << endl;
cout << s1[0] << s1[1] << " : " << s2 << " = " << k << endl;
cout << s1[1] << " : " << s2[0] << " = " << l << endl;
cout << s1[1] << " : " << c[0] << " = " << m << endl;
cout << s1[1] << " : " << c[0] << " = " << n << endl;

s1与s2相同时,返回0;
s1大于s2时,返回正整数;
s1小于s2时,返回负整数.
__pos1从s1的哪个位置开始比较,__n1取s1的前几个字符进行比较.
s1.compare(const char *__s2);
s1.compare(const basic_string &__s2);
s1.compare(size_type __pos1, size_type __n1, const char *__s2);
s1.compare(size_type __pos1, size_type __n1, const basic_string &__s2);
s1.compare(size_type __pos1, size_type __n1, const char *__s2, size_type __n2);
s1.compare(size_type __pos1, size_type __n1, const basic_string &__s2, size_type __pos2, size_type __n2);
例:
string s1 = "155";
string s2 = "52";
char c[] = "34";
s1[1]与s2[0]进行比较
int result = s1.compare(1, 1, s2, 0, 1);
s1[1]与c[0]进行比较
result = s1.compare(1, 1, c, 1);
strcasecmp函数和strncasecmp函数原型
函数说明:
strcasecmp()用来比较参数s1和s2字符串,
比较时会自动忽略大小写的差异.
返回值:
若参数s1和s2字符串相同则返回0.
s1长度大于s2长度则返回大于0 的值,
s1长度若小于s2长度则返回小于0的值.
函数说明：
strncasecmp()用来比较参数s1和s2字符串前n个字符,
比较时会自动忽略大小写的差异.
返回值：
若参数s1和s2字符串相同则返回0.
s1若大于s2则返回大于0的值,
s1若小于s2则返回小于0的值.

判断string型字符串是否为空
成员函数empty()
如果字符串为空则empty()返回真(true),否则返回假(false).

swap()交换两个字符串的内容
交换两个char型字符串的函数swab(),只能交换字符串中的相邻的偶数字节和奇数字节的位置.
char ch1[15] = "ofru";
char ch2[15] = "";
swab(ch1, ch2, strlen(ch1));
//ofru
cout << ch2 << endl;
//four
cout << ch1 << endl;
C++方式:
string str1 = "give me";
string str2 = "a cup";
str1.swap(str2);
cout << str1 << endl;// a cup
cout << str2 << endl;// give me

将string型字符串转为char型字符串
string类的成员函数c_str()可以返回一个指向char型
字符串的const指针,该指针指向调用该函数的string型
字符串对象.
string str = "hello world";
const char *ch;
ch = str.c_str();
cout<< ch <<endl;

char型字符串与函数
char型字符串也就是C风格的字符串,它由一串字符组成,
结尾为字符串结束标志'\0'.字符串名是第1个字符的地址,
因此我们将字符串名作为参数传递到函数中时,其实就是将
地址传递到函数中去.由于字符串的最后一个字符是'\0',
因此我们不必传递字符串长度,只要在函数中设置一个循环体,
把结束字符作为循环结束的条件即可.
char ch[15] = "hello world";
char *p = "very well very well";
//可见字符的长度
cout << strlen(ch) << endl;
//可见字符的长度
cout << strlen(p) << endl;
//数组在定义时的大小
cout << sizeof(ch) << endl;
//不管有多少个字符,大小都8(可能不同的平台大小会不一样)
cout << sizeof(p) << endl;

字符串有3种表达方式:
1.char型数组
char ch[12] = "study";
2.未命名数组
"study"
3.指向char型数组的指针
char *p = "study";
由于这3种表达方式都是以指针的方式来保存字符串中
第1个字符的内存地址,因此都可作为参数传递到函数中去.

结构体是C时代的产物,到了C++之后结构体有了巨大的变化,
可以增加函数,可以设置成员的公有,私有和保护属性,可以
从别的类继承,也可以被别的类继承,可以有虚函数.唯一与
类不同之处是结构体定义中默认情况下的成员是public,
而类定义中默认情况下的成员是private.
struct people {
    people(double t_weight, double t_tall, int t_age,
           string t_name, char *t_native, bool t_sex);

    double weight;
    double tall;
    int age;
    string name;
    char *native;
    bool sex;
};
people::people(double t_weight, double t_tall, int t_age,
               string t_name, char *t_native, bool t_sex) {
    weight = t_weight;
    tall = t_tall;
    age = t_age;
    name = t_name;
    native = t_native;
    sex = t_sex;
}
people Jack(180.5, 179.3, 34, "Jack", "上海", 1);
people Alice(178.8, 169.8, 30, "Alice", "上海", 1);

结构体的赋值
前提是两个结构体必须是同类型的.
Jack = Alice;

结构体与函数
struct time_ {
    int hour;
    int minute;
};
//按址返回(这样不需要再复制数据)
time_ &sum(time_ t1, time_ t2) {
    time_ *total = new time_;
    total->hour = t1.hour + t2.hour + (t1.minute + t2.minute) / 60;
    return *total;
}
time_ one = {8, 15};
time_ two = {8, 15};
time_ day = sum(one, two);
time_ *day_ = &sum(one, two);

结构体与string

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







///////////////////////////STL///////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>

vector<typename T> c;
c.at(index);
c.front();
c.back();
c[index];
c.push_back(obj);

迭代器相关的函数：
begin容器中第一个元素
end最后一个元素的下一个位置
rbegin逆向迭代器的第一个元素
rend逆向迭代器的最后一个元素的下一个元素

find():Temlpate<class InputIterator, class T> inline
find_if():Temlpate<class InputIterator, class T, class Predicate> inline

for_each(c.begin(), c.end(), print);
Vector<int>::iterator location_index;
location_index = find(c.begin(), c.end(), 5);
location_index = find_if(c.begin(), c.end(), bind2nd(greater<int>(), 5));

STL的组成构件
STL的组件中最主要的是容器、迭代器、算法和仿函数.
容器(container):用来管理某类对象的集合
迭代器(iterator):用来在一个对象群集的元素上进行遍历动作
函数对象(function object)
算法(algorithms):用来处理群集内的元素

标准模板库：
1.容器：
1-1.顺序容器
vector：从后面快速的插入与删除,直接访问任何元素
deque：从前面或后面快速的插入与删除,直接访问任何元素
list：双链表,从任何地方快速插入与删除
#include <vector>
#include <deque>
#include <list>
vector<string> vt;
deque<int> dq;
list<student> lt;
C<T> c:创建一个名为c的容器,容器类型为C,如vector
或list,T为容器内元素的类型.适用于所有容器.
C c2(c)：创建一个c容器的副本,c2和c必须具有相同的容器类型和元素类型,适用于所有容器.
C c(b, e)：创建一个名为c的容器,元素是迭代器b,
e标示范围内的副本,适用于所有容器.
C c(n, t)：创建一个名为c的容器,元素为
n个个数,值为t,t的类型必须是容器C的元素类型或可以转换为该类型,只适用于顺序容器.
C c(n):创建一个名为c的容器,元素为n个初始化元素的值,元素类型为值n的类型,只适用于顺序容器.

函数：
a)begin和end
返回容器的迭代器,通过迭代器我们可以访问容器内的元素.
std::vector<int>::iterator iter = c.begin();
c.begin();
c.end();
c.rebegin();
c.rend();

b)添加元素
c.push_back():在容器尾部添加值为t的元素,
返回void.
c.push_front()：在容器头部添加值为t的元素,
返回void,只适用于list和deque.
c.insert(p, t)：在迭代器p所指向的元素前面插入
值为t的元素,返回指向t的迭代器.
c.insert(p, n, t)：在迭代器p所指向的元素前面插入
n个值为t的元素,返回void.
c.insert(p, b, e)：在迭代器p所指向的元素前面插入
由迭代器b和e标记的范围内的元素,返回void.

c)获得容器大小
c.size()：返回容器内元素个数,
返回类型为c::size_type.
c.max_size()：返回容器内最多可容纳的元素个数,
返回类型为c::size_type.
c.empty()：测试容器内是否有元素.
c.resize()：重新调整容器大小,使其能容纳n个元素.
c.resize(n, t)：重新调整容器大小,逾期能容纳n个
元素,新添加元素以值t进行初始化.

d)访问容器元素
c.front()：返回容器内第一个元素的引用,如果c为
空,该操作未定义.
c.back()：返回容器内最后一个元素的引用,如果c为
空,该操作未定义.
c[n] at方法：返回下标为n的引用,n越界时,该操作
未定义,只用于vector和deque.

e)删除元素
c.erase(p)：删除迭代器p指向的元素,返回一个指向
被删除元素后面的元素的迭代器.
c.erase(b, e)：删除迭代器b和e标记范围内的所有
元素,返回一个指向被删除元素段后面的元素的
迭代器.
c.clear()：删除容器内的所有元素,返回void.
c.pop_front()：删除容器的第一个元素,返回void,只
适用于list和deque.
c.pop_back()：删除容器的最后一个元素,返回void.

f)赋值操作
c1 = c2：删除c1的所有元素,将c2的所有元素复制
给c1,c1和c2的容器类型及元素类型必须相同.
c1.swap(c2)：交换c1和c2中的所有元素,c1和c2的容器
类型及元素类型必须相同.
c.assign(b, e)：重新给c赋值,内容为b和e所标记范围内
的元素,b和e必须不是指向c中的元素的迭代器.
c.assign(n, t)：将c中的元素重新调转为n个值为t的
元素.

1-2.关联容器
set：快速查找,不允许有重复的值
    //没有multiset这样的头文件
    multiset：快速查找,允许有重复的值
map：一对多映射,基于关键字快速查找,不允许有重复的值
    //没有multimap这样的头文件
    multimap：一对多映射,基于关键字快速查找,允许有重复的值

1-3.容器适配器
stack：先进后出
queue：先进先出
    //没有priority_queue这样的头文件
    priority_queue：最高优先级元素,问题第一个出列
    
C++ map注意事项
1、在map中,由key查找value时,首先要判断map中是否包含key.
2、如果不检查,直接返回map[key],可能会出现意想不到的行为.如果map包含key,没有问题,如果map不包含key,使用下标有一个危险的副作用,会在map中插入一个key的元素,value取默认值,返回value.也就是说,map[key]不可能返回null.
3、map提供了两种方式,查看是否包含key,m.count(key),m.find(key).
4、m.count(key)：由于map不包含重复的key,因此m.count(key)取值为0,或者1,表示是否包含.
5、m.find(key)：返回迭代器,判断是否存在.
6、对于下面的场景,存在key就使用,否则返回null,有下面两种写法：
1 if(m.count(key)>0)
2 {
3     return m[key];
4 }
5 return null;

1 iter = m.find(key);
2 if(iter!=m.end())
3 {
4     return iter->second;
5 }
6 return null;
这里需要注意：前一种方法很直观,但是效率差很多.因为前面的方法,需要执行两次查找.因此,推荐使用后一种方法.

2、迭代器
3、算法
4、仿函数

下面是具体使用部分:
map<string, string> nameAndContentMap;
// 插入元素
nameAndContentMap.insert(
	map<string, string>::value_type("", ""));
// map输出元素
for (map<string, string>::iterator iter = nameAndContentMap.begin();
	iter != nameAndContentMap.end();
	++iter) {
    // 键:iter->first 值:iter->second
    cout << iter->first << " = " << iter->second << endl;
}
// vector输出元素
vector<string> localDestFileContentVector;
localDestFileContentVector.push_back("");
size_t size = localDestFileContentVector.size();
// 第一种方式:
for (int i = 0; i < size; i++) {
    cout << localDestFileContentVector[i] << endl;
}
// 第二种方式:
for (vector<string>::iterator it = localDestFileContentVector.begin();
	it != localDestFileContentVector.end(); 
	++it) {
	cout << *it << "\t";
}
// 第三种方式:
for (auto destContent : localDestFileContentVector) {
    if (destContent.find("</resources>") == string::npos) {
        fputs((destContent + "\n").c_str(), destFile);
    } else {
        fputs((destContent).c_str(), destFile);
    }
}

vector<string> testVector;
testVector.push_back("test1");
testVector.push_back("test2");
testVector.push_back("test3");
testVector.push_back("test4");
testVector.push_back("test3");
for (auto tempStr : testVector) {
    cout << tempStr << endl;
}
//根据元素查找索引
std::vector<string>::iterator iter = 
	std::find(std::begin(testVector),
	           std::end(testVector),
	           "test4");
auto index = std::distance(std::begin(testVector), iter);
cout << "index: " << index << endl;
//根据索引修改元素
testVector[index] = "test10";
//根据索引删除元素
testVector.erase(testVector.begin() + index);
//根据索引插入元素(索引值不要超过vector的元素个数)
//原来index的位置的元素往后移动
testVector.insert(testVector.begin() + index, "test8");




///////////////////////////FILE///////////////////////////

判断文件或者目录是否存在：
在windows环境下头文件为：
#include <io.h>
在linux环境下头文件为：
#include <unistd.h>
int access(const char* _Filename, int _AccessMode);
上述函数在windows和linux环境下均可使用
该函数功能为确定文件或目录的访问权限,
如果指定的访问权限有效,则函数返回0,否则返回-1.
参数说明:
_Filename可以是文件路径,也可以是目录路径,
         可以使用绝对路径或相对路径.
_AccessMode表示要验证的文件访问权限,
           有可读、可写、可执行以及是否存在四种权限,
           当_Filename表示目录时仅能查询目录是否存在.
_AccessMode：
    头文件unistd.h中有如下定义：
    #define R_OK 4 /* Test for read permission. */
    #define W_OK 2 /* Test for write permission. */
    #define X_OK 1 /* Test for execute permission. */
    #define F_OK 0 /* Test for existence. */
    具体含义如下：
    R_OK 只判断是否有读权限
    W_OK 只判断是否有写权限
    X_OK 判断是否有执行权限
    F_OK 只判断是否存在
    在宏定义里面分别对应：
    00 只存在
    02 写权限
    04 读权限
    06 读和写权限
_AccessMode=00表示只判断是否存在
_AccessMode=02表示文件是否可执行
_AccessMode=04表示文件是否可写
_AccessMode=06表示文件是否可读
// 判断目录是否存在
int result = access("/root/temp_dir/test/", F_OK);
if (result == -1) {
    // root和temp_dir必须先存在
    // test表示目录的话,最好在其后面加上"/"
    mkdir("/root/temp_dir/test/", S_IRWXU);
}
// 判断文件是否存在
result = access("/root/temp_dir/test/strings.xml", F_OK);
if (result == -1) {
    destFile = fopen("/root/temp_dir/test/strings.xml", "w");
    fclose(destFile);
    destFile = nullptr;
}

创建目录:
父目录必须存在,只要有一个父目录不存在,则创建失败.
如果父目录不存在,则只能一级一级先创建好父目录.
windows环境下头文件为:
#include <direct.h>
函数原型为：
int mkdir(const char *_Path);
该函数功能为建立一个新的目录,创建成功则返回0,否则返回-1.
_Path：新建目录路径,可使用绝对路径,可也用相对路径.
windows环境下也可用函数_mkdir:
_mkdir(const char *_Path)
默认mode是0777,表示最大可能的访问权
linux环境下头文件为:
#include <sys/types.h>
#include <sys/stat.h>
函数原型为：
int mkdir(const char *pathname, mode_t mode);
该函数功能为创建一个新的目录,并指定它的执行权限.
如果创建成功则返回0,否则,返回-1.
S_IRWXU(如果是我使用的话就用这个吧)
00700权限,代表该文件所有者拥有读,写和执行操作的权限
S_IRUSR(S_IREAD)
00400权限,代表该文件所有者拥有可读的权限
S_IWUSR(S_IWRITE)
00200权限,代表该文件所有者拥有可写的权限
S_IXUSR(S_IEXEC)
00100权限,代表该文件所有者拥有执行的权限
S_IRWXG()
00070权限,代表该文件用户组拥有读,写和执行操作的权限
S_IRGRP()
00040权限,代表该文件用户组拥有可读的权限
S_IWGRP()
00020权限,代表该文件用户组拥有可写的权限
S_IXGRP()
00010权限,代表该文件用户组拥有执行的权限
S_IRWXO()
00007权限,代表其他用户拥有读,写和执行操作的权限
S_IROTH()
00004权限,代表其他用户拥有可读的权限
S_IWOTH()
00002权限,代表其他用户拥有可写的权限
S_IXOTH()
00001权限,代表其他用户拥有执行的权限
可叠加使用,
如0755表示S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH.
表示该文件所有者拥有读,写和执行操作权限去,
该文件用户组拥有可读,可执行的权限,
其他用户拥有可读,可执行的权限. 

创建多级目录,其中某些父目录是不存在的
void split(vector<string> &resultVector,
           const string &content,
           const string &pattern) {
    char *strTemp1 = new char[strlen(content.c_str()) + 1];
    strcpy(strTemp1, content.c_str());
    char *strTemp2 = strtok(strTemp1, pattern.c_str());
    while (strTemp2 != NULL) {
        resultVector.push_back(string(strTemp2));
        strTemp2 = strtok(NULL, pattern.c_str());
    }
    delete[] strTemp1;
    strTemp1 = NULL;
    strTemp2 = NULL;
}
string path = "/root/temp_dir/weidi/weidi/weidi/";
vector<string> resultVector;
split(resultVector, path, "/");
int resultVectorSize = resultVector.size();
string pathTemp;
for (int i = 0; i < resultVectorSize; i++) {
    if (!pathTemp.empty() && pathTemp.rfind("/") == string::npos) {
        pathTemp = pathTemp + "/" + resultVector[i] + "/";
    } else {
        pathTemp = pathTemp + "/" + resultVector[i];
    }
    if (i == resultVectorSize - 1) {
        pathTemp += "/";
    }
    int ret = access(pathTemp.c_str(), F_OK);
    if (ret == -1) {
        mkdir(pathTemp.c_str(), S_IRWXU);
    }
}
cout << pathTemp << endl;

删除目录(只能删除空的目录)：
windows环境下头文件：
#include <direct.h>
linux环境下头文件：
#include <dirent.h>
函数原型为：
int rmdir(const char *_Path);
函数功能是删除参数指定的目录,成功返回0,否则返回-1.
在windows环境下也可使用函数_rmdir.

C++方式:
C++文件流：
fstream　　 // 输入输出文件流
ifstream　 // 输入文件流
ofstream　 // 输出文件流
在C++中,有一个stream这个类,所有的I/O都以这个"流"类为基础的,
包括我们要认识的文件I/O,stream这个类有两个重要的运算符：
1、插入器(<<)
2、析取器(>>)
在C++中,对文件的操作是通过stream的子类fstream(file stream)
来实现的,所以,要用这种方式操作文件,就必须加入头文件fstream.h.
file1 << setw(20) << "姓名:" << "廉东方" << endl;

imbue(locale("chs"));设置中文模式

#include <stdio.h>
char buf[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
FILE *fp1 = fopen("c:\\test.dat", "w");
fwrite(buf, 1, 10, fp1);
fclose(fp1);
FILE *fp2 = fopen("c:\\test1.dat", "wb");
fwrite(buf, 1, 10, fp2);
fclose(fp2);
我们注意到,
第一段定义一个数组,每个分量的值为：10,等价的十六进制为0A.
第二段先是打开一个文件test1.dat进行写操作,
     再是把10个数据写入文件,最后关闭文件.
第三段功能差不多,只是文件名换成了test2.dat,
     打开文件的模式换成了wb.
第二段和第三段有什么区别呢？我们看一下最后的文件内容：
test1.dat:  0D 0A 0D 0A 0D 0A 0D 0A 0D 0A 0D 0A 0D 0A 0D 0A 0D 0A 0D 0A
test2.dat:  0A 0A 0A 0A 0A 0A 0A 0A 0A 0A
原因是为什么呢？这就是w 和 wb的区别,
w是以文本方式打开文件,wb是二进制方式打开文件,
以文本方式打开文件时,fwrite函数每碰到一个0x0A时,
就在它的前面加入0x0D.其它内容不做添加操作.
r 以只读方式打开文件,该文件必须存在.
r+ 以可读写方式打开文件,该文件必须存在.
rb+ 读写打开一个二进制文件,只允许读写数据.
rt+ 读写打开一个文本文件,允许读和写.
w 打开只写文件,若文件存在则文件长度清为0,即该文件内容会消失.若文件不存在则建立该文件.
w+ 打开可读写文件,若文件存在则文件长度清为零,即该文件内容会消失.若文件不存在则建立该文件.
a 以附加的方式打开只写文件.若文件不存在,则会建立该文件,如果文件存在,写入的数据会被加到文件尾,即文件原先的内容会被保留.(EOF符保留)
a+ 以附加方式打开可读写的文件.若文件不存在,则会建立该文件,如果文件存在,写入的数据会被加到文件尾后,即文件原先的内容会被保留. (原来的EOF符不保留)
wb 只写打开或新建一个二进制文件;只允许写数据.
wb+ 读写打开或建立一个二进制文件,允许读和写.
wt+ 读写打开或着建立一个文本文件;允许读写.
at+ 读写打开一个文本文件,允许读或在文本末追加数据.
ab+ 读写打开一个二进制文件,允许读或在文件末追加数据.
　　上述的形态字符串都可以再加一个b字符,如rb、w+b或ab＋等组合,加入b 字符用来告诉函数库打开的文件为二进制文件,而非纯文字文件.不过在POSIX系统,包含Linux都会忽略该字符.由fopen()所建立的新文件会具有S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH(0666)权限,此文件权限也会参考umask 值


一、打开文件
void open(const char* filename, int mode, int access);
// 有默认值
void open(const char *filename,
            ios_base::openmode mode = ios_base::in | ios_base::out);
void open(const wchar_t *_Filename,
            ios_base::openmode mode = ios_base::in | ios_base::out,
            int prot = ios_base::_Openprot);
参数：
filename:	打开的文件名
mode:		打开文件的方式
access:		打开文件的属性
打开文件的方式
在类ios(是所有流式I/O类的基类)中定义,常用的值如下：
ios::in　　　 			文件以输入方式打开
ios::out　　　			文件以输出方式打开
ios::ate　　			文件打开后定位到文件尾,ios:app就包含有此属性
ios::app　　　			以追加的方式打开文件
ios::trunc		　　  如果文件存在,把文件长度设为0
ios::nocreate       不建立文件,所以文件不存在时打开失败　
ios::noreplace      不覆盖文件,所以打开文件时如果文件存在失败
ios::binary 　       以二进制方式打开文件,缺省的方式是文本方式.两种方式的区别见前文

ios::in　　　　			= 0x01,　//供读,文件不存在则创建(ifstream默认的打开方式)
ios::out　　　 		= 0x02,　//供写,文件不存在则创建,若文件已存在则清空原内容(ofstream默认的打开方式)
ios::ate　　　 		= 0x04,　//文件打开时,指针在文件最后.可改变指针的位置,常和in、out联合使用
ios::app　　　 		= 0x08,　//供写,文件不存在则创建,若文件已存在则在原文件内容后写入新的内容,指针位置总在最后
ios::trunc　　 		= 0x10,　//在读写前先将文件长度截断为0(默认)
ios::nocreate　	    = 0x20,　//文件不存在时产生错误,常和in或app联合使用
ios::noreplace 	    = 0x40,　//文件存在时产生错误,常和out联合使用
ios::binary　　		= 0x80　 //二进制格式文件
可以用"或"把以上属性连接起来,如ios::out|ios::binary
打开文件的属性
0：普通文件,打开访问
1：只读文件
2：隐含文件
4：系统文件
filebuf::openprot;　　　//默认的兼容共享方式
filebuf::sh_none;　　　 //独占,不共享
filebuf::sh_read;　　　 //读共享
filebuf::sh_write;　　　//写共享
可以用"或"或者"+"把以上属性连接起来,
如3或1|2就是以只读和隐含属性打开文件.

例如:
第一种方式打开文件
fstream file1("c:\config.sys");
第二种方式打开文件
fstream file1;
file1.open("c:\config.sys", ios::binary|ios::in, 0);
如果open函数只有文件名一个参数,则是以读/写普通文件打开,即：
file1.open("c:\config.sys");
<=>
file1.open("c:\config.sys", ios::in|ios::out, 0);

fstream有两个子类：
ifstream(input file stream),默认以输入方式打开文件
ofstream(outpu file stream),默认以输出方式打开文件
所以,在实际应用中,根据需要的不同,选择不同的类来定义：
如果想以输入方式打开,就用ifstream来定义;
如果想以输出方式打开,就用ofstream来定义;
如果想以输入/输出方式来打开,就用fstream来定义.
//默认以ios::in的方式打开文件,文件不存在时操作失败
ifstream f("d:\\12.txt",ios::nocreate);
//默认以ios::out的方式打开文件
ofstream f("d:\\12.txt");
//以读写方式打开二进制文件
fstream　f("d:\\12.dat",ios::in|ios::out|ios::binary);
fstream f;
//利用同一对象对多个文件进行操作时要用到open函数
f.open("d:\\12.txt",ios::out);
成功：
　　if(f){...}　　　　　　　//对ifstream、ofstream对象可用,fstream对象不可用.
　　if(f.good()){...}
失败：
　　if(!f){...}　　　　　　 // !运算符已经重载
　　if(f.fail()){...}

二、关闭文件
打开的文件使用完成后一定要关闭,fstream提供了成员函数close()来完成此操作,
如：
file1.close();
就把file1相连的文件关闭.

三、读写文件
读写文件分为文本文件和二进制文件的读写.
1、文本文件的读写
文本文件的读写很简单：
用插入器(<<)向文件输出;
用析取器(>>)从文件输入.
file << "I Love You";//向文件写入字符串"I Love You"
int i;
file >> i;//从文件读取一个整数值
操纵符 							功能										 输入/输出
dec 								格式化为十进制数值数据			  输入和输出
endl 								输出一个换行符并刷新此流		 输出
ends 								输出一个空字符							输出
hex 								格式化为十六进制数值数据 		 输入和输出
oct 								格式化为八进制数值数据 			输入和输出
setpxecision(int p) 设置浮点数的精度位数				输出
例1:
ofstream out("out.txt");
if (out.is_open()) 
{
  out << "This is a line.\n";
  out << "This is another line.\n";
  out.close();
}
例2:
char buffer[256];
ifstream in("test.txt");
if (!in.is_open())
{ 
	cout << "Error opening file"; 
	exit (1);
}
while (!in.eof() )
{
  in.getline(buffer,100);
  cout << buffer << endl;
}
2、二进制文件的读写
使用put()函数
put()函数向流写入一个字符,其原型是
	ofstream &put(char ch);
使用也比较简单,如
file.put('c');
就是向流写一个字符'c'.

使用get()函数
	第一个get函数
	ifstream &get(char &ch);//与上面的put函数是对应的
从流中读取一个字符,结果保存在引用ch中,如果到文件尾,返回空字符'\0'.
如
file.get(x);
表示从文件中读取一个字符,并把读取的字符保存在x中.
	第二个get函数
	int get();
从流中返回一个字符,如果到达文件尾,返回EOF.
如
int x=file.get();
	第三个get函数
	ifstream &get(char *buf, int num, char delim=' ');
把字符读入由buf指向的数组中,直到读入了num个字符或遇到了由delim指定的字符,
如果没使用 delim 这个参数,将使用缺省值换行符' '.
file.get(str1, 127, 'A');
//从文件中读取字符到字符串str1,当遇到字符'A'或读取了127个字符时终止.
例1:
ifstream fin("d:\\简介.txt",ios::nocreate);
if(!fin){
  cout<<"File open error!\n";
  return;
}
char c;
while((c=fin.get())!=EOF)cout<<c;
fin.close();
例2:
ifstream fin("d:\\简介.txt",ios::nocreate);
if(!fin){
  cout<<"File open error!\n";
  return;
}
char c[80];
while(fin.get(c,80,'\0')!=NULL)cout<<c;
fin.close();
例3:
ifstream fin("d:\\简介.txt",ios::nocreate);
if(!fin){
  cout<<"File open error!\n";
  return;
}
char c[80];
while(!fin.eof())            //判断文件是否读结束
{
  fin.read(c,80);
  cout.write(c,fin.gcount());
}
fin.close();
例4:
ifstream fin("C:\\1.exe",ios::nocreate|ios::binary);
if(!fin){
  cout<<"File open error!\n";
  return;
}
ofstream fout("C:\\2.exe",ios::binary);
char c[1024];
while(!fin.eof())
{
  fin.read(c,1024);
  fout.write(c,fin.gcount());
}
fin.close();
fout.close();
cout<<"Copy over!\n";

读写数据块
读写二进制数据块,使用成员函数read()和write()成员函数.
	read(unsigned char *buf, int num);
	write(const unsigned char *buf, int num);
read()从文件中读取 num 个字符到 buf 指向的缓存中,
如果在还未读入 num 个字符时就到了文件尾,
可以用成员函数 int gcount();来取得实际读取的字符数;
而 write() 从buf 指向的缓存写 num 个字符到文件中,
值得注意的是缓存的类型是 unsigned char *,有时可能需要类型转换.
如
unsigned char str1[]="I Love You";
int n[5];
ifstream in("xxx.xxx");
ofstream out("yyy.yyy");
//把字符串str1全部写到yyy.yyy文件中
out.write(str1, strlen(str1));
//从xxx.xxx文件中读取指定个整数,注意类型转换
in.read((unsigned char*)n, sizeof(n));
in.close();
out.close();

四、检测EOF
成员函数eof()用来检测是否到达文件尾,
如果到达文件尾返回非0值,否则返回0.
原型是
int eof();
例：　　
if(in.eof())ShowMessage("已经到达文件尾！");
状态标志符的验证(Verification of state flags)
    bad()
    如果在读写过程中出错,返回 true .
    例如：当我们要对一个不是打开为写状态的文件进行写入时,
    或者我们要写入的设备没有剩余空间的时候.
    fail()
    除了与bad() 同样的情况下会返回 true 以外,
    加上格式错误时也返回true ,例如当想要读入一个整数,而获得了一个字母的时候.
    eof()
    如果读文件到达文件末尾,返回true.
    good()
    这是最通用的：如果调用以上任何一个函数返回true 的话,此函数返回 false .
要想重置以上成员函数所检查的状态标志,你可以使用成员函数clear(),没有参数.

五、文件定位
和C的文件操作方式不同的是,C++ I/O系统管理两个与一个文件相联系的指针.
一个是读指针,它说明输入操作在文件中的位置;
另一个是写指针,它下次写操作的位置.
每次执行输入或输出时,相应的指针自动变化.
所以,C++的文件定位分为读位置和写位置的定位,对应的成员函数是 
istream &seekg(streamoff offset, seek_dir origin);设置读位置
ostream &seekp(streamoff offset, seek_dir origin);设置写位置
streamoff定义于iostream.h中,定义有偏移量offset所能取得的最大值,
seek_dir表示移动的基准位置,是一个有以下值的枚举:
ios::beg：　0　	文件开头
ios::cur：　1　	文件当前位置
ios::end：　2　	文件结尾
这两个函数一般用于二进制文件,因为文本文件会因为系统对字符的解释而可能与预想的值不同.
例：
//把文件的读指针从当前位置向后移1234个字节
file1.seekg(1234, ios::cur);
//把文件的写指针从文件开头向后移1234个字节
file2.seekp(1234, ios::beg);

//绝对移动 输入流操作
seekg(绝对位置);　　　　　　
//相对操作
seekg(相对位置,参照位置);　 
//返回当前指针位置
tellg();　　　　　　　　　　
//绝对移动 输出流操作
seekp(绝对位置);　　　　　　
//相对操作　　　
seekp(相对位置,参照位置);
//返回当前指针位置
tellp();

缓存和同步(Buffers and Synchronization)
当我们对文件流进行操作的时候,它们与一个streambuf类型的缓存(buffer)联系在一起.
这个缓存(buffer)实际是一块内存空间,作为流(stream)和物理文件的媒介.
例如,对于一个输出流, 每次成员函数put (写一个单个字符)被调用,
这个字符不是直接被写入该输出流所对应的物理文件中的,而是首先被插入到该流的缓存(buffer)中.
当缓存被排放出来(flush)时,它里面的所有数据或者被写入物理媒质中(如果是一个输出流的话),
或者简单的被抹掉(如果是一个输入流的话).这个过程称为同步(synchronization),
它会在以下任一情况下发生：
    当文件被关闭时: 在文件被关闭之前,所有还没有被完全写出或读取的缓存都将被同步.
    当缓存buffer 满时:缓存Buffers 有一定的空间限制.当缓存满时,它会被自动同步.
    控制符明确指明:当遇到流中某些特定的控制符时,同步会发生.这些控制符包括：flush 和endl.
    明确调用函数sync(): 调用成员函数sync() (无参数)可以引发立即同步.
    这个函数返回一个int 值,等于-1 表示流没有联系的缓存或操作失败.

fstream file("/values/strings.xml");
string alineString;
while (getline(file, alineString)) {
    cout << alineString << endl;
}
file.close();




















///////////////////////////子线程///////////////////////////

并发: 指在同一时刻,只能有一条指令执行,
      但多个进程被快速轮换执行,使得在宏观上具有
      多个进程同时执行的效果(单核).
并行: 指在同一时刻,有多条指令在多个处理器上
      同时执行(真正的同时发生,多核).
同步: 彼此有依赖关系的调用不应该"同时发生",
      而同步就是要阻止那些"同时发生"的事情.
异步: 任何两个彼此独立的操作是异步的,它表明事情独立的发生.

对于我来说,第三种方式使用方便点,因此在一般情况下用第三种好了.

#include <thread>
线程类thread
主要方法:
join()
detach()
joinable()
使用说明:
join()让主线程等待子线程的完成
detach()让主线程与子线程分离,子线程就成了后台线程.
如果主线程退出了,子线程就不再执行.
joinable()条件判断.
返回true时,线程对象可以join()或者detach();
返回false时,线程对象不能join()或者detach().
如果调用了join()后就不能再调用join()或者detach(),
如果调用了detach()后也不能再调用join()或者detach().
因此最好的做法是调用join()或者detach()之前,
先调用joinable()方法判断一下,返回true时再调用.
第一种使用线程方法:
1.
void testThread() {
    cout << "我是子线程,现在正在执行任务..." << endl;
}
2.
thread childThread(testThread);
if (childThread.joinable()) {
    childThread.join();
}

第二种使用线程方法:
1.创建类,void operator()() {...}这个方法少不了
class MyThread {
public:
    // 如果没有这个方法,那么使用MyThread类创建的对象就不是可调用对象.
    // 如果不是可调用对象,那么这个类就是普通类,不能作为线程对象的参数.
    void operator()() {
        cout << "我是子线程,现在正在执行任务..." << endl;
    }
};
2.
MyThread myThread;
// thread类对象参数必须是可调用对象.
thread childThread(myThread);
if (childThread.joinable()) {
    childThread.join();
}

第三种使用线程方法:
用lambda表达式.
auto testThread = [] {
    cout << "我是子线程,现在正在执行任务..." << endl;
};
thread childThread(testThread);
if (childThread.joinable()) {
    childThread.join();
}




cat /usr/include/asm-generic/errno.h
man pthread_create

gcc -o test_thread test_thread.c -lpthread

linux多线程
#include <pthread.h>
Linux系统下的多线程遵循POSIX标准,叫做pthread.
编写Linux下的线程需要包含头文件pthread.h,在生成
可执行文件的时候需要链接库
libpthread.a或者libpthread.so.

                线程                  进程
标识符类型      pthread_t            pid_t
获取id         pthread_self()       getpid()
创建           pthread_create()     fork()


typedef unsigned long int pthread_t;线程标识符

extern int pthread_create __P (
    // 参数一: pthread_t指针,因此传递的参数是一个地址
    pthread_t *__thread,
    // 参数二: 线程属性
    __const pthread_attr_t *__attr,
    // 参数三: 函数指针
    void *(*__start_routine)(void *),
    // 参数四: 函数参数
    void *__arg);
参数说明:
第一个参数为指向线程标识符的指针.
第二个参数用来设置线程属性,
    为NULL时将生成默认属性的线程
第三个参数是线程运行函数的起始地址(意思就是执行线程代码的函数),
第四个参数是运行函数的参数,不需要传递参数时赋为NULL.
返回值说明:
当创建线程成功时,函数返回0,
若不为0则说明创建线程失败,常见的错误返回代码为EAGAIN和EINVAL.
前者表示系统限制创建新的线程,例如线程数目过多了;
后者表示第二个参数代表的线程属性值非法.

extern int pthread_join __P ((
     // 参数一: 线程id
     pthread_t __th,
     // 参数二:
     void **__thread_return));
第一个参数为被等待的线程标识符,
第二个参数为一个用户定义的指针,
    它可以用来存储被等待线程的返回值.
这个函数是一个线程阻塞的函数,
调用它的函数将一直等待到被等待的线程结束为止.
当函数返回时,被等待线程的资源被收回.

extern void pthread_exit __P ((void *__retval)) 
        __attribute__ ((__noreturn__));
唯一的参数是函数的返回代码,
只要pthread_join中的第二个参数thread_return不是NULL,
这个值将被传递给thread_return.
最后要说明的是,一个线程不能被多个线程等待,
否则第一个接收到信号的线程成功返回,
其余调用pthread_join的线程则返回错误代码ESRCH.

总结:
pthread_exit等各个线程退出后,进程才结束;
否则main进程强制结束了,线程可能还没反应过来,因此线程不会执行.
pthread_join后面的代码只有待线程里的代码全部执行完了才会执行.
pthread_exit后面的代码不会被执行.
pthread_join与pthread_exit只需要用一个,
一起使用也没有关系,只是pthread_exit后面不会被执行.


其他说明:
创建线程成功后,新创建的线程则运行参数三和参数四确定的函数,
原来的线程则继续运行下一行代码.

属性结构(/usr/include/pthread.h)
pthread_attr_t
属性值不能直接设置,须使用相关函数进行操作,
初始化的函数为
pthread_attr_init
这个函数必须在pthread_create函数之前调用.
属性对象主要包括是否绑定、是否分离、堆栈地址、堆栈大小、优先级.
默认的属性为非绑定、非分离、缺省1M的堆栈、
与父进程同样级别的优先级.

关于线程的绑定,牵涉到另外一个概念：
轻进程(LWP：Light Weight Process).
轻进程可以理解为内核线程,它位于用户层和系统层之间.
系统对线程资源的分配、对线程的控制是通过轻进程来实现的,
一个轻进程可以控制一个或多个线程.
默认状况下,启动多少轻进程、哪些轻进程来控制哪些线程是由
系统来控制的,这种状况即称为非绑定的.
绑定状况下,则顾名思义,即某个线程固定的"绑"在一个轻进程之上.
被绑定的线程具有较高的响应速度,这是因为CPU时间片的调度是
面向轻进程的,
绑定的线程可以保证在需要的时候它总有一个轻进程可用.
通过设置被绑定的轻进程的优先级和调度级可以使得
绑定的线程满足诸如实时反应之类的要求.
设置线程绑定状态的函数为
pthread_attr_setscope,
它有两个参数,
第一个是指向属性结构的指针,
第二个是绑定类型,它有两个取值：
PTHREAD_SCOPE_SYSTEM(绑定的)
PTHREAD_SCOPE_PROCESS(非绑定的)

设置线程是否分离
线程的分离状态决定一个线程以什么样的方式来终止自己.
线程的默认属性即为非分离状态,这种情况下,
原有的线程等待创建的线程结束.
只有当pthread_join()函数返回时,
创建的线程才算终止,才能释放自己占用的系统资源.
而分离线程不是这样子的,它没有被其他的线程所等待,
自己运行结束了,线程也就终止了,马上释放系统资源.
程序员应该根据自己的需要,选择适当的分离状态.
设置线程分离状态的函数为
pthread_attr_setdetachstate(
     pthread_attr_t *attr,
     int detachstate);
第二个参数可选为
PTHREAD_CREATE_DETACHED(分离线程)
PTHREAD_CREATE_JOINABLE(非分离线程)
这里要注意的一点是,如果设置一个线程为分离线程,
而这个线程运行又非常快,
它很可能在pthread_create函数返回之前就终止了,
它终止以后就可能将线程号和系统资源移交给其他的线程使用,
这样调用pthread_create的线程就得到了错误的线程号.
要避免这种情况可以采取一定的同步措施,最简单的方法之一是
可以在被创建的线程里调用pthread_cond_timewait函数,
让这个线程等待一会儿,留出足够的时间让函数pthread_create返回.
设置一段等待时间,是在多线程编程里常用的方法.
但是注意不要使用诸如wait()之类的函数,
它们是使整个进程睡眠,并不能解决线程同步的问题.
另外一个可能常用的属性是线程的优先级,
它存放在结构sched_param中.
用函数pthread_attr_getschedparam
和函数pthread_attr_setschedparam进行存放,
一般说来,我们总是先取优先级,对取得的值修改后再存放回去.

互斥锁
创建pthread_mutex_init;
销毁pthread_mutex_destroy;
加锁pthread_mutex_lock;
解锁pthread_mutex_unlock.

条件锁
创建pthread_cond_init;
销毁pthread_cond_destroy;
触发pthread_cond_signal;
广播pthread_cond_broadcast S;
等待pthread_cond_wait

void *say_hello_thread(void *args) {
    sleep(3);
    printf("Hello Runoob pid: %u\n", pthread_self());
}

void test_pthread(void) {
    pthread_t p_tids[PTHREADS_NUM];
    // 定义一个属性
    pthread_attr_t attr;
    // 初始化属性值,均设为默认值
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    for (int i = 0; i < PTHREADS_NUM; ++i) {
        printf("&p_tids\[%d\] = %p\n", i, &p_tids[i]);
        int ret = pthread_create(&p_tids[i], &attr, say_hello_thread, NULL);
        if (ret != 0) {
            printf("pthread_create error: error_code = %d\n", ret);
        }
    }

    printf("pthread_join\n");

    for (int i = 0; i < PTHREADS_NUM; ++i) {
        //say_hello_thread函数里面的代码执行完了pthread_join才会返回结果
        int joinResult = pthread_join(p_tids[i], NULL);
        printf("joinResult = %d, %p\n", joinResult, &p_tids[i]);
    }

    //使用pthread_exit的话其后的代码就不会再执行了
    //pthread_exit(NULL);

    printf("Game Over\n");
}


///////////////////////////Boost start///////////////////////////

Boost库中的lexical_cast为数值之间的转换提供了一个更好的方案.
如果转换发生了异常,lexical_cast会抛出一个bad_lexical_cast异常,
因此程序中需要对其进行try-catch.
boost/lexical_cast.hpp中定义了此函数,lexical_cast使用统一接口
形式实现任意类型之间的转换,增强了易用性.
try {
    // 声明,省略boost名字空间前缀
    using boost::lexical_cast;
    // int a = boost::lexical_cast<int>("123");
    // 转换前2位
    int a = boost::lexical_cast<int>("123abc", 2);
    printf("%d\n", a);
    float b = boost::lexical_cast<float>("1.23456");
    printf("%f\n", b);
    string c = boost::lexical_cast<string>("1.23456");
    printf("%s\n", c.c_str());
} catch (const boost::bad_lexical_cast &e) {
    cout << e.what() << endl;
} catch (const std::exception &e) {
    cout << e.what() << endl;
}

///////////////////////////Boost end///////////////////////////

///////////////////////////

// a是数组名,代表内存的首地址
char *a = "hello";
char **b = &a;
printf("%s\n", b[0]);
printf("%p\n", a);
printf("%p\n", *b);
hello
0x41b623
0x41b623

char *p[5]
5个字符串,每一个p[i]是一个字符串.
char **p
每一个p[i]也是一个字符串

邮件协议SMTP简介
打开回显(windows)
set localecho
open smtp.qq.com 25
告诉服务器,我要登陆
auth login

bjam stage --toolset=msvc-9.0 --without-python --stagedir=""

智能指针scoped_ptr
auto_ptr<Test> p1(new Test(30));
cout<<p1->age<<endl;
// 1.调用拷贝构造函数,此时p1失去所有权
auto_ptr<Test> p2(p1);
// 2.调用赋值函数,此时p1失去所有权
auto_ptr<Test> p2;
p2 = p1;

// p1失去所有权后,再用p1调用成员函数就会报错
// 可以使用p1.get()是否为NULL判断p1这个指针是否还有效
cout<<p1.get()<<endl;
cout<<p2->age<<endl;

#include <boost/scoped_ptr.hpp>




































Android.mk:
可执行文件,动态库,静态库
Jar包,APK

LOCAL_PATH:Android.mk文件所在的目录
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test
LOCAL_SRC_FILES := test.c

使用系统提供的函数添加源文件到Android.mk中
LOCAL_C_ALL_FILES := $(call all-c-files-under)
LOCAL_SRC_FILES := $(LOCAL_C_ALL_FILES)

LOCAL_MODULE_PATH := $(LOCAL_PATH)
include $(BUILD_EXECUTABLE)

all-c-files-under函数
build/core/definitions.mk

cat external/test/test.c
mmm external/test/
build/core/definitions.mk环境变量的配置文件
build/core/config.mk
build/core/main.mk
build/core/clear_vars.mk




























































