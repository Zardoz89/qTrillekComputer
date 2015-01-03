#ifndef FRMSETUPCPU_H
#define FRMSETUPCPU_H

#include <QGroupBox>

namespace Ui {
class FrmSetupCPU;
}

class FrmSetupCPU : public QGroupBox //QFrame
{
    Q_OBJECT

public:
    explicit FrmSetupCPU(QWidget *parent = 0);
    ~FrmSetupCPU();

    void setClock(unsigned khz);
    unsigned clock();

    void addCpu(const QString& text);
    QString cpu();

private:
    Ui::FrmSetupCPU *ui;
};

#endif // FRMSETUPCPU_H
