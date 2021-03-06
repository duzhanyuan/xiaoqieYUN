#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T13:41:35
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PenguinCloud
TEMPLATE = app
include(basicwidget/basicwidget.pri)
include(basiccontrol/basiccontrol.pri)
include(logic/logic.pri)
include(network/network.pri)
include(tools/tools.pri)
include(thread/thread.pri)
include(file/file.pri)
include(manage/manage.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    loginwidget.cpp \
    mainwidget.cpp \
    registerwidget.cpp \
    stable.cpp

HEADERS  += \
    loginwidget.h \
    mainwidget.h \
    registerwidget.h \
    stable.h

FORMS    +=


RC_FILE += PenguinCloud.rc

RESOURCES += \
    resource.qrc


SUBDIRS += \
    network/network.pro

CONFIG+=precompile_header
PRECOMPILED_HEADER=stable.h

INCLUDEPATH += $$PWD/poppler
win32: LIBS += -L$$PWD/poppler -llibpoppler
win32: LIBS += -L$$PWD/poppler -llibpoppler-qt5

