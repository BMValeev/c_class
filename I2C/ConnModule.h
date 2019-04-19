//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_CONNMODULE_H
#define C_CLASS_CONNMODULE_H

#include "../defs.h"
#include "../Rest/loggable.h"
#include <vector>
#include <map>

#define CONN_MODULE_ADDRESS 0x70
#define CONN_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class ConnModule : public Loggable
{
public:
    ConnModule(std::string filename, LogCallback cb = printToCout);

    // Address
    uint8_t getAddress() const { return mAddr; }
    void setAddress(uint8_t addr) { mAddr = addr & 0x7f; }
    // Commands
    uint8_t setUUID(uint32_t uuid, std::vector<uint8_t> &response);
    uint8_t setName(std::vector<uint8_t> data, std::vector<uint8_t> &response);
    uint8_t startInit(std::vector<uint8_t> &response);
    uint8_t writeString(std::vector<uint8_t> data, std::vector<uint8_t> &response);
    uint8_t endInit(std::vector<uint8_t> &response);
    uint8_t checkBonding(std::vector<uint8_t> &response);
    uint8_t writeValue(std::vector<uint8_t> id, std::vector<uint8_t> value, std::vector<uint8_t> &response);
    uint8_t startBonding(std::vector<uint8_t> db, std::vector<uint8_t> &response);
    uint8_t readValue(uint16_t id_rec, std::map <uint16_t, std::vector<uint8_t>> answer);
    uint8_t readLastChangedValue(std::map<uint16_t, std::vector<uint8_t> > &answer);

private:
    uint8_t mAddr;
    uint16_t mAttempts;

    // Helpers
    std::vector<uint8_t> writeArray(uint8_t command,std::vector<uint8_t> data, uint32_t len);
};
#endif //C_CLASS_CONNMODULE_H
