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
    virtual ALU::ALUOp getALUSignal(uint32_t instruction, bool ALUOp) = 0;

    // Retrieves control signals (used by other components like ALU, memory)
    [[nodiscard]] virtual bool getALUSrc() const = 0;
    [[nodiscard]] virtual bool getMemToReg() const = 0;
    [[nodiscard]] virtual bool getRegWrite() const = 0;
    [[nodiscard]] virtual bool getMemRead() const = 0;
    [[nodiscard]] virtual bool getMemWrite() const = 0;
    [[nodiscard]] virtual bool getALUOp() const = 0;

protected:
    // Common control signals
    bool RegWrite = false;
    bool ALUSrc = false;
    bool PCSrc = false;
    bool MemRead = false;
    bool MemWrite = false;
    bool MemToReg = false;
    bool ALUOp = false;
};

#endif // CONTROL_UNIT_BASE_H
