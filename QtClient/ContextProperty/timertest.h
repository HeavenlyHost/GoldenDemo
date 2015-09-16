#ifndef TIMERTEST_H
#define TIMERTEST_H

#include <QtTest/QtTest>

class timerTest: public QObject
{
    Q_OBJECT
private slots:
    void testTimer();
};

#endif // TIMERTEST_H
