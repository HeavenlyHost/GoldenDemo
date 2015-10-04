#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QWebSocket>

class dataStruct
{
public:
    dataStruct();
    dataStruct( QString m_tag );
    dataStruct( QString m_tag, QString m_valueType);
    dataStruct( QString m_tag, QWebSocket *m_sock );
    dataStruct( QString m_tag, QString m_valueType, bool is_Array );
    dataStruct( QString m_tag, QString m_valueType, QWebSocket *m_sock );
    dataStruct( QString m_tag, QString m_valueType, bool is_Array, QWebSocket *m_sock );
    dataStruct( QString m_tag, QString m_valueType, bool m_bool, int m_int, double m_dbl, QString m_str);
    dataStruct( QString m_tag, QString m_valueType, bool m_bool, int m_int, double m_dbl, QString m_str, QWebSocket *m_sock);

    QString gettag();
    bool getIsArray();
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
    QList<QString> get_str_arr_values();
    QList<bool> get_bool_arr_values();
    QList<double> get_dbl_arr_values();
    QList<int> get_int_arr_values();
    QList<QString> get_ftd_arr_values();

    void setIsArray(bool m_isArray);
    void setsock(QWebSocket *qws);
    void sethandshake(QString m_handshake);
    void setbln(bool m_bln);
    void setstr(QString m_str);
    void setint(int m_int);
    void setdbl(double m_dbl);
    void setnotComputed(bool m_notComputed);
    void setformattedValue(QString m_formattedValue);
    void setvalueType(QString m_valueType);
    void set_str_arr_values(QList<QString> m_strArray);
    void set_bool_arr_values(QList<bool> m_boolArray);
    void set_dbl_arr_values(QList<double> m_dblArray);
    void set_int_arr_values(QList<int> m_intArray);
    void set_ftd_arr_values(QList<QString> m_fdtArray);

private:
    void Initialise();

    QString tag;
    bool isArray;
    QList<QString> str_arr_values;
    QList<bool> bool_arr_values;
    QList<double> dbl_arr_values;
    QList<int> int_arr_values;
    QList<QString> ftd_arr_values;
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

#endif // DATASTRUCT_H
