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

LIBS += -L./lib -lenet -lSDL
INCLUDEPATH += ./include
