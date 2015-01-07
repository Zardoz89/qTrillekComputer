#ifndef KEYPRESSHOOK_H
#define KEYPRESSHOOK_H

#include <QObject>
#include <QEvent>

class KeyPressHook : public QObject
{
    Q_OBJECT
public:
    explicit KeyPressHook(QObject *parent = 0);

signals:

public slots:

protected:
     bool eventFilter(QObject *obj, QEvent *event);

     bool grabKeyboard;
};

#endif // KEYPRESSHOOK_H
