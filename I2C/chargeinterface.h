#ifndef CHARGEINTERFACE_H
#define CHARGEINTERFACE_H

#include <stdint.h>

class ChargeInterface
{
public:
    virtual ~ChargeInterface() = 0;
    // Заряд в процентах от 0 до 100
    virtual uint8_t read_charge() = 0;
    // заряжается или нет
    virtual bool read_is_charging() = 0;

};

#endif // CHARGEINTERFACE_H

