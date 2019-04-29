//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_CONNMODULE_H
#define C_CLASS_CONNMODULE_H

#include "../Rest/defs.h"
#include "i2cpacket.h"
#include <vector>
#include <map>

#define CONN_MODULE_ADDRESS 0x70
#define CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class ConnModule : public I2CPacket
{
public:
    ConnModule(std::string filename, LogCallback cb = printToCout);

    // Commands
    uint8_t setUUID(uint32_t uuid, std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setName(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t startInit(std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t writeRecord(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t endInit(std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t checkBonding(std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t writeValue(std::vector<uint8_t> id, std::vector<uint8_t> value, std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t startBonding(std::vector<uint8_t> db, std::vector<uint8_t> &response, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t readValue(uint16_t id_rec, std::map <uint16_t, std::vector<uint8_t>> answer, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t readLastChangedValue(std::map<uint16_t, std::vector<uint8_t> > &answer, int attempts = CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);

    enum Cmd
    {
        SET_UUID = 0x01,
        SET_NAME = 0x02,
        START_INIT = 0x03,
        WRITE_RECORD = 0x04,
        END_INIT = 0x05,
        WRITE_VALUE = 0x06,
        READ_VALUE = 0x07,
        START_BONDING = 0x08,
        CHECK_BONDING = 0x09,
        READ_LAST = 0x0C
    };

};
#endif //C_CLASS_CONNMODULE_H
