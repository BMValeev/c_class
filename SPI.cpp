/*
 *
 *
 *
 *
 *
 *Created by eleps on 27.04.18.
 */
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
#include <vector>
#include <algorithm>
using namespace std;
#define MUTEX_BLOCKED 127
/*#define
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
*/
class SPI
{
private:
    const unsigned int MaxLen=40;
    unsigned int MsgLen=40;
    std::vector<unsigned char> LastRecMsg;
    int Mutex;
    int NewData;
    int spifd;
    unsigned char mode;
    unsigned char bitsPerWord;
    unsigned int speed;
    std::string DeviceName;
    /*Unsafe Methods*/
    /*int SendRaw(unsigned char *buffer, unsigned int len) //original
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
    int ReceiveRaw(void) //Need to check original
    {
        struct spi_ioc_transfer spi_start[1];
        unsigned char garbage[this->LastRecMsg.size()];
        unsigned char * buffer;
        buffer=this->LastRecMsg.data();
        unsigned char TmpLen=this->LastRecMsg.size();
        int i = 0;
        int retVal = -1;
        for(i=0;i<TmpLen;i++)
        {
            garbage[i]='\0';
        }
        spi_start[0].tx_buf        = (unsigned long)(garbage); // transmit from "data"
        spi_start[0].rx_buf        = (unsigned long)(buffer) ; // receive into "data"
        spi_start[0].len           = sizeof(unsigned char) ;
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
            spi_all[i].rx_buf        = (unsigned long)(buffer+i+1) ; // receive into "data"
            spi_all[i].len           = sizeof(unsigned char) ;
            spi_all[i].delay_usecs   = 0 ;
            spi_all[i].speed_hz      = this->speed ;
            spi_all[i].bits_per_word = this->bitsPerWord ;
            spi_all[i].cs_change = 0;
            retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(TmpLen), &spi_all) ;
        }
//        this->MsgLen=TmpLen;
        if(retVal < 0){
            perror("Problem receiving spi data..ioctl");
            return 1;
        }

        return retVal;
    }*/
    int SendRaw(unsigned char *buffer, unsigned int len)
    {
        return 0;
    }
    int ReceiveRaw(void)
    {
        this->LastRecMsg.clear();
        this->LastRecMsg.push_back(0x02);
        this->LastRecMsg.push_back(0x02);
        this->LastRecMsg.push_back(CRC8(this->LastRecMsg.data(),2));
        return 0;
    }
    void SetDeviceName(std::string Name)
    {
        /*
         * Used for configuration of the device in construction
         */
        this->DeviceName=Name;
        cout<<this->DeviceName;
    }
    void CleanRecMsg(void)
    {
        /*
         * Explicitly cleans last read buffer
         */
        this->LastRecMsg.clear();
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

    int SendPacket(std::vector<unsigned char> Buffer)
    {
        unsigned int FullLen=Buffer.size()+2;
        unsigned char *temp;
        temp=Buffer.data();
        unsigned char Result[FullLen];
        Result[0]=FullLen;
        for(int i=0;i<Buffer.size();i++)
        {
            Result[i+1]=temp[i];
        }
        Result[FullLen-1]=CRC8(Result,FullLen-1);
        if(SendRaw(Result, FullLen))
        {
            return 1;
        }
        //cout<<Result;
        cout<<"Here works1\n";
        return 0;
    }
    int ReceivePacket(void)
    {
        CleanRecMsg();
        this->MsgLen=0;
        if(ReceiveRaw())
        {
            return 1;
        }

        if(CRC8(this->LastRecMsg.data(),this->LastRecMsg.size()))
        {
            return 1;
        }
        this->LastRecMsg.pop_back();
        cout<<"Here works3\n";
        return 0;
    }
    SPI(){};
    ~SPI() {};
    SPI(const SPI& root) = delete;
    SPI& operator=(const SPI&) = delete;
public:
    static SPI& instance(void)/*Need to check*/
    {
        static SPI theSingleInstance;
        return theSingleInstance;
    }
    int begin(std::string device)/*Need to check*/
    {
        this->Mutex=1;
        CleanRecMsg();
        SetDeviceName(device);
        int statusVal = -1;
        /*this->mode = SPI_MODE_0 ;
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
        }*/
        this->Mutex=0;
    }
    unsigned int transaction(std::vector<unsigned char> buffer) /*Need to check*/
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
        if (SendPacket(buffer))
        {
            this->Mutex=0;
            return 1;
        }
        usleep(100000);
        cout<<"Here works2\n";
        if (ReceivePacket())
        {
            this->Mutex=0;
            cout<<"Here not works3\n";
            return 1;
        }
        this->Mutex=0;
        return 0;
    }
    std::vector<unsigned char> recData(void) /*Need to check*/
    {
        this->NewData=0;
        return LastRecMsg;
    }
protected:

};


class MCU : public SPI
{
private:
    SPI & ptrSPI;

public:
    MCU(){}
    ~MCU(){}
    static void initInstance()
    {
        new MCU;
    }
    /*unsigned int SetStanby(unsigned int Status)
    {
        unsigned int cnt=3;
        unsigned char msg[4],*answer;
        unsigned int error;
        msg[0]=0x00;
        if (Status)
        {
            msg[1]=0x01;
        }
        else
        {
            msg[1]=0x00;
        }
        while(cnt--)
        {
            error=ptrSPI->Transaction(msg,2);
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

    }*/

    /*unsigned int CheckStatus()
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
    }*/
    void RenewAll(void );
    void SetSubroutine(unsigned char Routine);
    void SetConnector(void );
    void SetMaxVoltage(void );
    void SetPower(void );
    void SetMaxTime(void );
    void SetAutoStart(void );
    void SetAutoStartDelay(void );
    void SetAutoStop(void );
    void SetAutoStopResistance(void );
    void SetIrrigation(void );
    void SetModulation(void );
    void SetDutyRate(void );
protected:
};
int main(void)
{
    std::string filename="/dev/ttyUSB0";
    SPI & test = SPI::instance();
    MCU & mcu = MCU::initInstance();
    test.begin(filename);
    //std::string value= "12354";
    std::vector<unsigned char> data;
    std::vector<unsigned char> received;
    unsigned char value= 1;
    data.push_back( value);
    /*std::transform(value.begin(), value.end(), data.begin(),
                   [](char c)
                   {
                       return static_cast<unsigned char>(c);
                   });*/
    /*cout<<"Here works0";
    test.transaction(data);
    received=test.recData();
    cout<<received.size();*/
    return 1;
}