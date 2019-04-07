#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include "twinswidgetmother.h"

namespace Ui {
    class UiMainWindow;
}

class UiMainWindow : public QMainWindow {

    Q_OBJECT

    private:
    Ui::UiMainWindow * ui;

    public:
    explicit UiMainWindow(QWidget * parent = nullptr);
    ~UiMainWindow();

    private:
    void init();

};

#endif // UIMAINWINDOW_H
