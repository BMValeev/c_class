//
// Created by eleps on 29.04.19.
//

#ifndef HARDWARECLASSES_BOARD_FHD_H
#define HARDWARECLASSES_BOARD_FHD_H


#include "../Rest/defs.h"
#include "i2cpacket.h"
#include <vector>
#include <map>

#define BOARD_FHD_MODULE_ADDRESS 0x70
#define BOARD_FHD_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER 3

class Board_fhd : public I2CPacket
{
public:
    Board_fhd(std::string filename, LogCallback cb = printToCout);

    // Commands
    uint8_t getStatus(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts = BOARD_FHD_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);
    uint8_t setDisabled(std::vector<uint8_t> data, std::vector<uint8_t> &response, int attempts = BOARD_FHD_MODULE_PACKET_TRANSACTION_ATTEMPTS_NUMBER);

    enum Cmd
    {
        GET_STATUS = 0x01,
        SET_DISABLED = 0x02,

    };

};

#endif //HARDWARECLASSES_BOARD_FHD_H
