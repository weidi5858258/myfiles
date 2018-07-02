//
// Created by weidi5858258 on 2017/12/30.
//

/***
 使用：
 #define TAG "LinuxSocket"
 #define NEEDLOG true（每个类自己控制要不要打印日志）
 MyLog::i(this, TAG, __func__, NEEDLOG);
 MyLog::i(this, "LinuxSocket", "studyHard()", true);
 */

#include "MyLog.h"

// 初始化静态成员(静态属性必须要初始化)
bool MyLog::needLog_(false);
string MyLog::separator_("");

void MyLog::setNeedLog(const bool log) {
    MyLog::needLog_ = log;
}

void MyLog::setSeparator(string separator) {
    MyLog::separator_ = separator;
}

void MyLog::i(string TAG, string info) {
    if (MyLog::needLog_) {
        cout << TAG << MyLog::separator_ << info << endl;
    }
}

void MyLog::i(void *obj, string TAG, string info) {
    if (MyLog::needLog_) {
        cout << obj << MyLog::separator_ << TAG << MyLog::separator_ << info << endl;
    }
}

void MyLog::i(string TAG, string info, bool needLog) {
    if (MyLog::needLog_ && needLog) {
        cout << TAG << MyLog::separator_ << info << endl;
    }
}

void MyLog::i(void *obj, string TAG, string info, bool needLog) {
    if (MyLog::needLog_ && needLog) {
        cout << obj << MyLog::separator_ << TAG << MyLog::separator_ << info << endl;
    }
}
