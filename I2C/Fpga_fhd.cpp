//
// Created by eleps on 29.04.19.
//

#include "Fpga_fhd.h"
#include "I2C.h"

Fpga_fhd::Fpga_fhd(std::string filename, LogCallback cb)
        : I2CPacket(filename, FPGA_MODULE_ADDRESS, cb)
{
    I2C &i2c = I2C::getInstance();
    i2c.begin(filename, cb);
}
// Commands
uint8_t Fpga_fhd::whiteBalance(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts ){
    response=writeArray(WHITE_BALANCE, data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setRed(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts ){
    response=writeArray(SET_RED, data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setGreen(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts ){
    response=writeArray(SET_GREEN,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setBlue(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts ){
    response=writeArray(SET_BLUE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setExposition(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_EXPOSITION,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setAnalysisSurface(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_ANALYSIS_RATE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setColorness(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_COLOR,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setHue(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_HUE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setContrast(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_CONTRAST,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setSharpness(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_SHARPNESS,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setFreeze(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(FREEZE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setResolution(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_RESOLUTION,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setScan(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_SCANNING_MODE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setGamma(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_GAMMA,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setGammaRate(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_GAMMA_RATE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::setBrightness(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_BRIGHTNESS,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::pixelCorrection(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(CORRECT_PIXELS,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::SetScene(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SET_SCENE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::saveScene(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(SAVE_SCENE,  data, 3, attempts);
    if (response.size()!=1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "  succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "  failed, NACK");
    return NOK_I2C;
}
uint8_t Fpga_fhd::checkStatus(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts  ){
    response=writeArray(CHECK_STATUS,  data, 3, attempts);
    if (response.size()!=2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + " failed, NACK");
    return NOK_I2C;  
}
