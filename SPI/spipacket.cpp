#include "spipacket.h"

/* This file contains platform dependent functions of SPIPacket class.
 * It should be included by client application in any case, the code here
 * should be cross-platform. The platform dependent code is contained in
 * spipacketplatform.cpp, whereas it's platform independent version is in
 * spipacketdemo.cpp.
 */

#include <cassert>

#include "MCU.h" // this is only for debug mode to form answer, probably remove later

uint8_t SPIPacket::lenToByte(uint64_t len)
{
    return (len > 127)? static_cast<uint8_t>( len/128 + len%128? 1 : 0)
                      : static_cast<uint8_t>(len);
}

uint16_t SPIPacket::byteToLen(uint8_t byte)
{
    return (byte & 0x80)? (byte & 0x7F)*128 : (byte & 0x7F);
}

uint8_t SPIPacket::getRxCnt(uint8_t) const
{
    return 0;
}

uint8_t SPIPacket::sendBool(uint8_t cmd, bool value, int attempts) const
{
    // printLog(Debug_log, static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload, answer;

    payload.push_back(value? 0x01 : 0x00);

    return send(cmd, payload, answer, attempts);
}

uint8_t SPIPacket::send1Uint16(uint8_t cmd, uint16_t value, int attempts) const
{
    // printLog(Debug_log, static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload, answer;

    payload.push_back(value  & 0xff);
    payload.push_back(value >> 8);

    return send(cmd, payload, answer, attempts);
}

uint8_t SPIPacket::send1Uint8(uint8_t cmd, uint8_t value, int attempts) const
{
    // printLog(Debug_log, static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload, answer;

    payload.push_back(value);

    return send(cmd, payload, answer, attempts);
}

uint8_t SPIPacket::send2Uint8(uint8_t cmd, uint8_t value1, uint8_t value2, int attempts) const
{
    // printLog(Debug_log, static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload, answer;

    payload.push_back(value1);
    payload.push_back(value2);

    return send(cmd, payload, answer, attempts);
}

uint8_t SPIPacket::send2Uint16(uint8_t cmd, uint16_t value1, uint16_t value2, int attempts) const
{
    // printLog(Debug_log, static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload, answer;

    payload.push_back(value1  & 0xff);
    payload.push_back(value1 >> 8);
    payload.push_back(value2  & 0xff);
    payload.push_back(value2 >> 8);

    return send(cmd, payload, answer, attempts);
}
