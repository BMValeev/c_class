/*
 *
 *
 *
 *
 *
 *Created by eleps on 27.04.18.
 */
#ifndef C_CLASS_I2C_H
#define C_CLASS_I2C_H
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
using namespace std;
#ifndef C_CLASS_SPI_H
#define C_CLASS_SPI_H
class I2C
{
    static I2C * theOneTrueInstance;
private:
    const unsigned int MaxLen;
    unsigned int MsgLen;
    std::vector<unsigned char>  LastRecMsg;
    int Mutex;
    int NewData;
    int i2cfd;
    int init;
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
public:
    static I2C & getInstance();
    static void initInstance();
    unsigned int begin(std::string device, i2c_client *client);
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len);
    std::vector<unsigned char> recData(void);
protected:
    I2C();
    virtual ~I2C();
};
class BoardModule
{
private:
    uint16_t WrongTransactions=3;
    std::vector<unsigned char> addr;

public:
    std::vector<unsigned char>getAddress(void);
    std::vector<unsigned char> setAddress(std::vector<unsigned char>addr);
    BoardModule(std::string filename,struct i2c_client *client) ;
    ~BoardModule()
    std::vector<unsigned char> GetVersion(void);
    std::vector<unsigned char> GetTools(void);
    std::vector<unsigned char> GetPower(void);
    unsigned int SetEnergy(unsigned char energy);
    unsigned int SetVolume(unsigned char volume);

};
class ConnModule
{
private:
    uint16_t WrongTransactions;
    std::vector<unsigned char> addr;
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
protected:
};
#endif //C_CLASS_I2C_H
