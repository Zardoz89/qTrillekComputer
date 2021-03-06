#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <memory>
#include <tr-vcomputer/vc.hpp>

#include "dockscreen.h"
#include "computerrun.h"
#include "keypresshook.h"

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
    QLabel* lbl_cpupercent;
    QLabel* lbl_cpuOnOff;
    QPixmap startIcon;
    QPixmap stopIcon;
    QPixmap pauseIcon;

    QList<DockScreen*> vscreens;
    KeyPressHook* keyPressHook;

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

    void updateEmuSpeed(double speed);
};

#endif // MAINWINDOW_H
