//
// Created by weidi on 2018/5/18.
//

#include "MyHead2.h"

#ifdef UBUNTU_SYSTEM
#include "../include/MyHead.h"
#else
#include "MyHead.h"
#endif

class B0 {
public:
    B0() {
        cout << "B0 object is created." << endl;
    }

    ~B0() {
        cout << "B0 object is deleted." << endl;
    }

    void print() {
        cout << "B0 object print()." << endl;
    }

public:
    int b0 = 1;
};

class B1 : public B0 {
public:
    B1() {
        cout << "B1 object is created." << endl;
    }

    ~B1() {
        cout << "B1 object is deleted." << endl;
    }

    virtual void print() {
        cout << "B1 object print()." << endl;
    }

public:
    int m = 100;
    int b1 = 10;
};

class B2 : virtual public B0 {
public:
    B2() {
        cout << "B2 object is created." << endl;
    }

    ~B2() {
        cout << "B2 object is deleted." << endl;
    }

    void print() {
        cout << "B2 object print()." << endl;
    }

public:
    int m = 200;
    int b2 = 20;
};

class C : public B1, virtual public B2 {
public:
    C() {
        cout << "C object is created." << endl;
    }

    ~C() {
        cout << "C object is deleted." << endl;
    }

    void print() {
        cout << "C object print()." << endl;
    }

public:
    int c = 30;
};

class Base {
public:
    Base() {}

    ~Base() {}

private:
    int x = 0;
};