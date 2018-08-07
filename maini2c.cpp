#include "I2C.h"
void PrintToC(uint8_t status, string msg)
{
    cout<<status<<msg<<endl;
}
int main(void)
{
    std::string filename="/dev/i2c-2";
    cout<<"1"<<endl;
    ConnModule mcu(filename,PrintToC);
    cout<<"2"<<endl;
    std::vector<unsigned char> data;
    mcu.StartInit(data);
    cout<<"3\n";
    printf("%d\n",data.size());
    return 1;
}
