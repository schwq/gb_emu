#include "ram.hpp"

u8 RAM::ReadU8Data(u16 Address) {
    return EmulatorMemory[Address];
}

void RAM::WriteU8Data(u16 Address, u8 Data) {
    EmulatorMemory[Address] = Data;
}

u16 RAM::ReadU16Data(u16 Address) {
    return (u16) (EmulatorMemory[Address] << 8) | EmulatorMemory[Address + 1]; 
}

void RAM::WriteU16Data(u16 Address, u16 Data) {
    EmulatorMemory[Address] = (u8)((Data & 0xFF00) >> 8);
    EmulatorMemory[Address + 1] = (u8)(Data & 0x00FF);
}