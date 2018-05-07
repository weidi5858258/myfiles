#include <stdio.h>
#include <iostream>

#include "MyHead.h"

using namespace std;

int main(int argc, char *argv[]) {
    printf("\n");
    printf("argc = %d\n", argc);
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    printf("The run result:\n");
    printf("------------------------------------------\n");
    Test t;
    cout << t.getData() << endl;
    Test t2(10);
    Test t3(t2);
    Test t4 = t2;

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}

/***
“C++中的四少”：
构造函数
拷贝构造函数
赋值函数
析构函数
用户自己不重写上面的函数时，系统会自动生成这样的函数

 写成员函数时，有返回值的先把成员函数写在最前面，然后
 才是“类名::成员函数”

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
如果把一个真实的类对象作为参数传递到拷贝构造函数中去，那么
会引起无穷递归。
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
Test t4;// 需要重写Test()函数，不然没法使用
t4 = t;// 赋值函数（因为t4这个变量已经存在）

拷贝构造函数在三种情况下的使用：
1.
用一个对象初始化另一个对象时，前提必须是初始化。
2.
当函数的形参是类的对象（就是一个普通的对象），调用函数时，
进行形参与实参结合时使用（实际就是用一个对象初始化另一个对象）。
这时要在内存新建立一个局部对象，并把实参拷贝到新的对象中。
3.
当函数的返回值是类对象，函数执行完成后返回调用者时使用。
理由也是要建立一个临时对象，再返回调用者。
因为局部对象在离开建立它的函数时就消亡了，不可能在返回
调用函数后继续生存，所以在处理这种情况时，编译系统会在
调用函数的表达式中创建一个无名临时对象，该临时对象的
生存周期只在函数调用处的表达式中。所谓return对象，
实际上是调用拷贝构造函数把该对象的值拷入临时对象。如果
返回的是变量，处理过程类似，只是不调用构造函数。
int getData(int x, int y){
     int sum = x + y;
     return sum;
 }
 在getData函数中返回的值不是sum地址处的值，情况是这样的，
 在返回时，会生成一个临时变量，然后sum的值赋给这个临时变量，
 因为sum的作用域只能在getData函数中，出了函数就消亡了；当getData
 函数的结果赋给调用者的某个值后，这个生成的临时变量也就消亡了。
 这个临时变量是我们不可见的。

 5.4.3 赋值函数（赋值语句）
 Test t2;先实例化一个对象
 t2 = t;然后这样才是赋值，才会调用到下面的函数（t2必须事先已经定义过了）
实际上是这样的调用形式：t2.operator=(t)，
然后又会转化为：operator=(&t2, t);

 Test &Test::operator=(Test * const this, const Test &t)
 Test &Test::operator=(const Test &t) {
    std::cout << "Call operator=(Test &): " << this << std::endl;
    if (this != &t) {
        this->data = t.data;
    }
    return *this;
    this代表地址，那么*this就是地址处的值，这个值就是当前对象。
}
上面operator函数有三个重点：
 1.
 形参为：const Test &t
 以引用方式接收对象，就不会再发生拷贝构造这样的步骤，也就是不会再
 产生无名临时对象。
 2.
 if (this != &t)
 3.
 返回值类型为：Test &
 返回值类型设为Test&的目的是为了能够“连等赋值”。
 如：
 Test t(10);
 Test t2, t3;
 t3 = t2 = t;// t3.operator=(t2.operator=(t))

用“引用”类型作为返回值的类型的原则：
如果return的对象（如return *this）不受当前
函数的影响（如operator函数），
那么返回值可以使用“引用”而不会出现问题；
如果return的对象的作用域只在这个函数中时，
那么使用“引用”时就会出现问题而得不到正确的结果，
此时的返回值类型应该是普通对象，因为这样才能在函数
内部产生一个无名临时对象，然后才能把这个临时对象返回。
使用“引用”的好处是效率和空间的提升。

5.4.4 函数的调用优化
1.
return Test(10);
// 创建无名临时对象的语法
2.
Test temp(10);
return temp;
// 这样会创建一个无名临时对象，然后出了函数就把temp给free掉，
// 也就是说这样的操作创建了两个对象
3.
// 返回值必须是Test，而不能是Test &（不然得到不正确结果）。
Test fun(Test &t){
    int value = t.getData();
    // 创建无名临时对象的语法
    return Test(value);
}
使用：
Test t(100);
1.
Test t2 = fun(t);
// 最优状态，此时会把fun函数中创建的无名临时对象直接赋给t2对象，
// 而不会再另外产生一个t2对象。也就是说这样的初始化操作
// 不会再调用拷贝构造函数（这是编译器优化的）。
// 像Test t2 = t;这样是会调用拷贝构造函数的。
2.
Test t2;
t2 = fun(t);

上面的例子的前提是自己重写了“operator=”函数。

有下面这样一种现象（前提是自己没有重写“operator=”函数）：
返回值是Test &，而不是Test（如果是Test也是可以的）。
Test& fun(Test &t){
    int value = t.getData();
    // 创建无名临时对象的语法
    return Test(value);
}
然后这样使用：
Test t(100);
Test t2 = fun(t);// 这样就能得到正确的结果

5.4.5 深拷贝与线拷贝
主要内容在于“四少”中什么时候哪些函数需要重写，哪些不需要重写。
int a = 100;
我们能够这样操作“&a”，对a这个变量取地址，这是因为编译器已经帮
我们实现了“&”取地址这样的操作，但是我们没法重写这个操作，因为
这是普通类型的变量；如果是一个对象，那么我们可以重写“&”这样的
操作，这就是运算符重载。
Test t;
Test *pt = &t;// Test *pt = t.operator&();
因此需要重写下面函数：
第5个函数
Test* operator&(){
    return this;
}
如果不重写某个类的上面的函数，那么编译器也会自动生成这样的函数，
只不过我们不能额外的干些其他的事了，比如执行到这个函数时打印一些
日志。
const Test t;
const Test *pt = &t;// 去年const编译是不通过的
此时const封锁的是*pt，因此不能再用*pt去指向其他的对象。
还有加了const后，是不会调用到上面重写的函数，需要重写下面的函数：
第6个函数
const Test* operator&() const{
    return this;
}
到现在为止，如果我们自己不重写任何的函数，那么编译器也会自动生成
有关的函数能够让代码正常运行。如果要重写，那么我们有6个函数可以
重写，就是“四少”，普通对象的取地址函数和const对象的取地址函数。
一个人不重写这6个函数，但是进行相关的操作时是能够正常执行的。
如果要操作其他的功能，那么必须要重写相关的函数后才能执行。
如输出对象的运算符重载。
深拷贝：不但拷贝指针的指向，还创建出一个空间用于存放内容
浅拷贝：只拷贝指针的指向
char* data;
data = (char*)malloc(sizeof(char) + (strlen(str) + 1));
free(data);
申请空间之前，先判断原来的变量有没有指向，如果有指向，那么先要
释放掉，然后才是重新申请空间。
重写赋值函数时一般有四个步骤：
1.
判断是不是给自己赋值，不是的情况下才操作if(this != &T){...}
2.
释放原有空间
3.
申请新的空间，然后进行成员变量的赋值(可能有异常，如申请不成功)
4.
return *this;

5.4.6 运算符重载
运算符的重载实际是一种特殊的函数重载，必须定义一个函数，并告诉C++
编译器，当遇到该重载的运算符时调用此函数。这个函数叫做运算符重载
函数，通常为类的成员函数。
定义运算符重载函数的一般格式：
返回值类型 类名::operator重载的运算符（参数表）{...}
operator是关键字，它与重载的运算符一起构成函数名。
因函数名的特殊性，C++编译器可以将这类函数识别出来。
细解运算符重载：
复数类+的重载：
Complex Complex::operator+(const Complex &c){
    return Complex(real+c.real, imag+c.imag);
}
这里采用Complex对象的引用而不是对象本身，调用时不再重新分配内存
建立一个复制的对象，函数效率会更高。而在引用形式参数类型说明前
加const关键字，表示被引用的实参是不可改变的，防止程序员不小心在
函数体中重新赋值了被引用的实参，如果这样做了那么C++编译器会认为
出错。
当用类的成员函数实现运算符的重载时，运算符重载函数的参数（当为
双目运算符时）为一个或（当为单目运算符时）没有。运算符的左操作数
一定是对象，因为重载的运算符是该对象的成员函数，而右操作数是该
函数的参数。
单目运算符“++”和“--”存在前置与后置的问题。
前置“++”格式为：
返回类型 类名::operator++(){...}
后置“++”格式为：
返回类型 类名::operator++(int){...}
后置“++”中的参数int公用途区分，并无实际意义，可以给一个变量名，
也可以不给变量名。
Int& operator++(){
    m_i++;
    return *this;
}
Int operator++(int){
    Int temp(m_i);
    m_i++;
    return temp;
}
C++不允许重载的运算符有：
1.
?:
2.
. .*
3.
::
4.
sizeof

5.4.7 友元
一个常规的成员函数声明描述了三件在逻辑上相互不同的事情：
1.该函数能访问类声明的私有部分
2.该函数位于类的作用域中
3.该函数必须经由一个对象去激活（有一个this指针）
通过将函数声明为static，可以让它只有前两种性质
通过将一个函数声明为友元可以使它只具有第一咱性质

成员函数的形参中第一个参数才是this指针，如
void fun(Test * const this),只是这个this指针不需要程序员去关心。
然后调用函数是这样的：
Test t(100);
t.fun();// 转化为：fun(&t);
class Test{
// 友元函数在类中声明
friend void show(Test &);
};
// 友元函数在实现
void show(Test &t){...}

友元函数注意点：
1.
友元函数不是类的成员函数，在函数体中访问对象的成员，必须用对象名
加运算符“.”加对象成员名。但是友元函数可以访问类中的所有成员。
一般函数只能访问类中的公有成员。
2.
友元函数不受类中的访问权限关键字限制，可以把它放在类的公有、私有、
保护部分，但结果都一样。
3.
某类的友元函数的作用域并非该类作用域。如果该友元函数是另一类的
成员函数，则其作用域为另一类的作用域，否则与一般函数相同。
友元类：
整个类可以是另一个类的友元。友元类的每个成员函数都是另一个类的
友元函数，都可以访问另一个类中的保护或私有数据成员。
定义方法如下：
class A{
// 声明B为A的友元类
friend class B;
}
下面几个都是声明为Complex类的友元函数：
Complex operator+(int i, const Complex &c);
Complex operator+(const Complex &c, int i);
Complex comp(100,200);
Complex comp2 = 10 + comp;
Complex comp3 = comp + 10;
// 这样操作之后就可以直接输出一个对象了cout<<comp<<endl;
ostream& operator<<(ostream &out, const Complex &c);
istream& operator>>(istream &in, Complex &c);

也可以定义为Complex类的成员函数进行输出
ostream& operator<<(ostream &out){
    cout<<"("<<m_real<<", "<<m_imag<<")"<<endl;
    return out;
}
不过需要这样调用：comp<<cout<<endl;


5.4.8 static方法与const方法

模板是标准C++实现代码复用的有力工具，特别是有关数据结构的算法。
5.4.9 函数模板
1.
定义模板：
template<typename Type>
Type max(Type a, Type b){
    cout<<typeid(Type).name()<<endl;
    return a > b ? a : b;
}
使用：
max(1, (int)2.3);
max((double)1, 2.3);
max<int>(1, 2.3);
2.
定义模板：
template<typename Type1, typename Type2>
Type1 max(Type1 a, Type2 b){
     return a > b ? a : b;
}

5.4.10 类模板
例子：
// 声明类模板
template<typename Type>
class List;

template<typename Type>
class ListNode {
    friend class List<Type>;

public:
    ListNode() :
            data(Type()), next(NULL) {}

    ListNode(Type d, ListNode<Type> *n = NULL) :
            data(d), next(n) {}

    ~ListNode() {}

private:
    Type data;
    ListNode<Type> *next;
};

template<typename Type>
class List {
public:
    List();

    bool push_back(Type x);

private:
    ListNode<Type> *first;
    ListNode<Type> *last;
    size_t size;
};

template<typename Type>
List<Type>::List() {
    first = last = (ListNode<Type> *) malloc(sizeof(ListNode<Type>));
    last->next = NULL;
    size = 0;
}

template<typename Type>
bool List<Type>::push_back(Type x) {
    ListNode<Type> *s = (ListNode<Type> *) malloc(sizeof(ListNode<Type>));
    if (s == NULL) {
        return false;
    }
    s->data = x;
    s->next = NULL;

    last->data = s;
    last = s;
    size++;
    return true;
}

void main() {
    List<int> intList;
    for (int i = 1; i <= 10; ++i) {
        intList.push_back(i);
    }
}

动态内存开辟new_delete
堆内存的分配与释放
堆对象与构造函数
深拷贝与浅拷贝
int *p = (int*)malloc(sizeof(int) * SIZE);
if(p == NULL) exit(1);
free(p);
int *q = new int[10];
delete []q;

#include<list>
#include<stack>
容器分为三大类：
顺序容器
vector（参量）                 从后面快速插入与删除，直接访问任何元素
deque（双端队列）              从前面或后面快速插入与删除，直接访问任何元素
list（列表）                   从任何地方快速插入与删除，双链表
关联容器
set（集合）                    快速查找，不允许重复值
multiset（多重集合）           快速查找，允许重复值
map（映射）                    一对一映射，基于关键字快速查找，不允许重复值
multimap（多重映射）           一对多映射，基于关键字快速查找，允许重复值
容器适配器
stack（栈）                    后进先出
queue（队列）                  先进先出
priority queue（优先级队列）   最高优先级元素问题第一个出列



字符串
字符串是以空字符结尾的字符数组.
char型字符串
char占用一个字节的空间.
char man[4] = {'J','a','c','k'};
然而该char型数组man却不是一个字符串,
因为它没有字符串的结束标志---"\0".
只有为其添加"\0"这个结束标志后,这个char型数组man才转化为字符串.如
char man[4] = {'J','a','c','k', '\0'};
"\0"













*/