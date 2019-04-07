# 添加模块
QT += core gui
# 高于4版本时,添加widgets模块.为了兼容QT4
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# QT5基本模块有:
# webkit test sql network mulitmedia (mulitmedia widgets)
# quick qml (quick controls) (quick dialogs)这4个是另外一个框架的东西

# 在c++11中能够使用Lambda表达式
CONFIG += c++11
#弹出黑窗口显示输出内容
#CONFIG += console
#不弹出黑窗口,使用QT控制台输出内容
CONFIG -= console
CONFIG -= app_bundle

# 应用程序名称
TARGET = MyStudy
# 指定makefile的类型
TEMPLATE = app

# 头文件
HEADERS += \
    firstwidget.h \
    myheader.h \
    twinswidgetboss.h \
    twinswidgetpenis.h \
    twinswidgetmother.h \
    mainwindow.h

# 源文件
SOURCES += \
    main.cpp \
    firstwidget.cpp \
    twinswidgetboss.cpp \
    twinswidgetpenis.cpp \
    mainwindow.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
# disables all the APIs deprecated before Qt 6.0.0

DISTFILES += \
    readme.txt

#FORMS    += dialog.ui
