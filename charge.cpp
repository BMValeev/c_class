#include "charge.h"
#include "I2C.h"

Charge::Charge(std::string filename, LogCallback cb)
    : m_cb(cb)
{
    // Initialize I2C
    I2C &ptrI2C = I2C::getInstance(m_cb);
    ptrI2C.begin(filename);

    // Initialize STC3115
    if (!STC3115::Init(&m_stc_config, &m_stc_battery_data))
        m_cb(Warning_log,"Failed to initialize STC3115");
}

Charge::~Charge()
{
    STC3115::Stop();
}

uint8_t Charge::read_charge()
{
    switch (STC3115::Task(&m_stc_config, &m_stc_battery_data))
    {
        case -1:
            m_cb(Warning_log,"Failed to read from STC3115");
            return 0;
        case 0:
        case 1:
        default:
            return static_cast<uint8_t>(m_stc_battery_data.SOC/10);
    }
}

bool Charge::read_is_charging()
{
    return false;
}
