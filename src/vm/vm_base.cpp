/**
 * @file vm_base.cpp
 * @brief File containing the base class for the virtual machine
 * @author Vishank Singh, https://VishankSingh
 */

#include "vm_base.h"

#include "../globals.h"




// TODO: Implement the assembleProgram function
// Right now it just returns a mock AssembledProgram
AssembledProgram VMBase::assembleProgram(const std::string &filename) {
    AssembledProgram program;
    program.filename = filename;
    program.instruction_buffer = {
            std::bitset<32>(0b00000000000000000000000000110011),
            std::bitset<32>(0b00000000000000000000000000110011),
    };
    return program;
}

void VMBase::loadProgram(const AssembledProgram &program) {
    unsigned int counter = 0;
    for (const auto &instruction: program.instruction_buffer) {
        memory_controller_.writeWord(counter, static_cast<uint32_t>(instruction.to_ulong()));
        counter += 4;
    }

    unsigned int data_counter = 0;
    for (const auto& data : program.data_buffer) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;  // Deduce the type of the value
            if constexpr (std::is_same_v<T, uint8_t>) {
                memory_controller_.writeByte(data_counter, value);  // Write a byte
                data_counter += 1;
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                memory_controller_.writeHalfWord(data_counter, value);  // Write a halfword (16 bits)
                data_counter += 2;
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                memory_controller_.writeWord(data_counter, value);  // Write a word (32 bits)
                data_counter += 4;
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                memory_controller_.writeDoubleWord(data_counter, value);  // Write a double word (64 bits)
                data_counter += 8;
            } else if constexpr (std::is_same_v<T, std::string>) {
                for (size_t i = 0; i < value.size(); i++) {
                    memory_controller_.writeByte(data_counter, static_cast<uint8_t>(value[i]));  // Write each byte of the string
                    data_counter += 1;
                }
            }
        }, data);
    }
}

uint64_t VMBase::getProgramCounter() const {
    return program_counter_;
}

void VMBase::updateProgramCounter(int64_t value) {
    program_counter_ += value;
}

void VMBase::addBreakpoint(uint64_t address) {
    breakpoints_.emplace_back(address);
}

void VMBase::removeBreakpoint(uint64_t address) {
    breakpoints_.erase(std::remove(breakpoints_.begin(), breakpoints_.end(), address), breakpoints_.end());
}

bool VMBase::checkBreakpoint(uint64_t address) {
    return std::find(breakpoints_.begin(), breakpoints_.end(), address) != breakpoints_.end();
}


// void VMBase::run() {
//     while (true) {
//         step();
//     }
// }

// void VMBase::step() {
//     if (checkBreakpoint(program_counter_)) {
//         dumpState("breakpoint_dump.txt");
//         return;
//     }
// 
//     uint32_t instruction = memory_controller_.readWord(program_counter_);
//     std::bitset<32> instruction_bits(instruction);
//     std::cout << "Executing instruction: " << instruction_bits << std::endl;
// 
// 
//     updateProgramCounter(4);
//     instructions_retired_++;
// }