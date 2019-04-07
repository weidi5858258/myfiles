#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "twinswidgetmother.h"

class MainWindow : public QMainWindow {

    Q_OBJECT

    private:
    QAction * action = nullptr;
    QIcon qIcon;

    public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

    signals:

    public slots:

    private:
    QToolBar * addToolBar(Qt::ToolBarArea area,
                          const QString & title);
    void init();
    void initFileMenu(QMenuBar * qMenuBar);
    void initFileMenuWithNew(QMenu * fileMenu);
    void initFileMenuWithOpen(QMenu * fileMenu);
    void initFileMenuWithSetting(QMenu * fileMenu);
    void initFileMenuWithExit(QMenu * fileMenu);
    void initEditMenu(QMenuBar * qMenuBar);
    void initViewMenu(QMenuBar * qMenuBar);
    void initNavigateMenu(QMenuBar * qMenuBar);
    void initCodeMenu(QMenuBar * qMenuBar);
    void initAnalyzeMenu(QMenuBar * qMenuBar);
    void initRefactorMenu(QMenuBar * qMenuBar);
    void initBuildMenu(QMenuBar * qMenuBar);
    void initRunMenu(QMenuBar * qMenuBar);
    void initToolsMenu(QMenuBar * qMenuBar);
    void initVcsMenu(QMenuBar * qMenuBar);
    void initWindowMenu(QMenuBar * qMenuBar);
    void initHelpMenu(QMenuBar * qMenuBar);
    void initIconToolBar();
    void initLeftToolBar();
    void initRightToolBar();
    void initStatusBar();
    void initTextEdit();

};

#endif // MAINWINDOW_H
