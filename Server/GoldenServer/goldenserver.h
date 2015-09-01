#ifndef GOLDENSERVER_H
#define GOLDENSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class GoldenServer : public QObject
{
    Q_OBJECT
public:
    explicit GoldenServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~GoldenServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
};

#endif // GOLDENSERVER_H
