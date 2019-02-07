#include "backlight_pwm.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
#define BACKLIGHT_MAIN_PATH "/sys/class/backlight/pwm-backlight"

backlight_driver::backlight_driver(){
    unsigned int max_bright;
    ofstream interface(BACKLIGHT_MAIN_PATH+"/bl_power");
    if(interface.fail()) {
        interface.close();
        throw "There is no such backlight!";
    }
    interface<<'0';
    if(interface.fail()) {
        interface.close();
        throw "backlight is not exported";
    }
    interface<<'1';
    interface.close();
    ifstream max_value(BACKLIGHT_MAIN_PATH+"/max_brightness");
    max_bright<<max_value.get();
    MaxResolution=(unsigned int) max_bright;

}
backlight_driver::~backlight_driver(){
    // Если запущен, останавливаем
    if (IsOn())
        Stop();
}
backlight_driver::SetPower(unsigned int l_power){
    unsigned int value=l_power*MaxResolution/100;
    ofstream interface(BACKLIGHT_MAIN_PATH+"/brightness");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<std::to_string(value);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    power=l_power;
    interface.close();
}
backlight_driver::Start(){
    ofstream interface(PWM_MAIN_PATH+"/bl_power");
    interface<<std::to_string(0);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    m_enable=true;
    return true;
}
backlight_driver::Stop(){
    ofstream interface(PWM_MAIN_PATH+"/bl_power");
    interface<<std::to_string(1);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    m_enable=false;
    return true;
}
