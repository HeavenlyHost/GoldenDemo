#include "xcppxbutton.h"
#include <QtCore>

CppXButton::CppXButton()
{

}

QString CppXButton::getName()
{
    return name;
}

void CppXButton::setName(QString sValue)
{
    if (name == sValue)
        return;
    name = sValue;
    emit nameChanged();
}

int CppXButton::CheckTest(int input)
{
    return input*2;
}

