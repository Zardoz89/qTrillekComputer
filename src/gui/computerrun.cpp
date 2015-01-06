#include <assert.h>
#include <QDebug>

#include "computerrun.h"

ComputerRun::ComputerRun(const CPUConfig& cpu_cfg, QObject *parent) :
    QThread(parent),
    paused(true)
{
    this->rom = new trillek::Byte[32*1024];

    this->devices.insert(3, std::make_shared<trillek::computer::tda::TDADev>());

    this->computer.reset(new trillek::computer::VComputer());
    this->setCPUConfig(cpu_cfg);
    addDevicesToComputer();

    //this->connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ComputerRun::~ComputerRun()
{
    delete[] rom;
}

void ComputerRun::run()
{
    QThread::run();
    while (1) {
        m_cmp.lock();
        if (! this->computer->isOn()) {
            m_cmp.unlock();
            this->exit();
        }
        m_cmp.unlock();

        if (paused) {
            yieldCurrentThread();
            continue;
        }

        m_cmp.lock();
        this->computer->Update(1.0);
        m_cmp.unlock();

        this->msleep(100);
    }
}

void ComputerRun::on()
{
    m_cmp.lock();
    qDebug() << this->computer->haveCpu();
    this->computer->On();
    qDebug() << this->computer->isOn();
    if (this->computer->isOn()) {
        paused = false;
        this->start();
    }
    m_cmp.unlock();
}

void ComputerRun::off()
{
    m_cmp.lock();
    this->computer->Off();
    m_cmp.unlock();
}

void ComputerRun::pause(bool pause)
{
    paused = pause;
}

void ComputerRun::reset()
{
    m_cmp.lock();
    this->computer->Reset();
    m_cmp.unlock();
}

void ComputerRun::resizeRam(std::size_t ram_size)
{
    assert(ram_size > 0);
    m_cmp.lock();
    std::unique_ptr<trillek::computer::ICPU> cpu = std::move(this->computer->RmCPU());
    this->computer.reset(new trillek::computer::VComputer(ram_size));
    this->computer->SetCPU(std::move(cpu));

    addDevicesToComputer();
    m_cmp.unlock();
}

void ComputerRun::setCPUConfig(const CPUConfig& cfg)
{
    m_cmp.lock();
    qDebug() << cfg.cpu << " " << cfg.clock;
    if (cfg.cpu == QString("TR3200") && cfg.clock > 0) {
        using namespace  trillek::computer;
        std::unique_ptr<ICPU> cpu(new TR3200(cfg.clock * 1000) );
        this->computer->SetCPU(std::move(cpu));
    }
    m_cmp.unlock();
}

void ComputerRun::loadROM(const QString& filename)
{
    int size = trillek::computer::LoadROM(filename.toStdString(), this->rom);
    qDebug() << "Read from rom file :" << size << " bytes";
    this->computer->SetROM(rom, size);
}

void ComputerRun::addDevicesToComputer()
{
    QMapIterator<unsigned, std::shared_ptr<trillek::computer::Device>> it(this->devices);
    while (it.hasNext()) {
        it.next();
        this->computer->AddDevice(it.key(), it.value());
    }
}
