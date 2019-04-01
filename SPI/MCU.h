//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_MCU_H
#define C_CLASS_MCU_H
#include "SPI.h"
class MCU
{
public:
    MCU(std::string filename, LogCallback cb);
    ~MCU();
    uint8_t SetStanby(uint8_t Status);
    uint8_t CheckStatus(std::vector<unsigned char> &answer);
    void RenewAll( );
    uint8_t SetSubroutine(uint8_t Routine1,uint8_t Routine2);
    uint8_t SetConnector(uint8_t  Connector);
    uint8_t SetMaxVoltage(uint16_t BlueButton, uint16_t YellowButton);
    uint8_t SetPower(uint16_t BlueButton, uint16_t YellowButton);
    uint8_t SetMaxTime(uint8_t MaxTime );
    uint8_t SetAutoStart(uint8_t Enabled );
    uint8_t SetAutoStartDelay(uint8_t DelayTime );
    uint8_t SetAutoStop(uint8_t Enabled );
    uint8_t SetAutoStopResistance(uint16_t Resistance );
    uint8_t SetIrrigation(uint8_t Enabled );
    uint8_t SetIrrigationDelay(uint8_t Delay );
    uint8_t SetModulation(uint16_t Frequency1, uint16_t Frequency2);
    uint8_t SetPedalCoag(uint8_t  Status);
    uint8_t SetPedalCut(uint8_t  Status);
    uint8_t SetFilter(uint8_t  Filter1,uint8_t  Filter2);
    uint8_t SetDutyCycle(uint8_t DutyCycle1 ,uint8_t DutyCycle2);
    uint8_t RenewAll(uint8_t connector, uint8_t routineCut, uint8_t routineCoag, uint16_t maxVoltageCut, uint16_t maxVoltageCoag,
                     uint16_t powerCut, uint16_t powerCoag, uint8_t maxTime, uint8_t autostart, uint8_t autostartDelay, uint8_t autostop,
                     uint16_t autostopResistance, uint8_t irrigation, uint8_t irrigationDelay, uint16_t cutPWMFrequency, uint16_t coagPWMFrequency,
                     uint8_t dutyCycleCut, uint8_t dutyCycleCoag, uint8_t filterCut, uint8_t filterCoag, uint8_t cutPedal, uint8_t coagPedal);

private:
    uint8_t WrongTransactions=3;
    void PrintLog(uint8_t status, std::string text);
    static void PrintToCout(uint8_t status, std::string msg);
    LogCallback m_cb = MCU::PrintToCout;

protected:
    uint8_t SendBool(uint8_t command,uint16_t value);
    uint8_t SendInt(uint8_t command,uint16_t value);
    uint8_t SendChar(uint8_t command,uint8_t value);
    uint8_t SendDoubleInt(uint8_t command,uint16_t value1,uint16_t value2);
};
#endif //C_CLASS_MCU_H
