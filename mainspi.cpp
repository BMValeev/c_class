int main(void)
{
    std::string filename="/dev/spidev1.0";
    MCU ConnModule(filename,NULL);

    unsigned char value= 1;
    data.push_back( value);
    cout<<"Here works0";
    mcu.SetStanby(1);
    mcu.CheckStatus(received);
    return 1;
}
