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

        virtual ~Mather() {
            cout << "~Mather()" << endl;
        }
    };

    // 儿子类
    class Son : public Father, public Mather {
    public:
        Son() {
            cout << "Son()" << endl;
        }

        virtual ~Son() {
            cout << "~Son()" << endl;
        }
    };

    /////////////////////////////////////////////////////////////////////////////////



}

/***
 有关类的知识点:

 */

#endif //MYSTUDY_TESTCLASS_HPP
