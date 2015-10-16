#ifndef CONFIGMANAGER
#define CONFIGMANAGER
#include <QObject>
#include <QWebSocket>
#include <QTimer>

#include "datastruct.h"

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    ConfigManager();

public slots:

    void triggerUpdateSlot(dataStruct data);
    void dataChangedSlot(dataStruct data);
    void interfaceStatusSlot(dataStruct data);
    void updateDateTimeSlot();
    void updateArraysSlot();

signals:

    void triggerUpdateSignal(dataStruct data);
    void dataChangedSignal(dataStruct data);
    void interfaceStatusSignal(dataStruct data, bool doSubscribe, bool triggerUpdate);
    void timeout();

private:
    void updateArrayData();
    void sendArrayUpdates();
    void updateArrayDataRandom();
    int getRandomNo(int low, int high);
    int getRandomLoop(int low, int high);

    QList<dataStruct> GiData;
    QTimer *updateDateTime;
    QTimer *updateArrays;

};


#endif // CONFIGMANAGER

