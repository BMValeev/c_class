/*
 *Created by eleps on 27.04.18.
 */
#ifndef C_CLASS_I2C_H
#define C_CLASS_I2C_H

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <map>
#include <mutex>
#include <cstdio>
#include <cerrno>
#include <cstdlib>

#include "Rest/defs.h"
#include "../Rest/loggable.h"

#define ACK_I2C 0x06
#define NACK_I2C 0x015
#define BOF_I2C 0x20
#define MSP_I2C 0x21

#define OK_I2C 0x00
#define NOK_I2C 0x01
#ifdef __linux__
class I2C : public Loggable {
public:
    static I2C & getInstance();

    uint32_t begin(std::string device, LogCallback cb = printToCout);
    std::vector<uint8_t> recData() { return mLastRecMsg; }
    void cleanRecMsg() { mLastRecMsg.clear(); }
    bool isInitialized() const { return theOneTrueInstance != nullptr; }

    // Transmission
    uint32_t transaction(uint8_t address, std::vector<uint8_t> buffer, uint32_t ansLen);

protected:
    I2C();
    ~I2C() { }

private:
    static I2C * theOneTrueInstance;
    bool mInit;
    std::string mDeviceName;
    std::vector<uint8_t>  mLastRecMsg;
    std::mutex mMutex;
    const uint32_t mMaxLen;
    uint32_t mMsgLen;
    struct i2c_client *mI2CData;

    // Helpers
    void setDeviceName(std::string name);

};
#endif
#endif //C_CLASS_I2C_H
