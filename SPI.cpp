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
#define INVALID_DATA 0x03
#define TR_ERR 0x05
#define ACK 0x06
#define NACK 0x015
#define BOF 0x20
#define MSP 0x21
#define OK 0x00
#define NOK 0x01
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
class SPI
{
    static SPI * theOneTrueInstance;
private:
    std::vector<unsigned char> LastRecMsg;
    int Mutex;
    int NewData;
    int spifd;
    unsigned char mode;
    unsigned char bitsPerWord;
    unsigned int speed;
    std::string DeviceName;
    int init=0;
    /*Unsafe Methods*/
    /*int SendRaw_new(unsigned char *buffer, unsigned int len) //original
    {
        struct spi_ioc_transfer spi[len];
        uint8_t tx[] = {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };
        int retVal = -1;
        unsigned char garbage[len];*/
        /*for (int i = 0 ; i < len ; i++){

            spi[i].tx_buf        = (unsigned char )(tx + i); // transmit from "data"
            spi[i].rx_buf        = (unsigned char )(garbage + i) ; // receive into "data"
            spi[i].len           = sizeof(unsigned char) ;
            spi[i].delay_usecs   = 0 ;
            spi[i].speed_hz      = this->speed ;
            spi[i].bits_per_word = this->bitsPerWord ;
            spi[i].cs_change = 0;
        }*/

        /*struct spi_ioc_transfer send;
        send.tx_buf = (unsigned long)buffer;
        send.rx_buf = (unsigned long)garbage;
        send.len = ARRAY_SIZE(tx);
        send.delay_usecs = 10;
        send.speed_hz = this->speed;
        send.bits_per_word = this->bitsPerWord;
        send.cs_change = 0;
        retVal= ioctl (this->spifd, ARRAY_SIZE(tx), &send);
        //retVal= ioctl (this->spifd, SPI_IOC_MESSAGE(len), &send) ;
        if(retVal < 0)
        {
            cout<<"Problem transmitting spi data..ioctl";
            return NOK;
        }
        return OK;
    }*/
    /*int ReceiveRaw_new(void) //Need to check original
    {
        struct spi_ioc_transfer spi_start[1];
        unsigned char garbage[this->LastRecMsg.size()];
        unsigned char * buffer;
        int i=0;
        buffer=this->LastRecMsg.data();
        unsigned char TmpLen=this->LastRecMsg.size();
        int retVal = -1;
        spi_start[0].tx_buf        = (unsigned char)(garbage); // transmit from "data"
        spi_start[0].rx_buf        = (unsigned char)(buffer) ; // receive into "data"
        spi_start[0].len           = sizeof(unsigned char) ;
        spi_start[0].delay_usecs   = 0 ;
        spi_start[0].speed_hz      = this->speed ;
        spi_start[0].bits_per_word = this->bitsPerWord ;
        spi_start[0].cs_change = 0;
        retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi_start) ;
        TmpLen=this->LastRecMsg.front();
        if (TmpLen==0)
        {
            cout<<"Problem receiving spi data..ioctl";
            return NOK;
        }
        TmpLen--;
        struct spi_ioc_transfer spi_all[TmpLen];
        for(i=0;i<TmpLen;i++)
        {
            spi_all[i].tx_buf        = (unsigned char)(garbage+i); // transmit from "data"
            spi_all[i].rx_buf        = (unsigned char)(buffer+i+1) ; // receive into "data"
            spi_all[i].len           = sizeof(unsigned char) ;
            spi_all[i].delay_usecs   = 0 ;
            spi_all[i].speed_hz      = this->speed ;
            spi_all[i].bits_per_word = this->bitsPerWord ;
            spi_all[i].cs_change = 0;
            retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(TmpLen), &spi_all) ;
        }
        if(retVal < 0){
            cout<<"Problem receiving spi data..ioctl";
            return NOK;
        }
        return OK;
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
public:
    static SPI & getInstance() {
        if (!theOneTrueInstance) initInstance();
        return *theOneTrueInstance;
    }
    static void initInstance() { new SPI; }
    int begin(std::string device)/*Need to check*/
    {
        this->Mutex=1;
        if (this->init==1)
        {
            cout<<"everything initialized";
            this->Mutex=0;
            return 1;
        }
        this->init=1;
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
    SPI() {
        if (theOneTrueInstance) throw std::logic_error("Instance already exists");
        theOneTrueInstance = this;
    }

    virtual ~SPI() { }
};

SPI* SPI::theOneTrueInstance = 0;


class MCU
{

private:
    uint16_t WrongTransactions=3;
public:
    MCU(std::string filename)
    {
        SPI & ptrSPI=SPI::getInstance();
        ptrSPI.begin(filename);
    }
    ~MCU(){}

    uint16_t SetStanby(uint16_t Status)
    {
        return SendBool(0x00, Status);
    }
    std::vector<unsigned char> CheckStatus(void)
    {
        SPI & ptrSPI=SPI::getInstance();
        uint16_t cnt=this->WrongTransactions;
        uint16_t error;
        std::vector<unsigned char> msg, answer,null;
        msg.push_back(0x01);
        while(cnt--) {
            cout<<"prepared status "<<cnt<<"\n";
            error=ptrSPI.transaction(msg);
            if (error==0) {
                answer = ptrSPI.recData();
                if (answer.size()==2) {
                    return answer;
                }
                else if (cnt==0) {
                    return null;
                }
            }
            else if (cnt==0) {
                return null;
            }
        }
    }
    void RenewAll(void );
    uint16_t SetSubroutine(unsigned char Routine)
    {
        return SendInt(0x00, Routine);
    }
    uint16_t SetConnector(uint16_t  Connector)
    {
        return SendInt(0x00, Connector);
    }
    uint16_t SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton)
    {
        return SendDoubleInt(0x00, BlueButton,YellowButton);
    }
    uint16_t SetPower(uint16_t BlueButton, uint16_t YellowButton)
    {
        return SendDoubleInt(0x00, BlueButton,YellowButton);
    }
    uint16_t SetMaxTime(uint16_t MaxTime )
    {
        return SendBool(0x00, MaxTime);
    }
    uint16_t SetAutoStart(uint8_t Enabled )
    {
        return SendBool(0x00, Enabled);
    }
    uint16_t SetAutoStartDelay(uint16_t DelayTime )
    {
        return SendInt(0x00, DelayTime);
    }
    uint16_t SetAutoStop(uint8_t Enabled )
    {
        return SendInt(0x00, Enabled);
    }
    uint16_t SetAutoStopResistance(uint16_t Resistance )
    {
        return SendInt(0x00, Resistance);
    }
    uint16_t SetIrrigation(uint8_t Enabled )
    {
        return SendBool(0x00, Enabled);
    }
    uint16_t SetModulation(uint16_t Frequency )
    {
        return SendInt(0x00, Frequency);
    }
    uint16_t SetDutyRate(uint16_t CrestFactor )
    {
        return SendInt(0x00, CrestFactor);
    }
protected:
    uint16_t SendBool(uint8_t command,uint16_t value)
    {
        SPI & ptrSPI=SPI::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(command);
        msg.push_back(value ? 0x01 : 0x00);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrSPI.transaction(msg);
            if (error==0) {
                answer=ptrSPI.recData();
                if (answer.size()==1) {
                    return answer.front()==ACK? ACK:NACK;
                }
                else if (cnt==0) {
                    return TR_ERR;
                }
            }
            else if (cnt==0) {
                return TR_ERR;
            }
        }
    }
    uint16_t SendInt(uint8_t command,uint16_t value)
    {
        SPI & ptrSPI=SPI::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(command);
        msg.push_back(uint8_t(value&0xff));
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrSPI.transaction(msg);
            if (error==0) {
                answer=ptrSPI.recData();
                if (answer.size()==1) {
                    return answer.front()==ACK? ACK:NACK;
                }
                else if (cnt==0) {
                    return TR_ERR;
                }
            }
            else if (cnt==0) {
                return TR_ERR;
            }
        }
    }
    uint16_t SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2)
    {
        SPI & ptrSPI=SPI::getInstance();
        uint16_t cnt=this->WrongTransactions;
        uint16_t error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(command);
        msg.push_back((value1&0xff));
        msg.push_back((value1>>8));
        msg.push_back(value2&0xff);
        msg.push_back((value2>>8));
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrSPI.transaction(msg);
            if (error==0) {
                answer=ptrSPI.recData();
                if (answer.size()==1) {
                    return answer.front()==ACK? ACK:NACK;
                }
                else if (cnt==0) {
                    return TR_ERR;
                }
            }
            else if (cnt==0) {
                return TR_ERR;
            }
        }
    }
};
int main(void)
{
    std::string filename="/dev/ttyUSB0";
    //SPI & test =SPI::getInstance();
    MCU mcu(filename);
    MCU mcu2(filename);
    //test.begin(filename);
    std::vector<unsigned char> data;
    std::vector<unsigned char> received;
    unsigned char value= 1;
    data.push_back( value);
    cout<<"Here works0";
    //test.transaction(data);
    //received=test.recData();
    //cout<<received.size()<<"\n";
    mcu.SetStanby(1);
    mcu.CheckStatus();
    mcu2.SetStanby(1);
    mcu2.CheckStatus();
    return 1;
}