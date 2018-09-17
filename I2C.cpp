/*
 *Created by eleps on 27.04.18.
 */

#include "I2C.h"

// I2C class
I2C* I2C::theOneTrueInstance;

// Construction and destruction
I2C::I2C() {
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"I2C constructor called\n");
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
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Device name set to :"
                       +(std::string)this->DeviceName +(std::string)"\n");
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

    /*PrintLog(Debug_log,(std::string) __func__+  (std::string)"Send to address:"
                       +(std::string)address +(std::string)"Data: "+(std::string)buffer+(std::string)"\n");*/
    unsigned char buf_rec[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
            ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    std::string data_send,data_rec;
    //data_send=data_send+(std::string)(address.front());
    for (int cnt=0;cnt<buffer.size();cnt++)
    {
        //data_send=data_send+(std::string)(buffer.data()[cnt]);
    }
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Send message"
                       +data_send +(std::string)"\n");
    i2c_rdwr_ioctl_data message;
    i2c_msg message_packet[2];
    printf("%02x\n",address.front());
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
    //cout<<"i2copen"<<endl;
    if (file == -1)
    {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)"Device open error\n");
        return NOK;
        //perror("/dev/i2c-2");
    }
    if (ioctl(file, I2C_SLAVE, address.front()) < 0)
    {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)"Failed to acquire bus access and/or talk to slave\n");
        //perror("Failed to acquire bus access and/or talk to slave");
        return NOK;
    }
    ret=ioctl(file, I2C_RDWR, &message);
    if (ret<0) {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)strerror(-ret)
                             +(std::string)"Unable to send message\n");
        //fprintf (stderr, "%s.\n", strerror(-ret));
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
    for (cnt=0;cnt<this->LastRecMsg.size();cnt++)
    {
        //data_rec=data_rec+(std::string)(this->LastRecMsg.data()[cnt]);
    }
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Send message"
                       +data_rec +(std::string)"\n");
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
    for(unsigned int i=0;i<buffer.size();i++)
    {
        crc_tr.push_back(temp_buf[i]);
    }
    for(unsigned int i=0;i<buffer.size();i++)
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
    for(unsigned int i=0;i<this->LastRecMsg.size();i++)
    {
        crc_rec.push_back(temp_rec[i]);
        printf("%02x\n",temp_rec[i]);
    }
    printf("%02x\n",CRC8(crc_rec.data(),crc_rec.size()));
    if(CRC8(crc_rec.data(),crc_rec.size()))
    {
        PrintLog(Warning_log,(std::string) __func__ +(std::string)"CRC error\n");
        return NOK;
    }
    this->LastRecMsg.pop_back();
    this->NewData=1;
    return OK;
}

unsigned int I2C::begin(std::string device,CallbackFunction cb)
{
    this->Mutex=1;
    if (this->init==1)
    {
        cout<<"everything initialized";
        this->Mutex=0;
        return 1;
    }
    this->m_cb=0;
    this->m_cb=cb;
    CleanRecMsg();
    SetDeviceName(device);
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Initialized\n");
    this->Mutex=0;
}
unsigned int I2C::transaction(std::vector<unsigned char> address,std::vector<unsigned char> buffer, unsigned int len)
{
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
    this->Mutex=0;
    return OK;
}


// ConnModule class
// Construction and destruction
ConnModule::ConnModule(std::string filename,CallbackFunction cb)
{
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
void ConnModule::setAddress(std::vector<unsigned char> addr)
{
    unsigned char temp_addr=addr.front();
    addr.pop_back();
    addr.push_back(temp_addr&0x7f);
    this->addr=(addr);
}
uint8_t ConnModule::SetUUID(std::vector<unsigned char> uuid, std::vector<unsigned char> &response)
{

    response=WriteArray(0x01, uuid, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set UUID failed\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set UUID succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set UUID failed, NACK\n");
    return NOK;
}
uint8_t ConnModule::SetName(std::vector<unsigned char> data, std::vector<unsigned char> &response)
{
    response=WriteArray(0x02, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set SetName failed\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set SetName succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"Set SetName failed, NACK\n");
    return NOK;
}
uint8_t ConnModule::StartInit(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    response=WriteArray(0x03, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartInit failed\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartInit succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartInit failed, NACK\n");
        return NOK;
}
uint8_t ConnModule::WriteString(std::vector<unsigned char> data, std::vector<unsigned char> &response)
{
    response=WriteArray(0x04, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteString failed\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteString succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteString failed, NACK\n");
    return NOK;
}
uint8_t ConnModule::EndInit(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    response=WriteArray(0x05, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"EndInit failed\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"EndInit succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"EndInit failed, NACK\n");
    return NOK;
}
uint8_t ConnModule::CheckBonding(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    response=WriteArray(0x09, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"CheckBonding failed\n");
        return NOK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"CheckBonding failed succesfuly\n");
    return OK;
}

uint8_t ConnModule::WriteValue(std::vector<unsigned char> id, std::vector<unsigned char> value, std::vector<unsigned char> &response)
{
    std::vector<unsigned char> data;
    data.insert(std::end(data), std::begin(id), std::end(id));
    data.insert(std::end(data), std::begin(value), std::end(value));
    response=WriteArray(0x06, data, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteValue failed, wrong responce\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteValue succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"WriteValue failed, NACK\n");
    return NOK;
}
uint8_t ConnModule::StartBonding(std::vector<unsigned char> db, std::vector<unsigned char> &response)
{
    response=WriteArray(0x08, db, 3);
    if (response.size()!=1)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartBonding failed\n");
        return NOK;
    }
    if (response.front()==ACK_I2C)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartBonding succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"StartBonding failed\n");
    return NOK;
}
uint8_t ConnModule::ReadValue(uint16_t id_rec, std::map <uint16_t,std::vector<unsigned char> > answer) /*change order of message*/
{
    std::vector<unsigned char> responce;
    std::vector<unsigned char> data,val;
    uint16_t id=0x0000;
    uint32_t value=0x00000000;
    data.push_back(id_rec&0xff);
    data.push_back((id_rec>>8)&0xff);
    responce=WriteArray(0x07, data, 8);
    if (responce.size()==0)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadValue failed\n");
        return NOK;
    }
    if (responce.size()%4)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadValue failed\n");
        return NOK;
    }
    /*id=responce.front()<<8;
    responce.erase(responce.begin());
    id|=responce.front();
    responce.erase(responce.begin());*/
    id=id_rec;
    val.push_back(responce.front());
    responce.erase(responce.begin());
    val.push_back(responce.front());
    responce.erase(responce.begin());
    val.push_back(responce.front());
    responce.erase(responce.begin());
    val.push_back(responce.front());
    responce.erase(responce.begin());
    answer.insert (pair<uint16_t,std::vector<unsigned char>>(id_rec,val));
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadValue succesfuly\n");
    return OK;

}
uint8_t ConnModule::ReadLastChangedValue(std::map <uint16_t,std::vector<unsigned char>> answer) /*change order of message*/
{
    std::vector<unsigned char> responce;
    std::vector<unsigned char> data,val;
    uint16_t id;
    uint32_t value;
    responce=WriteArray(0x0C, data, 10);
    if (responce.size()==0)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue failed\n");
        return NOK;
    }
    if (responce.size()%6)
    {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue failed\n");
        return NOK;
    }
    while(responce.size()!=0)
    {
        id=0x0000;
        value=0x00000000;
        val.clear();
        id=responce.front()<<8;
        responce.erase(responce.begin());
        id|=responce.front();
        responce.erase(responce.begin());
        val.push_back(responce.front());
        responce.erase(responce.begin());
        val.push_back(responce.front());
        responce.erase(responce.begin());
        val.push_back(responce.front());
        responce.erase(responce.begin());
        val.push_back(responce.front());
        responce.erase(responce.begin());
        answer.insert ( pair<uint16_t,std::vector<unsigned char>>(id,val) );
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue succesfuly\n");
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
BoardModule::BoardModule(std::string filename,CallbackFunction cb)
{
    I2C &ptrI2C = I2C::getInstance();
    //this->m_cb=0;
    //this->m_cb=cb;
    ptrI2C.begin(filename,cb);
    std::vector<unsigned char> address;
    address.push_back(0b01001000);
    printf("%02x\n",address.front());
    this->addr=address;
}
BoardModule::~BoardModule()
{

}

std::vector<unsigned char> BoardModule::getAddress(void)
{
    return this->addr;
}
uint8_t BoardModule::SetBonding(unsigned char enable,std::vector<unsigned char> &response)
{
    std::vector<unsigned char> msg;
    msg.push_back(enable);
    response = WriteArray(0x06, msg, 4);
    if (response.size() != 2) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion failed\n");
        return NOK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion succesfuly\n");
    return OK;
}
void BoardModule::setAddress(std::vector<unsigned char> addr)
{
    unsigned char temp_addr=addr.front();
    addr.pop_back();
    addr.push_back(temp_addr&0x7f);
    this->addr=(addr);
}
uint8_t BoardModule::GetVersion(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> msg;
    response = WriteArray(0x01, msg, 4);
    if (response.size() != 2) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion failed\n");
        return NOK;
    }
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetVersion succesfuly\n");
        return OK;
}
uint8_t BoardModule::GetTools(std::vector<unsigned char> &response)
{
    std::vector<unsigned char> msg;
    response = WriteArray(0x02, msg, 4);
    if (response.size() != 2) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetTools failed\n");
        return NOK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetTools succesfuly\n");
    return OK;
}
uint8_t BoardModule::GetPower(std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    responce = WriteArray(0x03, msg, 3);
    if (responce.size() != 1) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetPower failed\n");
        return NOK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"GetPower succesfuly\n");
    return OK;
}
uint8_t BoardModule::SetEnergy(unsigned char energy,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    msg.push_back(energy);
    responce = WriteArray(0x04, msg, 3);
    if (responce.size() != 1) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetEnergy failed\n");
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetEnergy succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetEnergy failed\n");
    return NOK;
}
uint8_t BoardModule::SetVolume(unsigned char volume,std::vector<unsigned char> &responce)
{
    std::vector<unsigned char> msg;
    msg.push_back(volume);
    responce = WriteArray(0x05, msg, 3);
    if (responce.size() != 1) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetVolume failed\n");
        return NOK;
    }
    if (responce.front() == ACK_I2C) {
        PrintLog(Debug_log,(std::string) __func__ +(std::string)"ReadLastChangedValue succesfuly\n");
        return OK;
    }
    PrintLog(Debug_log,(std::string) __func__ +(std::string)"SetVolume failed\n");
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

#ifndef QTAPP
void PrintToC(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
int main(void)
{
    std::string filename="/dev/i2c-2";
    cout<<"1"<<endl;
    ConnModule mcu(filename,PrintToC);
    cout<<"2"<<endl;
    std::vector<unsigned char> data;
    mcu.StartInit(data);
    cout<<"3\n";
    printf("%zu\n",data.size());
    return 1;
}
#endif // QTAPP
