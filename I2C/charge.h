#ifndef CHARGE_H
#define CHARGE_H

#include <vector>
#include <string>

#include "chargeinterface.h"
#include "../defs.h"
#include "stc3115_Driver.h"

class Charge : public ChargeInterface
{
public:
    Charge(std::string filename, LogCallback cb);
    ~Charge() override;

    // Заряд в процентах от 0 до 100
    uint8_t read_charge() override;
    // заряжается или нет
    bool read_is_charging() override;

private:
    LogCallback m_cb;
    STC3115_ConfigData_TypeDef m_stc_config;
    STC3115_BatteryData_TypeDef m_stc_battery_data;
};

#endif // CHARGE_H
