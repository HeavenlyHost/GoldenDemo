#-------------------------------------------------
#
# Project created by QtCreator 2015-08-06T10:50:07
#
#-------------------------------------------------

QT       -= gui

TARGET = DemoLib
TEMPLATE = lib

DEFINES += DEMOLIB_LIBRARY

SOURCES += demolib.cpp \
    cppxbutton.cpp

HEADERS += demolib.h\
        demolib_global.h \
    cppxbutton.h \
    websockservice.h \
    singleton.h \
    call_once.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
