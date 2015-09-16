#include "timertest.h"

void timerTest::testTimer()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

//QTEST_MAIN(timerTest)
