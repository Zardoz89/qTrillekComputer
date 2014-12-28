#ifndef DOCKSCREEN_H
#define DOCKSCREEN_H

#include <QDockWidget>
#include <QRect>
#include <QString>
#include <memory>
#include <assert.h>

#include "qtdascreen.h"

namespace Ui {
class DockScreen;
}

/** DockWidget that displays a TDA Screen */
class DockScreen : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockScreen(const QString& title, QWidget *parent = 0);
    ~DockScreen();

private:
    Ui::DockScreen *ui;
    qTDAScreen* tdaScreen;

    Qt::WindowFlags oldDockFlags;
    QString oldStyleSheet;
    QRect oldGeometry;
signals:

public slots:

    /** Sets the TDA Screen to show */
    void setScreen (std::shared_ptr<trillek::computer::tda::TDAScreen> screen)
    {
      assert(tdaScreen != nullptr);
      tdaScreen->setScreen(screen);
    }

    /** Auto updates the texture at 25 Hz */
    void start ()
    {
      assert(tdaScreen != nullptr);
      tdaScreen->start();
    }

    /** Disables auto update of the texture */
    void stop ()
    {
      assert(tdaScreen != nullptr);
      tdaScreen->stop();
    }

    /** Enables/Disables full screen mode */
    void setFullScreen (bool fullscreen);

    /** Toggles between fullscreen and normal mode*/
    void toggleFullScreen ();
};

#endif // DOCKSCREEN_H
