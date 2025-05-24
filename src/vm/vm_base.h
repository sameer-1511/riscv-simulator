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

class VmBase {
public:
    VmBase() = default;
    ~VmBase() = default;

    // AssembledProgram program_;

    std::vector<uint64_t> breakpoints_;

    uint32_t current_instruction_{};
    uint64_t program_counter_{};
    
    unsigned int cycle_s_{};
    unsigned int instructions_retired_{};
    float cpi_{};
    float ipc_{};
    unsigned int stall_cycles_{};
    unsigned int branch_mispredictions_{};



    MemoryController memory_controller_;
    RegisterFile registers_;
    
    alu::Alu alu_;


    void LoadProgram(const AssembledProgram &program);
    uint64_t program_size_ = 0;

    uint64_t GetProgramCounter() const;
    void UpdateProgramCounter(int64_t value);
    
    int32_t ImmGenerator(uint32_t instruction);

    void AddBreakpoint(uint64_t address);
    void RemoveBreakpoint(uint64_t address);
    bool CheckBreakpoint(uint64_t address);

    // void fetchInstruction();
    // void decodeInstruction();
    // void executeInstruction();
    // void memoryAccess();
    // void writeback();

    void HandleSyscall();

    virtual void Run() = 0;
    virtual void DebugRun() = 0;
    virtual void Step() = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
    virtual void Reset() = 0;
    virtual void DumpState(const std::string &filename) = 0;

};

#endif // VM_BASE_H
