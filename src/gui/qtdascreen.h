#ifndef QTDASCREEN_H
#define QTDASCREEN_H

#include <QLabel>
#include <QTimer>

#include <tr-vcomputer/devices/tda.hpp>

class qTDAScreen : public QLabel
{
    Q_OBJECT
public:
    explicit qTDAScreen(QWidget *parent = 0);
    ~qTDAScreen();

    /** Asign a new screen */
    void setScreen (const trillek::computer::tda::TDAScreen& screen);

    void start ();
    void stop ();

    trillek::DWord* tdata = nullptr;    /// Screen texture data
    const trillek::computer::tda::TDAScreen* screen = nullptr;    /// TDA Screen

signals:

public slots:
    /** Updates screen texture and paints it*/
    void updateScreen ();
private:
    QTimer *timer;
};

#endif // QTDASCREEN_H
