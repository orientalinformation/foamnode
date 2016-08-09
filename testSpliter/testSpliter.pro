#-------------------------------------------------
#
# Project created by QtCreator 2015-09-30T08:39:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testSpliter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    foamcase.cpp \
    foamnode.cpp

HEADERS  += mainwindow.h \
    foamcase.h \
    foamnode.h \
    openfoam_global.h

FORMS    += mainwindow.ui
