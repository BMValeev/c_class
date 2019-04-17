#include "spipacket.h"

#include <cassert>

#include "MCU.h" // this is only for debug mode to form answer, probably remove later

SPIPacket::SPIPacket(std::string deviceName, LogCallback cb)
    : mCb(cb)
    , mDeviceName(deviceName)
{ }

uint8_t SPIPacket::send(uint8_t cmd, std::vector<uint8_t> &payload, std::vector<uint8_t> &answer, int attempts) const
{
    std::ignore = payload;
    std::ignore = attempts;
    // This is DEMO branch
    printLog(Debug_log, static_cast<std::string>(__func__) + " DEMO answer");
    answer.clear();
    if (cmd == MCU::REQUEST_STATUS) // answer to any command other than REQUEST_STATUS
    { // answer to REQUEST_STATUS
        answer.push_back(0x00);
        answer.push_back(0x00);
    }
    return OK_SPI;
}

uint8_t SPIPacket::transaction(uint8_t cmd, std::vector<uint8_t> &txMsg, std::vector<uint8_t> &rxMsg, uint8_t rxLen) const
{
    std::ignore = cmd;
    std::ignore = txMsg;
    std::ignore = rxMsg;
    std::ignore = rxLen;
    assert(txMsg.size() < SPI_PACKET_MAX_TX_SIZE);
    return OK_SPI;
}
