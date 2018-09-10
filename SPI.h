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
#include <functional>
#include <vector>
#include <algorithm>

#define MUTEX_BLOCKED 127
#define INVALID_DATA 0x03
#define ACK 0x30
#define EXEC 0x03
#define TR_ERR 0x05
#define TEST 0x03
#define OK 0x00
#define NACK 0x02
#define NOK 0x01
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define QTAPP // flags that is used by submodules to determine wether they are compiled within application or stand-alone

enum Log_status { Info_log = 1, Debug_log=2, Warning_log= 3,Critical_log=4 };
// Low level class that implements basic information exchange via SPI on Hamming board
typedef std::function<void(uint8_t, std::string)> CallbackFunction;
class SPI
{
public:
    static SPI & getInstance();
    static void initInstance();
    uint8_t begin(std::string device,CallbackFunction cb);
    uint8_t transaction(std::vector<unsigned char> buffer, uint8_t ans_len);
    std::vector<unsigned char> recData(void);

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
    static void PrintToCout(uint8_t status, std::string msg);
    int SendRaw_new(unsigned char *buffer, unsigned int len, uint8_t ans_len);
    void SetDeviceName(std::string Name);
    void CleanRecMsg(void);
    unsigned char CRC8(unsigned char *buffer, unsigned int len);
    int SendPacket(std::vector<unsigned char> Buffer, uint8_t ans_len);
    CallbackFunction m_cb = SPI::PrintToCout;
};












class MCU
{
public:
    MCU(std::string filename,CallbackFunction cb);
    ~MCU();
    uint8_t SetStanby(uint8_t Status);
    uint8_t CheckStatus(std::vector<unsigned char> &answer);
    void RenewAll( );
    uint8_t SetSubroutine(uint8_t Routine1,uint8_t Routine2);
    uint8_t SetConnector(uint8_t  Connector);
    uint8_t SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton);
    uint8_t SetPower(uint16_t BlueButton, uint16_t YellowButton);
    uint8_t SetMaxTime(uint8_t MaxTime );
    uint8_t SetAutoStart(uint8_t Enabled );
    uint8_t SetAutoStartDelay(uint8_t DelayTime );
    uint8_t SetAutoStop(uint8_t Enabled );
    uint8_t SetAutoStopResistance(uint16_t Resistance );
    uint8_t SetIrrigation(uint8_t Enabled );
    uint8_t SetIrrigationDelay(uint8_t Delay );
    uint8_t SetModulation(uint16_t Frequency1, uint16_t Frequency2);
    uint8_t SetPedalCoag(uint8_t  Status);
    uint8_t SetPedalCut(uint8_t  Status);
    uint8_t SetFilter(uint8_t  Filter1,uint8_t  Filter2);
    uint8_t SetDutyCycle(uint8_t DutyCycle1 ,uint8_t DutyCycle2);

private:
    uint8_t WrongTransactions=3;
    void PrintLog(uint8_t status, std::string text);
    static void PrintToCout(uint8_t status, std::string msg);
    CallbackFunction m_cb = MCU::PrintToCout;

protected:
    uint8_t SendBool(uint8_t command,uint16_t value);
    uint8_t SendInt(uint8_t command,uint16_t value);
    uint8_t SendChar(uint8_t command,uint8_t value);
    uint8_t SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2);
};

#endif //C_CLASS_SPI_H

