/********************************************************************************
** Form generated from reading UI file 'uimainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIMAINWINDOW_H
#define UI_UIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiMainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSetting;
    QAction *actionNewProject;
    QAction *actionImportProject;
    QAction *actionModalityDialog;
    QAction *actionModelessDialog;
    QAction *actionAboutDialog;
    QAction *actionConfirmDialog;
    QAction *actionFileDialog;
    QAction *actionExit;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuNew;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuNavigate;
    QMenu *menuCode;
    QMenu *menuAnalyze;
    QMenu *menuRefactor;
    QMenu *menuBuild;
    QMenu *menuRun;
    QMenu *menuTools;
    QMenu *menuVcs;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *UiMainWindow)
    {
        if (UiMainWindow->objectName().isEmpty())
            UiMainWindow->setObjectName(QString::fromUtf8("UiMainWindow"));
        UiMainWindow->resize(1013, 327);
        actionOpen = new QAction(UiMainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8(":/image/v001_24.png");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionOpen->setIcon(icon);
        actionSetting = new QAction(UiMainWindow);
        actionSetting->setObjectName(QString::fromUtf8("actionSetting"));
        QIcon icon1;
        iconThemeName = QString::fromUtf8(":/image/v016_24.png");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionSetting->setIcon(icon1);
        QFont font;
        actionSetting->setFont(font);
        actionNewProject = new QAction(UiMainWindow);
        actionNewProject->setObjectName(QString::fromUtf8("actionNewProject"));
        actionImportProject = new QAction(UiMainWindow);
        actionImportProject->setObjectName(QString::fromUtf8("actionImportProject"));
        actionModalityDialog = new QAction(UiMainWindow);
        actionModalityDialog->setObjectName(QString::fromUtf8("actionModalityDialog"));
        actionModelessDialog = new QAction(UiMainWindow);
        actionModelessDialog->setObjectName(QString::fromUtf8("actionModelessDialog"));
        actionAboutDialog = new QAction(UiMainWindow);
        actionAboutDialog->setObjectName(QString::fromUtf8("actionAboutDialog"));
        actionConfirmDialog = new QAction(UiMainWindow);
        actionConfirmDialog->setObjectName(QString::fromUtf8("actionConfirmDialog"));
        actionFileDialog = new QAction(UiMainWindow);
        actionFileDialog->setObjectName(QString::fromUtf8("actionFileDialog"));
        actionExit = new QAction(UiMainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(UiMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(446, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(446, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        UiMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(UiMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1013, 28));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setMinimumSize(QSize(250, 0));
        menuNew = new QMenu(menuFile);
        menuNew->setObjectName(QString::fromUtf8("menuNew"));
        menuNew->setMinimumSize(QSize(250, 0));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuNavigate = new QMenu(menubar);
        menuNavigate->setObjectName(QString::fromUtf8("menuNavigate"));
        menuCode = new QMenu(menubar);
        menuCode->setObjectName(QString::fromUtf8("menuCode"));
        menuAnalyze = new QMenu(menubar);
        menuAnalyze->setObjectName(QString::fromUtf8("menuAnalyze"));
        menuRefactor = new QMenu(menubar);
        menuRefactor->setObjectName(QString::fromUtf8("menuRefactor"));
        menuBuild = new QMenu(menubar);
        menuBuild->setObjectName(QString::fromUtf8("menuBuild"));
        menuRun = new QMenu(menubar);
        menuRun->setObjectName(QString::fromUtf8("menuRun"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuVcs = new QMenu(menubar);
        menuVcs->setObjectName(QString::fromUtf8("menuVcs"));
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        UiMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(UiMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        UiMainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuNavigate->menuAction());
        menubar->addAction(menuCode->menuAction());
        menubar->addAction(menuAnalyze->menuAction());
        menubar->addAction(menuRefactor->menuAction());
        menubar->addAction(menuBuild->menuAction());
        menubar->addAction(menuRun->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuVcs->menuAction());
        menubar->addAction(menuWindow->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(menuNew->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSetting);
        menuFile->addSeparator();
        menuFile->addAction(actionModalityDialog);
        menuFile->addAction(actionModelessDialog);
        menuFile->addAction(actionAboutDialog);
        menuFile->addAction(actionConfirmDialog);
        menuFile->addAction(actionFileDialog);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuNew->addAction(actionNewProject);
        menuNew->addAction(actionImportProject);

        retranslateUi(UiMainWindow);
        QObject::connect(actionExit, SIGNAL(triggered(bool)), UiMainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(UiMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *UiMainWindow)
    {
        UiMainWindow->setWindowTitle(QApplication::translate("UiMainWindow", "MainWindow", nullptr));
        actionOpen->setText(QApplication::translate("UiMainWindow", "Open...", nullptr));
        actionSetting->setText(QApplication::translate("UiMainWindow", "Setting...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSetting->setShortcut(QApplication::translate("UiMainWindow", "Ctrl+Alt+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionNewProject->setText(QApplication::translate("UiMainWindow", "New Project...", nullptr));
        actionImportProject->setText(QApplication::translate("UiMainWindow", "Import Project...", nullptr));
        actionModalityDialog->setText(QApplication::translate("UiMainWindow", "\346\250\241\346\200\201\345\257\271\350\257\235\346\241\206", nullptr));
        actionModelessDialog->setText(QApplication::translate("UiMainWindow", "\351\235\236\346\250\241\346\200\201\345\257\271\350\257\235\346\241\206", nullptr));
        actionAboutDialog->setText(QApplication::translate("UiMainWindow", "\345\205\263\344\272\216\345\257\271\350\257\235\346\241\206", nullptr));
        actionConfirmDialog->setText(QApplication::translate("UiMainWindow", "\347\241\256\350\256\244\345\257\271\350\257\235\346\241\206", nullptr));
        actionFileDialog->setText(QApplication::translate("UiMainWindow", "\346\226\207\344\273\266\345\257\271\350\257\235\346\241\206", nullptr));
        actionExit->setText(QApplication::translate("UiMainWindow", "Exit", nullptr));
        pushButton->setText(QApplication::translate("UiMainWindow", "PushButton", nullptr));
        menuFile->setTitle(QApplication::translate("UiMainWindow", "File", nullptr));
        menuNew->setTitle(QApplication::translate("UiMainWindow", "New", nullptr));
        menuEdit->setTitle(QApplication::translate("UiMainWindow", "Edit", nullptr));
        menuView->setTitle(QApplication::translate("UiMainWindow", "View", nullptr));
        menuNavigate->setTitle(QApplication::translate("UiMainWindow", "Navigate", nullptr));
        menuCode->setTitle(QApplication::translate("UiMainWindow", "Code", nullptr));
        menuAnalyze->setTitle(QApplication::translate("UiMainWindow", "Analyze", nullptr));
        menuRefactor->setTitle(QApplication::translate("UiMainWindow", "Refactor", nullptr));
        menuBuild->setTitle(QApplication::translate("UiMainWindow", "Build", nullptr));
        menuRun->setTitle(QApplication::translate("UiMainWindow", "Run", nullptr));
        menuTools->setTitle(QApplication::translate("UiMainWindow", "Tools", nullptr));
        menuVcs->setTitle(QApplication::translate("UiMainWindow", "Vcs", nullptr));
        menuWindow->setTitle(QApplication::translate("UiMainWindow", "Window", nullptr));
        menuHelp->setTitle(QApplication::translate("UiMainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UiMainWindow: public Ui_UiMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIMAINWINDOW_H
