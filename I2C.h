﻿/*
 *Created by eleps on 27.04.18.
 */
#ifndef C_CLASS_I2C_H
#define C_CLASS_I2C_H


#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
#include <mutex>
#include <cstdio>
#include <cerrno>
#include <cstdlib>

#include "defs.h"

#define TR_ERR_SPI 0x05
#define ACK_I2C 0x06
#define NACK_I2C 0x015
#define BOF_I2C 0x20
#define MSP_I2C 0x21

#define OK_I2C 0x00
#define NOK_I2C 0x01


typedef std::function<void(uint8_t, std::string)> CallbackFunction;
class I2C  {
public:
    static I2C &getInstance(CallbackFunction cb=PrintToCout);
    static void initInstance(CallbackFunction cb=PrintToCout);
    unsigned int begin(std::string device);
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    std::vector<unsigned char> recData(void);
protected:
    I2C(CallbackFunction cb=PrintToCout);
    virtual ~I2C();
    unsigned int MsgLen;
    std::vector<unsigned char>  LastRecMsg;
    int SendRaw_new(std::vector<unsigned char> address, std::vector<unsigned char> buffer, unsigned int rlen);
    CallbackFunction m_cb = I2C::PrintToCout;
    static I2C * theOneTrueInstance;
    static void PrintToCout(uint8_t status, std::string msg);
    void CleanRecMsg(void);
    void PrintLog(uint8_t status, std::string text);

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

class ConnModule
{
public:
    std::vector<unsigned char>getAddress();
    void setAddress(std::vector<unsigned char> addr);
    ConnModule(std::string filename,CallbackFunction cb);
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
    CallbackFunction m_cb = ConnModule::PrintToCout;
};

class BoardModule
{
public:
    std::vector<unsigned char>getAddress();
    void setAddress(std::vector<unsigned char> addr);
    BoardModule(std::string filename,CallbackFunction cb) ;
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
    CallbackFunction m_cb = BoardModule::PrintToCout;
};

#endif //C_CLASS_I2C_H
