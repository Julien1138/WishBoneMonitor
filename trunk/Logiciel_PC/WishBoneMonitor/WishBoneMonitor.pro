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
    virtualtab.cpp \
    graphtab.cpp

HEADERS  += mainwindow.h \
    wishbonemonitor.h \
    controltab.h \
    wishboneregister.h \
    registerdisplay.h \
    addregisterdialog.h \
    mailboxdriver.h \
    paneldoc.h \
    virtualtab.h \
    graphtab.h

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_

QWT_ROOT = C:\Qwt-5.2.1

include( $${QWT_ROOT}/qwtconfig.pri )

MOC_DIR      = moc
INCLUDEPATH += $${QWT_ROOT}/src
DEPENDPATH  += $${QWT_ROOT}/src
OBJECTS_DIR  = obj

CONFIG(debug, debug|release):QWTLIB = qwtd
else:QWTLIB  = qwt

win32 {
    contains(CONFIG, QwtDll) {
        DEFINES    += QT_DLL QWT_DLL
        QWTLIB = $${QWTLIB}$${VER_MAJ}
    }

    win32-msvc:LIBS  += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc.net:LIBS  += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2002:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2003:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2005:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-msvc2008:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib
    win32-g++:LIBS   += -L$${QWT_ROOT}/lib -l$${QWTLIB}
}
else {
    LIBS        += -L$${QWT_ROOT}/lib -l$${QWTLIB}
}
