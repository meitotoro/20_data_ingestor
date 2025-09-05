/****************************************************************************
** Meta object code from reading C++ file 'LinuxSerialIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../LinuxSerialIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinuxSerialIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LinuxSerialIO_t {
    QByteArrayData data[6];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LinuxSerialIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LinuxSerialIO_t qt_meta_stringdata_LinuxSerialIO = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LinuxSerialIO"
QT_MOC_LITERAL(1, 14, 11), // "handleError"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(4, 56, 5), // "error"
QT_MOC_LITERAL(5, 62, 15) // "handleReadyRead"

    },
    "LinuxSerialIO\0handleError\0\0"
    "QSerialPort::SerialPortError\0error\0"
    "handleReadyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LinuxSerialIO[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       5,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void LinuxSerialIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LinuxSerialIO *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 1: _t->handleReadyRead(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LinuxSerialIO::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LinuxSerialIO.data,
    qt_meta_data_LinuxSerialIO,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LinuxSerialIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinuxSerialIO::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LinuxSerialIO.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "PlatformIO"))
        return static_cast< PlatformIO*>(this);
    return QObject::qt_metacast(_clname);
}

int LinuxSerialIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
