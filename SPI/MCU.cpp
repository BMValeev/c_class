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

MCU::MCU(std::string filename, LogCallback cb) : m_cb(cb), m_filename(filename)
{
    SPI::getInstance().begin(m_filename,m_cb);
}

uint8_t MCU::setStanby(uint8_t status, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return sendBool(SET_STANDBY, status, attempts);
}

uint8_t MCU::checkStatus(std::vector<uint8_t> &answer, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");

    std::vector<uint8_t> tx_msg;
    uint8_t rx_len = getRxCnt(REQUEST_STATUS);
    tx_msg.push_back(REQUEST_STATUS);

    return send(tx_msg, answer, rx_len, attempts);
}

uint8_t MCU::setSubprogram(uint8_t subprogramCut, uint8_t subprogramCoag, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_SUBPROGRAM, subprogramCut, subprogramCoag, attempts);
}

uint8_t MCU::setConnector(uint8_t connector, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_CONNECTOR, connector, attempts);
}

uint8_t MCU::setMaxVoltage(uint16_t cutVoltage, uint16_t coagVoltage, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_MAX_U, cutVoltage, coagVoltage, attempts);
}

uint8_t MCU::setPower(uint16_t cutPower, uint16_t coagPower, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_POWER, cutPower, coagPower, attempts);
}

uint8_t MCU::setMaxTime(uint8_t maxTime , int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_MAX_ACT_TIME, maxTime, attempts);
}

uint8_t MCU::setAutoStartDelay(uint8_t delayTime , int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_AUTOSTART_DELAY, delayTime, attempts);
}

uint8_t MCU::setAutoStopResistance(uint16_t resistance , int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint16(SET_AUTOSTOP_THRESHOLD, resistance, attempts);
}

uint8_t MCU::setIrrigation(uint8_t enabled , int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return sendBool(SET_IRRIGATION, enabled, attempts);
}

uint8_t MCU::setIrrigationDelay(uint8_t delay , int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send1Uint8(SET_IRRIGATION_DELAY, delay, attempts);
}

uint8_t MCU::setModFrequency(uint16_t frequencyCut, uint16_t frequencyCoag, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint16(SET_MOD_DUTY_CYCLE, frequencyCut, frequencyCoag, attempts);
}

uint8_t MCU::setModDutyCycle(uint8_t dutyCycleCut , uint8_t dutyCycleCoag, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_MOD_DUTY_CYCLE, dutyCycleCut, dutyCycleCoag, attempts);
}

uint8_t MCU::setCircuit(uint8_t circuitCut, uint8_t circuitCoag, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    return send2Uint8(SET_CIRCUIT, circuitCut, circuitCoag, attempts);
}

uint8_t MCU::renewAll(uint8_t connector, uint8_t routineCut, uint8_t routineCoag, uint16_t maxVoltageCut,
                      uint16_t maxVoltageCoag, uint16_t powerCut, uint16_t powerCoag, uint8_t maxTime,
                      uint8_t autostart, uint8_t autostartDelay, uint8_t autostop, uint16_t autostopResistance,
                      uint8_t irrigation, uint8_t irrigationDelay, uint16_t cutPWMFrequency,
                      uint16_t coagPWMFrequency, uint8_t dutyCycleCut, uint8_t dutyCycleCoag,
                      uint8_t filterCut, uint8_t filterCoag, uint8_t cutPedal, uint8_t coagPedal, int attempts)
{
    printLog(Info_log,static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> tx_msg, answer;
    uint8_t rx_len = getRxCnt(SET_ALL);
    // Fill the buffer
    tx_msg.push_back(SET_ALL); // command code
    tx_msg.push_back(connector);
    tx_msg.push_back(routineCut);
    tx_msg.push_back(routineCoag);
    tx_msg.push_back(maxVoltageCut & 0xFF); // lsb
    tx_msg.push_back(maxVoltageCut >> 8); // msb
    tx_msg.push_back(maxVoltageCoag & 0xFF);
    tx_msg.push_back(maxVoltageCoag >> 8);
    tx_msg.push_back(powerCut & 0xFF);
    tx_msg.push_back(powerCut >> 8);
    tx_msg.push_back(powerCoag & 0xFF);
    tx_msg.push_back(powerCoag >> 8);
    tx_msg.push_back(maxTime);
    tx_msg.push_back(autostart);
    tx_msg.push_back(autostartDelay);
    tx_msg.push_back(autostop);
    tx_msg.push_back(autostopResistance & 0xFF);
    tx_msg.push_back(autostopResistance >> 8);
    tx_msg.push_back(irrigation);
    tx_msg.push_back(irrigationDelay);
    tx_msg.push_back(cutPWMFrequency & 0xFF);
    tx_msg.push_back(cutPWMFrequency >> 8);
    tx_msg.push_back(coagPWMFrequency & 0xFF);
    tx_msg.push_back(coagPWMFrequency >> 8);
    tx_msg.push_back(dutyCycleCut);
    tx_msg.push_back(dutyCycleCoag);
    tx_msg.push_back(filterCut);
    tx_msg.push_back(filterCoag);
    tx_msg.push_back(cutPedal);
    tx_msg.push_back(coagPedal);

    return send(tx_msg, answer, rx_len, attempts);
}

void MCU::printLog(uint8_t status, std::string text)
{
    if (m_cb != nullptr)
    {
        m_cb(status,text);
    }
}

void MCU::printToCout(uint8_t status, std::string msg)
{
    std::cout << status << msg << std::endl;
}

uint8_t MCU::getRxCnt(uint8_t cmd)
{
    if (cmd == REQUEST_STATUS)
        return 5;

    return 3;
}

uint8_t MCU::sendBool(uint8_t command, bool value, int attempts)
{
    //PrintLog(Debug_log,
    //         static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> tx_msg, answer;
    uint8_t rx_len = getRxCnt(command);

    tx_msg.push_back(command);
    tx_msg.push_back(value? 0x01 : 0x00);

    return send(tx_msg, answer, rx_len, attempts);
}

uint8_t MCU::send1Uint16(uint8_t command, uint16_t value, int attempts)
{   
    //PrintLog(Debug_log,
    //         static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> tx_msg, answer;
    uint8_t rx_len = getRxCnt(command);

    tx_msg.push_back(command);
    tx_msg.push_back(value  & 0xff);
    tx_msg.push_back(value >> 8);

    return send(tx_msg, answer, rx_len, attempts);
}

uint8_t MCU::send1Uint8(uint8_t command, uint8_t value, int attempts)
{
    //PrintLog(Debug_log,
    //         static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> tx_msg, answer;
    uint8_t rx_len = getRxCnt(command);

    tx_msg.push_back(command);
    tx_msg.push_back(value);

    return send(tx_msg, answer, rx_len, attempts);
}

uint8_t MCU::send2Uint8(uint8_t command, uint8_t value1, uint8_t value2, int attempts)
{
    //PrintLog(Debug_log,
    //         static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> tx_msg, answer;
    uint8_t rx_len = getRxCnt(command);

    tx_msg.push_back(command);
    tx_msg.push_back(value1);
    tx_msg.push_back(value2);

    return send(tx_msg, answer, rx_len, attempts);
}

uint8_t MCU::send2Uint16(uint8_t command, uint16_t value1, uint16_t value2, int attempts)
{
    //PrintLog(Debug_log,
    //         static_cast<std::string>(__func__) + " started");
    std::vector<uint8_t> tx_msg, answer;
    uint8_t rx_len = getRxCnt(command);

    tx_msg.push_back(command);
    tx_msg.push_back(value1  & 0xff);
    tx_msg.push_back(value1 >> 8);
    tx_msg.push_back(value2  & 0xff);
    tx_msg.push_back(value2 >> 8);

    return send(tx_msg, answer, rx_len, attempts);
}

uint8_t MCU::send(std::vector<uint8_t> &tx_msg, std::vector<uint8_t> &rx_msg, uint8_t rx_len, int attempts)
{
    SPI& spi = SPI::getInstance();

    while(attempts--)
    {
        printLog(Debug_log,
                 static_cast<std::string>(__func__) + "Transactions left: " + std::to_string(attempts));

        // Send message
        if (spi.transaction(tx_msg,rx_len))
            continue;

        rx_msg = spi.recData();

        if (rx_msg.size() != rx_len-1)
            continue;

        rx_msg.erase(rx_msg.begin());

        if (rx_msg.front() != (ACK_SPI|EXEC_SPI))
        {
            if (attempts == 0)
            {
                if ( (rx_msg.front() & ACK_SPI) != ACK_SPI )
                {
                    return NACK; // not acknowledged
                }
                else
                {
                    return NOK_SPI; // not executed
                }
            } else {
                continue;
            }
        }

        // Acknowledged & executed
        rx_msg.erase(rx_msg.begin()); // Remove acknowledge byte
        printLog(Debug_log,
                 static_cast<std::string>(__func__) + " - success");
        return OK_SPI;
    }
    return TR_ERR_SPI;
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
#endif // QTAPP
