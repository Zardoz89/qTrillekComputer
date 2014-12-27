#include "dockscreen.h"
#include "ui_dockscreen.h"

DockScreen::DockScreen(const QString& title, QWidget* parent) :
    QDockWidget(parent),
    ui(new Ui::DockScreen)
{
    ui->setupUi(this);

    tdaScreen = nullptr;


    tdaScreen = new qTDAScreen(this);

    this->setWidget(tdaScreen);
    this->tdaScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setWindowTitle(title);
}

DockScreen::~DockScreen()
{
    delete ui;
}
