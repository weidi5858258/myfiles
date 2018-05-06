#include <QCoreApplication>
#include <iostream>

using namespace std;

class Test{
public:
    Test(int d = 0):data(d){
        cout<<"Create Test(int) Object: "<<this<<endl;
    }
    ~Test(){
        cout<<"Free Test Object: "<<this<<endl;
    }

private:
    int data;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<<"Hello World"<<endl;
    Test t(10);

    return a.exec();
}
