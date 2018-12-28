#include "pwm.h"

PWM::PWM(uint16_t pin)
    : m_pin(pin)
{ }

PWM::PWM(uint16_t pin, uint32_t duty_cycle_ms, uint32_t period_ms)
    : m_pin(pin)
    , m_period_ms(period_ms)
    , m_duty_cycle_ms(duty_cycle_ms)
{ }

PWM::~PWM()
{
    // Чистка, остановка и тд
}

bool PWM::set_params(uint32_t duty_cycle_ms, uint32_t period_ms)
{
    return true;
}

bool PWM::start() const
{
    return true;
}

bool PWM::stop() const
{
    return true;
}

bool PWM::is_on() const
{
    return true;
}

uint32_t PWM::period_ms() const
{
    return m_period_ms;
}

uint32_t PWM::duty_cycle_ms() const
{
    return m_duty_cycle_ms;
}
