#include <QFileDialog>
#include <QDebug>
#include <memory>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpudialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vscreen(nullptr)
{
    ui->setupUi(this);

    cpu_config.cpu = QString("TR3200");
    cpu_config.clock = 100;
    ui->lbl_cpu->setText(cpu_config.cpu + " @ " + QString::number(cpu_config.clock) + "KHz");

    computer = new ComputerRun(cpu_config, this);
    ui->statusbar->showMessage("Computer OFF");
    ui->lbl_ram->setText(QString::number(128) + QString(" KiB") );

    vscreen = new DockScreen(QString("Screen 0"), this);
    computer->screens.insert(3, vscreen);
    vscreen->start();

    this->addDockWidget(Qt::RightDockWidgetArea, vscreen);
    //this->tabifiedDockWidgets();


}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::openROMfile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open ROM file"), QDir::currentPath(), tr("ROM Files (*.bin *.rom *.ffi);;Any file (*)") );
    qDebug() << "Romfile ="<< filename;
    computer->loadROM(filename);

}

void MainWindow::setupDevices()
{

}

void MainWindow::setupCPU()
{
    using namespace trillek::computer;
    CPUDialog* dialog = new CPUDialog(this);
    dialog->config().addCpu("TR3200");
    if (dialog->exec() == QDialog::Accepted) {
        auto cpu = dialog->config().cpu();
        qDebug() << "CPU = " << cpu << "\tCLOCK = " << dialog->config().clock();

        this->cpu_config.cpu = cpu;
        this->cpu_config.clock = dialog->config().clock();
        ui->lbl_cpu->setText(cpu_config.cpu + " @ " + QString::number(cpu_config.clock) + "KHz");

        computer->setCPUConfig(cpu_config);
    }

}

void MainWindow::start()
{
    if (! this->computer->isOn()) {
        this->computer->on();
        if (this->computer->isOn()) {
            ui->statusbar->showMessage("Computer ON");
            ui->action_Start->setEnabled(false);
            ui->actionPause->setEnabled(true);
            ui->action_Stop->setEnabled(true);
        } else {
            ui->statusbar->showMessage("Computer OFF");
        }
    }
}

void MainWindow::pause(bool pause)
{
    this->computer->pause(pause);
    if (pause) {
        ui->statusbar->showMessage("Computer ON (Paused)");
        ui->action_Start->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->action_Stop->setEnabled(true);
    } else {
        ui->statusbar->showMessage("Computer ON");
        ui->action_Start->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->action_Stop->setEnabled(true);
    }
}

void MainWindow::stop()
{
    this->computer->off();
    ui->statusbar->showMessage("Computer OFF");
    ui->action_Start->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->action_Stop->setEnabled(false);
}
