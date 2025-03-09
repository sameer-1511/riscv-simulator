/**
 * @file vm_base.cpp
 * @brief File containing the base class for the virtual machine
 * @author Vishank Singh, https://VishankSingh
 */

#include "vm_base.h"

#include "../globals.h"



void VMBase::loadProgram(const AssembledProgram &program) {
    unsigned int counter = 0;
    for (const auto &instruction: program.text_buffer) {
        memory_controller_.writeWord(counter, static_cast<uint32_t>(instruction.to_ulong()));
        counter += 4;
    }

    program_size_ = counter;
    addBreakpoint(program_size_);

    unsigned int data_counter = 0;
    uint64_t base_data_address = globals::data_section_start;
    for (const auto& data : program.data_buffer) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;  // Deduce the type of the value
            if constexpr (std::is_same_v<T, uint8_t>) {
                memory_controller_.writeByte(base_data_address + data_counter, value);  // Write a byte
                data_counter += 1;
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                memory_controller_.writeHalfWord(base_data_address + data_counter, value);  // Write a halfword (16 bits)
                data_counter += 2;
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                memory_controller_.writeWord(base_data_address + data_counter, value);  // Write a word (32 bits)
                data_counter += 4;
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                memory_controller_.writeDoubleWord(base_data_address + data_counter, value);  // Write a double word (64 bits)
                data_counter += 8;
            } else if constexpr (std::is_same_v<T, std::string>) {
                for (size_t i = 0; i < value.size(); i++) {
                    memory_controller_.writeByte(base_data_address + data_counter, static_cast<uint8_t>(value[i]));  // Write each byte of the string
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

auto sign_extend = [](uint32_t value, unsigned int bits) -> int32_t {
    int32_t mask = 1 << (bits - 1);
    return (value ^ mask) - mask;
};


int32_t VMBase::imm_generator(uint32_t instruction) {
    int32_t imm = 0;
    uint8_t opcode = instruction & 0b1111111;

    switch (opcode) {
        /*** I-TYPE (Load, ALU Immediate, JALR, FPU Loads) ***/
        case 0b0010011: // ALU Immediate (ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI)
        case 0b0000011: // Load (LB, LH, LW, LD, LBU, LHU, LWU)
        case 0b1100111: // JALR
        case 0b0001111: // FENCE
        case 0b0000111: // FLW, FLD (Floating-point load)
            imm = (instruction >> 20) & 0xFFF;
            imm = sign_extend(imm, 12);
            break;

        /*** S-TYPE (Store, Floating-Point Store) ***/
        case 0b0100011: // Store (SB, SH, SW, SD)
        case 0b0100111: // FSW, FSD (Floating-point store)
            imm = ((instruction >> 7) & 0x1F) | ((instruction >> 25) & 0x7F) << 5;
            imm = sign_extend(imm, 12);
            break;

        /*** SB-TYPE (Branch Instructions) ***/
        case 0b1100011: // Branch (BEQ, BNE, BLT, BGE, BLTU, BGEU)
            imm = ((instruction >> 8) & 0xF) // Bits 11:8
                  | ((instruction >> 25) & 0x3F) << 4 // Bits 10:5
                  | ((instruction >> 7) & 0x1) << 10 // Bit 4
                  | ((instruction >> 31) & 0x1) << 11; // Bit 12
            imm <<= 1;
            imm = sign_extend(imm, 13);
            break;

        /*** U-TYPE (LUI, AUIPC) ***/
        case 0b0110111: // LUI
        case 0b0010111: // AUIPC
            imm = (instruction & 0xFFFFF000) >> 12;  // Upper 20 bits
            
            break;

        /*** J-TYPE (JAL) ***/
        case 0b1101111: // JAL
            imm = ((instruction >> 21) & 0x3FF)  // Bits 10:1
                | ((instruction >> 20) & 0x1) << 10  // Bit 11
                | ((instruction >> 12) & 0xFF) << 11  // Bits 19:12
                | ((instruction >> 31) & 0x1) << 19;  // Bit 20
            imm <<= 1;  // Shift left by 1
            // if (imm & 0x1000) {
            //    imm |= 0xFFFFE000;
            // }
            imm = sign_extend(imm, 21); // Might be 20
            break;

        /*** M-EXTENSION (Multiplication, Division) - R-TYPE ***/
        case 0b0110011: // MUL, MULH, MULHU, MULHSU, DIV, DIVU, REM, REMU
            // R-Type (no immediate needed)
            imm = 0;
            break;

        /*** F-EXTENSION (Floating Point Operations) - R-TYPE ***/
        case 0b1010011: // Floating-point (FADD, FSUB, FMUL, FDIV, FSQRT, etc.)
            // R-Type (no immediate needed)
            imm = 0;
            break;

        /*** D-EXTENSION (Double-Precision Floating Point) ***/
        case 0b1000011: // FLD (Double floating-point load)
//        case 0b1100011: // FSD (Double floating-point store)
            imm = (instruction >> 20) & 0xFFF;
            imm = sign_extend(imm, 12);
            break;

        default:
            imm = 0;
            break;
    }

    return imm;
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