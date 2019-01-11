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
    uint8_t power_val();
    // установить мощность, в процентах, от 0 до 100
    bool lset_power(uint8_t power);
    ScreenLight(void);
    // Глобально установить мощность
    bool set_power(uint8_t power); // установить мощность, в процентах, от 0 до 100


private:
    static ScreenLight sl;
    uint8_t m_power = DEFAULT_SCREEN_LIGHT_POWER;
// Статическая часть

};

#endif // SCREENLIGHT_H
