#include "mytimer.h"
#include <QtCore>
#include <QDebug>

MyTimer::MyTimer()
{
    currentTime = "not defined";
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1000);
}

QString MyTimer::getCurrentTime()
{
    return currentTime;
}

bool MyTimer::getTimerEnabled()
{
    return timer->isActive();
}

void MyTimer::setTimerEnabled(bool bValue)
{
    if (!bValue)
        timer->stop();
    else
        timer->start();
    emit timerEnableChanged();
}

QString MyTimer::getInterfaceName()
{
    return interfaceName;
}

void MyTimer::setInterfaceName(QString sValue)
{
    if (interfaceName == sValue)
        return;
    interfaceName = sValue;
    emit interfaceNameChanged();
}

void MyTimer::update()
{
    QString temp = QTime::currentTime().toString();
    if (currentTime == temp)
        return;
    currentTime = temp;
    qDebug() << "update fired!!!";
    emit currentTimeChanged();
}
