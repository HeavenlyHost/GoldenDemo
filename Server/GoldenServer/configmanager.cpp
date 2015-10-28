#include "configmanager.h"
#include <QtCore/QDebug>
#include <QTimer>
#include <QThread>

ConfigManager::ConfigManager()
{
    //Intialise strings
    GiData << dataStruct("strServerDateTime", "String", false,0,0,"system date/time");
    GiData << dataStruct("strItem1", "string");
    GiData << dataStruct("strItem2", "string");
    GiData << dataStruct("strItem3", "string");
    GiData << dataStruct("strItem4", "string");
    GiData << dataStruct("strItem5", "string");
    GiData << dataStruct("strItem6", "string");
    GiData << dataStruct("strItem7", "string");
    GiData << dataStruct("strItem8", "string");
    GiData << dataStruct("strItem9", "string");
    GiData << dataStruct("strItem10", "string");
    GiData << dataStruct("strItem11", "string");
    GiData << dataStruct("strItem12", "string");
    GiData << dataStruct("strItem13", "string");
    GiData << dataStruct("strItem14", "string");
    GiData << dataStruct("strItem15", "string");
    GiData << dataStruct("strArray1", "string", true);

    //Intialise doubles
    GiData << dataStruct("dblItem1", "double");
    GiData << dataStruct("dblItem2", "double");
    GiData << dataStruct("dblItem3", "double");
    GiData << dataStruct("dblItem4", "double");
    GiData << dataStruct("dblArray1", "double", true);

    //Intialise integers
    GiData << dataStruct("intItem1", "integer");
    GiData << dataStruct("intItem2", "integer");
    GiData << dataStruct("intItem3", "integer");
    GiData << dataStruct("intItem4", "integer");
    GiData << dataStruct("intArray1", "integer", true);

    //Intialise integers
    GiData << dataStruct("blnItem1", "boolean");
    GiData << dataStruct("blnItem2", "boolean");
    GiData << dataStruct("blnItem3", "boolean");
    GiData << dataStruct("blnItem4", "boolean");
    GiData << dataStruct("blnItem5", "boolean");
    GiData << dataStruct("blnItem6", "boolean");
    GiData << dataStruct("blnItem7", "boolean");
    GiData << dataStruct("blnItem8", "boolean");
    GiData << dataStruct("blnItem9", "boolean");
    GiData << dataStruct("blnItem10", "boolean");
    GiData << dataStruct("blnItem11", "boolean");
    GiData << dataStruct("blnItem12", "boolean");
    GiData << dataStruct("blnItem13", "boolean");
    GiData << dataStruct("blnItem14", "boolean");
    GiData << dataStruct("blnItem15", "boolean");
    GiData << dataStruct("blnArray1", "boolean", true);

    updateDateTime = new QTimer(this);
    connect(updateDateTime, SIGNAL(timeout()), this, SLOT(updateDateTimeSlot()));
    updateDateTime->start(100);

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
            if (GiData[index].getvalueType() == "boolean")
            {
                QList<bool> temparr = GiData[index].get_bool_arr_values();
                if (temparr.length() > 1)
                {
                    int i = getRandomLoop(0, temparr.length() - 1);
                    temparr[i] = !temparr[i];
                }
                GiData[index].set_bool_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "integer")
            {
                QList<int> temparr = GiData[index].get_int_arr_values();
                if (temparr.length() > 1)
                {
                    int i = getRandomLoop(0, temparr.length() - 1);
                    temparr[i]++;
                }
                GiData[index].set_int_arr_values(temparr);
            }
            else if (GiData[index].getvalueType() == "double")
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
            if (GiData[index].getvalueType() == "boolean")
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
            else if (GiData[index].getvalueType() == "integer")
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
            else if (GiData[index].getvalueType() == "double")
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
            else if (GiData[index].getvalueType() == "string")
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
            if (myStruct.getvalueType() == "double")
            {
                myStruct.set_dbl_arr_values(GiData[index].get_dbl_arr_values());
            }
            else if (myStruct.getvalueType() == "boolean")
            {
                myStruct.set_bool_arr_values(GiData[index].get_bool_arr_values());
            }
            else if (myStruct.getvalueType() == "integer")
            {
                myStruct.set_int_arr_values(GiData[index].get_int_arr_values());
            }
            else if (myStruct.getvalueType() == "string")
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
                if (GiData[index].getvalueType() == "double")
                {
                    data.set_dbl_arr_values(GiData[index].get_dbl_arr_values());
                }
                else if (GiData[index].getvalueType() == "boolean")
                {
                    data.set_bool_arr_values(GiData[index].get_bool_arr_values());
                }
                else if (GiData[index].getvalueType() == "integer")
                {
                    data.set_int_arr_values(GiData[index].get_int_arr_values());
                }
                else if (GiData[index].getvalueType() == "string")
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
            //simulate change in handshake state
            data.sethandshake("requestSent");
            emit interfaceStatusSignal(data, false, false);

            //simulate change in handshake state
            data.sethandshake("hostInProgress");
            emit interfaceStatusSignal(data, false, false);

            //simulate change in handshake state
            data.sethandshake("hostComplete");
            emit interfaceStatusSignal(data, false, false);

            data.setvalueType(GiData[index].getvalueType());

            //raise correct data change event
            if (data.getvalueType() == "string")
            {
                GiData[index].setstr(data.getstr());
            }
            else if (data.getvalueType() == "boolean")
            {
                GiData[index].setbln(data.getbln());
            }
            else if (data.getvalueType() == "integer")
            {
                GiData[index].setint(data.getint());
            }
            else if (data.getvalueType() == "double")
            {
                GiData[index].setdbl(data.getdbl());
            }

            emit dataChangedSignal(data);
            break;
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
                emit interfaceStatusSignal(data, false, true);

                //simulate change in handshake state
                GiData[index].sethandshake("hostInProgress");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, false, true);

                //simulate change in handshake state
                GiData[index].sethandshake("hostComplete");
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, true, true);
            }
            else
            {
                data.sethandshake(GiData[index].gethandshake());
                emit interfaceStatusSignal(data, true, true);
            }

            return;
        }
    }

    //interface not found
    data.setErrorState(true);
    data.setErrorReason("Interface Tag Not Available !!!");
    emit interfaceStatusSignal(data, false, false);
}
