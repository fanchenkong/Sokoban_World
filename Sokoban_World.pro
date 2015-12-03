#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T10:38:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sokoban_World
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp

HEADERS  += mainwindow.h \
    board.h

FORMS    += mainwindow.ui \
    board.ui

CONFIG += c++11

RESOURCES += \
    images.qrc
