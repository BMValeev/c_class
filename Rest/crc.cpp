#include "crc.h"

unsigned char CRC::crc8(unsigned char *buffer, unsigned long len)
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

unsigned short CRC::crc16(unsigned char *buffer, unsigned long len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--)
    {
        crc ^= *buffer++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}
bool CRC::crcBool(unsigned char *buffer, unsigned int len)
{
    unsigned char crc = 0x82;
    unsigned int i;

    while (len--)
    {
        crc ^= *buffer++;
        for (i = 0; i < 8; i++)
            crc = (crc & 1)? (crc >> 1) ^ 0x8c : crc >> 1;
    }
    return (crc==0x00)?true:false;
}
