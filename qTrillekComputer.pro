#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T12:44:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = qTrillekComputer
TEMPLATE = app

unix:QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/main.cpp

include(src/gui/gui.pri)
# qt-json
include(qt-json/qt-json.pri)

INCLUDEPATH +=src/gui

unix:INCLUDEPATH +=/usr/local/lib
#unix:DEPENDPATH += /usr/local/lib
win32:INCLUDEPATH += "C:/Program Files (x86)/VCOMPUTER/include/"
win32:DEPENDPATH += "C:/Program Files (x86)/VCOMPUTER/include/"

win32-g++: PRE_TARGETDEPS += "C:/Program Files (x86)/VCOMPUTER/lib/static/libVCOMPUTER.a"
else:win32: PRE_TARGETDEPS += "C:/Program Files (x86)/VCOMPUTER/lib/static/VCOMPUTER.lib"
#else:win32: PRE_TARGETDEPS += "C:/Program Files (x86)/VCOMPUTER/bin/VCOMPUTER.dll"


unix: LIBS += -L/usr/local/lib -lVCOMPUTER
#win32: LIBS += "-LC:/Program Files (x86)/VCOMPUTER/bin/VCOMPUTER.dll"
win32: LIBS += "-LC:/Program Files (x86)/VCOMPUTER/lib/static/" -lVCOMPUTER
