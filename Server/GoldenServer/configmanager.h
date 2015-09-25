#ifndef CONFIGMANAGER
#define CONFIGMANAGER

#include <QObject>
#include <QWebSocket>

class dataStruct
{
public:
    dataStruct();

    dataStruct( QString m_tag );
    dataStruct( QString m_tag, QString m_valueType );
    dataStruct( QString m_tag, QString m_valueType, QWebSocket *m_sock );
    dataStruct( QString m_tag, QString m_valueType, bool m_bool, int m_int, double m_dbl, QString m_str);
    dataStruct( QString m_tag, QString m_valueType, bool m_bool, int m_int, double m_dbl, QString m_str, QWebSocket *m_sock);

    QString gettag();
    QString getstr();
    double getdbl();
    int getint();
    bool getbln();
    bool getdisabledState();
    bool geterrorState();
    QString getdisabledReason();
    QString geterrorReason();
    int getminimumValue();
    int getmaximumValue();
    QString gethandshake();
    QString getmenu();
    bool getnotComputed();
    QString getformattedValue();
    QString getvalueType();
    QWebSocket *getsock();

    void setsock(QWebSocket *qws);
    void sethandshake(QString m_handshake);
    void setbln(bool m_bln);
    void setstr(QString m_str);
    void setint(int m_int);
    void setdbl(double m_dbl);
    void setnotComputed(bool m_notComputed);
    void setformattedValue(bool m_formattedValue);
    void setvalueType(QString m_valueType);

private:
    void Initialise();

    QString tag;
    QString strVal;
    double dblVal;
    int intVal;
    bool blnVal;
    bool disabledState;
    bool errorState;
    QString disabledReason;
    QString errorReason;
    int minimumValue;
    int maximumValue;
    QString handshake;
    QString menu;
    bool notComputed;
    QString formattedValue;
    QString valueType;
    QWebSocket *sock;
};

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    ConfigManager();
    void bindMe();
    void unbindMe();

public slots:

    void triggerUpdateSlot(dataStruct data);
    void dataChangedSlot(dataStruct data);
    void interfaceStatusSlot(dataStruct data);

signals:

    void triggerUpdateSignal(dataStruct data);
    void dataChangedSignal(dataStruct data);
    void interfaceStatusSignal(dataStruct data);

private:
    QList<dataStruct> GiData;
};


#endif // CONFIGMANAGER

