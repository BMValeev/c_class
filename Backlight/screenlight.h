#ifndef SCREENLIGHT_H
#define SCREENLIGHT_H
#include <stdint.h>

#include "backlightinterface.h"
class ScreenLight
{
public:
    unsigned int GetPower(BackLightInterface *backlight);
    void SetPower(BackLightInterface *backlight, unsigned int l_power);
    void SetState(BackLightInterface *backlight, bool l_enable);
    bool GetState(BackLightInterface *backlight);
    void SetMaxPower(unsigned int value);

private:
    unsigned int max_power = 100;
};
#endif // SCREENLIGHT_H
