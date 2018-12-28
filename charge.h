#ifndef CHARGE_H
#define CHARGE_H

#include <stdint.h>

#include "chargeinterface.h"

class Charge : public ChargeInterface
{
public:
    Charge();

    // Заряд в процентах от 0 до 100
    uint8_t read_charge() override;
    // заряжается или нет
    bool read_is_charging() override;

};

#endif // CHARGE_H
