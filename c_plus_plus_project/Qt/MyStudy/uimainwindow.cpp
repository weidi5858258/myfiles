#include "uimainwindow.h"
#include "ui_uimainwindow.h"

UiMainWindow::UiMainWindow(QWidget * parent) :
QMainWindow(parent),
ui(new Ui::UiMainWindow) {
    // 这句代码放最前面
    ui->setupUi(this);

    init();
}

UiMainWindow::~UiMainWindow() {
    delete ui;
}

void UiMainWindow::init() {
    resize(800, 500);
}
