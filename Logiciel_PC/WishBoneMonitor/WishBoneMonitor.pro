#-------------------------------------------------
#
# Project created by QtCreator 2010-09-02T19:31:58
#
#-------------------------------------------------

QT       += core gui

TARGET = WishBoneMonitor
TEMPLATE = app
DEPENDPATH += ./QextSerialBase
QMAKE_LIBDIR += ../WishBoneMonitor/QextSerialBase

CONFIG      += qt thread warn_on

SOURCES += main.cpp\
        mainwindow.cpp \
    wishbonemonitor.cpp \
    controltab.cpp \
    wishboneregister.cpp \
    registerdisplay.cpp \
    addregisterdialog.cpp \
    mailboxdriver.cpp \
    paneldoc.cpp \
    virtualtab.cpp

HEADERS  += mainwindow.h \
    wishbonemonitor.h \
    controltab.h \
    wishboneregister.h \
    registerdisplay.h \
    addregisterdialog.h \
    mailboxdriver.h \
    paneldoc.h \
    virtualtab.h

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_
