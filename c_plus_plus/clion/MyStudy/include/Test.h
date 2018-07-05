//
// Created by weidi on 2018/5/5.
//

#ifndef MYSTUDY_TEST_H
#define MYSTUDY_TEST_H


class Test {
public:
    Test();

    Test(int);

    Test(const Test &t);

    Test &operator=(const Test &t);

    ~Test();

    int getData();

private:
    int data;
};


#endif //MYSTUDY_TEST_H
