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
    virtual void Reset()=0;
    virtual void Set()=0;
    virtual void Toggle()=0;
    virtual bool Read()=0;
private:
    unsigned int pin;
    bool out;
protected:
};

#endif //C_CLASS_GPIOINTERFACE_H
