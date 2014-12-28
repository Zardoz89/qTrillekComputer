#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mwindow;
    mwindow.show();

    return a.exec();
}
