#include <QFileDialog>
#include <QDebug>
#include <memory>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpudialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    screens(nullptr)
{
    ui->setupUi(this);

    cpu_config.cpu = QString("TR3200");
    cpu_config.clock = 100;
    ui->lbl_cpu->setText(cpu_config.cpu + " @ " + QString::number(cpu_config.clock) + "KHz");

    computer = new ComputerRun(cpu_config, this);
    ui->statusbar->showMessage("Computer OFF");
    ui->lbl_ram->setText(QString::number(128) + QString(" KiB") );
/*
    auto screen = std::shared_ptr<trillek::computer::tda::TDAScreen>(new trillek::computer::tda::TDAScreen({0}));
    screen->txt_buffer[0]  = 0x0F00 | 'H';
    screen->txt_buffer[1]  = 0x1F00 | 'e';
    screen->txt_buffer[2]  = 0x2F00 | 'l';
    screen->txt_buffer[3]  = 0x3F00 | 'l';
    screen->txt_buffer[4]  = 0x4F00 | 'o';
    screen->txt_buffer[5]  = 0x5F00 | ' ';
    screen->txt_buffer[6]  = 0x6F00 | 'w';
    screen->txt_buffer[7]  = 0x7F00 | 'o';
    screen->txt_buffer[8]  = 0x8F00 | 'r';
    screen->txt_buffer[9]  = 0x9F00 | 'l';
    screen->txt_buffer[10] = 0xAF00 | 'd';
    screen->txt_buffer[11] = 0xBF00 | '!';

    screen->cursor = true;
    screen->cur_col = 20;
    screen->cur_row = 0;
    screen->cur_start = 5;
    screen->cur_end = 7;
    screen->cur_color = 15;

    for (unsigned i= 40; i < trillek::computer::tda::WIDTH_CHARS * trillek::computer::tda::HEIGHT_CHARS; i++ ) {
        trillek::Byte fg = i % 16;
        trillek::Byte bg = (15 - i) % 16;
        screen->txt_buffer[i] = (bg << 12) | (fg << 8) | ((i-40) % 256);
    }
*/
    screens = new DockScreen(QString("Screen 0"), this);
    //screens->setScreen(screen);
    //screens->start();

    this->addDockWidget(Qt::RightDockWidgetArea, screens);
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
