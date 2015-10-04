#include "goldenserver.h"
#include "configmanager.h"
#include "singleton.h"
#include "datastruct.h"
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
#include <QVariantList>
#include <QVariant>
#include <QJsonArray>

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

    connect(this, SIGNAL(dataChangedSignal(dataStruct)), &ConfigManagerInstance, SLOT(dataChangedSlot(dataStruct)));
    connect(this, SIGNAL(triggerUpdateSignal(dataStruct)), &ConfigManagerInstance, SLOT(triggerUpdateSlot(dataStruct)));
    connect(this, SIGNAL(interfaceStatusSignal(dataStruct)), &ConfigManagerInstance, SLOT(interfaceStatusSlot(dataStruct)));
    connect(&ConfigManagerInstance, SIGNAL(dataChangedSignal(dataStruct)), this, SLOT(dataChangedSlot(dataStruct)));
    connect(&ConfigManagerInstance, SIGNAL(triggerUpdateSignal(dataStruct)), this, SLOT(triggerUpdateSlot(dataStruct)));
    connect(&ConfigManagerInstance, SIGNAL(interfaceStatusSignal(dataStruct, bool)), this, SLOT(interfaceStatusSlot(dataStruct, bool)));
}

GoldenServer::~GoldenServer()
{
    m_pWebSocketServer->close();
    m_clients.clear();
    //Disconnects
    disconnect(this, SIGNAL(dataChangedSignal(dataStruct)), &ConfigManagerInstance, SLOT(dataChangedSlot(dataStruct)));
    disconnect(this, SIGNAL(triggerUpdateSignal(dataStruct)), &ConfigManagerInstance, SLOT(triggerUpdateSlot(dataStruct)));
    disconnect(this, SIGNAL(interfaceStatusSignal(dataStruct)), &ConfigManagerInstance, SLOT(interfaceStatusSlot(dataStruct)));
    disconnect(&ConfigManagerInstance, SIGNAL(dataChangedSignal(dataStruct)), this, SLOT(dataChangedSlot(dataStruct)));
    disconnect(&ConfigManagerInstance, SIGNAL(triggerUpdateSignal(dataStruct)), this, SLOT(triggerUpdateSlot(dataStruct)));
    disconnect(&ConfigManagerInstance, SIGNAL(interfaceStatusSignal(dataStruct, bool)), this, SLOT(interfaceStatusSlot(dataStruct, bool)));
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

void GoldenServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &GoldenServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &GoldenServer::socketDisconnected);

    sockStruct newSock;
    newSock.sock = pSocket;

    m_clients << newSock;

    qDebug() << "Connection Open";
}

QString GoldenServer::EncodeToWsProtocol_ReportScalar(sockProtocol dataToEncode)
{
    //encode to websocket protocol
    QJsonObject jsonObj; // define new JSON object
    jsonObj["title"] = dataToEncode.title;
    jsonObj["interfaceTag"] = dataToEncode.interfaceTag;
    jsonObj["notComputed"] = dataToEncode.notComputed;
    jsonObj["valueType"] = dataToEncode.valueType;
    //Check for type
    if (dataToEncode.valueType == "Boolean")
    {
        jsonObj["booleanVal"] = dataToEncode.booleanVal;
    }
    else if (dataToEncode.valueType == "Integer")
    {
        jsonObj["integerVal"] = dataToEncode.integerVal;
    }
    else if (dataToEncode.valueType == "Double")
    {
        jsonObj["doubleVal"] = dataToEncode.doubleVal;
    }
    else if (dataToEncode.valueType == "String")
    {
        jsonObj["stringVal"] = dataToEncode.stringVal;
    }
    //Add formatted value
    jsonObj["formattedValue"] = dataToEncode.formattedValue;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

QString GoldenServer::EncodeToWsProtocol_ReportArray(sockProtocol dataToEncode)
{
    //encode to websocket protocol
    QJsonObject jsonObj; // define new JSON object
    jsonObj["title"] = dataToEncode.title;
    jsonObj["interfaceTag"] = dataToEncode.interfaceTag;
    jsonObj["notComputed"] = dataToEncode.notComputed;
    jsonObj["valueType"] = dataToEncode.valueType;
    //Check for value type
    if (dataToEncode.valueType == "Boolean")
        jsonObj["booleanValues"] = QJsonArray::fromVariantList(ToVariantList(dataToEncode.booleanArray));
    else if (dataToEncode.valueType == "Integer")
        jsonObj["integerValues"] = QJsonArray::fromVariantList(ToVariantList(dataToEncode.integerArray));
    else if (dataToEncode.valueType == "Double")
        jsonObj["doubleValues"] = QJsonArray::fromVariantList(ToVariantList(dataToEncode.doubleArray));
    else if (dataToEncode.valueType == "String")
        jsonObj["stringValues"] = QJsonArray::fromVariantList(ToVariantList(dataToEncode.stringArray));
    //Add formatted type
    jsonObj["formattedValues"] = QJsonArray::fromVariantList(ToVariantList(dataToEncode.formattedArray));

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

template <typename T>
QVariantList GoldenServer::ToVariantList( const QList<T> &list )
{
    QVariantList newList;
    foreach( const T &item, list )
        newList << item;

    return newList;
}

QString GoldenServer::EncodeToWsProtocol_InterfaceStatus(sockProtocol dataToEncode)
{
    //encode to websocket protocol
    QJsonObject jsonObj; // define new JSON object
    jsonObj["title"] = dataToEncode.title;
    jsonObj["interfaceTag"] = dataToEncode.interfaceTag;
    jsonObj["disabledState"] = dataToEncode.disabledState;
    jsonObj["errorState"] = dataToEncode.errorState;
    jsonObj["disabledReason"] = dataToEncode.disabledReason;
    jsonObj["minimumVal"] = dataToEncode.minimumVal;
    jsonObj["maximumVal"] = dataToEncode.maximumVal;
    jsonObj["handshake"] = dataToEncode.handshake;
    jsonObj["menu"] = dataToEncode.menu;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

void GoldenServer::DecodeToWsProtocol_Base(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->title = jsonObj["title"].toString();
}

void GoldenServer::DecodeToWsProtocol_ScalarSubscription(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->title = jsonObj["title"].toString();
    destForData->interfaceTag = jsonObj["interfaceTag"].toString();
    destForData->actionType = jsonObj["actionType"].toString();
    destForData->parameter = jsonObj["parameter"].toString();
    destForData->formatType = jsonObj["formatType"].toString();
    destForData->formatString = jsonObj["formatString"].toString();
    destForData->unitSuffix = jsonObj["unitSuffix"].toBool();
    destForData->period = jsonObj["period"].toBool();
    destForData->gain = jsonObj["gain"].toInt();
    destForData->offset = jsonObj["offset"].toInt();
    destForData->quantity = jsonObj["quantity"].toString();
    destForData->period = jsonObj["period"].toInt();
    destForData->phase = jsonObj["phase"].toInt();
}

void GoldenServer::DecodeToWsProtocol_ScalarUnsubscription(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->title = jsonObj["title"].toString();
    destForData->interfaceTag = jsonObj["interfaceTag"].toString();
}

void GoldenServer::DecodeToWsProtocol_ArraySubscription(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->title = jsonObj["title"].toString();
    destForData->interfaceTag = jsonObj["interfaceTag"].toString();
    destForData->actionType = jsonObj["actionType"].toString();
    destForData->parameter = jsonObj["parameter"].toString();
    destForData->formatType = jsonObj["formatType"].toString();
    destForData->formatString = jsonObj["formatString"].toString();
    destForData->unitSuffix = jsonObj["unitSuffix"].toBool();
    destForData->period = jsonObj["period"].toBool();
    destForData->gain = jsonObj["gain"].toInt();
    destForData->offset = jsonObj["offset"].toInt();
    destForData->quantity = jsonObj["quantity"].toString();
    destForData->period = jsonObj["period"].toInt();
    destForData->phase = jsonObj["phase"].toInt();}

void GoldenServer::DecodeToWsProtocol_ArrayUnsubscription(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->title = jsonObj["title"].toString();
    destForData->interfaceTag = jsonObj["interfaceTag"].toString();
}

void GoldenServer::DecodeToWsProtocol_RequestScalar(sockProtocol *destForData, QString response)
{
    //encode to websocket protocol
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    destForData->title = jsonObj["title"].toString();
    destForData->interfaceTag = jsonObj["interfaceTag"].toString();
    destForData->valueType = jsonObj["valueType"].toString();
    destForData->booleanVal = jsonObj["parameter"].toBool();
    destForData->integerVal = jsonObj["parameter"].toInt();
    destForData->doubleVal = jsonObj["parameter"].toDouble();
    destForData->stringVal = jsonObj["parameter"].toString();
}

void GoldenServer::processTextMessage(QString message)
{    
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    sockProtocol data;
    sockProtocol *destForData = &data;    
    DecodeToWsProtocol_Base(destForData, message);
    if (destForData->title == "arraySubscription")
    {
        DecodeToWsProtocol_ArraySubscription(destForData, message);

        //Find entry in clients list
        int i = 0;
        while (i < m_clients.length())
        {
            if (m_clients[i].sock == pClient)
            {
                dataStruct myStruct(destForData->interfaceTag, m_clients[i].sock);
                emit interfaceStatusSignal(myStruct);
                break;
            }
            i++;
        }
    }
    else if (destForData->title == "arrayUnsubscription")
    {
        DecodeToWsProtocol_ArrayUnsubscription(destForData, message);

        //Find entry in clients list
        int i = 0;
        while (i < m_clients.length())
        {
            if (m_clients[i].sock == pClient)
            {
                mutex.lock();
                try
                {
                    int index = m_clients[i].subscriptions.indexOf(destForData->interfaceTag);
                    if (index != -1)
                    {
                        m_clients[i].subscriptions.removeAt(index);
                    }
                }
                catch(...)
                {
                    qDebug() << "Exception raised on unsubscribe";
                }
                mutex.unlock();
                break;
            }
            i++;
        }
    }
    else if (destForData->title == "scalarSubscription")
    {
        DecodeToWsProtocol_ScalarSubscription(destForData, message);

        //Find entry in clients list
        int i = 0;
        while (i < m_clients.length())
        {
            if (m_clients[i].sock == pClient)
            {
                dataStruct myStruct(destForData->interfaceTag, destForData->valueType, m_clients[i].sock);                
                emit interfaceStatusSignal(myStruct);
                break;
            }
            i++;
        }
    }
    else if (destForData->title == "scalarUnsubscription")
    {
        DecodeToWsProtocol_ScalarUnsubscription(destForData, message);

        //Find entry in clients list
        int i = 0;
        while (i < m_clients.length())
        {
            if (m_clients[i].sock == pClient)
            {
                mutex.lock();
                try
                {
                    int index = m_clients[i].subscriptions.indexOf(destForData->interfaceTag);
                    if (index != -1)
                    {
                        m_clients[i].subscriptions.removeAt(index);
                    }
                }
                catch(...)
                {
                    qDebug() << "Exception raised on unsubscribe";
                }
                mutex.unlock();
                break;
            }
            i++;
        }
    }
    else if (destForData->title == "requestScalar")
    {
        DecodeToWsProtocol_RequestScalar(destForData, message);

        int i = 0;
        while (i < m_clients.length())
        {
            try
            {
                if (m_clients[i].sock == pClient)
                {
                    //Check for subscription
                    int index = m_clients[i].subscriptions.indexOf(destForData->interfaceTag);
                    if (index != -1)
                    {
                        dataStruct myStruct = dataStruct(   destForData->interfaceTag,
                                                            destForData->valueType,
                                                            destForData->booleanVal,
                                                            destForData->integerVal,
                                                            destForData->doubleVal,
                                                            destForData->stringVal,
                                                            m_clients[i].sock);
                        emit dataChangedSignal(myStruct);
                        break;
                    }
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

    qDebug() << "socketDisconnected:" << pClient;
}

void GoldenServer::dataChangedSlot(dataStruct data)
{
    //Send to web sockets
    sockProtocol baseProtocol;

    if (data.getIsArray())
    {
        baseProtocol.title = "reportArray";
        baseProtocol.interfaceTag = data.gettag();
        baseProtocol.notComputed = data.getnotComputed();
        baseProtocol.valueType = data.getvalueType();
        baseProtocol.booleanArray = data.get_bool_arr_values();
        baseProtocol.integerArray = data.get_int_arr_values();
        baseProtocol.doubleArray = data.get_dbl_arr_values();
        baseProtocol.stringArray = data.get_str_arr_values();
        baseProtocol.formattedArray = data.get_ftd_arr_values();

        int i = 0;
        while (i < m_clients.length())
        {
            try
            {
                int index = m_clients[i].subscriptions.indexOf(baseProtocol.interfaceTag);
                if (index != -1)
                {
                    //Check for valid subscription
                    QString jsonstr = EncodeToWsProtocol_ReportArray(baseProtocol);
                    m_clients[i].sock->sendTextMessage(jsonstr);
                }
                i++;
            }
            catch(...)
            {
                qDebug() << "GoldenServer::update - Exception";
            }
        }
    }
    else
    {
        baseProtocol.title = "reportScalar";
        baseProtocol.notComputed = data.getnotComputed();
        baseProtocol.interfaceTag = data.gettag();
        baseProtocol.valueType = data.getvalueType();
        baseProtocol.booleanVal = data.getbln();
        baseProtocol.integerVal = data.getint();
        baseProtocol.doubleVal = data.getdbl();
        baseProtocol.stringVal = data.getstr();
        baseProtocol.formattedValue = data.getformattedValue();

        int i = 0;
        while (i < m_clients.length())
        {
            try
            {
                int index = m_clients[i].subscriptions.indexOf(baseProtocol.interfaceTag);
                if (index != -1)
                {
                    //Check for valid subscription
                    QString jsonstr = EncodeToWsProtocol_ReportScalar(baseProtocol);
                    m_clients[i].sock->sendTextMessage(jsonstr);
                }
                i++;
            }
            catch(...)
            {
                qDebug() << "GoldenServer::update - Exception";
            }
        }

    }
}

void GoldenServer::triggerUpdateSlot(dataStruct data)
{
    //Send to web sockets
    sockProtocol baseProtocol;

    if (data.getIsArray())
    {
        qDebug() << "Report Array";
        qDebug() << data.gettag();

        baseProtocol.title = "reportArray";
        baseProtocol.interfaceTag = data.gettag();
        baseProtocol.notComputed = data.getnotComputed();
        baseProtocol.valueType = data.getvalueType();
        baseProtocol.booleanArray = data.get_bool_arr_values();
        baseProtocol.integerArray = data.get_int_arr_values();
        baseProtocol.doubleArray = data.get_dbl_arr_values();
        baseProtocol.stringArray = data.get_str_arr_values();
        baseProtocol.formattedArray = data.get_ftd_arr_values();

        if (data.getsock() != NULL)
        {
            qDebug() << "Array Have Socket";

            QString jsonstr = EncodeToWsProtocol_ReportArray(baseProtocol);

            qDebug() << "REPORT ARRAY DATA: " + jsonstr;

            data.getsock()->sendTextMessage(jsonstr);
        }
    }
    else
    {
        baseProtocol.title = "reportScalar";
        baseProtocol.interfaceTag = data.gettag();
        baseProtocol.notComputed = data.getnotComputed();
        baseProtocol.valueType = data.getvalueType();
        baseProtocol.booleanVal = data.getbln();
        baseProtocol.integerVal = data.getint();
        baseProtocol.doubleVal = data.getdbl();
        baseProtocol.stringVal = data.getstr();
        baseProtocol.formattedValue = data.getformattedValue();

        if (data.getsock() != NULL)
        {
            QString jsonstr = EncodeToWsProtocol_ReportScalar(baseProtocol);
            data.getsock()->sendTextMessage(jsonstr);
        }
    }
}

void GoldenServer::interfaceStatusSlot(dataStruct data, bool triggerUpdate)
{
    //Send to web sockets
    sockProtocol baseProtocol;
    baseProtocol.title = "interfaceStatus";
    baseProtocol.interfaceTag = data.gettag();
    baseProtocol.disabledState = data.getdisabledState();
    baseProtocol.errorState = data.geterrorState();
    baseProtocol.disabledReason = data.getdisabledReason();
    baseProtocol.errorReason = data.geterrorReason();
    baseProtocol.minimumVal = data.getminimumValue();
    baseProtocol.maximumVal = data.getmaximumValue();
    baseProtocol.handshake = data.gethandshake();
    baseProtocol.menu = data.getmenu();

    if (data.getsock() != NULL)
    {
        QString jsonstr = EncodeToWsProtocol_InterfaceStatus(baseProtocol);
        data.getsock()->sendTextMessage(jsonstr);
        if (data.gethandshake() == "HostComplete")
        {
            //Do subscription only when the item has been confirmed to exist
            for (int i = 0; i < m_clients.length(); i++)
            {
                if (m_clients[i].sock == data.getsock())
                {
                    m_clients[i].subscriptions << data.gettag();
                    break;
                }
            }
            if (triggerUpdate)
                emit triggerUpdateSignal(data);
        }
    }
}
