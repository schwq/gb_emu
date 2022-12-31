#ifndef CARTRIDGE_HEADER
#define CARTRIDGE_HEADER

#include "include.hpp"
#include "ram.hpp"

class CartridgeHeader {
public:
    u8 HeaderEntry[4];
    //u8 logo[0x30];
    char HeaderTitle[16];

    char HeaderManufacturerCode[4];
    u8 HeaderCGBflag;
    u8 HeaderLicensse[2];
    u8 HeaderSGBflag;
    u8 HeaderCartridgeType;
    u8 HeaderROMsize;
    u8 HeaderRAMsize;
    u8 HeaderDestinationCode;
    u8 HeaderOldLicensse;
    u8 HeaderVersionROM;
    u8 HeaderChecksum;
    u16 HeadergGlobalChecksum;
};

class CARTRIDGE  {
public:
    
    CARTRIDGE(RAM& ReferenceRAM);
    ~CARTRIDGE();
    bool LoadCartridge(const char* GameROM);

private:
    void MakeCartridgeHeader();

    CartridgeHeader Header;
    u32 CartridgeSize;
    u8 *GameDataPointer;
    RAM& MainRAM;
};

#endif // CARTRIDGE_HEADER