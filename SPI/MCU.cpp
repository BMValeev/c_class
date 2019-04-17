//
// Created by eleps on 07.02.19.
//

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

uint8_t MCU::setStanby(uint8_t status, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return sendBool(SET_STANDBY, status, attempts);
}

uint8_t MCU::checkStatus(std::vector<uint8_t> &answer, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload;
    return send(REQUEST_STATUS, payload, answer, attempts);
}

uint8_t MCU::setConnector(uint8_t cutConnector, uint8_t coagConnector, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_CONNECTOR, cutConnector, coagConnector, attempts);
}

uint8_t MCU::setSubprogram(uint8_t subprogramCut, uint8_t subprogramCoag, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_SUBPROGRAM, subprogramCut, subprogramCoag, attempts);
}

uint8_t MCU::setMaxActTime(uint8_t maxTime , int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_MAX_ACT_TIME, maxTime, attempts);
}

uint8_t MCU::setIrrigation(uint8_t enabled , int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return sendBool(SET_IRRIGATION, enabled, attempts);
}

uint8_t MCU::setIrrigationDelay(uint8_t delay , int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_IRRIGATION_DELAY, delay, attempts);
}

uint8_t MCU::setMainLoopDelay(uint16_t delay_us, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint16(SET_MAIN_LOOP_DELAY, delay_us, attempts);
}

uint8_t MCU::writeCallibrCharacteristic(uint8_t connector,
                                        uint8_t circuit,
                                        uint8_t voltage,
                                        std::vector<uint16_t>& callibrData,
                                        int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");

    // Payload also contains connector, circuit, voltage and 2 bytes CRC16
    uint16_t payloadLen = static_cast<uint8_t>(callibrData.size()*2 + 5);
    std::vector<uint8_t> payload, answer;
    payload.reserve(payloadLen);
    // Fill in the payload
    payload.push_back(connector);
    payload.push_back(circuit);
    payload.push_back(voltage);
    serializeUint16toUint8(callibrData, payload);
    uint16_t crc16 = CRC::crc16(payload.data(),payloadLen - 2);
    payload.push_back((crc16 & 0xFF));        // Extract the LSB
    payload.push_back((crc16 & 0xFF00) >> 8); // Extract the MSB
    payload.resize(payloadLen,0); // padding with 0s

    return send(WRITE_CALLIBR_CHARACT, payload, answer, attempts);
}

uint8_t MCU::setPower(uint16_t cutPower, uint16_t coagPower, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_POWER, cutPower, coagPower, attempts);
}

uint8_t MCU::setMaxVoltage(uint16_t cutVoltage, uint16_t coagVoltage, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_MAX_U, cutVoltage, coagVoltage, attempts);
}

uint8_t MCU::setAutoStartDelay(uint8_t delayTime , int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_AUTOSTART_DELAY, delayTime, attempts);
}

uint8_t MCU::setAutoStopResistance(uint16_t resistance , int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint16(SET_AUTOSTOP_THRESHOLD, resistance, attempts);
}

uint8_t MCU::setModFrequency(uint16_t frequencyCut, uint16_t frequencyCoag, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_MOD_DUTY_CYCLE, frequencyCut, frequencyCoag, attempts);
}

uint8_t MCU::setModDutyCycle(uint8_t dutyCycleCut , uint8_t dutyCycleCoag, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_MOD_DUTY_CYCLE, dutyCycleCut, dutyCycleCoag, attempts);
}

uint8_t MCU::setCircuit(uint8_t circuitCut, uint8_t circuitCoag, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_CIRCUIT, circuitCut, circuitCoag, attempts);
}

uint8_t MCU::setADCBufferSize(uint16_t cutBufferSize, uint16_t coagBufferSize, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_ADC_BUFFER_SIZE, cutBufferSize, coagBufferSize, attempts);
}

uint8_t MCU::setLoadCharacteristic(bool isCut, std::vector<uint16_t> &loadCharData, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");

    // Payload also contains connector, circuit, voltage and 2 bytes CRC16
    uint16_t payloadLen = static_cast<uint16_t>(loadCharData.size()*2 + 3);
    std::vector<uint8_t> payload, answer;
    payload.reserve(payloadLen);
    // Fill in the payload
    payload.push_back(isCut);
    serializeUint16toUint8(loadCharData, payload);
    uint16_t crc16 = CRC::crc16(payload.data(),payloadLen - 2);
    payload.push_back((crc16 & 0xFF));        // Extract the LSB
    payload.push_back((crc16 & 0xFF00) >> 8); // Extract the MSB
    payload.resize(payloadLen,0); // padding with 0s

    return send(SET_LOAD_CHARACT, payload, answer, attempts);
}

uint8_t MCU::setStartVoltageLevel(uint8_t cutLevel, uint8_t coagLevel, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_START_VOLTAGE_LEVEL, cutLevel, coagLevel, attempts);
}

uint8_t MCU::setWorkVoltageLevel(uint8_t cutLevel, uint8_t coagLevel, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_WORK_VOLTAGE_LEVEL, cutLevel, coagLevel, attempts);
}

uint8_t MCU::setMaxCurrent(uint16_t cutCurrent, uint16_t coagCurrent, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_MAX_CURRENT, cutCurrent, coagCurrent, attempts);
}

uint8_t MCU::setWorkCurrent(uint16_t cutCurrent, uint16_t coagCurrent, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_WORK_CURRENT, cutCurrent, coagCurrent, attempts);
}

uint8_t MCU::setIrrigationCurrent(uint16_t cutCurrent, uint16_t coagCurrent, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_IRRIGATION_CURRENT, cutCurrent, coagCurrent, attempts);
}

uint8_t MCU::setMaxAttempts(uint8_t cutAttempts, uint8_t coagAttempts, int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_MAX_PLAZMA_ATTEMPTS, cutAttempts, coagAttempts, attempts);
}

uint8_t MCU::renewAll(std::vector<uint8_t>& payload, int attempts) const
{
    if (payload.size() != MCU_SETALL_BUFFER_SIZE) {
        printLog(Info_log,static_cast<std::string>(__func__) + " wrong payload size");
        return NOK_SPI;
    }

    std::vector<uint8_t> answer;
    return send(SET_ALL, payload, answer, attempts);
}

uint8_t MCU::renewAll(uint8_t cutConnector,
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
                      int attempts) const
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> payload;
    payload.reserve(49);
    // Fill in the payload
    // Common
    payload.push_back(maxActTime);                    // 0
    payload.push_back(irrigation);                    // 1
    payload.push_back(irrigationDelay);               // 2
    payload.push_back(mainLoopDelay & 0xFF);          // 3
    payload.push_back(mainLoopDelay >> 8);            // 4
    // Cut                                            //
    payload.push_back(cutConnector);                  // 5
    payload.push_back(cutSubprogram);                 // 6
    payload.push_back(cutPower & 0xFF);               // 7
    payload.push_back(cutPower >> 8);                 // 8
    payload.push_back(cutMaxVoltage & 0xFF);          // 9
    payload.push_back(cutMaxVoltage >> 8);            // 10
    payload.push_back(cutPWMFrequency & 0xFF);        // 11
    payload.push_back(cutPWMFrequency >> 8);          // 12
    payload.push_back(cutDutyCycle);                  // 13
    payload.push_back(cutCircuit);                    // 14
    payload.push_back(cutAdcBufferSize & 0xFF);       // 15
    payload.push_back(cutAdcBufferSize >> 8);         // 16
    payload.push_back(cutPlazmaStartLevel);           // 17
    payload.push_back(cutPlazmaWorkLevel);            // 18
    payload.push_back(cutPlazmaMaxCurrent & 0xFF);    // 19
    payload.push_back(cutPlazmaMaxCurrent >> 8);      // 20
    payload.push_back(cutPlazmaWorkCurrent & 0xFF);   // 21
    payload.push_back(cutPlazmaWorkCurrent >> 8);     // 22
    payload.push_back(cutPlazmaAttempts);             // 23
    payload.push_back(cutPlazmaIrrigCurrent & 0xFF);  // 24
    payload.push_back(cutPlazmaIrrigCurrent >> 8);    // 25
    // Coag                                           //
    payload.push_back(coagConnector);                 // 26
    payload.push_back(coagSubprogram);                // 27
    payload.push_back(coagPower & 0xFF);              // 28
    payload.push_back(coagPower >> 8);                // 29
    payload.push_back(coagMaxVoltage & 0xFF);         // 30
    payload.push_back(coagMaxVoltage >> 8);           // 31
    payload.push_back(coagPWMFrequency & 0xFF);       // 32
    payload.push_back(coagPWMFrequency >> 8);         // 33
    payload.push_back(coagDutyCycle);                 // 34
    payload.push_back(coagCircuit);                   // 35
    payload.push_back(coagAdcBufferSize & 0xFF);      // 36
    payload.push_back(coagAdcBufferSize >> 8);        // 37
    payload.push_back(coagPlazmaStartLevel);          // 38
    payload.push_back(coagPlazmaWorkLevel);           // 39
    payload.push_back(coagPlazmaMaxCurrent & 0xFF);   // 40
    payload.push_back(coagPlazmaMaxCurrent >> 8);     // 41
    payload.push_back(coagPlazmaWorkCurrent & 0xFF);  // 42
    payload.push_back(coagPlazmaWorkCurrent >> 8);    // 43
    payload.push_back(coagPlazmaAttempts);            // 44
    payload.push_back(coagPlazmaIrrigCurrent & 0xFF); // 45
    payload.push_back(coagPlazmaIrrigCurrent >> 8);   // 46
    // Only applied to coag
    payload.push_back(autostartDelay);                // 47
    payload.push_back(autostopResistance & 0xFF);     // 48
    payload.push_back(autostopResistance >> 8);       // 49

    return renewAll(payload,attempts);
}

uint8_t MCU::getRxCnt(uint8_t cmd) const
{
    if (cmd == REQUEST_STATUS)
        return 5;

    return 3;
}

void MCU::serializeUint16toUint8(const std::vector<uint16_t> &input, std::vector<uint8_t> &output) {
    // Little endian
    for(auto snumber : input) {
        output.push_back((snumber & 0xFF));        // Extract the LSB
        output.push_back((snumber & 0xFF00) >> 8); // Extract the MSB
    }
}

#ifdef C_CLASS_DEBUG
// Stand-alone compile
void printToC(uint8_t status, std::string msg)
{
    std::cout << status<< msg << std::endl;
}
int main(void)
{
    std::string filename = "/dev/spidev1.0";
    MCU mcu(filename,printToC);
    std::vector<uint8_t> data;
    uint8_t value = 1;
    data.push_back(value);
    cout << "Here works";
    mcu.setStanby(1);
    return 1;
}
#endif // C_CLASS_DEBUG
