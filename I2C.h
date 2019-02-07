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

#define TR_ERR_SPI 0x05
#define ACK_I2C 0x06
#define NACK_I2C 0x015
#define BOF_I2C 0x20
#define MSP_I2C 0x21

#define OK_I2C 0x00
#define NOK_I2C 0x01


typedef std::function<void(uint8_t, std::string)> CallbackFunction;
class I2C
{
public:
    static I2C & getInstance(LogCallback cb=PrintToCout);
    static void initInstance(LogCallback cb=PrintToCout);
    unsigned int begin(std::string device);
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    std::vector<unsigned char> recData(void);

protected:
    I2C(LogCallback cb=PrintToCout);
    virtual ~I2C();

private:
    static I2C * theOneTrueInstance;
    const unsigned int MaxLen=40;
    unsigned int MsgLen;
    std::vector<unsigned char>  LastRecMsg;
    std::mutex Mutex;
    int init=0;
    std::string DeviceName;
    struct i2c_client *i2c_data;
    /*Unsafe Methods*/
    int SendRaw_new(std::vector<unsigned char> address, std::vector<unsigned char> buffer, unsigned int rlen);
    int SendRaw(std::vector<unsigned char> address,std::vector<unsigned char>, unsigned int len);
    void SetDeviceName(std::string Name);
    void CleanRecMsg(void);
    /*Safe functions*/
    static void PrintToCout(uint8_t status, std::string msg);
    void PrintLog(uint8_t status, std::string text);
    int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    LogCallback m_cb = I2C::PrintToCout;

private:
    virtual int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    const unsigned int MaxLen=40;
    int init=0;
    void SetDeviceName(std::string Name);
    std::mutex Mutex;
    std::string DeviceName;
    struct i2c_client *i2c_data;
};

class I2C_ELEPS : public I2C
{
public:
    static I2C_ELEPS &getInstance(CallbackFunction cb=PrintToCout)  : getInstance(cb) {}
    static void initInstance(CallbackFunction cb=PrintToCout)   :  initInstance(cb) {}

protected:
    I2C_ELEPS(CallbackFunction cb=PrintToCout) : I2C(cb)
    {}
    virtual ~I2C_ELEPS();
    int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len) override;
private:
};

#endif //C_CLASS_I2C_H
