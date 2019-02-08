/*
 *Created by eleps on 27.04.18.
 */
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include "crc.h"
#include "I2C.h"

// I2C class
I2C* I2C::theOneTrueInstance;

// Construction and destruction
I2C::I2C(LogCallback cb) {
    this->m_cb=cb;
    this->PrintLog(Debug_log,(std::string) __func__+  (std::string)"I2C constructor called");
    if (theOneTrueInstance) throw std::logic_error("Instance already exists");
    theOneTrueInstance = this;
}
I2C::~I2C() { }
I2C& I2C::getInstance(LogCallback cb) {
    if (!theOneTrueInstance) initInstance(cb);
    return *theOneTrueInstance;
}
//void I2C::initInstance(CallbackFunction cb) { new I2C(cb); }
void I2C::initInstance(LogCallback cb) { new I2C(cb); }

std::vector<unsigned char> I2C::recData(void)
{

    return LastRecMsg;
}
void I2C::CleanRecMsg(void)
{
    /*
         * Explicitly cleans last read buffer
         */
    this->LastRecMsg.clear();
}
void I2C::SetDeviceName(std::string Name)
{
    /*
         * Used for configuration of the device in construction
         */
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Device name set to :" +(std::string)DeviceName);
    this->DeviceName=Name;
}
void I2C::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=nullptr)
    {
        m_cb(status,text);
    }
}

void I2C::PrintToCout(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}

/*Unsafe Methods*/
int I2C::SendRaw_new(std::vector<unsigned char> address, std::vector<unsigned char> buffer, unsigned int rlen)
{
    int ret;
    //int errnum;
    unsigned int cnt, cnt_all;
    /*unsigned char buf[10]= {0};
    buf[0] = 0b11100000;
    buf[1] = 0b0000001;
    buf[2] = 0b0000011;
    buf[3] = 0b0000111;
    buf[4] = 0b0001111;
    buf[5] = 0b0011111;
    buf[6] = CRC8(buf,6);*/

    /*PrintLog(Debug_log,(std::string) __func__+  (std::string)"Send to address:"
                       +(std::string)address +(std::string)"Data: "+(std::string)buffer);*/
    unsigned char buf_rec[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
            ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    std::string data_send,data_rec;
    //data_send=data_send+(std::string)(address.front());
    for (unsigned int cnt=0;cnt<buffer.size();cnt++)
    {
        //data_send=data_send+(std::string)(buffer.data()[cnt]);
    }
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Send message"
                       +data_send);
    i2c_rdwr_ioctl_data message;
    i2c_msg message_packet[2];
    //printf("%02x\n",address.front());
    message_packet[0].addr=address.front();
    message_packet[0].flags=0;
    message_packet[0].len=buffer.size();
    message_packet[0].buf=buffer.data();
    message_packet[1].addr=address.front();
    message_packet[1].flags=I2C_M_RD;//I2C_M_RD|I2C_M_RECV_LEN;I2C_M_NOSTART
    message_packet[1].buf=buf_rec;
    message_packet[1].len=10;
    if (rlen==0) {
        message_packet[1].len=10;
        cnt_all=10;
    }
    else if (rlen>10) {
        message_packet[1].len=20;
        cnt_all=20;
    }
    else {
        message_packet[1].len=rlen;
        cnt_all=rlen;
    }
    memset(&message, 0, sizeof(message));
    message.msgs=message_packet;
    if (rlen!=0) {
        message.nmsgs=2;
    }
    else {
        message.nmsgs=1;
    }
    int file = open(this->DeviceName.c_str(), O_RDWR);
    //cout<<"i2copen"<<endl;
    if (file == -1)
    {
        /*errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));*/
        PrintLog(Warning_log,(std::string) __func__+  (std::string)"Device open error");
        close(file);
        return NOK_I2C;
        //perror("/dev/i2c-2");
    }
    if (ioctl(file, I2C_SLAVE, address.front()) < 0)
    {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)"Failed to acquire bus access and/or talk to slave");
        //perror("Failed to acquire bus access and/or talk to slave");
        close(file);
        return NOK_I2C;
    }
    ret=ioctl(file, I2C_RDWR, &message);
    if (ret<0) {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)strerror(-ret)
                             +(std::string)"Unable to send message");
        //fprintf (stderr, "%s.\n", strerror(-ret));
        close(file);
        return NOK_I2C;
    }
    close(file);
    CleanRecMsg();
    for (cnt = 0; cnt < (cnt_all); cnt++) {
        this->LastRecMsg.push_back(buf_rec[cnt]);
    }
    if((this->LastRecMsg.front()==0x00)) {
        return NOK_I2C;
    }
    if((this->LastRecMsg.front()==0xff)) {
        return NOK_I2C;
    }
    cnt_all=cnt_all-this->LastRecMsg.front();
    while (--cnt_all) {
        this->LastRecMsg.pop_back();
    }
    for (cnt=0;cnt<this->LastRecMsg.size();cnt++) {
        //data_rec=data_rec+(std::string)(this->LastRecMsg.data()[cnt]);
    }
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Send message"
                       +data_rec);
    return OK_I2C;
}

int I2C::SendRaw(std::vector<unsigned char> address,std::vector<unsigned char>, unsigned int len)
{
    this->LastRecMsg.clear();
    this->LastRecMsg.push_back(0x02);
    this->LastRecMsg.push_back(0x02);
    this->LastRecMsg.push_back(CRC::crc8(this->LastRecMsg.data(),2));
    return 0;
}

int I2C::SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
{
    unsigned char *temp_buf,*temp_addr,*temp_rec;
    std::vector<unsigned char> package;
    std::vector<unsigned char> crc_tr;
    std::vector<unsigned char> crc_rec;
    temp_buf=buffer.data();
    temp_addr=address.data();
    crc_tr.push_back(temp_addr[0]<<1);
    for(unsigned int i=0;i<buffer.size();i++)
    {
        crc_tr.push_back(temp_buf[i]);
    }
    for(unsigned int i=0;i<buffer.size();i++)
    {
        package.push_back(temp_buf[i]);
    }
    CleanRecMsg();
    if(SendRaw_new(address, package,len))
    {
        return NOK_I2C;
    }
    temp_rec=this->LastRecMsg.data();
    crc_rec.push_back(temp_addr[0]<<1);
    for(unsigned int i=0;i<this->LastRecMsg.size();i++)
    {
        crc_rec.push_back(temp_rec[i]);
        //("%02x\n",temp_rec[i]);
    }
    //("%02x\n",CRC::crc8(crc_rec.data(),crc_rec.size()));
    return OK_I2C;
}

unsigned int I2C::begin(std::string device)
{
    this->Mutex.lock();
    if (this->init==1)
    {
        PrintLog(Info_log,"I2C initialized");
        this->Mutex.unlock();
        return 1;
    }
    CleanRecMsg();
    SetDeviceName(device);
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Initialized");
    this->init=1;
    this->Mutex.unlock();
    return 0;
}
unsigned int I2C::transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
{
    this->Mutex.lock();
    if (SendPacket(address,buffer,len))
    {
        this->Mutex.unlock();
        return NOK_I2C;
    }
    this->Mutex.unlock();
    return OK_I2C;
}


I2C_ELEPS& I2C_ELEPS::getInstance(LogCallback cb) {
   return I2C_ELEPS::getInstance(cb);
}
void I2C_ELEPS::initInstance(LogCallback cb) { I2C::initInstance(cb); }

int I2C_ELEPS::SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len) {
    unsigned char *temp_buf,*temp_addr,*temp_rec;
    std::vector<unsigned char> package;
    std::vector<unsigned char> crc_tr;
    std::vector<unsigned char> crc_rec;
    temp_buf=buffer.data();
    temp_addr=address.data();
    crc_tr.push_back(temp_addr[0]<<1);
    for(unsigned int i=0;i<buffer.size();i++) {
        crc_tr.push_back(temp_buf[i]);
    }
    for(unsigned int i=0;i<buffer.size();i++) {
        package.push_back(temp_buf[i]);
    }
    package.push_back(CRC::crc8(crc_tr.data(),crc_tr.size()));
    CleanRecMsg();
    if(SendRaw_new(address, package,len)) {
        return NOK_I2C;
    }
    temp_rec=this->LastRecMsg.data();
    crc_rec.push_back(temp_addr[0]<<1);
    for(unsigned int i=0;i<this->LastRecMsg.size();i++) {
        crc_rec.push_back(temp_rec[i]);
        //("%02x\n",temp_rec[i]);
    }
    //("%02x\n",CRC::crc8(crc_rec.data(),crc_rec.size()));
    if(CRC::crc8(crc_rec.data(),crc_rec.size())) {
        PrintLog(Warning_log,(std::string) __func__ +(std::string)"CRC error");
        return NOK_I2C;
    }
    this->LastRecMsg.pop_back();
    return OK_I2C;
}
