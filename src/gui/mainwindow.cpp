#include <QFileDialog>
#include <QDebug>
#include <memory>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpudialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // UI Setup
    ui->setupUi(this);
    lbl_cpuOnOff = new QLabel(this);
    lbl_cpuOnOff->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    startIcon = QIcon::fromTheme("media-playback-start").pixmap(16);
    stopIcon = QIcon::fromTheme("media-playback-stop").pixmap(16);
    pauseIcon = QIcon::fromTheme("media-playback-pause").pixmap(16);
    lbl_cpuOnOff->setPixmap(stopIcon);
    ui->statusbar->addPermanentWidget(lbl_cpuOnOff);


    // Initial setup
    cpu_config.cpu = QString("TR3200");
    cpu_config.clock = 100;
    ui->lbl_cpu->setText(cpu_config.cpu + " @ " + QString::number(cpu_config.clock) + "KHz");

    computer = new ComputerRun(cpu_config, this);
    ui->statusbar->showMessage("Computer OFF");
    ui->lbl_ram->setText(QString::number(128) + QString(" KiB") );

    // Keyboard capture
    this->keyPressHook = new KeyPressHook(computer, this);
    this->installEventFilter(this->keyPressHook);

    // Hardwired virtual screen .. TODO A way to create/delete devices and associated virtual screen for TDA
    vscreens.append( new DockScreen(QString("Screen 0"), this));
    computer->screens.insert(5, vscreens.first() );

    this->addDockWidget(Qt::RightDockWidgetArea, vscreens.first());
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
            this->keyPressHook->setGrab(true); // enable vkeyboard
            this->grabKeyboard();

            ui->action_Start->setEnabled(false);
            ui->actionPause->setEnabled(true);
            ui->action_Stop->setEnabled(true);

            QListIterator<DockScreen*> it(this->vscreens);
            while (it.hasNext()) {
                it.next()->start();
            }
            lbl_cpuOnOff->setPixmap(startIcon);
        } else {
            ui->statusbar->showMessage("Computer OFF");
            this->keyPressHook->setGrab(false); // disables vkeyboard
            this->releaseKeyboard();
            lbl_cpuOnOff->setPixmap(stopIcon);
        }
    }
}

void MainWindow::pause(bool pause)
{
    this->computer->pause(pause);
    if (pause) {
        ui->statusbar->showMessage("Computer ON (Paused)");
        this->keyPressHook->setGrab(false);
        this->releaseKeyboard();

        ui->action_Start->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->action_Stop->setEnabled(true);

        QListIterator<DockScreen*> it(this->vscreens);
        while (it.hasNext()) {
            it.next()->stop();
        }
        lbl_cpuOnOff->setPixmap(pauseIcon);
    } else {
        ui->statusbar->showMessage("Computer ON");
        this->keyPressHook->setGrab(true);
        this->grabKeyboard();

        ui->action_Start->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->action_Stop->setEnabled(true);

        QListIterator<DockScreen*> it(this->vscreens);
        while (it.hasNext()) {
            it.next()->start();
        }
        lbl_cpuOnOff->setPixmap(startIcon);
    }
}

void MainWindow::stop()
{
    this->computer->off();
    ui->statusbar->showMessage("Computer OFF");
    this->keyPressHook->setGrab(false);
    this->releaseKeyboard();

    ui->action_Start->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->action_Stop->setEnabled(false);

    QListIterator<DockScreen*> it(this->vscreens);
    while (it.hasNext()) {
        it.next()->stop();
    }
    lbl_cpuOnOff->setPixmap(stopIcon);
}
