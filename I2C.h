/*
 *Created by eleps on 27.04.18.
 */
#ifndef C_CLASS_I2C_H
#define C_CLASS_I2C_H


#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <mutex>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <iostream>

#include "defs.h"



typedef std::function<void(uint8_t, std::string)> CallbackFunction;
class I2C
{
public:
    static I2C & getInstance(LogCallback cb=PrintToCout);
    static void initInstance(LogCallback cb=PrintToCout);
    unsigned int begin(std::string device);
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    std::vector<unsigned char> recData(void);
private:

protected:
    I2C(LogCallback cb=PrintToCout);
    virtual ~I2C();
    void CleanRecMsg(void);
    virtual int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    const unsigned int MaxLen=40;
    static void PrintToCout(uint8_t status, std::string msg);
    int SendRaw_new(std::vector<unsigned char> address, std::vector<unsigned char> buffer, unsigned int rlen);
    void PrintLog(uint8_t status, std::string text);
    void SetDeviceName(std::string Name);
    static I2C * theOneTrueInstance;
    unsigned int MsgLen;
    std::vector<unsigned char>  LastRecMsg;
    LogCallback m_cb = I2C::PrintToCout;
    int init=0;
    std::mutex Mutex;
    std::string DeviceName;
    struct i2c_client *i2c_data;
};


#endif //C_CLASS_I2C_H
