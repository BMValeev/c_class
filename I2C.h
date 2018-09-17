/*
 *Created by eleps on 27.04.18.
 */
#ifndef C_CLASS_I2C_H
#define C_CLASS_I2C_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using namespace std;
#include <unistd.h>
#include <cstdint>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>

#define MUTEX_BLOCKED 127
#define INVALID_DATA 0x03
#define TR_ERR 0x05
#define ACK_I2C 0x06
#define NACK_I2C 0x015
#define BOF 0x20
#define MSP 0x21

#define OK 0x00
#define NOK 0x01

#define QTAPP // flags that is used by submodules to determine wether they are compiled within application or stand-alone

#ifndef QTAPP
// Stand-alone compile
enum Log_status { Info_log = 1, Debug_log=2, Warning_log= 3,Critical_log=4 };
#else
// Within QT app compile
#include "SPI.h"
#endif // QTAPP

typedef std::function<void(uint8_t, std::string)> CallbackFunction;
class I2C
{
public:
    static I2C & getInstance();
    static void initInstance();
    unsigned int begin(std::string device,CallbackFunction cb);
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    std::vector<unsigned char> recData(void);

protected:
    I2C();
    virtual ~I2C();

private:
    static I2C * theOneTrueInstance;
    const unsigned int MaxLen=40;
    unsigned int MsgLen;
    std::vector<unsigned char>  LastRecMsg;
    int Mutex;
    int NewData;
    int i2cfd;
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
    unsigned char CRC8(unsigned char *buffer, unsigned int len);
    int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    CallbackFunction m_cb = I2C::PrintToCout;

};

class ConnModule
{
public:
    std::vector<unsigned char>getAddress();
    void setAddress(std::vector<unsigned char> addr);
    ConnModule(std::string filename,CallbackFunction cb);
    ~ConnModule();
    uint8_t SetUUID(std::vector<unsigned char> uuid,std::vector<unsigned char> &response);
    uint8_t SetName(std::vector<unsigned char> data,std::vector<unsigned char> &response);
    uint8_t StartInit(std::vector<unsigned char> &response);
    uint8_t WriteString(std::vector<unsigned char> data,std::vector<unsigned char> &response);
    uint8_t EndInit(std::vector<unsigned char> &response);
    uint8_t CheckBonding(std::vector<unsigned char> &response);
    uint8_t WriteValue(std::vector<unsigned char> id,std::vector<unsigned char> value,std::vector<unsigned char> &response);
    uint8_t StartBonding(std::vector<unsigned char> db,std::vector<unsigned char> &response);
    uint8_t ReadValue(std::map <uint16_t,uint32_t> answer);
    uint8_t ReadLastChangedValue(std::map <uint16_t,uint32_t> answer);


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
