#include "firstwidget.h"

FirstWidget::FirstWidget(QWidget * parent) : QWidget(parent) {

    resize(500, 500);

    button1.setParent(this);
    button1.setText("close");
    button1.move(200, 200);

    button2 = new QPushButton(this);
    button2->setText("button2");
    button2->move(300, 300);

    connect(&button1, &QPushButton::pressed, this, &FirstWidget::close);// *NOPAD*
    //connect(button2, &QPushButton::released, this, &FirstWidget::testSlot); // *NOPAD*
    connect(button2, &QPushButton::released, // *NOPAD*
            &button1, &QPushButton::hide); // *NOPAD*
}

void FirstWidget::testSlot() {
    button2->setText("Hahaha");
}

FirstWidget::~FirstWidget() {

}




/***
    const QObject *sender,      信号发出者
    const QMetaMethod &signal,  怎样产生信号
    const QObject *receiver,    信号接收者
    const QMetaMethod &method,  处理信号的槽函数
    Qt::ConnectionType type = Qt::AutoConnection

    const QObject *sender,
    const char *signal,
    const QObject *receiver,
    const char *member,
    Qt::ConnectionType = Qt::AutoConnection

    const QObject *sender,
    const char *signal,
    const char *member,
    Qt::ConnectionType type = Qt::AutoConnection

    &QPushButton::pressed  在某个按钮上一按就会触发
    &QPushButton::clicked  在某个按钮上按下去然后在这个按钮上松开鼠标才会触发
    &QPushButton::released 在某个按钮上按下去然后鼠标一移开此按钮就会触发
    &QPushButton::toggled  使用于切换状态的按钮

    自定义槽,普通函数的用法
    QT5: 任意成员函数,普通全局函数,静态函数
    槽函数需要和信号一致(参数, 返回值)
    由于信号都是没有返回值的,所以槽函数一定没有返回值
*/
