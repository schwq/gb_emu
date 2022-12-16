#ifndef CARTRIDGE_HEADER
#define CARTRIDGE_HEADER

#include "include.hpp"
#include "system.hpp"
#include "memory.hpp"

class CARTRIDGE : public SYSTEM {
public:
    
    CARTRIDGE(MEMORY& mem);
    ~CARTRIDGE();
    void cartridgeHeader();
    bool loadCartridge(const char* rom);

private:

    u32 CartridgeSize;
    MEMORY& memory;

};

#endif // CARTRIDGE_HEADER