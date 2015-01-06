#include "tdawidget.h"
#include <QDebug>

#include <algorithm>
#include <assert.h>

TDAWidget::TDAWidget(QWidget *parent) :
    QLabel(parent),
    screen(nullptr)
{
    timer = new QTimer(this);
    // setup signal and slot
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScreen()));

    this->tdata = new trillek::DWord[320*240];
    this->setMinimumSize(320, 240);
    this->setAlignment(Qt::AlignCenter);

    this->screen = std::shared_ptr<trillek::computer::tda::TDAScreen>(new trillek::computer::tda::TDAScreen());
}

TDAWidget::~TDAWidget()
{
    if (this->tdata != nullptr) {
        delete[] tdata;
    }
}

std::shared_ptr<trillek::computer::tda::TDAScreen> TDAWidget::getScreen ()
{
    return this->screen;
}

void TDAWidget::lockScreen()
{
    mutex.lock();
}

void TDAWidget::unlockScreen()
{
    mutex.unlock();
}

void TDAWidget::updateScreen ()
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

void TDAWidget::start ()
{
    timer->start(40); // 25 Hz
}

void TDAWidget::stop ()
{
    timer->stop();
}
