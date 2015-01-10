#include <cassert>
#include <QDebug>
#include <QElapsedTimer>
#include <tr-vcomputer/devices/dummy_device.hpp>

#include "computerrun.h"

ComputerRun::ComputerRun(const CPUConfig& cpu_cfg, QObject *parent) :
    QThread(parent),
    paused(true)
{
    this->rom = new trillek::Byte[32*1024];

    this->devices.insert(5, std::make_shared<trillek::computer::tda::TDADev>());
    // Hardwired TDA to slot 5
    this->devices.insert(4, std::make_shared<trillek::computer::gkeyboard::GKeyboardDev>());
    active_keyboard = 4; // Hardwired keyboard to slot 4

    this->devices.insert(10, std::make_shared<trillek::computer::DummyDevice>());
    // Hardwired floppy drive at slot 6
    this->devices.insert(6, std::make_shared<trillek::computer::m5fdd::M5FDD>());


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
    //QThread::run();
    unsigned aux_counter = 0;
    unsigned vsync_counter = 0;
    bool loop = true;
    QElapsedTimer timer;
    timer.start();

    while (loop) {
        m_cmp.lock();
        if (! this->computer->isOn()) {
            m_cmp.unlock();
            loop = false;
            continue;
        }
        m_cmp.unlock();

        if (paused) {
            yieldCurrentThread();
            continue;
        }      

        if (vsync_counter > 40000) { // 1000.000 / 25 = 40000
            vsync_counter -= 40000;
            // VSync screen devices
            QMapIterator<unsigned, DockScreen*> it(this->screens);
            while (it.hasNext()) {
                it.next();
                std::shared_ptr<trillek::computer::tda::TDADev> dev =
                        std::dynamic_pointer_cast<trillek::computer::tda::TDADev>( this->devices[it.key()]);
                dev->DoVSync();
                auto screen = it.value()->getScreen();
                auto ptr_scr = screen.get();
                dev->DumpScreen( *ptr_scr);
            }
        }

        auto deltans = timer.nsecsElapsed();
        timer.start();
        double delta = (double)deltans / 1000000000.0; // Delta time on seconds

        m_cmp.lock();
        auto ticks = this->computer->Update(delta);
        m_cmp.unlock();
        vsync_counter += ticks; aux_counter += ticks;
        // TODO Calc how many ms need to sleep. Should take 50 ms bettwen every iteration
        unsigned long sleeptime = 50000 ; // ticks
        sleeptime = sleeptime < 10000 ? 10000 : sleeptime;
        sleeptime = sleeptime > 100000 ? 100000 : sleeptime;
        if (aux_counter > 800000) { // At ~1,25 Hz
            aux_counter -= 800000;
            //qDebug() << delta << " s Ticks " << ticks << " Sleeptime" << sleeptime << " us";
            const double ttick = (delta / ticks); // Time of a base clock tick
            const double tclk = 1.0 / 1000000.0; // Teorical Base clock tick time (1Mhz)
            calc_speed = (tclk / ttick);
            //qDebug() << "Speed of " << 100.0f * calc_speed << " %";
            emit updateSpeed(calc_speed);
        }

        this->usleep(sleeptime);
    }
}

void ComputerRun::on()
{
    m_cmp.lock();
    this->computer->On();
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
    emit updateSpeed(-1);
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

double ComputerRun::getEstimatedSpeed() const
{
    return calc_speed;
}

bool ComputerRun::processKeyEvent(QKeyEvent *keyEvent)
{
    using namespace trillek::computer::gkeyboard;
    // Get keys that are used by virtual keyboard and send to actual virtual keyboard device
    // Uses keyEvent->text() to get the unicode simbol generated by the keyevent (ie 'A' or 'a', etc)
    auto vkeyboard = std::dynamic_pointer_cast<trillek::computer::gkeyboard::GKeyboardDev>(this->devices[active_keyboard]);

    trillek::Byte status = 0; // Seting status byte
    switch (keyEvent->modifiers()) {
    case Qt::ShiftModifier:
        status |= KEY_MODS::KEY_MOD_SHIFT;
    case Qt::ControlModifier:
        status |= KEY_MODS::KEY_MOD_CTRL;
    case Qt::AltModifier:
        status |= KEY_MODS::KEY_MOD_ALTGR;
    default:
        ;
    }

    // Generate scancode and correct keycode if is necesary
    auto text = keyEvent->text().toUtf8();
    SCANCODES scancode;
    qDebug() << "K: #"<< text << "# " << keyEvent->key();
    switch (keyEvent->key()) {
    case Qt::Key_Space:
        scancode = SCANCODES::SCAN_SPACE;
        text.clear();
        text.append(KEYCODES::KEY_SPACEBAR);
        break;
    case Qt::Key_Apostrophe:
        scancode = SCANCODES::SCAN_APOSTROPHE;
        break;
    case Qt::Key_Comma:
        scancode = SCANCODES::SCAN_COMMA;
        break;
    case Qt::Key_Minus:
        scancode = SCANCODES::SCAN_MINUS;
        break;
    case Qt::Key_Colon:
    case Qt::Key_Period:
        scancode = SCANCODES::SCAN_PERIOD;
        break;
    case Qt::Key_Slash:
        scancode = SCANCODES::SCAN_SLASH;
        break;
    case Qt::Key_0:
        scancode = SCANCODES::SCAN_0;
        break;
    case Qt::Key_1:
        scancode = SCANCODES::SCAN_1;
        break;
    case Qt::Key_2:
        scancode = SCANCODES::SCAN_2;
        break;
    case Qt::Key_3:
        scancode = SCANCODES::SCAN_3;
        break;
    case Qt::Key_4:
        scancode = SCANCODES::SCAN_4;
        break;
    case Qt::Key_5:
        scancode = SCANCODES::SCAN_5;
        break;
    case Qt::Key_6:
        scancode = SCANCODES::SCAN_6;
        break;
    case Qt::Key_7:
        scancode = SCANCODES::SCAN_7;
        break;
    case Qt::Key_8:
        scancode = SCANCODES::SCAN_8;
        break;
    case Qt::Key_9:
        scancode = SCANCODES::SCAN_9;
        break;
    case Qt::Key_Semicolon:
        scancode = SCANCODES::SCAN_SEMICOLON;
        break;
    case Qt::Key_Equal:
        scancode = SCANCODES::SCAN_EQUAL;
        break;
    case Qt::Key_A:
        scancode = SCANCODES::SCAN_A;
        break;
    case Qt::Key_B:
        scancode = SCANCODES::SCAN_B;
        break;
    case Qt::Key_C:
        scancode = SCANCODES::SCAN_C;
        break;
    case Qt::Key_D:
        scancode = SCANCODES::SCAN_D;
        break;
    case Qt::Key_E:
        scancode = SCANCODES::SCAN_E;
        break;
    case Qt::Key_F:
        scancode = SCANCODES::SCAN_F;
        break;
    case Qt::Key_G:
        scancode = SCANCODES::SCAN_G;
        break;
    case Qt::Key_H:
        scancode = SCANCODES::SCAN_H;
        break;
    case Qt::Key_I:
        scancode = SCANCODES::SCAN_I;
        break;
    case Qt::Key_J:
        scancode = SCANCODES::SCAN_J;
        break;
    case Qt::Key_K:
        scancode = SCANCODES::SCAN_K;
        break;
    case Qt::Key_L:
        scancode = SCANCODES::SCAN_L;
        break;
    case Qt::Key_M:
        scancode = SCANCODES::SCAN_M;
        break;
    case Qt::Key_N:
        scancode = SCANCODES::SCAN_N;
        break;
    case Qt::Key_O:
        scancode = SCANCODES::SCAN_O;
        break;
    case Qt::Key_P:
        scancode = SCANCODES::SCAN_P;
        break;
    case Qt::Key_Q:
        scancode = SCANCODES::SCAN_Q;
        break;
    case Qt::Key_R:
        scancode = SCANCODES::SCAN_R;
        break;
    case Qt::Key_S:
        scancode = SCANCODES::SCAN_S;
        break;
    case Qt::Key_T:
        scancode = SCANCODES::SCAN_T;
        break;
    case Qt::Key_U:
        scancode = SCANCODES::SCAN_U;
        break;
    case Qt::Key_V:
        scancode = SCANCODES::SCAN_V;
        break;
    case Qt::Key_W:
        scancode = SCANCODES::SCAN_W;
        break;
    case Qt::Key_X:
        scancode = SCANCODES::SCAN_X;
        break;
    case Qt::Key_Y:
        scancode = SCANCODES::SCAN_Y;
        break;
    case Qt::Key_Z:
        scancode = SCANCODES::SCAN_Z;
        break;
    case Qt::Key_BracketLeft:
        scancode = SCANCODES::SCAN_LEFT_BRACKET;
        break;
    case Qt::Key_Backslash:
        scancode = SCANCODES::SCAN_BACKSLASH;
        break;
    case Qt::Key_BracketRight:
        scancode = SCANCODES::SCAN_RIGHT_BRACKET;
        break;
    case Qt::Key_Agrave: // Grave Acent
        scancode = SCANCODES::SCAN_GRAVE_ACCENT;
        break;
    //  GLFW_KEY_WORLD_1 & 2  maps to ??
    case Qt::Key_Escape:
        scancode = SCANCODES::SCAN_ESCAPE;
        text.clear();
        text.append(KEYCODES::KEY_ESC);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        scancode = SCANCODES::SCAN_ENTER;
        text.clear();
        text.append(KEYCODES::KEY_RETURN);
        break;
    case Qt::Key_Tab:
        scancode = SCANCODES::SCAN_TAB;
        text.clear();
        text.append(KEYCODES::KEY_TAB);
        break;
    case Qt::Key_Backspace:
        scancode = SCANCODES::SCAN_BACKSPACE;
        text.clear();
        text.append(KEYCODES::KEY_BACKSPACE);
        break;
    case Qt::Key_Insert:
        scancode = SCANCODES::SCAN_INSERT;
        text.clear();
        text.append(KEYCODES::KEY_INSERT);
        break;
    case Qt::Key_Delete:
        scancode = SCANCODES::SCAN_DELETE;
        text.clear();
        text.append(KEYCODES::KEY_DELETE);
        break;
    case Qt::Key_Right:
        scancode = SCANCODES::SCAN_RIGHT;
        text.clear();
        text.append(KEYCODES::KEY_ARROW_RIGHT);
        break;
    case Qt::Key_Left:
        scancode = SCANCODES::SCAN_LEFT;
        text.clear();
        text.append(KEYCODES::KEY_ARROW_LEFT);
        break;
    case Qt::Key_Down:
        scancode = SCANCODES::SCAN_DOWN;
        text.clear();
        text.append(KEYCODES::KEY_ARROW_DOWN);
        break;
    case Qt::Key_Up:
        scancode = SCANCODES::SCAN_UP;
        text.clear();
        text.append(KEYCODES::KEY_ARROW_UP);
        break;
    case Qt::Key_Shift:
        scancode = SCANCODES::SCAN_LEFT_SHIFT;
        text.clear();
        text.append(KEYCODES::KEY_SHIFT);
        break;
    case Qt::Key_Control:
        scancode = SCANCODES::SCAN_RIGHT_CONTROL;
        text.clear();
        text.append(KEYCODES::KEY_CONTROL);
        break;
    case Qt::Key_Alt:
        scancode = SCANCODES::SCAN_LEFT_ALT;
        text.clear();
        text.append(KEYCODES::KEY_ALT);
        break;

        // Unknow keys or not capture keys
    case 0:
    case Qt::Key_unknown:
    default:
        return false;
    }


    vkeyboard->EnforceSendKeyEvent(scancode, text.at(0), status );

    return true;
}
