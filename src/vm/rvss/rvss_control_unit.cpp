/**
 * @file rvss_control_unit.cpp
 * @brief RVSS Control Unit implementation
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "rvss_control_unit.h"
#include "../alu.h"

// TODO: check if this is correct

void RVSSControlUnit::setControlSignals(uint32_t instruction) {
    uint8_t opcode = instruction & 0b1111111; // Extract opcode (assuming RISC-V-like encoding)

    // Reset all signals
    ALUSrc = MemToReg = RegWrite = MemRead = MemWrite = Branch = false;
    ALUOp = false;

    switch (opcode) {
        case 0b0110011: // R-type (ADD, SUB, AND, OR, XOR, SLL, SRL, etc.)
            RegWrite = true;
            ALUOp = true;
            break;

        case 0b0000011: // Load instructions (LB, LH, LW, LD)
            ALUSrc = true;
            MemToReg = true;
            RegWrite = true;
            MemRead = true;
            break;

        case 0b0100011: // Store instructions (SB, SH, SW, SD)
            ALUSrc = true;
            ALUOp = true;
            MemWrite = true;
            break;

        case 0b1100011: // Branch instructions (BEQ, BNE, BLT, BGE)
            ALUOp = true;
            Branch = true;
            break;

        case 0b0010011: // I-type ALU instructions (ADDI, ANDI, ORI, XORI, SLTI, SLLI, SRLI)
            ALUSrc = true;
            RegWrite = true;
            ALUOp = true;
            break;

        case 0b0110111: // LUI (Load Upper Immediate)
            ALUSrc = true;
            RegWrite = true;
            ALUOp = true; // ALU will add immediate to zero
            break;

        case 0b0010111: // AUIPC (Add Upper Immediate to PC)
            ALUSrc = true;
            RegWrite = true;
            ALUOp = true; // ALU will add immediate to PC
            break;

        case 0b1101111: // JAL (Jump and Link)
            RegWrite = true;
            Branch = true;
            break;

        case 0b1100111: // JALR (Jump and Link Register)
            ALUSrc = true;
            RegWrite = true;
            Branch = true;
            break;

        // case 0b0000001: // MUL
        //     RegWrite = true;
        //     ALUOp = true;
        //     break;

        default:
            // Invalid or unimplemented opcode, keep all control signals disabled
            break;
    }

    
}

ALU::ALUOp RVSSControlUnit::getALUSignal(uint32_t instruction, bool ALUOp) {
    (void)ALUOp; // Suppress unused variable warning
    // DONT UNCOMMENT THIS WITHOUT SUPPORTING ALUOP IN CONTROL SIGNAL SETTING
    // if (!ALUOp) {
    //     return ALU::ALUOp::NONE;
    // }
    uint8_t opcode = instruction & 0b1111111; // Extract opcode (assuming RISC-V-like encoding)
    uint8_t funct3 = (instruction >> 12) & 0b111;
    uint8_t funct7 = (instruction >> 25) & 0b1111111;
    // uint8_t funct2 = (instruction >> 25) & 0b11;
    // uint8_t funct6 = (instruction >> 26) & 0b111111;

    switch (opcode)
    {
    case 0b0110011: {// R-Type
        switch (funct3)
        {
        case 0b000:{ // ADD, SUB, MUL
            switch (funct7)
            {
            case 0x0000000: {// ADD
                return ALU::ALUOp::ADD;
                break;
            }
            case 0b0100000: {// SUB
                return ALU::ALUOp::SUB;
                break;
            }
            case 0b0000001: {// MUL
                return ALU::ALUOp::MUL;
                break;
            }
            }
            break;
        }
        case 0b001: {// SLL, MULH
            switch (funct7)
            {
            case 0b0000000: {// SLL
                return ALU::ALUOp::SLL;
                break;
            }
            case 0b0000001: {// MULH
                return ALU::ALUOp::MULH;
                break;
            }
            }
            break;
        }
        case 0b010: {// SLT, MULHSU
            switch (funct7)
            {
            case 0b0000000: {// SLT
                return ALU::ALUOp::SLT;
                break;
            }
            case 0b0000001: {// MULHSU
                return ALU::ALUOp::MULHSU;
                break;
            }
            }
            break;
        }
        case 0b011: {// SLTU, MULHU
            switch (funct7)
            {
            case 0b0000000: {// SLTU
                return ALU::ALUOp::SLTU;
                break;
            }
            case 0b0000001: {// MULHU
                return ALU::ALUOp::MULHU;
                break;
            }
            }
            break;
        }
        case 0b100: {// XOR, DIV
            switch (funct7)
            {
            case 0b0000000: {// XOR
                return ALU::ALUOp::XOR;
                break;
            }
            case 0b0000001: {// DIV
                return ALU::ALUOp::DIV;
                break;
            }
            }
            break;
        }
        case 0b101: {// SRL, SRA, DIVU
            switch (funct7)
            {
            case 0b0000000: {// SRL
                return ALU::ALUOp::SRL;
                break;
            }
            case 0b0100000: {// SRA
                return ALU::ALUOp::SRA;
                break;
            }
            case 0b0000001: {// DIVU
                return ALU::ALUOp::DIVU;
                break;
            }
            }
            break;
        }
        case 0b110: {// OR, REM
            switch (funct7)
            {
            case 0b0000000: {// OR
                return ALU::ALUOp::OR;
                break;
            }
            case 0b0000001: {// REM
                return ALU::ALUOp::REM;
                break;
            }
            }
            break;
        }
        case 0b111: {// AND, REMU
            switch (funct7)
            {
            case 0b0000000: {// AND
                return ALU::ALUOp::AND;
                break;
            }
            case 0b0000001: {// REMU
                return ALU::ALUOp::REMU;
                break;
            }
            }
            break;
        }
        }
        break;
    }
    case 0b0010011: {// I-Type
        switch (funct3)
        {
        case 0b000: {// ADDI
            return ALU::ALUOp::ADD;
            break;
        }
        case 0b001: {// SLLI
            return ALU::ALUOp::SLL;
            break;
        }
        case 0b010: {// SLTI
            return ALU::ALUOp::SLT;
            break;
        }
        case 0b011: {// SLTIU
            return ALU::ALUOp::SLTU;
            break;
        }
        case 0b100: {// XORI
            return ALU::ALUOp::XOR;
            break;
        }
        case 0b101: {// SRLI & SRAI
            switch (funct7)
            {
            case 0b0000000: {// SRLI
                return ALU::ALUOp::SRL;
                break;
            }
            case 0b0100000: {// SRAI
                return ALU::ALUOp::SRA;
                break;
            }
            }
            break;
        }
        case 0b110: {// ORI
            return ALU::ALUOp::OR;
            break;
        }
        case 0b111: {// ANDI
            return ALU::ALUOp::AND;
            break;
        }
        }
        break;
    }
    case 0b1100011: {// B-Type
        switch (funct3)
        {
        case 0b000: {// BEQ
            return ALU::ALUOp::SUB;
            break;
        }
        case 0b001: {// BNE
            return ALU::ALUOp::SUB;
            break;
        }
        case 0b100: {// BLT
            return ALU::ALUOp::SLT;
            break;
        }
        case 0b101: {// BGE
            return ALU::ALUOp::SLT;
            break;
        }
        case 0b110: {// BLTU
            return ALU::ALUOp::SLTU;
            break;
        }
        case 0b111: {// BGEU
            return ALU::ALUOp::SLTU;
            break;
        }
        }
        break;
    }
    case 0b0000011: {// Load
        return ALU::ALUOp::ADD;
        break;
    }
    case 0b0100011: {// Store
        return ALU::ALUOp::ADD;
        break;
    }
    case 0b1100111: {// JALR
        return ALU::ALUOp::ADD;
        break;
    }
    case 0b1101111: {// JAL
        return ALU::ALUOp::ADD;
        break;
    }
    case 0b0110111: {// LUI
        return ALU::ALUOp::ADD;
        break;
    }
    case 0b0010111: {// AUIPC
        return ALU::ALUOp::ADD;
        break;
    }
    case 0b0000000: {// FENCE
        return ALU::ALUOp::NONE;
        break;
    }
    case 0b1110011: {// SYSTEM
        switch (funct3) 
        {
        case 0b000: // ECALL
            return ALU::ALUOp::NONE;
            break;
        case 0b001: // CSRRW
            return ALU::ALUOp::NONE;
            break;
        default:
            break;
        }
        break;
    }
    case 0b0011011: {// R4-Type
        switch (funct3) 
        {
            case 0b000: {// ADDIW
                return ALU::ALUOp::ADDW;
                break;
            }
            case 0b001: {// SLLIW
                return ALU::ALUOp::SLLW;
                break;
            }
            case 0b101: {// SRLIW & SRAIW
                switch (funct7) 
                {
                case 0b0000000: {// SRLIW
                    return ALU::ALUOp::SRLW;
                    break;
                }
                case 0b0100000: {// SRAIW
                        return ALU::ALUOp::SRAW;
                        break;
                    }
                }
                break;
            }
        }
        break;
    }
    case 0b0111011: {// R4-Type
        switch (funct3) {
        case 0b000: {// ADDW, SUBW, MULW
            switch (funct7) 
            {
            case 0b0000000: {// ADDW
                return ALU::ALUOp::ADDW;
                break;
            }
            case 0b0100000: {// SUBW
                return ALU::ALUOp::SUBW;
                break;
            }
            case 0b0000001: {// MULW
                return ALU::ALUOp::MULW;
                break;
            }
            }
            break;
        }
        case 0b001: {// SLLW
            return ALU::ALUOp::SLLW;
            break;
        }
        case 0b100: {// DIVW
            switch (funct7) {// DIVW
                case 0b0000001: {// DIVW
                    return ALU::ALUOp::DIVW;
                    break;
                }
            }
            break;
        }
        case 0b101: {// SRLW, SRAW, DIVUW
            switch (funct7) {
                case 0b0000000: {// SRLW
                    return ALU::ALUOp::SRLW;
                    break;
                }
                case 0b0100000: {// SRAW
                    return ALU::ALUOp::SRAW;
                    break;
                }
                case 0b0000001: {// DIVUW
                    return ALU::ALUOp::DIVUW;
                    break;
                }
            }
            break;
        }
        case 0b110: {// REMW
            switch (funct7) 
            {
            case 0b0000001: {// REMW
                return ALU::ALUOp::REMW;
                break;
            }
            }
            break;
        }
        case 0b111: {// REMUW
                switch (funct7) {
                    case 0b0000001: {// REMUW
                        return ALU::ALUOp::REMUW;
                        break;
                    }
                }
                break;
            }
        }
        break;
    }
    
    
    }
    
    return ALU::ALUOp::NONE; 
}



