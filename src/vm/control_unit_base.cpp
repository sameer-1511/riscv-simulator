/**
 * @file control_unit_base.cpp
 * @brief Control unit base class implementation
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "control_unit_base.h"

bool ControlUnit::getALUSrc() const {
    return ALUSrc;
}

bool ControlUnit::getMemToReg() const {
    return MemToReg;
}

bool ControlUnit::getRegWrite() const {
    return RegWrite;
}

bool ControlUnit::getMemRead() const {
    return MemRead;
}

bool ControlUnit::getMemWrite() const {
    return MemWrite;
}

uint8_t ControlUnit::getALUOp() const {
    return ALUOp;
}

bool ControlUnit::getBranch() const {
    return Branch;
}