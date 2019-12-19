#-------------------------------------------------
#
# Project created by QtCreator 2019-12-17T20:01:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = macaoTalkClient
TEMPLATE = app

QT += widgets
QT += core

SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    createchat.cpp

HEADERS  += mainwindow.h \
    login.h \
    createchat.h

FORMS    += mainwindow.ui \
    login.ui \
    createchat.ui

DEFINS += QT_BISABLE_DEPRECATED_BEFORE=0x000000
