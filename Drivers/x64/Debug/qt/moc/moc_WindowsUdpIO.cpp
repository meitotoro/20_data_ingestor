/****************************************************************************
** Meta object code from reading C++ file 'WindowsUdpIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../WindowsUdpIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowsUdpIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WindowsUdpIO_t {
    QByteArrayData data[23];
    char stringdata0[307];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WindowsUdpIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WindowsUdpIO_t qt_meta_stringdata_WindowsUdpIO = {
    {
QT_MOC_LITERAL(0, 0, 12), // "WindowsUdpIO"
QT_MOC_LITERAL(1, 13, 12), // "dataReceived"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "data"
QT_MOC_LITERAL(4, 32, 16), // "datagramReceived"
QT_MOC_LITERAL(5, 49, 16), // "QNetworkDatagram"
QT_MOC_LITERAL(6, 66, 8), // "datagram"
QT_MOC_LITERAL(7, 75, 13), // "errorOccurred"
QT_MOC_LITERAL(8, 89, 11), // "errorString"
QT_MOC_LITERAL(9, 101, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(10, 125, 9), // "connected"
QT_MOC_LITERAL(11, 135, 12), // "sendDatagram"
QT_MOC_LITERAL(12, 148, 14), // "sendDatagramTo"
QT_MOC_LITERAL(13, 163, 4), // "host"
QT_MOC_LITERAL(14, 168, 4), // "port"
QT_MOC_LITERAL(15, 173, 11), // "handleError"
QT_MOC_LITERAL(16, 185, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(17, 214, 5), // "error"
QT_MOC_LITERAL(18, 220, 15), // "handleReadyRead"
QT_MOC_LITERAL(19, 236, 18), // "handleStateChanged"
QT_MOC_LITERAL(20, 255, 28), // "QAbstractSocket::SocketState"
QT_MOC_LITERAL(21, 284, 5), // "state"
QT_MOC_LITERAL(22, 290, 16) // "attemptReconnect"

    },
    "WindowsUdpIO\0dataReceived\0\0data\0"
    "datagramReceived\0QNetworkDatagram\0"
    "datagram\0errorOccurred\0errorString\0"
    "connectionStatusChanged\0connected\0"
    "sendDatagram\0sendDatagramTo\0host\0port\0"
    "handleError\0QAbstractSocket::SocketError\0"
    "error\0handleReadyRead\0handleStateChanged\0"
    "QAbstractSocket::SocketState\0state\0"
    "attemptReconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WindowsUdpIO[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       7,    1,   70,    2, 0x06 /* Public */,
       9,    1,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   76,    2, 0x0a /* Public */,
      12,    3,   79,    2, 0x0a /* Public */,
      15,    1,   86,    2, 0x08 /* Private */,
      18,    0,   89,    2, 0x08 /* Private */,
      19,    1,   90,    2, 0x08 /* Private */,
      22,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::QString, QMetaType::UShort,    3,   13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,

       0        // eod
};

void WindowsUdpIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WindowsUdpIO *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->datagramReceived((*reinterpret_cast< const QNetworkDatagram(*)>(_a[1]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->connectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->sendDatagram((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->sendDatagramTo((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 6: _t->handleError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 7: _t->handleReadyRead(); break;
        case 8: _t->handleStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 9: _t->attemptReconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkDatagram >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WindowsUdpIO::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WindowsUdpIO::dataReceived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WindowsUdpIO::*)(const QNetworkDatagram & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WindowsUdpIO::datagramReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WindowsUdpIO::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WindowsUdpIO::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WindowsUdpIO::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WindowsUdpIO::connectionStatusChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WindowsUdpIO::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WindowsUdpIO.data,
    qt_meta_data_WindowsUdpIO,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WindowsUdpIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WindowsUdpIO::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WindowsUdpIO.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "PlatformIO"))
        return static_cast< PlatformIO*>(this);
    return QObject::qt_metacast(_clname);
}

int WindowsUdpIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void WindowsUdpIO::dataReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WindowsUdpIO::datagramReceived(const QNetworkDatagram & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WindowsUdpIO::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WindowsUdpIO::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
