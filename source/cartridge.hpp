#ifndef CARTRIDGE_HEADER
#define CARTRIDGE_HEADER

#include "include.hpp"
#include "system.hpp"
#include "ram.hpp"

class CartridgeHeader {
public:
    u8 entry[4];
    //u8 logo[0x30];
    char title[16];

    char manufacturerCode[4];
    u8 CGBflag;
    u8 licensse[2];
    u8 SGBflag;
    u8 cartridgeType;
    u8 ROMsize;
    u8 RAMsize;
    u8 destinationCode;
    u8 oldLicensse;
    u8 versionROM;
    u8 headerChecksum;
    u16 globalChecksum;
};

class CARTRIDGE : public SYSTEM {
public:
    
    CARTRIDGE(RAM& ram);
    ~CARTRIDGE();
    bool loadCartridge(const char* rom);

private:
    void cartridgeHeader();

    CartridgeHeader header;
    u32 CartridgeSize;
    u8 *data;
    RAM& ram;
};

#endif // CARTRIDGE_HEADER