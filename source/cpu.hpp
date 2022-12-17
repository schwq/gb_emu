#ifndef CPU_HEADER
#define CPU_HEADER

#include "include.hpp"

typedef struct {

    u8 data;

} u8Reg;

typedef struct : public u8Reg {

    u16 data;
    u8 HighBit;
    u8 LowBit;

} u16Reg;

class CPU {
public:

private:

    

};

#endif // CPU_HEADER