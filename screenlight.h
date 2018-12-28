#ifndef SCREENLIGHT_H
#define SCREENLIGHT_H

#include <stdint.h>

#include "pwm.h"

#ifndef SCREEN_LIGHT_PIN
    #define SCREEN_LIGHT_PIN 1
#endif

#ifndef DEFAULT_SCREEN_LIGHT_POWER
    #define DEFAULT_SCREEN_LIGHT_POWER 100
#endif

#ifndef DEFAULT_SCREEN_LIGHT_PWM_PERIOD_MS
    #define DEFAULT_SCREEN_LIGHT_PWM_PERIOD_MS 100
#endif

class ScreenLight : private PWM
{
private:
    ScreenLight();
    uint8_t m_power = DEFAULT_SCREEN_LIGHT_POWER;

    bool lset_power(uint8_t power); // установить мощность, в процентах, от 0 до 100

// Статическая часть
public:
    // Глобально установить мощность
    static bool set_power(uint8_t power); // установить мощность, в процентах, от 0 до 100
    static uint8_t power();

private:
    static ScreenLight sl;
};

#endif // SCREENLIGHT_H
