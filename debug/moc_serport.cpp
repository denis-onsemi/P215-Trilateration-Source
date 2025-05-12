/****************************************************************************
** Meta object code from reading C++ file 'serport.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/serport/serport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SerPort[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x05,
      35,    8,    8,    8, 0x05,
      47,    8,    8,    8, 0x05,
      59,    8,    8,    8, 0x05,
      81,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SerPort[] = {
    "SerPort\0\0,\0signal_log(int,QString)\0"
    "signal_tx()\0signal_rx()\0signal_disconnected()\0"
    "signal_connected()\0"
};

void SerPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SerPort *_t = static_cast<SerPort *>(_o);
        switch (_id) {
        case 0: _t->signal_log((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->signal_tx(); break;
        case 2: _t->signal_rx(); break;
        case 3: _t->signal_disconnected(); break;
        case 4: _t->signal_connected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SerPort::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SerPort::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SerPort,
      qt_meta_data_SerPort, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SerPort::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SerPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SerPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SerPort))
        return static_cast<void*>(const_cast< SerPort*>(this));
    return QThread::qt_metacast(_clname);
}

int SerPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SerPort::signal_log(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerPort::signal_tx()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SerPort::signal_rx()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SerPort::signal_disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SerPort::signal_connected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
