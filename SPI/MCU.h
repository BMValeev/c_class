//
// Created by eleps on 07.02.19.
//

#ifndef C_CLASS_MCU_H
#define C_CLASS_MCU_H

#include "SPI.h"
#include "spipacket.h"

#define MCU_SETALL_BUFFER_SIZE 50

// This is a leaner class that contains commands and command definitions for ESU.
// It inherits SPIPacket class and thus all the basic logic is contained there,
// So the essense of the data interpretation is here, while logic is in the SPIPacket
class MCU : private SPIPacket
{
public:
    MCU(std::string filename, LogCallback cb = printToCout) : SPIPacket(filename, cb) { }

    // Common
    uint8_t setStanby(uint8_t status, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t checkStatus(std::vector<uint8_t> &answer, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setConnector(uint8_t cutConnector, uint8_t coagConnector, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setSubprogram(uint8_t subprogramCut, uint8_t subprogramCoag, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setMaxActTime(uint8_t maxTime, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setIrrigation(uint8_t enabled, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setIrrigationDelay(uint8_t delay, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setMainLoopDelay(uint16_t delay_us, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t writeCallibrCharacteristic(uint8_t  connector, uint8_t  circuit, uint8_t voltage, std::vector<uint16_t>& callibrData, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    // Termic modes
    uint8_t setPower(uint16_t cutPower, uint16_t coagPower, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setMaxVoltage(uint16_t cutVoltage, uint16_t coagVoltage, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setAutoStartDelay(uint8_t delayTime, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setAutoStopResistance(uint16_t resistance, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setModFrequency(uint16_t frequencyCut, uint16_t frequencyCoag, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setModDutyCycle(uint8_t dutyCycleCut , uint8_t dutyCycleCoag, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setCircuit(uint8_t  circuitCut, uint8_t  circuitCoag, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setADCBufferSize(uint16_t cutBufferSize, uint16_t coagBufferSize, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setLoadCharacteristic(bool isCut, std::vector<uint16_t>& loadCharData, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    // Plazma modes
    uint8_t setStartVoltageLevel(uint8_t cutLevel , uint8_t coagLevel, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setWorkVoltageLevel(uint8_t cutLevel , uint8_t coagLevel, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setMaxCurrent(uint16_t cutCurrent, uint16_t coagCurrent, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setWorkCurrent(uint16_t cutCurrent, uint16_t coagCurrent, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setIrrigationCurrent(uint16_t cutCurrent, uint16_t coagCurrent, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t setMaxAttempts(uint8_t cutAttempts , uint8_t coagAttempts, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    // Set all command
    uint8_t renewAll(std::vector<uint8_t> &payload, int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;
    uint8_t renewAll(uint8_t cutConnector,
                     uint8_t coagConnector,
                     uint8_t cutSubprogram,
                     uint8_t coagSubprogram,
                     uint8_t maxActTime,
                     bool irrigation,
                     uint8_t irrigationDelay,
                     uint16_t mainLoopDelay,
                     uint16_t cutPower,
                     uint16_t coagPower,
                     uint16_t cutMaxVoltage,
                     uint16_t coagMaxVoltage,
                     uint8_t autostartDelay,
                     uint16_t autostopResistance,
                     uint16_t cutPWMFrequency,
                     uint16_t coagPWMFrequency,
                     uint8_t cutDutyCycle,
                     uint8_t coagDutyCycle,
                     uint8_t cutCircuit,
                     uint8_t coagCircuit,
                     uint16_t cutAdcBufferSize,
                     uint16_t coagAdcBufferSize,
                     uint8_t cutPlazmaStartLevel,
                     uint8_t coagPlazmaStartLevel,
                     uint8_t cutPlazmaWorkLevel,
                     uint8_t coagPlazmaWorkLevel,
                     uint16_t cutPlazmaMaxCurrent,
                     uint16_t coagPlazmaMaxCurrent,
                     uint16_t cutPlazmaWorkCurrent,
                     uint16_t coagPlazmaWorkCurrent,
                     uint8_t cutPlazmaAttempts,
                     uint8_t coagPlazmaAttempts,
                     uint16_t cutPlazmaIrrigCurrent,
                     uint16_t coagPlazmaIrrigCurrent,
                     int attempts = SPI_PACKET_TRANSACTION_ATTEMPTS_NUMBER) const;

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

private:
    // Helpers
    virtual uint8_t getRxCnt(uint8_t cmd) const override;
    static void serializeUint16toUint8(const std::vector<uint16_t>& input, std::vector<uint8_t>& output);

};

#endif //C_CLASS_MCU_H
