/****************************************************************************
** Meta object code from reading C++ file 'goldenserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GoldenServer/goldenserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'goldenserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GoldenServer_t {
    QByteArrayData data[9];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GoldenServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GoldenServer_t qt_meta_stringdata_GoldenServer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "GoldenServer"
QT_MOC_LITERAL(1, 13, 6), // "closed"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 7), // "timeout"
QT_MOC_LITERAL(4, 29, 15), // "onNewConnection"
QT_MOC_LITERAL(5, 45, 18), // "processTextMessage"
QT_MOC_LITERAL(6, 64, 7), // "message"
QT_MOC_LITERAL(7, 72, 18), // "socketDisconnected"
QT_MOC_LITERAL(8, 91, 6) // "update"

    },
    "GoldenServer\0closed\0\0timeout\0"
    "onNewConnection\0processTextMessage\0"
    "message\0socketDisconnected\0update"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GoldenServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   46,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GoldenServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GoldenServer *_t = static_cast<GoldenServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->timeout(); break;
        case 2: _t->onNewConnection(); break;
        case 3: _t->processTextMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->socketDisconnected(); break;
        case 5: _t->update(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GoldenServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GoldenServer::closed)) {
                *result = 0;
            }
        }
        {
            typedef void (GoldenServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GoldenServer::timeout)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject GoldenServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GoldenServer.data,
      qt_meta_data_GoldenServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GoldenServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GoldenServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GoldenServer.stringdata0))
        return static_cast<void*>(const_cast< GoldenServer*>(this));
    return QObject::qt_metacast(_clname);
}

int GoldenServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void GoldenServer::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void GoldenServer::timeout()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
