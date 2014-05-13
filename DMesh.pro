#-------------------------------------------------
#
# Project created by QtCreator 2013-08-20T15:59:57
#
#-------------------------------------------------

QT       += core gui opengl network webkit

TARGET = DMesh
TEMPLATE = app


SOURCES += main.cpp \
    Core_Fuctions/snappy_dmesh.cpp \
    Core_Fuctions/openfoam.cpp \
    Core_Fuctions/mythread.cpp \
    Core_Fuctions/dmesh.cpp \
    Core_Fuctions/block_dmesh.cpp \
    DMesh_Widget/w_meshsurfacerefinegeneral.cpp \
    DMesh_Widget/w_meshboundrefinegeneral.cpp \
    mainwindow.cpp \
    Core_Fuctions/patchdict.cpp \
    DMesh_Widget/w_meshstlrefineadvance.cpp \
    License_Function/smtp.cpp \
    License_Function/sendmail.cpp \
    License_Function/registerdialog.cpp \
    License_Function/previewdialog.cpp \
    License_Function/clsencryption.cpp

HEADERS  += \
    Core_Fuctions/snappy_dmesh.h \
    Core_Fuctions/openfoam.h \
    Core_Fuctions/mythread.h \
    Core_Fuctions/dmesh.h \
    Core_Fuctions/block_dmesh.h \
    DMesh_Widget/w_meshsurfacerefinegeneral.h \
    DMesh_Widget/w_meshboundrefinegeneral.h \
    mainwindow.h \
    Core_Fuctions/patchdict.h \
    DMesh_Widget/w_meshstlrefineadvance.h \
    License_Function/smtp.h \
    License_Function/sendmail.h \
    License_Function/registerdialog.h \
    License_Function/previewdialog.h \
    License_Function/clsencryption.h

FORMS += \
    DMesh_Widget/w_meshsurfacerefinegeneral.ui \
    DMesh_Widget/w_meshboundrefinegeneral.ui \
    mainwindow.ui \
    DMesh_Widget/w_meshstlrefineadvance.ui \
    License_Function/sendmail.ui \
    License_Function/registerdialog.ui \
    License_Function/previewdialog.ui

RESOURCES += \
    resource.qrc

RC_FILE += \
    setIcon.rc
