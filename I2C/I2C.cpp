/*
 *Created by eleps on 27.04.18.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include "../Rest/crc.h"
#include "I2C.h"

// I2C class
I2C* I2C::theOneTrueInstance = nullptr;

I2C& I2C::getInstance() {
    if (theOneTrueInstance == nullptr) {
        // Allocate on heap so that it does not get destroyed after
        theOneTrueInstance = new I2C();
    }
    return *theOneTrueInstance;
}

I2C::I2C()
    : Loggable ()
    , mInit(false)
    , mMaxLen(40)
{
    printLog(DebugLog, static_cast<std::string>(__func__) + "I2C constructor called");
    if (theOneTrueInstance) throw std::logic_error("Instance already exists");
    theOneTrueInstance = this;
}

void I2C::setDeviceName(std::string name)
{
    if (mDeviceName == name)
        return;

    printLog(DebugLog, static_cast<std::string>(__func__) + "Device name set to :" + name);
    mDeviceName = name;
}

uint32_t I2C::begin(std::string device, LogCallback cb)
{
    // Lock the mutex first
    std::lock_guard<std::mutex> lock(mMutex); // automatically unlocks when function is leaved, no need to call unlock

    setLogCallback(cb);

    if (mInit) {
        printLog(InfoLog, static_cast<std::string>(__func__) + " I2C already initialized");
        return NOK_I2C;
    }

    cleanRecMsg();
    printLog(DebugLog, static_cast<std::string>(__func__) + device);
    setDeviceName(device);
    printLog(DebugLog, static_cast<std::string>(__func__) + " Initialized");
    mInit = true;

    return OK_I2C;
}

uint32_t I2C::transaction(uint8_t address, std::vector<uint8_t> buffer, uint32_t ansLen)
{
    // Lock the mutex first
    std::lock_guard<std::mutex> lock(mMutex); // automatically unlocks when function is leaved, no need to call unlock

    cleanRecMsg();
    int ret;
    uint32_t cnt, cnt_all;
    uint8_t buf_rec[40] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
            ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    std::string data_send,data_rec;
    i2c_rdwr_ioctl_data message;
    memset(&message, 0, sizeof(message));
    i2c_msg message_packet[2];
    printLog(DebugLog, static_cast<std::string>(__func__) + " Send message");
    memset(&message_packet, 0, sizeof(i2c_msg)*2);
    message_packet[0].addr=address;
    message_packet[0].flags=0;
    message_packet[0].len = static_cast<uint16_t>(buffer.size());
    message_packet[0].buf=buffer.data();
    message_packet[1].addr=address;
    message_packet[1].flags=I2C_M_RD;//I2C_M_RD|I2C_M_RECV_LEN;I2C_M_NOSTART
    message_packet[1].buf=buf_rec;
    message_packet[1].len=10;
    message_packet[1].len = static_cast<uint16_t>((ansLen==0)? 10 : ansLen);
    cnt_all= (ansLen==0)? 10 : ansLen;
    message.msgs=message_packet;
    message.nmsgs =(ansLen!=0) ?  2:1;
    int file = open(mDeviceName.c_str(), O_RDWR);
    if (file == -1) {
        printLog(WarningLog,static_cast<std::string>(__func__) + "Device open error");
        close(file);
        return NOK_I2C;
    }
    if (ioctl(file, I2C_SLAVE, address) < 0) {
        printLog(WarningLog,static_cast<std::string>(__func__) + "Failed to acquire bus access and/or talk to slave");
        close(file);
        return NOK_I2C;
    }
    ret=ioctl(file, I2C_RDWR, &message);
    if (ret<0) {
        printLog(WarningLog,static_cast<std::string>(__func__) + strerror(-ret) + "Unable to send message");
        fprintf (stderr, "%s.\n", strerror(-ret));
        close(file);
        return NOK_I2C;
    }
    close(file);
    cleanRecMsg();
    for (cnt = 0; cnt < (cnt_all); cnt++) {
        mLastRecMsg.push_back(buf_rec[cnt]);
    }
    printLog(DebugLog,static_cast<std::string>(__func__) + " Send message");
    return OK_I2C;
}
/*
 *  if (flag){
        cnt_all=cnt_all-LastRecMsg.front();
        while (--cnt_all) {
            LastRecMsg.pop_back();
        }
    }
*/
