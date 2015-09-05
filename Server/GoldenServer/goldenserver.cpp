#include "goldenserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QTimer>

QT_USE_NAMESPACE

GoldenServer::GoldenServer(quint16 port, bool debug, QObject *parent):
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("goldenserver"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(true)
{ 
    port = 8081;

    if (m_pWebSocketServer->listen(QHostAddress::AnyIPv4, port)) {
        if (m_debug)
            qDebug() << "goldenserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &GoldenServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &GoldenServer::closed);
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(50);
}

GoldenServer::~GoldenServer()
{
    timer->stop();
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void GoldenServer::update()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString sdt = dt.toString(Qt::TextDate);
    int index = 0;
    while(index < m_clients.length())
    {
        try
        {
            qDebug() << "message sent: " + sdt + ", index = " + index;
            m_clients[index]->sendTextMessage(sdt);
            index++;
        }
        catch(...)
        {
            qDebug() << "GoldenServer::update - Exception";
        }
    }
}

void GoldenServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    qDebug() << "Connection Open";

    connect(pSocket, &QWebSocket::textMessageReceived, this, &GoldenServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &GoldenServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &GoldenServer::socketDisconnected);

    m_clients << pSocket;
}

void GoldenServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Message received:" << message;
    if (pClient) {
        pClient->sendTextMessage(message);
    }
}

void GoldenServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void GoldenServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

