#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    screens(nullptr)
{
    ui->setupUi(this);

    screens = new DockScreen(QString("Screen 0"), this);
    screens->tdaScreen->start();

    this->addDockWidget(Qt::RightDockWidgetArea, screens);
    //this->tabifiedDockWidgets();


}

MainWindow::~MainWindow()
{
    delete ui;
}
