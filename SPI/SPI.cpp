/*
 *Created by eleps on 27.04.18.
*/
#include "SPI.h"

#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <cassert>

#include "../Rest/crc.h"

#include <QDebug> // Remove this!!!

// SPI class
SPI* SPI::theOneTrueInstance = nullptr;

SPI &SPI::getInstance()
{
    if (theOneTrueInstance == nullptr) {
        // Allocate on heap so that it does not get destroyed after
        theOneTrueInstance = new SPI();
    }
    return *theOneTrueInstance;
}

SPI::SPI()
    : Loggable()
    , mInit(false)
    , mHardwareInitialized(false)
{
    printLog(DebugLog, static_cast<std::string>(__func__) + "SPI constructor called");
    if (theOneTrueInstance != nullptr) throw std::logic_error("Instance already exists");

    theOneTrueInstance = this;
}

/* Used for configuration of the device in construction */
void SPI::setDeviceName(std::string name)
{
    if (mDeviceName == name)
        return;

    mDeviceName = name;
    printLog(InfoLog, "SPI --> new device name set - " + mDeviceName);
}

uint8_t SPI::begin(std::string deviceName, LogCallback cb) /*Need to check*/
{
    // Lock the mutex first
    std::lock_guard<std::mutex> lock(mMutex); // automatically unlocks when function is leaved, no need to call unlock

    setLogCallback(cb);
    printLog(DebugLog, static_cast<std::string>(__func__) + " started");

    // Setups
    mLastRecMsg.reserve(SPI_RX_BUFFER_SIZE); // will always have fixed size
    setDeviceName(deviceName);

    // Initialization
    mHardwareInitialized = false;
    mMode = SPI_MODE;
    mBitsPerWord = SPI_BITS_PER_WORD;
    mSpeed = SPI_SPEED_HZ;

    mSpifd = -1;
    mSpifd = open(mDeviceName.c_str(), O_RDWR);
    if(mSpifd < 0){
        printLog(CriticalLog, static_cast<std::string>(__func__) + " could not open SPI device");
        return NOK_SPI;
    }

    int result = -1;
    result = ioctl(mSpifd, SPI_IOC_WR_MODE, &mMode);
    if(result < 0){
        printLog(CriticalLog,static_cast<std::string>(__func__) + " could not set SPIMode (WR)...ioctl fail");
        return NOK_SPI;
    }

    result = -1;
    result = ioctl(mSpifd, SPI_IOC_RD_MODE, &mMode);
    if(result < 0) {
        printLog(CriticalLog, static_cast<std::string>(__func__) + " could not set SPIMode (RD)...ioctl fail");
        return NOK_SPI;
    }

    result = -1;
    result = ioctl(mSpifd, SPI_IOC_WR_BITS_PER_WORD, &mBitsPerWord);
    if(result < 0) {
        printLog(CriticalLog,static_cast<std::string>(__func__) + " could not set SPI bitsPerWord (WR)...ioctl fail");
        return NOK_SPI;
    }

    result = -1;
    result = ioctl(mSpifd, SPI_IOC_RD_BITS_PER_WORD, &mBitsPerWord);
    if(result < 0) {
        printLog(CriticalLog,static_cast<std::string>(__func__) + " could not set SPI bitsPerWord(RD)...ioctl fail");
        return NOK_SPI;
    }

    result = -1;
    result = ioctl(mSpifd, SPI_IOC_WR_MAX_SPEED_HZ, &mSpeed);
    if(result < 0) {
        printLog(CriticalLog,static_cast<std::string>(__func__) + " could not set SPI speed (WR)...ioctl fail");
        return NOK_SPI;
    }

    result = -1;
    result = ioctl(mSpifd, SPI_IOC_RD_MAX_SPEED_HZ, &mSpeed);
    if(result < 0) {
        printLog(CriticalLog, static_cast<std::string>(__func__) + " could not set SPI speed (RD)...ioctl fail ");
        return NOK_SPI;
    }

    // The SPI is launched correctly
    mHardwareInitialized = true;
    mInit = true;

    printLog(DebugLog, static_cast<std::string>(__func__)+ " ended succesfully");

    return OK_SPI;
}

uint8_t SPI::transaction(std::vector<uint8_t>& buffer, uint8_t ansLen, uint16_t pause) /*Need to check*/
{
    // Lock the mutex first
    std::lock_guard<std::mutex> lock(mMutex); // automatically unlocks when function is leaved, no need to call unlock

    printLog(DebugLog, static_cast<std::string>(__func__) + " started");
    printLog(DebugLog, static_cast<std::string>(__func__) + std::to_string(buffer.size()));

    // uint8_t receive[ansLen];
    mLastRecMsg.resize(ansLen, 0);
    resetRecData();
    spi_ioc_transfer send[2];
    // Tx config
    send[0].tx_buf = reinterpret_cast<unsigned long>(buffer.data());
    send[0].rx_buf = reinterpret_cast<unsigned long>(nullptr);
    send[0].len = static_cast<unsigned int>(buffer.size());
    send[0].delay_usecs = pause;
    send[0].speed_hz = mSpeed;
    send[0].bits_per_word = mBitsPerWord;
    send[0].tx_nbits = 0;
    send[0].rx_nbits = 0;
    send[0].pad = 0;
    send[0].cs_change = 0;
    // Rx config
    send[1].tx_buf = reinterpret_cast<unsigned long>(nullptr);
    // send[1].rx_buf = (unsigned long)receive;
    send[1].rx_buf = reinterpret_cast<unsigned long>(mLastRecMsg.data());
    send[1].len = ansLen;
    send[1].delay_usecs = 0;
    send[1].speed_hz = mSpeed;
    send[1].bits_per_word = mBitsPerWord;
    send[1].tx_nbits = 0;
    send[1].rx_nbits = 0;
    send[1].pad = 0;
    send[1].cs_change = 0;

    // Perform communication
    int result = -1;
    result = ioctl(mSpifd, SPI_IOC_MESSAGE(2), &send);
    if(result < 0)
    {
        printLog(WarningLog, static_cast<std::string>(__func__) + static_cast<std::string>(strerror(errno)) + " error ocurred during transmission" );
        return NOK_SPI;
    }
    qWarning() << "received buffer = " << mLastRecMsg;

    printLog(DebugLog, static_cast<std::string>(__func__) + " ended succesfully");
    return OK_SPI;
}




