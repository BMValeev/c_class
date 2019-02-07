//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_BOARDMODULE_H
#define C_CLASS_BOARDMODULE_H

#include "I2C.h"
class BoardModule
{
public:
    std::vector<unsigned char>getAddress();
    void setAddress(std::vector<unsigned char> addr);
    BoardModule(std::string filename,LogCallback cb) ;
    ~BoardModule();
    uint8_t GetVersion(std::vector<unsigned char> &response);
    uint8_t SetBonding(unsigned char enable,std::vector<unsigned char> &response);
    uint8_t GetTools(std::vector<unsigned char> &response);
    uint8_t GetPower(std::vector<unsigned char> &response);
    uint8_t SetEnergy(unsigned char energy,std::vector<unsigned char> &response);
    uint8_t SetVolume(unsigned char volume,std::vector<unsigned char> &response);


private:
    uint16_t WrongTransactions=3;
    std::vector<unsigned char> addr;
    std::vector<unsigned char> WriteArray(uint8_t command,std::vector<unsigned char> data,unsigned int len);
    void PrintLog(uint8_t status, std::string text);
    static void PrintToCout(uint8_t status, std::string msg);
    LogCallback m_cb = BoardModule::PrintToCout;
};
#endif //C_CLASS_BOARDMODULE_H
