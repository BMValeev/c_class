#include "screenlight.h"

ScreenLight::ScreenLight()
    : PWM(SCREEN_LIGHT_PIN,
          DEFAULT_SCREEN_LIGHT_POWER*DEFAULT_SCREEN_LIGHT_PWM_PERIOD_MS/100,
          DEFAULT_SCREEN_LIGHT_PWM_PERIOD_MS)
{    
    start(); // Запускаем подсветку через ШИМ
}

bool ScreenLight::set_power(uint8_t power)
{
    return sl.lset_power(power);
}

bool ScreenLight::lset_power(uint8_t power)
{
    if (power > 100)
        return false;

    if (m_power == power)
        return false;

    if (!set_params(power*DEFAULT_SCREEN_LIGHT_PWM_PERIOD_MS/100,
                    DEFAULT_SCREEN_LIGHT_PWM_PERIOD_MS))
        return false;

    m_power = power;
    return true;
}

uint8_t ScreenLight::power()
{
    return sl.m_power;
}
