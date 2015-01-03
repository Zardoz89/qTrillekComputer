#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <tr-vcomputer/vc.hpp>

#include "dockscreen.h"

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

    DockScreen* screens;

    std::unique_ptr<trillek::computer::VComputer> computer;
    trillek::Byte* rom;

signals:

public slots:
    void openROMfile();
    void setupDevices();
    void setupCPU();
};

#endif // MAINWINDOW_H
