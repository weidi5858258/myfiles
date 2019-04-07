#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
    init();
}

MainWindow::~MainWindow() {

}

QToolBar * MainWindow::addToolBar(Qt::ToolBarArea area,
                                  const QString & title) {
    QToolBar * qToolBar = new QToolBar(title);
    QMainWindow::addToolBar(area, qToolBar);
    return qToolBar;
}

void MainWindow::init() {
    resize(800, 800);
    setWindowTitle(QString("Android Studio 3.0"));

    // 菜单栏
    QMenuBar * qMenuBar = menuBar();
    // 定义各种菜单
    initFileMenu(qMenuBar);
    initEditMenu(qMenuBar);
    initViewMenu(qMenuBar);
    initNavigateMenu(qMenuBar);
    initCodeMenu(qMenuBar);
    initAnalyzeMenu(qMenuBar);
    initRefactorMenu(qMenuBar);
    initBuildMenu(qMenuBar);
    initRunMenu(qMenuBar);
    initToolsMenu(qMenuBar);
    initVcsMenu(qMenuBar);
    initWindowMenu(qMenuBar);
    initHelpMenu(qMenuBar);
    // 工具栏
    initIconToolBar();
    initLeftToolBar();
    initRightToolBar();
    // 状态栏
    initStatusBar();
    // 文本编辑区
    initTextEdit();

    // 浮动窗口
    //QDockWidget * qDockWidget = new QDockWidget(this);
    //Qt::DockWidgetArea dockWidgetArea = Qt::RightDockWidgetArea;
    //addDockWidget(dockWidgetArea, qDockWidget);
}

void MainWindow::initFileMenu(QMenuBar * qMenuBar) {
    // File菜单
    QMenu * fileMenu = qMenuBar->addMenu(QString("File"));
    fileMenu->setMinimumWidth(250);
    // 添加菜单项
    // New
    initFileMenuWithNew(fileMenu);
    // Open
    initFileMenuWithOpen(fileMenu);
    // 分隔线
    fileMenu->addSeparator();
    // Setting
    initFileMenuWithSetting(fileMenu);
    // 分隔线
    fileMenu->addSeparator();
    //Exit
    initFileMenuWithExit(fileMenu);
}

void MainWindow::initFileMenuWithNew(QMenu * fileMenu) {
    action = fileMenu->addAction(QString("New"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "New...";
    });
}

void MainWindow::initFileMenuWithOpen(QMenu * fileMenu) {
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v016_12.png"));
    action = fileMenu->addAction(qIcon, QString("Open..."));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Open...";
    });

}

void MainWindow::initFileMenuWithSetting(QMenu * fileMenu) {
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v017_12.png"));
    action = fileMenu->addAction(qIcon, QString("Setting..."));// Ctrl+Alt+S
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Setting...";
    });

    // 弹出对话框后会挡住后面的操作
    action = fileMenu->addAction(QString("模态对话框"));
    connect(action, & QAction::triggered,
    []() {
        QDialog dialog;
        dialog.exec();
        qDebug() << "模态对话框";
    });

    // 弹出对话框后不会挡住后面的操作
    action = fileMenu->addAction(QString("非模态对话框"));
    connect(action, & QAction::triggered,
    [ = ]() {
        /***
            当应用被关闭后,这个对话框还显示着
        */
        QDialog * dialog = new QDialog;
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
        qDebug() << "非模态对话框";
    });

    // 关于对话框
    action = fileMenu->addAction(QString("关于对话框"));
    connect(action, & QAction::triggered,
    [ = ]() {
        QMessageBox::about(this, QString("关于"), QString("好嗨哟!!!"));
        qDebug() << "关于对话框";
    });

    // 确认对话框
    action = fileMenu->addAction(QString("确认对话框"));
    connect(action, & QAction::triggered,
    [ = ]() {
        int result = QMessageBox::question(this, QString("确认"),
                                           QString("今天心情好吗?"),
                                           QMessageBox::Ok | QMessageBox::No);
        switch (result) {
            case QMessageBox::Ok:
                qDebug() << "QMessageBox::Ok" << result;
                break;
            case QMessageBox::No:
                qDebug() << "QMessageBox::No" << result;
                break;
            default:
                break;
        }
    });

    // 文件对话框
    action = fileMenu->addAction(QString("文件对话框"));
    connect(action, & QAction::triggered,
    [ = ]() {
        QString fileName = QFileDialog::getOpenFileName(this,
                           QString("请选择文件"),
                           QString("../"),
                           QString("源文件(*.h *.cpp);;"
                                   "图片(*.jpg *.png *.bmp);;"
                                   "音频(*.mp3 *.aac);;"
                                   "视频(*.mp4 *.avi *.mkv);;"
                                   "全部(*.*)"));
        qDebug() << fileName.toUtf8().data();
    });
}

void MainWindow::initFileMenuWithExit(QMenu * fileMenu) {
    action = fileMenu->addAction(QString("Exit"));
    connect(action, & QAction::triggered,
    [ = ]() {
        qDebug() << "Exit...";
        close();
        // emit close();
    });
}

void MainWindow::initEditMenu(QMenuBar * qMenuBar) {
    QMenu * editMenu = qMenuBar->addMenu(QString("Edit"));
}

void MainWindow::initViewMenu(QMenuBar * qMenuBar) {
    QMenu * viewMenu = qMenuBar->addMenu(QString("View"));
}

void MainWindow::initNavigateMenu(QMenuBar * qMenuBar) {
    QMenu * navigateMenu = qMenuBar->addMenu(QString("Navigate"));
}

void MainWindow::initCodeMenu(QMenuBar * qMenuBar) {
    QMenu * codeMenu = qMenuBar->addMenu(QString("Code"));
}

void MainWindow::initAnalyzeMenu(QMenuBar * qMenuBar) {
    QMenu * analyzeMenu = qMenuBar->addMenu(QString("Analyze"));
}

void MainWindow::initRefactorMenu(QMenuBar * qMenuBar) {
    QMenu * refactorMenu = qMenuBar->addMenu(QString("Refactor"));
}

void MainWindow::initBuildMenu(QMenuBar * qMenuBar) {
    QMenu * buildMenu = qMenuBar->addMenu(QString("Build"));
}

void MainWindow::initRunMenu(QMenuBar * qMenuBar) {
    QMenu * runMenu = qMenuBar->addMenu(QString("Run"));
}

void MainWindow::initToolsMenu(QMenuBar * qMenuBar) {
    QMenu * toolsMenu = qMenuBar->addMenu(QString("Tools"));
}

void MainWindow::initVcsMenu(QMenuBar * qMenuBar) {
    QMenu * vcsMenu = qMenuBar->addMenu(QString("VCS"));
}

void MainWindow::initWindowMenu(QMenuBar * qMenuBar) {
    QMenu * windowMenu = qMenuBar->addMenu(QString("Window"));
}

void MainWindow::initHelpMenu(QMenuBar * qMenuBar) {
    QMenu * helpMenu = qMenuBar->addMenu(QString("Help"));
}

void MainWindow::initIconToolBar() {
    // Top工具栏
    QToolBar * iconToolBar = addToolBar(Qt::TopToolBarArea,
                                        QString("IconToolBar"));
    // 工具栏不可移动
    iconToolBar->setMovable(false);
    // 定义功能
    // 创建图标
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v015_24.png"));
    // 把该功能添加到工具栏上
    action = iconToolBar->addAction(qIcon, QString("Pull"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Pull...";
    });
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v010_24.png"));
    action = iconToolBar->addAction(qIcon, QString("Sync"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Sync...";
    });
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v023_24.png"));
    action = iconToolBar->addAction(qIcon, QString("Push"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Push...";
    });
    // 分隔线
    iconToolBar->addSeparator();
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v010_24.png"));
    action = iconToolBar->addAction(qIcon, QString("Tool"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Tool...";
    });
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v021_24.png"));
    QPushButton * button = new QPushButton(qIcon, QString("app"), this);
    iconToolBar->addWidget(button);
}

void MainWindow::initLeftToolBar() {
    // Left工具栏
    QToolBar * leftTextToolBar = addToolBar(Qt::LeftToolBarArea,
                                            QString("LeftTextToolBar"));
    leftTextToolBar->setMovable(false);
    qIcon.addFile(
        QString("C:\\Users\\weidi5858258\\Pictures\\icon\\v007_12.png"));
    action = leftTextToolBar->addAction(qIcon, QString("Project"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Project...";
    });
}

void MainWindow::initRightToolBar() {
    // Right工具栏
    QToolBar * rightTextToolBar = addToolBar(Qt::RightToolBarArea,
                                  QString("RightTextToolBar"));
    rightTextToolBar->setMovable(false);
    action = rightTextToolBar->addAction(QString("Gradle"));
    connect(action, & QAction::triggered,
    []() {
        qDebug() << "Gradle...";
    });
}

void MainWindow::initStatusBar() {
    // 状态栏
    QStatusBar * qStatusBar = statusBar();
    QLabel * label = nullptr;
    label = new QLabel(QString("1 问题"), this);
    // 从左往右加
    qStatusBar->addWidget(label);
    label = new QLabel(QString("2 Search Results"), this);
    qStatusBar->addWidget(label);
    label = new QLabel(QString("3 应用程序输出"), this);
    qStatusBar->addWidget(label);
    label = new QLabel(QString("4 编译输出"), this);
    qStatusBar->addWidget(label);
    label = new QLabel(QString("Plain Text"), this);
    // 从右往左加
    qStatusBar->addPermanentWidget(label);
}

void MainWindow::initTextEdit() {
    // 核心控件
    QTextEdit * qTextEdit = new QTextEdit(QString(""), this);
    setCentralWidget(qTextEdit);
}



































