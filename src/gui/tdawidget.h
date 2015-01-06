#ifndef QTDASCREEN_H
#define QTDASCREEN_H

#include <QLabel>
#include <QTimer>
#include <QMutex>

#include <memory>

#include <tr-vcomputer/devices/tda.hpp>

class TDAWidget : public QLabel
{
    Q_OBJECT
public:
    explicit TDAWidget(QWidget *parent = 0);
    ~TDAWidget();

    /** Get a shared_ptr to screen state to render */
    std::shared_ptr<trillek::computer::tda::TDAScreen> getScreen ();
    void lockScreen();
    void unlockScreen();

    void start ();
    void stop ();

signals:

public slots:
    /** Updates screen texture and paints it*/
    void updateScreen ();
private:
    trillek::DWord* tdata = nullptr;    /// Screen texture data
    std::shared_ptr<trillek::computer::tda::TDAScreen> screen;  /// TDA Screen
    QTimer *timer;
    QMutex mutex;
};

#endif // QTDASCREEN_H
