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

uint8_t SPI::begin(std::string device)/*Need to check*/
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    this->Mutex=1;
    CleanRecMsg();
    SetDeviceName(device);
    int statusVal = -1;
    this->mode = SPI_MODE_0 ;
    this->bitsPerWord = 8;
    this->speed = 100000;
    this->spifd = -1;
    this->spifd = open(this->DeviceName.c_str(), O_RDWR);
    if(this->spifd < 0){
        PrintLog(Critical_log, "could not open SPI device\n");
        this->status=0;
        return NOK;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    if(statusVal < 0){
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPIMode (WR)...ioctl fail\n");
        this->status=0;
        return NOK;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
    if(statusVal < 0) {
        PrintLog(Critical_log, (std::string) __func__+  (std::string)"Could not set SPIMode (RD)...ioctl fail\n");
        this->status=0;
        return NOK;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPI bitsPerWord (WR)...ioctl fail\n");
        this->status=0;
        return NOK;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPI bitsPerWord(RD)...ioctl fail\n");
        this->status=0;
        return NOK;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
        PrintLog(Critical_log,(std::string) __func__+  (std::string)"Could not set SPI speed (WR)...ioctl fail\n");
        this->status=0;
        return NOK;
    }
    statusVal = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
        PrintLog(Critical_log, (std::string) __func__+  (std::string)"Could not set SPI speed (RD)...ioctl fail \n");
        this->status=0;
        return NOK;
    }
    this->status=1;
    this->init=1;
    this->Mutex=0;
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
    return 0;
}
uint8_t SPI::transaction(std::vector<unsigned char> buffer, uint8_t ans_len) /*Need to check*/
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    //unsigned int status=0;
    if(this->Mutex)
    {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)": Mutex blocked\n");
        return NOK;
    }
    this->Mutex=1;
    if (SendPacket(buffer,ans_len))
    {
        this->Mutex=0;
        PrintLog(Warning_log,(std::string) __func__+  (std::string)": Packet not send\n");
        return NOK;
    }
    if(CRC8(this->LastRecMsg.data(),this->LastRecMsg.size()))
    {
        PrintLog(Warning_log,(std::string) __func__+  (std::string)": CRC error\n");
        return NOK;
    }
    this->LastRecMsg.pop_back();
    PrintLog(Debug_log,(std::string) __func__+  (std::string)": Function ended succesfully\n");
    this->Mutex=0;
    return OK;
}
std::vector<unsigned char> SPI::recData(void) /*Need to check*/
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)": Function started\n");
    this->NewData=0;
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
    return LastRecMsg;

}
void SPI::changePrint(void (SPI::*ptrFunc)(uint8_t, std::string))
{
    this->ptrPrintLog=ptrFunc;
}
/*Unsafe methods*/
int SPI::SendRaw_new(unsigned char *buffer, unsigned int len, uint8_t ans_len) //original
{
    int retVal = 0;
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    unsigned char receive[ans_len];
    if (this->status==0)
    {
        PrintLog(Debug_log,(std::string) __func__+  (std::string)"Test branch\n");
        CleanRecMsg();
        this->LastRecMsg.push_back(0x03);
        this->LastRecMsg.push_back(0x01);
        this->LastRecMsg.push_back(0xEA);
        return OK;
    }
    spi_ioc_transfer send[2];
    send[0].tx_buf = (unsigned long)buffer;
    send[0].rx_buf = (unsigned long)NULL;
    send[0].len = ARRAY_SIZE(buffer);
    send[0].delay_usecs = 1000;
    send[0].speed_hz = this->speed;
    send[0].bits_per_word = this->bitsPerWord;
    send[0].tx_nbits=0;
    send[0].rx_nbits=0;
    send[0].pad=0;
    send[0].cs_change = 0;
    send[1].tx_buf = (unsigned long)NULL;
    send[1].rx_buf = (unsigned long)buffer;
    send[1].len = ans_len;
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
        PrintLog(Warning_log,(std::string) __func__+ (std::string)strerror(errno)  +(std::string)"Error during transmission\n" );
        return NOK;
    }
    CleanRecMsg();
    for (retVal = 0; retVal < (ans_len); retVal++)
    {
        this->LastRecMsg.push_back(buffer[retVal]);
    }
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
    return OK;
}
/*int SPI::ReceiveRaw_new(void) //Need to check original
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
    send.tx_nbits=0;
    send.rx_nbits=0;
    send.pad=0;
    send.cs_change = 0;

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
    if (TmpLen>4)
    {
        cout<<"Problem receiving spi data..ioctl";
        return NOK;
    }
    TmpLen=TmpLen-1;
    struct spi_ioc_transfer spi_all;
    spi_all.tx_buf = (unsigned long) &garbage[1];
    spi_all.rx_buf = (unsigned long) &buffer[1];
    spi_all.len = TmpLen*ARRAY_SIZE(test);
    spi_all.delay_usecs = 10;
    spi_all.speed_hz = this->speed;
    spi_all.bits_per_word = this->bitsPerWord;
    spi_all.cs_change = 0;
    spi_all.tx_nbits=0;
    spi_all.rx_nbits=0;
    spi_all.pad=0;
    retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(1), &spi_all) ;
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
}*/
void SPI::SetDeviceName(std::string Name)
{
    /*
             * Used for configuration of the device in construction
             */
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    this->DeviceName=Name;
    cout<<this->DeviceName;
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
}
void SPI::CleanRecMsg(void)
{
    /*
             * Explicitly cleans last read buffer
             */
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    this->LastRecMsg.clear();
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
}
/*Safe methods*/
void SPI::PrintLog(uint8_t status, std::string text)
{
    cout<<status<<text<<endl;
}
unsigned char SPI::CRC8(unsigned char *buffer, unsigned int len)
{
    unsigned char crc = 0x82;
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    while (len--)
    {
        crc ^= *buffer++;; crc = (crc & 1)? (crc >> 1) ^ 0x8c : crc >> 1;
    }
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
    return crc;
}
int SPI::SendPacket(std::vector<unsigned char> Buffer, uint8_t ans_len)
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    unsigned int FullLen;
    unsigned int i=0;
    unsigned char *temp;
    unsigned char Result[Buffer.size()+2];
    FullLen=Buffer.size()+2;
    temp=Buffer.data();
    Result[0]=FullLen;
    for(i=0;i<Buffer.size();i++)
    {
        Result[i+1]=temp[i];
    }
    Result[FullLen-1]=CRC8(Result,FullLen-1);
    if(SendRaw_new(Result, FullLen, ans_len))
    {
        PrintLog(Info_log,(std::string) __func__+  (std::string)"Transmisison error\n");
        return NOK;
    }
    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
    return OK;
}
/*int SPI::ReceivePacket(void)
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
}*/

// MCU class
// Construction and destruction
MCU::MCU(std::string filename)
{
    SPI & ptrSPI=SPI::getInstance();
    ptrSPI.begin(filename);
}
MCU::~MCU(){

}
void MCU::changePrint(void (MCU::*ptrFunc)(uint8_t, std::string))
{
    SPI & ptrSPI=SPI::getInstance();
    //ptrSPI.changePrint(&ptrFunc);
    this->ptrPrintLog=ptrFunc;
}
void MCU::PrintLog(uint8_t status, std::string text)
{
    cout<<status<<text<<endl;
}

uint8_t MCU::SetStanby(uint16_t Status)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendBool(0x00, Status);
}
std::vector<unsigned char> MCU::CheckStatus(void)
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    SPI & ptrSPI=SPI::getInstance();
    uint16_t cnt=this->WrongTransactions;
    uint16_t error;
    std::vector<unsigned char> msg, answer, null;
    msg.push_back(0x01);
    while(cnt--)
    {
        error=ptrSPI.transaction(msg,4);
        if (error==0)
        {
            answer = ptrSPI.recData();
            if (answer.size()==2)
            {
                PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
                return answer;
            }
        }
    }
    return null;
}
void MCU::RenewAll(void ) {

}
uint8_t MCU::SetSubroutine(unsigned char Routine)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, Routine);
}
uint8_t MCU::SetConnector(uint16_t  Connector)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, Connector);
}
uint8_t MCU::SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendDoubleInt(0x00, BlueButton,YellowButton);
}
uint8_t MCU::SetPower(uint16_t BlueButton, uint16_t YellowButton)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendDoubleInt(0x00, BlueButton,YellowButton);
}
uint8_t MCU::SetMaxTime(uint16_t MaxTime )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendBool(0x00, MaxTime);
}
uint8_t MCU::SetAutoStart(uint8_t Enabled )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendBool(0x00, Enabled);
}
uint8_t MCU::SetAutoStartDelay(uint16_t DelayTime )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, DelayTime);
}
uint8_t MCU::SetAutoStop(uint8_t Enabled )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, Enabled);
}
uint8_t MCU::SetAutoStopResistance(uint16_t Resistance )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, Resistance);
}
uint8_t MCU::SetIrrigation(uint8_t Enabled )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendBool(0x00, Enabled);
}
uint8_t MCU::SetModulation(uint16_t Frequency )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, Frequency);
}
uint8_t MCU::SetDutyRate(uint16_t CrestFactor )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started\n");
    return SendInt(0x00, CrestFactor);
}

uint8_t MCU::SendBool(uint8_t command,uint16_t value)
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    SPI & ptrSPI=SPI::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned int error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back(value ? 0x01 : 0x00);
    while(cnt--)
    {
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==1)
            {
                PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
                return answer.front()==(ACK|EXEC)? OK:NOK;
            }
            else if (cnt==0)
            {
                return TR_ERR;
            }
        }
    }
    return TR_ERR;
}
uint8_t MCU::SendInt(uint8_t command,uint16_t value)
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    SPI & ptrSPI=SPI::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned int error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back(uint8_t(value&0xff));
    while(cnt--)
    {
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==1)
            {
                PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
                return answer.front()==(ACK|EXEC)? OK:NOK;
            }
        }
    }
    return TR_ERR;
}
uint8_t MCU::SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2)
{
    PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started\n");
    SPI & ptrSPI=SPI::getInstance();
    uint16_t cnt=this->WrongTransactions;
    uint16_t error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back((value1&0xff));
    msg.push_back((value1>>8));
    msg.push_back(value2&0xff);
    msg.push_back((value2>>8));
    while(cnt--)
    {
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==1)
            {
                PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully\n");
                return answer.front()==(ACK|EXEC)? OK:NOK;
            }
        }
    }
    return TR_ERR;
}

int main(void)
{
    std::string filename="/dev/spidev1.0";
    MCU mcu(filename);
    std::vector<unsigned char> data;
    std::vector<unsigned char> received;
    unsigned char value= 1;
    data.push_back( value);
    cout<<"Here works0";
    mcu.SetStanby(1);
    //mcu.CheckStatus();
    return 1;
}
