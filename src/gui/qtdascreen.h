#ifndef QTDASCREEN_H
#define QTDASCREEN_H

#include <QLabel>
#include <QTimer>

#include <memory>

#include <tr-vcomputer/devices/tda.hpp>

class qTDAScreen : public QLabel
{
    Q_OBJECT
public:
    explicit qTDAScreen(QWidget *parent = 0);
    ~qTDAScreen();

    /** Asign a new screen */
    void setScreen (std::shared_ptr<trillek::computer::tda::TDAScreen> screen);

    void start ();
    void stop ();

    trillek::DWord* tdata = nullptr;    /// Screen texture data
    std::shared_ptr<trillek::computer::tda::TDAScreen> screen;  /// TDA Screen

signals:

public slots:
    /** Updates screen texture and paints it*/
    void updateScreen ();
private:
    QTimer *timer;
};

#endif // QTDASCREEN_H
