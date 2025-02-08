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

    void decode(uint32_t instruction) override;
    bool getRegWrite() const override;
    bool getMemRead() const override;
    bool getMemWrite() const override;
    bool getALUSrc() const override;
    ALU::ALUOp getALUOp() const override;

};



#endif // RVSS_CONTROL_UNIT_H