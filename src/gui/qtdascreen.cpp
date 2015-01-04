#include "qtdascreen.h"
#include <QDebug>

#include <algorithm>
#include <assert.h>

qTDAScreen::qTDAScreen(QWidget *parent) :
    QLabel(parent),
    screen(nullptr)
{
    timer = new QTimer(this);
    // setup signal and slot
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScreen()));

    this->tdata = new trillek::DWord[320*240];
    this->setMinimumSize(320, 240);
    this->setAlignment(Qt::AlignCenter);
}

qTDAScreen::~qTDAScreen()
{
    if (this->tdata != nullptr) {
        delete[] tdata;
    }
}

void qTDAScreen::setScreen (std::shared_ptr<trillek::computer::tda::TDAScreen> screen)
{
    mutex.lock();
    this->screen = screen;
    mutex.unlock();
}

void qTDAScreen::updateScreen ()
{
    assert(tdata != nullptr);
    if (screen) {
        mutex.lock();
        trillek::computer::tda::TDAtoRGBATexture(*screen.get(), tdata);
        mutex.unlock();
        // We interchanged B and R components
        for (unsigned i=0; i < 320*240 ; i++) {
            trillek::DWord g_a   = tdata[i] & 0xFF00FF00;
            trillek::DWord red   = tdata[i] & 0x000000FF;
            trillek::DWord blue  = tdata[i] & 0x00FF0000;
            tdata[i]  = g_a | (red << 16) | (blue >> 16);
        }

        QImage img((unsigned char*)tdata, 320, 240, QImage::Format_ARGB32 );
        QPixmap pixmap = QPixmap::fromImage(img);
        this->setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
    }

}

void qTDAScreen::start ()
{
    timer->start(40); // 25 Hz
}

void qTDAScreen::stop ()
{
    timer->stop();
}
