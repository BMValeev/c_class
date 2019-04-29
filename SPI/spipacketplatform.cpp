#include "spipacket.h"

/* This file contains platform dependent functions of SPIPacket class. It is not
 * to be included in case if the appplication is built for different platform,
 * e.g. in DEMO regime. It is up to application itself to include corresponding cpp
 * files (either spipacketplatform.cpp or spipacketdemo.cpp).
 */

#include <cassert>
#include "MCU.h"// this is only for debug mode to form answer, probably remove later
#include "SPI.h"
SPIPacket::SPIPacket(std::string deviceName, LogCallback cb)
    : Loggable(cb)
    , mDeviceName(deviceName) {
    SPI::getInstance().begin(deviceName, cb);
}

uint8_t SPIPacket::send(uint8_t cmd,
                        std::vector<uint8_t> &payload,
                        std::vector<uint8_t> &answer,
                        int attempts,
                        uint16_t pause) const {
    // Get answer length
    uint8_t rxLen = getRxCnt(cmd);

    while(attempts--)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + " transactions left: " + std::to_string(attempts));

        // Send message
        if (transaction(cmd, payload, answer, rxLen, pause) != OK_SPI)
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
            } else
                continue;
        }

        // Acknowledged & executed
        answer.erase(answer.begin()); // Remove acknowledge byte
        printLog(DebugLog, static_cast<std::string>(__func__) + " - success");
        return OK_SPI;
    }
    return TR_ERR_SPI;
}

uint8_t SPIPacket::transaction(uint8_t cmd,
                               std::vector<uint8_t> &txMsg,
                               std::vector<uint8_t> &rxMsg,
                               uint8_t rxLen,
                               uint16_t pause) const {
    assert(txMsg.size() < SPI_PACKET_MAX_TX_SIZE);

    printLog(DebugLog, static_cast<std::string>(__func__) + " started");

    // If the msg is less than 128 bytes, just use it's length
    // If the msg is larger, set length to multiples of 128
    uint64_t txLen = txMsg.size() + 3;
    uint64_t txFullLen = txLen < 128? txLen : 128*(txLen/128 + (txLen%128? 1 : 0));

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
    uint8_t r = SPI::getInstance().transaction(tx_buffer, rxLen, pause);

    // Check if transmission was sucessfull
    if (r != OK_SPI) {
        printLog(WarningLog, static_cast<std::string>(__func__) + ": Packet not send");
        return NOK_SPI;
    }

    rxMsg = SPI::getInstance().recData();

    // Compute CRC over all received message including CRC itself - should equal 0
    if(CRC::crc8(rxMsg.data(),rxMsg.size()))
    {
        printLog(WarningLog, static_cast<std::string>(__func__) + ": CRC error");
        return NOK_SPI;
    }

    rxMsg.pop_back();
    printLog(DebugLog, static_cast<std::string>(__func__) + " ended succesfully");
    return OK_SPI;
}
