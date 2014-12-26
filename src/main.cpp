#include <QApplication>

#include "gui/mainwindow.h"
//#include "qtdascreen.h"


//trillek::computer::tda::TDAScreen screen = {0}; // Test screen

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mwindow;
    mwindow.show();
    //scr.tdaScreen->start();

    return a.exec();
}
