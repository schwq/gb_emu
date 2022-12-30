#ifndef CPU_HEADER
#define CPU_HEADER

#include "../include.hpp"
#include "../system.hpp"
#include "../ram.hpp"
#include "instruction.hpp"

struct Registers{
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

class CPU : public SYSTEM {
public:

    void init(RAM &ram);
    void cycle(int num);
    void terminate();
    void execute();
    
private:

    Registers reg;
    RAM ram;

    int IME = 0;
    bool IME_scheduled = false;

    #ifdef DEBUG
    void debug(instruction inst);
    #endif

    u16 u8Tou16(u8 r1, u8 r2);
    u8 &lookupRegU8(operand opt);
    u16 &lookupRegU16(operand opt);

    u8 set_bit(u8 number, int bit);
    void bit_rotateLeft(u8& reg, bool carry);
    void bit_rotateRight(u8& reg, bool carry);
    void bit_swap(u8& reg);
    void bit_test(u8& reg, int bit);
    void bit_reset(u8& reg, int bit);
    void bit_rra(bool carry);
    void bit_sla(u8& reg);
    void bit_srl(u8& reg);
    void bit_sra(u8& reg);
    void bit_set(u8& reg, int bit);

    void bit_rotateLeft(u16& reg, bool carry);
    void bit_rotateRight(u16& reg, bool carry);
    void bit_swap(u16& reg);
    void bit_test(u16& reg, int bit);
    void bit_reset(u16& reg, int bit);
    void bit_sla(u16& reg);
    void bit_srl(u16& reg);
    void bit_sra(u16& reg);
    void bit_set(u16& reg, int bit);
    

    instruction instructionByOpcode(const u8 opcode);
    template<typename T> register_type checkResgisterType(T opt);

    void checkFlag(const char* flagSetup);
    void instructionExecute(const instruction inst);
    bool conditionalInstruction(const char* cc);
    void checkAffectedFlags(u16 result, const char* affectflags = "----", u16 carry = 1);
    int getFlag(FLAGS flag);
    void setFlag(bool value, FLAGS flag);
    void instructionCBexecute(const instruction inst, const u8 immediate);
    


};

#endif // CPU_HEADER