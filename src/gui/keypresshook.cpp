#include "keypresshook.h"
#include <QKeyEvent>
#include <QDebug>
#include <cassert>

KeyPressHook::KeyPressHook(ComputerRun* computer, QObject *parent) :
    QObject(parent),
    grabKeyboard(false)
{
    assert(computer != nullptr);
    this->computer = computer;
}

bool KeyPressHook::eventFilter(QObject *obj, QEvent *event)
{
    if (grabKeyboard && event->type() == QEvent::KeyPress ) {
        // Capturing keyboard. Send to the computer to be procesed and perhaps stolen
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        return this->computer->processKeyEvent(keyEvent) | QObject::eventFilter(obj, event);
    } else {
        // Standard event procesing
        return QObject::eventFilter(obj, event);
    }
}

void KeyPressHook::toogleGrab()
{
    this->grabKeyboard = ! this->grabKeyboard;
}

void KeyPressHook::setGrab(bool grab)
{
    this->grabKeyboard = grab;
}
