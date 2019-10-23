//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_TEST_H
#define MYSTUDY_TEST_H

#include "MyHead.h"

class Test {
public:
    Test();

    /***
     如果声明成Test(int d = 0);那么就会报错,因为这样跟默认构造是一样的,这样就二义性.
     */
    Test(int d);

    Test(const Test &t);

    Test &operator=(const Test &t);

    ~Test();

    int getData();

private:
    int data;
};


#endif //MYSTUDY_TEST_H
