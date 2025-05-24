/**
 * @file vm_asm_mw.h
 * @brief Contains functions to handle assembler input for the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef VM_ASM_MW_H
#define VM_ASM_MW_H

#include "pch.h"

struct AssembledProgram {
  std::map<unsigned int, unsigned int> line_number_instruction_number_mapping;
  std::map<unsigned int, unsigned int> instruction_number_line_number_mapping;

  //TODO: add symbol table
  std::unordered_map<std::string, unsigned int> symbol_table;

  std::string filename;
  std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, std::string, float, double>> data_buffer;
  std::vector<std::bitset<32>> text_buffer;
};

#endif // VM_ASM_MW_H