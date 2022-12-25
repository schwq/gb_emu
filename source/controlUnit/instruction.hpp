#ifndef INSTRUCTION_HEADER
#define INSTRUCTION_HEADER

#include "../include.hpp"

enum FLAGS {
    ZERO,
    ADDSUB,
    HALF,
    CARRY
};

enum mnemonic {
    IN_LD,
    IN_LDH,
    IN_PUSH,
    IN_POP,
    IN_ADD,
    IN_ADC,
    IN_SUB,
    IN_SBC,
    IN_CP,
    IN_INC,
    IN_DEC,
    IN_AND,
    IN_OR,
    IN_XOR,
    IN_CCF,
    IN_SCF,
    IN_DAA,
    IN_CPL,
    IN_JP,
    IN_JR,
    IN_CALL,
    IN_RET,
    IN_RETI,
    IN_RST,
    IN_HALT,
    IN_STOP,
    IN_DI,
    IN_EI,
    IN_NOP,
    IN_RRA,
    IN_RLA,
    IN_CB,
    IN_RRCA,
    IN_RLCA,
    IN_NONE
};

enum addr_mode {
    AM_IMP,
    AM_R_D16,
    AM_R_R,
    AM_MR_R,
    AM_R,
    AM_R_D8,
    AM_R_MR,
    AM_R_HLI,
    AM_R_HLD,
    AM_HLI_R,
    AM_HLD_R,
    AM_R_A8,
    AM_A8_R,
    AM_HL_SPR,
    AM_D16,
    AM_D8,
    AM_D16_R,
    AM_MR_D8,
    AM_MR,
    AM_A16_R,
    AM_R_A16,
    AM_NONE
};

enum operand {
    reg_a,
    reg_b, 
    reg_c,
    reg_d,
    reg_e,
    reg_h,
    reg_l,
    reg_bc,
    reg_hl,
    reg_sp,
    reg_de,
    reg_af,
    immediate_u8,
    immediate_u16,
    NONE
};


enum register_type {
    U8REG,
    U16REG,
    NONE_REG
};

struct instruction {
    mnemonic MNC;
    addr_mode addr;
    operand op01;
    operand op02;
    const char* affectFlags;
    int cycles;
    u8 rst_opcode;
    
};

#endif // INSTRUCTION_HEADER