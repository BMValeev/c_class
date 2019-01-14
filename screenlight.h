#ifndef SCREENLIGHT_H
#define SCREENLIGHT_H

#include <stdint.h>

#include "pwm.h"

#ifndef SCREEN_LIGHT_PIN
    #define SCREEN_LIGHT_PIN 3
#endif

#ifndef DEFAULT_SCREEN_LIGHT_POWER
    #define DEFAULT_SCREEN_LIGHT_POWER 90
#endif

#ifndef DEFAULT_SCREEN_LIGHT_PWM_PERIOD_NS
    #define DEFAULT_SCREEN_LIGHT_PWM_PERIOD_NS 1000000
#endif

class ScreenLight : private PWM
{
public:
    ScreenLight();

    // Текущая мощность, в процентах, от 0 до 100
    uint8_t power_val();
    // Установить мощность, в процентах, от 0 до 100
    bool set_power(uint8_t power);

private:
    uint8_t m_power = DEFAULT_SCREEN_LIGHT_POWER;

};

#endif // SCREENLIGHT_H
