//
// Created by root on 19-8-31.
//

#ifndef MYSTUDY_TESTSTRING_HPP
#define MYSTUDY_TESTSTRING_HPP

namespace alexander {
    // 类的定义
    class TestString {
    private:
        // 私有属性
    public:
        // 公有属性
    public:
        //公有方法
        TestString();

        ~TestString();

        // 测试string各种构造函数
        void testStringConstructor();

        void testSmartPointer();

    private:
        // 私有方法
    };

    //////////////////////////////////////////////////////////////////////

    // 类的实现
    TestString::TestString() {}

    TestString::~TestString() {}

    void TestString::testStringConstructor() {
        using namespace std;

        // 下面是怎样创建一个string对象

        // 用一个字符串初始化一个string对象
        string one("Lottery Winner!");
        cout << "one:   " << one << endl;
        cout << "one.size(): " << one.size() << endl;
        // 访问时越界也不会出错,只不过结果是个乱码而已
        cout << "one[100]: " << one[100] << endl;

        // 用20个'$'字符初始化一个string对象
        string two(20, '$');
        cout << "two:   " << two << endl;

        // 用其他string对象初始化一个string对象
        string three(one);
        cout << "three: " << three << endl;

        // 字符串拼接(直接用"+"就行了)
        one += " Oops!";
        cout << "one:   " << one << endl;

        two = "Sorry! That was ";
        // three字符串的第一个字符被改成了P,那么three字符串的内容就是: Pottery Winner!
        three[0] = 'P';
        string four;
        four = two + three;
        cout << "four:  " << four << endl;

        string four2 = two + three;
        cout << "four2: " << four2 << endl;

        char alls[] = "All's well that ends well";
        // 用alls字符串的前20个字符初始化一个string对象
        string five(alls, 20);
        // All's well that ends!
        cout << "five:  " << five << "!\n";

        string five2(alls, 40);
        // All's well that ends well �P�  �zE     !
        cout << "five2: " << five2 << "!\n";

        // 结果为alls的6~9之间的内容(不包括10这个位置的内容)
        string six(alls + 6, alls + 10);
        // well, .
        cout << "six:   " << six << ", ." << endl;

        // &不能少
        string six2(&alls[6], &alls[10]);
        // well, .
        cout << "six2:  " << six2 << ", ." << endl;

        string seven(&five[6], &five[10]);
        // well...
        cout << "seven: " << seven << "...\n";

        // 用four字符串的第7个位置开始,复制16个字符初始化一个string对象
        string eight(four, 7, 16);
        // That was Pottery
        cout << "eight: " << eight << endl;

        // 读文件操作
        /*ifstream fin;
        fin.open("tobuy.txt");
        if (!fin.is_open()) {
            cerr << "Can't open file. Bye." << endl;
            exit(EXIT_FAILURE);
        }
        string item;
        int count = 0;
        getline(fin, item, ':');
        while (fin) {
            count++;
            cout << count << ": " << item << endl;
            getline(fin, item, ':');
        }
        fin.close();*/
    }

    void TestString::testSmartPointer() {
        // 智能指针的创建
        // 第一种方式
        shared_ptr<double> pd1(new double(10.000000));
        double *p_reg = new double;
        // 第二种方式
        shared_ptr<double> pd2;
        pd2 = shared_ptr<double>(p_reg);
        // 第三种方式
        shared_ptr<double> pd3(p_reg);

        // 使用auto_ptr的缺陷(赋给其他变量后就不能再使用)
        auto_ptr<string> films(new string("Hello World!"));
        cout << *films << endl;
        auto_ptr<string> pwin;
        // 被赋值后,films不再引用原来的字符串
        // 总之:使用auto_ptr创建的智能指针赋给其他智能指针后,就不能再次使用,不然会出现段错误
        // pwin接管string对象的所有权,films的所有权被剥夺
        pwin = films;
        // 会出现段错误
        // cout << *films << endl;
        cout << *pwin << endl;

        // 使用shared_ptr进行赋值后就不会有问题
        shared_ptr<string> tempStr(new string("shared_ptr"));
        shared_ptr<string> pStr;
        pStr = tempStr;
        cout << *tempStr << endl;
        cout << *pStr << endl;

    }

}

#endif //MYSTUDY_TESTSTRING_HPP
