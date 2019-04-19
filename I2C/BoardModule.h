//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_BOARDMODULE_H
#define C_CLASS_BOARDMODULE_H

#include "../defs.h"
#include "../Rest/loggable.h"
#include <vector>

#define BOARD_MODULE_ADDRESS 0x40
#define BOARD_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class BoardModule : public Loggable
{
public:
    BoardModule(std::string filename, LogCallback cb = printToCout) ;

    // Address
    uint8_t getAddress() const { return mAddr; }
    void setAddress(uint8_t addr) { mAddr = addr & 0x7f; }
    // Commands
    uint8_t getVersion(std::vector<uint8_t> &response);
    uint8_t setBonding(uint8_t enable,std::vector<uint8_t> &response);
    uint8_t getTools(std::vector<uint8_t> &response);
    uint8_t getPower(std::vector<uint8_t> &response);
    uint8_t setEnergy(uint8_t energy,std::vector<uint8_t> &response);
    uint8_t setVolume(uint8_t volume,std::vector<uint8_t> &response);


private:
    uint8_t mAddr;
    uint16_t mAttempts;

    // Helpers
    std::vector<uint8_t> writeArray(uint8_t command,std::vector<uint8_t> data, uint32_t len);
};
#endif //C_CLASS_BOARDMODULE_H
