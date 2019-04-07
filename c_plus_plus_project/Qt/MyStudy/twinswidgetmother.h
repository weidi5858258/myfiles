#ifndef TWINSWIDGETMOTHER_H
#define TWINSWIDGETMOTHER_H

#include "myheader.h"

/***
    这个类的作用就是专门放信号函数的
    要使用到信号的地方都可以共用这个类
    这样就不需要某个类中包含另外类的对象

    老大发送母亲这里定义好的信号给老二,相当于老大跟老二通信时需要通过母亲中转.
    老二发送信号给老大也是同样的情况.

    在其他头文件中,只要包含这个头文件就行了
*/
class TwinsWidgetMother : public QWidget {
    // *INDENT-OFF*

    Q_OBJECT

    public:
        explicit TwinsWidgetMother(QWidget * parent = nullptr) :
            QWidget(parent) {
        }

    // 定义各种信号(widget之间通信时用到的信号)
    signals:
        void callBossSignals();
        void callPenisSignals();
        void callBossSignals(int, QString);

    // 定义各种槽函数(因为槽函数是要在各自的类中使用的,所以不在这里定义)
    public slots:

    // *INDENT-ON*
};

#endif // TWINSWIDGETMOTHER_H
