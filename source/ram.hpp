#ifndef RAM_HEADER
#define RAM_HEADER

#include "include.hpp"


class RAM {
public:
    u8 ReadU8Data(u16 Address);
    void WriteU8Data(u16 Address, u8 Data);
    u16 ReadU16Data(u16 Address);
    void WriteU16Data(u16 Address, u16 Data);

private:

    u8 EmulatorMemory[0xFFFF];

};

#endif // RAM_HEADER
