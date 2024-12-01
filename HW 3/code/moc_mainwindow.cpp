/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../HW3/mainwindow.h"
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
    QByteArrayData data[19];
    char stringdata0[375];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 22), // "on_selectinput_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 31), // "on_comboBox_currentIndexChanged"
QT_MOC_LITERAL(4, 67, 5), // "index"
QT_MOC_LITERAL(5, 73, 22), // "on_convolution_clicked"
QT_MOC_LITERAL(6, 96, 26), // "on_tableWidget_itemChanged"
QT_MOC_LITERAL(7, 123, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(8, 141, 4), // "item"
QT_MOC_LITERAL(9, 146, 26), // "on_gaussian_K_valueChanged"
QT_MOC_LITERAL(10, 173, 4), // "arg1"
QT_MOC_LITERAL(11, 178, 30), // "on_gaussian_sigma_valueChanged"
QT_MOC_LITERAL(12, 209, 4), // "arg2"
QT_MOC_LITERAL(13, 214, 20), // "on_Laplacian_clicked"
QT_MOC_LITERAL(14, 235, 33), // "on_comboBox_2_currentIndexCha..."
QT_MOC_LITERAL(15, 269, 27), // "on_localenhancement_clicked"
QT_MOC_LITERAL(16, 297, 27), // "on_parameter_c_valueChanged"
QT_MOC_LITERAL(17, 325, 25), // "on_sobeloperators_clicked"
QT_MOC_LITERAL(18, 351, 23) // "on_zerocrossing_clicked"

    },
    "MainWindow\0on_selectinput_clicked\0\0"
    "on_comboBox_currentIndexChanged\0index\0"
    "on_convolution_clicked\0"
    "on_tableWidget_itemChanged\0QTableWidgetItem*\0"
    "item\0on_gaussian_K_valueChanged\0arg1\0"
    "on_gaussian_sigma_valueChanged\0arg2\0"
    "on_Laplacian_clicked\0"
    "on_comboBox_2_currentIndexChanged\0"
    "on_localenhancement_clicked\0"
    "on_parameter_c_valueChanged\0"
    "on_sobeloperators_clicked\0"
    "on_zerocrossing_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    1,   75,    2, 0x08 /* Private */,
       5,    0,   78,    2, 0x08 /* Private */,
       6,    1,   79,    2, 0x08 /* Private */,
       9,    1,   82,    2, 0x08 /* Private */,
      11,    1,   85,    2, 0x08 /* Private */,
      13,    0,   88,    2, 0x08 /* Private */,
      14,    1,   89,    2, 0x08 /* Private */,
      15,    0,   92,    2, 0x08 /* Private */,
      16,    1,   93,    2, 0x08 /* Private */,
      17,    0,   96,    2, 0x08 /* Private */,
      18,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
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
        case 0: _t->on_selectinput_clicked(); break;
        case 1: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_convolution_clicked(); break;
        case 3: _t->on_tableWidget_itemChanged((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->on_gaussian_K_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->on_gaussian_sigma_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->on_Laplacian_clicked(); break;
        case 7: _t->on_comboBox_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_localenhancement_clicked(); break;
        case 9: _t->on_parameter_c_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_sobeloperators_clicked(); break;
        case 11: _t->on_zerocrossing_clicked(); break;
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
