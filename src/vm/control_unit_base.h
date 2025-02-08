/**
 * @file control_unit_base.h
 * @brief Control unit base class definition
 * @author Vishank Singh, https://github.com/VishankSingh
 */
#ifndef CONTROL_UNIT_BASE_H
#define CONTROL_UNIT_BASE_H

#include "../pch.h"
#include "registers.h"
#include "alu.h"

/**
 * @brief The ControlUnit class is the base class for the control unit of the CPU.
 */
class ControlUnit {
public:
    virtual ~ControlUnit() = default;

    // Sets control signals based on the opcode or instruction
    virtual void setControlSignals(uint32_t instruction) = 0;

    // Retrieves control signals (used by other components like ALU, memory)
    virtual void decode(uint32_t instruction) = 0;
    virtual bool getRegWrite() const = 0;
    virtual bool getMemRead() const = 0;
    virtual bool getMemWrite() const = 0;
    virtual bool getALUSrc() const = 0;
    virtual ALU::ALUOp getALUOp() const = 0;


protected:
    // Common control signals
    bool RegDst = false;
    bool ALUSrc = false;
    bool MemToReg = false;
    bool RegWrite = false;
    bool MemRead = false;
    bool MemWrite = false;
    bool Branch = false;
    ALU::ALUOp ALUOp = ALU::ALUOp::NONE;
};


#endif // CONTROL_UNIT_BASE_H
