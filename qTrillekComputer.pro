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

QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/main.cpp

include(src/gui/gui.pri)
# qt-json
include(qt-json/qt-json.pri)

unix|win32: LIBS += -L/usr/local/lib -lVCOMPUTER
INCLUDEPATH +=/usr/local/lib
INCLUDEPATH +=src/gui


