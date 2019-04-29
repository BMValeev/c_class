#include "i2cpacket.h"

#include "I2C.h"
#include "../Rest/crc.h"
#if defined __linux__ && !defined DEMO
I2CPacket::I2CPacket(std::string filename, uint8_t addr, LogCallback cb)
    : Loggable(cb)
    , mAddr(addr)
{    
    I2C &i2c = I2C::getInstance();
    i2c.begin(filename);
}

std::vector<uint8_t> I2CPacket::writeArray(uint8_t cmd, std::vector<uint8_t> data, uint32_t ansLen, int attempts)
{
    I2C& i2c = I2C::getInstance();
    uint8_t l_len = 2;
    std::vector<uint8_t> msg, answer, null, l_answer;
    l_len += static_cast<uint8_t>(data.size());
    msg.push_back(static_cast<uint8_t>(mAddr << 1));
    msg.push_back(l_len);
    msg.push_back(cmd);
    for(uint32_t i=0;i<data.size();i++) {
        msg.push_back(data[i]);
    }
    msg.push_back(CRC::crc8(msg.data(),msg.size()));
    msg.erase(msg.begin());
    while(attempts--) {
        if(i2c.transaction(mAddr, msg, ansLen)==OK_I2C){
            answer=i2c.recData();
            l_len=answer.front()+1;
            l_answer.clear();
            l_answer.push_back(static_cast<uint8_t>(mAddr << 1));
            while (l_len--) {
                l_answer.push_back(answer.front());
                answer.erase(answer.begin());
            }
            if (CRC::crc8(l_answer.data(), l_answer.size()-1) == l_answer.back()) {
                printLog(DebugLog, static_cast<std::string>(__func__) + "CRC Ok");
                l_answer.erase(l_answer.begin());
                l_answer.erase(l_answer.begin());
                l_answer.pop_back();
                return l_answer;
            }
            printLog(DebugLog, static_cast<std::string>(__func__) + "CRC NOK");
        }
    }
    // What is this?
    if (cmd == 0x0C) {
        null.push_back('c');
        return null;
    }
    return null;
}
#else
I2CPacket::I2CPacket(std::string filename, uint8_t addr, LogCallback cb)
    : Loggable(cb)
    , mAddr(addr){    }

std::vector<uint8_t> I2CPacket::writeArray(uint8_t cmd, std::vector<uint8_t> data, uint32_t ansLen, int attempts)
{
       I2C& i2c = I2C::getInstance();
    uint8_t l_len = 2;
    std::vector<uint8_t> msg, answer, null, l_answer;
    l_len += static_cast<uint8_t>(data.size());
    msg.push_back(static_cast<uint8_t>(mAddr << 1));
    msg.push_back(l_len);
    msg.push_back(cmd);
    for(uint32_t i=0;i<data.size();i++) {
        msg.push_back(data[i]);
    }
    msg.push_back(CRC::crc8(msg.data(),msg.size()));
    msg.erase(msg.begin());
    while(attempts--) {
        if(i2c.transaction(mAddr, msg, ansLen)==OK_I2C){
            answer=i2c.recData();
            l_len=answer.front()+1;
            l_answer.clear();
            l_answer.push_back(static_cast<uint8_t>(mAddr << 1));
            while (l_len--) {
                l_answer.push_back(answer.front());
                answer.erase(answer.begin());
            }
            if (CRC::crc8(l_answer.data(), l_answer.size()-1) == l_answer.back()) {
                printLog(DebugLog, static_cast<std::string>(__func__) + "CRC Ok");
                l_answer.erase(l_answer.begin());
                l_answer.erase(l_answer.begin());
                l_answer.pop_back();
                return l_answer;
            }
            printLog(DebugLog, static_cast<std::string>(__func__) + "CRC NOK");
        }
    }
    // What is this?
    if (cmd == 0x0C) {
        null.push_back('c');
        return null;
    }
    return null;
}
#endif