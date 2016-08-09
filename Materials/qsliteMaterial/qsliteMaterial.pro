
QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qsliteMaterial
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    materialstable.cpp \
    materials.cpp

HEADERS  += mainwindow.h \
    database.h \
    materialstable.h \
    materials.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Materials/bin/ -lmaterial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Materials/bin/ -lmateriald
#else:unix: LIBS += -L$$PWD/../Materials/bin/ -lmaterial

INCLUDEPATH += $$PWD/../Materials/include
DEPENDPATH += $$PWD/../Materials/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../QxOrm/lib/ -lQxOrm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QxOrm/lib/ -lQxOrmd
else:unix: LIBS += -L$$PWD/../QxOrm/lib/ -lQxOrm

INCLUDEPATH += $$PWD/../QxOrm/include
DEPENDPATH += $$PWD/../QxOrm/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Materials/bin/release/ -lmateriald
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Materials/bin/debug/ -lmateriald
else:unix:!macx: LIBS += -L$$PWD/../Materials/bin/ -lmateriald

win32{
INCLUDEPATH += $$PWD/../boost_1_57/include
DEPENDPATH += $$PWD/../boost_1_57/include

 LIBS += -L$$PWD/../boost_1_57/lib_shared_64b/
 LIBS += -lboost_serialization-vc120-mt-1_57 \
        -lboost_serialization-vc120-mt-gd-1_57 \
        -lboost_wserialization-vc120-mt-1_57 \
        -lboost_wserialization-vc120-mt-gd-1_57
}

unix{
INCLUDEPATH += $$PWD/../libboost1.57/include
DEPENDPATH += $$PWD/../libboost1.57/include
 LIBS += -L$$PWD/../libboost1.57/lib/
 LIBS += -lboost_wserialization \
         -lboost_serialization
 PRE_TARGETDEPS += $$PWD/../libboost1.57/lib/libboost_wserialization.a
 PRE_TARGETDEPS += $$PWD/../libboost1.57/lib/libboost_serialization.a
}
