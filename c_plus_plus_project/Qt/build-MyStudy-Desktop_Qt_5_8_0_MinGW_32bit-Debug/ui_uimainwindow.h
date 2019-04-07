/********************************************************************************
** Form generated from reading UI file 'uimainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIMAINWINDOW_H
#define UI_UIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
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
            UiMainWindow->setObjectName(QStringLiteral("UiMainWindow"));
        UiMainWindow->resize(1241, 698);
        actionOpen = new QAction(UiMainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon(QIcon::fromTheme(QStringLiteral(":/image/v001_24.png")));
        actionOpen->setIcon(icon);
        actionSetting = new QAction(UiMainWindow);
        actionSetting->setObjectName(QStringLiteral("actionSetting"));
        QIcon icon1(QIcon::fromTheme(QStringLiteral(":/image/v016_24.png")));
        actionSetting->setIcon(icon1);
        QFont font;
        font.setUnderline(false);
        actionSetting->setFont(font);
        actionNewProject = new QAction(UiMainWindow);
        actionNewProject->setObjectName(QStringLiteral("actionNewProject"));
        actionImportProject = new QAction(UiMainWindow);
        actionImportProject->setObjectName(QStringLiteral("actionImportProject"));
        actionModalityDialog = new QAction(UiMainWindow);
        actionModalityDialog->setObjectName(QStringLiteral("actionModalityDialog"));
        actionModelessDialog = new QAction(UiMainWindow);
        actionModelessDialog->setObjectName(QStringLiteral("actionModelessDialog"));
        actionAboutDialog = new QAction(UiMainWindow);
        actionAboutDialog->setObjectName(QStringLiteral("actionAboutDialog"));
        actionConfirmDialog = new QAction(UiMainWindow);
        actionConfirmDialog->setObjectName(QStringLiteral("actionConfirmDialog"));
        actionFileDialog = new QAction(UiMainWindow);
        actionFileDialog->setObjectName(QStringLiteral("actionFileDialog"));
        actionExit = new QAction(UiMainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralwidget = new QWidget(UiMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        UiMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(UiMainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1241, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFile->setMinimumSize(QSize(250, 0));
        menuNew = new QMenu(menuFile);
        menuNew->setObjectName(QStringLiteral("menuNew"));
        menuNew->setMinimumSize(QSize(250, 0));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuNavigate = new QMenu(menubar);
        menuNavigate->setObjectName(QStringLiteral("menuNavigate"));
        menuCode = new QMenu(menubar);
        menuCode->setObjectName(QStringLiteral("menuCode"));
        menuAnalyze = new QMenu(menubar);
        menuAnalyze->setObjectName(QStringLiteral("menuAnalyze"));
        menuRefactor = new QMenu(menubar);
        menuRefactor->setObjectName(QStringLiteral("menuRefactor"));
        menuBuild = new QMenu(menubar);
        menuBuild->setObjectName(QStringLiteral("menuBuild"));
        menuRun = new QMenu(menubar);
        menuRun->setObjectName(QStringLiteral("menuRun"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        menuVcs = new QMenu(menubar);
        menuVcs->setObjectName(QStringLiteral("menuVcs"));
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QStringLiteral("menuWindow"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        UiMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(UiMainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
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

        QMetaObject::connectSlotsByName(UiMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *UiMainWindow)
    {
        UiMainWindow->setWindowTitle(QApplication::translate("UiMainWindow", "MainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("UiMainWindow", "Open...", Q_NULLPTR));
        actionSetting->setText(QApplication::translate("UiMainWindow", "Setting...", Q_NULLPTR));
        actionSetting->setShortcut(QApplication::translate("UiMainWindow", "Ctrl+Alt+S", Q_NULLPTR));
        actionNewProject->setText(QApplication::translate("UiMainWindow", "New Project...", Q_NULLPTR));
        actionImportProject->setText(QApplication::translate("UiMainWindow", "Import Project...", Q_NULLPTR));
        actionModalityDialog->setText(QApplication::translate("UiMainWindow", "\346\250\241\346\200\201\345\257\271\350\257\235\346\241\206", Q_NULLPTR));
        actionModelessDialog->setText(QApplication::translate("UiMainWindow", "\351\235\236\346\250\241\346\200\201\345\257\271\350\257\235\346\241\206", Q_NULLPTR));
        actionAboutDialog->setText(QApplication::translate("UiMainWindow", "\345\205\263\344\272\216\345\257\271\350\257\235\346\241\206", Q_NULLPTR));
        actionConfirmDialog->setText(QApplication::translate("UiMainWindow", "\347\241\256\350\256\244\345\257\271\350\257\235\346\241\206", Q_NULLPTR));
        actionFileDialog->setText(QApplication::translate("UiMainWindow", "\346\226\207\344\273\266\345\257\271\350\257\235\346\241\206", Q_NULLPTR));
        actionExit->setText(QApplication::translate("UiMainWindow", "Exit", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("UiMainWindow", "File", Q_NULLPTR));
        menuNew->setTitle(QApplication::translate("UiMainWindow", "New", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("UiMainWindow", "Edit", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("UiMainWindow", "View", Q_NULLPTR));
        menuNavigate->setTitle(QApplication::translate("UiMainWindow", "Navigate", Q_NULLPTR));
        menuCode->setTitle(QApplication::translate("UiMainWindow", "Code", Q_NULLPTR));
        menuAnalyze->setTitle(QApplication::translate("UiMainWindow", "Analyze", Q_NULLPTR));
        menuRefactor->setTitle(QApplication::translate("UiMainWindow", "Refactor", Q_NULLPTR));
        menuBuild->setTitle(QApplication::translate("UiMainWindow", "Build", Q_NULLPTR));
        menuRun->setTitle(QApplication::translate("UiMainWindow", "Run", Q_NULLPTR));
        menuTools->setTitle(QApplication::translate("UiMainWindow", "Tools", Q_NULLPTR));
        menuVcs->setTitle(QApplication::translate("UiMainWindow", "Vcs", Q_NULLPTR));
        menuWindow->setTitle(QApplication::translate("UiMainWindow", "Window", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("UiMainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class UiMainWindow: public Ui_UiMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIMAINWINDOW_H
