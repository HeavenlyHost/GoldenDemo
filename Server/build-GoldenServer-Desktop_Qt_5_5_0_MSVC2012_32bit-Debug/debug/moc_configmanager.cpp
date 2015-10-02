/****************************************************************************
** Meta object code from reading C++ file 'configmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GoldenServer/configmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'configmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConfigManager_t {
    QByteArrayData data[14];
    char stringdata0[203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConfigManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConfigManager_t qt_meta_stringdata_ConfigManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ConfigManager"
QT_MOC_LITERAL(1, 14, 19), // "triggerUpdateSignal"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "dataStruct"
QT_MOC_LITERAL(4, 46, 4), // "data"
QT_MOC_LITERAL(5, 51, 17), // "dataChangedSignal"
QT_MOC_LITERAL(6, 69, 21), // "interfaceStatusSignal"
QT_MOC_LITERAL(7, 91, 13), // "triggerUpdate"
QT_MOC_LITERAL(8, 105, 7), // "timeout"
QT_MOC_LITERAL(9, 113, 17), // "triggerUpdateSlot"
QT_MOC_LITERAL(10, 131, 15), // "dataChangedSlot"
QT_MOC_LITERAL(11, 147, 19), // "interfaceStatusSlot"
QT_MOC_LITERAL(12, 167, 18), // "updateDateTimeSlot"
QT_MOC_LITERAL(13, 186, 16) // "updateArraysSlot"

    },
    "ConfigManager\0triggerUpdateSignal\0\0"
    "dataStruct\0data\0dataChangedSignal\0"
    "interfaceStatusSignal\0triggerUpdate\0"
    "timeout\0triggerUpdateSlot\0dataChangedSlot\0"
    "interfaceStatusSlot\0updateDateTimeSlot\0"
    "updateArraysSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConfigManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       6,    2,   65,    2, 0x06 /* Public */,
       8,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   71,    2, 0x0a /* Public */,
      10,    1,   74,    2, 0x0a /* Public */,
      11,    1,   77,    2, 0x0a /* Public */,
      12,    0,   80,    2, 0x0a /* Public */,
      13,    0,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConfigManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConfigManager *_t = static_cast<ConfigManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->triggerUpdateSignal((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 1: _t->dataChangedSignal((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 2: _t->interfaceStatusSignal((*reinterpret_cast< dataStruct(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->timeout(); break;
        case 4: _t->triggerUpdateSlot((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 5: _t->dataChangedSlot((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 6: _t->interfaceStatusSlot((*reinterpret_cast< dataStruct(*)>(_a[1]))); break;
        case 7: _t->updateDateTimeSlot(); break;
        case 8: _t->updateArraysSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConfigManager::*_t)(dataStruct );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigManager::triggerUpdateSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ConfigManager::*_t)(dataStruct );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigManager::dataChangedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ConfigManager::*_t)(dataStruct , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigManager::interfaceStatusSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (ConfigManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigManager::timeout)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ConfigManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ConfigManager.data,
      qt_meta_data_ConfigManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConfigManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigManager.stringdata0))
        return static_cast<void*>(const_cast< ConfigManager*>(this));
    return QObject::qt_metacast(_clname);
}

int ConfigManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ConfigManager::triggerUpdateSignal(dataStruct _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConfigManager::dataChangedSignal(dataStruct _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConfigManager::interfaceStatusSignal(dataStruct _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ConfigManager::timeout()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
