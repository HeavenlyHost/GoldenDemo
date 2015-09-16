#include <QString>
#include <QtTest/QTest>

#include "../ContextProperty/mytimer.h"
#include "../DemoLib/DemoLib/cppxbutton.h"

class UnitTestsTest : public QObject
{
    Q_OBJECT

public:
    UnitTestsTest();

private Q_SLOTS:
    void testCase1();
    void testCase2();
};

UnitTestsTest::UnitTestsTest()
{
}

void UnitTestsTest::testCase1()
{
//    QVERIFY2(true, "Failure");
    MyTimer *tmr = new MyTimer();
    QCOMPARE(tmr->timer->isActive(), false);
}

void UnitTestsTest::testCase2()
{
    CppXButton *button = new CppXButton();
    QCOMPARE(button->CheckTest(3), 5);
}

QTEST_MAIN(UnitTestsTest)

#include "tst_unitteststest.moc"
