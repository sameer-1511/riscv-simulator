/**
 * @file rvss_vm.h
 * @brief RVSS VM definition
 * @author Vishank Singh, https://github.com/VishankSingh
 */
#ifndef RVSS_VM_H
#define RVSS_VM_H

#include "../../pch.h"

#include "../vm_base.h"

#include "rvss_control_unit.h"

struct RegisterChange {
    unsigned int reg_index;
    unsigned int reg_type; // 0 for GPR, 1 for CSR, 2 for FPR
    uint64_t old_value;
    uint64_t new_value;
};

struct MemoryChange {
    uint64_t address;
    uint64_t old_bytes;
    uint64_t new_bytes;
};

struct StepDelta {
    uint64_t old_pc;
    uint64_t new_pc;
    std::vector<RegisterChange> register_changes;
    std::vector<MemoryChange> memory_changes;
};

class RVSSVM : public VMBase {
public: 
    RVSSControlUnit controlUnit;

    std::stack<StepDelta> undo_stack_;
    std::stack<StepDelta> redo_stack_;
    StepDelta current_delta_;


    // intermediate variables
    int64_t execution_result_ = 0;
    int64_t memory_result_ = 0;
    // int64_t memory_address_ = 0;
    // int64_t memory_data_ = 0;
    uint64_t return_address_ = 0;
    
    bool branch_flag_ = false;
    int64_t next_pc_ = 0; // for jal, jalr, 

    // CSR intermediate variables
    uint16_t csr_target_address_;
    uint64_t csr_old_value_;
    uint64_t csr_write_val_;
    uint8_t csr_uimm_;

    void fetch();

    void decode();

    void execute();
    void executeFloat();
    void executeDouble();
    void executeVector();
    void executeCSR();

    void memory();
    void memoryFloat();
    void memoryDouble();
    void memoryVector();

    void writeBack();
    void writeBackFloat();
    void writeBackDouble();
    void writeBackVector();
    void writeBackCSR();

    RVSSVM();
    ~RVSSVM();

    void run() override;
    void debugRun() override;
    void step() override;
    void undo() override;
    void redo() override;
    void reset() override;
    void dumpState(const std::string &filename) override;

    void printType() {
        std::cout << "rvssvm" << std::endl;
    }
};


#endif // RVSS_VM_H
