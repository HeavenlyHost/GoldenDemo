#ifndef CONFIGMANAGER
#define CONFIGMANAGER

#include <QObject>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    ConfigManager();
    void bindMe();
    void unbindMe();

private:
    QString strConfigItem1;

    int lngConfigItem1;

    double dblConfigItem1;

    struct demoGridStruct
    {
        int valueInt;
        QString valueString;
        double valueDouble;
    };

    QList<demoGridStruct> demoTableData;
};

#endif // CONFIGMANAGER

