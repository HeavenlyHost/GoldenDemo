#include "configmanager.h"
#include <QtCore/QDebug>
#include <QTimer>

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

    //Intialise doubles
    GiData << dataStruct("dblItem1", "Double");
    GiData << dataStruct("dblItem2", "Double");
    GiData << dataStruct("dblItem3", "Double");
    GiData << dataStruct("dblItem4", "Double");

    //Intialise integers
    GiData << dataStruct("intItem1", "Integer");
    GiData << dataStruct("intItem2", "Integer");
    GiData << dataStruct("intItem3", "Integer");
    GiData << dataStruct("intItem4", "Integer");

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

    updateDateTime = new QTimer(this);
    connect(updateDateTime, SIGNAL(timeout()), this, SLOT(updateDateTimeSlot()));
    updateDateTime->start(500);
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
            data.setbln(GiData[index].getbln());
            data.setdbl(GiData[index].getdbl());
            data.setint(GiData[index].getint());
            data.setstr(GiData[index].getstr());

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
                emit interfaceStatusSignal(data);

                //simulate change in handshake state
                GiData[index].sethandshake("HostInProgress");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data);

                //simulate change in handshake state
                GiData[index].sethandshake("HostComplete");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data);
            }
            else
            {
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data);
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

dataStruct::dataStruct( QString m_tag, QString m_valueType )
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
}

dataStruct::dataStruct( QString m_tag, QString m_valueType, QWebSocket *m_sock )
{
    Initialise();

    tag = m_tag;
    valueType = m_valueType;
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

void dataStruct::setvalueType(QString m_valueType){valueType = m_valueType;}
void dataStruct::setsock(QWebSocket *qws){sock = qws;}
void dataStruct::sethandshake(QString m_handshake){handshake = m_handshake;}
void dataStruct::setbln(bool m_bln){blnVal = m_bln;}
void dataStruct::setstr(QString m_str){strVal = m_str;}
void dataStruct::setint(int m_int){intVal = m_int;}
void dataStruct::setdbl(double m_dbl){dblVal = m_dbl;}
void dataStruct::setnotComputed(bool m_notComputed){notComputed = m_notComputed;}
void dataStruct::setformattedValue(bool m_formattedValue){formattedValue = m_formattedValue;}

