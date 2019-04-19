#include "loggable.h"

#include <iostream>

void Loggable::setLogCallback(LogCallback cb)
{
    mCb = cb;
    printLog(InfoLog, "Loggable --> log callback changed");
}

void Loggable::printLog(uint8_t status, std::string text) const
{
    if (mCb == nullptr)
        return;

    mCb(status, text);
}

void Loggable::printToCout(uint8_t status, std::string msg)
{
    std::cout << status << msg << std::endl;
}
