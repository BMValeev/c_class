//
// Created by eleps on 29.04.19.
//

#include "Board_FHD.h"
#include "I2C.h"
Board_fhd::Board_fhd(std::string filename, LogCallback cb)
        : I2CPacket(filename, BOARD_FHD_MODULE_ADDRESS, cb) {
    I2C &i2c = I2C::getInstance();
    i2c.begin(filename, cb);
}
// Commands
uint8_t Board_fhd::getStatus(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts ){
    response=writeArray(GET_STATUS, data, 3, attempts);
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
uint8_t Board_fhd::setDisabled(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts ){
    response=writeArray(SET_DISABLED, data, 3, attempts);
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
