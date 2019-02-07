//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_GPIO_H
#define C_CLASS_GPIO_H
#include "gpiointerface.h"
class GPIO :  {
public:
    void SetDirection(bool value) ;
    bool GetDirection();
    void Reset();
    void Set();
    void Toggle();
    bool Read();
private:
protected:
};

#endif //C_CLASS_GPIO_H
