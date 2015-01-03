#include "frmsetupcpu.h"
#include "ui_frmsetupcpu.h"

FrmSetupCPU::FrmSetupCPU(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::FrmSetupCPU)
{
    ui->setupUi(this);
}

FrmSetupCPU::~FrmSetupCPU()
{
    delete ui;
}

void FrmSetupCPU::setClock(unsigned khz)
{
    ui->spin_clock->setValue(khz);
}

unsigned FrmSetupCPU::clock()
{
    return ui->spin_clock->value();
}

void FrmSetupCPU::addCpu(const QString& text)
{
    ui->cmb_cpu_type->addItem(text);
}

QString FrmSetupCPU::cpu()
{
   return ui->cmb_cpu_type->currentText();
}
