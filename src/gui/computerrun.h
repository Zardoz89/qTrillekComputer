#ifndef COMPUTERRUN_H
#define COMPUTERRUN_H

#include <QThread>
#include <QMutex>
#include <QMap>
#include <QKeyEvent>
#include <memory>
#include <tr-vcomputer/vc.hpp>

#include "dockscreen.h"

struct CPUConfig {
    QString cpu;
    unsigned int clock;
};

// TODO Cada X ciclos base de la maquina virtual, hacer el VSync() y actualizar el TDAScreen
// TDAScreen es un shared_ptr, pero hay que crearlo en algun sitio, y enlazar la pantalla-TDAScreen-dispositivo
// Quizas hacer que TDAScreen sea creado por qTDAScreen y que lo devuelva en un metodo, con lo que solo habria que
// mapear el par pantalla-dispositivo

class ComputerRun : public QThread
{
    Q_OBJECT
public:
    explicit ComputerRun(const CPUConfig &cpu_cfg, QObject *parent = 0);

    ~ComputerRun();

    void run();

    std::unique_ptr<trillek::computer::VComputer> computer;
    QMap<unsigned, std::shared_ptr<trillek::computer::Device>> devices; // To store devices
    QMap<unsigned, DockScreen* > screens; // Links a device with a screen
    unsigned active_keyboard; // Device slot with the active virtual keyboard

    bool isPaused() const
    {
        return this->paused;
    }

    bool isOn() const
    {
        return computer->isOn();
    }

    bool processKeyEvent(QKeyEvent *keyEvent); /// Process a key event. Return true if is a valid vcomputer keyboard key

    double getEstimatedSpeed() const; /// Returs the estimated emulation speed
signals:
    void updateSpeed(double); /// Informs of the estimated emulation speed. -1 if is stoped

public slots:
    void on();
    void off();
    void pause(bool pause);
    void reset();

    void resizeRam(std::size_t ram_size);
    void setCPUConfig(const CPUConfig& cfg);

    void loadROM(const QString& filename);


private:
    void addDevicesToComputer();

    bool paused;
    QMutex m_cmp;

    trillek::Byte* rom;
    double calc_speed;
};

#endif // COMPUTERRUN_H
