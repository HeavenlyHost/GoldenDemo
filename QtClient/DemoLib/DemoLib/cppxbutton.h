#ifndef CPPXBUTTON_H
#define CPPXBUTTON_H

#include <QtCore>

class CppXButton : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sName READ getName WRITE setName NOTIFY nameChanged)

public:
    CppXButton();
    QString getName();
    void setName(QString sValue);
    int CheckTest(int input);
signals:
    void nameChanged();
private:
    QString name;
};

#endif // CPPXBUTTON_H
