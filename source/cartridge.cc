#include "cartridge.hpp"

CARTRIDGE::CARTRIDGE(MEMORY& mem) : memory(mem) {}

void CARTRIDGE::cartridgeHeader() {
    
}

bool CARTRIDGE::loadCartridge(const char* rom) {

    FILE* fp = fopen(rom, "rb");
    
    if(fp == NULL) {
        LogError("[ERROR]: Cannot load ROM file!");
        return false;
    }

    std::cout << "Opening rom" << std::endl;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char* buffer = (char*)malloc(sizeof(char) * size);
    
    if(buffer == NULL) {
        LogError("[ERROR]: Cannot allocate memory for ROM file!");
        return false;
    }

    size_t result = fread(buffer, sizeof(char), (size_t)size, fp);

    if(result != size) {
        LogError("[ERROR]: Cannot read ROM!");
        return false;
    }

    fclose(fp);
    free(buffer);
}