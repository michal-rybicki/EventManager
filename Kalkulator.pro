#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T18:21:09
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QTPLUGIN += QSQLMYSQL
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kalkulator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        wystawy.cpp \
    nowestoisko.cpp \
    nowymagazyn.cpp \
    edycjamagazynu.cpp \
    edycjastoisk.cpp \
    tools.cpp \
    edycjawystawy.cpp \
    historia.cpp \
    wystawazbazy.cpp


HEADERS += \
    mainwindow.h \
    wystawy.h \
    nowestoisko.h \
    nowymagazyn.h \
    edycjamagazynu.h \
    edycjastoisk.h \
    tools.h \
    edycjawystawy.h \
    historia.h \
    wystawazbazy.h


FORMS += \
        mainwindow.ui \
    nowestoisko.ui \
    nowymagazyn.ui \
    edycjamagazynu.ui \
    edycjastoisk.ui \
    edycjawystawy.ui \
    historia.ui \
    wystawazbazy.ui

