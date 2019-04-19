//
// Created by eleps on 07.02.19.
//

#include "ConnModule.h"

#include "../Rest/crc.h"
#include "I2C.h"

// ConnModule class
// Construction and destruction
ConnModule::ConnModule(std::string filename, LogCallback cb)
    : I2CPacket(filename, CONN_MODULE_ADDRESS, cb)
{
    I2C &i2c = I2C::getInstance();
    i2c.begin(filename, cb);
}

uint8_t ConnModule::setUUID(uint32_t uuid, std::vector<uint8_t> &response)
{
    // Construct uuid array of bytes
    std::vector<uint8_t> uuid_b;
    uuid_b.push_back(uuid & 0xFF);
    uuid_b.push_back((uuid>>8)  & 0xFF);
    uuid_b.push_back((uuid>>16) & 0xFF);
    uuid_b.push_back((uuid>>24) & 0xFF);
    // Send
    response=writeArray(0x01, uuid_b, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "Set UUID failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "Set UUID succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "Set UUID failed, NACK");
    return NOK_I2C;
}

uint8_t ConnModule::setName(std::vector<uint8_t> data, std::vector<uint8_t> &response)
{
    response=writeArray(0x02, data, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "Set SetName failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "Set SetName succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "Set SetName failed, NACK");
    return NOK_I2C;
}

uint8_t ConnModule::startInit(std::vector<uint8_t> &response)
{
    std::vector<uint8_t> data;
    response=writeArray(0x03, data, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "StartInit failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "StartInit succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "StartInit failed, NACK");
    return NOK_I2C;
}

uint8_t ConnModule::writeString(std::vector<uint8_t> data, std::vector<uint8_t> &response)
{
    response=writeArray(0x04, data, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "WriteString failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "WriteString succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "WriteString failed, NACK");
    return NOK_I2C;
}

uint8_t ConnModule::endInit(std::vector<uint8_t> &response)
{
    std::vector<uint8_t> data;
    response=writeArray(0x05, data, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "EndInit failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "EndInit succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "EndInit failed, NACK");
    return NOK_I2C;
}

uint8_t ConnModule::checkBonding(std::vector<uint8_t> &response)
{
    std::vector<uint8_t> data;
    response=writeArray(0x09, data, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "CheckBonding failed");
        return NOK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "CheckBonding failed succesfuly");
    return OK_I2C;
}

uint8_t ConnModule::writeValue(std::vector<uint8_t> id, std::vector<uint8_t> value, std::vector<uint8_t> &response)
{
    std::vector<uint8_t> data;
    data.insert(std::end(data), std::begin(id), std::end(id));
    data.insert(std::end(data), std::begin(value), std::end(value));
    response=writeArray(0x06, data, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "WriteValue failed, wrong responce");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "WriteValue succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "WriteValue failed, NACK");
    return NOK_I2C;
}

uint8_t ConnModule::startBonding(std::vector<uint8_t> db, std::vector<uint8_t> &response)
{
    response=writeArray(0x08, db, 3);
    if (response.size()!=1)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "StartBonding failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "StartBonding succesfuly");
        return OK_I2C;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "StartBonding failed");
    return NOK_I2C;
}

uint8_t ConnModule::readValue(uint16_t id_rec, std::map<uint16_t,std::vector<uint8_t>> answer) /*change order of message*/
{
    std::vector<uint8_t> responce;
    std::vector<uint8_t> data,val;
    //uint16_t id=0x0000;
    //uint32_t value=0x00000000;
    data.push_back(id_rec&0xff);
    data.push_back((id_rec>>8)&0xff);
    responce=writeArray(0x07, data, 8);
    if (responce.size()==0)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "ReadValue failed");
        return NOK_I2C;
    }
    if (responce.size()%4)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "ReadValue failed");
        return NOK_I2C;
    }
    /*id=responce.front()<<8;
    responce.erase(responce.begin());
    id|=responce.front();
    responce.erase(responce.begin());*/
    //id=id_rec;
    val.push_back(responce.front());
    responce.erase(responce.begin());
    val.push_back(responce.front());
    responce.erase(responce.begin());
    val.push_back(responce.front());
    responce.erase(responce.begin());
    val.push_back(responce.front());
    responce.erase(responce.begin());
    answer.insert (std::pair<uint16_t,std::vector<uint8_t>>(id_rec,val));
    printLog(DebugLog, static_cast<std::string>(__func__) + "ReadValue succesfuly");
    return OK_I2C;

}
uint8_t ConnModule::readLastChangedValue(std::map<uint16_t,std::vector<uint8_t>>& answer) /*change order of message*/
{
    std::vector<uint8_t> response;
    std::vector<uint8_t> data, val;
    uint16_t id;

    response=writeArray(0x0C, data, 10);
    if (response.size()==0)
    {
        return OK_I2C;
    }
    if (response.size()%6)
    {
        printLog(DebugLog, static_cast<std::string>(__func__) + "ReadLastChangedValue failed");
        return NOK_I2C;
    }
    while(response.size()!=0)
    {
        id=0x0000;
        //value=0x00000000;
        val.clear();
        id = static_cast<uint16_t>(response.front() << 8);
        response.erase(response.begin());
        id |= response.front();
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        answer.insert(std::pair<uint16_t,std::vector<uint8_t>>(id,val) );
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "ReadLastChangedValue succesfuly");
    return OK_I2C;
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
    ConnModule mcu(filename,printToC);
    std::cout<<"2"<<std::endl;
    std::vector<uint8_t> data;
    mcu.StartInit(data);
    std::cout<<"3\n";
    printf("%zu\n",data.size());
    return 1;
}
#endif
