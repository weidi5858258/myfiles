//
// Created by root on 19-5-30.
//

#ifndef MYSTUDY_TESTCLASS_HPP
#define MYSTUDY_TESTCLASS_HPP

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



}

/***
 有关类的知识点:

 */

#endif //MYSTUDY_TESTCLASS_HPP
