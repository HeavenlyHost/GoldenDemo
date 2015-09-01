#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "goldenserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("GoldenDemo Server");

    QCommandLineParser parser;
    parser.setApplicationDescription("GoldenDemo: server");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
        QCoreApplication::translate("main", "Debug output [default: on]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
        QCoreApplication::translate("main", "Port for goldenserver [default: 8080]."),
        QCoreApplication::translate("main", "port"), QLatin1Literal("8080"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    GoldenServer *server = new GoldenServer(port, debug);
    QObject::connect(server, &GoldenServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
