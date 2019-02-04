#ifndef DEFS_H
#define DEFS_H

#ifdef C_CLASS_DEBUG
    // Stand-alone compile
#else
    // Within QT app compile
#endif // C_CLASS_DEBUG

#include <functional>
typedef std::function<void(uint8_t, std::string)> LogCallback;

enum Log_status { Info_log = 1, Debug_log=2, Warning_log= 3, Critical_log=4 };

#endif // DEFS_H
