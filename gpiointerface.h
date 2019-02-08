//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_GPIOINTERFACE_H
#define C_CLASS_GPIOINTERFACE_H
class GPIOInterface  {
public:
    virtual ~GPIOInterface() = 0;
    virtual void SetDirection(bool value)=0;
    virtual bool GetDirection()=0;
    virtual bool Reset()=0;
    virtual bool Set()=0;
    virtual bool Toggle()=0;
    virtual bool Read()=0;
private:

protected:
    unsigned int pin;
    bool out;
};

#endif //C_CLASS_GPIOINTERFACE_H
