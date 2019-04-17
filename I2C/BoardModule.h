//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_BOARDMODULE_H
#define C_CLASS_BOARDMODULE_H

#include "../defs.h"
#include <vector>

#define BOARD_MODULE_ADDRESS 0x40
#define BOARD_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class BoardModule
{
public:
    BoardModule(std::string filename, LogCallback cb = printToCout) ;
    ~BoardModule() { }

    // Address
    uint8_t getAddress();
    void setAddress(uint8_t mAddr);
    // Commands
    uint8_t getVersion(std::vector<uint8_t> &response);
    uint8_t setBonding(uint8_t enable,std::vector<uint8_t> &response);
    uint8_t getTools(std::vector<uint8_t> &response);
    uint8_t getPower(std::vector<uint8_t> &response);
    uint8_t setEnergy(uint8_t energy,std::vector<uint8_t> &response);
    uint8_t setVolume(uint8_t volume,std::vector<uint8_t> &response);


private:
    LogCallback mCb;
    uint16_t mAttempts = 3;
    uint8_t mAddr;
    std::vector<uint8_t> writeArray(uint8_t command,std::vector<uint8_t> data, uint32_t len);
    void printLog(uint8_t status, std::string text);
    static void printToCout(uint8_t status, std::string msg);
};
#endif //C_CLASS_BOARDMODULE_H
