//
// Created by eleps on 07.02.19.
//
using namespace std;
#include "crc.h"
#include "BoardModule.h"

// BoardModule class
// Construction and destruction
BoardModule::BoardModule(std::string filename, LogCallback cb)
{
    I2C &ptrI2C = I2C::getInstance(cb);
    //this->m_cb=0;
    this->m_cb=cb;
    ptrI2C.begin(filename);
    std::vector<unsigned char> address;
    address.push_back(0b01000000); // changed to 0x80 (we write 0x40)
    //printf("%02x\n",address.front());
    this->addr=address;
}
BoardModule::~BoardModule()
{

}

std::vector<unsigned char> BoardModule::getAddress(void)
{
    return this->addr;
}
uint8_t BoardModule::SetBonding(unsigned char enable,std::vector<unsigned char> &response)
{
    std::vector<unsigned char> msg;
    msg.push_back(enable);
    response = WriteArray(0x06, msg, 4);
    if (response.size() != 2) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion failed");
        return NOK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion succesfuly");
    return OK_I2C;
}
void BoardModule::setAddress(std::vector<unsigned char> addr)
{
    unsigned char temp_addr=addr.front();
    addr.pop_back();
    addr.push_back(temp_addr&0x7f);
    this->addr=(addr);
}
uint8_t BoardModule::GetVersion(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> msg;
    response = WriteArray(0x01, msg, 4);
    if (response.size() != 2) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion failed");
        return NOK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion succesfuly");
    return OK_I2C;
}
uint8_t BoardModule::GetTools(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> msg;
    response = WriteArray(0x02, msg, 4);
    if (response.size() != 2) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetTools failed");
        return NOK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetTools succesfuly");
    return OK_I2C;
}
uint8_t BoardModule::GetPower(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    responce = WriteArray(0x03, msg, 3);
    if (responce.size() != 1) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetPower failed");
        return NOK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetPower succesfuly");
    return OK_I2C;
}
uint8_t BoardModule::SetEnergy(unsigned char energy,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    msg.push_back(energy);
    responce = WriteArray(0x04, msg, 3);
    if (responce.size() != 1) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetEnergy failed");
        return NOK_I2C;
    }
    if (responce.front() == ACK_I2C) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetEnergy succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetEnergy failed");
    return NOK_I2C;
}
uint8_t BoardModule::SetVolume(unsigned char volume,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    msg.push_back(volume);
    responce = WriteArray(0x05, msg, 3);
    if (responce.size() != 1) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetVolume failed");
        return NOK_I2C;
    }
    if (responce.front() == ACK_I2C) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetVolume failed");
    return NOK_I2C;
}
std::vector<unsigned char> BoardModule::WriteArray(uint8_t command,std::vector<unsigned char> data,unsigned int len)
{
    I2C & ptrI2C=I2C::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned char *array=data.data();
    unsigned int error;
    unsigned char l_len=2;
    std::vector<unsigned char> msg, answer,null;
    l_len=l_len+data.size();
    msg.push_back(l_len);
    msg.push_back(command);
    for(int i=data.size()-1;i>=0;i--) {
        msg.push_back(array[i]);
    }
    msg.push_back(CRC::crc8(msg.data(),msg.size()));
    while(cnt--) {
        if(ptrI2C.transaction(this->addr,msg,len)==OK_I2C){
            answer=ptrI2C.recData();
            l_len=answer.front();
            while (answer.size() > l_len) {
                answer.pop_back();
            }
            if (CRC::crc8(answer.data(),answer.size()-1)==answer.back()) {
                answer.erase(answer.begin());
                answer.pop_back();
                return answer;
            }
        }
    }
    return null;
}
void BoardModule::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=0)
    {
        m_cb(status,text);
    }
}

void BoardModule::PrintToCout(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}

#ifdef C_CLASS_DEBUG
void PrintToC(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
int main(void)
{
    std::string filename="/dev/i2c-2";
    cout<<"1"<<endl;
    BoardModule mcu(filename,PrintToC);
    cout<<"2"<<endl;
    std::vector<unsigned char> data;
    mcu.GetTools(data);
    cout<<"3\n";
    printf("%zu\n",data.size());
    return 1;
}
#endif
