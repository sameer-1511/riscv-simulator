/**
 * @file vm_asm_mw.h
 * @brief Contains functions to handle assembler input for the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef VM_ASM_MW_H
#define VM_ASM_MW_H

#include "pch.h"

struct AssembledProgram {
    std::string filename;
    std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, std::string>> data_buffer;
    std::vector<std::bitset<32>> instruction_buffer;
};

#endif // VM_ASM_MW_H