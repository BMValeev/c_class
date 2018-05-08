/*
 *
 *
 *
 *
 *
 *Created by eleps on 27.04.18.
 */
#include <SPI.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;
#define MUTEX_BLOCKED 127
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define

class SPI
{
private:
    const unsigned int MaxLen=40;
    unsigned int MsgLen=40;
    unsigned char LastRecMsg[MaxLen];
    int Mutex;
    int NewData;
    int spifd;
    unsigned char mode;
    unsigned char bitsPerWord;
    unsigned int speed;
    std::string DeviceName;
    /*Unsafe Methods*/
    int SendRaw(unsigned char *buffer, unsigned int len)
    {
        struct spi_ioc_transfer spi[len];
        int i = 0;
        int retVal = -1;
        unsigned char garbage[len];
        for (i = 0 ; i < len ; i++){

            spi[i].tx_buf        = (unsigned long)(buffer + i); // transmit from "data"
            spi[i].rx_buf        = (unsigned long)(garbage + i) ; // receive into "data"
            spi[i].len           = sizeof(*(buffer + i)) ;
            spi[i].delay_usecs   = 0 ;
            spi[i].speed_hz      = this->speed ;
            spi[i].bits_per_word = this->bitsPerWord ;
            spi[i].cs_change = 0;
        }
        retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(len), &spi) ;
        if(retVal < 0)
        {

            perror("Problem transmitting spi data..ioctl");
            return 1;
        }

        return retVal;
    }
    int ReceiveRaw(void)
    {
        struct spi_ioc_transfer spi_start[1];

        unsigned char garbage[this->MaxLen];
        unsigned char TmpLen=MaxLen;
        int i = 0;
        int retVal = -1;
        for(i=0;i<this->MaxLen;i++)
        {
            garbage[i]='\0';
        }
        spi_start[0].tx_buf        = (unsigned long)(garbage); // transmit from "data"
        spi_start[0].rx_buf        = (unsigned long)(LastRecMsg) ; // receive into "data"
        spi_start[0].len           = sizeof(*(LastRecMsg)) ;
        spi_start[0].delay_usecs   = 0 ;
        spi_start[0].speed_hz      = this->speed ;
        spi_start[0].bits_per_word = this->bitsPerWord ;
        spi_start[0].cs_change = 0;
        retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi_start) ;
        TmpLen=LastRecMsg[0];
        if (TmpLen==0)
        {
            return 1;
        }
        TmpLen--;
        struct spi_ioc_transfer spi_all[TmpLen];
        for(i=0;i<TmpLen;i++)
        {
            spi_all[i].tx_buf        = (unsigned long)(garbage+i); // transmit from "data"
            spi_all[i].rx_buf        = (unsigned long)(LastRecMsg+i+1) ; // receive into "data"
            spi_all[i].len           = sizeof(*(LastRecMsg)) ;
            spi_all[i].delay_usecs   = 0 ;
            spi_all[i].speed_hz      = this->speed ;
            spi_all[i].bits_per_word = this->bitsPerWord ;
            spi_all[i].cs_change = 0;
            retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(TmpLen), &spi_all) ;
        }
        this->MsgLen=TmpLen;
        if(retVal < 0){
            perror("Problem receiving spi data..ioctl");
            return 1;
        }

        return retVal;
    }
    void SetDeviceName(std::string Name)
    {
        /*
         * Used for configuration of the device in construction
         */
        this->DeviceName=Name;
    }
    void CleanRecMsg(void)
    {
        /*
         * Explicitly cleans last read buffer
         */
        for(unsigned int i=0;i<this->MaxLen;i++)
        {
            this->LastRecMsg[i]='\0';
        }
    }
    void SetRecMsg(unsigned char *data,unsigned int len)
    {
        /*
         * Explicitly write message to storage
         */
        unsigned int cnt;
        CleanRecMsg();
        cnt=(len>(this->MaxLen))?MaxLen:len;
        for(unsigned int i=0;i<cnt;i++)
        {
            this->LastRecMsg[i]=data[i];
        }
    }

    /*Safe functions*/
    unsigned char CRC8(unsigned char *buffer, unsigned int len)
    {
        unsigned char crc = 0x82;
        unsigned int i;

        while (len--)
        {
            crc ^= *buffer++;; crc = (crc & 1)? (crc >> 1) ^ 0x8c : crc >> 1;
        }
        return crc;
    }

    int SendPacket(unsigned char *Buffer, unsigned int Len)
    {
        unsigned int FullLen=Len+2;
        unsigned char *Result[FullLen];
        Result[0]=FullLen;
        for(int i=0;i<Len-2;i++)
        {
            Result[i+1]=Buffer[i];
        }
        Result[Len-1]=CRC8(Result,Len-1);
        if(SendRaw(Result[], FullLen))
        {
            return 1;
        }
        return 0;
    }
    int ReceivePacket(void)
    {
        CleanRecMsg();
        this->MsgLen=0;
        if(ReceiveRaw(void))
        {
            return 1;
        }
        if(CRC8(this->LastRecMsg,this->MsgLen))
        {
            return 1;
        }
        this->LastRecMsg[this->MsgLen]='\0';
        this->MsgLen--;
        return 0;
    }
    SPI(){};
    SPI(const SPI& root) = delete;
    SPI& operator=(const SPI&) = delete;
public:
    static SPI& Instance(void)
    {
        static SPI theSingleInstance;
        return theSingleInstance;
    }
    int Init(std::string device)
    {
        this->Mutex=1;
        this->MsgLen=0;
        CleanRecMsg(void);
        SetDeviceName(device);
        int statusVal = -1;
        this->mode = SPI_MODE_0 ;
        this->bitsPerWord = 8;
        this->speed = 1000000;
        this->spifd = -1;
        this->spifd = open(DeviceName.c_str(), O_RDWR);
        if(this->spifd < 0){
            perror("could not open SPI device");
        }
        statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
        if(statusVal < 0){
            perror("Could not set SPIMode (WR)...ioctl fail");;
        }
        statusVal = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
        if(statusVal < 0) {
            perror("Could not set SPIMode (RD)...ioctl fail");
        }
        statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
        if(statusVal < 0) {
            perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
        }
        statusVal = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
        if(statusVal < 0) {
            perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
        }
        statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
        if(statusVal < 0) {
            perror("Could not set SPI speed (WR)...ioctl fail");
        }
        statusVal = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
        if(statusVal < 0) {
            perror("Could not set SPI speed (RD)...ioctl fail");
        }
        this->Mutex=0;
    }
    unsigned int Transaction(unsigned char *buffer, unsigned int len)
    {
        unsigned int status=0;
        if(this->Mutex)
        {
            return 1;
        }
        if(this->NewData)
        {
            return 1;
        }
        this->Mutex=1;
        if (SendPacket(buffer, len))
        {
            this->Mutex=0;
            return 1;
        }
        sleep(100);
        if (ReceivePacket())
        {
            this->Mutex=0;
            return 1;
        }
        this->Mutex=0;
        return 0;
    }
    unsigned char* RecData(void)
    {
        if(this->NewData)
        {
            return 1;
        }
        this->NewData=0;
        return *LastRecMsg[];
    }
protected:

};
class MCU
{
private:
    SPI *ptrSPI;
public:
    MCU(std::string device)
    {

    }
    ~MCU();
    unsigned int SetStanby(unsigned int Status)
    {
        unsigned int cnt=3;
        unsigned char msg[4],*answer;
        unsigned int error;
        msg[0]=0x03;
        msg[1]=0x00;
        if (Status)
        {
            msg[2]=0x01;
        }
        else
        {
            msg[2]=0x00;
        }
        msg[3]=ptrSPI->CRC8(msg,3);
        while(cnt--)
        {
            error=ptrSPI->Transaction(msg,4);
            if (error==0)
            {
                answer=ptrSPI->RecData();
                if (answer==0)
                {
                    return 0;
                }
                if (answer!=0&&cnt==0)
                {
                    return 1;
                }
            }
            if (error!=0&&cnt==0)
            {
                return 1;
            }
        }

    }

    CheckStatus()
    {
        unsigned int cnt=3;
        unsigned char msg[4],*answer;
        unsigned int error;
        msg[0]=0x03;
        msg[1]=0x00;
        msg[2]=0x01;
        while(cnt--)
        {
            error=this->ptrSPI->Transaction(msg,3);
            if (error==0)
            {
                answer = this->ptrSPI->RecData();
                if (answer==0)
                {
                    return 0;
                }
                if (answer!=0&&cnt==0)
                {
                    return 1;
                }
            }
            if (error!=0&&cnt==0)
            {
                return 1;
            }
        }
    }
    RenewAll();
    SetSubroutine(unsigned char Routine);
    SetConnector();
    SetMaxVoltage();
    SetPower();
    SetMaxTime();
    SetAutoStart();
    SetAutoStartDelay();
    SetAutoStop();
    SetAutoStopResistance();
    SetIrrigation();
    SetModulation();
    SetDutyRate();
protected:
};