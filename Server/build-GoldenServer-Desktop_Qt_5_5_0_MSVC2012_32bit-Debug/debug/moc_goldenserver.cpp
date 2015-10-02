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
    QByteArrayData data[18];
    char stringdata0[242];
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
QT_MOC_LITERAL(4, 29, 19), // "triggerUpdateSignal"
QT_MOC_LITERAL(5, 49, 10), // "dataStruct"
QT_MOC_LITERAL(6, 60, 4), // "data"
QT_MOC_LITERAL(7, 65, 17), // "dataChangedSignal"
QT_MOC_LITERAL(8, 83, 21), // "interfaceStatusSignal"
QT_MOC_LITERAL(9, 105, 15), // "onNewConnection"
QT_MOC_LITERAL(10, 121, 18), // "processTextMessage"
QT_MOC_LITERAL(11, 140, 7), // "message"
QT_MOC_LITERAL(12, 148, 18), // "socketDisconnected"
QT_MOC_LITERAL(13, 167, 6), // "update"
QT_MOC_LITERAL(14, 174, 17), // "triggerUpdateSlot"
QT_MOC_LITERAL(15, 192, 15), // "dataChangedSlot"
QT_MOC_LITERAL(16, 208, 19), // "interfaceStatusSlot"
QT_MOC_LITERAL(17, 228, 13) // "triggerUpdate"

    },
    "GoldenServer\0closed\0\0timeout\0"
    "triggerUpdateSignal\0dataStruct\0data\0"
    "dataChangedSignal\0interfaceStatusSignal\0"
    "onNewConnection\0processTextMessage\0"
    "message\0socketDisconnected\0update\0"
    "triggerUpdateSlot\0dataChangedSlot\0"
    "interfaceStatusSlot\0triggerUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GoldenServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    1,   76,    2, 0x06 /* Public */,
       7,    1,   79,    2, 0x06 /* Public */,
       8,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   85,    2, 0x0a /* Public */,
      10,    1,   86,    2, 0x0a /* Public */,
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    1,   91,    2, 0x0a /* Public */,
      15,    1,   94,    2, 0x0a /* Public */,
      16,    2,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Bool,    6,   17,

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
        case 2: _t->triggerUpdateSignal((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 3: _t->dataChangedSignal((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 4: _t->interfaceStatusSignal((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 5: _t->onNewConnection(); break;
        case 6: _t->processTextMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->socketDisconnected(); break;
        case 8: _t->update(); break;
        case 9: _t->triggerUpdateSlot((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 10: _t->dataChangedSlot((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 11: _t->interfaceStatusSlot((*reinterpret_cast< dataStruct(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
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
        {
            typedef void (GoldenServer::*_t)(dataStruct );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GoldenServer::triggerUpdateSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (GoldenServer::*_t)(dataStruct );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GoldenServer::dataChangedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (GoldenServer::*_t)(dataStruct );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GoldenServer::interfaceStatusSignal)) {
                *result = 4;
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
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

// SIGNAL 2
void GoldenServer::triggerUpdateSignal(dataStruct _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GoldenServer::dataChangedSignal(dataStruct _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GoldenServer::interfaceStatusSignal(dataStruct _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
