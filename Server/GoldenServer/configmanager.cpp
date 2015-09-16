#include "configmanager.h"
#include <QtCore/QDebug>

ConfigManager::ConfigManager(QObject* parent)
{
}

void ConfigManager::bindMe()
{
    qDebug() << "bindMe!!!";
}

void ConfigManager::unbindMe()
{
    qDebug() << "unbindMe!!!";
}
