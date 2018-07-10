/*
 *Created by eleps on 27.04.18.
*/

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

int SPI::begin(std::string device)/*Need to check*/
{
    this->Mutex=1;
    /*if (this->init==1)
        {
            cout<<"everything initialized";
            this->Mutex=0;
            return 1;
        }*/

    CleanRecMsg();
    SetDeviceName(device);
    int statusVal = -1;
    this->mode = SPI_MODE_0 ;
    this->bitsPerWord = 8;
    this->speed = 100000;
    this->spifd = -1;
    this->spifd = open(this->DeviceName.c_str(), O_RDWR);
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
    this->init=1;
    this->Mutex=0;
    return 0;
}
unsigned int SPI::transaction(std::vector<unsigned char> buffer) /*Need to check*/
{
    //unsigned int status=0;
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
    usleep(200000);
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
std::vector<unsigned char> SPI::recData(void) /*Need to check*/
{
    this->NewData=0;
    return LastRecMsg;
}

/*Unsafe methods*/
int SPI::SendRaw_new(unsigned char *buffer, unsigned int len) //original
{
    /*struct spi_ioc_transfer spi[len];
            uint8_t tx[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};*/
    int retVal = 0;
    printf("%d",len);
    unsigned char garbage[len];
    struct spi_ioc_transfer send;
    send.tx_buf = (unsigned long)buffer;
    send.rx_buf = (unsigned long)garbage;
    send.len = ARRAY_SIZE(buffer);
    send.delay_usecs = 0;
    send.speed_hz = this->speed;
    send.bits_per_word = this->bitsPerWord;
    send.cs_change = 0;
    retVal= ioctl (this->spifd, SPI_IOC_MESSAGE(1), &send);
    if(retVal < 0)
    {
        printf("ioctl failed and returned errno %s \n",strerror(errno));
        return NOK;
    }
    for (unsigned int tst = 0; tst < len; tst++) {
        if (!(tst % 6))
            puts("");
        printf("%.2X ", buffer[tst]);
    }
    puts("");
    return OK;
}
int SPI::ReceiveRaw_new(void) //Need to check original
{
    unsigned char garbage[5]={0x03,0x01,0xEA,0x00,0x00};
    unsigned char buffer[5]={0x03,0x01,0x00,0x00,0x01};
    unsigned char test[1]={0x00};
    unsigned char TmpLen;
    int retVal = 0;
    struct spi_ioc_transfer send;
    send.tx_buf = (unsigned long)garbage;
    send.rx_buf = (unsigned long)buffer;
    send.len = ARRAY_SIZE(test);
    send.delay_usecs = 1;
    send.speed_hz = this->speed;
    send.bits_per_word = this->bitsPerWord;
    send.cs_change = 0;
    cout<<"Here works24\n";

    retVal= ioctl (this->spifd, SPI_IOC_MESSAGE(1), &send);
    if(retVal < 0)
    {
        printf("ioctl failed and returned errno %s \n",strerror(errno));
        return NOK;
    }
    for (retVal = 0; retVal < 1; retVal++) {
        if (!(retVal % 6))
            puts("");
        printf("%.2X ", buffer[retVal]);
    }
    TmpLen=buffer[0];
    printf("%d",TmpLen);
    if (TmpLen==0)
    {
        cout<<"Problem receiving spi data..ioctl";
        return NOK;
    }
    cout<<"Here works25\n";
    TmpLen=TmpLen-1;
    struct spi_ioc_transfer spi_all;
    spi_all.tx_buf = (unsigned long) &garbage[1];
    spi_all.rx_buf = (unsigned long) &buffer[1];
    spi_all.len = TmpLen*ARRAY_SIZE(test);
    spi_all.delay_usecs = 10;
    spi_all.speed_hz = this->speed;
    spi_all.bits_per_word = this->bitsPerWord;
    spi_all.cs_change = 0;
    retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi_all) ;
    cout<<"Here works26\n";;
    if(retVal < 0)
    {
        printf("ioctl failed and returned errno %s \n",strerror(errno));
        return NOK;
    }
    CleanRecMsg();
    for (retVal = 0; retVal < (TmpLen+1); retVal++) {
        if (!(retVal % 6))
            puts("");
        printf("%.2X ", buffer[retVal]);
        this->LastRecMsg.push_back(buffer[retVal]);
    }
    return OK;
}
int SPI::SendRaw(unsigned char *buffer, unsigned int len)
{
    return 0;
}
int SPI::ReceiveRaw(void)
{
    this->LastRecMsg.clear();
    this->LastRecMsg.push_back(0x02);
    this->LastRecMsg.push_back(0x02);
    this->LastRecMsg.push_back(CRC8(this->LastRecMsg.data(),2));
    return 0;
}
void SPI::SetDeviceName(std::string Name)
{
    /*
             * Used for configuration of the device in construction
             */
    this->DeviceName=Name;
    cout<<this->DeviceName;
}
void SPI::CleanRecMsg(void)
{
    /*
             * Explicitly cleans last read buffer
             */
    this->LastRecMsg.clear();
}
/*Safe methods*/
unsigned char SPI::CRC8(unsigned char *buffer, unsigned int len)
{
    unsigned char crc = 0x82;

    while (len--)
    {
        crc ^= *buffer++;; crc = (crc & 1)? (crc >> 1) ^ 0x8c : crc >> 1;
    }
    return crc;
}
int SPI::SendPacket(std::vector<unsigned char> Buffer)
{
    unsigned int FullLen=Buffer.size()+2;
    unsigned char *temp;
    temp=Buffer.data();
    unsigned char Result[FullLen];
    Result[0]=FullLen;
    cout<<this->spifd<<"\n";
    for(unsigned int i=0;i<Buffer.size();i++)
    {
        Result[i+1]=temp[i];
    }
    Result[FullLen-1]=CRC8(Result,FullLen-1);
    if(SendRaw_new(Result, FullLen))
    {
        return 1;
    }
    cout<<"Here works1\n";
    return 0;
}
int SPI::ReceivePacket(void)
{
    cout<<"Here works21\n";
    CleanRecMsg();
    cout<<this->spifd<<"\n";
    if(ReceiveRaw_new())
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

// MCU class
// Construction and destruction
MCU::MCU(std::string filename)
{
    SPI & ptrSPI=SPI::getInstance();
    ptrSPI.begin(filename);
}
MCU::~MCU(){

}

uint16_t MCU::SetStanby(uint16_t Status)
{
    return SendBool(0x00, Status);
}
std::vector<unsigned char> MCU::CheckStatus(void)
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
    return answer;
}
void MCU::RenewAll(void ) {

}
uint16_t MCU::SetSubroutine(unsigned char Routine)
{
    return SendInt(0x00, Routine);
}
uint16_t MCU::SetConnector(uint16_t  Connector)
{
    return SendInt(0x00, Connector);
}
uint16_t MCU::SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton)
{
    return SendDoubleInt(0x00, BlueButton,YellowButton);
}
uint16_t MCU::SetPower(uint16_t BlueButton, uint16_t YellowButton)
{
    return SendDoubleInt(0x00, BlueButton,YellowButton);
}
uint16_t MCU::SetMaxTime(uint16_t MaxTime )
{
    return SendBool(0x00, MaxTime);
}
uint16_t MCU::SetAutoStart(uint8_t Enabled )
{
    return SendBool(0x00, Enabled);
}
uint16_t MCU::SetAutoStartDelay(uint16_t DelayTime )
{
    return SendInt(0x00, DelayTime);
}
uint16_t MCU::SetAutoStop(uint8_t Enabled )
{
    return SendInt(0x00, Enabled);
}
uint16_t MCU::SetAutoStopResistance(uint16_t Resistance )
{
    return SendInt(0x00, Resistance);
}
uint16_t MCU::SetIrrigation(uint8_t Enabled )
{
    return SendBool(0x00, Enabled);
}
uint16_t MCU::SetModulation(uint16_t Frequency )
{
    return SendInt(0x00, Frequency);
}
uint16_t MCU::SetDutyRate(uint16_t CrestFactor )
{
    return SendInt(0x00, CrestFactor);
}

uint16_t MCU::SendBool(uint8_t command,uint16_t value)
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
    return answer.front()==ACK? ACK:NACK;
}
uint16_t MCU::SendInt(uint8_t command,uint16_t value)
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
    }
    return TR_ERR;
}
uint16_t MCU::SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2)
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
    }
    return TR_ERR;
}

/*int main(void)
{
    std::string filename="/dev/spidev1.0";
    //SPI & test =SPI::getInstance();
    MCU mcu(filename);
    //MCU mcu2(filename);
    //test.begin(filename);
    std::vector<unsigned char> data;
    std::vector<unsigned char> received;
    unsigned char value= 1;
    data.push_back( value);
    cout<<"Here works0";
    mcu.SetStanby(1);
    mcu.CheckStatus();
    return 1;
}*/
