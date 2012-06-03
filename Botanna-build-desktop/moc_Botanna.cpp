/****************************************************************************
** Meta object code from reading C++ file 'Botanna.h'
**
** Created: Mon Jun 4 00:36:22 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../source/Botanna.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Botanna.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Botanna[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,    9,    8,    8, 0x08,
      53,   36,    8,    8, 0x08,
     105,   92,    8,    8, 0x08,
     151,  127,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Botanna[] = {
    "Botanna\0\0connected\0joinPublic(bool)\0"
    "message,userName\0"
    "processPublicMessage(QString&,QString)\0"
    "userToRemove\0gdfUserLeave(QString)\0"
    "oldUserName,newUserName\0"
    "updateGDFUser(QString,QString)\0"
};

void Botanna::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Botanna *_t = static_cast<Botanna *>(_o);
        switch (_id) {
        case 0: _t->joinPublic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->processPublicMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->gdfUserLeave((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->updateGDFUser((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Botanna::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Botanna::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Botanna,
      qt_meta_data_Botanna, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Botanna::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Botanna::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Botanna::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Botanna))
        return static_cast<void*>(const_cast< Botanna*>(this));
    return QObject::qt_metacast(_clname);
}

int Botanna::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
