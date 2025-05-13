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

    void reset() {
        ALUSrc = false;
        MemToReg = false;
        RegWrite = false;
        MemRead = false;
        MemWrite = false;
        Branch = false;
        ALUOp = 0;
    }

    // Sets control signals based on the opcode or instruction
    virtual void setControlSignals(uint32_t instruction) = 0;
    virtual ALU::ALUOp getALUSignal(uint32_t instruction, bool ALUOp) = 0;

    // Retrieves control signals (used by other components like ALU, memory)
    [[nodiscard]] bool getALUSrc() const;
    [[nodiscard]] bool getMemToReg() const;
    [[nodiscard]] bool getRegWrite() const;
    [[nodiscard]] bool getMemRead() const;
    [[nodiscard]] bool getMemWrite() const;
    [[nodiscard]] uint8_t getALUOp() const;
    [[nodiscard]] bool getBranch() const;

protected:
    // Common control signals
    bool RegWrite = false;
    bool Branch = false;
    bool ALUSrc = false;
    bool MemRead = false;
    bool MemWrite = false;
    bool MemToReg = false;
    bool PCSrc = false;

    uint8_t ALUOp = 0;
};

#endif // CONTROL_UNIT_BASE_H
