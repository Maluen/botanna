/****************************************************************************
** Meta object code from reading C++ file 'Chat.h'
**
** Created: Mon Jun 4 00:36:23 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../source/Chat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Chat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Chat[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,    6,    5,    5, 0x05,
      35,    6,    5,    5, 0x05,
      71,   54,    5,    5, 0x05,
     106,   54,    5,    5, 0x05,
     150,  141,    5,    5, 0x05,
     172,  141,    5,    5, 0x05,
     195,  141,    5,    5, 0x05,
     220,  141,    5,    5, 0x05,
     246,  141,    5,    5, 0x05,
     290,  266,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
     322,    5,    5,    5, 0x0a,
     349,  340,    5,    5, 0x08,
     363,    5,    5,    5, 0x28,
     370,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Chat[] = {
    "Chat\0\0status\0connectFinished(bool)\0"
    "joinFinished(bool)\0message,userName\0"
    "newPublicMessage(QString&,QString)\0"
    "newPrivateMessage(QString,QString)\0"
    "userName\0userLoggedIn(QString)\0"
    "userLoggedOut(QString)\0userJoinChannel(QString)\0"
    "userLeaveChannel(QString)\0userKicked(QString)\0"
    "oldUserName,newUserName\0"
    "userChangeNick(QString,QString)\0"
    "connectFinished()\0roomName\0read(QString)\0"
    "read()\0readFinished()\0"
};

void Chat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Chat *_t = static_cast<Chat *>(_o);
        switch (_id) {
        case 0: _t->connectFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->joinFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->newPublicMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->newPrivateMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->userLoggedIn((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->userLoggedOut((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->userJoinChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->userLeaveChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->userKicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->userChangeNick((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->connectFinished(); break;
        case 11: _t->read((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->read(); break;
        case 13: _t->readFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Chat::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Chat::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chat,
      qt_meta_data_Chat, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Chat::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Chat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Chat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Chat))
        return static_cast<void*>(const_cast< Chat*>(this));
    return QObject::qt_metacast(_clname);
}

int Chat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Chat::connectFinished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chat::joinFinished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Chat::newPublicMessage(QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Chat::newPrivateMessage(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Chat::userLoggedIn(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Chat::userLoggedOut(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Chat::userJoinChannel(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Chat::userLeaveChannel(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Chat::userKicked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Chat::userChangeNick(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
