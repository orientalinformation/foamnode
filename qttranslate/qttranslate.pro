#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T11:53:04
#
#-------------------------------------------------

QT       += core gui
target.path = $$PWD/qttranslate
INSTALLS += target

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qttranslate
TEMPLATE = app

TRANSLATIONS = qttranslate.ts

unix {
    qttranslate.commands = "echo translation materials  && cd $$PWD && lrelease qttranslate.pro"
    qttranslate.depends = Makefile

    QMAKE_CLEAN += qttranslate.o
    QMAKE_EXTRA_TARGETS += qttranslate
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
