/*
 *Created by eleps on 27.04.18.
*/
#ifndef C_CLASS_SPI_H
#define C_CLASS_SPI_H

#include <string>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <mutex>

#include "../defs.h"
#include "../Rest/loggable.h"

#define ACK_SPI 0x30
#define EXEC_SPI 0x03
#define TR_ERR_SPI 0x05
#define OK_SPI 0x00
#define NACK 0x02
#define NOK_SPI 0x01
#define TR_ERR_SPI 0x05
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define PACKED_LENGTH_SPI 32

// SPI Settings
#ifndef SPI_SPEED_HZ
    #define SPI_SPEED_HZ 100000
#endif
#ifndef SPI_BITS_PER_WORD
    #define SPI_BITS_PER_WORD 8
#endif
#ifndef SPI_MODE
    #define SPI_MODE SPI_MODE_0 // SPI_MODE_0 is defined in spidev.h
#endif
#ifndef SPI_TX_RX_PAUSE_US
    #define SPI_TX_RX_PAUSE_US 3000 // delay between transmit & receive portion
#endif
#ifndef SPI_RX_BUFFER_SIZE
    #define SPI_RX_BUFFER_SIZE 10
#endif

// Low level class that implements basic information exchange via SPI on Hamming board
class SPI : public Loggable
{
public:
    static SPI & getInstance();

    uint8_t begin(std::string deviceName, LogCallback cb = printToCout);
    std::vector<uint8_t> recData() const { return mLastRecMsg; }
    void resetRecData() { std::fill(mLastRecMsg.begin(), mLastRecMsg.end(), 0); }
    bool isInitialized() const { return theOneTrueInstance != nullptr; }
    bool isHardwareInitialized() const { return mHardwareInitialized; }

    // Transmission
    uint8_t transaction(std::vector<uint8_t> &buffer, uint8_t ansLen, uint16_t pause = SPI_TX_RX_PAUSE_US);

protected:
    SPI();
    ~SPI() { }

private:
    static SPI* theOneTrueInstance;
    bool mInit;
    std::string mDeviceName;
    std::vector<uint8_t> mLastRecMsg;
    std::mutex mMutex;
    int mSpifd;
    uint32_t mSpeed;
    bool mHardwareInitialized;
    uint8_t mMode;
    uint8_t mBitsPerWord;

    // Helpers
    void setDeviceName(std::string name);

};
#endif //C_CLASS_SPI_H
