/****************************************************************************
** Meta object code from reading C++ file 'CFARsim.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CFARsim.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CFARsim.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CFARsim[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      25,    8,    8,    8, 0x08,
      44,    8,    8,    8, 0x08,
      60,    8,    8,    8, 0x08,
      86,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CFARsim[] = {
    "CFARsim\0\0slot_PlotData()\0slot_updateTable()\0"
    "slot_updateCB()\0slot_updateCFARcontrols()\0"
    "slot_updateDynScaling()\0"
};

void CFARsim::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CFARsim *_t = static_cast<CFARsim *>(_o);
        switch (_id) {
        case 0: _t->slot_PlotData(); break;
        case 1: _t->slot_updateTable(); break;
        case 2: _t->slot_updateCB(); break;
        case 3: _t->slot_updateCFARcontrols(); break;
        case 4: _t->slot_updateDynScaling(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CFARsim::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CFARsim::staticMetaObject = {
    { &Applet::staticMetaObject, qt_meta_stringdata_CFARsim,
      qt_meta_data_CFARsim, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CFARsim::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CFARsim::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CFARsim::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFARsim))
        return static_cast<void*>(const_cast< CFARsim*>(this));
    return Applet::qt_metacast(_clname);
}

int CFARsim::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Applet::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
