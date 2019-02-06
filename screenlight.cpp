#include "screenlight.h"

unsigned int  GetPower(BackLightInterface *backlight){
    return backlight->GetPower();
}
void SetPower(BackLightInterface *backlight,unsigned int l_power){
    backlight->SetPower(l_power);
}
void SetState(BackLightInterface *backlight,bool l_enable){
    if(l_enable){
        backlight->Start();
    }
    else{
        backlight->Stop();
    }
}
bool GetState(BackLightInterface *backlight){
    return backlight->IsOn();
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
