/**
 * @file rvss_control_unit.h
 * @brief RVSS Control Unit
 * @author Vishank Singh, https://github.com/VishankSingh
 */
#ifndef RVSS_CONTROL_UNIT_H
#define RVSS_CONTROL_UNIT_H

#include "../control_unit_base.h"

#include "../../pch.h"

class RVSSControlUnit : public ControlUnit {
public:
    void setControlSignals(uint32_t instruction) override;

    ALU::ALUOp getALUSignal(uint32_t instruction, bool ALUOp) override;

    
};



#endif // RVSS_CONTROL_UNIT_H