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
#include <I2C.h>
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

class I2C
{
private:
    const unsigned int MaxLen=40;
    unsigned int MsgLen;
    unsigned char LastRecMsg[MaxLen];
    int Mutex;
    int NewData;
    int i2cfd;
    std::string DeviceName;
    struct i2c_client *i2c_data;
    /*Unsafe Methods*/
    int SendRaw(struct i2c_client *client, unsigned char *buffer, unsigned int len)
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
    int SendPacket(unsigned char Address,unsigned char *Buffer, unsigned int Len)
    {
        unsigned int FullLen=Len+2;
        unsigned char *Result[FullLen];
        Result[0]=(unsigned char *) Address;
        Result[1]=(unsigned char *)FullLen;
        for(int i=0;i<Len-2;i++)
        {
            Result[i+1]=(unsigned char *) Buffer[i];
        }
        Result[Len-1]=(unsigned char *) CRC8(Result[1],Len-1);
        CleanRecMsg();
        if(SendRaw(this->i2c_data,Result[0], FullLen))
        {
            return 1;
        }
        this->MsgLen=this->LastRecMsg[0];
        if(CRC8(this->LastRecMsg,this->MsgLen))
        {
            return 1;
        }
        this->LastRecMsg[this->MsgLen]='\0';
        this->MsgLen--;
        return 0;
    }
    I2C(){};
    I2C(const I2C& root) = delete;
    I2C& operator=(const I2C&) = delete;
public:
    static I2C& Instance(void)
    {
        static I2C theSingleInstance;
        return theSingleInstance;
    }
    unsigned int Init(std::string device)
    {
        this->Mutex=1;
        this->MsgLen=0;
        CleanRecMsg(void);
        SetDeviceName(device);
        int statusVal = -1;
        this->i2cfd = -1;
        SetDeviceName(device);
        char *filename = "/dev/i2c-2";
        if ((this->i2cfd = open(this->DeviceName, O_RDWR)) < 0)
        {
            perror("Failed to open the i2c bus");
            this->Mutex=0;
        }
        this->Mutex=0;
    }
    unsigned int Transaction(unsigned char address,unsigned char *buffer, unsigned int len)
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
        if (SendPacket(address,buffer, len))
        {
            this->Mutex=0;
            return 1;
        }
        this->Mutex=0;
        return 0;
    }
    unsigned char RecData(void)
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
class ConnModule
{
private:
    I2C *ptrI2C;
    unsigned char getAddress(void);
    unsigned char setAddress(unsigned char address);

public:
    ConnModule(I2C *ptrClass);
    ~ConnModule();
    unsigned int SetUUID(std::vector<unsigned char> uuid)
    {
        unsigned int cnt=3;
        unsigned char msg[4],*answer;
        unsigned int error;

        msg[0]=0x01;
        for(int i=0;i<4;i++)
        {
            msg[i+1]=uuid[i];
        }
        while(cnt--)
        {
            error=this->ptrI2C->Transaction(getAddress(),msg,5);
            if (error==0)
            {
                answer=this->ptrI2C->RecData();
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
    unsigned int SetName(unsigned char *name[]);
    unsigned int StartInit(void);
    unsigned int WriteString(unsigned char *string[]);
    unsigned int EndInit(void);
    unsigned int WriteValue(unsigned char *string[]);
    unsigned int ReadValue(unsigned char *string[]);
    unsigned int ReadLastChangedValue(void);
    unsigned int StartBonding(unsigned char *string[]);
    unsigned int CheckBonding(unsigned char *string[]);

protected:
};

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
};