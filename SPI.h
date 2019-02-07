/*
 *Created by eleps on 27.04.18.
*/
#ifndef C_CLASS_SPI_H
#define C_CLASS_SPI_H
/*
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
#include "crc.h"*/

#include <string>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <mutex>

#include "defs.h"

#define ACK_SPI 0x30
#define EXEC_SPI 0x03
#define TR_ERR_SPI 0x05
#define OK_SPI 0x00
#define NACK 0x02
#define NOK_SPI 0x01
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define PACKED_LENGTH_SPI 32


// Low level class that implements basic information exchange via SPI on Hamming board

class SPI
{
public:
    static SPI & getInstance();
    static void initInstance();
    uint8_t begin(std::string device,LogCallback cb);
    uint8_t transaction(std::vector<unsigned char> buffer, uint8_t ans_len);
    std::vector<unsigned char> recData(void);

protected:
    SPI();
    virtual ~SPI();

private:
    static SPI* theOneTrueInstance;
    std::vector<unsigned char> LastRecMsg;
    std::mutex Mutex;
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
    int SendPacket(std::vector<unsigned char> Buffer, uint8_t ans_len);
    LogCallback m_cb = SPI::PrintToCout;
};



#endif //C_CLASS_SPI_H

