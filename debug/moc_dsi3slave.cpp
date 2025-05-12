/****************************************************************************
** Meta object code from reading C++ file 'dsi3slave.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dsi3slave.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dsi3slave.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DSI3Slave[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      32,   10,   10,   10, 0x08,
      52,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DSI3Slave[] = {
    "DSI3Slave\0\0slot_VersionUpdate()\0"
    "slot_UpdateLabels()\0slot_GainGraphUpdate()\0"
};

void DSI3Slave::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DSI3Slave *_t = static_cast<DSI3Slave *>(_o);
        switch (_id) {
        case 0: _t->slot_VersionUpdate(); break;
        case 1: _t->slot_UpdateLabels(); break;
        case 2: _t->slot_GainGraphUpdate(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DSI3Slave::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DSI3Slave::staticMetaObject = {
    { &Applet::staticMetaObject, qt_meta_stringdata_DSI3Slave,
      qt_meta_data_DSI3Slave, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DSI3Slave::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DSI3Slave::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DSI3Slave::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DSI3Slave))
        return static_cast<void*>(const_cast< DSI3Slave*>(this));
    return Applet::qt_metacast(_clname);
}

int DSI3Slave::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Applet::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
