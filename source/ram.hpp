#ifndef RAM_HEADER
#define RAM_HEADER

#include "include.hpp"

class RAM {
public:
    u8 readU8(u16 addr);
    void writeU8(u16 addr, u8 data);
    u16 readU16(u16 addr);
    void writeU16(u16 addr, u16 data);

};

#endif // RAM_HEADER
