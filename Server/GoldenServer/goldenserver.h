#ifndef GOLDENSERVER_H
#define GOLDENSERVER_H

#include "configmanager.h"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QCoreApplication>
#include <QWebSocket>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QTimer)

class GoldenServer : public QObject
{
    Q_OBJECT
public:
    explicit GoldenServer(quint16 port, bool debug = false, QString appDirPath = "", QObject *parent = Q_NULLPTR);
    ~GoldenServer();

Q_SIGNALS:
    void closed();
    void timeout();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void update();

private:
    struct sockStruct
    {
        QWebSocket *sock; // 2 bytes
        QList<QString> subscriptions;  // 1 byte
    };
    QList<sockStruct> m_clients;
    struct sockProtocol
    {
        QString taskId;
        QString smString;
        QString type;
        QString value;
        QString status;
    };
    QString wsIP;
    quint16 wsPort;
    QWebSocketServer *m_pWebSocketServer;
    bool m_debug;
    QTimer *timer;
    QString sdt;
    void ReadConfigXML(QString appDirPath);
    QString EncodeToWsProtocol(sockProtocol dataToEncode);
    void DecodeToWsProtocol(sockProtocol *destForData, QString response);
    void sendData(QWebSocket *soc, sockProtocol dataToEncode);
    ConfigManager cMan;
};

#endif // GOLDENSERVER_H
