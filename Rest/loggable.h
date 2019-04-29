#ifndef LOGGABLE_H
#define LOGGABLE_H

#include "defs.h"

typedef std::function<void(uint8_t, std::string)> LogCallback;

class Loggable
{
public:
    Loggable(LogCallback cb = printToCout) : mCb(cb) { }

    enum LogStatus
    {
        InfoLog = 1,
        DebugLog = 2,
        WarningLog = 3,
        CriticalLog = 4
    };

    void setLogCallback(LogCallback cb);

protected:
    void printLog(uint8_t status, std::string text) const;
    static void printToCout(uint8_t status, std::string msg);

private:
    LogCallback mCb;

};

#endif // LOGGABLE_H
