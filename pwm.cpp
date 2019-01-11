#include "pwm.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#define PWM_MAIN_PATH "/sys/class/pwm/pwmchip"


PWM::PWM(uint16_t pin, uint32_t duty_cycle_ns, uint32_t period_ns)
    : m_pin(pin)
    , m_period_ns(period_ns)
    , m_duty_cycle_ns(duty_cycle_ns)
{
    ofstream interface(PWM_MAIN_PATH+std::to_string(this->pin())+"/export");
    if(interface.fail()) {
        interface.close();
        throw "There is no such pwmchip!";
    }
    interface<<'0';
    if(interface.fail()) {
        interface.close();
        throw "Chip is not exported";
    }
    interface.close();
    interface.open(PWM_MAIN_PATH+std::to_string(this->pin())+"/pwm0/period");
    if(interface.fail())
    {
        interface.close();
    }
    interface<<std::to_string(m_period_ns);
    if(interface.fail()) {
        interface.close();
    }
    interface.close();
    interface.open(PWM_MAIN_PATH+std::to_string(this->pin())+"/pwm0/duty_cycle");
    if(interface.fail()) {
        interface.close();
    }
    interface<<std::to_string(m_duty_cycle_ns);
    if(interface.fail()) {
        interface.close();
    }
    interface.close();
    interface.open(PWM_MAIN_PATH+std::to_string(this->pin())+"/pwm0/enable");
    if(interface.fail()) {
        interface.close();
    }
    if (is_on()) {
        interface<<'1';
    }
    else {
        interface<<'0';
    }
    if(interface.fail()) {
        interface.close();
    }
    interface.close();
}

PWM::~PWM()
{
    // Если запущен, останавливаем
    if (is_on())
        stop();
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin())+"/unexport");
    if(interface.fail())
    {
        interface.close();
    }
    interface<<'0';
}

bool PWM::set_params(uint32_t duty_cycle_ns, uint32_t period_ns)
{
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin())+"/pwm0/period");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<std::to_string(period_ns);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    m_period_ns=period_ns;
    interface.close();
    interface.open(PWM_MAIN_PATH+std::to_string(pin())+"/pwm0/duty_cycle");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<std::to_string(duty_cycle_ns);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    m_duty_cycle_ns=duty_cycle_ns;
    interface.close();
    return true;
}

bool PWM::start()
{
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin())+"/pwm0/enable");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<'1';
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface.close();
    m_enable=true;
    return true;
}

bool PWM::stop()
{
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin())+"/pwm0/enable");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<'0';
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface.close();
    m_enable=false;
    return true;
}

bool PWM::is_on() const
{
    return m_enable;
}

uint32_t PWM::period_ns() const
{
    return m_period_ns;
}

uint32_t PWM::duty_cycle_ns() const
{
    return m_duty_cycle_ns;
}
uint16_t PWM::pin() const
{
    return m_pin;
}
