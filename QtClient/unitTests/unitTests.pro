#-------------------------------------------------
#
# Project created by QtCreator 2015-08-05T12:17:15
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_unitteststest
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_unitteststest.cpp \
    ../ContextProperty/mytimer.cpp \
    ../DemoLib/DemoLib/cppxbutton.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../ContextProperty/mytimer.h \
    ../DemoLib/DemoLib/cppxbutton.h
