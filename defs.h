#ifndef DEFS_H
#define DEFS_H

#ifdef C_CLASS_DEBUG
    // Stand-alone compile
#else
    // Within QT app compile
#endif // C_CLASS_DEBUG

#include <functional>
typedef std::function<void(uint8_t, std::string)> LogCallback;

enum Log_status { Info_log = 1, Debug_log=2, Warning_log= 3, Critical_log=4 };
#define TR_ERR_SPI 0x05
#define ACK_I2C 0x06
#define NACK_I2C 0x015
#define BOF_I2C 0x20
#define MSP_I2C 0x21

#define OK_I2C 0x00
#define NOK_I2C 0x01

#endif // DEFS_H
