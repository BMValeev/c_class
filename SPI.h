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

#define MUTEX_BLOCKED 127
#define INVALID_DATA 0x03
#define ACK 0x30
#define EXEC 0x03
#define TR_ERR 0x05
#define TEST 0x03
#define OK 0x00
#define NOK 0x01
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
enum Log_status { Info_log = 1, Debug_log=2, Warning_log= 3,Critical_log=4 };
// Low level class that implements basic information exchange via SPI on Hamming board
class SPI
{
public:
    static SPI & getInstance();
    static void initInstance();
    uint8_t begin(std::string device);
    uint8_t transaction(std::vector<unsigned char> buffer, uint8_t ans_len);
    std::vector<unsigned char> recData(void);
    void changePrint(void (SPI::*ptrFunc)(uint8_t, std::string));

protected:
    SPI();
    virtual ~SPI();

private:
    static SPI* theOneTrueInstance;
    std::vector<unsigned char> LastRecMsg;
    uint8_t Mutex;
    uint8_t status=0;
    uint8_t NewData;
    int spifd;
    unsigned char mode;
    unsigned char bitsPerWord;
    unsigned int speed;
    std::string DeviceName;
    uint8_t init=0;
    void PrintLog(uint8_t status, std::string text);
    void (SPI::*ptrPrintLog)(uint8_t, std::string) = &SPI::PrintLog;
    int SendRaw_new(unsigned char *buffer, unsigned int len, uint8_t ans_len);
    void SetDeviceName(std::string Name);
    void CleanRecMsg(void);
    unsigned char CRC8(unsigned char *buffer, unsigned int len);
    int SendPacket(std::vector<unsigned char> Buffer, uint8_t ans_len);
};

class MCU
{
public:
    MCU(std::string filename);
    ~MCU();
    uint8_t SetStanby(uint16_t Status);
    std::vector<unsigned char> CheckStatus(void);
    void RenewAll(void );
    uint8_t SetSubroutine(unsigned char Routine);
    uint8_t SetConnector(uint16_t  Connector);
    uint8_t SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton);
    uint8_t SetPower(uint16_t BlueButton, uint16_t YellowButton);
    uint8_t SetMaxTime(uint16_t MaxTime );
    uint8_t SetAutoStart(uint8_t Enabled );
    uint8_t SetAutoStartDelay(uint16_t DelayTime );
    uint8_t SetAutoStop(uint8_t Enabled );
    uint8_t SetAutoStopResistance(uint16_t Resistance );
    uint8_t SetIrrigation(uint8_t Enabled );
    uint8_t SetModulation(uint16_t Frequency );
    uint8_t SetDutyRate(uint16_t CrestFactor );
    void changePrint(void (MCU::*ptrFunc)(uint8_t, std::string));

private:
    uint16_t WrongTransactions=3;
    void PrintLog(uint8_t status, std::string text);
    void (MCU::*ptrPrintLog)(uint8_t, std::string) = &MCU::PrintLog;

protected:
    uint8_t SendBool(uint8_t command,uint16_t value);
    uint8_t SendInt(uint8_t command,uint16_t value);
    uint8_t SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2);
};

#endif //C_CLASS_SPI_H

