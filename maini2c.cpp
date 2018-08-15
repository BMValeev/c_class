#include "I2C.h"
void PrintToC(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
int main(void)
{
    std::string filename="/dev/i2c-2";
    std::vector<unsigned char> responce;
    BoardModule mcu(filename,PrintToC);
    std::vector<unsigned char> data;
    mcu.SetVolume(0x01,responce);
    mcu.SetEnergy(0x01,responce);
    mcu.GetPower(responce);
    mcu.GetTools(responce);
    mcu.GetVersion(responce);

    printf("%d\n",responce.size());
    ConnModule mcu1(filename,PrintToC);
    mcu1.StartInit(responce);
    printf("%d\n",responce.size());
    return 1;
}
