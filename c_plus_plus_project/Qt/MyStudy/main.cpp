// #include <QCoreApplication>
// QCoreApplication a(argc, argv);

#include "firstwidget.h"
#include "twinswidgetboss.h"
#include "twinswidgetpenis.h"
#include "mainwindow.h"
#include "uimainwindow.h"

int main(int argc, char * argv[]) {
    // *INDENT-OFF*
        QApplication a(argc, argv);
    // *INDENT-ON*
    // 创建构造函数时,肯定是先创建父类构造函数,再创建自己的(先有父母亲才会有孩子)
    FirstWidget firstWidget;
    firstWidget.setWindowTitle(QString("我的第一个窗口应用"));
    QPushButton button;
    button.setText("^_^");
    button.setParent(&firstWidget);// *NOPAD*
    button.move(100, 100);
    // 放在最后show时button才会显示
    // firstWidget.show();
    TwinsWidgetMother mother;
    TwinsWidgetBoss boss;
    TwinsWidgetPenis penis;
    boss.setMother( & mother);
    penis.setMother( & mother);
    // boss.show();
    //MainWindow mainWindow;
    //mainWindow.show();
    UiMainWindow uiMainWindow;
    uiMainWindow.show();
    // 让程序一直执行,等待用户操作
    // 等待事件发生
    return a.exec();
}

/***
    类的定义:
    1.定义类的属性
    2.定义类的方法
*/
class Test {
    // *INDENT-OFF*

    // 先定义类的属性
    private:
        int data;

    public:
        int name;


    public:
        Test() : data(0) {

        }

        Test(int d = 0): data(d) {
            // 需要输出的内容跟前面的内容之间会自动添加空格,而且会自动换行
            qDebug() << "Create Test(int) Object:" << this;
            cout << "Create Test(int) Object: " << this << endl;
        }

        ~Test() {
            qDebug() << "Destroy Test Object:" << this ;
        }

    private:
        void test1() {

        }

    // *INDENT-ON*
};


















