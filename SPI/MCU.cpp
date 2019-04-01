//
// Created by eleps on 07.02.19.
//
using namespace std;
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "../Rest/crc.h"
#include "SPI.h"
#include "MCU.h"

// MCU class
// Construction and destruction
MCU::MCU(std::string filename,LogCallback cb)
{
    this->m_cb=cb;
    SPI & ptrSPI=SPI::getInstance();
    ptrSPI.begin(filename,cb);
}
MCU::~MCU(){

}

void MCU::PrintLog(uint8_t status, std::string text)
{
    if (this->m_cb!=0)
    {
        m_cb(status,text);
    }
}

void MCU::PrintToCout(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}

uint8_t MCU::SetStanby(uint8_t Status)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendBool(0x00, Status);
}
uint8_t MCU::CheckStatus(std::vector<unsigned char> &answer)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    SPI & ptrSPI=SPI::getInstance();
    uint16_t cnt=this->WrongTransactions;
    uint16_t error;
    std::vector<unsigned char> msg,  null;
    msg.push_back(0x01);
    while(cnt--)
    {
        PrintLog(Debug_log, (std::string) __func__+  (std::string)"Transactions left: "+ std::to_string(cnt) );
        error=ptrSPI.transaction(msg,5);
        if (error==0)
        {
            answer = ptrSPI.recData();
            if (answer.size()==4)
            {
                answer.erase(answer.begin());
                if(answer.front()==(ACK_SPI|EXEC_SPI))
                {
                    answer.erase(answer.begin());
                    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
                    return OK_SPI;
                }
                else if (cnt==0)
                {
                    if ((answer.front()&ACK_SPI)!=(ACK_SPI))
                    {
                        return NACK;
                    }
                    else
                    {
                        return NOK_SPI;
                    }
                }
            }
        }
    }
    return TR_ERR_SPI;
}
uint8_t MCU::SetSubroutine(uint8_t Routine1,uint8_t Routine2)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendInt(0x03, uint16_t(Routine2<<8|Routine1));
}
uint8_t MCU::SetConnector(uint8_t  Connector)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendChar(0x04, Connector);
}
uint8_t MCU::SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendDoubleInt(0x05, BlueButton,YellowButton);
}
uint8_t MCU::SetPower(uint16_t BlueButton, uint16_t YellowButton)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendDoubleInt(0x06, BlueButton,YellowButton);
}
uint8_t MCU::SetMaxTime(uint8_t MaxTime )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendChar(0x07, MaxTime);
}
uint8_t MCU::SetAutoStart(uint8_t Enabled )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendBool(0x08, Enabled);
}
uint8_t MCU::SetAutoStartDelay(uint8_t DelayTime )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendChar(0x09, DelayTime);
}
uint8_t MCU::SetAutoStop(uint8_t Enabled )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendBool(0x0A, Enabled);
}
uint8_t MCU::SetAutoStopResistance(uint16_t Resistance )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendInt(0x0b, Resistance);
}
uint8_t MCU::SetIrrigation(uint8_t Enabled )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendBool(0x0c, Enabled);
}
uint8_t MCU::SetIrrigationDelay(uint8_t Delay )
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendChar(0x0d, Delay);
}

uint8_t MCU::SetModulation(uint16_t Frequency1, uint16_t Frequency2)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendDoubleInt(0x0E, Frequency1,Frequency2);
}
uint8_t MCU::SetDutyCycle(uint8_t DutyCycle1 ,uint8_t DutyCycle2)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendInt(0x0f, uint16_t(DutyCycle2<<8|DutyCycle1));
}

uint8_t MCU::RenewAll(uint8_t connector, uint8_t routineCut, uint8_t routineCoag, uint16_t maxVoltageCut,
                      uint16_t maxVoltageCoag, uint16_t powerCut, uint16_t powerCoag, uint8_t maxTime,
                      uint8_t autostart, uint8_t autostartDelay, uint8_t autostop, uint16_t autostopResistance,
                      uint8_t irrigation, uint8_t irrigationDelay, uint16_t cutPWMFrequency,
                      uint16_t coagPWMFrequency, uint8_t dutyCycleCut, uint8_t dutyCycleCoag,
                      uint8_t filterCut, uint8_t filterCoag, uint8_t cutPedal, uint8_t coagPedal)
{
    SPI & ptrSPI=SPI::getInstance();
    uint8_t cnt=this->WrongTransactions;
    uint16_t error;
    std::vector<unsigned char> msg, answer;
    // Fill the buffer
    msg.push_back(0x13); // command code
    msg.push_back(connector);
    msg.push_back(routineCut);
    msg.push_back(routineCoag);
    msg.push_back(maxVoltageCut & 0xFF); // lsb
    msg.push_back(maxVoltageCut >> 8); // msb
    msg.push_back(maxVoltageCoag & 0xFF);
    msg.push_back(maxVoltageCoag >> 8);
    msg.push_back(powerCut & 0xFF);
    msg.push_back(powerCut >> 8);
    msg.push_back(powerCoag & 0xFF);
    msg.push_back(powerCoag >> 8);
    msg.push_back(maxTime);
    msg.push_back(autostart);
    msg.push_back(autostartDelay);
    msg.push_back(autostop);
    msg.push_back(autostopResistance & 0xFF);
    msg.push_back(autostopResistance >> 8);
    msg.push_back(irrigation);
    msg.push_back(irrigationDelay);
    msg.push_back(cutPWMFrequency & 0xFF);
    msg.push_back(cutPWMFrequency >> 8);
    msg.push_back(coagPWMFrequency & 0xFF);
    msg.push_back(coagPWMFrequency >> 8);
    msg.push_back(dutyCycleCut);
    msg.push_back(dutyCycleCoag);
    msg.push_back(filterCut);
    msg.push_back(filterCoag);
    msg.push_back(cutPedal);
    msg.push_back(coagPedal);
    while(cnt--)
    {
        PrintLog(Debug_log, (std::string) __func__+  (std::string)"Transactions left: "+ std::to_string(cnt) );
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==2)
            {
                answer.erase(answer.begin());
                if (answer.front()==(ACK_SPI|EXEC_SPI))
                {
                    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
                    return OK_SPI;
                }
                else if (cnt==0)
                {
                    if ((answer.front()&ACK_SPI)!=(ACK_SPI))
                    {
                        return NACK;
                    }
                    else
                    {
                        return NOK_SPI;
                    }
                }
            }
        }
    }
    return TR_ERR_SPI;
}
uint8_t MCU::SetFilter(uint8_t  Filter1,uint8_t  Filter2)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendInt(0x10, uint16_t(Filter2<<8|Filter1));
}
uint8_t MCU::SetPedalCut(uint8_t  Status)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendBool(0x11, Status);
}
uint8_t MCU::SetPedalCoag(uint8_t  Status)
{
    PrintLog(Info_log,(std::string) __func__+  (std::string)"Function started");
    return SendBool(0x12, Status);
}



uint8_t MCU::SendBool(uint8_t command,uint16_t value)
{
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    SPI & ptrSPI=SPI::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned int error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back(value ? 0x01 : 0x00);
    while(cnt--)
    {
        PrintLog(Debug_log, (std::string) __func__+  (std::string)"Transactions left: "+ std::to_string(cnt) );
        error=ptrSPI.transaction(msg,3);
        if (error==0) {
            answer = ptrSPI.recData();
            if (answer.size() == 2) {
                answer.erase(answer.begin());
                if (answer.front() == (ACK_SPI | EXEC_SPI)) {
                    PrintLog(Debug_log, (std::string) __func__ + (std::string) "Function ended succesfully");
                    return OK_SPI;
                } else if (cnt == 0) {
                    if ((answer.front() & ACK_SPI) != (ACK_SPI)) {
                        return NACK;
                    } else {
                        return NOK_SPI;
                    }
                }
            }
        }
    }
    return TR_ERR_SPI;
}
uint8_t MCU::SendInt(uint8_t command,uint16_t value)
{
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    SPI & ptrSPI=SPI::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned int error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back(uint8_t(value&0xff));
    msg.push_back(uint8_t(value>>8));
    while(cnt--)
    {
        PrintLog(Debug_log, (std::string) __func__+  (std::string)"Transactions left: "+ std::to_string(cnt) );
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==2)
            {
                answer.erase(answer.begin());
                if (answer.front()==(ACK_SPI|EXEC_SPI))
                {
                    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
                    return OK_SPI;
                }
                else if (cnt==0)
                {
                    if ((answer.front()&ACK_SPI)!=(ACK_SPI))
                    {
                        return NACK;
                    }
                    else
                    {
                        return NOK_SPI;
                    }
                }            }
        }
    }
    return TR_ERR_SPI;
}
uint8_t MCU::SendChar(uint8_t command,uint8_t value)
{
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    SPI & ptrSPI=SPI::getInstance();
    unsigned int cnt=this->WrongTransactions;
    unsigned int error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back(uint8_t(value&0xff));
    while(cnt--)
    {
        PrintLog(Debug_log, (std::string) __func__+  (std::string)"Transactions left: "+ std::to_string(cnt) );
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==2)
            {
                answer.erase(answer.begin());
                if (answer.front()==(ACK_SPI|EXEC_SPI))
                {
                    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
                    return OK_SPI;
                }
                else if (cnt==0)
                {
                    if ((answer.front()&ACK_SPI)!=(ACK_SPI))
                    {
                        return NACK;
                    }
                    else
                    {
                        return NOK_SPI;
                    }
                }            }
        }
    }
    return TR_ERR_SPI;
}
uint8_t MCU::SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2)
{
    //PrintLog(Debug_log,(std::string) __func__+  (std::string)"Function started");
    SPI & ptrSPI=SPI::getInstance();
    uint8_t cnt=this->WrongTransactions;
    uint16_t error;
    std::vector<unsigned char> msg, answer;
    msg.push_back(command);
    msg.push_back((value1&0xff));
    msg.push_back((value1>>8));
    msg.push_back(value2&0xff);
    msg.push_back((value2>>8));
    while(cnt--)
    {
        PrintLog(Debug_log, (std::string) __func__+  (std::string)"Transactions left: "+ std::to_string(cnt) );
        error=ptrSPI.transaction(msg,3);
        if (error==0)
        {
            answer=ptrSPI.recData();
            if (answer.size()==2)
            {
                answer.erase(answer.begin());
                if (answer.front()==(ACK_SPI|EXEC_SPI))
                {
                    PrintLog(Debug_log, (std::string) __func__+  (std::string)"Function ended succesfully");
                    return OK_SPI;
                }
                else if (cnt==0)
                {
                    if ((answer.front()&ACK_SPI)!=(ACK_SPI))
                    {
                        return NACK;
                    }
                    else
                    {
                        return NOK_SPI;
                    }
                }
            }
        }
    }
    return TR_ERR_SPI;
}

#ifdef C_CLASS_DEBUG
// Stand-alone compile
void PrintToC(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
int main(void)
{
    std::string filename="/dev/spidev1.0";
    MCU mcu(filename,PrintToC);
    std::vector<unsigned char> data;
    unsigned char value= 1;
    data.push_back( value);
    cout<<"Here works0";
    mcu.SetStanby(1);
    return 1;
}
#endif // QTAPP
