/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Controller_t {
    QByteArrayData data[15];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Controller_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Controller_t qt_meta_stringdata_Controller = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Controller"
QT_MOC_LITERAL(1, 11, 7), // "getWord"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 10), // "closeAllUI"
QT_MOC_LITERAL(4, 31, 20), // "gotoWordSentencesWnd"
QT_MOC_LITERAL(5, 52, 17), // "gotoCollectSenWnd"
QT_MOC_LITERAL(6, 70, 18), // "gotoCollectWordWnd"
QT_MOC_LITERAL(7, 89, 20), // "SentenceWinToHomeWnd"
QT_MOC_LITERAL(8, 110, 22), // "CollectSenWndToHomeWnd"
QT_MOC_LITERAL(9, 133, 23), // "CollectWordWndToHomeWnd"
QT_MOC_LITERAL(10, 157, 20), // "moveEventHappenedSig"
QT_MOC_LITERAL(11, 178, 7), // "srcEnum"
QT_MOC_LITERAL(12, 186, 1), // "e"
QT_MOC_LITERAL(13, 188, 8), // "newPoint"
QT_MOC_LITERAL(14, 197, 25) // "updateCollectSentencePage"

    },
    "Controller\0getWord\0\0closeAllUI\0"
    "gotoWordSentencesWnd\0gotoCollectSenWnd\0"
    "gotoCollectWordWnd\0SentenceWinToHomeWnd\0"
    "CollectSenWndToHomeWnd\0CollectWordWndToHomeWnd\0"
    "moveEventHappenedSig\0srcEnum\0e\0newPoint\0"
    "updateCollectSentencePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,
       7,    0,   69,    2, 0x06 /* Public */,
       8,    0,   70,    2, 0x06 /* Public */,
       9,    0,   71,    2, 0x06 /* Public */,
      10,    2,   72,    2, 0x06 /* Public */,
      14,    0,   77,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QPoint,   12,   13,
    QMetaType::Void,

       0        // eod
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Controller *_t = static_cast<Controller *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getWord(); break;
        case 1: _t->closeAllUI(); break;
        case 2: _t->gotoWordSentencesWnd(); break;
        case 3: _t->gotoCollectSenWnd(); break;
        case 4: _t->gotoCollectWordWnd(); break;
        case 5: _t->SentenceWinToHomeWnd(); break;
        case 6: _t->CollectSenWndToHomeWnd(); break;
        case 7: _t->CollectWordWndToHomeWnd(); break;
        case 8: _t->moveEventHappenedSig((*reinterpret_cast< srcEnum(*)>(_a[1])),(*reinterpret_cast< QPoint(*)>(_a[2]))); break;
        case 9: _t->updateCollectSentencePage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::getWord)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::closeAllUI)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::gotoWordSentencesWnd)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::gotoCollectSenWnd)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::gotoCollectWordWnd)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::SentenceWinToHomeWnd)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::CollectSenWndToHomeWnd)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::CollectWordWndToHomeWnd)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(srcEnum , QPoint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::moveEventHappenedSig)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Controller::updateCollectSentencePage)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject Controller::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Controller.data,
      qt_meta_data_Controller,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Controller.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Controller::getWord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Controller::closeAllUI()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Controller::gotoWordSentencesWnd()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Controller::gotoCollectSenWnd()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Controller::gotoCollectWordWnd()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Controller::SentenceWinToHomeWnd()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Controller::CollectSenWndToHomeWnd()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Controller::CollectWordWndToHomeWnd()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Controller::moveEventHappenedSig(srcEnum _t1, QPoint _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Controller::updateCollectSentencePage()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
