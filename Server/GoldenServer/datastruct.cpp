#include "datastruct.h"
#include <QtCore/QDebug>

dataStruct::dataStruct()
{
    Initialise();
}

dataStruct::dataStruct( QString m_tag )
{
    Initialise();

    tag = m_tag;
}

dataStruct::dataStruct( QString m_tag, QWebSocket *m_sock )
{
    Initialise();

    tag = m_tag;
    sock = m_sock;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType )
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType, bool is_Array )
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
    isArray = is_Array;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType, QWebSocket *m_sock )
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
    sock = m_sock;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType, bool is_Array, QWebSocket *m_sock )
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
    isArray = is_Array;
    sock = m_sock;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType, bool m_bool, int m_int, double m_dbl, QString m_str)
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
    blnVal = m_bool;
    intVal = m_int;
    dblVal = m_dbl;
    strVal = m_str;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType, bool m_bool, int m_int, double m_dbl, QString m_str, QWebSocket *m_sock)
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
    blnVal = m_bool;
    intVal = m_int;
    dblVal = m_dbl;
    strVal = m_str;
    sock = m_sock;
}

void dataStruct::Initialise()
{
    tag = "";
    isArray = false;
    str_arr_values << "Bob" << "Fred" << "Joe" << "Bill" << "Toad" << "Mole" << "Jess" << "Anna" << "Kim" << "Max" << "Bobby" << "Freddy" << "Joey" << "Billy" << "Toady" << "Moley" << "Jessy" << "Annay" << "Kimy" << "Maxy";
    bool_arr_values << false << true << false << true << true << true << false << true << false << true << false << true << false << true << true << true << false << true << false << true;
    dbl_arr_values << 0.1 << 0.2 << 0.3 << 0.4 << 0.5 << 0.6 << 0.7 << 0.8 << 0.9 << 1.0 << 1.1 << 1.2 << 1.3 << 1.4 << 1.5 << 1.6 << 1.7 << 1.8 << 1.9 << 2.0;
    int_arr_values << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20;
    strVal = "Intialised !!!";
    dblVal = 0;
    intVal = 0;
    blnVal = false;
    disabledState = false;
    errorState = false;
    disabledReason = "";
    errorReason = "";
    minimumValue = 0;
    maximumValue = 0;
    handshake = "None";
    menu = "";
    notComputed = false;
    formattedValue = "";
    valueType = "";
    sock = NULL;
}

QString dataStruct::gettag(){return tag;}
bool dataStruct::getIsArray(){return isArray;}
QString dataStruct::getvalueType(){return valueType;}
QString dataStruct::getstr(){return strVal;}
double dataStruct::getdbl(){return dblVal;}
int dataStruct::getint(){return intVal;}
bool dataStruct::getbln(){return blnVal;}
bool dataStruct::getdisabledState(){return disabledState;}
bool dataStruct::geterrorState(){return errorState;}
QString dataStruct::getdisabledReason(){return disabledReason;}
QString dataStruct::geterrorReason(){return errorReason;}
int dataStruct::getminimumValue(){return minimumValue;}
int dataStruct::getmaximumValue(){return maximumValue;}
QString dataStruct::gethandshake(){return handshake;}
QString dataStruct::getmenu(){return menu;}
bool dataStruct::getnotComputed(){return notComputed;}
QString dataStruct::getformattedValue(){return formattedValue;}
QWebSocket *dataStruct::getsock(){return sock;}
QList<QString> dataStruct::get_str_arr_values(){return str_arr_values;}
QList<bool> dataStruct::get_bool_arr_values(){return bool_arr_values;}
QList<double> dataStruct::get_dbl_arr_values(){return dbl_arr_values;}
QList<int> dataStruct::get_int_arr_values(){return int_arr_values;}
QList<QString> dataStruct::get_ftd_arr_values(){return ftd_arr_values;}

void dataStruct::setIsArray(bool m_isArray){isArray = m_isArray;}
void dataStruct::setvalueType(QString m_valueType){valueType = m_valueType;}
void dataStruct::setsock(QWebSocket *qws){sock = qws;}
void dataStruct::sethandshake(QString m_handshake){handshake = m_handshake;}
void dataStruct::setbln(bool m_bln){blnVal = m_bln;}
void dataStruct::setstr(QString m_str){strVal = m_str;}
void dataStruct::setint(int m_int){intVal = m_int;}
void dataStruct::setdbl(double m_dbl){dblVal = m_dbl;}
void dataStruct::setdisabledState(bool m_disabledState){disabledState = m_disabledState;}
void dataStruct::setErrorState(bool m_errorState){errorState = m_errorState;}
void dataStruct::setDisabledReason(QString m_disabledReason){disabledReason = m_disabledReason;}
void dataStruct::setErrorReason(QString m_errorReason){errorReason = m_errorReason;}

void dataStruct::setnotComputed(bool m_notComputed){notComputed = m_notComputed;}
void dataStruct::setformattedValue(QString m_formattedValue){formattedValue = m_formattedValue;}
void dataStruct::set_str_arr_values(QList<QString> m_strArray){str_arr_values = m_strArray;}
void dataStruct::set_bool_arr_values(QList<bool> m_boolArray){bool_arr_values = m_boolArray;}
void dataStruct::set_dbl_arr_values(QList<double> m_dblArray){dbl_arr_values = m_dblArray;}
void dataStruct::set_int_arr_values(QList<int> m_intArray){int_arr_values = m_intArray;}
void dataStruct::set_ftd_arr_values(QList<QString> m_ftdArray){ftd_arr_values = m_ftdArray;}
