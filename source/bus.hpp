#ifndef BUS_HEADER
#define BUS_HEADER
#include "include.hpp"

class BUS {
public:
    BUS();
    ~BUS();

    void writeBus(u16 addr, u8 data);
    u8 readBus(u16 addr);

    void U16writeBus(u16 addr, u16 data);
    u16 U16readBus(u16 addr);

};


#endif // BUS_HEADER