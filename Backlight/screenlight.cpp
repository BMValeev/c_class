#include "screenlight.h"

unsigned int  ScreenLight::GetPower(BackLightInterface *backlight){
    unsigned int value=backlight->GetPower();
    return value*100/max_power;
}

void ScreenLight::SetPower(BackLightInterface *backlight,unsigned int l_power){
    l_power=(l_power*max_power)/100;
    if (l_power<2){
        l_power=2;
    }
    backlight->SetPower(l_power);
}

void ScreenLight::SetState(BackLightInterface *backlight,bool l_enable){
    if(l_enable){
        backlight->Start();
    }
    else{
        backlight->Stop();
    }
}

bool ScreenLight::GetState(BackLightInterface *backlight){
    return backlight->IsOn();
}

void ScreenLight::SetMaxPower(unsigned int value){
    if (value<10 || value >100){
        return;
    }
    this->max_power = value;
}

#ifdef C_CLASS_DEBUG
#include <unistd.h>
#include <iostream>
#include "backlight_pwm.h"
#include "backlight_driver.h"
#include "screenlight.h"
using namespace std;
int main(void)
{
try{
    backlight_pwm pwm(2);
    backlight_driver driver;
    ScreenLight light;
    cout <<'1';
    light.SetState(&pwm, true);
    cout <<'2';
    usleep(5*1000*1000);
    cout <<'3';
    light.SetPower(&pwm,(unsigned int) 0);
    usleep(5*1000*1000);
    cout <<'4';
    light.SetPower(&pwm,(unsigned int) 100);
    usleep(5*1000*1000);
    cout <<'5';
    light.SetPower(&pwm,(unsigned int) 0);
    usleep(5*1000*1000);
    cout <<'6';
    light.SetState(&driver, true);
    cout <<'2';
    usleep(5*1000*1000);
    cout <<'3';
    light.SetPower(&driver,(unsigned int) 0);
    usleep(5*1000*1000);
    cout <<'4';
    light.SetPower(&driver,(unsigned int) 100);
    usleep(5*1000*1000);
    cout <<'5';
    light.SetPower(&driver,(unsigned int) 0);
    usleep(5*1000*1000);
    cout <<'6';
} catch (const char* msg) {
     cerr << msg << endl;
   }
    return 1;
}
#endif // C_CLASS_DEBUG
