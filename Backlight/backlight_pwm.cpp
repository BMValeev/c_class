#include "backlight_pwm.h"
#include <fstream>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cerrno>
#include <cstdlib>

using namespace std;
#define PWM_MAIN_PATH "/sys/class/pwm/pwmchip"

backlight_pwm::backlight_pwm(unsigned int l_pin){
    pin=l_pin;
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin)+"/export");
    if(interface.fail()) {
        interface.close();
        throw "There is no such pwmchip!";
    }
    interface<<'0';
    if(interface.fail()) {
        interface.close();
        throw "Chip is not exported";
    }
    interface.close();
    SetPeriod(DEFAULT_PERIOD_NS);
    SetPower(DEFAULT_POWER);
    Start();
}
backlight_pwm::~backlight_pwm(){
    // Если запущен, останавливаем
    if (IsOn())
        Stop();
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin)+"/unexport");
    if(interface.fail())
    {
        interface.close();
    }
    interface<<'0';
}
bool backlight_pwm::SetPower(unsigned int l_power){
	uint32_t value=l_power*period/100;
	SetDuty(value);
	power=l_power;
	duty=value;
    return true;
}
bool backlight_pwm::Start(){
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin)+"/pwm0/enable");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<'1';
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface.close();
    m_enable=true;
    return true;	
}
bool backlight_pwm::Stop(){
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin)+"/pwm0/enable");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<'0';
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface.close();
    m_enable=false;
    return true;
}
bool backlight_pwm::SetPeriod(unsigned int l_period) {
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin)+"/pwm0/period");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<std::to_string(l_period);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    period=l_period;
    interface.close();
    return true;
}
bool backlight_pwm::SetDuty(unsigned int l_duty){
    ofstream interface(PWM_MAIN_PATH+std::to_string(pin)+"/pwm0/duty_cycle");
    if(interface.fail()) {
        interface.close();
        return false;
    }
    interface<<std::to_string(l_duty);
    if(interface.fail()) {
        interface.close();
        return false;
    }
    duty=l_duty;
    interface.close();
    return true;
}
