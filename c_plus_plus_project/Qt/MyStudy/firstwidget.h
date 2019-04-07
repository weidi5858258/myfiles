#ifndef FIRSTWIDGET_H
#define FIRSTWIDGET_H

#include "twinswidgetmother.h"

class FirstWidget : public QWidget {
    // 信号与槽的时候使用到
    Q_OBJECT

    // *INDENT-OFF*

    private:
        QPushButton button1;
        QPushButton *button2;

    public:
        explicit FirstWidget(QWidget * parent = 0);
        ~FirstWidget();
        void testSlot();

    private:
        void test();

    /***
        信号必须有signals关键字来声明
        信号没有返回值,但可以有参数
        信号就是函数的声明,只需声明,无需定义,只是直到标识的作用
        信号可以重载
        能够做到接收到信号的前提是首先注册这个信号
        发送信号: emit testSignal();
        可能使用的场景:
        在某个控件上先定义好接收到这个信号后该怎么做,
        然后点击某个按钮后在槽函数中发送一个这个信号,
        这样之前定义好的控件就能接收到这个信号,这样就能使用
        槽函数处理事情了.
    */
    signals:
        void testSignal();

    public slots:

    // *INDENT-ON*
};

#endif // FIRSTWIDGET_H
