/****************************************************************************
** Meta object code from reading C++ file 'trilateration.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../trilateration.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trilateration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Trilateration[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Trilateration[] = {
    "Trilateration\0\0index\0"
    "on_cbAlgChoice_currentIndexChanged(int)\0"
};

void Trilateration::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Trilateration *_t = static_cast<Trilateration *>(_o);
        switch (_id) {
        case 0: _t->on_cbAlgChoice_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Trilateration::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Trilateration::staticMetaObject = {
    { &Applet::staticMetaObject, qt_meta_stringdata_Trilateration,
      qt_meta_data_Trilateration, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Trilateration::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Trilateration::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Trilateration::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Trilateration))
        return static_cast<void*>(const_cast< Trilateration*>(this));
    return Applet::qt_metacast(_clname);
}

int Trilateration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Applet::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
