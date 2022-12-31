#ifndef INSTRUCTION_HEADER
#define INSTRUCTION_HEADER

#include "../include.hpp"

enum FLAGS {
    ZERO,
    ADDSUB,
    HALF,
    CARRY
};

static const char* DebugLookUpFlag(FLAGS FlagToLookUp) {
    switch(FlagToLookUp) {
        case ZERO: return "ZERO";
        case ADDSUB: return "ADDSUB";
        case HALF: return "HALF";
        case CARRY: return "CARRY";
        default: return "[WARNING]: DebugLookUpFlag() cannot find flag to return const char*!";
    }
}

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

const char* DebugLookUpMnemonic(mnemonic MnemonicToLookUp) {
    switch(MnemonicToLookUp) {
        case IN_LD: return "Instruction LD";
        case IN_LDH: return "Instruction LDH";
        case IN_PUSH: return "Instruction PUSH";
        case IN_POP: return "Instruction POP";
        case IN_ADD: return "Instruction ADD";
        case IN_ADC: return "Instruction ADC";
        case IN_SUB: return "Instruction SUB";
        case IN_SBC: return "Instruction SBC";
        case IN_CP: return "Instruction CP";
        case IN_INC: return "Instruction INC";
        case IN_DEC: return "Instruction DEC";
        case IN_AND: return "Instruction AND";
        case IN_OR: return "Instruction OR";
        case IN_XOR: return "Instruction XOR";
        case IN_CCF: return "Instruction CCF";
        case IN_SCF: return "Instruction SCF";
        case IN_DAA: return "Instruction DAA";
        case IN_CPL: return "Instruction CPL";
        case IN_JP: return "Instruction JP";
        case IN_JR: return "Instruction JR";
        case IN_CALL: return "Instruction CALL";
        case IN_RET: return "Instruction RET";
        case IN_RETI: return "Instruction RETI";
        case IN_RST: return "Instruction RST";
        case IN_HALT: return "Instruction HALT";
        case IN_STOP: return "Instruction STOP";
        case IN_DI: return "Instruction DI";
        case IN_EI: return "Instruction EI";
        case IN_NOP: return "Instruction NOP";
        case IN_RRA: return "Instruction RRA";
        case IN_RLA: return "Instruction RLA";
        case IN_CB: return "Instruction CB";
        case IN_RRCA: return "Instruction RRCA";
        case IN_RLCA: return "Instruction RLCA";
        case IN_NONE: return "Instruction NONE";
        default: return "[WARNING]: DebugLookUpMnemonic() cannot find Mnemonic to return const char*!";
    }
}

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

const char* DebugLookUpAddressMode(addr_mode AddressModeToLookUp) {
    switch(AddressModeToLookUp) {
        case AM_IMP: return "Address Mode IMP";
        case AM_R_D16: return "Address Mode R_D16";
        case AM_R_R: return "Address Mode R_R";
        case AM_MR_R: return "Address Mode MR_R";
        case AM_R: return "Address Mode R";
        case AM_R_D8: return "Address Mode R_D8";
        case AM_R_MR: return "Address Mode R_MR";
        case AM_R_HLI: return "Address Mode R_HLI";
        case AM_R_HLD: return "Address Mode R_HLD";
        case AM_HLI_R: return "Address Mode HLI_R";
        case AM_HLD_R: return "Address Mode HLD_R";
        case AM_R_A8: return "Address Mode R_A8";
        case AM_A8_R: return "Address Mode A8_R";
        case AM_HL_SPR: return "Address Mode HL_SPR";
        case AM_D16: return "Address Mode D16";
        case AM_D8: return "Address Mode D8";
        case AM_D16_R: return "Address Mode D16_R";
        case AM_MR_D8: return "Address Mode MR_D8";
        case AM_MR: return "Address Mode MR";
        case AM_A16_R: return "Address Mode A16_R";
        case AM_R_A16: return "Address Mode R_A16";
        case AM_NONE: return "Address Mode NONE";
        default: return "[WARNING]: DebugLookUpAddressMode() cannot find addr_mode to return const char*!";
    }
}

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

const char* DebugLookUpOperand(operand OperandToLookUp) {
    switch(OperandToLookUp) {
        case reg_a: return "Register A";
        case reg_b: return "Register B"; 
        case reg_c: return "Register C";
        case reg_d: return "Register D";
        case reg_e: return "Register E";
        case reg_h: return "Register H";
        case reg_l: return "Register L";
        case reg_bc: return "Register BC";
        case reg_hl: return "Register HL";
        case reg_sp: return "Register SP";
        case reg_de: return "Register DE";
        case reg_af: return "Register AF";
        case immediate_u8: return "Immediate 8 bit";
        case immediate_u16: return "Immediate 16 bit";
        case NONE: return "Operand NONE";
        default: return "[WARNING]: DebugLookUpOperand() cannot find operand to return const char*!";
    }
}

enum register_type {
    U8REG,
    U16REG,
    NONE_REG
};

struct instruction {
    mnemonic InstructionMnemonic;
    addr_mode InstructionAddressMode;
    operand InstructionOperand01;
    operand InstructionOperand02;
    const char* InstructionAffectFlags;
    int InstructionCycles;
    u8 InstructionRSTOpcode;
};

const struct instruction InstructionSet[0x100] = { 

        // [opcode] = {instruction_function, address_mode, operand01, operand02, "flags", cycles, opcode_jp_condition}

    [0x00] = {IN_NOP, AM_IMP, NONE, NONE, "----", 1, 0},
    [0x01] = {IN_LD, AM_R_D16, reg_bc, immediate_u16, "----", 3, 0},
    [0x02] = {IN_LD, AM_MR_R, reg_bc, reg_a, "----", 2, 0},
    [0x03] = {IN_INC, AM_R, reg_bc, NONE, "----", 1, 0},
    [0x04] = {IN_INC, AM_R, reg_b, NONE, "Z0H-", 1, 0},
    [0x05] = {IN_DEC, AM_R, reg_b, NONE, "Z1H-", 1, 0},
    [0x06] = {IN_LD, AM_R_D8, reg_b, immediate_u8, "----", 2, 0},
    [0x07] = {IN_RLCA, AM_NONE, NONE, NONE, "000C", 1, 0},
    [0x08] = {IN_LD, AM_A16_R, immediate_u16, reg_sp, "----", 5, 0},
    [0x09] = {IN_ADD, AM_R_R, reg_hl, reg_bc, "-0HC", 2, 0},
    [0x0A] = {IN_LD, AM_R_MR, reg_a, reg_bc, "----", 2, 0},
    [0x0B] = {IN_DEC, AM_R, reg_bc, NONE, "----", 2, 0},
    [0x0C] = {IN_INC, AM_R, reg_c, NONE, "Z0H-", 1, 0},
    [0x0D] = {IN_DEC, AM_R, reg_c, NONE, "Z1H-", 1, 0},
    [0x0E] = {IN_LD, AM_R_D8, reg_c, immediate_u8, "----", 2, 0},
    [0x0F] = {IN_RRCA, AM_NONE, NONE, NONE, "000C", 1, 0},

    //0x1X
    [0x10] = {IN_STOP, AM_NONE, NONE, NONE, "----", 1, 0},
    [0x11] = {IN_LD, AM_R_D16, reg_de, immediate_u16, "----", 3, 0},
    [0x12] = {IN_LD, AM_MR_R, reg_de, reg_a, "----", 2, 0},
    [0x13] = {IN_INC, AM_R, reg_de, NONE, "----", 2, 0},
    [0x14] = {IN_INC, AM_R, reg_d, NONE, "Z0H-", 1, 0},
    [0x15] = {IN_DEC, AM_R, reg_d, NONE, "Z1H-", 1, 0},
    [0x16] = {IN_LD, AM_R_D8, reg_d, immediate_u8, "----", 2, 0},
    [0x17] = {IN_RLA, AM_NONE, NONE, NONE, "000C", 1, 0},
    [0x18] = {IN_JR, AM_D8, immediate_u8, NONE, "----", 3, 0},
    [0x19] = {IN_ADD, AM_R_R, reg_hl, reg_de, "-0HC", 2, 0},
    [0x1A] = {IN_LD, AM_R_MR, reg_a, reg_de, "----", 2, 0},
    [0x1B] = {IN_DEC, AM_R, reg_de, NONE, "----", 2, 0},
    [0x1C] = {IN_INC, AM_R, reg_e, NONE, "Z0H-", 1, 0},
    [0x1D] = {IN_DEC, AM_R, reg_e, NONE, "Z1H-", 1, 0},
    [0x1E] = {IN_LD, AM_R_D8, reg_e, immediate_u8, "----", 2, 0},
    [0x1F] = {IN_RRA, AM_NONE, NONE, NONE, "000C", 1, 0},

    //0x2X
    [0x20] = {IN_JR, AM_D8, NONE, NONE, "CC_NZ", 2, 0},
    [0x21] = {IN_LD, AM_R_D16, reg_hl, immediate_u16, "----", 3, 0},
    [0x22] = {IN_LD, AM_HLI_R, reg_hl, reg_a, "----", 2, 0},
    [0x23] = {IN_INC, AM_R, reg_hl, NONE, "----", 2, 0},
    [0x24] = {IN_INC, AM_R, reg_h, NONE, "Z0H-", 1, 0},
    [0x25] = {IN_DEC, AM_R, reg_h, NONE, "Z1H-", 1, 0},
    [0x26] = {IN_LD, AM_R_D8, reg_h, NONE, "----", 2, 0},
    [0x27] = {IN_DAA, AM_NONE, NONE, NONE, "Z-0C", 1, 0},
    [0x28] = {IN_JR, AM_D8, NONE, NONE, "CC_Z", 2, 0},
    [0x29] = {IN_ADD, AM_R_R, reg_hl, reg_hl, "-0HC", 2, 0},
    [0x2A] = {IN_LD, AM_R_HLI, reg_a, reg_hl, "----", 2, 0},
    [0x2B] = {IN_DEC, AM_R, reg_hl, NONE, "----", 2, 0},
    [0x2C] = {IN_INC, AM_R, reg_l, NONE, "Z0H-", 1, 0},
    [0x2D] = {IN_DEC, AM_R, reg_l, NONE, "Z1H-", 1, 0},
    [0x2E] = {IN_LD, AM_R_D8, reg_l, immediate_u8, "----", 2, 0},
    [0x2F] = {IN_CPL, AM_NONE, NONE, NONE, "-11-", 1, 0},

    //0x3X
    [0x30] = {IN_JR, AM_D8, NONE, NONE, "CC_NC", 2, 0},
    [0x31] = {IN_LD, AM_R_D16, reg_sp, immediate_u16, "----", 3, 0},
    [0x32] = {IN_LD, AM_HLD_R, reg_hl, reg_a, "----", 2, 0},
    [0x33] = {IN_INC, AM_R, reg_sp, NONE, "----", 2, 0},
    [0x34] = {IN_INC, AM_MR, reg_hl, NONE, "Z0H-", 3, 0},
    [0x35] = {IN_DEC, AM_MR, reg_hl, NONE, "Z1H-", 3, 0},
    [0x36] = {IN_LD, AM_MR_D8, reg_hl, immediate_u8, "----", 3, 0},
    [0x37] = {IN_SCF, AM_NONE, NONE, NONE, "-001", 1, 0},
    [0x38] = {IN_JR, AM_D8, NONE, NONE, "CC_C", 2, 0},
    [0x39] = {IN_ADD, AM_R_R, reg_hl, reg_sp, "-0HC", 2, 0},
    [0x3A] = {IN_LD, AM_R_HLD, reg_a, reg_hl, "----", 2, 0},
    [0x3B] = {IN_DEC, AM_R, reg_sp, NONE, "----", 2, 0},
    [0x3C] = {IN_INC, AM_R, reg_a, NONE, "Z0H-", 1, 0},
    [0x3D] = {IN_DEC, AM_R, reg_a, NONE, "Z1H-", 1, 0},
    [0x3E] = {IN_LD, AM_R_D8, reg_a, immediate_u8, "----", 2, 0},
    [0x3F] = {IN_CCF, AM_NONE, NONE, NONE, "-00C", 1, 0},

    //0x4X
    [0x40] = {IN_LD, AM_R_R, reg_b, reg_b, "----", 1, 0},
    [0x41] = {IN_LD, AM_R_R, reg_b, reg_c, "----", 1, 0},
    [0x42] = {IN_LD, AM_R_R, reg_b, reg_d, "----", 1, 0},
    [0x43] = {IN_LD, AM_R_R, reg_b, reg_e, "----", 1, 0},
    [0x44] = {IN_LD, AM_R_R, reg_b, reg_h, "----", 1, 0},
    [0x45] = {IN_LD, AM_R_R, reg_b, reg_l, "----", 1, 0},
    [0x46] = {IN_LD, AM_R_MR, reg_b, reg_hl, "----", 2, 0},
    [0x47] = {IN_LD, AM_R_R, reg_b, reg_a, "----", 1, 0},
    [0x48] = {IN_LD, AM_R_R, reg_c, reg_b, "----", 1, 0},
    [0x49] = {IN_LD, AM_R_R, reg_c, reg_c, "----", 1, 0},
    [0x4A] = {IN_LD, AM_R_R, reg_c, reg_d, "----", 1, 0},
    [0x4B] = {IN_LD, AM_R_R, reg_c, reg_e, "----", 1, 0},
    [0x4C] = {IN_LD, AM_R_R, reg_c, reg_h, "----", 1, 0},
    [0x4D] = {IN_LD, AM_R_R, reg_c, reg_l, "----", 1, 0},
    [0x4E] = {IN_LD, AM_R_MR, reg_c, reg_hl, "----", 2, 0},
    [0x4F] = {IN_LD, AM_R_R, reg_c, reg_a, "----", 1, 0},

    //0x5X
    [0x50] = {IN_LD, AM_R_R,  reg_d, reg_b, "----", 1, 0},
    [0x51] = {IN_LD, AM_R_R,  reg_d, reg_c, "----", 1, 0},
    [0x52] = {IN_LD, AM_R_R,  reg_d, reg_d, "----", 1, 0},
    [0x53] = {IN_LD, AM_R_R,  reg_d, reg_e, "----", 1, 0},
    [0x54] = {IN_LD, AM_R_R,  reg_d, reg_h, "----", 1, 0},
    [0x55] = {IN_LD, AM_R_R,  reg_d, reg_l, "----", 1, 0},
    [0x56] = {IN_LD, AM_R_MR, reg_d, reg_hl, "----", 2, 0},
    [0x57] = {IN_LD, AM_R_R,  reg_d, reg_a, "----", 1, 0},
    [0x58] = {IN_LD, AM_R_R,  reg_e, reg_b, "----", 1, 0},
    [0x59] = {IN_LD, AM_R_R,  reg_e, reg_c, "----", 1, 0},
    [0x5A] = {IN_LD, AM_R_R,  reg_e, reg_d, "----", 1, 0},
    [0x5B] = {IN_LD, AM_R_R,  reg_e, reg_e, "----", 1, 0},
    [0x5C] = {IN_LD, AM_R_R,  reg_e, reg_h, "----", 1, 0},
    [0x5D] = {IN_LD, AM_R_R,  reg_e, reg_l, "----", 1, 0},
    [0x5E] = {IN_LD, AM_R_MR, reg_e, reg_hl, "----", 2, 0},
    [0x5F] = {IN_LD, AM_R_R,  reg_e, reg_a, "----", 1, 0},

    //0x6X
    [0x60] = {IN_LD, AM_R_R,  reg_h, reg_b, "----", 1, 0},
    [0x61] = {IN_LD, AM_R_R,  reg_h, reg_c, "----", 1, 0},
    [0x62] = {IN_LD, AM_R_R,  reg_h, reg_d, "----", 1, 0},
    [0x63] = {IN_LD, AM_R_R,  reg_h, reg_e, "----", 1, 0},
    [0x64] = {IN_LD, AM_R_R,  reg_h, reg_h, "----", 1, 0},
    [0x65] = {IN_LD, AM_R_R,  reg_h, reg_l, "----", 1, 0},
    [0x66] = {IN_LD, AM_R_MR, reg_h, reg_hl, "----", 2, 0},
    [0x67] = {IN_LD, AM_R_R,  reg_h, reg_a, "----", 1, 0},
    [0x68] = {IN_LD, AM_R_R,  reg_l, reg_b, "----", 1, 0},
    [0x69] = {IN_LD, AM_R_R,  reg_l, reg_c, "----", 1, 0},
    [0x6A] = {IN_LD, AM_R_R,  reg_l, reg_d, "----", 1, 0},
    [0x6B] = {IN_LD, AM_R_R,  reg_l, reg_e, "----", 1, 0},
    [0x6C] = {IN_LD, AM_R_R,  reg_l, reg_h, "----", 1, 0},
    [0x6D] = {IN_LD, AM_R_R,  reg_l, reg_l, "----", 1, 0},
    [0x6E] = {IN_LD, AM_R_MR, reg_l, reg_hl, "----", 2, 0},
    [0x6F] = {IN_LD, AM_R_R,  reg_l, reg_a, "----", 1, 0},

    //0x7X
    [0x70] = {IN_LD, AM_MR_R,  reg_hl, reg_b, "----", 2, 0},
    [0x71] = {IN_LD, AM_MR_R,  reg_hl, reg_c, "----", 2, 0},
    [0x72] = {IN_LD, AM_MR_R,  reg_hl, reg_d, "----", 2, 0},
    [0x73] = {IN_LD, AM_MR_R,  reg_hl, reg_e, "----", 2, 0},
    [0x74] = {IN_LD, AM_MR_R,  reg_hl, reg_h, "----", 2, 0},
    [0x75] = {IN_LD, AM_MR_R,  reg_hl, reg_l, "----", 2, 0},
    [0x76] = {IN_HALT, AM_NONE, NONE, NONE, "----", 1, 0},
    [0x77] = {IN_LD, AM_MR_R,  reg_hl, reg_a, "----", 2, 0},
    [0x78] = {IN_LD, AM_R_R,  reg_a, reg_b, "----", 1, 0},
    [0x79] = {IN_LD, AM_R_R,  reg_a, reg_c, "----", 1, 0},
    [0x7A] = {IN_LD, AM_R_R,  reg_a, reg_d, "----", 1, 0},
    [0x7B] = {IN_LD, AM_R_R,  reg_a, reg_e, "----", 1, 0},
    [0x7C] = {IN_LD, AM_R_R,  reg_a, reg_h, "----", 1, 0},
    [0x7D] = {IN_LD, AM_R_R,  reg_a, reg_l, "----", 1, 0},
    [0x7E] = {IN_LD, AM_R_MR, reg_a, reg_hl, "----", 2, 0},
    [0x7F] = {IN_LD, AM_R_R,  reg_a, reg_a, "----", 1, 0},

    //0x8X
    [0x80] = {IN_ADD, AM_R_R, reg_a, reg_b, "Z0HC", 1, 0},
    [0x81] = {IN_ADD, AM_R_R, reg_a, reg_c, "Z0HC", 1, 0},
    [0x82] = {IN_ADD, AM_R_R, reg_a, reg_d, "Z0HC", 1, 0},
    [0x83] = {IN_ADD, AM_R_R, reg_a, reg_e, "Z0HC", 1, 0},
    [0x84] = {IN_ADD, AM_R_R, reg_a, reg_h, "Z0HC", 1, 0},
    [0x85] = {IN_ADD, AM_R_R, reg_a, reg_l, "Z0HC", 1, 0},
    [0x86] = {IN_ADD, AM_R_MR, reg_a, reg_hl, "Z0HC", 2, 0},
    [0x87] = {IN_ADD, AM_R_R, reg_a, reg_a, "Z0HC", 1, 0},
    [0x88] = {IN_ADC, AM_R_R, reg_a, reg_b, "Z0HC", 1, 0},
    [0x89] = {IN_ADC, AM_R_R, reg_a, reg_c, "Z0HC", 1, 0},
    [0x8A] = {IN_ADC, AM_R_R, reg_a, reg_d, "Z0HC", 1, 0},
    [0x8B] = {IN_ADC, AM_R_R, reg_a, reg_e, "Z0HC", 1, 0},
    [0x8C] = {IN_ADC, AM_R_R, reg_a, reg_h, "Z0HC", 1, 0},
    [0x8D] = {IN_ADC, AM_R_R, reg_a, reg_l, "Z0HC", 1, 0},
    [0x8E] = {IN_ADC, AM_R_MR, reg_a, reg_hl, "Z0HC", 2, 0},
    [0x8F] = {IN_ADC, AM_R_R, reg_a, reg_a, "Z0HC", 1, 0},

    //0x9X
    [0x90] = {IN_SUB, AM_R_R, reg_a, reg_b, "Z1HC", 1, 0},
    [0x91] = {IN_SUB, AM_R_R, reg_a, reg_c, "Z1HC", 1, 0},
    [0x92] = {IN_SUB, AM_R_R, reg_a, reg_d, "Z1HC", 1, 0},
    [0x93] = {IN_SUB, AM_R_R, reg_a, reg_e, "Z1HC", 1, 0},
    [0x94] = {IN_SUB, AM_R_R, reg_a, reg_h, "Z1HC", 1, 0},
    [0x95] = {IN_SUB, AM_R_R, reg_a, reg_l, "Z1HC", 1, 0},
    [0x96] = {IN_SUB, AM_R_MR, reg_a, reg_hl, "Z1HC", 2, 0},
    [0x97] = {IN_SUB, AM_R_R, reg_a, reg_a, "Z1HC", 1, 0},
    [0x98] = {IN_SBC, AM_R_R, reg_a, reg_b, "Z1HC", 1, 0},
    [0x99] = {IN_SBC, AM_R_R, reg_a, reg_c, "Z1HC", 1, 0},
    [0x9A] = {IN_SBC, AM_R_R, reg_a, reg_d, "Z1HC", 1, 0},
    [0x9B] = {IN_SBC, AM_R_R, reg_a, reg_e, "Z1HC", 1, 0},
    [0x9C] = {IN_SBC, AM_R_R, reg_a, reg_h, "Z1HC", 1, 0},
    [0x9D] = {IN_SBC, AM_R_R, reg_a, reg_l, "Z1HC", 1, 0},
    [0x9E] = {IN_SBC, AM_R_MR, reg_a, reg_hl, "Z1HC", 2, 0},
    [0x9F] = {IN_SBC, AM_R_R, reg_a, reg_a, "Z1HC", 1, 0},


    //0xAX
    [0xA0] = {IN_AND, AM_R_R, reg_a, reg_b, "Z010", 1, 0},
    [0xA1] = {IN_AND, AM_R_R, reg_a, reg_c, "Z010", 1, 0},
    [0xA2] = {IN_AND, AM_R_R, reg_a, reg_d, "Z010", 1, 0},
    [0xA3] = {IN_AND, AM_R_R, reg_a, reg_e, "Z010", 1, 0},
    [0xA4] = {IN_AND, AM_R_R, reg_a, reg_h, "Z010", 1, 0},
    [0xA5] = {IN_AND, AM_R_R, reg_a, reg_l, "Z010", 1, 0},
    [0xA6] = {IN_AND, AM_R_MR, reg_a, reg_hl, "Z010", 2, 0},
    [0xA7] = {IN_AND, AM_R_R, reg_a, reg_a, "Z000", 1, 0},
    [0xA8] = {IN_XOR, AM_R_R, reg_a, reg_b, "Z000", 1, 0},
    [0xA9] = {IN_XOR, AM_R_R, reg_a, reg_c, "Z000", 1, 0},
    [0xAA] = {IN_XOR, AM_R_R, reg_a, reg_d, "Z000", 1, 0},
    [0xAB] = {IN_XOR, AM_R_R, reg_a, reg_e, "Z000", 1, 0},
    [0xAC] = {IN_XOR, AM_R_R, reg_a, reg_h, "Z000", 1, 0},
    [0xAD] = {IN_XOR, AM_R_R, reg_a, reg_l, "Z000", 1, 0},
    [0xAE] = {IN_XOR, AM_R_MR, reg_a, reg_hl, "Z000", 2, 0},
    [0xAF] = {IN_XOR, AM_R_R, reg_a, reg_a, "Z000", 1, 0},

    //0xBX
    [0xB0] = {IN_OR, AM_R_R, reg_a, reg_b, "Z000", 1, 0},
    [0xB1] = {IN_OR, AM_R_R, reg_a, reg_c, "Z000", 1, 0},
    [0xB2] = {IN_OR, AM_R_R, reg_a, reg_d, "Z000", 1, 0},
    [0xB3] = {IN_OR, AM_R_R, reg_a, reg_e, "Z000", 1, 0},
    [0xB4] = {IN_OR, AM_R_R, reg_a, reg_h, "Z000", 1, 0},
    [0xB5] = {IN_OR, AM_R_R, reg_a, reg_l, "Z000", 1, 0},
    [0xB6] = {IN_OR, AM_R_MR, reg_a, reg_hl, "Z000", 2, 0},
    [0xB7] = {IN_OR, AM_R_R, reg_a, reg_a, "Z000", 1, 0},
    [0xB8] = {IN_CP, AM_R_R, reg_a, reg_b, "Z1HC", 1, 0},
    [0xB9] = {IN_CP, AM_R_R, reg_a, reg_c, "Z1HC", 1, 0},
    [0xBA] = {IN_CP, AM_R_R, reg_a, reg_d, "Z1HC", 1, 0},
    [0xBB] = {IN_CP, AM_R_R, reg_a, reg_e, "Z1HC", 1, 0},
    [0xBC] = {IN_CP, AM_R_R, reg_a, reg_h, "Z1HC", 1, 0},
    [0xBD] = {IN_CP, AM_R_R, reg_a, reg_l, "Z1HC", 1, 0},
    [0xBE] = {IN_CP, AM_R_MR, reg_a, reg_hl, "Z1HC", 2, 0},
    [0xBF] = {IN_CP, AM_R_R, reg_a, reg_a, "Z1HC", 1, 0},

    [0xC0] = {IN_RET, AM_IMP, NONE, NONE, "CC_NZ", 2, 0},
    [0xC1] = {IN_POP, AM_R, reg_bc, NONE, "----", 3, 0},
    [0xC2] = {IN_JP, AM_D16, NONE, NONE, "CC_NZ", 3, 0},
    [0xC3] = {IN_JP, AM_D16, immediate_u16, NONE, "----", 4, 0},
    [0xC4] = {IN_CALL, AM_D16, NONE, NONE, "CC_NZ", 3, 0},
    [0xC5] = {IN_PUSH, AM_R, reg_bc, NONE, "----", 4, 0},
    [0xC6] = {IN_ADD, AM_R_D8, reg_a, immediate_u8, "Z0HC", 2, 0},
    [0xC7] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x00},
    [0xC8] = {IN_RET, AM_IMP, NONE, NONE, "CC_Z", 2, 0},
    [0xC9] = {IN_RET, AM_NONE, NONE, NONE, "----", 4, 0},
    [0xCA] = {IN_JP, AM_D16, NONE, NONE, "CC_Z", 3, 0},
    [0xCB] = {IN_CB, AM_D8, immediate_u8, NONE, "----", 1, 0},
    [0xCC] = {IN_CALL, AM_D16, NONE, NONE, "CC_Z", 3, 0},
    [0xCD] = {IN_CALL, AM_D16, immediate_u16, NONE, "----", 3, 0},
    [0xCE] = {IN_ADC, AM_R_D8, reg_a, immediate_u8, "Z0HC", 2, 0},
    [0xCF] = {IN_RST, AM_IMP, NONE, NONE, "----", 4 ,0x08},

    [0xD0] = {IN_RET, AM_IMP, NONE, NONE, "CC_NC", 2, 0},
    [0xD1] = {IN_POP, AM_R, reg_de, NONE, "----", 3, 0},
    [0xD2] = {IN_JP, AM_D16, NONE, NONE, "CC_NC", 3, 0},
    [0xD3] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xD4] = {IN_CALL, AM_D16, NONE, NONE, "CC_NC", 3, 0},
    [0xD5] = {IN_PUSH, AM_R, reg_de, NONE, "----", 4, 0},
    [0xD6] = {IN_SUB, AM_R_D8, reg_a, immediate_u8, "Z1HC", 2, 0},
    [0xD7] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x10},
    [0xD8] = {IN_RET, AM_IMP, NONE, NONE, "CC_C", 2, 0},
    [0xD9] = {IN_RETI, AM_NONE, NONE, NONE, "----", 4, 0},
    [0xDA] = {IN_JP, AM_D16, NONE, NONE, "CC_C", 3, 0},
    [0xDB] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xDC] = {IN_CALL, AM_D16, NONE, NONE, "CC_C", 3, 0},
    [0xDD] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xDE] = {IN_SBC, AM_R_D8, reg_a, immediate_u8, "Z1HC", 2, 0},
    [0xDF] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x18},

    //0xEX
    [0xE0] = {IN_LDH, AM_A8_R, NONE, reg_a, "----", 3, 0},
    [0xE1] = {IN_POP, AM_R, reg_hl, NONE, "----", 3, 0},
    [0xE2] = {IN_LD, AM_MR_R, reg_c, reg_a, "----", 2, 0},
    [0xE3] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xE4] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xE5] = {IN_PUSH, AM_R, reg_hl, NONE, "----", 4, 0},
    [0xE6] = {IN_AND, AM_R_D8, reg_a, immediate_u8, "Z010", 2, 0},
    [0xE7] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x20},
    [0xE8] = {IN_ADD, AM_R_D8, reg_sp, immediate_u8, "00HC", 4, 0},
    [0xE9] = {IN_JP, AM_R, reg_hl, NONE, "----", 1, 0},
    [0xEA] = {IN_LD, AM_A16_R, NONE, reg_a, "----", 4, 0},
    [0xEB] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xEC] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xED] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xEE] = {IN_XOR, AM_R_D8, reg_a, immediate_u8, "Z000", 2, 0},
    [0xEF] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x28},

    //0xFX
    [0xF0] = {IN_LDH, AM_R_A8, reg_a, immediate_u8, "----", 3, 0},
    [0xF1] = {IN_POP, AM_R, reg_af, NONE, "ZNHC", 3, 0},
    [0xF2] = {IN_LD, AM_R_MR, reg_a, reg_c, "----", 2, 0},
    [0xF3] = {IN_DI, AM_NONE, NONE, NONE, "----", 1, 0},
    [0xF4] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xF5] = {IN_PUSH, AM_R, reg_af, NONE, "----", 4, 0},
    [0xF6] = {IN_OR, AM_R_D8, reg_a, immediate_u8, "Z000", 2, 0},
    [0xF7] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x30},
    [0xF8] = {IN_LD, AM_HL_SPR, reg_hl, reg_sp, "00HC", 3, 0},
    [0xF9] = {IN_LD, AM_R_R, reg_sp, reg_hl, "----", 2, 0},
    [0xFA] = {IN_LD, AM_R_A16, reg_a, immediate_u16, "----", 4, 0},
    [0xFB] = {IN_EI, AM_NONE, NONE, NONE, "----", 1, 0},
    [0xFC] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xFD] = {IN_NONE, AM_NONE, NONE, NONE, "----", 0, 0},
    [0xFE] = {IN_CP, AM_R_D8, reg_a, immediate_u8, "Z1HC", 2, 0},
    [0xFF] = {IN_RST, AM_IMP, NONE, NONE, "----", 4, 0x38}
};

#endif // INSTRUCTION_HEADER