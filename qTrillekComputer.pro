#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T12:44:37
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = qTrillekComputer
TEMPLATE = app


SOURCES += main.cpp \
    qtdascreen.cpp

HEADERS  += \
    qtdascreen.h

FORMS    +=

unix|win32: LIBS += -L/usr/local/lib -lVCOMPUTER
INCLUDEPATH +=/usr/local/lib
