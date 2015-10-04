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
    updateArrays->start(100);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

void ConfigManager::updateArraysSlot(){

    //Update the arrays
//    updateArrayData();

    //Update the arrays
    updateArrayDataRandom();

    //Send data change signals
    sendArrayUpdates();

}

int ConfigManager::getRandomNo(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

int ConfigManager::getRandomLoop(int low, int high)
{
    int myRand = -1;

    while (myRand < 0 || myRand > GiData.length()-1)
    {
        myRand = getRandomNo(low, high);
    }

    return myRand;
}

void ConfigManager::updateArrayDataRandom()
{
    if (GiData.length() == 0)
    {
        return;
    }

    for (int index = 0; index < GiData.length(); index++)
    {
        if (GiData[index].getIsArray())
        {
            if (GiData[index].getvalueType() == "Boolean")
            {
                QList<bool> temparr = GiData[index].get_bool_arr_values();
                if (temparr.length() > 1)
                {
                    int i = getRandomLoop(0, temparr.length() - 1);
                    temparr[i] = !temparr[i];
                }
                GiData[index].set_bool_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "Integer")
            {
                QList<int> temparr = GiData[index].get_int_arr_values();
                if (temparr.length() > 1)
                {
                    int i = getRandomLoop(0, temparr.length() - 1);
                    temparr[i]++;
                }
                GiData[index].set_int_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "Double")
            {
                QList<double> temparr = GiData[index].get_dbl_arr_values();
                if (temparr.length() > 1)
                {
                    int i = getRandomLoop(0, temparr.length() - 1);
                    temparr[i]++;
                }
                GiData[index].set_dbl_arr_values(temparr);
            }
        }
    }
}

void ConfigManager::updateArrayData()
{
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
}

void ConfigManager::sendArrayUpdates()
{
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
                }
                else if (GiData[index].getvalueType() == "Boolean")
                {
                    data.set_bool_arr_values(GiData[index].get_bool_arr_values());
                }
                else if (GiData[index].getvalueType() == "Integer")
                {
                    data.set_int_arr_values(GiData[index].get_int_arr_values());
                }
                else if (GiData[index].getvalueType() == "String")
                {
                    data.set_str_arr_values(GiData[index].get_str_arr_values());
                }
            }
            else
            {
                data.setbln(GiData[index].getbln());
                data.setdbl(GiData[index].getdbl());
                data.setint(GiData[index].getint());
                data.setstr(GiData[index].getstr());
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
