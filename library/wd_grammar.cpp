// #include <linux/in.h>
// #include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void test1(void);
void test2(void);

void test_grammar()
{
    // test1();
    test2();
}

void test2()
{
    Mat img = imread("/root/Pictures/res/test.jpg");
    imshow("image", img);
    waitKey(6000);
}

namespace A
{
void fun()
{
    cout << "This is namespace A" << endl;
}
}
namespace B
{
void fun()
{
    cout << "This is namespace B" << endl;
}
}

void test1()
{
    using namespace B;
    fun();
}

/***
全名空间的使用
*/
