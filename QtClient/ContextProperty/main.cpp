#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>
#include <QTimer>
#include <QQmlEngine>
#include <QtQml>

#include "mytimer.h"
#include "xcppxbutton.h"

#include "singleton.h"
#include "websockservice.h"
#include "call_once.h"

#define MyClassInstance Singleton<WebSockService>::instance()

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MyTimer>("com.thales.timers", 1 , 0, "MyTimer");
    qmlRegisterType<CppXButton>("com.thales.cppxbutton", 1 , 0, "CppXButton");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //MyTimer *mTimer = new MyTimer();
    //engine.rootContext()->setContextProperty("myTime", mTimer);

    return app.exec();
}
