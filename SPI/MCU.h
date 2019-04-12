//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_MCU_H
#define C_CLASS_MCU_H

#include "SPI.h"

#define MCU_TRANSACTION_ATTEMPTS_NUMBER 3

class MCU
{
public:
    MCU(std::string filename, LogCallback cb = printToCout);
    ~MCU() {}

    enum Programs
    {
        OFF = 0x00,
        TERMLC,
        TERMU,
        AUTOSTOP,
        AUTOSTART,
        PLAZMA,
        TEST
    };

    enum Cmd
    {
        // Common parameters
        SET_STANDBY            = 0x01,
        REQUEST_STATUS         = 0x02,
        SET_CONNECTOR          = 0x03,
        SET_SUBPROGRAM         = 0x04,
        SET_ALL                = 0x05,
        SET_MAX_ACT_TIME       = 0x06,
        SET_IRRIGATION         = 0x07,
        SET_IRRIGATION_DELAY   = 0x08,
        SET_MAIN_LOOP_DELAY    = 0x09,
        WRITE_CALLIBR_CHARACT  = 0x0A,
        // Termic modes parameters
        SET_POWER              = 0x0B,
        SET_MAX_U              = 0x0C,
        SET_AUTOSTART_DELAY    = 0x0D,
        SET_AUTOSTOP_THRESHOLD = 0x0E,
        SET_MOD_FREQ           = 0x0F,
        SET_MOD_DUTY_CYCLE     = 0x10,
        SET_CIRCUIT            = 0x11,
        SET_ADC_BUFFER_SIZE    = 0x12,
        SET_LOAD_CHARACT       = 0x13,
        // Plazma modes parameters
        SET_START_VOLTAGE_LEVEL = 0x14,
        SET_WORK_VOLTAGE_LEVEL  = 0x15,
        SET_MAX_CURRENT         = 0x16,
        SET_WORK_CURRENT        = 0x17,
        SET_MAX_PLAZMA_ATTEMPTS = 0x18,
        SET_IRRIGATION_CURRENT  = 0x19
    };


    // Common
    uint8_t setStanby(uint8_t status, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t checkStatus(std::vector<uint8_t> &answer, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);

    void renewAll( );
    uint8_t setSubprogram(uint8_t subprogramCut, uint8_t subprogramCoag, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setConnector(uint8_t  connector, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setMaxVoltage(uint16_t cutVoltage, uint16_t coagVoltage, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setPower(uint16_t cutPower, uint16_t coagPower, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setMaxTime(uint8_t maxTime, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setAutoStartDelay(uint8_t delayTime, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setAutoStopResistance(uint16_t resistance, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setIrrigation(uint8_t enabled, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setIrrigationDelay(uint8_t delay, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setModFrequency(uint16_t frequencyCut, uint16_t frequencyCoag, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setModDutyCycle(uint8_t dutyCycleCut , uint8_t dutyCycleCoag, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setCircuit(uint8_t  circuitCut, uint8_t  circuitCoag, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t renewAll(uint8_t connector, uint8_t routineCut, uint8_t routineCoag, uint16_t maxVoltageCut, uint16_t maxVoltageCoag,
                     uint16_t powerCut, uint16_t powerCoag, uint8_t maxTime, uint8_t autostart, uint8_t autostartDelay, uint8_t autostop,
                     uint16_t autostopResistance, uint8_t irrigation, uint8_t irrigationDelay, uint16_t cutPWMFrequency, uint16_t coagPWMFrequency,
                     uint8_t dutyCycleCut, uint8_t dutyCycleCoag, uint8_t filterCut, uint8_t filterCoag, uint8_t cutPedal, uint8_t coagPedal,
                     int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);


private:
    void printLog(uint8_t status, std::string text);
    static void printToCout(uint8_t status, std::string msg);
    const LogCallback m_cb;
    const std::string m_filename;

    // Helpers
    static uint8_t getRxCnt(uint8_t cmd);
    uint8_t sendBool(uint8_t command, bool value, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t send1Uint8(uint8_t command, uint8_t value, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t send2Uint8(uint8_t command, uint8_t value1, uint8_t value2, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t send1Uint16(uint8_t command, uint16_t value, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t send2Uint16(uint8_t command, uint16_t value1, uint16_t value2, int attempts = MCU_TRANSACTION_ATTEMPTS_NUMBER);

protected:
    // SPI is only used in this function, so possible to reimplement over other protocol
    uint8_t send(std::vector<uint8_t>& tx_msg, std::vector<uint8_t>& rx_msg, uint8_t rx_len, int attempts);

};

#endif //C_CLASS_MCU_H
