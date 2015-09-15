#include "goldenserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QFile>
#include <QtCore/QDebug>
#include <QTimer>
#include <QXmlStreamReader>
#include <QCoreApplication>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QStandardPaths>
#include <QDir>

QT_USE_NAMESPACE

GoldenServer::GoldenServer(quint16 port, bool debug, QString appDirPath, QObject *parent):
    QObject(parent),

    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("goldenserver"),
                                            QWebSocketServer::SecureMode, this)),
    m_clients(),
    m_debug(true)
{ 
    ReadConfigXML(appDirPath);

//    port = 8081;

    QString userHome = QDir::homePath();
    QSslConfiguration sslConfiguration;
    QFile certFile(userHome + "\\mycert.crt");
    QFile keyFile(userHome + "\\mykey.key");
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);

    if (m_pWebSocketServer->listen(QHostAddress::AnyIPv4, wsPort)) {
        if (m_debug)
            qDebug() << "Any IPv4 Address, listening on port" << wsPort;
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

void GoldenServer::ReadConfigXML(QString appDirPath)
{
    wsIP = "0.0.0.0";
    wsPort = 8080;

    QString fileName = appDirPath + "/../../../config.xml";

    QFile *xmlFile = new QFile(fileName);

    qDebug() << "File Exists - " << xmlFile->exists();

    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "CRITICAL ERROR - Couldn't open config.xml to load settings";
        return;
    }

    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
            // Read next element
            QXmlStreamReader::TokenType token = xmlReader->readNext();
            //If token is just StartDocument - go to next
            if(token == QXmlStreamReader::StartDocument) {
                    continue;
            }
            //If token is StartElement - read it
            if(token == QXmlStreamReader::StartElement) {

                    if(xmlReader->name() == "config") {
                            continue;
                    }

                    if(xmlReader->name() == "ws") {
                            continue;
                    }

                    if(xmlReader->name() == "ip") {
                        qDebug() << xmlReader->readElementText();
                    }

                    if(xmlReader->name() == "port") {
                        QString strPort = xmlReader->readElementText();
                        QTextStream ts(&strPort);
                        wsPort = 0;
                        ts >> wsPort;
                    }

            }
    }

    if(xmlReader->hasError()) {
        qDebug() << "PARSE ERROR - Could not parse config.xml to load settings";
        qDebug() << "PARSE ERROR - " << xmlReader->errorString();
    }

    //close reader and flush file
    xmlReader->clear();
    xmlFile->close();
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

