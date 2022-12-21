#include "cpu.hpp"

void CPU::init() {
    reg.PC = 0x0100 - 1;
    reg.A, reg.F = 0x01B0;
    reg.B, reg.C = 0x0013;
    reg.D, reg.E = 0x00D8;
    reg.H, reg.L = 0x014D;
}

void CPU::setFlag(bool value, FLAGS flag) {
    switch(flag) {
        case ZERO:
            BIT_SET(reg.F, 7, (int)value);
            break;
        case ADDSUB:
            BIT_SET(reg.F, 6, (int)value);
            break;
        case HALF:
            BIT_SET(reg.F, 5, (int)value);
            break;
        case CARRY:
            BIT_SET(reg.F, 4, (int)value);
            break;
        default:
            std::cerr << "[ERROR]: setFlag could not find case for FLAG set" << NEWLINE;
    }
}

u16 CPU::u8Tou16(u8 r1, u8 r2) {
    return (r2 << 8) | r1;
}

u8 &CPU::lookupRegU8(operand opt) {
    switch(opt) {
        case reg_a: return reg.A;
        case reg_b: return reg.B;
        case reg_c: return reg.C;
        case reg_d: return reg.D;
        case reg_e: return reg.E;   
        case reg_h: return reg.H;
        case reg_l: return reg.L;
        case immediate_u8: u8 addr = ram.readU8(reg.PC++); return addr; 
        default: std::cout << "[ERROR]: Cannot find U8 register_lookup: " << opt << NEWLINE; 
    }
}

u16 &CPU::lookupRegU16(operand opt) {
    switch(opt) {
        case reg_bc: return reg.BC;
        case reg_hl: return reg.HL;
        case reg_sp: return reg.SP;
        case reg_de: return reg.DE;
        case immediate_u16: u16 addr = ram.readU16(u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++))); return addr; 
        default: std::cout << "[ERROR]: Cannot find U16 register_lookup: " << opt << NEWLINE; 
    }
}

instruction CPU::instructionOpcode(u8 opcode) {
    return instructions[opcode];
}

template<typename T>
register_type CPU::checkResgisterType(T opt) {
    if(decltype(opt) == u8) {
        return U8REG;
    } if else(decltype(opt) == u16) {
        return U16REG;
    } else {
        return NONE_REG;
    }
}

void CPU::instructionExecute(instruction inst) {

    switch(inst.MNC) {
        case IN_LD:
            switch(inst.addr) {
                case AM_MR_R:
                    ram.writeU8(lookupRegU16(inst.op01), lookupRegU8(inst.op02));
                    break;
                case AM_R_R:
                    if(inst.op01 == reg_sp && inst.op02 == reg_hl) {
                        reg.SP = reg.HL;
                        break;
                    }
                    lookupRegU8(inst.op01) = lookupRegU8(inst.op02);
                    break;  
                case AM_R_MR:
                    lookupRegU8(inst.op01) = ram.readU8(lookupRegU16(inst.op02));
                    break;
                case AM_R_HLI:
                    lookupRegU8(inst.op01) = ram.readU8(reg.HL++);
                    break;
                case AM_R_HLD:
                    lookupRegU8(inst.op01) = ram.readU8(reg.HL--);
                    break;
                case AM_HLI_R:
                    ram.writeU8(ram.readU8(reg.HL++), lookupRegU8(inst.op02));
                    break;
                case AM_HLD_R:
                    ram.writeU8(ram.readU8(reg.HL--), lookupRegU8(inst.op02));
                    break;
                case AM_HL_SPR:
                    // config
                    break;
                case AM_MR_D8:
                    u8 nn = ram.readU8(reg.PC++);
                    ram.writeU8(lookupRegU16(inst.op01), nn);
                    break;
                case AM_A16_R:
                    u16 nn = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                    ram.writeU8(nn, (u8) ((reg.SP & 0xFF00) >> 8));
                    ram.writeU8(nn + 1, (u8)(reg.SP & 0x00FF));
                    break;
                case AM_R_A16:
                    u16 nn = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                    lookupRegU8(inst.op01) = ram.readU8(nn); // reg.A = ram.readU8(nn);
                    break;
                case AM_NONE:
                default:
                    std::cerr << "[ERROR]: Cannot find addr_modo for instruction IN_LD! : " << inst.addr << std::endl;
            }
            break;
        case IN_NOP:
            break;
        case IN_PUSH:
            reg.SP--;
            ram.writeU8(reg.SP--, (u8) ((lookupRegU16(inst.op01) & 0xFF00) >> 8));
            ram.writeU8(reg.SP, (u8)(lookupRegU16(inst.op02) & 0x00FF));

    }

}

