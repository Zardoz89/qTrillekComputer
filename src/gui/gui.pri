INCLUDEPATH += gui
DEPENDPATH += gui

SOURCES += $${PWD}/mainwindow.cpp \
           $${PWD}/dockscreen.cpp \
           $${PWD}/tdawidget.cpp \
           $${PWD}/frmsetupcpu.cpp \
           $${PWD}/cpudialog.cpp \
           $${PWD}/computerrun.cpp

HEADERS += $${PWD}/mainwindow.h \
           $${PWD}/dockscreen.h \
           $${PWD}/tdawidget.h \
           $${PWD}/frmsetupcpu.h \
           $${PWD}/cpudialog.h \
           $${PWD}/computerrun.h

FORMS   += $${PWD}/mainwindow.ui \
           $${PWD}/dockscreen.ui \
           $${PWD}/frmsetupcpu.ui \
           $${PWD}/cpudialog.ui
