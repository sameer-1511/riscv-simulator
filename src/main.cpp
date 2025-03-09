#include "pch.h"

#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

#include "./assembler/assembler.h"

#include "./vm/memory_controller.h"

#include "./assembler/elf_util.h"
#include "utils.h"
#include "globals.h"

int main() {
    AssembledProgram program;
    try {
        program = assemble("/home/vis/Desk/codes/assembler/examples/load_store_test.s");
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 0;
    }

    std::cout << "Program: " << program.filename << std::endl;

    //for (const auto &pair: program.instruction_number_line_number_mapping) {
    //    std::cout << pair.first << " -> " << pair.second << '\n';
    //}
    //std::cout << std::endl;
    //for (const auto &pair: program.line_number_instruction_number_mapping) {
    //    std::cout << pair.first << " -> " << pair.second << '\n';
    //}



    unsigned int count = 0;
    for (const std::bitset<32> &instruction : program.text_buffer) {
        std::cout << instruction
                  << " | "
                  << std::setw(8) << std::setfill('0') << std::hex << instruction.to_ulong()
                  << " | "
                  << std::setw(0) << count
                  << std::dec << "\n";
        count += 4;
    }

    // generateElfFile(program, "/home/vis/Desk/codes/assembler/examples/e1.elf");

    // std::cout << "globals::config_file: " << globals::config_file << std::endl;


    // MemoryController memoryController;
    // memoryController.printCacheStatus();

    std::array<uint64_t, 32> registers;
    for (int i = 0; i < 32; i++) {
        registers[i] = i;
    }

    dumpRegisters(globals::registers_dump_file, registers);

    



    return 0;
}