#ifndef SPIPACKET_H
#define SPIPACKET_H

#include <vector>

#include "../defs.h"
#include "../Rest/crc.h"

#define SPI_PACKET_MAX_TX_SIZE 16384
#define SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

// Class that implements the most of the SPI protocol logic (currently used in ESU)
// It constructs transmit buffer with length byte, command byte, payload and CRC8
// The several helper functions are also available to form common types of payload
// The "send" function implements repeated send logic, "transaction" calls SPI functions
class SPIPacket
{
public:
    SPIPacket(std::string deviceName, LogCallback cb = printToCout);
    virtual ~SPIPacket() { }

private:
    const LogCallback mCb;
    const std::string mDeviceName;

    // Helpers
    static uint8_t lenToByte(uint64_t len);
    static uint16_t byteToLen(uint8_t byte);

protected:
    void printLog(uint8_t status, std::string text) const;
    static void printToCout(uint8_t status, std::string msg);

    // Helpers
    virtual uint8_t getRxCnt(uint8_t) const;
    uint8_t sendBool(uint8_t cmd, bool value, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t send1Uint8(uint8_t cmd, uint8_t value, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t send2Uint8(uint8_t cmd, uint8_t value1, uint8_t value2, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t send1Uint16(uint8_t cmd, uint16_t value, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t send2Uint16(uint8_t cmd, uint16_t value1, uint16_t value2, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t send(uint8_t cmd, std::vector<uint8_t>& txMsg, std::vector<uint8_t>& answer, int attempts) const;
    // SPI is only used in this function, so possible to reimplement over another protocol
    uint8_t transaction(uint8_t cmd, std::vector<uint8_t>& txMsg, std::vector<uint8_t>& rxMsg, uint8_t rxLen) const;

};

#endif // SPIPACKET_H
