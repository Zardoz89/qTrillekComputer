INCLUDEPATH += gui
DEPENDPATH += gui

SOURCES += $${PWD}/mainwindow.cpp \
           $${PWD}/dockscreen.cpp \
           $${PWD}/qtdascreen.cpp \
           $${PWD}/frmsetupcpu.cpp \
    src/gui/cpudialog.cpp

HEADERS += $${PWD}/mainwindow.h \
           $${PWD}/dockscreen.h \
           $${PWD}/qtdascreen.h \
           $${PWD}/frmsetupcpu.h \
    src/gui/cpudialog.h

FORMS   += $${PWD}/mainwindow.ui \
           $${PWD}/dockscreen.ui \
           $${PWD}/frmsetupcpu.ui \
    src/gui/cpudialog.ui
