//
// Created by eleps on 07.02.19.
//

#include "BoardModule.h"

#include "../Rest/crc.h"
#include "I2C.h"

// BoardModule class
// Construction and destruction
BoardModule::BoardModule(std::string filename, LogCallback cb)
    : I2CPacket(filename, BOARD_MODULE_ADDRESS, cb)
{ }

uint8_t BoardModule::setBonding(uint8_t enable, std::vector<uint8_t> &response, int attempts)
{
    std::vector<uint8_t> msg;
    msg.push_back(enable);
    response = writeArray(SET_BONDING, msg, 4, attempts);
    if (response.size() != 2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::getVersion(std::vector<uint8_t> &response, int attempts)
{
    std::vector<uint8_t> msg;
    response = writeArray(GET_VERSION, msg, 4, attempts);
    if (response.size() != 2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::getTools(std::vector<uint8_t> &response, int attempts)
{
    std::vector<uint8_t> msg;
    response = writeArray(GET_TOOLS, msg, 4, attempts);
    if (response.size() != 2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetTools failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetTools succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::getPower(std::vector<uint8_t> &responce, int attempts)
{
    std::vector<uint8_t> msg;
    responce = writeArray(GET_POWER, msg, 3, attempts);
    if (responce.size() != 1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetPower failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetPower succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::setEnergy(uint8_t energy, std::vector<uint8_t> &responce, int attempts)
{
    std::vector<uint8_t> msg;
    msg.push_back(energy);
    responce = writeArray(SET_ENERGY, msg, 3, attempts);
    if (responce.size() != 1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "SetEnergy failed");
        return NOK_I2C;
    }
    if (responce.front() == ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "SetEnergy succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "SetEnergy failed");
    return NOK_I2C;
}

uint8_t BoardModule::setVolume(uint8_t volume,std::vector<uint8_t> &responce, int attempts)
{
    std::vector<uint8_t> msg;
    msg.push_back(volume);
    responce = writeArray(SET_VOLUME, msg, 3, attempts);
    if (responce.size() != 1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "SetVolume failed");
        return NOK_I2C;
    }
    if (responce.front() == ACK_I2C) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "ReadLastChangedValue succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "SetVolume failed");
    return NOK_I2C;
}

#ifdef C_CLASS_DEBUG
void printToC(uint8_t status, std::string msg)
{
    std::cout << status << msg << std::endl;
}
int main(void)
{
    std::string filename="/dev/i2c-2";
    std::cout<<"1"<<std::endl;
    BoardModule mcu(filename,printToC);
    std::cout<<"2"<<std::endl;
    std::vector<uint8_t> data;
    mcu.getTools(data);
    std::cout<<"3\n";
    printf("%zu\n",data.size());
    return 1;
}
#endif
