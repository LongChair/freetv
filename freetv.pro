#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T09:32:05
#
#-------------------------------------------------

QT       += core
greaterThan(QT_MAJOR_VERSION, 4): QT += network qml declarative

#DEFINES += TARGET_RPI

TARGET = freetv
target.path = /home/pi
INSTALLS += target

TEMPLATE = app


SOURCES += main.cpp\
    MainView.cpp \
    TvPlayer.cpp

TARGET_RPI {
    SOURCES += DispmanxImage.cpp
}

HEADERS  += \
    MainView.h \
    TvPlayer.h \
    Channel.h

TARGET_RPI {
    SOURCES += DispmanxImage.h
}

FORMS    +=

DISTFILES +=

RESOURCES += \
    ressource.qrc

TARGET_RPI {
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../mnt/rasp-pi-rootfs/opt/vc/lib/release/ -lbcm_host
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../mnt/rasp-pi-rootfs/opt/vc/lib/debug/ -lbcm_host
    else:unix: LIBS += -L$$PWD/../../../mnt/rasp-pi-rootfs/opt/vc/lib/ -lbcm_host

    INCLUDEPATH += $$PWD/../../../mnt/rasp-pi-rootfs/opt/vc/include
    DEPENDPATH += $$PWD/../../../mnt/rasp-pi-rootfs/opt/vc/include
}

