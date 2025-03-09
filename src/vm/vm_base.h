/**
 * @file vm_base.h
 * @brief File containing the base class for the virtual machine
 * @author Vishank Singh, https://VishankSingh
 */
#ifndef VM_BASE_H
#define VM_BASE_H

#include "../pch.h"

#include "registers.h"
#include "memory_controller.h"
#include "alu.h"

#include "../vm_asm_mw.h"

class VMBase {
public:
    VMBase() = default;
    ~VMBase() = default;

    std::vector<uint64_t> breakpoints_;

    uint32_t current_instruction_;
    uint64_t program_counter_ = 0;
    
    unsigned int cycle_s = 0;
    unsigned int instructions_retired_ = 0;
    float cpi_ = 0.0;
    float ipc_ = 0.0;
    unsigned int stall_cycles_ = 0;
    unsigned int branch_mispredictions_ = 0;



    MemoryController memory_controller_;
    RegisterFile registers_;
    ALU::ALU alu_;


    void loadProgram(const AssembledProgram &program);
    uint64_t program_size_ = 0;

    uint64_t getProgramCounter() const;
    void updateProgramCounter(int64_t value);
    
    int32_t imm_generator(uint32_t instruction);

    void addBreakpoint(uint64_t address);
    void removeBreakpoint(uint64_t address);
    bool checkBreakpoint(uint64_t address);

    // void fetchInstruction();
    // void decodeInstruction();
    // void executeInstruction();
    // void memoryAccess();
    // void writeback();

    virtual void run() = 0;
    virtual void debugRun() = 0;
    virtual void step() = 0;
    virtual void reset() = 0;
    virtual void dumpState(const std::string &filename) = 0;

};

#endif // VM_BASE_H
