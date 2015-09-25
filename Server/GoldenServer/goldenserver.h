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

public slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void update();

    void triggerUpdateSlot(dataStruct data);
    void dataChangedSlot(dataStruct data);
    void interfaceStatusSlot(dataStruct data);

signals:
    void closed();
    void timeout();
    void triggerUpdateSignal(dataStruct data);
    void dataChangedSignal(dataStruct data);
    void interfaceStatusSignal(dataStruct data);

private:
    struct sockStruct
    {
        QWebSocket *sock; // 2 bytes
        QList<QString> subscriptions;  // 1 byte
    };

    QList<sockStruct> m_clients;
    struct sockProtocol
    {
        QString title;
        QString interfaceTag;
        QString actionType;
        QString Parameter;
        QString FormatString;
        QString UnitSuffix;
        int Gain;
        int Offset;
        QString Quantity;
        int Period;
        int Phase;
        QString valueType;
        bool Boolean;
        int Integer;
        double Double;
        QString String;
        QString notComputed;
        QString FormatType;
        QString FormattedValue;
        bool disabledState;
        bool errorState;
        QString disabledReason;
        QString errorReason;
        int minimumVal;
        int maximumVal;
        QString handshake;
        QString menu;
        QString description;
    };
    QString wsIP;
    quint16 wsPort;
    QWebSocketServer *m_pWebSocketServer;
    bool m_debug;
    QTimer *timer;
    QString sdt;
    void ReadConfigXML(QString appDirPath);

    QString EncodeToWsProtocol_ReportScalar(sockProtocol dataToEncode);
    QString EncodeToWsProtocol_InterfaceStatus(sockProtocol dataToEncode);

    void DecodeToWsProtocol_Base(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_ScalarSubscription(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_RequestScalar(sockProtocol *destForData, QString response);

    QMutex mutex;
};

#endif // GOLDENSERVER_H
