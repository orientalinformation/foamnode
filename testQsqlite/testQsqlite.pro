#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T08:41:15
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(/home/dongtp/Workspaces/QxOrm/QxOrm.pri)

TARGET = testQsqlite
TEMPLATE = app

INCLUDEPATH += /home/dongtp/Workspaces/MaterialEntities/include \
               /home/dongtp/Workspaces/QxOrm/include

LIBS += -L/home/dongtp/Workspaces/MaterialEntities/bin \
        -L/home/dongtp/Workspaces/QxOrm/lib

CONFIG(debug, debug|release) {
LIBS += -l"QxOrmd"
} else {
LIBS += -l"QxOrm"
} # CONFIG(debug, debug|release)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
