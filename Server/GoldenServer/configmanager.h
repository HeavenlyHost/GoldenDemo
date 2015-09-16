#ifndef CONFIGMANAGER
#define CONFIGMANAGER

#include <QObject>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    ConfigManager(QObject* parent = 0);
    void bindMe();
    void unbindMe();
};

#endif // CONFIGMANAGER

