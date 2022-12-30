#include "cpu.hpp"

void CPU::init(RAM &ram)
{
    this->ram = ram;

    reg.PC = 0x0100;
    reg.AF = 0x01B0;
    reg.BC = 0x0013;
    reg.DE = 0x00D8;
    reg.HL = 0x014D;
}

#ifdef DEBUG
void CPU::debug(instruction inst) {
    std::cout << "STATE: Reg_A: " << reg.A << " / Reg_B: " << reg.B << " / Reg_C: " << reg.C << " / Reg_D: " << reg.D << " / Reg_E: " << reg.E << " / Reg_F: " << reg.F << " / Reg_SP: " << reg.SP << " / Reg_H: " << reg.H << " / Reg_L: " << reg.L << "" << NEWLINE;
    std::cout << "PC: " << std::hex << reg.PC << NEWLINE;
    std::cout << "FLAGS: Carry: " << getFlag(CARRY) << " / AddSub: " << getFlag(ADDSUB) << " / Half: " << getFlag(HALF) << " / Zero: " << getFlag(ZERO) << NEWLINE;
    std::cout << "Instruction: " << inst.MNC << " / Addr_Mode: " << inst.addr << " / Op01: " << inst.op01 << " / Op02: " << inst.op02 << " / Flags: " << inst.affectFlags << " / RST_Opcode: " << inst.rst_opcode << " / Cycles: " << inst.cycles << NEWLINE;
    std::cout << "______________________" << NEWLINE;
} 
#endif

void CPU::execute() {
    instruction inst = instructionByOpcode(reg.PC);
    instructionExecute(inst);
    debug(inst);
}

void CPU::setFlag(bool value, FLAGS flag)
{
    switch (flag)
    {
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

int CPU::getFlag(FLAGS flag) {
    switch(flag) {
        case ZERO:
            return BIT(reg.F, 7);
        case ADDSUB:
            return BIT(reg.F, 6);
        case HALF:
            return BIT(reg.F, 5);
        case CARRY:
            return BIT(reg.F, 4);
        default:   
            std::cerr << "[ERROR]: Cannot find flag in function getFlag()!" << std::endl;
            return -1;
    }
}

void CPU::checkAffectedFlags(u16 result, const char* affectflags, u16 carry) {
    if(affectflags == "----") return; 
    switch(affectflags[0]) {
        case '-':
            break;
        case 'Z':
            if(result == 0) {
                setFlag(1, ZERO);
                break;
            } else {
                setFlag(0, ZERO);
                break;
            }
        case '1':
            setFlag(1, ZERO);
            break;
        case '0': 
            setFlag(0, ZERO);
            break;
    }

    switch(affectflags[1]) {
        case '-':
            break;
        case '1':
            setFlag(1, ADDSUB);
            break;
        case '0':
            setFlag(0, ADDSUB);
            break;
    }

    switch(affectflags[2]) {
        case '-': 
            break;
        case 'H':
            if(BIT(carry, 3)) {
                setFlag(1, HALF);
                break;
            } else {
                setFlag(0, HALF);
                break;
            }
        case '1':
            setFlag(1, HALF);
            break;
        case '0':
            setFlag(0, HALF);
            break;
    }

    switch(affectflags[3]) {
        case '-': 
            break;
        case 'H':
            if(BIT(carry, 7)) {
                setFlag(1, CARRY);
                break;
            } else {
                setFlag(0, CARRY);
                break;
            }
        case '1':
            setFlag(1, CARRY);
            break;
        case '0':
            setFlag(0, CARRY);
            break;
    }
}

bool CPU::conditionalInstruction(const char* cc) {
    if(cc == "CC_NZ") {
        return (getFlag(ADDSUB) == 1 && getFlag(ZERO) == 1) ? true : false;
    }
    if(cc == "CC_Z") {
        return getFlag(ZERO) == 1 ? true : false;
    }
    if(cc == "CC_NC") {
        return (getFlag(ADDSUB) == 1 && getFlag(CARRY) == 1) ? true : false;
    }
    if(cc == "CC_C") {
        return getFlag(CARRY) == 1 ? true : false;     
    }
    std::cerr << "[ERROR]: Cannot find conditional_instruction_cc pattern! : " << cc << std::endl;
    return NULL;
}

u8 set_bit(u8 number, int bit) {
    if(bit <= 0) {
        return number;
    }
    return (number | (1 << (bit - 1)));
}

u16 CPU::u8Tou16(u8 r1, u8 r2)
{
    return (r2 << 8) | r1;
}

u8 &CPU::lookupRegU8(operand opt) {
    switch (opt) {
        case reg_a:
            return reg.A;
        case reg_b:
            return reg.B;
        case reg_c:
            return reg.C;
        case reg_d:
            return reg.D;
        case reg_e:
            return reg.E;
        case reg_h:
            return reg.H;
        case reg_l:
            return reg.L;
        case immediate_u8:
        {
            u8 addr = ram.readU8(reg.PC++);
            return addr;
        }
        default:
        {
            std::cout << "[ERROR]: Cannot find U8 register_lookup: " << opt << NEWLINE;
            u8 error = 0x00;
            return error;
        }
    }
}

u16 &CPU::lookupRegU16(operand opt)
{
    switch (opt)
    {
    case reg_bc:
        return reg.BC;
    case reg_hl:
        return reg.HL;
    case reg_sp:
        return reg.SP;
    case reg_de:
        return reg.DE;
    case reg_af:
        return reg.AF;
    case immediate_u16:
    {
        u16 addr = ram.readU16(reg.PC++);
        return addr;
    }
    default:
        std::cout << "[ERROR]: Cannot find U16 register_lookup: " << opt << NEWLINE;
    }
}

instruction CPU::instructionByOpcode(u8 opcode)
{
    return instructions[opcode];
}

template <typename T>
register_type CPU::checkResgisterType(T opt)
{
    if (sizeof(opt) == sizeof(char)){
        return U8REG;
    } else if(sizeof(opt) == sizeof(short)){
        return U16REG;
    } else {
        std::cerr << "[ERROR]: NONE_REG in checkRegisterType was been thown!" << std::endl;
        return NONE_REG;
    }
}

void CPU::bit_rotateLeft(u8& reg, bool carry) {
    u8 bit7 = (reg & 0x80) >> 7;
    u8 value = reg << 1;

    if(carry) {
        reg = value + bit7;
    } else {
        reg = value + getFlag(CARRY);
    }

   setFlag(0, HALF);
   setFlag(0, ADDSUB);
   setFlag((reg == 0), ZERO);
   setFlag((bool)bit7, CARRY);
}

void CPU::bit_rotateRight(u8& reg, bool carry) {
    u8 bit0 = reg & 0x01;
    u8 value = reg >> 1;

    if(carry) {
        reg = value + (bit0 << 7);
    } else {
        reg = value + (getFlag(CARRY) << 7);
    }

    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag((reg == 0), ZERO);
    setFlag((bool)bit0, CARRY);
}

void CPU::bit_swap(u8& reg) {
    reg = (reg >> 4) | ((reg & 0xF) << 4);

    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag((reg == 0), ZERO);
    setFlag(0, CARRY);
    
}
 
void CPU::bit_test(u8& reg, int bit) {
    u8 value = (reg >> bit) & 1;
    
    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag((value == 0), ZERO);
}

void CPU::bit_reset(u8& reg, int bit) {
    u8 bitMask = 1 << bit;
    u8 value = reg & ~bitMask;
    reg = value;
}

void CPU::bit_sla(u8& reg) {
    u8 bit7 = (reg & 0x80) >> 7;
    reg = reg << 1;

    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag((reg == 0), ZERO);
    setFlag((bool)bit7, CARRY);
}

void CPU::bit_srl(u8& reg) {
    u8 bit0 = reg & 0x01;
    reg = reg >> 1;

    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag((reg == 0), ZERO);
    setFlag((bool)bit0, CARRY);
}

void CPU::bit_sra(u8& reg) {
    u8 bit0 = reg & 0x01;
    u8 bit7 = reg & 0x80;
    reg = (reg >> 1) + bit7;

    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag((reg == 0), ZERO);
    setFlag((bool)bit0, CARRY);
}

void CPU::bit_rra(bool carry) {
    u8 bit0 = reg.A & 0x01;
    u8 value = reg.A >> 1;

    if(carry) {
        reg.A = value + (bit0 << 7);
    } else {
        reg.A = value + (getFlag(CARRY) << 7);
    }

    setFlag(0, HALF);
    setFlag(0, ADDSUB);
    setFlag(0, ZERO);
    setFlag((bool)bit0, CARRY);
}

void bit_set(u8& reg, int bit) {
    u8 bitMask = 1 << bit;
    u8 value = reg | bitMask;
    reg = value;
}

void CPU::instructionExecute(instruction inst) {
    u8 opcode = reg.PC;
    u16 result;
    u16 carry;

    switch (inst.MNC)
    {
    case IN_LD:
        { 
            switch (inst.addr) {
            case AM_MR_R:
                if (inst.op01 == reg_c && inst.op02 == reg_a)
                { // 0xE2
                    ram.writeU8(u8Tou16(reg.C, 0xFF), reg.A);
                    break;
                }

                ram.writeU8(lookupRegU16(inst.op01), lookupRegU8(inst.op02));
                break;
                
            case AM_R_R:
                if (inst.op01 == reg_sp && inst.op02 == reg_hl)
                {
                    reg.SP = reg.HL;
                    break;
                }
                lookupRegU8(inst.op01) = lookupRegU8(inst.op02);
                break;
            case AM_R_MR:
                if (inst.op01 == reg_a && inst.op02 == reg_c)
                { // 0xF2
                    reg.A = ram.readU8(u8Tou16(reg.C, 0xFF));
                    break;
                }
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
                {
                // config
                u8 e = ram.readU8(reg.PC++);
                u8 result = ~(reg.SP + e) + 1;
                reg.HL = result;
                break;
                }
            case AM_MR_D8:
                {
                u8 nn = ram.readU8(reg.PC++);
                ram.writeU8(lookupRegU16(inst.op01), nn);
                break;
                }
            case AM_A16_R:
                {
                u16 nn = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                ram.writeU8(nn, (u8)((reg.SP & 0xFF00) >> 8));
                ram.writeU8(nn + 1, (u8)(reg.SP & 0x00FF));
                break;
                }
            case AM_R_A16:
                {
                u16 nn = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                lookupRegU8(inst.op01) = ram.readU8(nn); // reg.A = ram.readU8(nn);
                break;
                }
            case AM_R_D16:
                lookupRegU16(inst.op01) = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                break; 
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_LD! : " << inst.addr << std::endl;
            }
            break;
        }
    case IN_LDH:
        {
        if (inst.op01 == reg_a && inst.op02 == immediate_u8) { // 0xF0
            
            u8 n = ram.readU8(reg.PC++);
            reg.A = ram.readU8(u8Tou16(n, 0xFF));
            break;
            
        } else { // 0xE0
            u8 n = ram.readU8(reg.PC++);
            ram.writeU8(u8Tou16(n, 0xFF), reg.A);
            break;
        }
        }
    case IN_DEC:
        switch(inst.addr) {
            case AM_R:
            if(checkResgisterType(inst.op01) == U8REG) {
                result, carry = lookupRegU8(inst.op01) -= 1;
                break;
            } else {
                result, carry = lookupRegU16(inst.op01) -= 1;
                break;
            }
            
            case AM_MR:
                result, carry = ram.readU8(reg.HL) - 1;
                ram.writeU8(reg.HL, result);
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_DEC! : " << inst.addr << std::endl;
        }
        

    case IN_ADD:
        switch(inst.addr) {
            case AM_R_R:
                if(opcode == 0x09 || opcode == 0x19 || opcode == 0x29 || opcode == 0x39) {
                    carry = (u16) (lookupRegU16(inst.op01) += lookupRegU16(inst.op02));
                    result = lookupRegU16(inst.op01);
                    break;
                } else {
                    carry = (u16) (lookupRegU8(inst.op01) += lookupRegU8(inst.op02));
                    result = (u16) lookupRegU8(inst.op01);
                    break;
                }
            case AM_R_MR: 
                carry = reg.A += ram.readU8(reg.HL);
                result = (u16) reg.A;
                break;
            case AM_R_D8:
                if(opcode == 0xC6) {
                carry = reg.A += ram.readU8(reg.PC++);
                result = (u16) reg.A;
                break;
                } else if (opcode == 0xE8) {
                    // config
                } 
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_ADD! : " << inst.addr << std::endl;
        }

    case IN_OR:
        switch(inst.addr) {
            case AM_R_R:
                result = reg.A |= lookupRegU8(inst.op02);
                break;
            case AM_R_MR:
                result = reg.A |= ram.readU8(reg.HL);
                break;
            case AM_R_D8:
                result = reg.A |= ram.readU8(reg.PC++);
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_OR! : " << inst.addr << std::endl;
        }
        break;

    case IN_XOR:
        switch(inst.addr) {
            case AM_R_R:
                result = (u16) (reg.A ^= lookupRegU8(inst.op02));
                break;
            case AM_R_MR:
                result = (u16) (reg.A ^= ram.readU8(reg.HL));
                break;
            case AM_R_D8:
                result = (u16) (reg.A ^= ram.readU8(reg.PC++));
                break;
            
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_XOR! : " << inst.addr << std::endl;
        }
        break;

    case IN_AND:
        switch(inst.addr) {
            case AM_R_R:
                result = (u16) (reg.A &= lookupRegU8(inst.op02));
                break;
            case AM_R_MR:
                result = (u16) (reg.A &= ram.readU8(reg.HL));
                break;
            case AM_R_D8:
                result = (u16) ( reg.A &= ram.readU8(reg.PC++));
                break;
            
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_AND! : " << inst.addr << std::endl;
    }
        break;

    case IN_JP:
        switch(inst.addr) {
            case AM_D16:
                
                if(opcode == 0xC3) {
                    reg.PC = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                    break;
                } 

            case AM_R:
                reg.PC = reg.HL;
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_JP! : " << inst.addr << std::endl;
        }
        break;

    case IN_JR:
        {
        if(opcode == 0x18) {
            char ee = static_cast<char>(ram.readU8(reg.PC++));
            reg.PC += (ee + 1);
            break;
        } else {
            char ee = static_cast<char>(ram.readU8(reg.PC++));
            if(conditionalInstruction(inst.affectFlags)) {
                reg.PC += (ee + 1);
            }
            break;
        }
        }
        break;

    case IN_CALL:
        switch(inst.addr) {
            case AM_D16:
            {
                if(opcode == 0xC4 || opcode == 0xCC || opcode == 0xD4 || opcode == 0xDC) {
                    u16 nn = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                    if(conditionalInstruction(inst.affectFlags)) {
                            reg.SP--;
                            ram.writeU8(reg.SP--, ((u8)(reg.PC & 0x00FF)));
                            ram.writeU8(reg.SP, ((u8)((reg.PC & 0xFF00) >> 8)));
                            reg.PC = nn;
                            inst.cycles = 6;
                            break;
                        } 
                    reg.PC = nn;
                    break;
                } else { // 0xCD
                    u16 nn = u8Tou16(ram.readU8(reg.PC++), ram.readU8(reg.PC++));
                    reg.SP--;
                    ram.writeU8(reg.SP--, ((u8)(reg.PC & 0x00FF)));
                    ram.writeU8(reg.SP, ((u8)((reg.PC & 0xFF00) >> 8)));
                    reg.PC = nn;
                }
                break;
            }
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_CALL! : " << inst.addr << std::endl;
        }

    case IN_RET:
        switch(inst.addr) {
            case AM_NONE:
                reg.PC = u8Tou16(ram.readU8(reg.SP++), ram.readU8(reg.SP++));
                break;
            case AM_IMP:
                if(conditionalInstruction(inst.affectFlags)) {
                    reg.PC = u8Tou16(ram.readU8(reg.SP++), ram.readU8(reg.SP++));
                    inst.cycles = 5;
                }
                break;
            default: 
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_RET! : " << inst.addr << std::endl;
         }
         break;

    case IN_RST:
        {   
            reg.SP--;
            ram.writeU8(reg.SP--, ((u8)(reg.PC & 0x00FF)));
            ram.writeU8(reg.SP, ((u8)((reg.PC & 0xFF00) >> 8)));
            reg.PC = u8Tou16(inst.rst_opcode, 0x00);
        }
        break;

    case IN_CP:
        switch(inst.addr) { 
            case AM_R_R:
                result, carry = reg.A - lookupRegU8(inst.op02);
                break;
            case AM_MR:
                result, carry = reg.A - ram.readU8(reg.HL);
                break;
            case AM_R_D8:
                result, carry = reg.A - ram.readU8(reg.PC++);
                break;
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_CP! : " << inst.addr << std::endl; 
        }
        break;

    case IN_SUB:
        switch(inst.addr) {
            case AM_R_R:
                result, carry = reg.A -= lookupRegU8(inst.op02);
                break;
            case AM_R_MR:
                result, carry = reg.A -= ram.readU8(reg.HL);
                break;
            case AM_R_D8:
                result, carry = reg.A -= ram.readU8(reg.PC++);
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_SUB! : " << inst.addr << std::endl;
        }
        break;

    case IN_ADC:
        switch(inst.addr) {
            case AM_R_R:
                result, carry = reg.A += (getFlag(CARRY) + lookupRegU8(inst.op02));
                break;
            case AM_R_MR:
                result, carry = reg.A += (getFlag(CARRY) + ram.readU8(reg.HL));
                break;
            case AM_R_D8:
                result, carry = reg.A += (getFlag(CARRY) + ram.readU8(reg.PC++));
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_ADC! : " << inst.addr << std::endl;
        }
        break;
    
    case IN_SBC:
        switch(inst.addr) {
            case AM_R_R:
                result, carry = reg.A -= (getFlag(CARRY) - lookupRegU8(inst.op02));
                break;
            case AM_R_MR:
                result, carry = reg.A -= (getFlag(CARRY) - ram.readU8(reg.HL));
                break;
            case AM_R_D8:
                result, carry = reg.A -= (getFlag(CARRY) - ram.readU8(reg.PC++));
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_SBC! : " << inst.addr << std::endl;
        }
        break;

    case IN_POP:
        lookupRegU16(inst.op01) = u8Tou16(ram.readU8(reg.SP++), ram.readU8(reg.SP++));
        break;

    case IN_CCF:
        setFlag(false, ADDSUB);
        setFlag(false, HALF);
        if(BIT(reg.F, 4) == 1) {
            setFlag(false, CARRY);
        } else {
            setFlag(true, CARRY);
        }
        break;

    case IN_PUSH:
        reg.SP--;
        ram.writeU8(reg.SP--, (u8)((lookupRegU16(inst.op01) & 0xFF00) >> 8));
        ram.writeU8(reg.SP, (u8)(lookupRegU16(inst.op02) & 0x00FF));
        break;

    case IN_SCF:
        setFlag(false, ADDSUB);
        setFlag(false, HALF);
        setFlag(true, CARRY);
        break;

    case IN_CPL:
        reg.A = ~reg.A;
        setFlag(true, ADDSUB);
        setFlag(true, HALF);
        break;

    case IN_DI:
        IME = 0;
        break;
    
    case IN_RETI:
        reg.PC = u8Tou16(ram.readU8(reg.SP++), ram.readU8(reg.SP++));
        IME = 1;
        break;

    case IN_EI:
        IME_scheduled = true;
        break;

    case IN_RRA:
        reg.A = ((reg.A >> 1) | (reg.A << (8 - 1)));
        break;
    
    case IN_RLA:
        reg.A = ((reg.A << 1) | (reg.A >> (8 - 1)));
        reg.A = set_bit(reg.A, getFlag(CARRY));
        break;

    case IN_NOP:
        break;

    case IN_CB:
        instructionCBexecute(inst, ram.readU8(reg.PC++));
        break;
    
    case IN_NONE:
    default:
        std::cerr << "[ERROR]: Cannot find MNEMONIC for instruction! " << std::endl;

    }
    
    cycle(inst.cycles);
    checkAffectedFlags(result, inst.affectFlags, carry);
    reg.PC++;
}               

void CPU::instructionCBexecute(const instruction inst, const u8 immediate) {
    switch(immediate) {
        case 0x37:  // Swap upper and lower nibbles of register A, flags updated
            bit_swap(reg.A);
            cycle(8);
            break;
        case 0x30:  // Swap upper and lower nibbles of register B, flags updated
            bit_swap(reg.B);
            cycle(8);
            break;
        case 0x31:  // Swap upper and lower nibbles of register C, flags updated
            bit_swap(reg.C);
            cycle(8);
            break;
        case 0x32:  // Swap upper and lower nibbles of register D, flags updated
            bit_swap(reg.D);
            cycle(8);
            break;
        case 0x33:  // Swap upper and lower nibbles of register E, flags updated
            bit_swap(reg.E);
            cycle(8);
            break;
        case 0x34:  // Swap upper and lower nibbles of register H, flags updated
            bit_swap(reg.H);
            cycle(8);
            break;
        case 0x35:  // Swap upper and lower nibbles of register L, flags updated
            bit_swap(reg.L);
            cycle(8);
            break;
        case 0x36:  // Swap upper and lower nibbles of byte at (HL), flags updated
            bit_swap(reg.HL);
            cycle(16);
            break;
        case 0x07:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.A, true); // Carry = true (Rotate left with carry RLC A)
            cycle(8);
            break;
        case 0x00:  // Rotate contents of B register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.B, true); // Carry = true (Rotate left with carry RLC B)
            cycle(8);
            break;
        case 0x01:  // Rotate contents of C register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.C, true); // Carry = true (Rotate left with carry RLC C)
            cycle(8);
            break;
        case 0x02:  // Rotate contents of D register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.D, true); // Carry = true (Rotate left with carry RLC D)
            cycle(8);
            break;
        case 0x03:  // Rotate contents of E register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.E, true); // Carry = true (Rotate left with carry RLC E)
            cycle(8);
            break;
        case 0x04:  // Rotate contents of H register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.H, true); // Carry = true (Rotate left with carry RLC H)
            cycle(8);
            break;
        case 0x05:  // Rotate contents of L register left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.L, true); // Carry = true (Rotate left with carry RLC L)
            cycle(8);
            break;
        case 0x06:  // Rotate contents of byte at (HL) left and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.HL, true); // Carry = true (Rotate left with carry RLC L)
            cycle(16);
            break;
        case 0x17:  // Rotate contents of A register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.A, false); // Carry = false (Rotate left RL A)
            cycle(8);
            break;
        case 0x10:  // Rotate contents of B register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.B, false); // Carry = false (Rotate left with carry RL B)
            cycle(8);
            break;
        case 0x11:  // Rotate contents of C register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.C, false); // Carry = false (Rotate left with carry RL C)
            cycle(8);
            break;
        case 0x12:  // Rotate contents of D register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.D, false); // Carry = false (Rotate left with carry RL D)
            cycle(8);
            break;
        case 0x13:  // Rotate contents of E register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.E, false); // Carry = false (Rotate left with carry RL E)
            cycle(8);
            break;
        case 0x14:  // Rotate contents of H register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.H, false); // Carry = false (Rotate left with carry RL H)
            cycle(8);
            break;
        case 0x15:  // Rotate contents of L register left through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.L, false); // Carry = false (Rotate left with carry RL L)
            cycle(8);
            break;
        case 0x16:  // Rotate contents of byte at (HL) lef through carry flag and store bit 7 in CF, flags updated
            bit_rotateLeft(reg.HL, false); // Carry = false (Rotate left with carry RL L)
            cycle(16);
            break;
        case 0x0f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.A, true); // Carry = true (Rotate right with carry RRC A)
            cycle(8);
            break;
        case 0x08:  // Rotate contents of B register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.B, true); // Carry = true (Rotate right with carry RRC B)
            cycle(8);
            break;
        case 0x09:  // Rotate contents of C register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.C, true); // Carry = true (Rotate right with carry RRC C)
            cycle(8);
            break;
        case 0x0a:  // Rotate contents of D register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.D, true); // Carry = true (Rotate right with carry RRC D)
            cycle(8);
            break;
        case 0x0b:  // Rotate contents of E register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.E, true); // Carry = true (Rotate right with carry RRC E)
            cycle(8);
            break;
        case 0x0c:  // Rotate contents of H register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.H, true); // Carry = true (Rotate right with carry RRC H)
            cycle(8);
            break;
        case 0x0d:  // Rotate contents of L register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.L, true); // Carry = true (Rotate right with carry RRC L)
            cycle(8);
            break;
        case 0x0e:  // Rotate contents of byte at (HL) right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.HL, true); // Carry = true (Rotate right with carry RRC (HL))
            cycle(16);
            break;
        case 0x1f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.A, false); // Carry = false (Rotate right RR A)
            cycle(8);
            break;
        case 0x18:  // Rotate contents of B register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.B, false); // Carry = false (Rotate right with carry RR B)
            cycle(8);
            break;
        case 0x19:  // Rotate contents of C register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.C, false); // Carry = false (Rotate right with carry RR C)
            cycle(8);
            break;
        case 0x1a:  // Rotate contents of D register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.D, false); // Carry = false (Rotate right with carry RR D)
            cycle(8);
            break;
        case 0x1b:  // Rotate contents of E register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.E, false); // Carry = false (Rotate right with carry RR E)
            cycle(8);
            break;
        case 0x1c:  // Rotate contents of H register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.H, false); // Carry = false (Rotate right with carry RR H)
            cycle(8);
            break;
        case 0x1d:  // Rotate contents of L register right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.L, false); // Carry = false (Rotate right with carry RR L)
            cycle(8);
            break;
        case 0x1e:  // Rotate contents of byte at (HL) right and store bit 0 in CF, flags updated
            bit_rotateRight(reg.HL, false); // Carry = false (Rotate right with carry RR (HL))
            cycle(16);
            break;
        case 0x27:  // Shift contents of register A left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.A);
            cycle(8);
            break;
        case 0x20:  // Shift contents of register B left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.B);
            cycle(8);
            break;
        case 0x21:  // Shift contents of register C left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.C);
            cycle(8);
            break;
        case 0x22:  // Shift contents of register D left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.D);
            cycle(8);
            break;
        case 0x23:  // Shift contents of register E left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.E);
            cycle(8);
            break;
        case 0x24:  // Shift contents of register H left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.H);
            cycle(8);
            break;
        case 0x25:  // Shift contents of register L left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.L);
            cycle(8);
            break;
        case 0x26:  // Shift contents of byte at (HL) left and store bit 7 in CF, bit0 = 0, flags updated
            bit_sla(reg.HL);
            cycle(16);
            break;
        case 0x2f:  // Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.A);
            cycle(8);
            break;
        case 0x28:  // Shift contents of register B right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.B);
            cycle(8);
            break;
        case 0x29:  // Shift contents of register C right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.C);
            cycle(8);
            break;
        case 0x2a:  // Shift contents of register D right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.D);
            cycle(8);
            break;
        case 0x2b:  // Shift contents of register E right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.E);
            cycle(8);
            break;
        case 0x2c:  // Shift contents of register H right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.H);
            cycle(8);
            break;
        case 0x2d:  // Shift contents of register L right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.L);
            cycle(8);
            break;
        case 0x2e:  // Shift contents byte at (HL) right and store bit 0 in CF, bit7 unchanged, flags updated
            bit_sra(reg.HL);
            cycle(16);
            break;
        case 0x3f:  // Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.A);
            cycle(8);
            break;
        case 0x38:  // Shift contents of register B right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.B);
            cycle(8);
            break;
        case 0x39:  // Shift contents of register C right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.C);
            cycle(8);
            break;
        case 0x3a:  // Shift contents of register D right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.D);
            cycle(8);
            break;
        case 0x3b:  // Shift contents of register E right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.E);
            cycle(8);
            break;
        case 0x3c:  // Shift contents of register H right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.H);
            cycle(8);
            break;
        case 0x3d:  // Shift contents of register L right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.L);
            cycle(8);
            break;
        case 0x3e:  // Shift contents byte at (HL) right and store bit 0 in CF, bit7 = 0, flags updated
            bit_srl(reg.HL);
            cycle(16);
            break;
        case 0x47:  // Test bit 0 in register A and set flags accordingly
            bit_test(reg.A, 0);
            cycle(8);
            break;
        case 0x40:  // Test bit 0 in register B and set flags accordingly
            bit_test(reg.B, 0);
            cycle(8);
            break;
        case 0x41:  // Test bit 0 in register C and set flags accordingly
            bit_test(reg.C, 0);
            cycle(8);
            break;
        case 0x42:  // Test bit 0 in register D and set flags accordingly
            bit_test(reg.D, 0);
            cycle(8);
            break;
        case 0x43:  // Test bit 0 in register E and set flags accordingly
            bit_test(reg.E, 0);
            cycle(8);
            break;
        case 0x44:  // Test bit 0 in register H and set flags accordingly
            bit_test(reg.H, 0);
            cycle(8);
            break;
        case 0x45:  // Test bit 0 in register L and set flags accordingly
            bit_test(reg.L, 0);
            cycle(8);
            break;
        case 0x46:  // Test bit 0 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 0);
            cycle(16);
            break;
        case 0x4f:  // Test bit 1 in register A and set flags accordingly
            bit_test(reg.A, 1);
            cycle(8);
            break;
        case 0x48:  // Test bit 1 in register B and set flags accordingly
            bit_test(reg.B, 1);
            cycle(8);
            break;
        case 0x49:  // Test bit 1 in register C and set flags accordingly
            bit_test(reg.C, 1);
            cycle(8);
            break;
        case 0x4a:  // Test bit 1 in register D and set flags accordingly
            bit_test(reg.D, 1);
            cycle(8);
            break;
        case 0x4b:  // Test bit 1 in register E and set flags accordingly
            bit_test(reg.E, 1);
            cycle(8);
            break;
        case 0x4c:  // Test bit 1 in register H and set flags accordingly
            bit_test(reg.H, 1);
            cycle(8);
            break;
        case 0x4d:  // Test bit 1 in register L and set flags accordingly
            bit_test(reg.L, 1);
            cycle(8);
            break;
        case 0x4e:  // Test bit 1 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 1);
            cycle(16);
            break;
        case 0x57:  // Test bit 2 in register A and set flags accordingly
            bit_test(reg.A, 2);
            cycle(8);
            break;
        case 0x50:  // Test bit 2 in register B and set flags accordingly
            bit_test(reg.B, 2);
            cycle(8);
            break;
        case 0x51:  // Test bit 2 in register C and set flags accordingly
            bit_test(reg.C, 2);
            cycle(8);
            break;
        case 0x52:  // Test bit 2 in register D and set flags accordingly
            bit_test(reg.D, 2);
            cycle(8);
            break;
        case 0x53:  // Test bit 2 in register E and set flags accordingly
            bit_test(reg.E, 2);
            cycle(8);
            break;
        case 0x54:  // Test bit 2 in register H and set flags accordingly
            bit_test(reg.H, 2);
            cycle(8);
            break;
        case 0x55:  // Test bit 2 in register L and set flags accordingly
            bit_test(reg.L, 2);
            cycle(8);
            break;
        case 0x56:  // Test bit 2 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 2);
            cycle(16);
            break;
        case 0x5f:  // Test bit 3 in register A and set flags accordingly
            bit_test(reg.A, 3);
            cycle(8);
            break;
        case 0x58:  // Test bit 3 in register B and set flags accordingly
            bit_test(reg.B, 3);
            cycle(8);
            break;
        case 0x59:  // Test bit 3 in register C and set flags accordingly
            bit_test(reg.C, 3);
            cycle(8);
            break;
        case 0x5a:  // Test bit 3 in register D and set flags accordingly
            bit_test(reg.E, 3);
            cycle(8);
            break;
        case 0x5b:  // Test bit 3 in register E and set flags accordingly
            bit_test(reg.D, 3);
            cycle(8);
            break;
        case 0x5c:  // Test bit 3 in register H and set flags accordingly
            bit_test(reg.H, 3);
            cycle(8);
            break;
        case 0x5d:  // Test bit 3 in register L and set flags accordingly
            bit_test(reg.L, 3);
            cycle(8);
            break;
        case 0x5e:  // Test bit 3 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 3);
            cycle(16);
            break;
        case 0x67:  // Test bit 4 in register A and set flags accordingly
            bit_test(reg.A, 4);
            cycle(8);
            break;
        case 0x60:  // Test bit 4 in register B and set flags accordingly
            bit_test(reg.B, 4);
            cycle(8);
            break;
        case 0x61:  // Test bit 4 in register C and set flags accordingly
            bit_test(reg.C, 4);
            cycle(8);
            break;
        case 0x62:  // Test bit 4 in register D and set flags accordingly
            bit_test(reg.D, 4);
            cycle(8);
            break;
        case 0x63:  // Test bit 4 in register E and set flags accordingly
            bit_test(reg.E, 4);
            cycle(8);
            break;
        case 0x64:  // Test bit 4 in register H and set flags accordingly
            bit_test(reg.H, 4);
            cycle(8);
            break;
        case 0x65:  // Test bit 4 in register L and set flags accordingly
            bit_test(reg.L, 4);
            cycle(8);
            break;
        case 0x66:  // Test bit 4 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 4);
            cycle(16);
            break;
        case 0x6f:  // Test bit 5 in register A and set flags accordingly
            bit_test(reg.A, 5);
            cycle(8);
            break;
        case 0x68:  // Test bit 5 in register B and set flags accordingly
            bit_test(reg.B, 5);
            cycle(8);
            break;
        case 0x69:  // Test bit 5 in register C and set flags accordingly
            bit_test(reg.C, 5);
            cycle(8);
            break;
        case 0x6a:  // Test bit 5 in register D and set flags accordingly
            bit_test(reg.D, 5);
            cycle(8);
            break;
        case 0x6b:  // Test bit 5 in register E and set flags accordingly
            bit_test(reg.E, 5);
            cycle(8);
            break;
        case 0x6c:  // Test bit 5 in register H and set flags accordingly
            bit_test(reg.H, 5);
            cycle(8);
            break;
        case 0x6d:  // Test bit 5 in register L and set flags accordingly
            bit_test(reg.L, 5);
            cycle(8);
            break;
        case 0x6e:  // Test bit 5 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 5);
            cycle(16);
            break;
        case 0x77:  // Test bit 6 in register A and set flags accordingly
            bit_test(reg.A, 6);
            cycle(8);
            break;
        case 0x70:  // Test bit 6 in register B and set flags accordingly
            bit_test(reg.B, 6);
            cycle(8);
            break;
        case 0x71:  // Test bit 6 in register C and set flags accordingly
            bit_test(reg.C, 6);
            cycle(8);
            break;
        case 0x72:  // Test bit 6 in register D and set flags accordingly
            bit_test(reg.D, 6);
            cycle(8);
            break;
        case 0x73:  // Test bit 6 in register E and set flags accordingly
            bit_test(reg.E, 6);
            cycle(8);
            break;
        case 0x74:  // Test bit 6 in register H and set flags accordingly
            bit_test(reg.H, 6);
            cycle(8);
            break;
        case 0x75:  // Test bit 6 in register L and set flags accordingly
            bit_test(reg.L, 6);
            cycle(8);
            break;
        case 0x76:  // Test bit 6 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 6);
            cycle(16);
            break;
        case 0x7f:  // Test bit 7 in register A and set flags accordingly
            bit_test(reg.A, 7);
            cycle(8);
            break;
        case 0x78:  // Test bit 7 in register B and set flags accordingly
            bit_test(reg.B, 7);
            cycle(8);
            break;
        case 0x79:  // Test bit 7 in register C and set flags accordingly
            bit_test(reg.C, 7);
            cycle(8);
            break;
        case 0x7a:  // Test bit 7 in register D and set flags accordingly
            bit_test(reg.D, 7);
            cycle(8);
            break;
        case 0x7b:  // Test bit 7 in register E and set flags accordingly
            bit_test(reg.E, 7);
            cycle(8);
            break;
        case 0x7c:  // Test bit 7 in register H and set flags accordingly
            bit_test(reg.H, 7);
            cycle(8);
            break;
        case 0x7d:  // Test bit 7 in register L and set flags accordingly
            bit_test(reg.L, 7);
            cycle(8);
            break;
        case 0x7e:  // Test bit 7 in byte at (HL) and set flags accordingly
            bit_test(reg.HL, 7);
            cycle(16);
            break;
        case 0xc7:  // Set bit 0 in register A, flags not affected
            bit_set(reg.A, 0);
            cycle(8);
            break;
        case 0xc0:  // Set bit 0 in register B, flags not affected
            bit_set(reg.B, 0);
            cycle(8);
            break;
        case 0xc1:  // Set bit 0 in register C, flags not affected
            bit_set(reg.C, 0);
            cycle(8);
            break;
        case 0xc2:  // Set bit 0 in register D, flags not affectedy
            bit_set(reg.D, 0);
            cycle(8);
            break;
        case 0xc3:  // Set bit 0 in register E, flags not affected
            bit_set(reg.E, 0);
            cycle(8);
            break;
        case 0xc4:  // Set bit 0 in register H, flags not affected
            bit_set(reg.H, 0);
            cycle(8);
            break;
        case 0xc5:  // Set bit 0 in register L, flags not affected
            bit_set(reg.L, 0);
            cycle(8);
            break;
        case 0xc6:  // Set bit 0 in byte at (HL), flags not affected
            bit_set(reg.HL, 0);
            cycle(16);
            break;
        case 0xcf:  // Set bit 1 in register A, flags not affected
            bit_set(reg.A, 1);
            cycle(8);
            break;
        case 0xc8:  // Set bit 1 in register B, flags not affected
            bit_set(reg.B, 1);
            cycle(8);
            break;
        case 0xc9:  // Set bit 1 in register C, flags not affected
            bit_set(reg.C, 1);
            cycle(8);
            break;
        case 0xca:  // Set bit 1 in register D, flags not affectedy
            bit_set(reg.D, 1);
            cycle(8);
            break;
        case 0xcb:  // Set bit 1 in register E, flags not affected
            bit_set(reg.E, 1);
            cycle(8);
            break;
        case 0xcc:  // Set bit 1 in register H, flags not affected
            bit_set(reg.H, 1);
            cycle(8);
            break;
        case 0xcd:  // Set bit 1 in register L, flags not affected
            bit_set(reg.L, 1);
            cycle(8);
            break;
        case 0xce:  // Set bit 1 in byte at (HL), flags not affected
            bit_set(reg.HL, 1);
            cycle(16);
            break;
        case 0xd7:  // Set bit 2 in register A, flags not affected
            bit_set(reg.A, 2);
            cycle(8);
            break;
        case 0xd0:  // Set bit 2 in register B, flags not affected
            bit_set(reg.B, 2);
            cycle(8);
            break;
        case 0xd1:  // Set bit 2 in register C, flags not affected
            bit_set(reg.C, 2);
            cycle(8);
            break;
        case 0xd2:  // Set bit 2 in register D, flags not affectedy
            bit_set(reg.D, 2);
            cycle(8);
            break;
        case 0xd3:  // Set bit 2 in register E, flags not affected
            bit_set(reg.E, 2);
            cycle(8);
            break;
        case 0xd4:  // Set bit 2 in register H, flags not affected
            bit_set(reg.H, 2);
            cycle(8);
            break;
        case 0xd5:  // Set bit 2 in register L, flags not affected
            bit_set(reg.L, 2);
            cycle(8);
            break;
        case 0xd6:  // Set bit 2 in byte at (HL), flags not affected
            bit_set(reg.HL, 2);
            cycle(16);
            break;
        case 0xdf:  // Set bit 3 in register A, flags not affected
            bit_set(reg.A, 3);
            cycle(8);
            break;
        case 0xd8:  // Set bit 3 in register B, flags not affected
            bit_set(reg.B, 3);
            cycle(8);
            break;
        case 0xd9:  // Set bit 3 in register C, flags not affected
            bit_set(reg.C, 3);
            cycle(8);
            break;
        case 0xda:  // Set bit 3 in register D, flags not affectedy
            bit_set(reg.D, 3);
            cycle(8);
            break;
        case 0xdb:  // Set bit 3 in register E, flags not affected
            bit_set(reg.E, 3);
            cycle(8);
            break;
        case 0xdc:  // Set bit 3 in register H, flags not affected
            bit_set(reg.H, 3);
            cycle(8);
            break;
        case 0xdd:  // Set bit 3 in register L, flags not affected
            bit_set(reg.L, 3);
            cycle(8);
            break;
        case 0xde:  // Set bit 3 in byte at (HL), flags not affected
            bit_set(reg.HL, 3);
            cycle(16);
            break;
        case 0xe7:  // Set bit 4 in register A, flags not affected
            bit_set(reg.A, 4);
            cycle(8);
            break;
        case 0xe0:  // Set bit 4 in register B, flags not affected
            bit_set(reg.B, 4);
            cycle(8);
            break;
        case 0xe1:  // Set bit 4 in register C, flags not affected
            bit_set(reg.C, 4);
            cycle(8);
            break;
        case 0xe2:  // Set bit 4 in register D, flags not affectedy
            bit_set(reg.D, 4);
            cycle(8);
            break;
        case 0xe3:  // Set bit 4 in register E, flags not affected
            bit_set(reg.E, 4);
            cycle(8);
            break;
        case 0xe4:  // Set bit 4 in register H, flags not affected
            bit_set(reg.H, 4);
            cycle(8);
            break;
        case 0xe5:  // Set bit 4 in register L, flags not affected
            bit_set(reg.L, 4);
            cycle(8);
            break;
        case 0xe6:  // Set bit 4 in byte at (HL), flags not affected
            bit_set(reg.HL, 4);
            cycle(16);
            break;
        case 0xef:  // Set bit 5 in register A, flags not affected
            bit_set(reg.A, 5);
            cycle(8);
            break;
        case 0xe8:  // Set bit 5 in register B, flags not affected
            bit_set(reg.B, 5);
            cycle(8);
            break;
        case 0xe9:  // Set bit 5 in register C, flags not affected
            bit_set(reg.C, 5);
            cycle(8);
            break;
        case 0xea:  // Set bit 5 in register D, flags not affectedy
            bit_set(reg.D, 5);
            cycle(8);
            break;
        case 0xeb:  // Set bit 5 in register E, flags not affected
            bit_set(reg.E, 5);
            cycle(8);
            break;
        case 0xec:  // Set bit 5 in register H, flags not affected
            bit_set(reg.H, 5);
            cycle(8);
            break;
        case 0xed:  // Set bit 5 in register L, flags not affected
            bit_set(reg.L, 5);
            cycle(8);
            break;
        case 0xee:  // Set bit 5 in byte at (HL), flags not affected
            bit_set(reg.HL, 5);
            cycle(16);
            break;
       case 0xf7:  // Set bit 6 in register A, flags not affected
            bit_set(reg.A, 6);
            cycle(8);
            break;
        case 0xf0:  // Set bit 6 in register B, flags not affected
            bit_set(reg.B, 6);
            cycle(8);
            break;
        case 0xf1:  // Set bit 6 in register C, flags not affected
            bit_set(reg.C, 6);
            cycle(8);
            break;
        case 0xf2:  // Set bit 6 in register D, flags not affectedy
            bit_set(reg.D, 6);
            cycle(8);
            break;
        case 0xf3:  // Set bit 6 in register E, flags not affected
            bit_set(reg.E, 6);
            cycle(8);
            break;
        case 0xf4:  // Set bit 6 in register H, flags not affected
            bit_set(reg.H, 6);
            cycle(8);
            break;
        case 0xf5:  // Set bit 6 in register L, flags not affected
            bit_set(reg.L, 6);
            cycle(8);
            break;
        case 0xf6:  // Set bit 6 in byte at (HL), flags not affected
            bit_set(reg.HL, 6);
            cycle(16);
            break;
        case 0xff:  // Set bit 7 in register A, flags not affected
            bit_set(reg.A, 7);
            cycle(8);
            break;
        case 0xf8:  // Set bit 7 in register B, flags not affected
            bit_set(reg.B, 7);
            cycle(8);
            break;
        case 0xf9:  // Set bit 7 in register C, flags not affected
            bit_set(reg.C, 7);
            cycle(8);
            break;
        case 0xfa:  // Set bit 7 in register D, flags not affectedy
            bit_set(reg.D, 7);
            cycle(8);
            break;
        case 0xfb:  // Set bit 7 in register E, flags not affected
            bit_set(reg.E, 7);
            cycle(8);
            break;
        case 0xfc:  // Set bit 7 in register H, flags not affected
            bit_set(reg.H, 7);
            cycle(8);
            break;
        case 0xfd:  // Set bit 7 in register L, flags not affected
            bit_set(reg.L, 7);
            cycle(8);
            break;
        case 0xfe:  // Set bit 7 in byte at (HL), flags not affected
            bit_set(reg.HL, 7);
            cycle(16);
            break;
        case 0x87:  // Reset bit 0 in register A, flags not affected
            bit_reset(reg.A, 0);
            cycle(8);
            break;
        case 0x80:  // Reset bit 0 in register B, flags not affected
            bit_reset(reg.B, 0);
            cycle(8);
            break;
        case 0x81:  // Reset bit 0 in register C, flags not affected
            bit_reset(reg.C, 0);
            cycle(8);
            break;
        case 0x82:  // Reset bit 0 in register D, flags not affectedy
            bit_reset(reg.D, 0);
            cycle(8);
            break;
        case 0x83:  // Reset bit 0 in register E, flags not affected
            bit_reset(reg.E, 0);
            cycle(8);
            break;
        case 0x84:  // Reset bit 0 in register H, flags not affected
            bit_reset(reg.H, 0);
            cycle(8);
            break;
        case 0x85:  // Reset bit 0 in register L, flags not affected
            bit_reset(reg.L, 0);
            cycle(8);
            break;
        case 0x86:  // Reset bit 0 in byte at (HL), flags not affected
            bit_reset(reg.HL, 0);
            cycle(16);
            break;
        case 0x8f:  // Reset bit 1 in register A, flags not affected
            bit_reset(reg.A, 1);
            cycle(8);
            break;
        case 0x88:  // Reset bit 1 in register B, flags not affected
            bit_reset(reg.B, 1);
            cycle(8);
            break;
        case 0x89:  // Reset bit 1 in register C, flags not affected
            bit_reset(reg.C, 1);
            cycle(8);
            break;
        case 0x8a:  // Reset bit 1 in register D, flags not affectedy
            bit_reset(reg.D, 1);
            cycle(8);
            break;
        case 0x8b:  // Reset bit 1 in register E, flags not affected
            bit_reset(reg.E, 1);
            cycle(8);
            break;
        case 0x8c:  // Reset bit 1 in register H, flags not affected
            bit_reset(reg.H, 1);
            cycle(8);
            break;
        case 0x8d:  // Reset bit 1 in register L, flags not affected
            bit_reset(reg.L, 1);
            cycle(8);
            break;
        case 0x8e:  // Reset bit 1 in byte at (HL), flags not affected
            bit_reset(reg.HL, 1);
            cycle(16);
            break;
        case 0x97:  // Reset bit 2 in register A, flags not affected
            bit_reset(reg.A, 2);
            cycle(8);
            break;
        case 0x90:  // Reset bit 2 in register B, flags not affected
            bit_reset(reg.B, 2);
            cycle(8);
            break;
        case 0x91:  // Reset bit 2 in register C, flags not affected
            bit_reset(reg.C, 2);
            cycle(8);
            break;
        case 0x92:  // Reset bit 2 in register D, flags not affectedy
            bit_reset(reg.D, 2);
            cycle(8);
            break;
        case 0x93:  // Reset bit 2 in register E, flags not affected
            bit_reset(reg.E, 2);
            cycle(8);
            break;
        case 0x94:  // Reset bit 2 in register H, flags not affected
            bit_reset(reg.H, 2);
            cycle(8);
            break;
        case 0x95:  // Reset bit 2 in register L, flags not affected
            bit_reset(reg.L, 2);
            cycle(8);
            break;
        case 0x96:  // Reset bit 2 in byte at (HL), flags not affected
            bit_reset(reg.HL, 2);
            cycle(16);
            break;
        case 0x9f:  // Reset bit 3 in register A, flags not affected
            bit_reset(reg.A, 3);
            cycle(8);
            break;
        case 0x98:  // Reset bit 3 in register B, flags not affected
            bit_reset(reg.B, 3);
            cycle(8);
            break;
        case 0x99:  // Reset bit 3 in register C, flags not affected
            bit_reset(reg.C, 3);
            cycle(8);
            break;
        case 0x9a:  // Reset bit 3 in register D, flags not affectedy
            bit_reset(reg.D, 3);
            cycle(8);
            break;
        case 0x9b:  // Reset bit 3 in register E, flags not affected
            bit_reset(reg.E, 3);
            cycle(8);
            break;
        case 0x9c:  // Reset bit 3 in register H, flags not affected
            bit_reset(reg.H, 3);
            cycle(8);
            break;
        case 0x9d:  // Reset bit 3 in register L, flags not affected
            bit_reset(reg.L, 3);
            cycle(8);
            break;
        case 0x9e:  // Reset bit 3 in byte at (HL), flags not affected
            bit_reset(reg.HL, 3);
            cycle(16);
            break;
        case 0xa7:  // Reset bit 4 in register A, flags not affected
            bit_reset(reg.A, 4);
            cycle(8);
            break;
        case 0xa0:  // Reset bit 4 in register B, flags not affected
            bit_reset(reg.B, 4);
            cycle(8);
            break;
        case 0xa1:  // Reset bit 4 in register C, flags not affected
            bit_reset(reg.C, 4);
            cycle(8);
            break;
        case 0xa2:  // Reset bit 4 in register D, flags not affectedy
            bit_reset(reg.D, 4);
            cycle(8);
            break;
        case 0xa3:  // Reset bit 4 in register E, flags not affected
            bit_reset(reg.E, 4);
            cycle(8);
            break;
        case 0xa4:  // Reset bit 4 in register H, flags not affected
            bit_reset(reg.H, 4);
            cycle(8);
            break;
        case 0xa5:  // Reset bit 4 in register L, flags not affected
            bit_reset(reg.L, 4);
            cycle(8);
            break;
        case 0xa6:  // Reset bit 4 in byte at (HL), flags not affected
            bit_reset(reg.HL, 4);
            cycle(16);
            break;
        case 0xaf:  // Reset bit 5 in register A, flags not affected
            bit_reset(reg.A, 5);
            cycle(8);
            break;
        case 0xa8:  // Reset bit 5 in register B, flags not affected
            bit_reset(reg.B, 5);
            cycle(8);
            break;
        case 0xa9:  // Reset bit 5 in register C, flags not affected
            bit_reset(reg.C, 5);
            cycle(8);
            break;
        case 0xaa:  // Reset bit 5 in register D, flags not affectedy
            bit_reset(reg.D, 5);
            cycle(8);
            break;
        case 0xab:  // Reset bit 5 in register E, flags not affected
            bit_reset(reg.E, 5);
            cycle(8);
            break;
        case 0xac:  // Reset bit 5 in register H, flags not affected
            bit_reset(reg.H, 5);
            cycle(8);
            break;
        case 0xad:  // Reset bit 5 in register L, flags not affected
            bit_reset(reg.L, 5);
            cycle(8);
            break;
        case 0xae:  // Reset bit 5 in byte at (HL), flags not affected
            bit_reset(reg.HL, 5);
            cycle(16);
            break;
        case 0xb7:  // Reset bit 6 in register A, flags not affected
            bit_reset(reg.A, 6);
            cycle(8);
            break;
        case 0xb0:  // Reset bit 6 in register B, flags not affected
            bit_reset(reg.B, 6);
            cycle(8);
            break;
        case 0xb1:  // Reset bit 6 in register C, flags not affected
            bit_reset(reg.C, 6);
            cycle(8);
            break;
        case 0xb2:  // Reset bit 6 in register D, flags not affectedy
            bit_reset(reg.D, 6);
            cycle(8);
            break;
        case 0xb3:  // Reset bit 6 in register E, flags not affected
            bit_reset(reg.E, 6);
            cycle(8);
            break;
        case 0xb4:  // Reset bit 6 in register H, flags not affected
            bit_reset(reg.H, 6);
            cycle(8);
            break;
        case 0xb5:  // Reset bit 6 in register L, flags not affected
            bit_reset(reg.L, 6);
            cycle(8);
            break;
        case 0xb6:  // Reset bit 6 in byte at (HL), flags not affected
            bit_reset(reg.HL, 6);
            cycle(16);
            break;
        case 0xbf:  // Reset bit 7 in register A, flags not affected
            bit_reset(reg.A, 7);
            cycle(8);
            break;
        case 0xb8:  // Reset bit 7 in register B, flags not affected
            bit_reset(reg.B, 7);
            cycle(8);
            break;
        case 0xb9:  // Reset bit 7 in register C, flags not affected
            bit_reset(reg.C, 7);
            cycle(8);
            break;
        case 0xba:  // Reset bit 7 in register D, flags not affectedy
            bit_reset(reg.D, 7);
            cycle(8);
            break;
        case 0xbb:  // Reset bit 7 in register E, flags not affected
            bit_reset(reg.E, 7);
            cycle(8);
            break;
        case 0xbc:  // Reset bit 7 in register H, flags not affected
            bit_reset(reg.H, 7);
            cycle(8);
            break;
        case 0xbd:  // Reset bit 7 in register L, flags not affected
            bit_reset(reg.L, 7);
            cycle(8);
            break;
        case 0xbe:  // Reset bit 7 in byte at (HL), flags not affected
            bit_reset(reg.HL, 7);
            cycle(16);
            break;
        
        default:
            std::cerr << "[ERROR]: Cannot find opcode for CB instruction! : " << immediate << std::endl;
    }
}
