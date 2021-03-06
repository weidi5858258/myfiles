//
// Created by weidi on 2018/5/5.
//

#ifdef WIN32

#include "Test.h"

#else

#include "../include/Test.h"

#endif

Test::Test() {
    std::cout << "Create Test() Object: " << this << std::endl;
    data = 0;
}

Test::Test(int d) : data(d) {
    std::cout << "Create Test(int) Object: " << this << std::endl;
}

Test::Test(const Test &t) {
    std::cout << "Create Test(Test &) Object: " << this << std::endl;
    this->data = t.data;
}

Test &Test::operator=(const Test &t) {
    std::cout << "Call operator=(Test &): " << this << std::endl;
    if (this != &t) {
        this->data = t.data;
    }
    return *this;
}

Test::~Test() {
    std::cout << "Free Test Object: " << this << std::endl;
}

int Test::getData() {
    printf("Test:%s\n", __FUNCTION__);
    return data;
}



