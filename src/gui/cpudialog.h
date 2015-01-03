#ifndef CPUDIALOG_H
#define CPUDIALOG_H

#include <QDialog>
#include "frmsetupcpu.h"

namespace Ui {
class CPUDialog;
}

class CPUDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPUDialog(QWidget *parent = 0);
    ~CPUDialog();

    FrmSetupCPU& config();

private:
    Ui::CPUDialog *ui;
};

#endif // CPUDIALOG_H
