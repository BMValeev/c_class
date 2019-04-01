//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_CONNMODULE_H
#define C_CLASS_CONNMODULE_H
#include "I2C.h"
class ConnModule
{
public:
    std::vector<unsigned char>getAddress();
    void setAddress(std::vector<unsigned char> addr);
    ConnModule(std::string filename,LogCallback cb);
    ~ConnModule();
    uint8_t SetUUID(uint32_t uuid,std::vector<unsigned char> &response);
    uint8_t SetName(std::vector<unsigned char> data,std::vector<unsigned char> &response);
    uint8_t StartInit(std::vector<unsigned char> &response);
    uint8_t WriteString(std::vector<unsigned char> data,std::vector<unsigned char> &response);
    uint8_t EndInit(std::vector<unsigned char> &response);
    uint8_t CheckBonding(std::vector<unsigned char> &response);
    uint8_t WriteValue(std::vector<unsigned char> id,std::vector<unsigned char> value,std::vector<unsigned char> &response);
    uint8_t StartBonding(std::vector<unsigned char> db,std::vector<unsigned char> &response);
    uint8_t ReadValue(uint16_t id_rec, std::map <uint16_t,std::vector<unsigned char>> answer);
    uint8_t ReadLastChangedValue(std::map <uint16_t,std::vector<unsigned char>> answer);

private:
    uint16_t WrongTransactions=3;
    std::vector<unsigned char> addr;
    std::vector<unsigned char> WriteArray(uint8_t command,std::vector<unsigned char> data,unsigned int len);
    void PrintLog(uint8_t status, std::string text);
    static void PrintToCout(uint8_t status, std::string msg);
    LogCallback m_cb = ConnModule::PrintToCout;
};
#endif //C_CLASS_CONNMODULE_H
