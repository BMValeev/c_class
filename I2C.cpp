/*
 *Created by eleps on 27.04.18.
 */

#include "I2C.h"

// I2C class
I2C* I2C::theOneTrueInstance;

// Construction and destruction
I2C::I2C() {
    if (theOneTrueInstance) throw std::logic_error("Instance already exists");
    theOneTrueInstance = this;
}
I2C::~I2C() { }
I2C & I2C::getInstance() {
    if (!theOneTrueInstance) initInstance();
    return *theOneTrueInstance;
}
void I2C::initInstance() { new I2C; }



std::vector<unsigned char> I2C::recData(void)
{
    this->NewData=0;
    return LastRecMsg;
}
void I2C::CleanRecMsg(void)
{
    /*
         * Explicitly cleans last read buffer
         */
    this->LastRecMsg.clear();
    this->NewData=0;
}
void I2C::SetDeviceName(std::string Name)
{
    /*
         * Used for configuration of the device in construction
         */
    this->DeviceName=Name;
}
void I2C::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=0)
    {
        m_cb(status,text);
    }
}

void I2C::PrintToCout(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
/*Safe functions*/
unsigned char I2C::CRC8(unsigned char *buffer, unsigned int len)
{
    unsigned char crc = 0x82;
    unsigned int i;

    while (len--)
    {
        crc ^= *buffer++;
        for (i = 0; i < 8; i++)
            crc = (crc & 1)? (crc >> 1) ^ 0x8c : crc >> 1;
    }
    return crc;
}

/*Unsafe Methods*/
int I2C::SendRaw_new(std::vector<unsigned char> address, std::vector<unsigned char> buffer, unsigned int rlen)
{
    int ret;
    unsigned int cnt, cnt_all;
    /*unsigned char buf[10]= {0};
    buf[0] = 0b11100000;
    buf[1] = 0b0000001;
    buf[2] = 0b0000011;
    buf[3] = 0b0000111;
    buf[4] = 0b0001111;
    buf[5] = 0b0011111;
    buf[6] = CRC8(buf,6);*/


    unsigned char buf_rec[3]={0x00,0x00,0x00};

    i2c_rdwr_ioctl_data message;
    i2c_msg message_packet[2];
    message_packet[0].addr=address.front();
    message_packet[0].flags=0;
    message_packet[0].len=buffer.size();
    message_packet[0].buf=buffer.data();
    message_packet[1].addr=address.front();
    message_packet[1].flags=I2C_M_RD;//I2C_M_RD|I2C_M_RECV_LEN;I2C_M_NOSTART
    message_packet[1].buf=buf_rec;
    message_packet[1].len=10;
    if (rlen==0)
    {
        message_packet[1].len=10;
        cnt_all=10;
    }
    else if (rlen>7)
    {
        message_packet[1].len=20;
        cnt_all=20;
    }
    else
    {
        message_packet[1].len=rlen;
        cnt_all=rlen;
    }
    memset(&message, 0, sizeof(message));
    message.msgs=message_packet;
    if (rlen!=0)
    {
        message.nmsgs=2;
    }
    else
    {
        message.nmsgs=1;
    }
    int file = open(this->DeviceName.c_str(), O_RDWR);
    cout<<"i2copen"<<endl;
    if (file == -1)
    {
        perror("/dev/i2c-2");
    }
    if (ioctl(file, I2C_SLAVE, address.front()) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        return NOK;
    }
    cout<<"ioctl"<<endl;
    ret=ioctl(file, I2C_RDWR, &message);
    cout<<"ioctlmessge"<<endl;
    if (ret<0) {
        fprintf (stderr, "%s.\n", strerror(-ret));
        return NOK;
    }
    CleanRecMsg();
    for (cnt = 0; cnt < (cnt_all); cnt++)
    {
        this->LastRecMsg.push_back(buf_rec[cnt]);
    }
    cnt_all=cnt_all-this->LastRecMsg.front();
    while (--cnt_all)
    {
        this->LastRecMsg.pop_back();
    }
    return OK;
}

int I2C::SendRaw(std::vector<unsigned char> address,std::vector<unsigned char>, unsigned int len)
{
    this->LastRecMsg.clear();
    this->LastRecMsg.push_back(0x02);
    this->LastRecMsg.push_back(0x02);
    this->LastRecMsg.push_back(CRC8(this->LastRecMsg.data(),2));
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
    for(int i=0;i<buffer.size();i++)
    {
        crc_tr.push_back(temp_buf[i]);
    }
    for(int i=0;i<buffer.size();i++)
    {
        package.push_back(temp_buf[i]);
    }
    package.push_back(CRC8(crc_tr.data(),crc_tr.size()));
    CleanRecMsg();
    if(SendRaw_new(address, package,len))
    {
        return NOK;
    }
    temp_rec=this->LastRecMsg.data();
    crc_rec.push_back(temp_addr[0]<<1);
    for(int i=0;i<this->LastRecMsg.size();i++)
    {
        crc_tr.push_back(temp_rec[i]);
        printf("%02x\n",temp_rec[i]);
    }
    printf("%02x\n",CRC8(crc_tr.data(),crc_tr.size()));
    if(CRC8(crc_tr.data(),crc_tr.size()))
    {
        return NOK;
    }
    this->LastRecMsg.pop_back();
    this->NewData=1;
    return OK;
}

unsigned int I2C::begin(std::string device,CallbackFunction cb)
{
    int file;
    this->Mutex=1;
    if (this->init==1)
    {
        cout<<"everything initialized";
        this->Mutex=0;
        return 1;
    }
    //this->m_cb=0;
    //this->m_cb=cb;
    CleanRecMsg();
    SetDeviceName(device);
    //file = open(this->DeviceName.c_str(), O_RDWR);
    /*if (file == -1)
    {
        perror(this->DeviceName.c_str());
    }
    else
    {
        this->i2cfd=file;
    }*/
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
unsigned int I2C::transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
{
    unsigned int status=0;
    if(this->Mutex)
    {
        return NOK;
    }
    if(this->NewData)
    {
        return NOK;
    }
    this->Mutex=1;
    if (SendPacket(address,buffer,len))
    {
        this->Mutex=0;
        return NOK;
    }
    cout<<"Here works3";
    this->Mutex=0;
    return OK;
}


// ConnModule class
// Construction and destruction
ConnModule::ConnModule(std::string filename,CallbackFunction cb) {
    I2C &ptrI2C = I2C::getInstance();
    std::vector<unsigned char> address;
    cout<<"3"<<endl;
    //this->m_cb=0;
    //this->m_cb=cb;
    cout<<"4"<<endl;
    ptrI2C.begin(filename,cb);
    cout<<"5"<<endl;
    address.push_back(0b01110000);
    cout<<"55"<<endl;
    //setAddress(address);
    this->addr=address;
    cout<<"6"<<endl;
}
ConnModule::~ConnModule() {

}

std::vector<unsigned char> ConnModule::getAddress(void) {
    return this->addr;
}
std::vector<unsigned char> ConnModule::setAddress(std::vector<unsigned char>addr)
{
    this->addr=addr;
}
uint8_t ConnModule::SetUUID(std::vector<unsigned char> uuid,std::vector<unsigned char> &responce)
{
    responce=WriteArray(0x01, uuid, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
    return NOK;
}
uint8_t ConnModule::SetName(std::vector<unsigned char> data,std::vector<unsigned char> &responce)
{
    responce=WriteArray(0x02, data, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
    return NOK;
}
uint8_t ConnModule::StartInit(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> data;
    cout<<"startinit"<<endl;
    responce=WriteArray(0x03, data, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
        return NOK;
}
uint8_t ConnModule::WriteString(std::vector<unsigned char> data,std::vector<unsigned char> &responce)
{
    responce=WriteArray(0x04, data, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
    return NOK;
}
uint8_t ConnModule::EndInit(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> data;
    responce=WriteArray(0x05, data, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
    return NOK;
}
uint8_t ConnModule::CheckBonding(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> data;
    responce=WriteArray(0x09, data, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    return OK;
}

uint8_t ConnModule::WriteValue(std::vector<unsigned char> id,std::vector<unsigned char> value,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> data;
    data.insert(std::end(data), std::begin(id), std::end(id));
    data.insert(std::end(data), std::begin(value), std::end(value));
    responce=WriteArray(0x06, data, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
    return NOK;
}
uint8_t ConnModule::StartBonding(std::vector<unsigned char> db,std::vector<unsigned char> &responce)
{
    responce=WriteArray(0x08, db, 3);
    if (responce.size()!=1)
    {
        return NOK;
    }
    if (responce.front()==ACK_I2C)
    {
        return OK;
    }
    return NOK;
}
uint8_t ConnModule::ReadValue(std::map <uint16_t,uint32_t> answer) /*change order of message*/
{
    std::vector<unsigned char> responce;
    std::vector<unsigned char> data;
    uint16_t id=0x0000;
    uint32_t value=0x00000000;
    responce=WriteArray(0x07, data, 8);
    if (responce.size()==0)
    {
        return NOK;
    }
    if (responce.size()%6)
    {
        return NOK;
    }
    id=responce.front()<<8;
    responce.erase(responce.begin());
    id|=responce.front();
    responce.erase(responce.begin());
    value=responce.front()<<24;
    responce.erase(responce.begin());
    value|=responce.front()<<16;
    responce.erase(responce.begin());
    value|=responce.front()<<8;
    responce.erase(responce.begin());
    value|=responce.front();
    responce.erase(responce.begin());
    answer.insert ( pair<uint16_t,uint32_t>(id,value) );
    return OK;

}
uint8_t ConnModule::ReadLastChangedValue(std::map <uint16_t,uint32_t> answer) /*change order of message*/
{
    std::vector<unsigned char> responce;
    std::vector<unsigned char> data;
    uint16_t id;
    uint32_t value;
    responce=WriteArray(0x0C, data, 10);
    if (responce.size()==0)
    {
        return NOK;
    }
    if (responce.size()%6)
    {
        return NOK;
    }
    while(responce.size()!=0)
    {
        id=0x0000;
        value=0x00000000;
        id=responce.front()<<8;
        responce.erase(responce.begin());
        id|=responce.front();
        responce.erase(responce.begin());
        value=responce.front()<<24;
        responce.erase(responce.begin());
        value|=responce.front()<<16;
        responce.erase(responce.begin());
        value|=responce.front()<<8;
        responce.erase(responce.begin());
        value|=responce.front();
        responce.erase(responce.begin());
        answer.insert ( pair<uint16_t,uint32_t>(id,value) );
    }
    return OK;
}




std::vector<unsigned char> ConnModule::WriteArray(uint8_t command,std::vector<unsigned char> data,unsigned int len)
{
    I2C & ptrI2C=I2C::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned char *array=data.data();
    unsigned int error;
    std::vector<unsigned char> msg, answer,null;
    msg.push_back(command);
    for(int i=data.size()-1;i>=0;i--)
    {
        msg.push_back(array[i]);
    }
    while(cnt--) {
        error=ptrI2C.transaction(this->addr,msg,len);
        if (error==0) {
            answer=ptrI2C.recData();
            answer.erase(answer.begin());
            return answer;
        }
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

// BoardModule class
// Construction and destruction
BoardModule::BoardModule(std::string filename,CallbackFunction cb) {
    I2C &ptrI2C = I2C::getInstance();
    this->m_cb=0;
    this->m_cb=cb;
    ptrI2C.begin(filename,cb);
    std::vector<unsigned char> address;
    address.push_back(0x01);
    this->addr=address;
}
BoardModule::~BoardModule()
{

}

std::vector<unsigned char> BoardModule::getAddress(void)
{
    return this->addr;
}
void BoardModule::setAddress(std::vector<unsigned char> addr)
{
    this->addr=addr;
}
uint8_t BoardModule::GetVersion(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    responce = WriteArray(0x01, msg, 3);
    if (responce.size() != 1) {
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        return OK;
    }
    return NOK;
}
uint8_t BoardModule::GetTools(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    responce = WriteArray(0x01, msg, 3);
    if (responce.size() != 1) {
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        return OK;
    }
    return NOK;
}
uint8_t BoardModule::GetPower(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    responce = WriteArray(0x03, msg, 3);
    if (responce.size() != 1) {
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        return OK;
    }
    return NOK;
}
uint8_t BoardModule::SetEnergy(unsigned char energy,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    msg.push_back(energy);
    responce = WriteArray(0x01, msg, 3);
    if (responce.size() != 1) {
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        return OK;
    }
    return NOK;
}
uint8_t BoardModule::SetVolume(unsigned char volume,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    msg.push_back(volume);
    responce = WriteArray(0x01, msg, 3);
    if (responce.size() != 1) {
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        return OK;
    }
    return NOK;
}
std::vector<unsigned char> BoardModule::WriteArray(uint8_t command,std::vector<unsigned char> data,unsigned int len)
{
    I2C & ptrI2C=I2C::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned char *array=data.data();
    unsigned int error;
    std::vector<unsigned char> msg, answer,null;
    msg.push_back(command);
    for(int i=data.size()-1;i>=0;i--)
    {
        msg.push_back(array[i]);
    }
    while(cnt--) {
        error=ptrI2C.transaction(this->addr,msg,len);
        if (error==0) {
            answer=ptrI2C.recData();
            answer.erase(answer.begin());
            return answer;
        }
    }
    return null;
}
void BoardModule::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=0)
    {
        m_cb(status,text);
    }
}

void BoardModule::PrintToCout(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
/*int main(void)
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
}*/
