/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../lib/ui/PcapReplayer/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[49];
    char stringdata0[733];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "SetCurProcess"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "nCurProcess"
QT_MOC_LITERAL(4, 38, 21), // "Signal_ToUIState_Stop"
QT_MOC_LITERAL(5, 60, 18), // "SetCurSendingSpeed"
QT_MOC_LITERAL(6, 79, 9), // "nCurSpeed"
QT_MOC_LITERAL(7, 89, 20), // "SetCurSendingPktTime"
QT_MOC_LITERAL(8, 110, 10), // "strPktTime"
QT_MOC_LITERAL(9, 121, 21), // "SetNextSendingPktTime"
QT_MOC_LITERAL(10, 143, 22), // "ShowNextSendingPktTime"
QT_MOC_LITERAL(11, 166, 5), // "bShow"
QT_MOC_LITERAL(12, 172, 12), // "SetNextPktNo"
QT_MOC_LITERAL(13, 185, 9), // "unNextPkt"
QT_MOC_LITERAL(14, 195, 23), // "StartPlayFromAutoDriver"
QT_MOC_LITERAL(15, 219, 23), // "StartInitialFirstScreen"
QT_MOC_LITERAL(16, 243, 16), // "ReadNextPcapFile"
QT_MOC_LITERAL(17, 260, 12), // "SetLoopCount"
QT_MOC_LITERAL(18, 273, 11), // "unLoopCount"
QT_MOC_LITERAL(19, 285, 11), // "onOpen_File"
QT_MOC_LITERAL(20, 297, 13), // "onOpen_Folder"
QT_MOC_LITERAL(21, 311, 20), // "onListWidget_Actived"
QT_MOC_LITERAL(22, 332, 5), // "index"
QT_MOC_LITERAL(23, 338, 6), // "onPlay"
QT_MOC_LITERAL(24, 345, 7), // "onPause"
QT_MOC_LITERAL(25, 353, 6), // "onStop"
QT_MOC_LITERAL(26, 360, 8), // "onRemove"
QT_MOC_LITERAL(27, 369, 7), // "onClose"
QT_MOC_LITERAL(28, 377, 16), // "UpdateCurProcess"
QT_MOC_LITERAL(29, 394, 14), // "ToUIState_Stop"
QT_MOC_LITERAL(30, 409, 14), // "UpdateCurSpeed"
QT_MOC_LITERAL(31, 424, 23), // "UpdateCurSendingPktTime"
QT_MOC_LITERAL(32, 448, 24), // "UpdateNextSendingPktTime"
QT_MOC_LITERAL(33, 473, 28), // "UpdateShowNextSendingPktTime"
QT_MOC_LITERAL(34, 502, 11), // "onScheduler"
QT_MOC_LITERAL(35, 514, 15), // "UpdateNextPktNo"
QT_MOC_LITERAL(36, 530, 13), // "onSave_Config"
QT_MOC_LITERAL(37, 544, 13), // "onLoad_Config"
QT_MOC_LITERAL(38, 558, 19), // "Scheduler_onConfirm"
QT_MOC_LITERAL(39, 578, 15), // "onRegularReplay"
QT_MOC_LITERAL(40, 594, 16), // "Init_FirstScreen"
QT_MOC_LITERAL(41, 611, 11), // "onRemoveAll"
QT_MOC_LITERAL(42, 623, 18), // "SelectNextPcapFile"
QT_MOC_LITERAL(43, 642, 11), // "onAddSrcMap"
QT_MOC_LITERAL(44, 654, 11), // "onAddDstMap"
QT_MOC_LITERAL(45, 666, 16), // "onRemoveScrMapIP"
QT_MOC_LITERAL(46, 683, 16), // "onRemoveDstMapIP"
QT_MOC_LITERAL(47, 700, 15), // "ConvertPcapFile"
QT_MOC_LITERAL(48, 716, 16) // "SetLoopCountSlot"

    },
    "MainWindow\0SetCurProcess\0\0nCurProcess\0"
    "Signal_ToUIState_Stop\0SetCurSendingSpeed\0"
    "nCurSpeed\0SetCurSendingPktTime\0"
    "strPktTime\0SetNextSendingPktTime\0"
    "ShowNextSendingPktTime\0bShow\0SetNextPktNo\0"
    "unNextPkt\0StartPlayFromAutoDriver\0"
    "StartInitialFirstScreen\0ReadNextPcapFile\0"
    "SetLoopCount\0unLoopCount\0onOpen_File\0"
    "onOpen_Folder\0onListWidget_Actived\0"
    "index\0onPlay\0onPause\0onStop\0onRemove\0"
    "onClose\0UpdateCurProcess\0ToUIState_Stop\0"
    "UpdateCurSpeed\0UpdateCurSendingPktTime\0"
    "UpdateNextSendingPktTime\0"
    "UpdateShowNextSendingPktTime\0onScheduler\0"
    "UpdateNextPktNo\0onSave_Config\0"
    "onLoad_Config\0Scheduler_onConfirm\0"
    "onRegularReplay\0Init_FirstScreen\0"
    "onRemoveAll\0SelectNextPcapFile\0"
    "onAddSrcMap\0onAddDstMap\0onRemoveScrMapIP\0"
    "onRemoveDstMapIP\0ConvertPcapFile\0"
    "SetLoopCountSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  214,    2, 0x06 /* Public */,
       4,    0,  217,    2, 0x06 /* Public */,
       5,    1,  218,    2, 0x06 /* Public */,
       7,    1,  221,    2, 0x06 /* Public */,
       9,    1,  224,    2, 0x06 /* Public */,
      10,    1,  227,    2, 0x06 /* Public */,
      12,    1,  230,    2, 0x06 /* Public */,
      14,    0,  233,    2, 0x06 /* Public */,
      15,    0,  234,    2, 0x06 /* Public */,
      16,    0,  235,    2, 0x06 /* Public */,
      17,    1,  236,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    0,  239,    2, 0x08 /* Private */,
      20,    0,  240,    2, 0x08 /* Private */,
      21,    1,  241,    2, 0x08 /* Private */,
      23,    0,  244,    2, 0x08 /* Private */,
      24,    0,  245,    2, 0x08 /* Private */,
      25,    0,  246,    2, 0x08 /* Private */,
      26,    0,  247,    2, 0x08 /* Private */,
      27,    0,  248,    2, 0x08 /* Private */,
      28,    1,  249,    2, 0x08 /* Private */,
      29,    0,  252,    2, 0x08 /* Private */,
      30,    1,  253,    2, 0x08 /* Private */,
      31,    1,  256,    2, 0x08 /* Private */,
      32,    1,  259,    2, 0x08 /* Private */,
      33,    1,  262,    2, 0x08 /* Private */,
      34,    0,  265,    2, 0x08 /* Private */,
      35,    1,  266,    2, 0x08 /* Private */,
      36,    0,  269,    2, 0x08 /* Private */,
      37,    0,  270,    2, 0x08 /* Private */,
      38,    0,  271,    2, 0x08 /* Private */,
      39,    0,  272,    2, 0x08 /* Private */,
      40,    0,  273,    2, 0x08 /* Private */,
      41,    0,  274,    2, 0x08 /* Private */,
      42,    0,  275,    2, 0x08 /* Private */,
      43,    0,  276,    2, 0x08 /* Private */,
      44,    0,  277,    2, 0x08 /* Private */,
      45,    0,  278,    2, 0x08 /* Private */,
      46,    0,  279,    2, 0x08 /* Private */,
      47,    0,  280,    2, 0x08 /* Private */,
      48,    1,  281,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::UInt,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,   18,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,   18,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetCurProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->Signal_ToUIState_Stop(); break;
        case 2: _t->SetCurSendingSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SetCurSendingPktTime((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->SetNextSendingPktTime((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->ShowNextSendingPktTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->SetNextPktNo((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 7: _t->StartPlayFromAutoDriver(); break;
        case 8: _t->StartInitialFirstScreen(); break;
        case 9: _t->ReadNextPcapFile(); break;
        case 10: _t->SetLoopCount((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 11: _t->onOpen_File(); break;
        case 12: _t->onOpen_Folder(); break;
        case 13: _t->onListWidget_Actived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->onPlay(); break;
        case 15: _t->onPause(); break;
        case 16: _t->onStop(); break;
        case 17: _t->onRemove(); break;
        case 18: _t->onClose(); break;
        case 19: _t->UpdateCurProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->ToUIState_Stop(); break;
        case 21: _t->UpdateCurSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->UpdateCurSendingPktTime((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->UpdateNextSendingPktTime((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->UpdateShowNextSendingPktTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->onScheduler(); break;
        case 26: _t->UpdateNextPktNo((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 27: _t->onSave_Config(); break;
        case 28: _t->onLoad_Config(); break;
        case 29: _t->Scheduler_onConfirm(); break;
        case 30: _t->onRegularReplay(); break;
        case 31: _t->Init_FirstScreen(); break;
        case 32: _t->onRemoveAll(); break;
        case 33: _t->SelectNextPcapFile(); break;
        case 34: _t->onAddSrcMap(); break;
        case 35: _t->onAddDstMap(); break;
        case 36: _t->onRemoveScrMapIP(); break;
        case 37: _t->onRemoveDstMapIP(); break;
        case 38: _t->ConvertPcapFile(); break;
        case 39: _t->SetLoopCountSlot((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::SetCurProcess)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::Signal_ToUIState_Stop)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::SetCurSendingSpeed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::SetCurSendingPktTime)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::SetNextSendingPktTime)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::ShowNextSendingPktTime)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::SetNextPktNo)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::StartPlayFromAutoDriver)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::StartInitialFirstScreen)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::ReadNextPcapFile)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(quint32 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::SetLoopCount)) {
                *result = 10;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 40;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::SetCurProcess(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::Signal_ToUIState_Stop()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MainWindow::SetCurSendingSpeed(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::SetCurSendingPktTime(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::SetNextSendingPktTime(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::ShowNextSendingPktTime(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::SetNextPktNo(unsigned int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::StartPlayFromAutoDriver()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void MainWindow::StartInitialFirstScreen()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}

// SIGNAL 9
void MainWindow::ReadNextPcapFile()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}

// SIGNAL 10
void MainWindow::SetLoopCount(quint32 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_END_MOC_NAMESPACE
