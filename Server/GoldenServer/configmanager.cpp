#include "configmanager.h"
#include <QtCore/QDebug>
#include <QTimer>
#include <QThread>

ConfigManager::ConfigManager()
{
    //Intialise strings
    GiData << dataStruct("strServerDateTime", "String", false,0,0,"system date/time");
    GiData << dataStruct("strItem1", "String");
    GiData << dataStruct("strItem2", "String");
    GiData << dataStruct("strItem3", "String");
    GiData << dataStruct("strItem4", "String");
    GiData << dataStruct("strItem5", "String");
    GiData << dataStruct("strItem6", "String");
    GiData << dataStruct("strItem7", "String");
    GiData << dataStruct("strItem8", "String");
    GiData << dataStruct("strItem9", "String");
    GiData << dataStruct("strItem10", "String");
    GiData << dataStruct("strItem11", "String");
    GiData << dataStruct("strItem12", "String");
    GiData << dataStruct("strItem13", "String");
    GiData << dataStruct("strItem14", "String");
    GiData << dataStruct("strItem15", "String");
    GiData << dataStruct("strArray1", "String", true);

    //Intialise doubles
    GiData << dataStruct("dblItem1", "Double");
    GiData << dataStruct("dblItem2", "Double");
    GiData << dataStruct("dblItem3", "Double");
    GiData << dataStruct("dblItem4", "Double");
    GiData << dataStruct("dblArray1", "Double", true);

    //Intialise integers
    GiData << dataStruct("intItem1", "Integer");
    GiData << dataStruct("intItem2", "Integer");
    GiData << dataStruct("intItem3", "Integer");
    GiData << dataStruct("intItem4", "Integer");
    GiData << dataStruct("intArray1", "Integer", true);

    //Intialise integers
    GiData << dataStruct("blnItem1", "Boolean");
    GiData << dataStruct("blnItem2", "Boolean");
    GiData << dataStruct("blnItem3", "Boolean");
    GiData << dataStruct("blnItem4", "Boolean");
    GiData << dataStruct("blnItem5", "Boolean");
    GiData << dataStruct("blnItem6", "Boolean");
    GiData << dataStruct("blnItem7", "Boolean");
    GiData << dataStruct("blnItem8", "Boolean");
    GiData << dataStruct("blnItem9", "Boolean");
    GiData << dataStruct("blnItem10", "Boolean");
    GiData << dataStruct("blnItem11", "Boolean");
    GiData << dataStruct("blnItem12", "Boolean");
    GiData << dataStruct("blnItem13", "Boolean");
    GiData << dataStruct("blnItem14", "Boolean");
    GiData << dataStruct("blnItem15", "Boolean");
    GiData << dataStruct("blnArray1", "Boolean", true);

    updateDateTime = new QTimer(this);
    connect(updateDateTime, SIGNAL(timeout()), this, SLOT(updateDateTimeSlot()));
    updateDateTime->start(50);

    updateArrays = new QTimer(this);
    connect(updateArrays, SIGNAL(timeout()), this, SLOT(updateArraysSlot()));
    updateArrays->start(1000);
}

void ConfigManager::updateArraysSlot(){

    //Update the arrays

    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].getIsArray())
        {
            if (GiData[index].getvalueType() == "Boolean")
            {
                QList<bool> temparr = GiData[index].get_bool_arr_values();
                if (temparr.length() > 1)
                {
                    bool value = temparr[0];
                    temparr.pop_front();
                    temparr.append(value);
                }
                GiData[index].set_bool_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "Integer")
            {
                QList<int> temparr = GiData[index].get_int_arr_values();
                if (temparr.length() > 1)
                {
                    int value = temparr[0];
                    temparr.pop_front();
                    temparr.append(value);
                }
                GiData[index].set_int_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "Double")
            {
                QList<double> temparr = GiData[index].get_dbl_arr_values();
                if (temparr.length() > 1)
                {
                    double value = temparr[0];
                    temparr.pop_front();
                    temparr.append(value);
                }
                GiData[index].set_dbl_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "String")
            {
                QList<QString> temparr = GiData[index].get_str_arr_values();
                if (temparr.length() > 1)
                {
                    QString value = temparr[0];
                    temparr.pop_front();
                    temparr.append(value);
                }
                GiData[index].set_str_arr_values(temparr);
            }
        }
    }

    //Send data change signals

    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].getIsArray())
        {
            dataStruct myStruct(GiData[index].gettag(),
                                GiData[index].getvalueType());
            myStruct.setIsArray(true);
            if (myStruct.getvalueType() == "Double")
            {
                myStruct.set_dbl_arr_values(GiData[index].get_dbl_arr_values());
            }
            else if (myStruct.getvalueType() == "Boolean")
            {
                myStruct.set_bool_arr_values(GiData[index].get_bool_arr_values());
            }
            else if (myStruct.getvalueType() == "Integer")
            {
                myStruct.set_int_arr_values(GiData[index].get_int_arr_values());
            }
            else if (myStruct.getvalueType() == "String")
            {
                myStruct.set_str_arr_values(GiData[index].get_str_arr_values());
            }

            emit dataChangedSignal(myStruct);
        }
    }
}

void ConfigManager::updateDateTimeSlot(){
    QDateTime dt = QDateTime::currentDateTime();
    QString tdt = dt.toString(Qt::TextDate);
    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].gettag() == "strServerDateTime")
        {
            //store new value
            GiData[index].setstr(tdt);

            //send data to GS via signaling
            dataStruct myStruct(GiData[index].gettag(),
                                GiData[index].getvalueType(),
                                GiData[index].getbln(),
                                GiData[index].getint(),
                                GiData[index].getdbl(),
                                GiData[index].getstr());

            emit dataChangedSignal(myStruct);
            break;
        }
    }    
}

void ConfigManager::triggerUpdateSlot(dataStruct data){
    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].gettag() == data.gettag())
        {            
            data.setvalueType(GiData[index].getvalueType());
            data.setIsArray(GiData[index].getIsArray());
            if (data.getIsArray())
            {
                if (GiData[index].getvalueType() == "Double")
                {
                    data.set_dbl_arr_values(GiData[index].get_dbl_arr_values());
                //    data.set_ftd_arr_values(GiData[index].get_dbl_arr_values());
                }
                else if (GiData[index].getvalueType() == "Boolean")
                {
                    data.set_bool_arr_values(GiData[index].get_bool_arr_values());
                //    data.set_ftd_arr_values(GiData[index].get_bool_arr_values());
                }
                else if (GiData[index].getvalueType() == "Integer")
                {
                    data.set_int_arr_values(GiData[index].get_int_arr_values());
                //    data.set_ftd_arr_values(GiData[index].get_int_arr_values());
                }
                else if (GiData[index].getvalueType() == "String")
                {
                    data.set_str_arr_values(GiData[index].get_str_arr_values());
                //    data.set_ftd_arr_values(GiData[index].get_str_arr_values());
                }
            }
            else
            {
                data.setbln(GiData[index].getbln());
                data.setdbl(GiData[index].getdbl());
                data.setint(GiData[index].getint());
                data.setstr(GiData[index].getstr());
/*                if (GiData[index].getvalueType() == "Double")
                    data.setformattedValue(GiData[index].getdbl().toString());
                else if (GiData[index].getvalueType() == "Boolean")
                    data.setformattedValue(GiData[index].getbln().toString());
                else if (GiData[index].getvalueType() == "Integer")
                    data.setformattedValue(GiData[index].getint().toString());
                else if (GiData[index].getvalueType() == "String")
                    data.setformattedValue(GiData[index].getstr());*/
            }

            //raise correct data change event
            emit triggerUpdateSignal(data);
            break;
        }
    }
}

void ConfigManager::dataChangedSlot(dataStruct data){
    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].gettag() == data.gettag())
        {
            if (GiData[index].getvalueType() == data.getvalueType())
            {
                //simulate change in handshake state
                data.sethandshake("requestSent");
                emit interfaceStatusSignal(data, false);

                //simulate change in handshake state
                data.sethandshake("HostInProgress");
                emit interfaceStatusSignal(data, false);

                //simulate change in handshake state
                data.sethandshake("HostComplete");
                emit interfaceStatusSignal(data, false);

                //raise correct data change event
                if (data.getvalueType() == "String")
                {
                    GiData[index].setstr(data.getstr());
                }
                else if (data.getvalueType() == "Boolean")
                {
                    GiData[index].setbln(data.getbln());
                }
                else if (data.getvalueType() == "Integer")
                {
                    GiData[index].setint(data.getint());
                }
                else if (data.getvalueType() == "Double")
                {
                    GiData[index].setdbl(data.getdbl());
                }

                emit dataChangedSignal(data);
                break;
            }
        }
    }
}

void ConfigManager::interfaceStatusSlot(dataStruct data){

    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].gettag() == data.gettag())
        {
            if (GiData[index].gethandshake() == "None")
            {
                //simulate change in handshake state
                GiData[index].sethandshake("requestSent");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, true);

                //simulate change in handshake state
                GiData[index].sethandshake("HostInProgress");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, true);

                //simulate change in handshake state
                GiData[index].sethandshake("HostComplete");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, true);
            }
            else
            {
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, true);
            }

            break;
        }
    }
}

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
void dataStruct::setnotComputed(bool m_notComputed){notComputed = m_notComputed;}
void dataStruct::setformattedValue(QString m_formattedValue){formattedValue = m_formattedValue;}
void dataStruct::set_str_arr_values(QList<QString> m_strArray){str_arr_values = m_strArray;}
void dataStruct::set_bool_arr_values(QList<bool> m_boolArray){bool_arr_values = m_boolArray;}
void dataStruct::set_dbl_arr_values(QList<double> m_dblArray){dbl_arr_values = m_dblArray;}
void dataStruct::set_int_arr_values(QList<int> m_intArray){int_arr_values = m_intArray;}
void dataStruct::set_ftd_arr_values(QList<QString> m_ftdArray){ftd_arr_values = m_ftdArray;}
