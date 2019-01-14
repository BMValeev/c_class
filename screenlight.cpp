#include "screenlight.h"

ScreenLight::ScreenLight()
    : PWM(SCREEN_LIGHT_PIN,
          DEFAULT_SCREEN_LIGHT_POWER*DEFAULT_SCREEN_LIGHT_PWM_PERIOD_NS/100,
          DEFAULT_SCREEN_LIGHT_PWM_PERIOD_NS)
{    
    start(); // Запускаем подсветку через ШИМ
}

bool ScreenLight::set_power(uint8_t power)
{
    if (power > 100)
        return false;

    if (m_power == power)
        return false;

    if (!set_params(power*DEFAULT_SCREEN_LIGHT_PWM_PERIOD_NS/100,
                    DEFAULT_SCREEN_LIGHT_PWM_PERIOD_NS))
        return false;

    m_power = power;
    return true;
}

uint8_t ScreenLight::power_val()
{
    return m_power;
}

#ifdef C_CLASS_DEBUG
#include <unistd.h>
#include <iostream>
using namespace std;
int main(void)
{
try{
    ScreenLight light;
    cout <<'1';
    light.set_power(100);
    cout <<'2';
    usleep(5*1000*1000);
    cout <<'3';
    light.set_power(0);
    usleep(5*1000*1000);
    cout <<'4';
} catch (const char* msg) {
     cerr << msg << endl;
   }
    return 1;
}
#endif
