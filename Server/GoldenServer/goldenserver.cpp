#include "goldenserver.h"
#include "configmanager.h"
#include "singleton.h"
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
#include <QJsonObject>
#include <QJsonDocument>

#define ConfigManagerInstance Singleton<ConfigManager>::instance()

QT_USE_NAMESPACE

GoldenServer::GoldenServer(quint16 port, bool debug, QString appDirPath, QObject *parent):
    QObject(parent),

    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("goldenserver"),
                                            QWebSocketServer::NonSecureMode, this)),
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
//    qDeleteAll(m_clients.begin(), m_clients.end());
    m_clients.clear();
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
    sdt = dt.toString(Qt::TextDate);
    int index = 0;
    while(index < m_clients.length())
    {
        try
        {
            // send out general date time message
            sockProtocol newDataOut;
            newDataOut.smString = "strSmDateTime";
            newDataOut.taskId = "GiData-" + newDataOut.smString;
            newDataOut.type = "string";
            newDataOut.status = "GiIdle";
            newDataOut.value = sdt;
            sendData(m_clients[index].sock, newDataOut);
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
    connect(pSocket, &QWebSocket::disconnected, this, &GoldenServer::socketDisconnected);

    sockStruct newSock;
    newSock.sock = pSocket;

    m_clients << newSock;
}

QString GoldenServer::EncodeToWsProtocol(sockProtocol dataToEncode)
{
    //encode to websocket protocol
    QJsonObject jsonObj; // define new JSON object
    jsonObj["taskId"] = dataToEncode.taskId;
    jsonObj["smString"] = dataToEncode.smString;
    jsonObj["type"] = dataToEncode.type;
    jsonObj["value"] = dataToEncode.value;
    jsonObj["status"] = dataToEncode.status;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

void GoldenServer::DecodeToWsProtocol(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->taskId = jsonObj["taskId"].toString();
    destForData->smString = jsonObj["smString"].toString();
    destForData->type = jsonObj["type"].toString();
    destForData->value = jsonObj["value"].toString();
    destForData->status = jsonObj["status"].toString();
}

void GoldenServer::sendData(QWebSocket *soc, sockProtocol dataToEncode)
{
    QString strToSend = EncodeToWsProtocol(dataToEncode);

    //send to specific socket only
    soc->sendTextMessage(strToSend);
}

void GoldenServer::processTextMessage(QString message)
{    
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    sockProtocol data;
    sockProtocol *destForData = &data;    
    DecodeToWsProtocol(destForData, message);
    //Send confirm straignt away
    QString taskId = destForData->taskId;
    destForData->taskId = destForData->taskId + "-" + destForData->smString;
    destForData->status = "DsAck";
    sendData(pClient, *destForData);
    //Process the message
    if (taskId == "generalRqt")
    {
        //Do something
    }
    else if (taskId == "subscribe")
    {
        //Find entry in clients list
        int i = 0;
        while (i < m_clients.length())
        {
            if (m_clients[i].sock == pClient)
            {
                ConfigManagerInstance.bindMe();
                m_clients[i].subscriptions << destForData->smString;
                break;
            }
            i++;
        }
        destForData->status = "GiAck";
        sendData(pClient, *destForData);
        destForData->taskId = "GiData-" + destForData->smString;
        destForData->value = destForData->smString + " - Click Me";
        destForData->status = "GiIdle";
        sendData(pClient, *destForData);
    }
    else if (taskId == "unsubscribe")
    {
        //Find entry in clients list
        int i = 0;
        while (i < m_clients.length())
        {
            if (m_clients[i].sock == pClient)
            {
                int j = m_clients[i].subscriptions.indexOf(destForData->smString);
                if (j != -1)
                {
                    ConfigManagerInstance.unbindMe();
                    m_clients[i].subscriptions.removeAt(j);
                }
                break;
            }
            i++;
        }
    }
    else if (taskId == "setData")
    {
        int i = 0;
        QString storedValue = destForData->value;
        while (i < m_clients.length())
        {
            try
            {
                //Check for valid subscription
                int j = m_clients[i].subscriptions.indexOf(destForData->smString);
                if (j != -1)
                {
                    //send data back
                    destForData->value = destForData->smString + " - " + storedValue;
                    destForData->status = "GiDataAck";
                    sendData(m_clients[i].sock, *destForData);
                }
                i++;
            }
            catch(...)
            {
                qDebug() << "GoldenServer::update - Exception";
            }
        }
    }
    qDebug() << "Message received:" << message;
}

void GoldenServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;

    if (pClient) {
        int i = 0;
        while(i < m_clients.count()) {
          if(m_clients.at(i).sock == pClient) {
            m_clients.removeAt(i);
          }
          else
          {
              i++;
          }
        }
        pClient->deleteLater();
    }
}

