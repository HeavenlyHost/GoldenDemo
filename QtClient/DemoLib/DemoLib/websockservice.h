#ifndef WEBSOCKSERVICE
#define WEBSOCKSERVICE

#include <QObject>

class WebSockService : public QObject
{
    Q_OBJECT

public:
    WebSockService(QObject* parent = 0);
};

#endif // WEBSOCKSERVICE

