#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T07:36:54
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += quickwidgets
QT       += multimedia
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = enVocabulary
TEMPLATE = app
DEFINES += QT_DEBUG
#embed_manifest_exe

unix: LIBS += -L/usr/lib -lpthread
unix: INCLUDEPATH += "/usr/local/include/"
win32: INCLUDEPATH += "C:\Users\weiyang\Downloads\boost_1_66_0"

MOC_DIR = "./Moc"
INCLUDEPATH += "./Inc/"
UI_DIR = "./Res/"
OBJECTS_DIR = "./OBJ"

macx{
    message("compile for mac os x")
    ICON = image/dict.icns
}

win32{
    message("compile for windows")
    RC_FILE = Res/res.rc
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler).
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
Src/main.cpp\
Src/search.cpp\
Src/httpmanager.cpp\
Src/log.cpp\
Src/endata.cpp\
Src/controller.cpp\
Src/mysentences.cpp\
Src/viewmanager.cpp\
Src/collectsentences.cpp\
Src/collectwords.cpp\
Src/qmlfunc.cpp

HEADERS += \
Inc/search.h\
Inc/httpmanager.h\
Inc/log.h\
Inc/endata.h\
Inc/controller.h\
Inc/mysentences.h\
Inc/viewmanager.h\
Inc/collectsentences.h\
Inc/collectwords.h\
Inc/qmlfunc.h

FORMS += \
Res/search.ui\
Res/mysentences.ui\
Res/collectsentences.ui\
Res/collectwords.ui

RESOURCES += \
    image.qrc

DISTFILES +=

