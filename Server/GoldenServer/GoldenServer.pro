#-------------------------------------------------
#
# Project created by QtCreator 2015-08-31T20:22:49
#
#-------------------------------------------------

QT       += core websockets network

QT       -= gui

TARGET = GoldenServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    goldenserver.cpp \
    configmanager.cpp

HEADERS += \
    goldenserver.h \
    configmanager.h \
    call_once.h \
    singleton.h
