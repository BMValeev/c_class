//
// Created by eleps on 07.02.19.
//

#include "GPIO.h"
#include "gpiointerface.h"
void SetDirection(GPIOInterface *gpio, bool value) {
    gpio->SetDirection(value);
}
bool GetDirection(GPIOInterface *gpio){
    return gpio->GetDirection();
}
void Reset(GPIOInterface *gpio){
    gpio->Reset();
}
void Set(GPIOInterface *gpio){
    gpio->Set();
}
void Toggle(GPIOInterface *gpio){
    gpio->Toggle();
}
bool Read(GPIOInterface *gpio){
    return gpio->Read();
}