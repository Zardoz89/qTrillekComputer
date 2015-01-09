#ifndef KEYPRESSHOOK_H
#define KEYPRESSHOOK_H

#include <QObject>
#include <QEvent>
#include "computerrun.h"

class KeyPressHook : public QObject
{
    Q_OBJECT
public:
    explicit KeyPressHook(ComputerRun* computer, QObject *parent = 0);

signals:

public slots:
    void toogleGrab();          /// Toogles keyboard stealing
    void setGrab(bool grab);    /// Enables/disables keyboard stealing

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    bool grabKeyboard;

    ComputerRun* computer;
};

#endif // KEYPRESSHOOK_H
