#-------------------------------------------------
#
# Project created by QtCreator 2011-12-05T13:41:55
#
#-------------------------------------------------

QT       += core gui

TARGET = QtClient
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    networkmanager.cpp

HEADERS  += mainwindow.h \
    networkmanager.h \
    messages.h \
    keys.h

FORMS    += mainwindow.ui

LIBS += -L../enet/enet-1.3.3/.libs/ -lenet
INCLUDEPATH += ../enet/enet-1.3.3/include/
