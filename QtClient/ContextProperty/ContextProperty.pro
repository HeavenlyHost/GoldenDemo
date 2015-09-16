TEMPLATE = app

QT += qml quick widgets testlib

SOURCES += main.cpp \
    mytimer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    XControl.qml

HEADERS += \
    mytimer.h

CONFIG += testcase

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../DemoLib/build-DemoLib-Desktop_VC_12_x86-Debug/release/ -lDemoLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../DemoLib/build-DemoLib-Desktop_VC_12_x86-Debug/debug/ -lDemoLib
else:unix: LIBS += -L$$PWD/../DemoLib/build-DemoLib-Desktop_VC_12_x86-Debug/ -lDemoLib

INCLUDEPATH += $$PWD/../DemoLib/DemoLib
DEPENDPATH += $$PWD/../DemoLib/DemoLib
