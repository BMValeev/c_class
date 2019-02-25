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
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Device name set to :" +(std::string)Name);
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
    int errnum;
    unsigned int cnt, cnt_all;
    unsigned char buf_rec[40]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
            ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    //unsigned char buf_rec[rlen];
    //memset(&buf_rec, 0, sizeof(unsigned char)*rlen);
    unsigned int flag=0;
    std::string data_send,data_rec;
    i2c_rdwr_ioctl_data message;
    memset(&message, 0, sizeof(message));
    i2c_msg message_packet[2];
    PrintLog(Debug_log,(std::string) __func__+  (std::string)" Send message");
    memset(&message_packet, 0, sizeof(i2c_msg)*2);
    message_packet[0].addr=address.front();
    message_packet[0].flags=0;
    message_packet[0].len=buffer.size();
    message_packet[0].buf=buffer.data();
    message_packet[1].addr=address.front();
    message_packet[1].flags=I2C_M_RD;//I2C_M_RD|I2C_M_RECV_LEN;I2C_M_NOSTART
    message_packet[1].buf=buf_rec;
    message_packet[1].len=10;
    message_packet[1].len= (rlen==0)?10:rlen;
    cnt_all= (rlen==0)?10:rlen;
    message.msgs=message_packet;
    message.nmsgs =(rlen!=0) ?  2:1;
    int file = open(this->DeviceName.c_str(), O_RDWR);
    if (file == -1) {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)"Device open error");
        close(file);
        return NOK_I2C;
    }
    if (ioctl(file, I2C_SLAVE, address.front()) < 0) {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)"Failed to acquire bus access and/or talk to slave");
        close(file);
        return NOK_I2C;
    }
    ret=ioctl(file, I2C_RDWR, &message);
    if (ret<0) {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)strerror(-ret)
                             +(std::string)"Unable to send message");
        fprintf (stderr, "%s.\n", strerror(-ret));
        close(file);
        return NOK_I2C;
    }
    close(file);
    CleanRecMsg();
    for (cnt = 0; cnt < (cnt_all); cnt++) {
        this->LastRecMsg.push_back(buf_rec[cnt]);
    }
    PrintLog(Debug_log,(std::string) __func__+  (std::string)" Send message");
    return OK_I2C;
}
/*
 *     if (flag){
        cnt_all=cnt_all-this->LastRecMsg.front();
        while (--cnt_all) {
            this->LastRecMsg.pop_back();
        }
    }
 */

int I2C::SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
{
    std::vector<unsigned char> package;
    for(unsigned int i=0;i<buffer.size();i++) {
        package.push_back(buffer[i]);
    }
    CleanRecMsg();
    return (SendRaw_new(address, package,len)) ?NOK_I2C:OK_I2C;
}

unsigned int I2C::begin(std::string device)
{
    this->Mutex.lock();
    if (this->init==1) {
        PrintLog(Info_log,"I2C initialized");
        this->Mutex.unlock();
        return 1;
    }
    CleanRecMsg();
    std::cout << device;
    SetDeviceName(device);
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Initialized");
    this->init=1;
    this->Mutex.unlock();
    return 0;
}
unsigned int I2C::transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
{
    this->Mutex.lock();
    if (SendPacket(address,buffer,len)) {
        this->Mutex.unlock();
        return NOK_I2C;
    }
    this->Mutex.unlock();
    return OK_I2C;
}
