#ifndef CPU_HEADER
#define CPU_HEADER

#include "../include.hpp"
#include "../ram.hpp"
#include "instruction.hpp"

class CPU  {
public:
    // Calls to initialize registers default values and reference RAM
    void InitializeCPU(RAM &ReferenceRAM);
    // Emulate cycle 
    void CycleCPU(int Cycles);
    // Terminate some processes and make sure to free()
    void TerminateCPU();
    // Fetch and execute instruction
    void ExecuteCPU();
    // Sets to true when a critical error have been thrown, sets all operations off and shutdown CPU
    bool CPUShutDown = false;
    
private:
    // Basic set of registers, make of u8 and u16 
    Registers RegisterSet;
    // Reference for RAM
    RAM MainRAM;

    // Enable interrupts
    int IME = 0;
    // Scheduled interrupts
    bool IMEScheduled = false;

    // Just in case of development, used to cout registers, flags, instruction, ect
    #ifdef DEBUG
    void DebugCPU(Instruction InstructionExecuted);
    #endif

    // Returns const char* of Instruction Mnemonic enum type
    const char* DebugLookUpMnemonic(EMnemonic MnemonicToLookUp);
    // Returns const char* of Instruction Operand enum type
    const char* DebugLookUpOperand(EOperand OperandToLookUp);
    // Returns const char* of Instruction AddressMode enum type 
    const char* DebugLookUpAddressMode(EAddressMode AddressModeToLookUp);

    // Convert two u8 numbers and return one u16
    u16 U8ToU16Number(u8 LowByte, u8 HighByte);
    // Takes EOperand and search for respective 8 bit registers associated
    u8 &LookUpRegisterU8(EOperand OperandToLookUp);
    // Takes EOperand and search for respective 16 bit registers associated
    u16 &LookUpRegisterU16(EOperand OperandToLookUp);

    // Rotate all bits to the left
    void BitOperationRL(u8& Register, bool Carry);
    void BitOperationRL(u16 Register, bool Carry);
    // Rotate all bits to the right
    void BitOperationRR(u8& Register, bool Carry);
    void BitOperationRR(u16 Register, bool Carry);
    // Swap the lest significate bits and the most significate bits of a register
    void BitOperationSwap(u8& Register);
    void BitOperationSwap(u16 Register);
    // Test a bit for value, if its 1, then ZERO flag is set, else nothing happens
    void BitOperationTest(u8& Register, int Bit);
     void BitOperationTest(u16 Register, int Bit);
    // Reset a bit to 0 
    void BitOperationReset(u8& Register, int Bit);
    void BitOperationReset(u16 Register, int Bit);
    // Rotate A right through carry
    void BitOperationRRA(bool Carry);
    // Shift left arithmetic (b0=0)
    void BitOperationSLA(u8& Register);
    void BitOperationSLA(u16 Register);
    // Shift right logical (b7=0)
    void BitOperationSRL(u8& Register);
    void BitOperationSRL(u16 Register);
    // Shift right arithmetic (b7=b7)
    void BitOperationSRA(u8& Register);
    void BitOperationSRA(u16 Register);
    // Sets a bit to 1
    void BitOperationSet(u8& Register, int Bit);
    void BitOperationSet(u16 Register, int Bit);
    
    // Searchs for respective instruction in Insctuctions list by opcode
    Instruction SearchInstructionByOpcode(const u8 SearchOpcode);
    // Returns register type (8 bit or 16 bit) 
    template<typename T> ERegisterType CheckRegisterType(const T RegisterToCheck);

    // Execute instruction find by the SearchInstructionByOpcode (Mega switch statement)
    void ExecuteInstruction(const Instruction InstructionToExecute);
    // Takes a const char* with a condition writeen and return if all of them are set
    bool CheckConditionForInstruction(const char* Condition);
    // Takes the instruction operation result (for zero flag check), the Instruction AffectFlags, and the Carry 16 bit variable
    void CheckAffectedFlags(u16 OperationResult, const char* AffectFlags = "----", u16 Carry = 1);
    // Returns if flag is set or not
    int GetFlagInteger(EFlags FlagToGet);
    // Sets flag to value
    void SetFlag(bool Value, EFlags FlagToSetValue);
    // Instruction prefixed "CB" have this special function been called, taking the immediate data
    void CBPrefixInstructionExecute(const u8 CBInstructionToExecute);
};

#endif // CPU_HEADER