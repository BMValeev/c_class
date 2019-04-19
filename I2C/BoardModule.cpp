//
// Created by eleps on 07.02.19.
//

#include "BoardModule.h"

#include "../Rest/crc.h"
#include "I2C.h"

// BoardModule class
// Construction and destruction
BoardModule::BoardModule(std::string filename, LogCallback cb)
    : Loggable(cb)
    , mAddr(BOARD_MODULE_ADDRESS)
    , mAttempts(BOARD_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER)
{
    I2C &i2c = I2C::getInstance();
    i2c.begin(filename);
}

uint8_t BoardModule::setBonding(uint8_t enable,std::vector<uint8_t> &response)
{
    std::vector<uint8_t> msg;
    msg.push_back(enable);
    response = writeArray(0x06, msg, 4);
    if (response.size() != 2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::getVersion(std::vector<uint8_t> &response)
{
    std::vector<uint8_t> msg;
    response = writeArray(0x01, msg, 4);
    if (response.size() != 2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetVersion succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::getTools(std::vector<uint8_t> &response)
{
    std::vector<uint8_t> msg;
    response = writeArray(0x02, msg, 4);
    if (response.size() != 2) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetTools failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetTools succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::getPower(std::vector<uint8_t> &responce)
{
    std::vector<uint8_t> msg;
    responce = writeArray(0x03, msg, 3);
    if (responce.size() != 1) {
        printLog(DebugLog, static_cast<std::string>(__func__) + "GetPower failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "GetPower succesfuly");
    return OK_I2C;
}

uint8_t BoardModule::setEnergy(uint8_t energy,std::vector<uint8_t> &responce)
{
    std::vector<uint8_t> msg;
    msg.push_back(energy);
    responce = writeArray(0x04, msg, 3);
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

uint8_t BoardModule::setVolume(uint8_t volume,std::vector<uint8_t> &responce)
{
    std::vector<uint8_t> msg;
    msg.push_back(volume);
    responce = writeArray(0x05, msg, 3);
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

std::vector<uint8_t> BoardModule::writeArray(uint8_t command,std::vector<uint8_t> data,uint32_t len)
{
    I2C& i2c = I2C::getInstance();
    uint32_t cnt = mAttempts;
    uint8_t l_len=2;
    std::vector<uint8_t> msg, answer,null,l_answer;
    l_len += static_cast<uint8_t>(data.size());
    msg.push_back(static_cast<uint8_t>(mAddr << 1));
    msg.push_back(l_len);
    msg.push_back(command);
    for(uint32_t i=0;i<data.size();i++) {
        msg.push_back(data[i]);
    }
    msg.push_back(CRC::crc8(msg.data(),msg.size()));
    msg.erase(msg.begin());
    while(cnt--) {
        if(i2c.transaction(mAddr,msg,len)==OK_I2C){
            answer=i2c.recData();
            l_len=answer.front()+1;
            l_answer.clear();
            l_answer.push_back(static_cast<uint8_t>(mAddr << 1));
            while (l_len--) {
                l_answer.push_back(answer.front());
                answer.erase(answer.begin());
            }
            if (CRC::crc8(l_answer.data(),l_answer.size()-1)==l_answer.back()) {
                printLog(DebugLog, static_cast<std::string>(__func__) + "CRC Ok");
                l_answer.erase(l_answer.begin());
                l_answer.erase(l_answer.begin());
                l_answer.pop_back();
                return l_answer;
            }
            printLog(DebugLog, static_cast<std::string>(__func__) + "CRC NOK");
        }
    }
    return null;
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
