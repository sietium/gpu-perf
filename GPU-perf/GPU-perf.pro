#-------------------------------------------------
#
# Project created by QtCreator 2021-02-07T12:22:14
#
#-------------------------------------------------

QT       += core gui sql network opengl
#QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPU-perf
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        gpuperf.cpp \
    database.cpp \
    infomag.cpp \
    syscheck.cpp \
    smtp.cpp

HEADERS += \
        gpuperf.h \
    database.h \
    common_def.h \
    infomag.h \
    syscheck.h \
    smtp.h

FORMS += \
        gpuperf.ui

LIBS += -lglut

DISTFILES += \
    info.txt \

RESOURCES += \
    res.qrc
