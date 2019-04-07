#include "twinswidgetboss.h"

TwinsWidgetBoss::TwinsWidgetBoss(QWidget * parent) : QWidget(parent) {
    resize(300, 300);
    setWindowTitle(QString("我是老大"));

    mBossBtn.setText("去老二家");
    mBossBtn.move(150, 150);
    mBossBtn.setParent(this);

    // 比较麻烦的注册信号方式
    //    connect( & mBossBtn, & QPushButton::pressed,
    //             this, & TwinsWidgetBoss::goToPenisHome);

    /***
        使用Lambda表达式进行注册(好处是不需要再写槽函数了,使用方便)
        []是可以进行传参的.
        [mBossBtn]: 这样在函数体中就可以使用mBossBtn这个变量了, 否则会报错的.
        [=]:        这样的意思是把外部所有局部变量,类中所有成员以值传递的方式传进来(建议使用).
        [&]:        把外部所有局部变量以引用方式传递进来(不要使用这种方式,很容易出问题).
        [this]:     类中所有成员以值传递的方式传进来.
        不过这样传进去的值是只读的,不能修改的,否则会报错的.
        在()后面加上mutable关键字的话就可以修改了.
        ()中的checked参数是clicked这个信号传递进去的.
    */
    connect( & mBossBtn, & QPushButton::clicked,
    [ = ](bool checked) mutable -> int {
        hide();
        // 发送callPenisSignals信号
        emit mMother->callPenisSignals();

        qDebug() << "发生了什么???" << checked;

        return 0;
    });
}

void TwinsWidgetBoss::setMother(TwinsWidgetMother * mother) {
    mMother = mother;

    // 注册TwinsWidgetMother::callBossSignals信号
    // 使用TwinsWidgetBoss::handlePenisSlots函数处理接收到信号后的操作
    // 注册无参的信号
    void (TwinsWidgetMother:: * boss1)() =
        & TwinsWidgetMother::callBossSignals;
    connect(mMother, boss1,
            this, & TwinsWidgetBoss::handlePenisSlots);
    // 注册有参的信号
    void (TwinsWidgetMother:: * boss2)(int, QString) =
        & TwinsWidgetMother::callBossSignals;
    connect(mMother, boss2,
            this, & TwinsWidgetBoss::handlePenisSlots2);
    /***
        使用QT4的方式注册信号
        1.QT4槽函数必须用slots关键字进行修饰
        2.SIGNAL宏和SLOT宏会把callBossSignals()当作字符串处理,从而不进行语法的检查,
        因此如果把有关的函数名改了的话,在编译时就不会报错,这样就不太容易发现问题.
        基于以上两点,不建议使用这种方式进行注册
        connect(mMother, SIGNAL(callBossSignals()),
            this, SLOT(handlePenisSlots()));
        connect(mMother, SIGNAL(callBossSignals(int, QString)),
            this, SLOT(handlePenisSlots2()));
    */
}

void TwinsWidgetBoss::goToPenisHome() {
    this->hide();

    // 发送callPenisSignals信号
    emit mMother->callPenisSignals();
}

void TwinsWidgetBoss::handlePenisSlots() {
    this->show();
}

void TwinsWidgetBoss::handlePenisSlots2(int a, QString str) {
    qDebug() << a << str.toUtf8().data();
}

/***
    这里执行的流程是:
    1.先对mBossBtn注册点击事件,点击事件执行的函数是goToPenisHome
    2.执行goToPenisHome函数时,发送一个callPenisSignals信号给老二
    3.老二能够接收到callPenisSignals这个信号首先需要注册这个信号
    4.
*/
