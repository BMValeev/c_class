/*
 *
 *
 *
 *
 *
 *
 */
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
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <vector>
using namespace std;
#define MUTEX_BLOCKED 127


class I2C
{
    static I2C * theOneTrueInstance;
private:
    const unsigned int MaxLen=40;
    unsigned int MsgLen;
    std::vector<unsigned char>  LastRecMsg;
    int Mutex;
    int NewData;
    int i2cfd;
    int init=0;
    std::string DeviceName;
    struct i2c_client *i2c_data;
    /*Unsafe Methods*/
    /*int SendRaw(struct i2c_client *client, unsigned char *buffer, unsigned int len)
    {
        struct i2c_msg msgs[2];
        unsigned char txbuf[len];
        int ret;
        for (int i=0;i<len;i++)
        {
            txbuf[i]=buffer[i];
        }
        msgs[0].addr = client->addr;
        msgs[0].flags = 0;
        msgs[0].len = len;
        msgs[0].buf = txbuf;

        msgs[1].addr = client->addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 3;
        msgs[1].buf = this->LastRecMsg;

        ret = i2c_transfer(client->adapter, msgs, 2);
        if (ret < 0)
        {
            return ret;
        }
        return 0;
    }*/
    int SendRaw(unsigned char *buffer, unsigned int len)
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
    int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer)
    {
        unsigned char FullLen=buffer.size()+3;
        unsigned char *temp;
        temp=buffer.data();
        unsigned char Result[FullLen];
        Result[0]=FullLen;
        cout<<"Here works1";
        Result[0]=address.front();
        Result[1]=FullLen;
        for(int i=0;i<buffer.size();i++)
        {
            Result[i+2]= temp[i];
        }
        Result[FullLen-1]=CRC8(Result+sizeof(unsigned char),FullLen-2);
        CleanRecMsg();
        if(SendRaw(Result, FullLen))
        {
            return 1;
        }
        cout<<"Here works2";
        if(CRC8(this->LastRecMsg.data(),this->LastRecMsg.size()))
        {
            return 1;
        }
        return 0;
    }
public:
    static I2C & getInstance() {
        if (!theOneTrueInstance) initInstance();
        return *theOneTrueInstance;
    }
    static void initInstance() { new I2C; }
    unsigned int begin(std::string device)
    {
        this->Mutex=1;
        if (this->init==1)
        {
            cout<<"everything initialized";
            this->Mutex=0;
            return 1;
        }
        CleanRecMsg();
        SetDeviceName(device);
        /*int statusVal = -1;
        this->i2cfd = -1;
        SetDeviceName(device);
        char *filename = "/dev/i2c-2";
        if ((this->i2cfd = open(this->DeviceName, O_RDWR)) < 0)
        {
            perror("Failed to open the i2c bus");
            this->Mutex=0;
        }*/
        this->Mutex=0;
    }
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer)
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
        if (SendPacket(address,buffer))
        {
            this->Mutex=0;
            return 1;
        }
        cout<<"Here works3";
        this->Mutex=0;
        return 0;
    }
    std::vector<unsigned char> recData(void)
    {
        this->NewData=0;
        return LastRecMsg;
    }
protected:
    I2C() {
        if (theOneTrueInstance) throw std::logic_error("Instance already exists");
        theOneTrueInstance = this;
    }

    virtual ~I2C() { }

};
I2C* I2C::theOneTrueInstance = 0;



class ConnModule
{
private:
    uint16_t WrongTransactions=3;
    std::vector<unsigned char> addr;

public:
    std::vector<unsigned char>getAddress(void);
    std::vector<unsigned char> setAddress(std::vector<unsigned char>addr)
    {
        this->addr=addr;
    }
    ConnModule(std::string filename) {
        I2C &ptrSPI = I2C::getInstance();
        ptrSPI.begin(filename);
        std::vector<unsigned char> address;
        address.push_back(0x01);
        this->addr=address;
    }
    ~ConnModule()
    {

    }
    unsigned int SetUUID(std::vector<unsigned char> uuid)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x00);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==1) {
                    //if answer
                    return 0;
                }
                else if (cnt==0) {
                    return 1;
                }
            }
            else if (cnt==0) {
                return 1;
            }
        }
    }
    unsigned int SetName(std::vector<unsigned char> data);
    unsigned int StartInit(void);
    unsigned int WriteString(std::vector<unsigned char> data);
    unsigned int EndInit(void);
    unsigned int WriteValue(std::vector<unsigned char> data);
    unsigned int ReadValue(std::vector<unsigned char> data);
    unsigned int ReadLastChangedValue(void);
    unsigned int StartBonding(std::vector<unsigned char> data);
    unsigned int CheckBonding(std::vector<unsigned char> data);

protected:
};
/*
class BoardModule
{
private:
    I2C *ptrSPI;
public:
    BoardModule(I2C *ptrClass);
    ~BoardModule();
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

protected:
};*/
int main(void)
{
    std::string filename="/dev/ttyUSB0";
    I2C & test =I2C::getInstance();
    test.begin(filename);
    std::vector<unsigned char> data;
    std::vector<unsigned char> address;
    address.push_back(0x01);
    std::vector<unsigned char> received;
    ConnModule module(filename);
    unsigned char value= 1;
    data.push_back( value);
    address.push_back( 0x01);
    cout<<"Here works0";
    test.transaction(address,data);
    module.SetUUID(data);
    return 1;
}