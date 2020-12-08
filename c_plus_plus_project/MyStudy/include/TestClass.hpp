//
// Created by root on 19-5-30.
//

#ifndef MYSTUDY_TESTCLASS_HPP
#define MYSTUDY_TESTCLASS_HPP

#include <cstring>

namespace alexander {
    // 爷爷类
    class Grandpa {
    public:
        Grandpa() {
            cout << "Grandpa()" << endl;
        }

        virtual ~Grandpa() {
            cout << "~Grandpa()" << endl;
        }
    };

    // 爸爸类
    class Father : virtual public Grandpa {
    public:
        Father() {
            cout << "Father()" << endl;
        }

        virtual ~Father() {
            cout << "~Father()" << endl;
        }
    };

    // 妈妈类
    class Mather : virtual public Grandpa {
    public:
        Mather() {
            cout << "Mather()" << endl;
        }

        Mather(int x) {
            cout << "Mather(int)" << endl;
        }

        virtual ~Mather() {
            cout << "~Mather()" << endl;
        }
    };

    // 儿子类(如果继承时不写public,那么默认就是private继承,这样父类中的任何成员函数都不能使用)
    class Son : virtual public Father, virtual public Mather {
    public:
        int data;
    public:
        Son() {
            cout << "Son()" << endl;
        }

        // 如果不显式的写出调用哪个父类构造函数的话,那么默认就是调用无参构造函数
        Son(int x) : Mather(x) {
            cout << "Son(int)" << endl;
        }

        virtual ~Son() {
            cout << "~Son()" << endl;
        }

        void setData() const {
            cout << "data: " << data << endl;
            // data = 10;
        }
    };

    /////////////////////////////////////////////////////////////////////////////////

    class RuleOfThree {
    private:
        char *cstring;
    public:
        RuleOfThree(const char *arg) : cstring(new char[std::strlen(arg) + 1]) {
            std::strcpy(cstring, arg);
        }

        ~RuleOfThree() {
            delete[] cstring;
        }

        // copy constructor
        RuleOfThree(const RuleOfThree &other) {
            cstring = new char[std::strlen(other.cstring) + 1];
            std::strcpy(cstring, other.cstring);
        }

        // copy assignment
        RuleOfThree &operator=(const RuleOfThree &other) {
            char *tmp_cstring = new char[std::strlen(other.cstring) + 1];
            std::strcpy(tmp_cstring, other.cstring);
            delete[] cstring;
            cstring = tmp_cstring;
            return *this;
        }

        // 如果不想要拷贝构造函数,那么只需要声明成private就行了,这样使用的时候就会报错
        // 这样做是为了阻止编译器自动生成相关的代码而引起错误导致程序运行不正常
    /*private:
        RuleOfThree(const RuleOfThree &other) = delete;

        RuleOfThree &operator=(const RuleOfThree &other) = delete;*/
    };

}

/***
 有关类的知识点:

 */

#endif //MYSTUDY_TESTCLASS_HPP
