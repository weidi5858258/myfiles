/****************************************************************************
** Meta object code from reading C++ file 'twinswidgetboss.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MyStudy/twinswidgetboss.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'twinswidgetboss.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TwinsWidgetBoss_t {
    QByteArrayData data[4];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TwinsWidgetBoss_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TwinsWidgetBoss_t qt_meta_stringdata_TwinsWidgetBoss = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TwinsWidgetBoss"
QT_MOC_LITERAL(1, 16, 16), // "handlePenisSlots"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 17) // "handlePenisSlots2"

    },
    "TwinsWidgetBoss\0handlePenisSlots\0\0"
    "handlePenisSlots2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TwinsWidgetBoss[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    2,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

       0        // eod
};

void TwinsWidgetBoss::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TwinsWidgetBoss *_t = static_cast<TwinsWidgetBoss *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handlePenisSlots(); break;
        case 1: _t->handlePenisSlots2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject TwinsWidgetBoss::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TwinsWidgetBoss.data,
      qt_meta_data_TwinsWidgetBoss,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TwinsWidgetBoss::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TwinsWidgetBoss::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TwinsWidgetBoss.stringdata0))
        return static_cast<void*>(const_cast< TwinsWidgetBoss*>(this));
    return QWidget::qt_metacast(_clname);
}

int TwinsWidgetBoss::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
