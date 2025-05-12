/****************************************************************************
** Meta object code from reading C++ file 'logs.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/logs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Logs[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,    6,    5,    5, 0x0a,
      41,    5,    5,    5, 0x08,
      64,    5,    5,    5, 0x08,
      92,    5,    5,    5, 0x08,
     110,    5,    5,    5, 0x08,
     131,    5,    5,    5, 0x08,
     153,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Logs[] = {
    "Logs\0\0type,text\0slot_addLog(int,QString)\0"
    "slot_pbClear_clicked()\0"
    "slot_LogContextMenu(QPoint)\0"
    "slot_ActionCopy()\0slot_ActionCopyAll()\0"
    "slot_ControllerSend()\0slot_ResetStatus()\0"
};

void Logs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Logs *_t = static_cast<Logs *>(_o);
        switch (_id) {
        case 0: _t->slot_addLog((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->slot_pbClear_clicked(); break;
        case 2: _t->slot_LogContextMenu((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 3: _t->slot_ActionCopy(); break;
        case 4: _t->slot_ActionCopyAll(); break;
        case 5: _t->slot_ControllerSend(); break;
        case 6: _t->slot_ResetStatus(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Logs::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Logs::staticMetaObject = {
    { &Applet::staticMetaObject, qt_meta_stringdata_Logs,
      qt_meta_data_Logs, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Logs::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Logs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Logs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Logs))
        return static_cast<void*>(const_cast< Logs*>(this));
    return Applet::qt_metacast(_clname);
}

int Logs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Applet::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
