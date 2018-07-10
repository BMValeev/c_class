/*
 *Created by eleps on 27.04.18.
*/
#ifndef C_CLASS_SPI_H
#define C_CLASS_SPI_H

using namespace std;
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

// Low level class that implements basic information exchange via SPI on Hamming board
class SPI
{
public:
    int begin(std::string device);
    static void initInstance();
    static SPI & getInstance();
    unsigned int transaction(std::vector<unsigned char> buffer);
    std::vector<unsigned char> recData(void);

protected:
    SPI();
    virtual ~SPI();

private:
    static SPI* theOneTrueInstance;
    std::vector<unsigned char> LastRecMsg;
    int Mutex;
    int NewData;
    int spifd;
    unsigned char mode;
    unsigned char bitsPerWord;
    unsigned int speed;
    std::string DeviceName;
    int init=0;
    int SendRaw_new(unsigned char *buffer, unsigned int len);
    int ReceiveRaw_new(void);
    int SendRaw(unsigned char *buffer, unsigned int len);
    int ReceiveRaw(void);
    void SetDeviceName(std::string Name);
    void CleanRecMsg(void);
    unsigned char CRC8(unsigned char *buffer, unsigned int len);
    int SendPacket(std::vector<unsigned char> Buffer);
    int ReceivePacket(void);
};

#define MUTEX_BLOCKED 127
#define INVALID_DATA 0x03
#define TR_ERR 0x05
#define ACK 0x06
#define NACK 0x015
#define BOF 0x20
#define MSP 0x21
#define OK 0x00
#define NOK 0x01
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

class MCU
{
public:
    MCU(std::string filename);
    ~MCU();
    uint16_t SetStanby(uint16_t Status);
    std::vector<unsigned char> CheckStatus(void);
    void RenewAll(void );
    uint16_t SetSubroutine(unsigned char Routine);
    uint16_t SetConnector(uint16_t  Connector);
    uint16_t SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton);
    uint16_t SetPower(uint16_t BlueButton, uint16_t YellowButton);
    uint16_t SetMaxTime(uint16_t MaxTime );
    uint16_t SetAutoStart(uint8_t Enabled );
    uint16_t SetAutoStartDelay(uint16_t DelayTime );
    uint16_t SetAutoStop(uint8_t Enabled );
    uint16_t SetAutoStopResistance(uint16_t Resistance );
    uint16_t SetIrrigation(uint8_t Enabled );
    uint16_t SetModulation(uint16_t Frequency );
    uint16_t SetDutyRate(uint16_t CrestFactor );

private:
    uint16_t WrongTransactions=3;

protected:
    uint16_t SendBool(uint8_t command,uint16_t value);
    uint16_t SendInt(uint8_t command,uint16_t value);
    uint16_t SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2);
};

#endif //C_CLASS_SPI_H

