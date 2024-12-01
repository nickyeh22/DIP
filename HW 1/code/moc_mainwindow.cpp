/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../HW1part2/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 22), // "on_pushButton1_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 22), // "on_pushButton2_clicked"
QT_MOC_LITERAL(4, 58, 10), // "printimage"
QT_MOC_LITERAL(5, 69, 11), // "int[64][64]"
QT_MOC_LITERAL(6, 81, 11), // "OutputArray"
QT_MOC_LITERAL(7, 93, 23), // "on_slider1_valueChanged"
QT_MOC_LITERAL(8, 117, 5), // "delta"
QT_MOC_LITERAL(9, 123, 13), // "showhistogram"
QT_MOC_LITERAL(10, 137, 5), // "int[]"
QT_MOC_LITERAL(11, 143, 10), // "histovalue"
QT_MOC_LITERAL(12, 154, 23), // "on_slider2_valueChanged"
QT_MOC_LITERAL(13, 178, 13), // "multiplicator"
QT_MOC_LITERAL(14, 192, 24), // "on_AverageButton_clicked"
QT_MOC_LITERAL(15, 217, 27) // "on_FunctionalButton_clicked"

    },
    "MainWindow\0on_pushButton1_clicked\0\0"
    "on_pushButton2_clicked\0printimage\0"
    "int[64][64]\0OutputArray\0on_slider1_valueChanged\0"
    "delta\0showhistogram\0int[]\0histovalue\0"
    "on_slider2_valueChanged\0multiplicator\0"
    "on_AverageButton_clicked\0"
    "on_FunctionalButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    1,   56,    2, 0x08 /* Private */,
       7,    1,   59,    2, 0x08 /* Private */,
       9,    1,   62,    2, 0x08 /* Private */,
      12,    1,   65,    2, 0x08 /* Private */,
      14,    0,   68,    2, 0x08 /* Private */,
      15,    0,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton1_clicked(); break;
        case 1: _t->on_pushButton2_clicked(); break;
        case 2: _t->printimage((*reinterpret_cast< int(*)[64][64]>(_a[1]))); break;
        case 3: _t->on_slider1_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->showhistogram((*reinterpret_cast< int(*)[]>(_a[1]))); break;
        case 5: _t->on_slider2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_AverageButton_clicked(); break;
        case 7: _t->on_FunctionalButton_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
