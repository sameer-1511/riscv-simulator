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

#include "../vm_asm_mw.h"

class VMBase {
public:
    VMBase() = default;
    ~VMBase() = default;

    std::vector<uint64_t> breakpoints_;

    uint64_t program_counter_ = 0;
    
    unsigned int cycle_s = 0;
    unsigned int instructions_retired_ = 0;
    float cpi_ = 0.0;
    float ipc_ = 0.0;

    MemoryController memory_controller_;
    RegisterFile registers_;

    AssembledProgram assembleProgram(const std::string &filename);

    void loadProgram(const AssembledProgram &program);

    uint64_t getProgramCounter() const;
    void updateProgramCounter(int64_t value);

    void addBreakpoint(uint64_t address);
    void removeBreakpoint(uint64_t address);
    bool checkBreakpoint(uint64_t address);


    virtual void run();
    virtual void step();
    virtual void reset();
    virtual void dumpState(const std::string &filename);


};

#endif // VM_BASE_H
