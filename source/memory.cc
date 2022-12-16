#include "memory.hpp"

u8 MEMORY::readMem(const u16 addr) noexcept {
    return memory[addr];
}

void MEMORY::writeMem(const u16 addr, u8 mem) noexcept {
    memory[addr] = mem;
}