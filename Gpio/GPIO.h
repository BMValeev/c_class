//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_GPIO_H
#define C_CLASS_GPIO_H
#include "gpiointerface.h"
class GPIO   {
public:
    void SetDirection(GPIOInterface *gpio,bool value)  ;
    bool GetDirection(GPIOInterface *gpio) ;
    void Reset(GPIOInterface *gpio) ;
    void Set(GPIOInterface *gpio) ;
    void Toggle(GPIOInterface *gpio) ;
    bool Read(GPIOInterface *gpio) ;
private:
protected:
};

#endif //C_CLASS_GPIO_H
