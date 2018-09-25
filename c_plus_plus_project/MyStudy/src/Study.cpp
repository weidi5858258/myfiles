//
// Created by root on 18-7-12.
//
#ifndef MYSTUDY_STUDY
#define MYSTUDY_STUDY

#ifdef WIN32
#include "MyHead.h"
#include "Test.h"
#endif
#ifdef linux
#include "../include/MyHead.h"
#include "../include/Test.h"
#endif

#ifndef MYSTUDY_B0
#define MYSTUDY_B0

#ifdef UBUNTU_SYSTEM
#include "../src/B0.cpp"
#else
#include "B0.cpp"
#endif

#endif //MYSTUDY_B0

#ifndef MYSTUDY_POINTER
#define MYSTUDY_POINTER

#ifdef UBUNTU_SYSTEM
#include "../src/Pointer.cpp"
#else
#include "Pointer.cpp"
#endif

#endif //MYSTUDY_POINTER

// 先转化成(int *),然后取该地址处的值
int cmp(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

/***
 要测试的代码放在main函数中,测试后放到这个函数中
 */
void test() {
    Test t;
    cout << t.getData() << endl;
    Test t2(10);
    Test t3(t2);
    Test t4 = t2;

    C co;
    // error
    // c.B0::b0 = 100;
    co.B1::b0 = 100;
    co.B2::b0 = 200;
    cout << co.B1::m << endl;
    cout << co.B2::m << endl;
    cout << co.c << endl;
    co.print();
    B1 *pb = &co;
    pb->print();

    // 指针
    Pointer pt;
    pt.point();

    char name[] = "ueventc";
    cout << strcmp(basename(name), "ueventd") << endl;
    size_t key_len = strlen("PATH");
    cout << "key_len = " << key_len << endl;// 4
    cout << "key_len = " << sizeof("PATH") << endl;// 5

    char ch = '!';
    printf("%d = %c.\n", ch, ch);
    int ich = 127;
    printf("%d = %c.\n", ich, ich);

    int tmp[5] = {1, 2, 3, 4, 5};
    int *p = tmp;
    cout << sizeof(tmp) << endl;// tmp数组占用空间大小
    cout << sizeof(tmp) / sizeof(int) << endl;// tmp数组元素个数

    cout << tmp << endl;
    cout << &tmp[0] << endl;
    cout << p << endl;

    cout << *tmp << endl;
    // cout << *(tmp++) << endl;// error
    cout << *(&tmp[0]) << endl;
    int j;
    for (j = 0; j < 5; ++j) {
        cout << *(p++) << endl;
    }

    cout << sizeof(char) << endl;// 1
    cout << sizeof(wchar_t) << endl;// 4
    cout << sizeof(int) << endl;// 4
    cout << sizeof(short) << endl;// 2
    cout << sizeof(float) << endl;// 4
    cout << sizeof(double) << endl;// 8
    cout << sizeof(long) << endl;// 8
    cout << sizeof(long long) << endl;// 8
    cout << sizeof(long long int) << endl;// 8
    cout << sizeof(long double) << endl;// 16

    cout << endl;

    cout << sizeof(signed char) << endl;// 1
    cout << sizeof(unsigned char) << endl;// 1
    cout << sizeof(unsigned wchar_t) << endl;// 4
    cout << sizeof(unsigned int) << endl;// 4
    cout << sizeof(unsigned short) << endl;// 2
    cout << sizeof(unsigned long) << endl;// 8
    cout << sizeof(unsigned long long) << endl;// 8
    cout << sizeof(unsigned long long int) << endl;// 8

    cout << endl;

    cout << CHAR_MIN << endl;
    cout << CHAR_MAX << endl;
    cout << INT_MIN << endl;
    cout << INT_MAX << endl;
    cout << LONG_MIN << endl;
    cout << LONG_MAX << endl;

    // char与signed char取值范围都是 -128 到 127(有符号位)
    // unsigned char取值范围是 0 到 255
    // 如果定义的变量的值在此范围内,那么输出的值就是当前值
    // 正常边界,正常输出
    char a = -128;
    signed char b = 127;
    unsigned char c = 0;
    unsigned char d = 255;
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);// -128 127 0
    printf("a=%c, b=%c, c=%c, d=%d\n", a, b, c, d);//
    // 超出边界,反向输出
    a = -129;
    b = 128;
    c = -1;
    d = 256;
    printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);// 127 -128 255 0

    cout << endl;

    FILE *fp;
    size_t size = 0;
    char *ar;
    // 二进制方式打开文件
    fp = fopen("src/B0.cpp", "rb");
    if (NULL == fp) {
        printf("Error:Open input.c file fail!\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    // 求得文件的大小
    size = ftell(fp);
    rewind(fp);
    // 申请一块能装下整个文件的空间
    ar = (char *) malloc(sizeof(char) * size);
    // 读文件
    // fread(ar, 1, all_pkts_size, fp);//每次读一个，共读size次
    fread(ar, size, 1, fp);
    // 内容的首地址
    printf("%p\n", ar);
    // 输出文件内容
    printf("%s\n", ar);
    fclose(fp);
    free(ar);
    ar = NULL;

    cout << endl;

    const char *path = "/data";
    printf("path = %s\n", path);

    const unsigned char image[] = "abcd\0efg";
    //  const unsigned char image[] = "";
    const unsigned char *pImage = image;
    size = sizeof(pImage);// 包括结尾隐藏的\0
    cout << size << endl;// 8 固定长度,用于存放指针地址的
    size = sizeof(image);
    cout << size << endl;// 9
    for (int i = 0; i < size; ++i) {
        // 这个例子说明了可以用pImage[i]这种方式一个字符一个字符的输出
        printf("%c\n", pImage[i]);
        if (pImage[i] == 0) {
            printf("遇到\\0换行\n");
        }
    }

    double tmpArray[] = {1, 2, 3, 4, 5};
    cout << sizeof(tmpArray) << endl;
    for (int i = 0; i < 5; i++) {
        printf("%f\t%f\t%f\t%p\t%p\n",
               tmpArray[i], *(&tmpArray[i]), *(tmpArray + i),
               &tmpArray[i], (tmpArray + i));
    }

    //不排序
    std::unordered_map<std::string, std::string> u = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}
    };
    for (const auto &n : u) {
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    }
    std::cout << u["RED"] << " " << u["GREEN"] << " " << u["GREEN"] << endl;

    //排序
    std::map<std::string, std::string> u3 = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}
    };
    for (const auto &n : u3) {
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    }

    // 下面的代码在这个工具中无法编译通过
    /*// 在栈上开辟一维数组
    int *pOne = (int[]){0};
    // 在栈上开辟二维数组
    int ab[3][4];
    int (*pTwo)[4] = (int[][4]){0};
    // 在栈上开辟三维数组
    int (*pThree)[3][4] = (int[][3][4]){0};*/

    // 下面代码使用SDL2简单显示一个窗口
    /*SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(
                "SDL Tutorial",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                640,
                480,
                SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        SDL_Delay(5000);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();*/

    /*SDL_Window *window = NULL;
    SDL_Renderer *renderer = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
            "hello",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(10000);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();*/

    /*glutInit(&argc,argv);
    //显示模式初始化
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    //定义窗口大小
    glutInitWindowSize(300,300);
    //定义窗口位置
    glutInitWindowPosition(100,100);
    //创建窗口
    glutCreateWindow("OpenGL Version");
    const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
    const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
    printf("OpenGL实现厂商的名字：%s\n", name);
    printf("渲染器标识符：%s\n", biaoshifu);
    printf("OOpenGL实现的版本号：%s\n",OpenGLVersion );
    printf("OGLU工具库版本：%s\n", gluVersion);*/

}

/***
 用typedef重定义类型名
 typedef 原类型名 新类型名;
 则以后可以用这个"新类型名"来代替"原类型名"进行使用了.
 注意:用typedef类型重定义只是给原类型取一个别名,没有创造新的类型.
 typedef unsigned int size_t;// 用于内存字节计算
 typedef char NAME[20];
 NAME a1,a2,a3;
 等效于
 char a1[20],a2[20],a3[20];

 typedef与#define的区别
 用"#define"一般用于表示常量的比较多.
 定义方式不同:
 1.
 typedef是用后面的名称来表示前面的类型(数据类型),
 #define是用前面的名称来表示后面的内容(常量)
 2.
 用typedef能实现的,用#define有可能出错.如:
 #define INTP int*;
 INTP p,q;其等价于int* p,q;// 实际只有p是指向整型的指针变量,而q仅仅是整型变量.
 typedef int* INTP;
 INTP p,q;// p,q都是指向整型的指针变量.

 C++中函数的高级用法
 内联函数:
 内联函数的声明和定义必须放在一个文件里,
 inline关键字
 是建议C++编译器将该函数设定为内联函数,但没有决定权;
 当函数里面有循环语句,复杂的条件语句,递归等时,
 则C++编译器无视inline关键字,将该函数作为普通函数处理.
 函数重载: 满足函数重载的条件是 有相同的函数名,不同的形参列表;与函数的返回值无关.
 递归函数: 自己调用自己的函数,注意必须要有退出函数的条件判断.

 





























“C++中的四少”：
构造函数
拷贝构造函数
赋值语句
析构函数
用户自己不重写上面的函数时，系统会自动生成这样的函数

关于继承的一些结论:
1.
当父类与子类有相同的函数(包括返回值,函数名,参数列名都相同),
父类的指针指向子类对象的地址,然后用这个指向调用这个函数时,
如果父类的这个函数没有用virtual修饰,那么调用的是父类的函数;
否则调用的是子类的函数.
2.
父类中有多个函数重载,可是子类却只有一个函数覆写了父类的函数,
那么当子类对象调用父类的重载函数时,只有自己的类中有的函数才可以调用,
其他的父类的重载函数不能调用,在子类中已经被同名隐藏了.
3.
3-1.
B1,B2都是普通继承B0,然后C顺次继承B1,B2,则创建C对象时,依次为B0,B1,B0,B2,C
3-2.
B1普通继承B0,B2使用virtual继承B0,然后C顺次继承B1,B2,则创建C对象时,依次为B0,B0,B1,B2,C
3-3.
B1普通继承B0,B2使用virtual继承B0,然后C顺次继承B1,virtual B2,则创建C对象时,依次为B0,B2,B0,B1,C
总结：有virtual继承时创建对象有优先权

写成员函数时，有返回值的先把成员函数写在最前面，然后
才是“类名::成员函数”

引用与拷贝构造函数
5.4.1 引用
C++函数中参数的传递方式是传值。
在函数域中为参数重新分配
内存，而把实参的数值传递到新分配的内存中。它的优点是
有效避免函数的副作用（意思就是不改变原来实参的值）。
如果要求改变实参的值（或者实参是一个复杂的对象），重新
分配内存会引起程序执行效率的降低，因此在C++中有一个新的
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
对数组只能引用数组元素，不能引用数组（数组名本身就是个地址）。
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
因为有临时对象，所以引用和类型转换都实现了。
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
实际上是这样的调用形式：t2.operator=(t),
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

 如果函数的参数是普通对象,而没有使用"引用"或者"指针",
 那么实参传递给形参时会调用拷贝构造函数.

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

派生类与基类
在任何需要基类对象的地方都可以用公有派生类的对象来代替,
这条规则称为赋值兼容规则.它包括以下情况:
1.
派生类的对象可以赋值给基类的对象,这时是把派生类对象中
从对应基类中继承来的隐藏对象赋值给基类对象.反过来赋值就
不行,因为派生类的新成员无值可赋.
2.
可以将一个派生类的对象的地址赋给基类的指针变量,但只能通过
这个指针访问类中由基类继承来的隐藏对象,不能访问派生类中的
新成员.同样也不能反过来做.
3.
派生类对象可以初始化基类的引用.引用是别名,但这个别名只能包含
派生类对象中的由基类继承来的隐藏对象.

基类指针或引用访问子类中的方法:
把子类的地址赋给基类的指针或者把子类赋给基类的引用,
此时用这个基类的指针或者引用去访问基类与子类都有的方法时,
结果访问的是基类中的方法,不会是子类中的方法;如果想要访问
子类中的方法,那么需要把基类中的方法加上virtual.
基类对象访问子类中的方法:
把子类对象赋给基类对象后,用基类对象访问子类中的方法时,
访问的还是基类中的方法.(前提是基类与子类都有相同的方法)

达到多态的条件:
1.
有继承关系
2.
有相同的方法名,返回值类型,参数列表
3.
基类方法加上virtual
4.
子类对象赋给基类的指针或者基类的引用

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
字符串
char占用一个字节的空间
char man[4] = {'J', 'a', 'c', 'k'};
然而该char型数组man却不是一个字符串，因为它没有的结束
标志---“\0”.只有我们为其添加“\0”这个结束标志后，这个char
型数组man才能转化为字符串。如：
char man[4] = {'J', 'a', 'c', 'k', '\0'};
“\0”是一个空字符标志，它的ASCII码为0，C++有好多处理字符串的函数，
它们都以“\0”为结束标志，也就是以空字符为结束标志，如cin和cout，
它们也是以空字符为结束标志，它们在碰到空字符后将会停止输入或输出。
char man[12];
cin>>man;如果没有下面的代码，那么遇到空格就结束输入
cin.get(man, 12);遇到空格也会存入，并输出到屏幕上
cin.get的结束标志是“\n”,也就是换行，因此语句遇到空格不会结束，而是
把空格也看做一个字符，同时在输入结束后，也就是用户输入一个回车后，
cin.get自动为当前接受输入的数组添加字符串结束标志“\n”，因此它实际
保存的有效字符是它的第2个参数减1，即11个字符。
同样，输出也是这样，假如我们在一行字符串的中间存放了0，
那么0之后（包括0）的内容都不会输出。
数字0跟字符'\0'的效果是一样的。
空字符的ASCII码为0，编译器会根据ASCII码来寻找字符，ASCII码为0的字符
是NULL，也就是空字符，cout遇到空字符就会停止输出。
char man[12] = {"Hello world"};
用双引号包括起来的字符串隐含了字符串结束标志“\0”，因此不用手动去添加它。
也可以省略定义字符串的长度，而把这个任务交给编译器去完成。
如：char man[] = {"Hello world"};

strlen函数与sizeof函数的区别
strlen函数返回的是字符串结束标志“\0”之前的字符串长度，而不是数组长度；
而sizeof函数返回的是数组的长度，也就是该数组共有多少个元素。
char man[12] = {"Hello world"};
strlen(man): 11
sizeof(man): 12

字符串是以空字符结尾的字符数组.
char型字符串
char占用一个字节的空间.
char man[4] = {'J','a','c','k'};
然而该char型数组man却不是一个字符串,
因为它没有字符串的结束标志---"\0".
只有为其添加"\0"这个结束标志后,这个char型数组man才转化为字符串.如
char man[4] = {'J','a','c','k', '\0'};
"\0"

string型字符串的赋值
char型字符串无法直接赋值.如:
char ch1[] = "give me";
char ch2[] = "a cup";
ch1 = ch2;
这样是错误的,不能将一个数组名直接赋给另一个数组名.
可以用strcpy拷贝函数实现赋值的目的.
char ch1[] = "give me";
char ch2[] = "a cup";
strcpy(ch1, ch2);
必须保证ch1的空间足够大,以便能够容纳ch2的字符串.
strcpy会将ch2中的所有字符,包括结束标志"\0"一起复制到ch1中去.
第一种方式赋值
#include <string>
string str1 = "what's your name";
string str2 = "my name is Jack";
str1 = str2;
output: str1 = my name is Jack
第二种方式赋值
string str1 = " gh ";
string str2 = " abcdef ";
str1.assign(str2, 3, 1);
output: d

string型字符串的合并
对两个char型字符串进行合并要用到strcat函数,strcat函数
将第2个字符串合并到第1个字符串中,因此第1个字符串必须保证能容纳两个字符串的长度.
char ch1[] = "what's your name";
char ch2[] = "my name is Jack";
strcat(ch1, ch2);
运行后就会立即崩溃,这是因为ch1的空间不够造成的.
string str1 = "what's your name";
string str2 = "my name is Jack";
str1 = str1 + str2;

string型字符串的部分合并
char ch1[10] = "ab";
char ch2[] = "abcdef";
strncat(ch1, ch2, 3);
output: ch1 = ababc

string str1 = "ab";
string str2 = "abcdef";
str1.append(str2, 2, 3);
output: str1 = abcde

string型字符串的替换
char ch1[10] = "gh";
char ch2[] = "abcdef";
strncpy(ch1, ch2, 3);
output: ch1 = abc

string str1 = "gh";
string str2 = "abcdef";
str1.replace(0, 1, str2, 4, 2);
output: str1 = efh

basic_string &replace(size_type index, size_type num, const basic_string &str);
从本字符串的index开始,用str中的所有字符替换本字符串中的num个字符.
basic_string &replace(size_type index, size_type num, const char *str);
从本字符串的index开始,用str中的所有字符替换本字符串中的num个字符.
basic_string &replace(size_type index1, size_type num1, const basic_string &str, size_type index2, size_type num2);
用str中的num2个字符(从index2开始)替换本字符串中的字符,从index1开始,最多有num1个字符.
basic_string &replace(size_type index, size_type num1, const char *str, size_type num2);
用str中的num2个字符本字符串中的字符,从index开始,共num1个字符.
basic_string &replace(size_type index, size_type num1, size_type num2, char ch);
用num2个ch字符替换本字符串中的字符,从index开始,fugai num1个字符.
basic_string &replace(iterator start, iterator end, const basic_string &str);
用str中的字符替换本字符串中的字符,迭代器start和end指示范围.
basic_string &replace(iterator start, iterator end, const char *str);
用str中的字符替换本字符串中的字符,迭代器start和end指示范围.
basic_string &replace(iterator start, iterator end, const char *str, size_type num);
用str中的num个字符替换本字符串中的内容,迭代器start和end指示范围.
basic_string &replace(iterator start, iterator end, size_type num, char ch);
用num个ch字符替换本字符串中的内容,迭代器start和end指示范围.

string型字符串的复制
char ch1[15] = "abcdefghijklmn";
char ch2[] = "1234567890";
memmove(ch1, ch2, 10);
output: ch1 = 1234567890klmn

从string型字符中复制到char型字符串的copy函数
string str = "abcd";
char ch[] = "1234";
int n = str.copy(ch, 4, 0);
output: n = 4, ch = abcd

string型字符串的插入
str.insert(index1, str1, index2, num);
string str1 = "12789";
string str2 = "3456";
str1.insert(2, str2, 0, 4);
output: str1 = 123456789

string型字符串的删除
string s("give me");
// 第1个参数表示从哪个位置开始删除，第2个参数表示删除几个
s.erase(2, 2);
output: s = gi me
// 表示从哪个位置开始删除
s.erase(2);
output: s = gi
// 参数默认值为0，则从0开始往后的字符全部删除
s.erase();
output: s = 没有内容

string型字符串的查找
char ch1[15];
char *p, c = 'w';
strcpy(ch1, "hello world");
p = strchr(ch1, c);

string str1("hello world");
// “w”表示要搜索的字符，0表示从str1的第1个字符开始搜索
int f = str1.find("w", 0);
if(f != string::npos){
    cout<<f<<endl;
}else{
    cout<<"not find"<<endl;
}
npos是string类提供的一个常量，用来表示不存在的位置，
许多平台中npos的值都不一样。因此它的值由平台决定，
一般是-1，不管什么平台，npos的值都不能用作数组的下标。
find_first_not_of(): 查找第一个与value中的某值不相等的字符
find_first_of(): 查找第一个与value中的某值相等的字符
find_last_of(): 查找最后一个与value中的某值相等的字符
find_last_not_of(): 查找最后一个与value中的所有值都不相等的字符
rfind(): 查找最后一个与value相等的字符（逆向查找）

string型字符串的比较
string s1 = "155";
string s2 = "52";
char c[] = "34";
int i,j,k,l,m,n;
// 将s1与s2比较，返回0为相等，返回1为s1大于s2，返回-1为s1小于s2
i = s1.compare(s2);
// 将char型字符串c与s2比较
j = s2.compare(c);
// 取s1前两个字符与s2比较，参数1表示位置，参数2表示取的个数
k = s1.compare(0, 2, s2);
//
l = s1.compare(1, 1, s2, 0, 1);
//
m = s1.compare(1, 1, c, 0, 1);
//
n = s1.compare(1, 1, c, 1);

string型字符串是否为空
string str = "";
if(str.empty()){
    cout<<"str is empty"<<endl;
}

字符串的使用
swap()交换两个字符串的内容
交换两个char型字符串的函数swab()，只能交换字符串中的相信的
偶数字节和奇数字节的位置。
char ch1[15] = "ofru";
char ch2[15] = "";
swab(ch1, ch2, strlen(ch1));
output: ch1 = ofru ch2 = four

string str1 = "give me";
string str2 = "a cup";
str1.swap(str2);
output: str1 = a cup str2 = give me

将string型字符串转为char型字符串
string类的成员函数c_str()可以返回一个指向char型字符串的const指针,
该指针指向调用该函数的string型字符串对象.
string str = "hello world";
const char *ch;
ch = str.c_str();
output: ch = hello world

char型字符串与函数
char型字符串就是C风格的字符串,它由一串字符组成,结尾为字符串结束
标志"\0".字符串名是第1个字符的地址,因此我们将字符串名作为参数
传递到函数中时,其实就是将地址传递到函数中去.
int get(const char p[])
int get(const char *p){
    int count = 0;
    while(*p){
        count++;
        p++;
    }
    return count;
}
char ch[15] = "hello world";
char *p = "very well";
int a = get(ch);
int b = get(p);
output: a = 10 b = 9

字符串有3种表达方式,如下:
1. char c[12] = "study";// char型数组
2. "study"              // 未命名数组
3. char *p = "study";   // 指向char型数组的指针
由于这3种表达方式才是以指针的方式来保存字符串中第1个字符的
内存地址,因此都可作为参数传递到函数中去.

函数如何返回字符串
char *get(char *str){
    char *p = new char(strlen(str) + 1);
    strcpy(p, str);
    return p;
}
char c[10];
char *ch;
cin>>c;
ch = get(c);
cout<<ch<<endl;
delete []ch;
ch = get("Jack");
cout<<ch<<endl;
delete []ch;
char *ch2 = "Mike";
ch = get(ch2);
cout<<ch<<endl;
delete []ch;

结构体
结构体是C时代的产物,到了C++之后结构体有了巨大的变化,可以增加
函数,可以设置成员的公有,私有和保护属性,可以从别的类继承,也
可以被别的类继承,可以有虚函数.唯一与类不同之后是结构体定义中
默认情况下的成员是public,而类定义中默认情况下的成员是private.
struct A{
public:// 有没有是一样的
    int get(){return x;}
    void set(int i){x = i;}
private:
    int x;
};
使用:
A a;
set(9);
cout<<a.get()<<endl;

struct people{
  double weight;
  double tall;
  int age;
  char *name;
  char *native;
  bool sex;
};
prople Jack={
  180.5,
  179.3,
  34,
  "Jack",
  "济南",
  1
};
cout<<Jack.name<<endl;
cout<<Jack.native<<endl;
cout<<Jack.tall<<endl;
cout<<Jack.weight<<endl;
cout<<Jack.age<<endl;

结构体中使用string
struct people{
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
         string t_name, char *t_native, bool t_sex){
  weight = t_weight;
  tall = t_tall;
  age = t_age;
  name = t_name;
  native = t_native;
  sex = t_sex;
};
people Jack(180.5, 179.3, 34, "Jack", "济南", 1);
由于是调用构造函数对Jack中的数据成员进行初始化,因此Jack后面没有
加赋值符(=),并且用括号替代了双引号.

结构体的赋值
既然结构是一种数据类型,那么我们就可以像对变量那样进行赋值操作,
不过前提是两个结构必须都是同类型的.
struct people{
  double weight;
  double tall;
};
people Mike = {185.2, 178.1};
people Jack = {65.4, 165.2};
Mike = Jack;
output: 两者输出是一样的

结构体与函数
struct time{
  int hour;
  int minute;
};
const int perhour = 60;
time sum(time t1, time t2){
    time total;
    total.minute = (t1.minute + t2.minute) % perhour;
    total.hour = t1.hour + t2.hour + (t1.minute + t2.minute)/perhour;
    return total;
}
void show(time t){
    cout<<t.hour<<":"<<t.minute<<endl;
}
time one = {8, 15};
time two = {6, 55};
time day = sum(one, two);
show(day);
time day3 = {9, 35};

time &sum(time t1, time t2){
    time *total = new time;
    total.minute = (t1.minute + t2.minute) % perhour;
    total.hour = t1.hour + t2.hour + (t1.minute + t2.minute)/perhour;
    return *total;
}
time *day = &sum(one, two);
show(*day);
time day2 = {9, 35};
time *p1 = &sum(*day, day2);
show(*p1);
delete day;
delete p1;

结构体与string
void show(const string p){
    cout<<p<<endl;
}
string str = "hello world";
show(str);

void show(const string &p){
    cout<<p<<endl;
}
show(str);

string数组与函数
假如要传递多个字符串,那么可以声明一个string对象数组,然后
将数组传递函数中.
void show(const string str[], int n){
    for(int i=0;i<n;i++){
        cout<<i+1<<":"<<str[i]<<endl;
    }
}
const int length = 5;
string str[length];
for(int i=0;i<length;i++){
    cout<<i+1<<":";
    cin>>str[i];
}
show(str, length);

string str[5];
char str[][5];
string对象数组与二维char型数组差不多.不同的是,string对象数组有
自动调节数组大小的功能,而二维char型数组却没有,因此使用二维char
型数组时,必须将数组定义得足够大,以避免数组越界.

流的使用
把控制数据流动的cin和cout叫做输入输出流.凡是跟输入输出流
有关系的类叫做流类,凡是实现流类功能的成员函数全部合并在一个
库中,该库叫做流类库.
重载输出运算符<<
class A{
  public:
    A(int x, int y)  {rx = x; ry = y}
  public:
    int rx;
    int ry;
};
ostream &operator<<(ostream &s, const A &a){
    s<<a.rx;// A的成员变量必须定义为public
    s<<a.ry;
    return s;
}

友元的方式重载输出运算符
class A{
  public:
    A(int x, int y)  {rx = x; ry = y;}
    friend ostream &operator<<(ostream &s, const A &a){
        s<<a.rx;
        s<<a.ry;
        return s;
    }
  private:
    int rx;
    int ry;
};
A a(3, 4), b(5, 6);
cout<<a<<b<<endl;

重载输入运算符>>
class A{
  public:
    A(int i)  {x = i;}
    friend istream &operator>>(istream &in, A &a){
        in>>a.x;
        return in;
    }
    friend ostream &operator<<(ostream &o, const A &a){
        o<<a.x;
        return o;
    }
  private:
    int x;
};
A a(3), b(4);
cin>>a>>b;
cout<<a<<b;

异常和错误处理
try{
    wrong();
}

 字符数组的初始化:
 基本方法同一维数组和二维数组的初始化方法.
 char ch[11] = {'h','e','l','l','o', '', 'w','o','r','l','d'};
 char ch[] = {'h','e','l','l','o'};
 '0'  ---> 48(ASCII码)
 '\0' ---> 0(ASCII码)
 为字符数组部分元素赋初值,其余元素默认填充'\0'.
 if(name[i] == '\n'){break;}
 在C语言中字符串是用字符数组来保存的.
 字符数组定义好大小后,先清零.
 字符数组的定义:char ch[5] = {'h','e','l','l','o'};//编译与运行都ok
 字符串的定义:char str[5] = "hello";//编译ok,运行error
            char str[6] = "hello";//编译与运行都ok
            char str[] = "hello";//编译与运行都ok
 字符串定义后会在后面自动加上'\0',因此需要多出一个字符的空间
 对字符数组使用%s格式说明符输入和输出(char str[20]):
 输入:scanf("%s", str);
    输入时,直接给出数组名即可,不用对数组名取地址,因为数组名本身就是地址.
    输入时,遇到空格或换行自动结束输入,空格或换行之前的字符即为字符数组的内容,字符数组其余元素为'\0'.
 输出:printf("%s", str);
    输出字符数组时,遇到'\0'时,自动结束输出.

 char str[20];
 str = "...";//这样是编译不通过的
 char str[] = "hello\0world";
 printf("%s\n",str);//hello
 gets(str);//遇到换行符才结束
 puts(str);//遇到'\0'表示输出结束

 求字符串长度: 有效字符的个数 (不为'\0'的字符为有效字符,用strlen()函数得到的就是有效字符个数)
 求字符串占据的内存字节数: 有效字符的个数 + 1 (用sizeof()函数得到的就是内存占用字节个数)
 字符数组的大小: 一开始就定义好的字符数组长度(最多可容纳的字符个数,与有效字符的个数不一定相等)

 内存分区:
 ---栈区:main函数或其他函数中字义的变量,局部变量
 ---堆区:程序员动态分配的内存
 ---文字常量区:字符串常量,字面值
 ---全局静态区:全局变量,静态变量
 ---代码区:程序代码

 strcspn函数
 函数原型：extern int strcspn(char *str1,char *str2)
 参数说明：str1为参照字符串，即str2中每个字符分别与str1中的每个字符比较。
 所在库名：#include <string.h>
 函数功能：以str1为参照，比较字符串str2中的字符是否与str1中
     某个字符相等（也就是检索str2中的字符是否在str1中存在），
     如果第一次发现相等，则停止并返回在str1中这个匹配相等的字符的索引值，
     失败则返回str1的长度。
 strncmp()
 int strncmp(const char *str1, const char *str2, size_t n)
 参数
    str1 -- 要进行比较的第一个字符串。
    str2 -- 要进行比较的第二个字符串。
    n -- 要比较的最大字符数。
 该函数返回值如下：
    如果返回值 < 0，则表示 str1 小于 str2。
    如果返回值 > 0，则表示 str2 小于 str1。
    如果返回值 = 0，则表示 str1 等于 str2。

 new/delete关键字用法
 与malloc,free的区别:
 malloc,free是函数,需要包含头文件<malloc.h>,
 而new/delete是操作符,不需要包含任何头文件.

 拷贝构造函数也是构造函数的一种,因此也可以用类的初始化列表进行初始化数据;
 拷贝构造函数的参数只能是该类的引用,不能是该类的对象;因为如果是该类的对象,
 那么调用拷贝构造函数时,就是值传递,而值传递又会调用拷贝构造函数构造类对象,
 这样就不断的调用拷贝构造函数,形成死循环了,因此拷贝构造函数的参数必须是
 该类的引用.

 关于拷贝构造函数,如果C++程序员不自己定义拷贝构造函数,则C++编译器会自动生成
 一个默认的拷贝构造函数,这种情况没有在类中重新申请开辟资源,因此是浅拷贝;
 如果C++程序员自定义了拷贝构造函数,则C++编译器不会再生成默认的拷贝构造函数.

 浅拷和深拷贝的概念:在类中没有再重新申请开辟资源,就是浅拷贝;
 如果又重新申请开辟资源,则就是深拷贝.

 什么情况下需要自定义拷贝构造函数,当类中需要动态开辟内存时,需要自定义拷贝
 构造函数,进入深拷贝,重新申请资源;例如,当类中有指针成员时,且需要在成员函数
 中new资源,这种情况就必须要自定义拷贝构造函数.

 链表的基本概念
 定义:
 链表是一种物理存储上非连续,数据元素的逻辑顺序通过链表中的指针链接次序,
 实现的一种线性存储结构.
 特点:
 链表由一系列节点(链表中每一个元素称为节点)组成,节点在运行时动态生成(malloc),
 每个节点包括两个部分:
    一个是存储数据元素的数据域
    另一个是存储下一个节点地址的指针域


 C语言位操作
 逻辑与&&
 逻辑或||
 位与&
    真值表: 1&1=1 1&0=0 0&1=0 0&0=0
 位或|
    真值表: 1|1=1 1|0=1 0|1=1 0|0=0
 位取反~
 位异或^
 左移<<   如: 3 << 4 == 3 * 2 * 2 * 2 * 2
 右移>>   如: 3 >> 4 == 3 * 1 / (2 * 2 * 2 * 2)
 位与位或位异或在操作寄存器时的特殊作用
 特定位清零用&
 特定位置1用|
 特定位取反用^

 指针函数
 指针函数是指一个函数的返回值为地址量的函数
 指针函数的定义一般形式如下:
 <数据类型> * <函数名称>(<参数说明>){
    语句序列;
 }

 //error 出了函数内存就不存在了
 char *mystring() {
    char str[20];
    strcpy(str, "Hello");
    return str;//变量过期
 }
 全局变量,静态变量,字符串常量这些数据是被放到了静态区.程序结束前一直有效.
 针对上面的指针函数有三种改法:
 1.
 把char str[20];改成全局变量.
 2.
 在函数中把char str[20];改成static.
 3.
 把char str[20];改成字符串常量(char *str = "Hello";),
 由于字符串常量是不允许再修改的,因此需要把strcpy这句代码注释掉.
 总结:
    1.对于返回类型是指针类型的,就需要看看返回的数据是否还有效.
    2.指针函数在什么情况下可以返回值:
    全局变量地址,static变量地址,字符串常量地址,这三种是有效的.
    还有在函数中动态申请内存并返回也是有效的.
 char *strcat_(char *dest, const char *src) {
    char *p = dest;
    //1
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';

    //2
    while(*dest++);
    dest--;
    while(*dest++ = *src++);
    return p;
 }

 函数指针
 函数指针用来存放函数的地址,这个地址是一个函数的入口地址.
 1.函数名代表了函数的入口地址.
 2.函数指针变量说明的一般形式如下:
   <数据类型> (*<函数指针名称>)(<参数说明列表>);
   int (*p)(int, int);//函数指针的声明
   int (*p[2])(int, int);//函数指针数组
   p = add;//函数指针的赋值
   p[0] = add;
   p[1] = sub;
   (*p)(a, b);//函数指针的调用
   优点:
   普通函数的参数相同时,定义函数指针可以指向不同的函数,达到调用方便的目的.













































































































*/




#endif //MYSTUDY_STUDY