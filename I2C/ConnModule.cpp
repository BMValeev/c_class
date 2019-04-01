//
// Created by eleps on 07.02.19.
//
using namespace std;
#include "I2C.h"
#include "ConnModule.h"
#include "../Rest/crc.h"
// ConnModule class
// Construction and destruction
ConnModule::ConnModule(std::string filename,CallbackFunction cb) {
    I2C &ptrI2C = I2C::getInstance(cb);
    std::vector<unsigned char> address;
    this->m_cb=cb;
    ptrI2C.begin(filename);
    address.push_back(0b01110000);
    this->addr=address;
}
ConnModule::~ConnModule() {

}

std::vector<unsigned char> ConnModule::getAddress(void) {
    return this->addr;
}
void ConnModule::setAddress(std::vector<unsigned char> addr)
{
    unsigned char temp_addr=addr.front();
    addr.pop_back();
    addr.push_back(temp_addr&0x7f);
    this->addr=(addr);
}
uint8_t ConnModule::SetUUID(uint32_t uuid, std::vector<unsigned char> &response)
{
    // Construct uuid array of bytes
    std::vector<uint8_t> uuid_b;
    uuid_b.push_back(uuid & 0xFF);
    uuid_b.push_back((uuid>>8)  & 0xFF);
    uuid_b.push_back((uuid>>16) & 0xFF);
    uuid_b.push_back((uuid>>24) & 0xFF);
    // Send
    response=WriteArray(0x01, uuid_b, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set UUID failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set UUID succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set UUID failed, NACK");
    return NOK_I2C;
}
uint8_t ConnModule::SetName(std::vector<unsigned char> data, std::vector<unsigned char> &response)
{
    response=WriteArray(0x02, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set SetName failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set SetName succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set SetName failed, NACK");
    return NOK_I2C;
}
uint8_t ConnModule::StartInit(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    response=WriteArray(0x03, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartInit failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartInit succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartInit failed, NACK");
    return NOK_I2C;
}
uint8_t ConnModule::WriteString(std::vector<unsigned char> data, std::vector<unsigned char> &response)
{
    response=WriteArray(0x04, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteString failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteString succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteString failed, NACK");
    return NOK_I2C;
}
uint8_t ConnModule::EndInit(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    response=WriteArray(0x05, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"EndInit failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"EndInit succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"EndInit failed, NACK");
    return NOK_I2C;
}
uint8_t ConnModule::CheckBonding(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    response=WriteArray(0x09, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"CheckBonding failed");
        return NOK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"CheckBonding failed succesfuly");
    return OK_I2C;
}

uint8_t ConnModule::WriteValue(std::vector<unsigned char> id, std::vector<unsigned char> value, std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    data.insert(std::end(data), std::begin(id), std::end(id));
    data.insert(std::end(data), std::begin(value), std::end(value));
    response=WriteArray(0x06, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteValue failed, wrong responce");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteValue succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteValue failed, NACK");
    return NOK_I2C;
}
uint8_t ConnModule::StartBonding(std::vector<unsigned char> db, std::vector<unsigned char> &response)
{
    response=WriteArray(0x08, db, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartBonding failed");
        return NOK_I2C;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartBonding succesfuly");
        return OK_I2C;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartBonding failed");
    return NOK_I2C;
}
uint8_t ConnModule::ReadValue(uint16_t id_rec, std::map <uint16_t,std::vector<unsigned char> > answer) /*change order of message*/
{
    std::vector<unsigned char> responce;
    std::vector<unsigned char> data,val;
    //uint16_t id=0x0000;
    //uint32_t value=0x00000000;
    data.push_back(id_rec&0xff);
    data.push_back((id_rec>>8)&0xff);
    responce=WriteArray(0x07, data, 8);
    if (responce.size()==0)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadValue failed");
        return NOK_I2C;
    }
    if (responce.size()%4)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadValue failed");
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
    answer.insert (pair<uint16_t,std::vector<unsigned char>>(id_rec,val));
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadValue succesfuly");
    return OK_I2C;

}
uint8_t ConnModule::ReadLastChangedValue(std::map <uint16_t,std::vector<unsigned char>> answer) /*change order of message*/
{
    std::vector<unsigned char> response;
    std::vector<unsigned char> data, val;
    uint16_t id;
    //uint32_t value;
    response=WriteArray(0x0C, data, 10);
    if (response.size()==0)
    {
        return OK_I2C;
    }
    if (response.size()%6)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue failed");
        return NOK_I2C;
    }
    while(response.size()!=0)
    {
        id=0x0000;
        //value=0x00000000;
        val.clear();
        id=response.front()<<8;
        response.erase(response.begin());
        id|=response.front();
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        val.push_back(response.front());
        response.erase(response.begin());
        answer.insert ( pair<uint16_t,std::vector<unsigned char>>(id,val) );
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue succesfuly");
    return OK_I2C;
}




std::vector<unsigned char> ConnModule::WriteArray(uint8_t command,std::vector<unsigned char> data,unsigned int len)
{
    I2C & ptrI2C=I2C::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned int error;
    unsigned char l_len=2;
    std::vector<unsigned char> msg, answer,null,l_answer;
    l_len=l_len+data.size();
    msg.push_back(getAddress().front()<<1);
    msg.push_back(l_len);
    msg.push_back(command);
    for(unsigned int i=0;i<data.size();i++) {
        msg.push_back(data[i]);
    }
    msg.push_back(CRC::crc8(msg.data(),msg.size()));
    msg.erase(msg.begin());
    while(cnt--) {
        if(ptrI2C.transaction(this->addr,msg,len)==OK_I2C){
            answer=ptrI2C.recData();
            l_len=answer.front()+1;
            l_answer.clear();
            l_answer.push_back(getAddress().front()<<1);
            while (l_len--) {
                l_answer.push_back(answer.front());
                answer.erase(answer.begin());
            }
            if (CRC::crc8(l_answer.data(),l_answer.size()-1)==l_answer.back()) {
                PrintLog(Debug_log,(std::string) __func__ +(std::string)"CRC Ok");
                l_answer.erase(l_answer.begin());
                l_answer.erase(l_answer.begin());
                l_answer.pop_back();
                return l_answer;
            }
            PrintLog(Debug_log,(std::string) __func__ +(std::string)"CRC NOK");
        }
    }
    if (command==0x0C) {
        null.push_back('c');
        return null;
    }
    return null;
}
void ConnModule::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=0)
    {
        m_cb(status,text);
    }
}

void ConnModule::PrintToCout(uint8_t status, string msg)
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
    ConnModule mcu(filename,PrintToC);
    cout<<"2"<<endl;
    std::vector<unsigned char> data;
    mcu.StartInit(data);
    cout<<"3\n";
    printf("%zu\n",data.size());
    return 1;
}
#endif
