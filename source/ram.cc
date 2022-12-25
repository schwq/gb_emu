#include "ram.hpp"

u8 RAM::readU8(u16 addr) {
    return memory[addr];
}

void RAM::writeU8(u16 addr, u8 data) {
    memory[addr] = data;
}

u16 RAM::readU16(u16 addr) {
    return (u16) (memory[addr] << 8) | memory[addr + 1]; 
}

void RAM::writeU16(u16 addr, u16 data) {
    memory[addr] = (u8)((data & 0xFF00) >> 8);
    memory[addr + 1] = (u8)(data & 0x00FF);
}