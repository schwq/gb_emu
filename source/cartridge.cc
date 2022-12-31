#include "cartridge.hpp"


CARTRIDGE::CARTRIDGE(RAM& ReferenceRAM) : MainRAM(ReferenceRAM) {}
CARTRIDGE::~CARTRIDGE() { free(GameDataPointer); }

void CARTRIDGE::MakeCartridgeHeader() {
    
    // Entry Point vector 0x0100 - 0x0103
    for(int x = 0; x < 4; x++) {
        Header.HeaderEntry[x] = GameDataPointer[0x0100 + x];
        Header.HeaderManufacturerCode[x] = GameDataPointer[0x013F + x];
    }

    for(int x = 0; x < 16; x++) {
        Header.HeaderTitle[x] = GameDataPointer[0x0134 + x];
    }
    
    Header.HeaderCGBflag = GameDataPointer[0x0143];
    Header.HeaderLicensse[0] = GameDataPointer[0x0144];
    Header.HeaderLicensse[1] = GameDataPointer[0x0145];
    Header.HeaderSGBflag = GameDataPointer[0x0146];
    Header.HeaderCartridgeType = GameDataPointer[0x0147];
    Header.HeaderROMsize = GameDataPointer[0x0148];
    Header.HeaderRAMsize = GameDataPointer[0x0149];
    Header.HeaderDestinationCode = GameDataPointer[0x014A];
    Header.HeaderOldLicensse = GameDataPointer[0x014B];
    Header.HeaderVersionROM = GameDataPointer[0x014C];
    Header.HeaderChecksum = GameDataPointer[0x014D];

    unsigned char ChecksumCount = 0;
    for(u16 i = 0x0134; i <= 0x014C; i++) {
        ChecksumCount = ChecksumCount - GameDataPointer[i] - 1;
    }
    printf("[INFO]: Checksum : %2.2X (%s)\n", Header.HeaderChecksum, (ChecksumCount & 0xFF) ? "PASSED" : "FAILED");
    Header.HeadergGlobalChecksum = (GameDataPointer[0x014E] << 8) | GameDataPointer[0x014F];
}

bool CARTRIDGE::LoadCartridge(const char* GameROM) {

    FILE* GameFILE = fopen(GameROM, "rb");
    
    if(GameFILE == NULL) {
        std::cout << "[ERROR]: Cannot open ROM!" << std::endl;
        return false;
    }

    std::cout << "[INFO]: Opening file!" << std::endl;

    fseek(GameFILE, 0, SEEK_END);
    CartridgeSize = ftell(GameFILE);
    rewind(GameFILE);
    GameDataPointer = (u8 *) malloc(CartridgeSize);
    fread(GameDataPointer, CartridgeSize, 1, GameFILE);
    if(GameDataPointer == NULL) {
        std::cout << "[ERROR]: Cannot allocate memory for ROM file!" << std::endl;
        return false;
    }
    
    fclose(GameFILE);
    MakeCartridgeHeader();

    return true;
}