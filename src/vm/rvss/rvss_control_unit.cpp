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
        case 0x33: // R-type (ADD, SUB, AND, OR, XOR, SLL, SRL, etc.)
            RegWrite = true;
            ALUOp = true;
            break;

        case 0x03: // Load instructions (LB, LH, LW, LD)
            ALUSrc = true;
            MemToReg = true;
            RegWrite = true;
            MemRead = true;
            break;

        case 0x23: // Store instructions (SB, SH, SW, SD)
            ALUSrc = true;
            ALUOp = true;
            MemWrite = true;
            break;

        case 0x63: // Branch instructions (BEQ, BNE, BLT, BGE)
            ALUOp = true;
            Branch = true;
            break;

        case 0x13: // I-type ALU instructions (ADDI, ANDI, ORI, XORI, SLTI, SLLI, SRLI)
            ALUSrc = true;
            RegWrite = true;
            ALUOp = true;
            break;

        case 0x37: // LUI (Load Upper Immediate)
            ALUSrc = true;
            RegWrite = true;
            ALUOp = true; // ALU will add immediate to zero
            break;

        case 0x17: // AUIPC (Add Upper Immediate to PC)
            ALUSrc = true;
            RegWrite = true;
            ALUOp = true; // ALU will add immediate to PC
            break;

        case 0x6F: // JAL (Jump and Link)
            RegWrite = true;
            Branch = true;
            break;

        case 0x67: // JALR (Jump and Link Register)
            ALUSrc = true;
            RegWrite = true;
            Branch = true;
            break;

        default:
            // Invalid or unimplemented opcode, keep all control signals disabled
            break;
    }

    
}

ALU::ALUOp RVSSControlUnit::getALUSignal(uint32_t instruction, bool ALUOp) {
    // DONT UNCOMMENT THIS WITHOUT SUPPORTING ALUOP IN CONTROL SIGNAL SETTING
    // if (!ALUOp) {
    //     return ALU::ALUOp::NONE;
    // }
    uint8_t opcode = instruction & 0b1111111; // Extract opcode (assuming RISC-V-like encoding)
    uint8_t funct3 = (instruction >> 12) & 0b111;
    uint8_t funct7 = (instruction >> 25) & 0b1111111;
    uint8_t funct2 = (instruction >> 25) & 0b11;
    uint8_t funct6 = (instruction >> 26) & 0b111111;

    switch (opcode)
    {
    case 0b0110011: {// R-Type
       
        switch (funct3)
        {
        case 0x0: // ADD
            return ALU::ALUOp::ADD;
            break;
        case 0x1: // SLL
            return ALU::ALUOp::SLL;
            break;
            break;
        case 0x2: // SLT
            return ALU::ALUOp::SLT;
            break;
        case 0x3: // SLTU
            return ALU::ALUOp::SLTU;
            break;
        case 0x4: // XOR
            return ALU::ALUOp::XOR;
            break;
        case 0x5: // SRL & SRA
            switch (funct6)
            {
            case 0x00: // SRL
                return ALU::ALUOp::SRL;
                break;
            case 0x10: // SRA
                return ALU::ALUOp::SRA;
                break;
            }
            
            break;
        case 0x6: // OR
            return ALU::ALUOp::OR;
            break;
        case 0x7: // AND
            return ALU::ALUOp::AND;
            break;
        default:
            break;
        }
        break;
    }
    case 0b0010011: {// I-Type
        switch (funct3)
        {
        case 0x0: // ADDI
            return ALU::ALUOp::ADD;
            break;
        case 0x1: // SLLI
            return ALU::ALUOp::SLL;
            break;
        case 0x2: // SLTI
            return ALU::ALUOp::SLT;
            break;
        case 0x3: // SLTIU
            return ALU::ALUOp::SLTU;
            break;
        case 0x4: // XORI
            return ALU::ALUOp::XOR;
            break;
        case 0x5: // SRLI & SRAI
            switch (funct6)
            {
            case 0x00: // SRLI
                return ALU::ALUOp::SRL;
                break;
            case 0x10: // SRAI
                return ALU::ALUOp::SRA;
                break;
            }
            break;
        case 0x6: // ORI
            return ALU::ALUOp::OR;
            break;
        case 0x7: // ANDI
            return ALU::ALUOp::AND;
            break;
        default:
            break;
        }
        break;
    }
    case 0b1100011: {// B-Type
        switch (funct3)
        {
        case 0x0: // BEQ
            return ALU::ALUOp::SUB;
            break;
        case 0x1: // BNE
            return ALU::ALUOp::SUB;
            break;
        case 0x4: // BLT
            return ALU::ALUOp::SLT;
            break;
        case 0x5: // BGE
            return ALU::ALUOp::SLT;
            break;
        case 0x6: // BLTU
            return ALU::ALUOp::SLTU;
            break;
        case 0x7: // BGEU
            return ALU::ALUOp::SLTU;
            break;
        default:
            break;
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

    default:
        break;
    }
    
    // Set ALU operation based on the function fields for R-type and I-type ALU instructions
    // if (opcode == 0x33 || opcode == 0x13) {
        

    //     switch (funct3) {
    //         case 0x0: // ADD, SUB, ADDI
    //             if (opcode == 0x33 && funct7 == 0x20) {
    //                 return ALU::ALUOp::SUB; // SUB
    //             } else {
    //                 return ALU::ALUOp::ADD; // ADD or ADDI
    //             }
    //             break;
    //         case 0x7: // AND, ANDI
    //             return ALU::ALUOp::AND;
    //             break;
    //         case 0x6: // OR, ORI
    //             return ALU::ALUOp::OR;
    //             break;
    //         case 0x4: // XOR, XORI
    //             return ALU::ALUOp::XOR;
    //             break;
    //         case 0x1: // SLL, SLLI
    //             return ALU::ALUOp::SLL;
    //             break;
    //         case 0x5: // SRL, SRLI, SRA, SRAI
    //             if (funct7 == 0x20) {
    //                 return ALU::ALUOp::SRA; // SRA or SRAI
    //             } else {
    //                 return ALU::ALUOp::SRL; // SRL or SRLI
    //             }
    //             break;
    //         case 0x2: // SLT, SLTI
    //             return ALU::ALUOp::SLT;
    //             break;
    //         default:
    //             return ALU::ALUOp::NONE; // Unhandled funct3
    //             break;
    //     }
    // }

    return ALU::ALUOp::NONE; 
}



