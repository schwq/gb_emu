#ifndef CPU_HEADER
#define CPU_HEADER

#include "../include.hpp"
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

class CPU  {
public:

    void InitializeCPU(RAM &ReferenceRAM);
    void CycleCPU(int Cycles);
    void TerminateCPU();
    void ExecuteCPU();
    
private:

    Registers RegisterSet;
    RAM MainRAM;

    int IME = 0;
    bool IMEScheduled = false;

    #ifdef DEBUG
    void DebugCPU(instruction InstructionExecuted);
    #endif

    u16 U8ToU16Number(u8 LowByte, u8 HighByte);
    u8 &LookUpRegisterU8(operand OperandToLookUp);
    u16 &LookUpRegisterU16(operand OperandToLookUp);

    void BitOperationRL(u8& Register, bool Carry);
    void BitOperationRR(u8& Register, bool Carry);
    void BitOperationSwap(u8& Register);
    void BitOperationTest(u8& Register, int Bit);
    void BitOperationReset(u8& Register, int Bit);
    void BitOperationRRA(bool Carry);
    void BitOperationSLA(u8& Register);
    void BitOperationSRL(u8& Register);
    void BitOperationSRA(u8& Register);
    void BitOperationSET(u8& Register, int Bit);

    void BitOperationRL(u16 Register, bool Carry);
    void BitOperationRR(u16 Register, bool Carry);
    void BitOperationSwap(u16 Register);
    void BitOperationTest(u16 Register, int Bit);
    void BitOperationReset(u16 Register, int Bit);
    void BitOperationSLA(u16 Register);
    void BitOperationSRL(u16 Register);
    void BitOperationSRA(u16 Register);
    void BitOperationSet(u16 Register, int Bit);
    
    instruction SearchInstructionByOpcode(const u8 SearchOpcode);
    template<typename T> register_type CheckRegisterType(T RegisterToCheck);

    void CheckFlagSetup(const char* FlagSetup);
    void ExecuteInstruction(const instruction InstructionToExecute);
    bool CheckConditionForInstruction(const char* Condition);
    void CheckAffectedFlags(u16 OperationResult, const char* AffectFlags = "----", u16 Carry = 1);
    int GetFlagInteger(FLAGS FlagToGet);
    void SetFlag(bool Value, FLAGS FlagToSetValue);
    void CBPrefixInstructionExecute(const u8 CBInstructionToExecute);
};

#endif // CPU_HEADER