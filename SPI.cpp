/*
 *Created by eleps on 27.04.18.
*/
using namespace std;
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "crc.h"
#include "SPI.h"



// SPI class
SPI* SPI::theOneTrueInstance;

// Construction and destruction
SPI & SPI::getInstance() {
    if (!theOneTrueInstance) initInstance();
    return *theOneTrueInstance;
}
void SPI::initInstance() {
    new SPI;
}
SPI::SPI() {
    if (theOneTrueInstance) throw std::logic_error("Instance already exists");
    theOneTrueInstance = this;
}
SPI::~SPI() {

}

uint8_t SPI::begin(std::string device,LogCallback cb)/*Need to check*/
{
    this->m_cb=cb;
    PrintLog(Debug_log,(std::string) __func__+  (std::string)" Function started");
    this->Mutex.lock();
    CleanRecMsg();
    SetDeviceName(device);
    int statusVal = -1;
    this->mode = SPI_MODE_0 ;
    this->bitsPerWord = 8;
    this->speed = 1000000;
    this->spifd = -1;
    this->spifd = open(this->DeviceName.c_str(), O_RDWR);
    if(this->spifd < 0){
        PrintLog(Critical_log, "could not open SPI device");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    if(statusVal < 0){
        PrintLog(Critical_log,(std::string) __func__+  (std::string)" Could not set SPIMode (WR)...ioctl fail");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
    if(statusVal < 0) {
        PrintLog(Critical_log, (std::string) __func__+  (std::string)"Could not set SPIMode (RD)...ioctl fail");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPI bitsPerWord (WR)...ioctl fail");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPI bitsPerWord(RD)...ioctl fail");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPI speed (WR)...ioctl fail");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
        PrintLog(Critical_log, (std::string) __func__+  (std::string)"Could not set SPI speed (RD)...ioctl fail ");
        this->status=0;
        this->Mutex.unlock();
        return NOK_SPI;
    }
    this->status=1;
    this->init=1;
    this->Mutex.unlock();
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
    return 0;
}
uint8_t SPI::transaction(std::vector<unsigned char> buffer, uint8_t ans_len) /*Need to check*/
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    //unsigned int status=0;
    this->Mutex.lock();
    if (SendPacket(buffer,ans_len))
    {
        this->Mutex.unlock();
        PrintLog(Warning_log,(std::string) __func__+  (std::string)": Packet not send");
        return NOK_SPI;
    }
    // Compute CRC over all message including CRC itself - should equal 0
    if(CRC::crc8(this->LastRecMsg.data(),this->LastRecMsg.size()))
    {
        this->Mutex.unlock();
        PrintLog(Warning_log,(std::string) __func__+  (std::string)": CRC error");
        return NOK_SPI;
    }
    this->LastRecMsg.pop_back();
    PrintLog(Debug_log,(std::string) __func__+  (std::string)": Function ended succesfully");
    this->Mutex.unlock();
    return OK_SPI;
}
std::vector<unsigned char> SPI::recData(void) /*Need to check*/
{
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)": Function started");
    this->NewData=0;
    //PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
    return LastRecMsg;

}
/*Unsafe methods*/
int SPI::SendRaw_new(unsigned char *buffer, unsigned int len, uint8_t ans_len) //original
{
    int retVal = 0;
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    unsigned char receive[len];
    unsigned char test[4]={0x05,0x33,0x00,0x00};
    unsigned char test1[2]={0x03,0x33};
    if (this->status==0)
    {
        if (ans_len==3)
        {
            PrintLog(Debug_log,(std::string) __func__+  (std::string)"Test branch");
            CleanRecMsg();
            this->LastRecMsg.push_back(0x03);
            this->LastRecMsg.push_back(0x33);
            this->LastRecMsg.push_back(CRC::crc8(test1,2));
            return OK_SPI;
        } else
        {
            PrintLog(Debug_log,(std::string) __func__+  (std::string)"Test branch");
            CleanRecMsg();
            this->LastRecMsg.push_back(0x05);
            this->LastRecMsg.push_back(0x33);
            this->LastRecMsg.push_back(0x00);
            this->LastRecMsg.push_back(0x00);
            this->LastRecMsg.push_back(CRC::crc8(test,4));
            return OK_SPI;
        }
    }
    PrintLog(Debug_log,(std::string) __func__+  std::to_string(len));
    spi_ioc_transfer send[2];
    send[0].tx_buf = (unsigned long)buffer;
    send[0].rx_buf = (unsigned long)NULL;
    send[0].len = len;
    send[0].delay_usecs = 3000;
    send[0].speed_hz = this->speed;
    send[0].bits_per_word = this->bitsPerWord;
    send[0].tx_nbits=0;
    send[0].rx_nbits=0;
    send[0].pad=0;
    send[0].cs_change = 0;
    send[1].tx_buf = (unsigned long)NULL;
    send[1].rx_buf = (unsigned long)receive;
    //send[1].len = ans_len;
    send[1].len = len;
    send[1].delay_usecs = 0;
    send[1].speed_hz = this->speed;
    send[1].bits_per_word = this->bitsPerWord;
    send[1].tx_nbits=0;
    send[1].rx_nbits=0;
    send[1].pad=0;
    send[1].cs_change = 0;
    retVal= ioctl (spifd, SPI_IOC_MESSAGE(2), &send);
    if(retVal < 0)
    {
        PrintLog(Warning_log,(std::string) __func__+ (std::string)strerror(errno)  +(std::string)"Error during transmission" );
        return NOK_SPI;
    }
    CleanRecMsg();
    for (retVal = 0; retVal < (ans_len); retVal++)
    {
        this->LastRecMsg.push_back(receive[retVal]);
    }
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
    return OK_SPI;
}
void SPI::SetDeviceName(std::string Name)
{
    /*
             * Used for configuration of the device in construction
             */
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    this->DeviceName=Name;    
    PrintLog(Info_log, "Function name - " + DeviceName + "");
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
}
void SPI::CleanRecMsg(void)
{
    /*
             * Explicitly cleans last read buffer
             */
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    this->LastRecMsg.clear();
    //PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
}
/*Safe methods*/
void SPI::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=0)
    {
        m_cb(status,text);
    }
}

void SPI::PrintToCout(uint8_t status, string msg)
{
        cout<<status<<msg<<endl;
}

int SPI::SendPacket(std::vector<unsigned char> Buffer, uint8_t ans_len)
{
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    unsigned int FullLen;
    unsigned int i=0;
    unsigned char *temp;
    //unsigned char Result[Buffer.size()+2];
    unsigned char Result[PACKED_LENGTH_SPI];
    FullLen=Buffer.size()+2;
    temp=Buffer.data();
    Result[0]=FullLen;
    for(i=1;i<Buffer.size()+1;i++)
    {
        Result[i]=temp[i-1];
    }
    Result[i++]=CRC::crc8(Result,FullLen-1);
    // Fill rest of the buffer with zeros
    for(; i<PACKED_LENGTH_SPI; i++) {
        Result[i] = 0;
    }
    if(SendRaw_new(Result, PACKED_LENGTH_SPI, ans_len))
    {
        PrintLog(Info_log,(std::string) __func__+  (std::string)"Transmisison error");
        return NOK_SPI;
    }
    //PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
    return OK_SPI;
}

