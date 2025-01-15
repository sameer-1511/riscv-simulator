/**
 * @file vm_asm_mw.h
 * @brief Contains functions to handle assembler input for the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef VM_ASM_MW_H
#define VM_ASM_MW_H

#include "pch.h"

struct VM_asm_input {
    std::vector<std::variant<>> data_buffer;
};

#endif // VM_ASM_MW_H