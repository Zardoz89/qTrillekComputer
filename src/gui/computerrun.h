#ifndef COMPUTERRUN_H
#define COMPUTERRUN_H

#include <QThread>
#include <QMutex>
#include <tr-vcomputer/vc.hpp>

struct CPUConfig {
    QString cpu;
    unsigned int clock;
};

class ComputerRun : public QThread
{
    Q_OBJECT
public:
    explicit ComputerRun(const CPUConfig &cpu_cfg, QObject *parent = 0);

    ~ComputerRun();

    void run();

    std::unique_ptr<trillek::computer::VComputer> computer;

    bool isPaused()
    {
        return this->paused;
    }

    bool isOn()
    {
        return computer->isOn();
    }

signals:


public slots:
    void on();
    void off();
    void pause(bool pause);
    void reset();

    void resizeRam(std::size_t ram_size);
    void setCPUConfig(const CPUConfig& cfg);

    void loadROM(const QString& filename);

private:
    bool paused;
    QMutex m_cmp;

    trillek::Byte* rom;
};

#endif // COMPUTERRUN_H
