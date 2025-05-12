/****************************************************************************
** Meta object code from reading C++ file 'applet.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bench/generic/applet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'applet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Applet[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      35,    7,    7,    7, 0x05,
      55,    7,    7,    7, 0x05,
      89,   87,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,    7,    7,    7, 0x08,
     136,    7,    7,    7, 0x08,
     161,    7,    7,    7, 0x08,
     185,    7,    7,    7, 0x08,
     206,  201,    7,    7, 0x0a,
     230,  201,    7,    7, 0x0a,
     254,  201,    7,    7, 0x0a,
     287,  283,    7,    7, 0x0a,
     321,  317,    7,    7, 0x0a,
     351,    7,    7,    7, 0x0a,
     376,    7,    7,    7, 0x0a,
     394,    7,    7,    7, 0x0a,
     429,  424,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Applet[] = {
    "Applet\0\0signal_updateAllControls()\0"
    "signal_HideWindow()\0signal_RegisterChanged(QString)\0"
    ",\0signal_log(int,QString)\0"
    "slot_ActionTriggered()\0slot_SetInactiveWindow()\0"
    "slot_ControlTriggered()\0slot_showHelp()\0"
    "name\0slot_updateSet(QString)\0"
    "slot_updateGet(QString)\0"
    "slot_updateControls(QString)\0reg\0"
    "slot_updateControls(Register)\0var\0"
    "slot_updateControls(Variable)\0"
    "slot_updateAllControls()\0slot_adjustSize()\0"
    "slot_capturedData(QByteArray)\0data\0"
    "transferVecData(QByteArray&)\0"
};

void Applet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Applet *_t = static_cast<Applet *>(_o);
        switch (_id) {
        case 0: _t->signal_updateAllControls(); break;
        case 1: _t->signal_HideWindow(); break;
        case 2: _t->signal_RegisterChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->signal_log((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->slot_ActionTriggered(); break;
        case 5: _t->slot_SetInactiveWindow(); break;
        case 6: _t->slot_ControlTriggered(); break;
        case 7: _t->slot_showHelp(); break;
        case 8: _t->slot_updateSet((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->slot_updateGet((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->slot_updateControls((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->slot_updateControls((*reinterpret_cast< const Register(*)>(_a[1]))); break;
        case 12: _t->slot_updateControls((*reinterpret_cast< const Variable(*)>(_a[1]))); break;
        case 13: _t->slot_updateAllControls(); break;
        case 14: _t->slot_adjustSize(); break;
        case 15: _t->slot_capturedData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 16: _t->transferVecData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Applet::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Applet::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Applet,
      qt_meta_data_Applet, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Applet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Applet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Applet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Applet))
        return static_cast<void*>(const_cast< Applet*>(this));
    return QWidget::qt_metacast(_clname);
}

int Applet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void Applet::signal_updateAllControls()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Applet::signal_HideWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Applet::signal_RegisterChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Applet::signal_log(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
