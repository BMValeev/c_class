#include "spipacket.h"

#include <cassert>

#include "MCU.h" // this is only for debug mode to form answer, probably remove later

SPIPacket::SPIPacket(std::string deviceName, LogCallback cb)
    : mCb(cb)
    , mDeviceName(deviceName)
{
    SPI::getInstance().begin(deviceName, cb);
}

uint8_t SPIPacket::lenToByte(uint64_t len)
{
    assert(len < 16384);

    if (len > 127)
        return static_cast<uint8_t>( len/128 + len%128? 1 : 0);

    return static_cast<uint8_t>(len);
}

uint16_t SPIPacket::byteToLen(uint8_t byte)
{
    if (byte & 0x80)
        return (byte & 0x7F)*128;
    else
        return (byte & 0x7F);
}

void SPIPacket::printLog(uint8_t status, std::string text) const
{
    if (mCb != nullptr)
    {
        mCb(status,text);
    }
}

void SPIPacket::printToCout(uint8_t status, std::string msg)
{
    std::cout << status << msg << std::endl;
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

uint8_t SPIPacket::getRxCnt(uint8_t) const
{
    return 0;
}

uint8_t SPIPacket::send(uint8_t cmd, std::vector<uint8_t> &payload, std::vector<uint8_t> &answer, int attempts) const
{
#ifdef DEBUG
    // This is debug branch, works only if SPI was not properly initialized
    if (!SPI::getInstance().isHardwareInitialized())
    {
        printLog(Debug_log, static_cast<std::string>(__func__) + " debug branch answer");
        answer.clear();
        if (cmd == MCU::REQUEST_STATUS) // answer to any command other than REQUEST_STATUS
        { // answer to REQUEST_STATUS
            answer.push_back(0x00);
            answer.push_back(0x00);
        }
        return OK_SPI;
    }
#endif

    // Get answer length
    uint8_t rxLen = getRxCnt(cmd);

    while(attempts--)
    {
        printLog(Debug_log, static_cast<std::string>(__func__) + " transactions left: " + std::to_string(attempts));

        // Send message
        if (transaction(cmd, payload, answer, rxLen))
            continue;

        if (answer.size() != rxLen-1)
            continue;

        answer.erase(answer.begin()); // remove length byte

        if (answer.front() != (ACK_SPI|EXEC_SPI))
        {
            if (attempts == 0)
            {
                if ( (answer.front() & ACK_SPI) != ACK_SPI )
                {
                    return NACK; // not acknowledged
                }
                else
                {
                    return NOK_SPI; // not executed
                }
            } else {
                continue;
            }
        }

        // Acknowledged & executed
        answer.erase(answer.begin()); // Remove acknowledge byte
        printLog(Debug_log, static_cast<std::string>(__func__) + " - success");
        return OK_SPI;
    }
    return TR_ERR_SPI;
}

uint8_t SPIPacket::transaction(uint8_t cmd, std::vector<uint8_t> &txMsg, std::vector<uint8_t> &rxMsg, uint8_t rxLen) const
{
    assert(txMsg.size() < SPI_PACKET_MAX_TX_SIZE);

    printLog(Debug_log, static_cast<std::string>(__func__) + " started");

    // If the msg is less than 128 bytes, just use it's length
    // If the msg is larger, set length to multiples of 128
    uint64_t txLen = txMsg.size() + 3;
    uint64_t txFullLen = txLen < 128? txLen : 128*(txLen/128 + txLen%128? 1 : 0);

    // Allocate transmit buffer
    std::vector<uint8_t> tx_buffer(txFullLen,0); // already initialized to 0

    // Set length
    tx_buffer[0] = lenToByte(txFullLen);
    tx_buffer[1] = cmd;
    // Fill in the message
    std::copy(txMsg.begin(), txMsg.end(), tx_buffer.begin()+2);
    // Add CRC8
    tx_buffer[txFullLen - 1] = CRC::crc8(tx_buffer.data(), txFullLen - 1);

    // Send
    uint8_t r = SPI::getInstance().transaction(tx_buffer, rxLen);

    // Check if transmission was sucessfull
    if (r != OK_SPI) {
        printLog(Warning_log, static_cast<std::string>(__func__) + ": Packet not send");
        return NOK_SPI;
    }

    rxMsg = SPI::getInstance().recData();

    // Compute CRC over all received message including CRC itself - should equal 0
    if(CRC::crc8(rxMsg.data(),rxMsg.size()))
    {
        printLog(Warning_log, static_cast<std::string>(__func__) + ": CRC error");
        return NOK_SPI;
    }

    rxMsg.pop_back();
    printLog(Debug_log, static_cast<std::string>(__func__) + " ended succesfully");
    return OK_SPI;
}
