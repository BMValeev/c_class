#ifndef DEFS_H
#define DEFS_H

#include <functional>

#ifdef C_CLASS_DEBUG
    // Stand-alone compile
#else
    // Within QT app compile
#endif // C_CLASS_DEBUG

void sleep(int ms);

#endif // DEFS_H
