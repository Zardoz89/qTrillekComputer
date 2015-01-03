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

    this->computer.reset(new trillek::computer::VComputer());
    this->rom = rom = new trillek::Byte[32*1024];
    ui->lbl_ram->setText(QString::number(this->computer->RamSize() / 1024) + QString(" KiB") );

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

    screens = new DockScreen(QString("Screen 0"), this);
    screens->setScreen(screen);
    screens->start();

    this->addDockWidget(Qt::RightDockWidgetArea, screens);
    //this->tabifiedDockWidgets();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete[] rom;
}

void MainWindow::openROMfile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM file"), QDir::currentPath(), tr("ROM Files (*.bin *.rom *.ffi);;Any file (*)") );
    qDebug() << "Romfile ="<< fileName;
    int size = trillek::computer::LoadROM(fileName.toStdString(), this->rom);
    qDebug() << "Read from rom file :" << size << " bytes";
    this->computer->SetROM(rom, size);
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

        if (cpu.compare(QString("TR3200")) && dialog->config().clock() > 0) {
            std::unique_ptr<ICPU> cpu(new TR3200(dialog->config().clock() * 100) );
            this->computer->SetCPU(std::move(cpu));
        }
    }

}
