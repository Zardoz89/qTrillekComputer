#ifndef DOCKSCREEN_H
#define DOCKSCREEN_H

#include <QDockWidget>
#include "qtdascreen.h"



namespace Ui {
class DockScreen;
}

class DockScreen : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockScreen(QWidget *parent = 0);
    ~DockScreen();

    trillek::computer::tda::TDAScreen screen; // Test screen
    qTDAScreen* tdaScreen;
private:
    Ui::DockScreen *ui;
};

#endif // DOCKSCREEN_H
