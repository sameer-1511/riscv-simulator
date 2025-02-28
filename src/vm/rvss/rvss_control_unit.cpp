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
    ALUSrc = MemToReg = RegWrite = MemRead = MemWrite = false;
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
            MemWrite = true;
            break;

        case 0x63: // Branch instructions (BEQ, BNE, BLT, BGE)
            ALUOp = true;
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
            break;

        case 0x67: // JALR (Jump and Link Register)
            ALUSrc = true;
            RegWrite = true;
            break;

        default:
            // Invalid or unimplemented opcode, keep all control signals disabled
            break;
    }

    
}

ALU::ALUOp RVSSControlUnit::getALUSignal(uint32_t instruction, bool ALUOp) {
    if (!ALUOp) {
        return ALU::ALUOp::NONE;
    }
    uint8_t opcode = instruction & 0b1111111; // Extract opcode (assuming RISC-V-like encoding)
    
    
    // Set ALU operation based on the function fields for R-type and I-type ALU instructions
    if (opcode == 0x33 || opcode == 0x13) {
        uint8_t funct3 = (instruction >> 12) & 0b111;
        uint8_t funct7 = (instruction >> 25) & 0b1111111;

        switch (funct3) {
            case 0x0: // ADD, SUB, ADDI
                if (opcode == 0x33 && funct7 == 0x20) {
                    return ALU::ALUOp::SUB; // SUB
                } else {
                    return ALU::ALUOp::ADD; // ADD or ADDI
                }
                break;
            case 0x7: // AND, ANDI
                return ALU::ALUOp::AND;
                break;
            case 0x6: // OR, ORI
                return ALU::ALUOp::OR;
                break;
            case 0x4: // XOR, XORI
                return ALU::ALUOp::XOR;
                break;
            case 0x1: // SLL, SLLI
                return ALU::ALUOp::SLL;
                break;
            case 0x5: // SRL, SRLI, SRA, SRAI
                if (funct7 == 0x20) {
                    return ALU::ALUOp::SRA; // SRA or SRAI
                } else {
                    return ALU::ALUOp::SRL; // SRL or SRLI
                }
                break;
            case 0x2: // SLT, SLTI
                return ALU::ALUOp::SLT;
                break;
            default:
                return ALU::ALUOp::NONE; // Unhandled funct3
                break;
        }
    }

    return ALU::ALUOp::NONE; 
}


bool RVSSControlUnit::getALUSrc() const { return ALUSrc; }
bool RVSSControlUnit::getMemToReg() const { return MemToReg; }
bool RVSSControlUnit::getRegWrite() const { return RegWrite; }
bool RVSSControlUnit::getMemRead() const { return MemRead; }
bool RVSSControlUnit::getMemWrite() const { return MemWrite; }
bool RVSSControlUnit::getALUOp() const { return ALUOp; }


