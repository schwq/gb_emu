#include "cpu.hpp"

void CPU::InitializeCPU(RAM &ReferenceRAM)
{
    this->MainRAM = ReferenceRAM;

    RegisterSet.PC = 0x0100;
    RegisterSet.AF = 0x01B0;
    RegisterSet.BC = 0x0013;
    RegisterSet.DE = 0x00D8;
    RegisterSet.HL = 0x014D;
}

#ifdef DEBUG
void CPU::DebugCPU(instruction InstructionExecuted) {
    std::cout << "STATE: Reg_A: " << RegisterSet.A << " / Reg_B: " << RegisterSet.B << " / Reg_C: " << RegisterSet.C << " / Reg_D: " << RegisterSet.D << " / Reg_E: " << RegisterSet.E << " / Reg_F: " << RegisterSet.F << " / Reg_SP: " << RegisterSet.SP << " / Reg_H: " << RegisterSet.H << " / Reg_L: " << RegisterSet.L << "" << NEWLINE;
    std::cout << "PC: " << std::hex << RegisterSet.PC << NEWLINE;
    std::cout << "FLAGS: Carry: " << GetFlagInteger(CARRY) << " / AddSub: " << GetFlagInteger(ADDSUB) << " / Half: " << GetFlagInteger(HALF) << " / Zero: " << GetFlagInteger(ZERO) << NEWLINE;
    std::cout << "Instruction: " << DebugLookUpMnemonic(InstructionExecuted.InstructionMnemonic) << " / Addr_Mode: " << DebugLookUpAddressMode(InstructionExecuted.InstructionAddressMode) << " / Op01: " << DebugLookUpOperand(InstructionExecuted.InstructionOperand01) << " / Op02: " << DebugLookUpOperand(InstructionExecuted.InstructionOperand02) << " / Flags: " << InstructionExecuted.InstructionAffectFlags << " / RST_Opcode: " << InstructionExecuted.InstructionRSTOpcode << " / Cycles: " << InstructionExecuted.InstructionCycles << NEWLINE;
    std::cout << "______________________" << NEWLINE;
} 
#endif

void CPU::ExecuteCPU() {
    instruction Instruction = SearchInstructionByOpcode(RegisterSet.PC);
    ExecuteInstruction(Instruction);
    DebugCPU(Instruction);
}

void CPU::CycleCPU(int Cycles) {
    return;        
}

void CPU::SetFlag(bool Value, FLAGS FlagToSetValue)
{
    switch (FlagToSetValue)
    {
    case ZERO:
        BIT_SET(RegisterSet.F, 7, (int)Value);
        break;
    case ADDSUB:
        BIT_SET(RegisterSet.F, 6, (int)Value);
        break;
    case HALF:
        BIT_SET(RegisterSet.F, 5, (int)Value);
        break;
    case CARRY:
        BIT_SET(RegisterSet.F, 4, (int)Value);
        break;
    default:
        std::cerr << "[ERROR]: setFlag could not find case for FLAG set" << NEWLINE;
    }
}

int CPU::GetFlagInteger(FLAGS FlagToGet) {
    switch(FlagToGet) {
        case ZERO:
            return BIT(RegisterSet.F, 7);
        case ADDSUB:
            return BIT(RegisterSet.F, 6);
        case HALF:
            return BIT(RegisterSet.F, 5);
        case CARRY:
            return BIT(RegisterSet.F, 4);
        default:   
            std::cerr << "[ERROR]: Cannot find flag in function getFlag()!" << std::endl;
            return -1;
    }
}

void CPU::CheckAffectedFlags(u16 OperationResult, const char* AffectFlags, u16 Carry) {
    if(AffectFlags == "----") return; 
    switch(AffectFlags[0]) {
        case '-':
            break;
        case 'Z':
            if(OperationResult == 0) {
                SetFlag(1, ZERO);
                break;
            } else {
                SetFlag(0, ZERO);
                break;
            }
        case '1':
            SetFlag(1, ZERO);
            break;
        case '0': 
            SetFlag(0, ZERO);
            break;
    }

    switch(AffectFlags[1]) {
        case '-':
            break;
        case '1':
            SetFlag(1, ADDSUB);
            break;
        case '0':
            SetFlag(0, ADDSUB);
            break;
    }

    switch(AffectFlags[2]) {
        case '-': 
            break;
        case 'H':
            if(BIT(Carry, 3)) {
                SetFlag(1, HALF);
                break;
            } else {
                SetFlag(0, HALF);
                break;
            }
        case '1':
            SetFlag(1, HALF);
            break;
        case '0':
            SetFlag(0, HALF);
            break;
    }

    switch(AffectFlags[3]) {
        case '-': 
            break;
        case 'H':
            if(BIT(Carry, 7)) {
                SetFlag(1, CARRY);
                break;
            } else {
                SetFlag(0, CARRY);
                break;
            }
        case '1':
            SetFlag(1, CARRY);
            break;
        case '0':
            SetFlag(0, CARRY);
            break;
    }
}

bool CPU::CheckConditionForInstruction(const char* Condition) {
    if(Condition == "CC_NZ") {
        return (GetFlagInteger(ADDSUB) == 1 && GetFlagInteger(ZERO) == 1) ? true : false;
    }
    if(Condition == "CC_Z") {
        return GetFlagInteger(ZERO) == 1 ? true : false;
    }
    if(Condition == "CC_NC") {
        return (GetFlagInteger(ADDSUB) == 1 && GetFlagInteger(CARRY) == 1) ? true : false;
    }
    if(Condition == "CC_C") {
        return GetFlagInteger(CARRY) == 1 ? true : false;     
    }
    std::cerr << "[ERROR]: Cannot find conditional_instruction_cc pattern! : " << Condition << std::endl;
    return NULL;
}

u16 CPU::U8ToU16Number(u8 LowByte, u8 HighByte)
{
    return (HighByte << 8) | LowByte;
}

u8 &CPU::LookUpRegisterU8(operand OperandToLookUp) {
    switch (OperandToLookUp) {
        case reg_a:
            return RegisterSet.A;
        case reg_b:
            return RegisterSet.B;
        case reg_c:
            return RegisterSet.C;
        case reg_d:
            return RegisterSet.D;
        case reg_e:
            return RegisterSet.E;
        case reg_h:
            return RegisterSet.H;
        case reg_l:
            return RegisterSet.L;
        case immediate_u8:
        {
            u8 addr = MainRAM.ReadU8Data(RegisterSet.PC++);
            return addr;
        }
        default:
        {
            std::cout << "[ERROR]: Cannot find U8 register_lookup: " << OperandToLookUp << NEWLINE;
            u8 error = 0x00;
            return error;
        }
    }
}

u16 &CPU::LookUpRegisterU16(operand OperandToLookUp)
{
    switch (OperandToLookUp)
    {
    case reg_bc:
        return RegisterSet.BC;
    case reg_hl:
        return RegisterSet.HL;
    case reg_sp:
        return RegisterSet.SP;
    case reg_de:
        return RegisterSet.DE;
    case reg_af:
        return RegisterSet.AF;
    case immediate_u16:
    {
        u16 addr = MainRAM.ReadU16Data(RegisterSet.PC++);
        return addr;
    }
    default:
        std::cout << "[ERROR]: Cannot find U16 register_lookup: " << OperandToLookUp << NEWLINE;
    }
}

instruction CPU::SearchInstructionByOpcode(u8 SearchOpcode)
{
    return InstructionSet[SearchOpcode];
}

template <typename T>
register_type CPU::CheckRegisterType(T RegisterToCheck)
{
    if (sizeof(RegisterToCheck) == sizeof(char)){
        return U8REG;
    } else if(sizeof(RegisterToCheck) == sizeof(short)){
        return U16REG;
    } else {
        std::cerr << "[ERROR]: NONE_REG in checkRegisterType was been thown!" << std::endl;
        return NONE_REG;
    }
}

void CPU::BitOperationRL(u8& Register, bool Carry) {
    u8 Bit7 = (Register & 0x80) >> 7;
    u8 value = Register << 1;

    if(Carry) {
        Register = value + Bit7;
    } else {
        Register = value + GetFlagInteger(CARRY);
    }

   SetFlag(0, HALF);
   SetFlag(0, ADDSUB);
   SetFlag((Register == 0), ZERO);
   SetFlag((bool)Bit7, CARRY);
}

void CPU::BitOperationRR(u8& Register, bool Carry) {
    u8 Bit0 = Register & 0x01;
    u8 value = Register >> 1;

    if(Carry) {
        Register = value + (Bit0 << 7);
    } else {
        Register = value + (GetFlagInteger(CARRY) << 7);
    }

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((Register == 0), ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationSwap(u8& Register) {
    Register = (Register >> 4) | ((Register & 0xF) << 4);

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((Register == 0), ZERO);
    SetFlag(0, CARRY);
    
}
 
void CPU::BitOperationTest(u8& Register, int Bit) {
    u8 value = (Register >> Bit) & 1;
    
    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((value == 0), ZERO);
}

void CPU::BitOperationReset(u8& Register, int Bit) {
    u8 BitMask = 1 << Bit;
    u8 value = Register & ~BitMask;
    Register = value;
}

void CPU::BitOperationSLA(u8& Register) {
    u8 Bit7 = (Register & 0x80) >> 7;
    Register = Register << 1;

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((Register == 0), ZERO);
    SetFlag((bool)Bit7, CARRY);
}

void CPU::BitOperationSRL(u8& Register) {
    u8 Bit0 = Register & 0x01;
    Register = Register >> 1;

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((Register == 0), ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationSRA(u8& Register) {
    u8 Bit0 = Register & 0x01;
    u8 Bit7 = Register & 0x80;
    Register = (Register >> 1) + Bit7;

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((Register == 0), ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationRRA(bool Carry) {
    u8 Bit0 = RegisterSet.A & 0x01;
    u8 value = RegisterSet.A >> 1;

    if(Carry) {
        RegisterSet.A = value + (Bit0 << 7);
    } else {
        RegisterSet.A = value + (GetFlagInteger(CARRY) << 7);
    }

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag(0, ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationSET(u8& Register, int Bit) {
    u8 BitMask = 1 << Bit;
    u8 value = Register | BitMask;
    Register = value;
}

void CPU::BitOperationRL(u16 Register, bool Carry) {
    
    u8 Bit7 = (MainRAM.ReadU8Data(Register) & 0x80) >> 7;
    u8 value = MainRAM.ReadU8Data(Register) << 1;

    if(Carry) {
        MainRAM.WriteU8Data(Register, (value + Bit7));
    } else {
        MainRAM.WriteU8Data(Register, (value + GetFlagInteger(CARRY)));
    }

   SetFlag(0, HALF);
   SetFlag(0, ADDSUB);
   SetFlag((MainRAM.ReadU8Data(Register) == 0), ZERO);
   SetFlag((bool)Bit7, CARRY);
}

void CPU::BitOperationRR(u16 Register, bool Carry) {
    u8 Bit0 = MainRAM.ReadU8Data(Register) & 0x01;
    u8 value = MainRAM.ReadU8Data(Register) >> 1;

    if(Carry) {
        MainRAM.WriteU8Data(Register, value + (Bit0 << 7));
    } else {
        MainRAM.WriteU8Data(Register, value + (GetFlagInteger(CARRY) << 7));
    }

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((MainRAM.ReadU8Data(Register) == 0), ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationSwap(u16 Register) {
    MainRAM.WriteU8Data(Register, (MainRAM.ReadU8Data(Register) >> 4) | ((MainRAM.ReadU8Data(Register) & 0xF) << 4));

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((MainRAM.ReadU8Data(Register) == 0), ZERO);
    SetFlag(0, CARRY);
    
}
 
void CPU::BitOperationTest(u16 Register, int Bit) {
    u8 value = (MainRAM.ReadU8Data(Register) >> Bit) & 1;
    
    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((value == 0), ZERO);
}

void CPU::BitOperationReset(u16 Register, int Bit) {
    u8 BitMask = 1 << Bit;
    u8 value = MainRAM.ReadU8Data(Register) & ~BitMask;
    MainRAM.WriteU8Data(Register, value);
}

void CPU::BitOperationSLA(u16 Register) {
    u8 Bit7 = (MainRAM.ReadU8Data(Register) & 0x80) >> 7;
    MainRAM.WriteU8Data(Register, (Register << 1));

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((MainRAM.ReadU8Data(Register) == 0), ZERO);
    SetFlag((bool)Bit7, CARRY);
}

void CPU::BitOperationSRL(u16 Register) {
    u8 Bit0 = MainRAM.ReadU8Data(Register) & 0x01;
    MainRAM.WriteU8Data(Register, (Register >> 1));

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((MainRAM.ReadU8Data(Register) == 0), ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationSRA(u16 Register) {
    u8 Bit0 = MainRAM.ReadU8Data(Register) & 0x01;
    u8 Bit7 = MainRAM.ReadU8Data(Register) & 0x80;
    MainRAM.WriteU8Data(Register, (MainRAM.ReadU8Data(Register) >> 1) + Bit7);

    SetFlag(0, HALF);
    SetFlag(0, ADDSUB);
    SetFlag((MainRAM.ReadU8Data(Register) == 0), ZERO);
    SetFlag((bool)Bit0, CARRY);
}

void CPU::BitOperationSet(u16 Register, int Bit) {
    u8 BitMask = 1 << Bit;
    u8 value = MainRAM.ReadU8Data(Register) | BitMask;
    MainRAM.WriteU8Data(Register, value);
}

void CPU::ExecuteInstruction(instruction InstructionToExecute) {
    u8 Opcode = RegisterSet.PC;
    u16 OperationResult;
    u16 Carry;

    switch (InstructionToExecute.InstructionMnemonic)
    {
    case IN_LD:
        { 
            switch (InstructionToExecute.InstructionAddressMode) {
            case AM_MR_R:
                if (InstructionToExecute.InstructionOperand01 == reg_c && InstructionToExecute.InstructionOperand02 == reg_a)
                { // 0xE2
                    MainRAM.WriteU8Data(U8ToU16Number(RegisterSet.C, 0xFF), RegisterSet.A);
                    break;
                }

                MainRAM.WriteU8Data(LookUpRegisterU16(InstructionToExecute.InstructionOperand01), LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
                
            case AM_R_R:
                if (InstructionToExecute.InstructionOperand01 == reg_sp && InstructionToExecute.InstructionOperand02 == reg_hl)
                {
                    RegisterSet.SP = RegisterSet.HL;
                    break;
                }
                LookUpRegisterU8(InstructionToExecute.InstructionOperand01) = LookUpRegisterU8(InstructionToExecute.InstructionOperand02);
                break;
            case AM_R_MR:
                if (InstructionToExecute.InstructionOperand01 == reg_a && InstructionToExecute.InstructionOperand02 == reg_c)
                { // 0xF2
                    RegisterSet.A = MainRAM.ReadU8Data(U8ToU16Number(RegisterSet.C, 0xFF));
                    break;
                }
                LookUpRegisterU8(InstructionToExecute.InstructionOperand01) = MainRAM.ReadU8Data(LookUpRegisterU16(InstructionToExecute.InstructionOperand02));
                break;
            case AM_R_HLI:
                LookUpRegisterU8(InstructionToExecute.InstructionOperand01) = MainRAM.ReadU8Data(RegisterSet.HL++);
                break;
            case AM_R_HLD:
                LookUpRegisterU8(InstructionToExecute.InstructionOperand01) = MainRAM.ReadU8Data(RegisterSet.HL--);
                break;
            case AM_HLI_R:
                MainRAM.WriteU8Data(MainRAM.ReadU8Data(RegisterSet.HL++), LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
            case AM_HLD_R:
                MainRAM.WriteU8Data(MainRAM.ReadU8Data(RegisterSet.HL--), LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
            case AM_HL_SPR:
                {
                // config
                u8 SignedInteger = MainRAM.ReadU8Data(RegisterSet.PC++);
                u8 result = ~(RegisterSet.SP + SignedInteger) + 1;
                RegisterSet.HL = result;
                break;
                }
            case AM_MR_D8:
                {
                u8 ResultHold = MainRAM.ReadU8Data(RegisterSet.PC++);
                MainRAM.WriteU8Data(LookUpRegisterU16(InstructionToExecute.InstructionOperand01), ResultHold);
                break;
                }
            case AM_A16_R:
                {
                u16 ResultHold = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), MainRAM.ReadU8Data(RegisterSet.PC++));
                MainRAM.WriteU8Data(ResultHold, (u8)((RegisterSet.SP & 0xFF00) >> 8));
                MainRAM.WriteU8Data(ResultHold + 1, (u8)(RegisterSet.SP & 0x00FF));
                break;
                }
            case AM_R_A16:
                {
                u16 ResultHold = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), MainRAM.ReadU8Data(RegisterSet.PC++));
                LookUpRegisterU8(InstructionToExecute.InstructionOperand01) = MainRAM.ReadU8Data(ResultHold); // reg.A = ram.ReadU8Data(nn);
                break;
                }
            case AM_R_D16:
                LookUpRegisterU16(InstructionToExecute.InstructionOperand01) = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), MainRAM.ReadU8Data(RegisterSet.PC++));
                break; 
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_LD! : " << InstructionToExecute.InstructionAddressMode << std::endl;
            }
            break;
        }
    case IN_LDH:
        {
        if (InstructionToExecute.InstructionOperand01 == reg_a && InstructionToExecute.InstructionOperand02 == immediate_u8) { // 0xF0
            RegisterSet.A = MainRAM.ReadU8Data(U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), 0xFF));
            break;
            
        } else { // 0xE0
            MainRAM.WriteU8Data(U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), 0xFF), RegisterSet.A);
            break;
        }
        }
    case IN_DEC:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R:
            if(CheckRegisterType(InstructionToExecute.InstructionOperand01) == U8REG) {
                OperationResult, Carry = LookUpRegisterU8(InstructionToExecute.InstructionOperand01) -= 1;
                break;
            } else {
                OperationResult, Carry = LookUpRegisterU16(InstructionToExecute.InstructionOperand01) -= 1;
                break;
            }
            
            case AM_MR:
                OperationResult, Carry = MainRAM.ReadU8Data(RegisterSet.HL) - 1;
                MainRAM.WriteU8Data(RegisterSet.HL, OperationResult);
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_DEC! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        

    case IN_ADD:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                if(Opcode == 0x09 || Opcode == 0x19 || Opcode == 0x29 || Opcode == 0x39) {
                    Carry = (u16) (LookUpRegisterU16(InstructionToExecute.InstructionOperand01) += LookUpRegisterU16(InstructionToExecute.InstructionOperand02));
                    OperationResult = LookUpRegisterU16(InstructionToExecute.InstructionOperand01);
                    break;
                } else {
                    Carry = (u16) (LookUpRegisterU8(InstructionToExecute.InstructionOperand01) += LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                    OperationResult = (u16) LookUpRegisterU8(InstructionToExecute.InstructionOperand01);
                    break;
                }
            case AM_R_MR: 
                Carry = RegisterSet.A += MainRAM.ReadU8Data(RegisterSet.HL);
                OperationResult = (u16) RegisterSet.A;
                break;
            case AM_R_D8:
                if(Opcode == 0xC6) {
                Carry = RegisterSet.A += MainRAM.ReadU8Data(RegisterSet.PC++);
                OperationResult = (u16) RegisterSet.A;
                break;
                } else if (Opcode == 0xE8) {
                    // config
                } 
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_ADD! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }

    case IN_OR:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                OperationResult = RegisterSet.A |= LookUpRegisterU8(InstructionToExecute.InstructionOperand02);
                break;
            case AM_R_MR:
                OperationResult = RegisterSet.A |= MainRAM.ReadU8Data(RegisterSet.HL);
                break;
            case AM_R_D8:
                OperationResult = RegisterSet.A |= MainRAM.ReadU8Data(RegisterSet.PC++);
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_OR! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        break;

    case IN_XOR:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                OperationResult = (u16) (RegisterSet.A ^= LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
            case AM_R_MR:
                OperationResult = (u16) (RegisterSet.A ^= MainRAM.ReadU8Data(RegisterSet.HL));
                break;
            case AM_R_D8:
                OperationResult = (u16) (RegisterSet.A ^= MainRAM.ReadU8Data(RegisterSet.PC++));
                break;
            
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_XOR! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        break;

    case IN_AND:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                OperationResult = (u16) (RegisterSet.A &= LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
            case AM_R_MR:
                OperationResult = (u16) (RegisterSet.A &= MainRAM.ReadU8Data(RegisterSet.HL));
                break;
            case AM_R_D8:
                OperationResult = (u16) ( RegisterSet.A &= MainRAM.ReadU8Data(RegisterSet.PC++));
                break;
            
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_AND! : " << InstructionToExecute.InstructionAddressMode << std::endl;
    }
        break;

    case IN_JP:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_D16:
                
                if(Opcode == 0xC3) {
                    RegisterSet.PC = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), MainRAM.ReadU8Data(RegisterSet.PC++));
                    break;
                } 

            case AM_R:
                RegisterSet.PC = RegisterSet.HL;
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_JP! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        break;

    case IN_JR:
        {
        if(Opcode == 0x18) {
            char SignedInteger = static_cast<char>(MainRAM.ReadU8Data(RegisterSet.PC++));
            RegisterSet.PC += (SignedInteger + 1);
            break;
        } else {
            char SignedInteger = static_cast<char>(MainRAM.ReadU8Data(RegisterSet.PC++));
            if(CheckConditionForInstruction(InstructionToExecute.InstructionAffectFlags)) {
                RegisterSet.PC += (SignedInteger + 1);
            }
            break;
        }
        }
        break;

    case IN_CALL:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_D16:
            {
                if(Opcode == 0xC4 || Opcode == 0xCC || Opcode == 0xD4 || Opcode == 0xDC) {
                    u16 ResultHold = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), MainRAM.ReadU8Data(RegisterSet.PC++));
                    if(CheckConditionForInstruction(InstructionToExecute.InstructionAffectFlags)) {
                            RegisterSet.SP--;
                            MainRAM.WriteU8Data(RegisterSet.SP--, ((u8)(RegisterSet.PC & 0x00FF)));
                            MainRAM.WriteU8Data(RegisterSet.SP, ((u8)((RegisterSet.PC & 0xFF00) >> 8)));
                            RegisterSet.PC = ResultHold;
                            InstructionToExecute.InstructionCycles = 6;
                            break;
                        } 
                    RegisterSet.PC = ResultHold;
                    break;
                } else { // 0xCD
                    u16 ResultHold = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.PC++), MainRAM.ReadU8Data(RegisterSet.PC++));
                    RegisterSet.SP--;
                    MainRAM.WriteU8Data(RegisterSet.SP--, ((u8)(RegisterSet.PC & 0x00FF)));
                    MainRAM.WriteU8Data(RegisterSet.SP, ((u8)((RegisterSet.PC & 0xFF00) >> 8)));
                    RegisterSet.PC = ResultHold;
                }
                break;
            }
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_CALL! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }

    case IN_RET:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_NONE:
                RegisterSet.PC = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.SP++), MainRAM.ReadU8Data(RegisterSet.SP++));
                break;
            case AM_IMP:
                if(CheckConditionForInstruction(InstructionToExecute.InstructionAffectFlags)) {
                    RegisterSet.PC = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.SP++), MainRAM.ReadU8Data(RegisterSet.SP++));
                    InstructionToExecute.InstructionCycles = 5;
                }
                break;
            default: 
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_RET! : " << InstructionToExecute.InstructionAddressMode << std::endl;
         }
         break;

    case IN_RST:
        {   
            RegisterSet.SP--;
            MainRAM.WriteU8Data(RegisterSet.SP--, ((u8)(RegisterSet.PC & 0x00FF)));
            MainRAM.WriteU8Data(RegisterSet.SP, ((u8)((RegisterSet.PC & 0xFF00) >> 8)));
            RegisterSet.PC = U8ToU16Number(InstructionToExecute.InstructionRSTOpcode, 0x00);
        }
        break;

    case IN_CP:
        switch(InstructionToExecute.InstructionAddressMode) { 
            case AM_R_R:
                OperationResult, Carry = RegisterSet.A - LookUpRegisterU8(InstructionToExecute.InstructionOperand02);
                break;
            case AM_MR:
                OperationResult, Carry = RegisterSet.A - MainRAM.ReadU8Data(RegisterSet.HL);
                break;
            case AM_R_D8:
                OperationResult, Carry = RegisterSet.A - MainRAM.ReadU8Data(RegisterSet.PC++);
                break;
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_CP! : " << InstructionToExecute.InstructionAddressMode << std::endl; 
        }
        break;

    case IN_SUB:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                OperationResult, Carry = RegisterSet.A -= LookUpRegisterU8(InstructionToExecute.InstructionOperand02);
                break;
            case AM_R_MR:
                OperationResult, Carry = RegisterSet.A -= MainRAM.ReadU8Data(RegisterSet.HL);
                break;
            case AM_R_D8:
                OperationResult, Carry = RegisterSet.A -= MainRAM.ReadU8Data(RegisterSet.PC++);
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_SUB! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        break;

    case IN_ADC:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                OperationResult, Carry = RegisterSet.A += (GetFlagInteger(CARRY) + LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
            case AM_R_MR:
                OperationResult, Carry = RegisterSet.A += (GetFlagInteger(CARRY) + MainRAM.ReadU8Data(RegisterSet.HL));
                break;
            case AM_R_D8:
                OperationResult, Carry = RegisterSet.A += (GetFlagInteger(CARRY) + MainRAM.ReadU8Data(RegisterSet.PC++));
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_ADC! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        break;
    
    case IN_SBC:
        switch(InstructionToExecute.InstructionAddressMode) {
            case AM_R_R:
                OperationResult, Carry = RegisterSet.A -= (GetFlagInteger(CARRY) - LookUpRegisterU8(InstructionToExecute.InstructionOperand02));
                break;
            case AM_R_MR:
                OperationResult, Carry = RegisterSet.A -= (GetFlagInteger(CARRY) - MainRAM.ReadU8Data(RegisterSet.HL));
                break;
            case AM_R_D8:
                OperationResult, Carry = RegisterSet.A -= (GetFlagInteger(CARRY) - MainRAM.ReadU8Data(RegisterSet.PC++));
                break;
            case AM_NONE:
            default:
                std::cerr << "[ERROR]: Cannot find addr_mode for instruction IN_SBC! : " << InstructionToExecute.InstructionAddressMode << std::endl;
        }
        break;

    case IN_POP:
        LookUpRegisterU16(InstructionToExecute.InstructionOperand01) = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.SP++), MainRAM.ReadU8Data(RegisterSet.SP++));
        break;

    case IN_CCF:
        SetFlag(false, ADDSUB);
        SetFlag(false, HALF);
        if(BIT(RegisterSet.F, 4) == 1) {
            SetFlag(false, CARRY);
        } else {
            SetFlag(true, CARRY);
        }
        break;

    case IN_PUSH:
        RegisterSet.SP--;
        MainRAM.WriteU8Data(RegisterSet.SP--, (u8)((LookUpRegisterU16(InstructionToExecute.InstructionOperand01) & 0xFF00) >> 8));
        MainRAM.WriteU8Data(RegisterSet.SP, (u8)(LookUpRegisterU16(InstructionToExecute.InstructionOperand02) & 0x00FF));
        break;

    case IN_SCF:
        SetFlag(false, ADDSUB);
        SetFlag(false, HALF);
        SetFlag(true, CARRY);
        break;

    case IN_CPL:
        RegisterSet.A = ~RegisterSet.A;
        SetFlag(true, ADDSUB);
        SetFlag(true, HALF);
        break;

    case IN_DI:
        IME = 0;
        break;
    
    case IN_RETI:
        RegisterSet.PC = U8ToU16Number(MainRAM.ReadU8Data(RegisterSet.SP++), MainRAM.ReadU8Data(RegisterSet.SP++));
        IME = 1;
        break;

    case IN_EI:
        IMEScheduled = true;
        break;

    case IN_RRA:
        RegisterSet.A = ((RegisterSet.A >> 1) | (RegisterSet.A << (8 - 1)));
        break;
    
    case IN_RLA:
        RegisterSet.A = ((RegisterSet.A << 1) | (RegisterSet.A >> (8 - 1)));
        BitOperationSET(RegisterSet.A, GetFlagInteger(CARRY));
        break;

    case IN_NOP:
        break;

    case IN_CB:
        CBPrefixInstructionExecute(MainRAM.ReadU8Data(RegisterSet.PC++));
        break;
    
    case IN_NONE:
    default:
        std::cerr << "[ERROR]: Cannot find MNEMONIC for instruction! " << std::endl;

    }
    
    CycleCPU(InstructionToExecute.InstructionCycles);
    CheckAffectedFlags(OperationResult, InstructionToExecute.InstructionAffectFlags, Carry);
    RegisterSet.PC++;
}               

void CPU::CBPrefixInstructionExecute(const u8 CBInstructionToExecute) {
    switch(CBInstructionToExecute) {
        case 0x37:  // Swap upper and lower nibbles of register A, flags updated
            BitOperationSwap(RegisterSet.A);
            CycleCPU(8);
            break;
        case 0x30:  // Swap upper and lower nibbles of register B, flags updated
            BitOperationSwap(RegisterSet.B);
            CycleCPU(8);
            break;
        case 0x31:  // Swap upper and lower nibbles of register C, flags updated
            BitOperationSwap(RegisterSet.C);
            CycleCPU(8);
            break;
        case 0x32:  // Swap upper and lower nibbles of register D, flags updated
            BitOperationSwap(RegisterSet.D);
            CycleCPU(8);
            break;
        case 0x33:  // Swap upper and lower nibbles of register E, flags updated
            BitOperationSwap(RegisterSet.E);
            CycleCPU(8);
            break;
        case 0x34:  // Swap upper and lower nibbles of register H, flags updated
            BitOperationSwap(RegisterSet.H);
            CycleCPU(8);
            break;
        case 0x35:  // Swap upper and lower nibbles of register L, flags updated
            BitOperationSwap(RegisterSet.L);
            CycleCPU(8);
            break;
        case 0x36:  // Swap upper and lower nibbles of byte at (HL), flags updated
            BitOperationSwap(RegisterSet.HL);
            CycleCPU(16);
            break;
        case 0x07:  // Rotate contents of A register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.A, true); // Carry = true (Rotate left with Carry RLC A)
            CycleCPU(8);
            break;
        case 0x00:  // Rotate contents of B register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.B, true); // Carry = true (Rotate left with Carry RLC B)
            CycleCPU(8);
            break;
        case 0x01:  // Rotate contents of C register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.C, true); // Carry = true (Rotate left with Carry RLC C)
            CycleCPU(8);
            break;
        case 0x02:  // Rotate contents of D register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.D, true); // Carry = true (Rotate left with Carry RLC D)
            CycleCPU(8);
            break;
        case 0x03:  // Rotate contents of E register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.E, true); // Carry = true (Rotate left with Carry RLC E)
            CycleCPU(8);
            break;
        case 0x04:  // Rotate contents of H register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.H, true); // Carry = true (Rotate left with Carry RLC H)
            CycleCPU(8);
            break;
        case 0x05:  // Rotate contents of L register left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.L, true); // Carry = true (Rotate left with Carry RLC L)
            CycleCPU(8);
            break;
        case 0x06:  // Rotate contents of byte at (HL) left and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.HL, true); // Carry = true (Rotate left with Carry RLC L)
            CycleCPU(16);
            break;
        case 0x17:  // Rotate contents of A register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.A, false); // Carry = false (Rotate left RL A)
            CycleCPU(8);
            break;
        case 0x10:  // Rotate contents of B register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.B, false); // Carry = false (Rotate left with Carry RL B)
            CycleCPU(8);
            break;
        case 0x11:  // Rotate contents of C register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.C, false); // Carry = false (Rotate left with Carry RL C)
            CycleCPU(8);
            break;
        case 0x12:  // Rotate contents of D register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.D, false); // Carry = false (Rotate left with Carry RL D)
            CycleCPU(8);
            break;
        case 0x13:  // Rotate contents of E register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.E, false); // Carry = false (Rotate left with Carry RL E)
            CycleCPU(8);
            break;
        case 0x14:  // Rotate contents of H register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.H, false); // Carry = false (Rotate left with Carry RL H)
            CycleCPU(8);
            break;
        case 0x15:  // Rotate contents of L register left through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.L, false); // Carry = false (Rotate left with Carry RL L)
            CycleCPU(8);
            break;
        case 0x16:  // Rotate contents of byte at (HL) lef through Carry flag and store Bit 7 in CF, flags updated
            BitOperationRL(RegisterSet.HL, false); // Carry = false (Rotate left with Carry RL L)
            CycleCPU(16);
            break;
        case 0x0f:  // Rotate contents of A register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.A, true); // Carry = true (Rotate right with Carry RRC A)
            CycleCPU(8);
            break;
        case 0x08:  // Rotate contents of B register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.B, true); // Carry = true (Rotate right with Carry RRC B)
            CycleCPU(8);
            break;
        case 0x09:  // Rotate contents of C register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.C, true); // Carry = true (Rotate right with Carry RRC C)
            CycleCPU(8);
            break;
        case 0x0a:  // Rotate contents of D register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.D, true); // Carry = true (Rotate right with Carry RRC D)
            CycleCPU(8);
            break;
        case 0x0b:  // Rotate contents of E register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.E, true); // Carry = true (Rotate right with Carry RRC E)
            CycleCPU(8);
            break;
        case 0x0c:  // Rotate contents of H register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.H, true); // Carry = true (Rotate right with Carry RRC H)
            CycleCPU(8);
            break;
        case 0x0d:  // Rotate contents of L register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.L, true); // Carry = true (Rotate right with Carry RRC L)
            CycleCPU(8);
            break;
        case 0x0e:  // Rotate contents of byte at (HL) right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.HL, true); // Carry = true (Rotate right with Carry RRC (HL))
            CycleCPU(16);
            break;
        case 0x1f:  // Rotate contents of A register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.A, false); // Carry = false (Rotate right RR A)
            CycleCPU(8);
            break;
        case 0x18:  // Rotate contents of B register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.B, false); // Carry = false (Rotate right with Carry RR B)
            CycleCPU(8);
            break;
        case 0x19:  // Rotate contents of C register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.C, false); // Carry = false (Rotate right with Carry RR C)
            CycleCPU(8);
            break;
        case 0x1a:  // Rotate contents of D register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.D, false); // Carry = false (Rotate right with Carry RR D)
            CycleCPU(8);
            break;
        case 0x1b:  // Rotate contents of E register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.E, false); // Carry = false (Rotate right with Carry RR E)
            CycleCPU(8);
            break;
        case 0x1c:  // Rotate contents of H register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.H, false); // Carry = false (Rotate right with Carry RR H)
            CycleCPU(8);
            break;
        case 0x1d:  // Rotate contents of L register right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.L, false); // Carry = false (Rotate right with Carry RR L)
            CycleCPU(8);
            break;
        case 0x1e:  // Rotate contents of byte at (HL) right and store Bit 0 in CF, flags updated
            BitOperationRR(RegisterSet.HL, false); // Carry = false (Rotate right with Carry RR (HL))
            CycleCPU(16);
            break;
        case 0x27:  // Shift contents of register A left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.A);
            CycleCPU(8);
            break;
        case 0x20:  // Shift contents of register B left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.B);
            CycleCPU(8);
            break;
        case 0x21:  // Shift contents of register C left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.C);
            CycleCPU(8);
            break;
        case 0x22:  // Shift contents of register D left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.D);
            CycleCPU(8);
            break;
        case 0x23:  // Shift contents of register E left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.E);
            CycleCPU(8);
            break;
        case 0x24:  // Shift contents of register H left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.H);
            CycleCPU(8);
            break;
        case 0x25:  // Shift contents of register L left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.L);
            CycleCPU(8);
            break;
        case 0x26:  // Shift contents of byte at (HL) left and store Bit 7 in CF, Bit0 = 0, flags updated
            BitOperationSLA(RegisterSet.HL);
            CycleCPU(16);
            break;
        case 0x2f:  // Shift contents of register A right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.A);
            CycleCPU(8);
            break;
        case 0x28:  // Shift contents of register B right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.B);
            CycleCPU(8);
            break;
        case 0x29:  // Shift contents of register C right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.C);
            CycleCPU(8);
            break;
        case 0x2a:  // Shift contents of register D right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.D);
            CycleCPU(8);
            break;
        case 0x2b:  // Shift contents of register E right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.E);
            CycleCPU(8);
            break;
        case 0x2c:  // Shift contents of register H right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.H);
            CycleCPU(8);
            break;
        case 0x2d:  // Shift contents of register L right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.L);
            CycleCPU(8);
            break;
        case 0x2e:  // Shift contents byte at (HL) right and store Bit 0 in CF, Bit7 unchanged, flags updated
            BitOperationSRA(RegisterSet.HL);
            CycleCPU(16);
            break;
        case 0x3f:  // Shift contents of register A right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.A);
            CycleCPU(8);
            break;
        case 0x38:  // Shift contents of register B right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.B);
            CycleCPU(8);
            break;
        case 0x39:  // Shift contents of register C right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.C);
            CycleCPU(8);
            break;
        case 0x3a:  // Shift contents of register D right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.D);
            CycleCPU(8);
            break;
        case 0x3b:  // Shift contents of register E right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.E);
            CycleCPU(8);
            break;
        case 0x3c:  // Shift contents of register H right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.H);
            CycleCPU(8);
            break;
        case 0x3d:  // Shift contents of register L right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.L);
            CycleCPU(8);
            break;
        case 0x3e:  // Shift contents byte at (HL) right and store Bit 0 in CF, Bit7 = 0, flags updated
            BitOperationSRL(RegisterSet.HL);
            CycleCPU(16);
            break;
        case 0x47:  // Test Bit 0 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 0);
            CycleCPU(8);
            break;
        case 0x40:  // Test Bit 0 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 0);
            CycleCPU(8);
            break;
        case 0x41:  // Test Bit 0 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 0);
            CycleCPU(8);
            break;
        case 0x42:  // Test Bit 0 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 0);
            CycleCPU(8);
            break;
        case 0x43:  // Test Bit 0 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 0);
            CycleCPU(8);
            break;
        case 0x44:  // Test Bit 0 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 0);
            CycleCPU(8);
            break;
        case 0x45:  // Test Bit 0 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 0);
            CycleCPU(8);
            break;
        case 0x46:  // Test Bit 0 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 0);
            CycleCPU(16);
            break;
        case 0x4f:  // Test Bit 1 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 1);
            CycleCPU(8);
            break;
        case 0x48:  // Test Bit 1 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 1);
            CycleCPU(8);
            break;
        case 0x49:  // Test Bit 1 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 1);
            CycleCPU(8);
            break;
        case 0x4a:  // Test Bit 1 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 1);
            CycleCPU(8);
            break;
        case 0x4b:  // Test Bit 1 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 1);
            CycleCPU(8);
            break;
        case 0x4c:  // Test Bit 1 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 1);
            CycleCPU(8);
            break;
        case 0x4d:  // Test Bit 1 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 1);
            CycleCPU(8);
            break;
        case 0x4e:  // Test Bit 1 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 1);
            CycleCPU(16);
            break;
        case 0x57:  // Test Bit 2 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 2);
            CycleCPU(8);
            break;
        case 0x50:  // Test Bit 2 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 2);
            CycleCPU(8);
            break;
        case 0x51:  // Test Bit 2 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 2);
            CycleCPU(8);
            break;
        case 0x52:  // Test Bit 2 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 2);
            CycleCPU(8);
            break;
        case 0x53:  // Test Bit 2 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 2);
            CycleCPU(8);
            break;
        case 0x54:  // Test Bit 2 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 2);
            CycleCPU(8);
            break;
        case 0x55:  // Test Bit 2 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 2);
            CycleCPU(8);
            break;
        case 0x56:  // Test Bit 2 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 2);
            CycleCPU(16);
            break;
        case 0x5f:  // Test Bit 3 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 3);
            CycleCPU(8);
            break;
        case 0x58:  // Test Bit 3 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 3);
            CycleCPU(8);
            break;
        case 0x59:  // Test Bit 3 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 3);
            CycleCPU(8);
            break;
        case 0x5a:  // Test Bit 3 in register D and set flags accordingly
            BitOperationTest(RegisterSet.E, 3);
            CycleCPU(8);
            break;
        case 0x5b:  // Test Bit 3 in register E and set flags accordingly
            BitOperationTest(RegisterSet.D, 3);
            CycleCPU(8);
            break;
        case 0x5c:  // Test Bit 3 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 3);
            CycleCPU(8);
            break;
        case 0x5d:  // Test Bit 3 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 3);
            CycleCPU(8);
            break;
        case 0x5e:  // Test Bit 3 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 3);
            CycleCPU(16);
            break;
        case 0x67:  // Test Bit 4 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 4);
            CycleCPU(8);
            break;
        case 0x60:  // Test Bit 4 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 4);
            CycleCPU(8);
            break;
        case 0x61:  // Test Bit 4 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 4);
            CycleCPU(8);
            break;
        case 0x62:  // Test Bit 4 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 4);
            CycleCPU(8);
            break;
        case 0x63:  // Test Bit 4 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 4);
            CycleCPU(8);
            break;
        case 0x64:  // Test Bit 4 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 4);
            CycleCPU(8);
            break;
        case 0x65:  // Test Bit 4 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 4);
            CycleCPU(8);
            break;
        case 0x66:  // Test Bit 4 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 4);
            CycleCPU(16);
            break;
        case 0x6f:  // Test Bit 5 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 5);
            CycleCPU(8);
            break;
        case 0x68:  // Test Bit 5 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 5);
            CycleCPU(8);
            break;
        case 0x69:  // Test Bit 5 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 5);
            CycleCPU(8);
            break;
        case 0x6a:  // Test Bit 5 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 5);
            CycleCPU(8);
            break;
        case 0x6b:  // Test Bit 5 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 5);
            CycleCPU(8);
            break;
        case 0x6c:  // Test Bit 5 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 5);
            CycleCPU(8);
            break;
        case 0x6d:  // Test Bit 5 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 5);
            CycleCPU(8);
            break;
        case 0x6e:  // Test Bit 5 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 5);
            CycleCPU(16);
            break;
        case 0x77:  // Test Bit 6 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 6);
            CycleCPU(8);
            break;
        case 0x70:  // Test Bit 6 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 6);
            CycleCPU(8);
            break;
        case 0x71:  // Test Bit 6 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 6);
            CycleCPU(8);
            break;
        case 0x72:  // Test Bit 6 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 6);
            CycleCPU(8);
            break;
        case 0x73:  // Test Bit 6 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 6);
            CycleCPU(8);
            break;
        case 0x74:  // Test Bit 6 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 6);
            CycleCPU(8);
            break;
        case 0x75:  // Test Bit 6 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 6);
            CycleCPU(8);
            break;
        case 0x76:  // Test Bit 6 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 6);
            CycleCPU(16);
            break;
        case 0x7f:  // Test Bit 7 in register A and set flags accordingly
            BitOperationTest(RegisterSet.A, 7);
            CycleCPU(8);
            break;
        case 0x78:  // Test Bit 7 in register B and set flags accordingly
            BitOperationTest(RegisterSet.B, 7);
            CycleCPU(8);
            break;
        case 0x79:  // Test Bit 7 in register C and set flags accordingly
            BitOperationTest(RegisterSet.C, 7);
            CycleCPU(8);
            break;
        case 0x7a:  // Test Bit 7 in register D and set flags accordingly
            BitOperationTest(RegisterSet.D, 7);
            CycleCPU(8);
            break;
        case 0x7b:  // Test Bit 7 in register E and set flags accordingly
            BitOperationTest(RegisterSet.E, 7);
            CycleCPU(8);
            break;
        case 0x7c:  // Test Bit 7 in register H and set flags accordingly
            BitOperationTest(RegisterSet.H, 7);
            CycleCPU(8);
            break;
        case 0x7d:  // Test Bit 7 in register L and set flags accordingly
            BitOperationTest(RegisterSet.L, 7);
            CycleCPU(8);
            break;
        case 0x7e:  // Test Bit 7 in byte at (HL) and set flags accordingly
            BitOperationTest(RegisterSet.HL, 7);
            CycleCPU(16);
            break;
        case 0xc7:  // Set Bit 0 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 0);
            CycleCPU(8);
            break;
        case 0xc0:  // Set Bit 0 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 0);
            CycleCPU(8);
            break;
        case 0xc1:  // Set Bit 0 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 0);
            CycleCPU(8);
            break;
        case 0xc2:  // Set Bit 0 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 0);
            CycleCPU(8);
            break;
        case 0xc3:  // Set Bit 0 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 0);
            CycleCPU(8);
            break;
        case 0xc4:  // Set Bit 0 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 0);
            CycleCPU(8);
            break;
        case 0xc5:  // Set Bit 0 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 0);
            CycleCPU(8);
            break;
        case 0xc6:  // Set Bit 0 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 0);
            CycleCPU(16);
            break;
        case 0xcf:  // Set Bit 1 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 1);
            CycleCPU(8);
            break;
        case 0xc8:  // Set Bit 1 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 1);
            CycleCPU(8);
            break;
        case 0xc9:  // Set Bit 1 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 1);
            CycleCPU(8);
            break;
        case 0xca:  // Set Bit 1 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 1);
            CycleCPU(8);
            break;
        case 0xcb:  // Set Bit 1 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 1);
            CycleCPU(8);
            break;
        case 0xcc:  // Set Bit 1 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 1);
            CycleCPU(8);
            break;
        case 0xcd:  // Set Bit 1 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 1);
            CycleCPU(8);
            break;
        case 0xce:  // Set Bit 1 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 1);
            CycleCPU(16);
            break;
        case 0xd7:  // Set Bit 2 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 2);
            CycleCPU(8);
            break;
        case 0xd0:  // Set Bit 2 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 2);
            CycleCPU(8);
            break;
        case 0xd1:  // Set Bit 2 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 2);
            CycleCPU(8);
            break;
        case 0xd2:  // Set Bit 2 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 2);
            CycleCPU(8);
            break;
        case 0xd3:  // Set Bit 2 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 2);
            CycleCPU(8);
            break;
        case 0xd4:  // Set Bit 2 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 2);
            CycleCPU(8);
            break;
        case 0xd5:  // Set Bit 2 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 2);
            CycleCPU(8);
            break;
        case 0xd6:  // Set Bit 2 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 2);
            CycleCPU(16);
            break;
        case 0xdf:  // Set Bit 3 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 3);
            CycleCPU(8);
            break;
        case 0xd8:  // Set Bit 3 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 3);
            CycleCPU(8);
            break;
        case 0xd9:  // Set Bit 3 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 3);
            CycleCPU(8);
            break;
        case 0xda:  // Set Bit 3 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 3);
            CycleCPU(8);
            break;
        case 0xdb:  // Set Bit 3 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 3);
            CycleCPU(8);
            break;
        case 0xdc:  // Set Bit 3 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 3);
            CycleCPU(8);
            break;
        case 0xdd:  // Set Bit 3 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 3);
            CycleCPU(8);
            break;
        case 0xde:  // Set Bit 3 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 3);
            CycleCPU(16);
            break;
        case 0xe7:  // Set Bit 4 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 4);
            CycleCPU(8);
            break;
        case 0xe0:  // Set Bit 4 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 4);
            CycleCPU(8);
            break;
        case 0xe1:  // Set Bit 4 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 4);
            CycleCPU(8);
            break;
        case 0xe2:  // Set Bit 4 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 4);
            CycleCPU(8);
            break;
        case 0xe3:  // Set Bit 4 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 4);
            CycleCPU(8);
            break;
        case 0xe4:  // Set Bit 4 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 4);
            CycleCPU(8);
            break;
        case 0xe5:  // Set Bit 4 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 4);
            CycleCPU(8);
            break;
        case 0xe6:  // Set Bit 4 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 4);
            CycleCPU(16);
            break;
        case 0xef:  // Set Bit 5 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 5);
            CycleCPU(8);
            break;
        case 0xe8:  // Set Bit 5 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 5);
            CycleCPU(8);
            break;
        case 0xe9:  // Set Bit 5 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 5);
            CycleCPU(8);
            break;
        case 0xea:  // Set Bit 5 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 5);
            CycleCPU(8);
            break;
        case 0xeb:  // Set Bit 5 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 5);
            CycleCPU(8);
            break;
        case 0xec:  // Set Bit 5 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 5);
            CycleCPU(8);
            break;
        case 0xed:  // Set Bit 5 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 5);
            CycleCPU(8);
            break;
        case 0xee:  // Set Bit 5 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 5);
            CycleCPU(16);
            break;
       case 0xf7:  // Set Bit 6 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 6);
            CycleCPU(8);
            break;
        case 0xf0:  // Set Bit 6 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 6);
            CycleCPU(8);
            break;
        case 0xf1:  // Set Bit 6 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 6);
            CycleCPU(8);
            break;
        case 0xf2:  // Set Bit 6 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 6);
            CycleCPU(8);
            break;
        case 0xf3:  // Set Bit 6 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 6);
            CycleCPU(8);
            break;
        case 0xf4:  // Set Bit 6 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 6);
            CycleCPU(8);
            break;
        case 0xf5:  // Set Bit 6 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 6);
            CycleCPU(8);
            break;
        case 0xf6:  // Set Bit 6 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 6);
            CycleCPU(16);
            break;
        case 0xff:  // Set Bit 7 in register A, flags not affected
            BitOperationSET(RegisterSet.A, 7);
            CycleCPU(8);
            break;
        case 0xf8:  // Set Bit 7 in register B, flags not affected
            BitOperationSET(RegisterSet.B, 7);
            CycleCPU(8);
            break;
        case 0xf9:  // Set Bit 7 in register C, flags not affected
            BitOperationSET(RegisterSet.C, 7);
            CycleCPU(8);
            break;
        case 0xfa:  // Set Bit 7 in register D, flags not affectedy
            BitOperationSET(RegisterSet.D, 7);
            CycleCPU(8);
            break;
        case 0xfb:  // Set Bit 7 in register E, flags not affected
            BitOperationSET(RegisterSet.E, 7);
            CycleCPU(8);
            break;
        case 0xfc:  // Set Bit 7 in register H, flags not affected
            BitOperationSET(RegisterSet.H, 7);
            CycleCPU(8);
            break;
        case 0xfd:  // Set Bit 7 in register L, flags not affected
            BitOperationSET(RegisterSet.L, 7);
            CycleCPU(8);
            break;
        case 0xfe:  // Set Bit 7 in byte at (HL), flags not affected
            BitOperationSet(RegisterSet.HL, 7);
            CycleCPU(16);
            break;
        case 0x87:  // Reset Bit 0 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 0);
            CycleCPU(8);
            break;
        case 0x80:  // Reset Bit 0 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 0);
            CycleCPU(8);
            break;
        case 0x81:  // Reset Bit 0 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 0);
            CycleCPU(8);
            break;
        case 0x82:  // Reset Bit 0 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 0);
            CycleCPU(8);
            break;
        case 0x83:  // Reset Bit 0 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 0);
            CycleCPU(8);
            break;
        case 0x84:  // Reset Bit 0 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 0);
            CycleCPU(8);
            break;
        case 0x85:  // Reset Bit 0 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 0);
            CycleCPU(8);
            break;
        case 0x86:  // Reset Bit 0 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 0);
            CycleCPU(16);
            break;
        case 0x8f:  // Reset Bit 1 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 1);
            CycleCPU(8);
            break;
        case 0x88:  // Reset Bit 1 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 1);
            CycleCPU(8);
            break;
        case 0x89:  // Reset Bit 1 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 1);
            CycleCPU(8);
            break;
        case 0x8a:  // Reset Bit 1 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 1);
            CycleCPU(8);
            break;
        case 0x8b:  // Reset Bit 1 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 1);
            CycleCPU(8);
            break;
        case 0x8c:  // Reset Bit 1 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 1);
            CycleCPU(8);
            break;
        case 0x8d:  // Reset Bit 1 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 1);
            CycleCPU(8);
            break;
        case 0x8e:  // Reset Bit 1 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 1);
            CycleCPU(16);
            break;
        case 0x97:  // Reset Bit 2 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 2);
            CycleCPU(8);
            break;
        case 0x90:  // Reset Bit 2 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 2);
            CycleCPU(8);
            break;
        case 0x91:  // Reset Bit 2 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 2);
            CycleCPU(8);
            break;
        case 0x92:  // Reset Bit 2 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 2);
            CycleCPU(8);
            break;
        case 0x93:  // Reset Bit 2 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 2);
            CycleCPU(8);
            break;
        case 0x94:  // Reset Bit 2 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 2);
            CycleCPU(8);
            break;
        case 0x95:  // Reset Bit 2 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 2);
            CycleCPU(8);
            break;
        case 0x96:  // Reset Bit 2 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 2);
            CycleCPU(16);
            break;
        case 0x9f:  // Reset Bit 3 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 3);
            CycleCPU(8);
            break;
        case 0x98:  // Reset Bit 3 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 3);
            CycleCPU(8);
            break;
        case 0x99:  // Reset Bit 3 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 3);
            CycleCPU(8);
            break;
        case 0x9a:  // Reset Bit 3 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 3);
            CycleCPU(8);
            break;
        case 0x9b:  // Reset Bit 3 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 3);
            CycleCPU(8);
            break;
        case 0x9c:  // Reset Bit 3 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 3);
            CycleCPU(8);
            break;
        case 0x9d:  // Reset Bit 3 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 3);
            CycleCPU(8);
            break;
        case 0x9e:  // Reset Bit 3 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 3);
            CycleCPU(16);
            break;
        case 0xa7:  // Reset Bit 4 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 4);
            CycleCPU(8);
            break;
        case 0xa0:  // Reset Bit 4 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 4);
            CycleCPU(8);
            break;
        case 0xa1:  // Reset Bit 4 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 4);
            CycleCPU(8);
            break;
        case 0xa2:  // Reset Bit 4 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 4);
            CycleCPU(8);
            break;
        case 0xa3:  // Reset Bit 4 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 4);
            CycleCPU(8);
            break;
        case 0xa4:  // Reset Bit 4 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 4);
            CycleCPU(8);
            break;
        case 0xa5:  // Reset Bit 4 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 4);
            CycleCPU(8);
            break;
        case 0xa6:  // Reset Bit 4 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 4);
            CycleCPU(16);
            break;
        case 0xaf:  // Reset Bit 5 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 5);
            CycleCPU(8);
            break;
        case 0xa8:  // Reset Bit 5 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 5);
            CycleCPU(8);
            break;
        case 0xa9:  // Reset Bit 5 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 5);
            CycleCPU(8);
            break;
        case 0xaa:  // Reset Bit 5 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 5);
            CycleCPU(8);
            break;
        case 0xab:  // Reset Bit 5 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 5);
            CycleCPU(8);
            break;
        case 0xac:  // Reset Bit 5 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 5);
            CycleCPU(8);
            break;
        case 0xad:  // Reset Bit 5 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 5);
            CycleCPU(8);
            break;
        case 0xae:  // Reset Bit 5 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 5);
            CycleCPU(16);
            break;
        case 0xb7:  // Reset Bit 6 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 6);
            CycleCPU(8);
            break;
        case 0xb0:  // Reset Bit 6 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 6);
            CycleCPU(8);
            break;
        case 0xb1:  // Reset Bit 6 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 6);
            CycleCPU(8);
            break;
        case 0xb2:  // Reset Bit 6 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 6);
            CycleCPU(8);
            break;
        case 0xb3:  // Reset Bit 6 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 6);
            CycleCPU(8);
            break;
        case 0xb4:  // Reset Bit 6 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 6);
            CycleCPU(8);
            break;
        case 0xb5:  // Reset Bit 6 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 6);
            CycleCPU(8);
            break;
        case 0xb6:  // Reset Bit 6 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 6);
            CycleCPU(16);
            break;
        case 0xbf:  // Reset Bit 7 in register A, flags not affected
            BitOperationReset(RegisterSet.A, 7);
            CycleCPU(8);
            break;
        case 0xb8:  // Reset Bit 7 in register B, flags not affected
            BitOperationReset(RegisterSet.B, 7);
            CycleCPU(8);
            break;
        case 0xb9:  // Reset Bit 7 in register C, flags not affected
            BitOperationReset(RegisterSet.C, 7);
            CycleCPU(8);
            break;
        case 0xba:  // Reset Bit 7 in register D, flags not affectedy
            BitOperationReset(RegisterSet.D, 7);
            CycleCPU(8);
            break;
        case 0xbb:  // Reset Bit 7 in register E, flags not affected
            BitOperationReset(RegisterSet.E, 7);
            CycleCPU(8);
            break;
        case 0xbc:  // Reset Bit 7 in register H, flags not affected
            BitOperationReset(RegisterSet.H, 7);
            CycleCPU(8);
            break;
        case 0xbd:  // Reset Bit 7 in register L, flags not affected
            BitOperationReset(RegisterSet.L, 7);
            CycleCPU(8);
            break;
        case 0xbe:  // Reset Bit 7 in byte at (HL), flags not affected
            BitOperationReset(RegisterSet.HL, 7);
            CycleCPU(16);
            break;
        
        default:
            std::cerr << "[ERROR]: Cannot find opcode for CB instruction! : " << CBInstructionToExecute << std::endl;
    }
}
