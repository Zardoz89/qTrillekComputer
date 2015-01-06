#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <tr-vcomputer/vc.hpp>

#include "dockscreen.h"
#include "computerrun.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    DockScreen* vscreen;

    CPUConfig cpu_config;
    ComputerRun* computer;

signals:

public slots:
    void openROMfile();
    void setupDevices();
    void setupCPU();

    void start();
    void pause(bool pause);
    void stop();
};

#endif // MAINWINDOW_H
