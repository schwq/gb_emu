#include "include.hpp"
#include "cartridge.hpp"
#include "cpu/cpu.hpp"
#include "ram.hpp"
#include "LogHandle.hpp"

static int width = 1080;
static int height = 540;

int main(int argc, const char** argv) {

    u8* data;

    FILE* GameFile = fopen("roms/pokemon.gb", "rb");
    if(GameFile == NULL) { ErrorLogMessage("Cannot open ROM!"); }
    fseek(GameFile, 0, SEEK_END);
    long size = ftell(GameFile);
    rewind(GameFile);
    data = (u8 *) malloc(size);
    fread(data, size, 1, GameFile);
    if(data == NULL) { ErrorLogMessage("Cannot ", "open ROM! ", 1); }

    fclose(GameFile);
    RAM ram;
    for(int x = 0; x < size; x++) {
        ram.WriteU8Data(0x0100 + x, data[x]);
    }
    for(int x = 0; x < 2000; x++) {
        std::cout << ram.ReadU8Data(0x0100 + x) << ", ";
    }
    
    free(data);

    CPU cpu;
    cpu.InitializeCPU(ram);

    while(!cpu.CPUShutDown) {
        cpu.ExecuteCPU();
    }

    return 0;
}