/**
 * @file rvss_control_unit.cpp
 * @brief RVSS Control Unit implementation
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "rvss_control_unit.h"
#include "../alu.h"

void RVSSControlUnit::setControlSignals(uint32_t instruction) {
    uint8_t opcode = (instruction >> 26) & 0x3F; // Extract opcode (assuming RISC-V-like encoding)

    // Reset all signals
    RegDst = ALUSrc = MemToReg = RegWrite = MemRead = MemWrite = Branch = false;
    ALUOp = ALU::ALUOp::NONE;

    switch (opcode) {
        case 0x33: // R-type (add, sub, etc.)
            RegDst = true;
            RegWrite = true;
            ALUOp = ALU::ALUOp::ADD;
            break;
        case 0x03: // Load (LW)
            ALUSrc = true;
            MemToReg = true;
            RegWrite = true;
            MemRead = true;
            break;
        case 0x23: // Store (SW)
            ALUSrc = true;
            MemWrite = true;
            break;
        case 0x63: // Branch (BEQ)
            Branch = true;
            ALUOp = ALU::ALUOp::SUB;
            break;
        default:
            break;
    }
}




