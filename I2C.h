/*
 *Created by eleps on 27.04.18.
 */
#ifndef C_CLASS_I2C_H
#define C_CLASS_I2C_H

using namespace std;
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <vector>

#define MUTEX_BLOCKED 127
#define INVALID_DATA 0x03
#define TR_ERR 0x05
#define ACK 0x06
#define NACK 0x015
#define BOF 0x20
#define MSP 0x21

class I2C
{
public:
    static I2C & getInstance();
    static void initInstance();
    unsigned int begin(std::string device, struct i2c_client *client);
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
    unsigned char CRC8(unsigned char *buffer, unsigned int len);
    int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
};

class ConnModule
{
public:
    std::vector<unsigned char>getAddress(void);
    std::vector<unsigned char> setAddress(std::vector<unsigned char>addr);
    ConnModule(std::string filename,struct i2c_client *client);
    ~ConnModule();
    unsigned int SetUUID(std::vector<unsigned char> uuid);
    unsigned int SetName(std::vector<unsigned char> data);
    unsigned int StartInit(void);
    unsigned int WriteString(std::vector<unsigned char> data);
    unsigned int EndInit(void);
    unsigned int WriteValue(std::vector<unsigned char> id,std::vector<unsigned char> value);
    std::vector<unsigned char> ReadValue(std::vector<unsigned char> id); /*change order of message*/
    std::vector<unsigned char> ReadLastChangedValue(void); /*change order of message*/
    unsigned int StartBonding(std::vector<unsigned char> db);
    std::vector<unsigned char> CheckBonding(std::vector<unsigned char> data);

private:
    uint16_t WrongTransactions=3;
    std::vector<unsigned char> addr;
};

class BoardModule
{
public:
    std::vector<unsigned char>getAddress(void);
    void setAddress(std::vector<unsigned char>addr);
    BoardModule(std::string filename,struct i2c_client *client) ;
    ~BoardModule();
    std::vector<unsigned char> GetVersion(void);
    std::vector<unsigned char> GetTools(void);
    std::vector<unsigned char> GetPower(void);
    unsigned int SetEnergy(unsigned char energy);
    unsigned int SetVolume(unsigned char volume);

private:
    uint16_t WrongTransactions=3;
    std::vector<unsigned char> addr;
};

#endif //C_CLASS_I2C_H
