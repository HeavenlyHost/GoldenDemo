#include "configmanager.h"
#include <QtCore/QDebug>

ConfigManager::ConfigManager():
    demoTableData()
{
    strConfigItem1 = "str config item 1";
    lngConfigItem1 = 777;
    dblConfigItem1 = 777.777;

    for (int i=0; i < 10; i++)
    {
        demoGridStruct newItem;
        newItem.valueInt = i*2;
        newItem.valueDouble = i*1.33;
        newItem.valueString = i*10;
        demoTableData << newItem;
    }
}

void ConfigManager::bindMe()
{
    qDebug() << "bindMe!!!";
}

void ConfigManager::unbindMe()
{
    qDebug() << "unbindMe!!!";
}
