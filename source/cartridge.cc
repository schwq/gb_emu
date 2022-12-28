#include "cartridge.hpp"
#include "system.hpp"

CARTRIDGE::CARTRIDGE(RAM& ram) : ram(this->ram) {}
CARTRIDGE::~CARTRIDGE() { free(data); }

void CARTRIDGE::cartridgeHeader() {
    
    // Entry Point vector 0x0100 - 0x0103
    for(int x = 0; x < 4; x++) {
        header.entry[x] = data[0x0100 + x];
    }

    for(int x = 0; x < 16; x++) {
        header.title[x] = data[0x0134 + x];
    }

    for(int x = 0; x < 4; x++) {
        header.manufacturerCode[x] = data[0x013F + x];
    }

    header.CGBflag = data[0x0143];
    header.licensse[0] = data[0x0144];
    header.licensse[1] = data[0x0145];
    header.SGBflag = data[0x0146];
    header.cartridgeType = data[0x0147];
    header.ROMsize = data[0x0148];
    header.RAMsize = data[0x0149];
    header.destinationCode = data[0x014A];
    header.oldLicensse = data[0x014B];
    header.versionROM = data[0x014C];
    header.headerChecksum = data[0x014D];

    unsigned char sum = 0;
    for(u16 i = 0x0134; i <= 0x014C; i++) {
        sum = sum - data[i] - 1;
    }
    printf("[INFO]: Checksum : %2.2X (%s)\n", header.headerChecksum, (sum & 0xFF) ? "PASSED" : "FAILED");

    u8 holder01 = data[0x014E];
    u8 holder02 = data[0x014F];
    header.globalChecksum = (holder01 << 8) | holder02;
}

bool CARTRIDGE::loadCartridge(const char* rom) {

    FILE* fp = fopen(rom, "rb");
    
    if(fp == NULL) {
        std::cout << "[ERROR]: Cannot open ROM!" << std::endl;
        return false;
    }

    std::cout << "[INFO]: Opening file!" << std::endl;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    CartridgeSize = ftell(fp);
    rewind(fp);
    data = (u8 *) malloc(CartridgeSize);
    fread(data, CartridgeSize, 1, fp);
    if(data == NULL) {
        std::cout << "[ERROR]: Cannot allocate memory for ROM file!" << std::endl;
        return false;
    }
    
    fclose(fp);

    std::cout << "[INFO]: ROM transferred to buffer with success!" << std::endl;

    std::cout << "Done" << NEWLINE;

    cartridgeHeader();

    return true;
}