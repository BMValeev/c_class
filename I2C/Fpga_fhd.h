//
// Created by eleps on 29.04.19.
//

#ifndef HARDWARECLASSES_FPGA_FULLHD_H
#define HARDWARECLASSES_FPGA_FULLHD_H


#include "../Rest/defs.h"
#include "i2cpacket.h"
#include <vector>
#include <map>
#define FPGA_MODULE_ADDRESS 0x70
#define FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class Fpga_fhd : public I2CPacket
{
public:
    Fpga_fhd(std::string filename, LogCallback cb = printToCout);

    // Commands
    uint8_t whiteBalance(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setRed(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setGreen(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setBlue(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setExposition(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setAnalysisSurface(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setColorness(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setHue(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setContrast(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setSharpness(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setFreeze(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setResolution(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setScan(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setGamma(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setGammaRate(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setBrightness(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t pixelCorrection(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t SetScene(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t saveScene(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t checkStatus(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts =  FPGA_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t returnValue(std::vector<uint8_t> &response);
    enum Cmd
    {
        WHITE_BALANCE = 0x01,
        SET_RED = 0x02,
        SET_BLUE = 0x03,
        SET_GREEN = 0x04,
        SET_EXPOSITION = 0x05,
        SET_ANALYSIS_RATE = 0x06,
        SET_COLOR = 0x07,
        SET_HUE = 0x08,
        SET_CONTRAST = 0x09,
        SET_SHARPNESS = 0x0A,
        FREEZE = 0x0B,
        SET_RESOLUTION= 0x0C,
        SET_SCANNING_MODE = 0x0D,
        SET_GAMMA = 0x0E,
        SET_GAMMA_RATE = 0x0F,
        SET_BRIGHTNESS = 0x10,
        CORRECT_PIXELS = 0x11,
        SET_SCENE = 0x12,
        SAVE_SCENE = 0x13,
        CHECK_STATUS = 0x14
    };

};



#endif //HARDWARECLASSES_FPGA_FULLHD_H
