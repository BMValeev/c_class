#ifndef SCREENLIGHT_H
#define SCREENLIGHT_H
#include <stdint.h>

#include "backlightinterface.h"
class ScreenLight
{
public:
    unsigned int  GetPower(BackLightInterface *backlight);
    void SetPower(BackLightInterface *backlight,unsigned int l_power);
    void SetState(BackLightInterface *backlight,bool l_enable);
    bool GetState(BackLightInterface *backlight);
private:
};
#endif // SCREENLIGHT_H
