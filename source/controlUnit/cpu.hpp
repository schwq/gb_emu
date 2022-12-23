#ifndef CPU_HEADER
#define CPU_HEADER

#include "include.hpp"
#include "system.hpp"
#include "ram.hpp"
#include "instruction.hpp"

enum FLAGS;

class CPU : public SYSTEM {
public:
    void init();
    void cycle(int num);
    void terminate();
    void setFlag(bool value, FLAGS flag);
    void execute();
    
    // Instruction setup functions
    u16 u8Tou16(u8 r1, u8 r2);
    u8 &lookupRegU8(operand opt);
    u16 &lookupRegU16(operand opt);
    void checkFlag(const char* flagSetup);
    void instructionExecute(instruction inst);
    bool conditionalInstruction(const char* cc);
    instruction instructionOpcode(u8 opcode);
    template<typename T>
    register_type checkResgisterType(T opt);
    void checkAffectedFlags(auto result, const char* affectflags = "----", u16 carry = 1);
    int getFlag(FLAGS flag);

    Registers reg;
    RAM ram;
private:
    const struct instruction instructions[0x100] = {
    [0x00] = {IN_NOP, NONE, NONE, "----", 1, AM_NONE},
    [0x02] = {IN_LD, reg_bc, reg_a, "----", 1,  AM_MR_R},
    
};

};

struct Registers {
    struct {
        union {
            struct {
                u8 A;
                u8 F;
            };
        };
        u16 AF;
    };
    struct {
        union {
            struct {
                u8 C;
                u8 B;
            };
        };
        u16 BC;
    };
    struct {
        union {
            struct {
                u8 E;
                u8 D;
            };
        };
        u16 DE;
    };
    struct {
        union {
            struct {
                u8 H;
                u8 L;
            };
        };
        u16 HL;
    };

    u16 SP;
    u16 PC;

};

#endif // CPU_HEADER