#include "cpu.hpp"

void CPU::init(RAM &ram)
{
    this->ram = ram;

    reg.PC = 0x0100 - 1;
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
    case IN_NOP:
        break;
    case IN_PUSH:
        reg.SP--;
        ram.writeU8(reg.SP--, (u8)((lookupRegU16(inst.op01) & 0xFF00) >> 8));
        ram.writeU8(reg.SP, (u8)(lookupRegU16(inst.op02) & 0x00FF));
        break;
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
    case IN_POP:
        lookupRegU16(inst.op01) = u8Tou16(ram.readU8(reg.SP++), ram.readU8(reg.SP++));
        break;

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

    case IN_CCF:
        setFlag(false, ADDSUB);
        setFlag(false, HALF);
        if(BIT(reg.F, 4) == 1) {
            setFlag(false, CARRY);
        } else {
            setFlag(true, CARRY);
        }
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

    
    
    case IN_NONE:
    default:
        std::cerr << "[ERROR]: Cannot find MNEMONIC for instruction! " << std::endl;

    }
    //cycle(inst.cycles);
    checkAffectedFlags(result, inst.affectFlags, carry);
    reg.PC++;
}               
