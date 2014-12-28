#include "dockscreen.h"
#include "ui_dockscreen.h"

#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>

DockScreen::DockScreen(const QString& title, QWidget* parent) :
    QDockWidget(parent),
    ui(new Ui::DockScreen)
{
    ui->setupUi(this);

    tdaScreen = ui->screen;

    this->setWindowTitle(title);

    QAction* actToggleFullScreen = new QAction(tdaScreen);
    actToggleFullScreen->setShortcut(Qt::Key_F11);
    this->connect(actToggleFullScreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()) );
    tdaScreen->addAction(actToggleFullScreen);
    this->addAction(actToggleFullScreen);
}

DockScreen::~DockScreen()
{
    delete ui;
}


void DockScreen::setFullScreen (bool fullscreen)
{
    if (fullscreen) {
        this->oldDockFlags = this->windowFlags();
        this->oldStyleSheet = this->styleSheet();
        this->oldGeometry = this->geometry();

        this->setWidget(new QLabel("h", this)); // Avoid the destruction of this
        this->hide();
        ui->container->setParent(nullptr);
        ui->container->showFullScreen();
    } else {
        this->setWindowFlags(oldDockFlags);
        this->setStyleSheet(oldStyleSheet);
        this->setGeometry(oldGeometry);
        this->setWidget(ui->container);
        ui->container->setParent(this);

        ui->container->show();
        this->show();
    }
}


void DockScreen::toggleFullScreen () {
   if (ui->container->isFullScreen()) {
       this->setFullScreen(false);
   } else {
       this->setFullScreen(true);
   }
}
