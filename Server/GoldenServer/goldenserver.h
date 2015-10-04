#ifndef GOLDENSERVER_H
#define GOLDENSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QCoreApplication>
#include <QWebSocket>

#include "configmanager.h"
#include "datastruct.h"

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

    void triggerUpdateSlot(dataStruct data);
    void dataChangedSlot(dataStruct data);
    void interfaceStatusSlot(dataStruct data, bool triggerUpdate);

signals:
    void closed();
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
        QString parameter;
        QString formatString;
        QString unitSuffix;
        int gain;
        int offset;
        QString quantity;
        int period;
        int phase;
        QString valueType;
        bool booleanVal;
        int integerVal;
        double doubleVal;
        QString stringVal;
        QList<bool> booleanArray;
        QList<int> integerArray;
        QList<double> doubleArray;
        QList<QString> stringArray;
        QList<QString> formattedArray;
        QString notComputed;
        QString formatType;
        QString formattedValue;
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
    QString EncodeToWsProtocol_ReportArray(sockProtocol dataToEncode);

    void DecodeToWsProtocol_Base(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_ScalarSubscription(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_ScalarUnsubscription(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_ArraySubscription(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_ArrayUnsubscription(sockProtocol *destForData, QString response);
    void DecodeToWsProtocol_RequestScalar(sockProtocol *destForData, QString response);

    template <typename T>
    QVariantList ToVariantList( const QList<T> &list );

    QMutex mutex;
};

#endif // GOLDENSERVER_H
