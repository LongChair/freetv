#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T09:32:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network core quickwidgets qml

TARGET = freetv
target.path = /home/pi
INSTALLS += target

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    +=

DISTFILES +=

RESOURCES += \
    ressource.qrc
