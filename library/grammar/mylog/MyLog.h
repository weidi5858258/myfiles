//
// Created by weidi5858258 on 2017/12/30.
//

#ifndef GRAMMAR_LOG_H
#define GRAMMAR_LOG_H

#include <iostream>
#include <string>

using namespace std;

class MyLog {
public:
    static void setNeedLog(bool needLog);

    static void setSeparator(string separator);

    static void i(string TAG, string info);

    static void i(void *obj, string TAG, string info);

    static void i(string TAG, string info, bool needLog);

    static void i(void *obj, string TAG, string info, bool needLog);

private:
    // 声明静态成员
    static bool needLog_;
    static string separator_;
};

#endif //GRAMMAR_LOG_H
