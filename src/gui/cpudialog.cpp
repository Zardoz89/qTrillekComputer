#include "cpudialog.h"
#include "ui_cpudialog.h"

CPUDialog::CPUDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPUDialog)
{
    ui->setupUi(this);
}

CPUDialog::~CPUDialog()
{
    delete ui;
}

FrmSetupCPU& CPUDialog::config()
{
    return *(this->ui->frame);
}
