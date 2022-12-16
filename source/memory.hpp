#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include "include.hpp"
#include "system.hpp"

/*
0000	3FFF	16 KiB ROM bank 00	/ From cartridge, usually a fixed bank
4000	7FFF	16 KiB ROM Bank 01~NN /	From cartridge, switchable bank via mapper (if any)
8000	9FFF	8 KiB Video RAM (VRAM) /	In CGB mode, switchable bank 0/1
A000	BFFF	8 KiB External RAM /	From cartridge, switchable bank if any
C000	CFFF	4 KiB Work RAM (WRAM)	
D000	DFFF	4 KiB Work RAM (WRAM) /	In CGB mode, switchable bank 1~7
E000	FDFF	Mirror of C000~DDFF (ECHO RAM) /	Nintendo says use of this area is prohibited.
FE00	FE9F	Sprite attribute table (OAM)	
FEA0	FEFF	Not Usable /	Nintendo says use of this area is prohibited
FF00	FF7F	I/O Registers	
FF80	FFFE	High RAM (HRAM)	
FFFF	FFFF	Interrupt Enable register (IE)	
*/

class MEMORY : public SYSTEM{
public:

    u8 readMem(const u16 addr) noexcept;
    void writeMem(const u16 addr, u8 mem) noexcept; 

private:

    u8 memory[0xFFFF];

};

#endif // MEMORY_HEADER