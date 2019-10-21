//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_TEST_H
#define MYSTUDY_TEST_H

#include "MyHead.h"

class Test {
public:
    Test();

    Test(int d = 0);

    Test(const Test &t);

    Test &operator=(const Test &t);

    ~Test();

    int getData();

private:
    int data;
};


#endif //MYSTUDY_TEST_H
