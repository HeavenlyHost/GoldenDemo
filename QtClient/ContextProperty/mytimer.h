#ifndef MYTIMER_H
#define MYTIMER_H

#include <QtCore>

class MyTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cTime READ getCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool eTimer READ getTimerEnabled WRITE setTimerEnabled NOTIFY timerEnableChanged)
    Q_PROPERTY(QString sName READ getInterfaceName WRITE setInterfaceName NOTIFY interfaceNameChanged)

public:
    MyTimer();
    QTimer *timer;
    QString getCurrentTime();
    bool getTimerEnabled();
    void setTimerEnabled(bool bValue);
    QString getInterfaceName();
    void setInterfaceName(QString sValue);
public slots:
    void update();
signals:
    void currentTimeChanged();
    void timerEnableChanged();
    void interfaceNameChanged();
private:
    QString currentTime;
    QString interfaceName;
};

#endif // MYTIMER_H
