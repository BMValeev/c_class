#ifndef I2CPACKET_H
#define I2CPACKET_H

#include <stdint.h>
#include <vector>

#include "../defs.h"
#include "../Rest/loggable.h"

#define I2C_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class I2CPacket : public Loggable
{
public:
    I2CPacket(std::string filename, uint8_t addr, LogCallback cb = printToCout);

    // Address
    uint8_t getAddress() const { return mAddr; }

protected:
    // Helpers
    std::vector<uint8_t> writeArray(uint8_t cmd,
                                    std::vector<uint8_t> data,
                                    uint32_t ansLen,
                                    int attempts = I2C_PACKET_TRANSACTION_ATTEMPTS_NUMBER);

private:
    const uint8_t mAddr;

};

#endif // I2CPACKET_H
