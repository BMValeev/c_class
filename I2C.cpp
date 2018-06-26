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
#define INVALID_DATA 0x03
#define TR_ERR 0x05
#define ACK 0x06
#define NACK 0x015
#define BOF 0x20
#define MSP 0x21
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
    int SendRaw_new(std::vector<unsigned char> address, std::vector<unsigned char> buffer, unsigned int r_ len)
    {
        struct i2c_client *client = this->i2c_data;
        struct i2c_msg msgs[2];
        unsigned int len=buffer.size();
        unsigned char txbuf[buffer.size()];
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
        msgs[1].len = r_ len;
        msgs[1].buf = this->LastRecMsg;

        ret = i2c_transfer(client->adapter, msgs, 2);
        if (ret < 0)
        {
            return ret;
        }
        return 0;
    }
    int SendRaw(std::vector<unsigned char> address,std::vector<unsigned char>, unsigned int len)
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
        this->NewData=0;
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
    int SendPacket(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
    {
        unsigned char FullLen=buffer.size()+3;
        unsigned char *temp;
        std::vector<unsigned char> package;
        temp=buffer.data();
        package.push_back(FullLen);
        cout<<"Here works1";
        for(int i=0;i<buffer.size();i++)
        {
            package.push_back(temp[i]);
        }
        package.push_back(CRC8(package.data(),FullLen-2));
        CleanRecMsg();
        if(SendRaw(address, package,len))
        {
            return 1;
        }
        cout<<"Here works2";
        if(CRC8(this->LastRecMsg.data(),this->LastRecMsg.size()))
        {
            return 1;
        }
        this->NewData=1;
        return 0;
    }
public:
    static I2C & getInstance() {
        if (!theOneTrueInstance) initInstance();
        return *theOneTrueInstance;
    }
    static void initInstance() { new I2C; }
    unsigned int begin(std::string device, i2c_client *client)
    {
        this->Mutex=1;
        if (this->init==1)
        {
            cout<<"everything initialized";
            this->Mutex=0;
            return 1;
        }
        this->i2c_data=client;
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
    unsigned int transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
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
        if (SendPacket(address,buffer,len))
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
    ConnModule(std::string filename,struct i2c_client *client) {
        I2C &ptrI2C = I2C::getInstance();
        ptrI2C.begin(filename, client);
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
        unsigned char *array=uuid.data();
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        if (uuid.size()!=4)
        {
            return INVALID_DATA;
        }
        msg.push_back(0x01);
        for(int i=uuid.size()-1;i>=0;i--)
        {
            msg.push_back(array[i]);
        }
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    unsigned int SetName(std::vector<unsigned char> data)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned char *array=data.data();
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        if (data.size()!=20) {
            return INVALID_DATA;
        }
        msg.push_back(0x02);
        for(int i=data.size()-1;i>=0;i--)
        {
            msg.push_back(array[i]);
        }
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    unsigned int StartInit(void)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x03);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    unsigned int WriteString(std::vector<unsigned char> data)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned char *array=data.data();
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        if (data.size()!=20)
        {
            return INVALID_DATA;
        }
        msg.push_back(0x04);
        for(int i=data.size()-1;i>=0;i--)
        {
            msg.push_back(array[i]);
        }
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==1) {
                    switch(answer.front()) {
                        case ACK:   return ACK;
                        case NACK:  return NACK;
                        case BOF:   return BOF;
                        default:    return NACK;
                    }
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
    unsigned int EndInit(void)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x05);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    unsigned int WriteValue(std::vector<unsigned char> id,std::vector<unsigned char> value)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned char *array_id=id.data();
        unsigned char *array_value=value.data();
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        if (id.size()!=2) {
            return INVALID_DATA;
        }
        if (value.size()!=4) {
            return INVALID_DATA;
        }
        msg.push_back(0x06);
        for(int i=id.size()-1;i>=0;i--) {
            msg.push_back(array_id[i]);
        }
        for(int i=value.size()-1;i>=0;i--) {
            msg.push_back(array_value[i]);
        }
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==1) {
                    switch(answer.front()) {
                        case ACK:   return ACK;
                        case NACK:  return NACK;
                        case MSP:   return MSP;
                        default:    return NACK;
                    }
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
    std::vector<unsigned char> ReadValue(std::vector<unsigned char> id) /*change order of message*/
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned char *array_id=id.data();
        std::vector<unsigned char> null;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        if (id.size()!=2) {
            return null;
        }
        msg.push_back(0x07);
        for(int i=id.size()-1;i>=0;i--) {
            msg.push_back(array_id[i]);
        }
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,6);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==4) {
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
    std::vector<unsigned char> ReadLastChangedValue(void) /*change order of message*/
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> null;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x0C);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,32);
            if (error==0) {
                answer=ptrI2C.recData();
                return answer;
            }
            else if (cnt==0) {
                return null;
            }
        }
    }
    unsigned int StartBonding(std::vector<unsigned char> db)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned char *array_id=db.data();
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        if (db.size()!=1) {
            return INVALID_DATA;
        }
        msg.push_back(0x08);
        for(int i=db.size()-1;i>=0;i--) {
            msg.push_back(array_id[i]);
        }
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    std::vector<unsigned char> CheckBonding(std::vector<unsigned char> data)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> null;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x09);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==1) {
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

protected:
};

class BoardModule
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
    BoardModule(std::string filename,struct i2c_client *client) {
        I2C &ptrI2C = I2C::getInstance();
        ptrI2C.begin(filename, client);
        std::vector<unsigned char> address;
        address.push_back(0x01);
        this->addr=address;
    }
    ~BoardModule()
    {

    }
    std::vector<unsigned char> GetVersion(void)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer, null;
        msg.push_back(0x01);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,4);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==2) {
                    //if answer
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
    std::vector<unsigned char> GetTools(void)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer, null;
        msg.push_back(0x02);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,4);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==2) {
                    //if answer
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
    std::vector<unsigned char> GetPower(void)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer, null;
        msg.push_back(0x03);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
                if (answer.size()==2) {
                    //if answer
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
    unsigned int SetEnergy(unsigned char energy)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x01);
        msg.push_back(energy[i]);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    unsigned int SetVolume(unsigned char volume)
    {
        I2C & ptrI2C=I2C::getInstance();
        unsigned int cnt=this->WrongTransactions;
        unsigned int error;
        std::vector<unsigned char> msg, answer;
        msg.push_back(0x01);
        msg.push_back(volume);
        while(cnt--) {
            cout<<"prepared "<<cnt<<"\n";
            error=ptrI2C.transaction(this->addr,msg,3);
            if (error==0) {
                answer=ptrI2C.recData();
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
    struct i2c_client *i2c_client;
    I2C & test =I2C::getInstance();
    test.begin(filename, i2c_client);
    std::vector<unsigned char> data;
    std::vector<unsigned char> address;
    address.push_back(0x01);
    std::vector<unsigned char> received;
    ConnModule module(filename, i2c_client);
    unsigned char value= 1;
    data.push_back( value);
    address.push_back( 0x01);
    cout<<"Here works0";
    test.transaction(address,data,3);
    module.SetUUID(data);
    return 1;
}