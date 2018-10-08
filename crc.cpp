#include "crc.h"

unsigned char CRC::crc8(unsigned char *buffer, unsigned int len)
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
