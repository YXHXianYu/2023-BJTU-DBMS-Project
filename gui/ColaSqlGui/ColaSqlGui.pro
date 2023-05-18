QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS += -lstdc++fs

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $${PWD}/../../cmd/src/*.cpp \
    $${PWD}/../../core/src/*.cpp \
    $${PWD}/../../core/src/constraint/*.cpp \
    $${PWD}/../../core/src/index/*.cpp \
    ../../file/src/filemanager.cpp \
#    ./src/*.cpp
    ./src/createindex.cpp \
    ./src/createrecord.cpp \
    ./src/createtable.cpp \
    ./src/mainwindow.cpp \
    ./src/ui_login.cpp \
    ./src/ui_register.cpp \
    ./src/columndialog.cpp \
    ./src/main.cpp

HEADERS += \
#    ./include/*.h \
    ./include/columndialog.h \
    ./include/createindex.h \
    ./include/createrecord.h \
    ./include/createtable.h \
    ./include/mainwindow.h \
    ./include/qtbstreambuf.h \
    ./include/ui_login.h \
    ./include/ui_register.h

FORMS += \
#    ./ui/*.h
    ./ui/createindex.ui \
    ./ui/createrecord.ui \
    ./ui/createtable.ui \
    ./ui/mainwindow.ui \
    ./ui/ui_login.ui \
    ./ui/ui_register.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=$$quote("../../cmd/include")
INCLUDEPATH +=$$quote("../../core/include")
INCLUDEPATH +=$$quote("../../core/include/constraint")
INCLUDEPATH +=$$quote("../../core/include/index")
INCLUDEPATH +=$$quote("../../file/include/")
INCLUDEPATH +=$$quote("./include")

RESOURCES += \
    resource/res.qrc



