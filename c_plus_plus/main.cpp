//#include <QCoreApplication>
#include <stdio.h>
#include <iostream>

// #include <jni.h>
// #include <unistd.h>
// #include <android/log.h>
// #include <sys/inotify.h>
// #incldue <stdlib.h>

#include "wd_string.h"
#include "wd_stdio.h"
#include "wd_opencv.h"
#include "socket.h"
#include "wd_grammar.h"

using namespace std;

void point(void);
void test(void);

int main(int argc, char* argv[])
{
    // QCoreApplication a(argc, argv);
    // return a.exec();
    printf("\n");
    printf("argc = %d\n", argc);
    // printf("Hello main method.\n");
    printf("The run result:\n");
    printf("---------------------------------------------------\n");
    test_grammar();
    printf("---------------------------------------------------\n");
    printf("\n");
    return 0;
}

void point(void)
{
    int i = 1;
    int* p = 0;
    p = &i;
    cout << "i: 		" << i << endl;
    cout << "&i: 		" << &i << endl;
    /***
    如果一个变量存放的是内存地址,那么在这个变量前面加"*"后的操作
    就是读取这块内存地址处的值.
    */
    cout << "*(&i): 		" << *(&i) << endl;
    cout << "p: 		" << p << endl;
    cout << "*(p): 		" << *(p) << endl;// *p = i;
    cout << "&p: 		" << &p << endl;
    int** addr = &p;
    /***
    addr保存的是p的地址,那么*(addr)的操作就是读取p的地址处的值,
    而p的地址处的值是i的地址,那么*(addr)就是读取到了i的地址,
    所以*(*addr)的操作就是读取到了i的地址处的值.
    */
    cout << "addr: 		" << addr << endl;
    cout << "*(&p): 		" << *(&p) << endl;
    cout << "*(addr): 	" << *(addr) << endl;// *addr = p;
    cout << "*(*addr): 	" << *(*addr) << endl;
    /***
    总结:
    如果能得到一块内存,那么用"*"操作后就能读取到这块内存处的值.
    二级指针就是用来保存一个指针变量的指针.
    */
    cout << endl;
    
    // 下面是引用部分
    int a;
    int& ra = a;
    cout << "a = 		" << a << endl;
    cout << "ra = 		" << ra << endl;
    a = 100;
    cout << "a = 		" << a << endl;
    cout << "ra = 		" << ra << endl;
    /***
    引用变量的内存地址跟它相关联的普通变量的内存地址是一样的.
    */
    cout << "&a = 		" << &a << endl;
    cout << "&ra = 		" << &ra << endl;
    int* pa = 0;
    pa = &ra;
    cout << "pa = 		" << pa << endl;
    *pa = 101;
    cout << "a = 		" << a << endl;
    cout << "ra = 		" << ra << endl;
    cout << "*pa = 		" << *pa << endl;
    cout << endl;
    int b;
    int& rb = b;
    b = 999;
    cout << "&b = 		" << &b << endl;
    cout << "&rb = 		" << &rb << endl;
    int c = 888;
    rb = c;
    cout << "b = 		" << b << endl;
    cout << "c = 		" << c << endl;
    cout << "rb = 		" << rb << endl;
    cout << "&b = 		" << &b << endl;
    cout << "&c = 		" << &c << endl;
    cout << "&rb = 		" << &rb << endl;
    rb = 1;
    cout << "b = 		" << b << endl;
    cout << "c = 		" << c << endl;
    cout << "rb = 		" << rb << endl;
    cout << "&b = 		" << &b << endl;
    cout << "&c = 		" << &c << endl;
    cout << "&rb = 		" << &rb << endl;
    c = 2;
    cout << "b = 		" << b << endl;
    cout << "c = 		" << c << endl;
    cout << "rb = 		" << rb << endl;
    cout << "&b = 		" << &b << endl;
    cout << "&c = 		" << &c << endl;
    cout << "&rb = 		" << &rb << endl;
    /***
    总结:
    1.决定一个引用变量的内存地址是它第一次跟某个普通变量建立关系时
    的那个普通变量的内存地址,以后这个内存地址就不会再改变.
    2.等到引用变量第一次跟普通变量建立关系后,由于两者的内存地址相同,
    那么以后不管谁的值改变了,另一方也跟着改变.
    3.第一次创建引用变量时,必须对其进行初始化,不能先创建引用变量,
    然后在下一行进行赋值.
    */
}

void test(void)
{
    const int a = 100;
    int b = a;
    int c = 100;
    const int d = c;
    // const int* pa = &a;
    // int* pb = pa;
    // int* pa = &a;
    // const int* pb = pa;
    int e;
    cout << "e = " << e << endl;
}

/***
“C++中的四少”：
构造函数
拷贝构造函数
赋值函数
析构函数

引用与拷贝构造函数
5.4.1 引用
C++函数中参数的传递方式是传值。在函数域中为参数重新分配
内存，而把实参的数值传递到新分配的内存中。它的优点是
有效避免函数的副作用（意思就是不改变原来实参的值）。
如果要求改变实参的值（或者实参是一个复杂的对象），重新
分配内存会引起程序执行效率的降低，因此在C++中有一咱新的
导出型数据类型---引用（又称为别名）。
void swap(int &x, int &y){
    int tmp = x;
    x = y;
    y = tmp;
}
swap函数的形参被定义为引用类型后，就不再为x和y重新分配
内存，其使用的是实参的地址。
如果swap函数的形参被定义为具体数据类型，那么会为x和y
重新分配内存。
int a = 10;
int b = 20;
调用方式为：swap(a, b);
引用不是定义一个新的变量，而是给一个已经定义的变量重新起
一个别名，也就是C++系统不为引用类型的变量分配内存空间。
引用主要用于函数之间的数据传递。定义的格式为：
类型 &引用变量名 = 已定义过的变量名；
引用变量的地址是第一次被赋值时的那个变量地址，以后不会
再改变。

引用的使用：
1.
引用作为函数的形参（拷贝构造函数中就是使用了引用）
2.
引用作为函数的返回值
一般函数返回值时，要生成一个临时变量作为返回值的拷贝，
而用引用作为返回值时，不生成值的拷贝。
3.
返回值为引用的函数作为左值

注意：
1.
对数组只能引用数组元素，不能引用数组（数组名本身就是个
地址）。
2.
不能定义引用的引用（引用也是一个地址），所以当函数的参数
为引用时，引用不能作为实参。
3.
不能定义空引用。如int &a;// 这样是编译不通过的。
正确的用法为：
int a = 10; 
int &b = a;
b的地址就是a的地址，以后即使被其他变量重新赋值其地址也
不会再改变，还是原来的a的地址。
下面的例子就能说明这个结论
int a = 10;
int b = 20;
fun(a, b);
int &c = a;
cout<<"a = "<<a<<", b = "<<b<<", c = "<<c<<endl;
cout<<"a: "<<&a<<", b: "<<&b<<", c: "<<&c<<endl;
c = b;
cout<<"a = "<<a<<", b = "<<b<<", c = "<<c<<endl;
cout<<"a: "<<&a<<", b: "<<&b<<", c: "<<&c<<endl;
结果为：
a = 10, b = 20, c = 10
a: 0xff914050, b: 0xff91404c, c: 0xff914050
a = 20, b = 20, c = 20
a: 0xff914050, b: 0xff91404c, c: 0xff914050

下面是指针的引用：
int a = 10;
int *P = &a;
int *q = p; // 这样的话p和q指向不同的地址
int *&q = p;// 这样的话p和q指向相同的地址（指针的引用）
下面是数组的引用：
int ar[5] = {1,2,3,4,5};
int (&br)[5] = ar;// 这样的话ar和br指向相同的地址
int &br[5] = ar;  // 这样的话是编译不通过的
下面是常量的引用：
const int x = 100;
int &y = x;// 这样的话是编译不通过的
const int &y = x;// 这样的话是正确的

int n = 20;
const int &m = n;// 这样的话是正确的
m不能再被赋值，但是n可以被重新赋值，赋值后，m和n的值相同。


const引用：
引用在内部存放的是被引用对象的地址，不可寻址的值是不能
引用的；当引用作为形参时，实参也不能使用不可寻址的值，
更不能进行类型转换（如：实数转换为整数）。
但是const引用不同，它是只读的，为了绝对保证不会发生误改，
编译器实现const引用时，生成一个临时对象，引用实际上指向
该临时对象，但用户不能访问它。所以const引用可以实现
不可寻址的值（包括字面常量）的引用，例如：
const double dval = 1024;
const int &ri = dval;
这是正确的，编译器将其转换为：
const double dval = 1024;
int temp = dval;
const int &ri = temp;
因有临时对象，引用和类型转换都实现了。
因为dval和ri的类型不一样，因此两者的地址也是不一样的，
这是因为有临时变量产生的缘故，ri的地址就是临时变量的
地址。
当const引用作为形参时，实参也能使用不可寻址的值，并能
进行类型转换。
从上面可以得到这样一个结果，类型不一样时，必须借助一个
临时变量才能完成转换，只是这个临时变量我们不能使用而已。
const double dval = 12.34;
int &ri = dval;
这样的话是编译不通过的。
因此这说明了临时变量具备常量的性质，是一个常量。

5.4.2 拷贝构造函数
同一个类的对象在内存中有完全相同的结构，如果作为一个
整体进行复制或称拷贝是完全可行的。这个拷贝过程只需要
拷贝数据成员，而函数成员是共用的（只有一份拷贝）。
在建立对象时可用同一类的另一个对象来初始化该对象，这时
所用的构造函数称为拷贝构造函数。
有CGoods类，其拷贝构造函数为：
CGoods(const CGoods &cgd){
    strcpy(name, cgd.name);
    price = cgd.price;
    amount = cgd.amount;
    totalValue = cgd.totalValue;
}

class Test{
public:
    Test(int d = 0):data(d){
        cout<<"Create Test(int) Object: "<<this<<endl;
    }
    Test(const Test &t){
    
        cout<<"Copy Create Test(Test) Object: "<<this<<endl;
    }
    ~Test(){
        cout<<"Free Test Object: "<<this<<endl;
    }
private:
    int data;
};
使用：
Test t(10);
Test t2(t);完全等价于Test t2 = t;
Test t3 = t;// 初始化（因为t3这个变量还不存在），拷贝构造函数
Test t4;
t4 = t;// 赋值函数（因为t4这个变量已经存在）

拷贝构造函数在另外两方面的使用：
1.
当函数的形参是类的对象，调用函数时，进行形参与实参
结合时使用。这时要在内存新建立一个局部对象，并把实参
拷贝到新的对象中。
2.
当函数的返回值是类对象，函数执行完成返回调用者时使用。
理由也是要建立一个临时对象，再返回调用者。
因为局部对象在离开建立它的函数时就消亡了，不可能在返回
调用函数后继续生存，所以在处理这种情况时，编译系统会在
调用函数的表达式中创建一个无名临时对象，该临时对象的
生存周期只在函数调用处的表达式中。所谓return对象，
实际上是调用拷贝构造函数把该对象的值拷入临时对象。如果
返回的是变量，处理过程类似，只是不调用构造函数。







































*/