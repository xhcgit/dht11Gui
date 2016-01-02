#-------------------------------------------------
#
# Project created by QtCreator 2015-11-28T23:04:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dht11Gui
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Average.cpp \
    Change.cpp \
    Limit.cpp \
    MyBeep.cpp

HEADERS  += MainWindow.h \
    Average.h \
    Change.h \
    Limit.h \
    MyBeep.h \
    SystemControl.h

FORMS    += MainWindow.ui \
    Average.ui \
    Change.ui \
    Limit.ui \
    MyBeep.ui
