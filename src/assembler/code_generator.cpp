/**
 * @file code_generator.cpp
 * @brief Contains the implementation of functions for generating machine code from intermediate code.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "code_generator.h"
#include "../pch.h"
#include "instructions.h"

std::vector<std::string> printIntermediateCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode) {
    std::vector<std::string> ICList;
    for (const auto &pair: IntermediateCode) {
        const ICUnit &block = pair.first;
        std::string code;

        if (InstructionSet::isValidRTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getRs1() + " " + block.getRs2();
        } else if (InstructionSet::isValidITypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getRs1() + " " + block.getImm();
        } else if (InstructionSet::isValidSTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRs2() + " " + block.getImm() + "(" + block.getRs1() + ")";
        } else if (InstructionSet::isValidBTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRs1() + " " + block.getRs2() + " " + block.getImm() + " <" +
                   block.getLabel() + ">";
        } else if (InstructionSet::isValidUTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getImm();
        } else if (InstructionSet::isValidJTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getImm() + " <" + block.getLabel() + ">";
        } else {
            code = block.getOpcode() + " " + block.getImm();
        }


        ICList.push_back(code);
    }
    return ICList;
}

// TODO: Implement the following functions, CORRECTLY!
std::bitset<32> generateRTypeMachineCode(const ICUnit &block) {
    InstructionSet::RTypeInstructionEncoding encoding = InstructionSet::R_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<5> rs1 = std::bitset<5>(std::stoi(block.getRs1().substr(1)));
    std::bitset<5> rs2 = std::bitset<5>(std::stoi(block.getRs2().substr(1)));
    code |= encoding.opcode.to_ulong();
    code |= (rs2.to_ulong() << 20);
    code |= (rs1.to_ulong() << 15);
    code |= (encoding.funct3.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    code |= (encoding.funct7.to_ulong() << 25);
    return code;
}

std::bitset<32> generateI1TypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::I1_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<5> rs1 = std::bitset<5>(std::stoi(block.getRs1().substr(1)));
    std::bitset<12> imm = std::bitset<12>(std::stoi(block.getImm()));
    code |= encoding.opcode.to_ulong();
    code |= (imm.to_ulong() << 20);
    code |= (rs1.to_ulong() << 15);
    code |= (encoding.funct3.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    return code;
}

std::bitset<32> generateI2TypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::I2_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<5> rs1 = std::bitset<5>(std::stoi(block.getRs1().substr(1)));
    std::bitset<6> imm = std::bitset<6>(std::stoi(block.getImm()));
    code |= encoding.opcode.to_ulong();
    code |= (imm.to_ulong() << 20);
    code |= (encoding.funct6.to_ulong() << 26);
    code |= (rs1.to_ulong() << 15);
    code |= (encoding.funct3.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    return code;
}

std::bitset<32> generateI3TypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::I3_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(0);
    std::bitset<5> rs1 = std::bitset<5>(0);
    std::bitset<12> imm = std::bitset<12>(0);
    code |= encoding.opcode.to_ulong();
    code |= (imm.to_ulong() << 20);
    code |= (rs1.to_ulong() << 15);
    code |= (encoding.funct3.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    return code;
}

std::bitset<32> generateSTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::S_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    return code;
}

std::bitset<32> generateBTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::B_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    return code;
}

std::bitset<32> generateUTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::U_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    return code;
}

std::bitset<32> generateJTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::J_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    return code;
}


std::vector<std::bitset<32>> generateMachineCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode) {
    std::vector<std::bitset<32>> machine_code;
    for (const auto &pair: IntermediateCode) {
        const ICUnit &block = pair.first;
        std::bitset<32> code;
        if (InstructionSet::isValidRTypeInstruction(block.getOpcode())) {
            code = generateRTypeMachineCode(block);
        } 
        else if (InstructionSet::isValidI1TypeInstruction(block.getOpcode())) {
            code = generateI1TypeMachineCode(block);
        } 
        else if (InstructionSet::isValidI2TypeInstruction(block.getOpcode())) {
            code = generateI2TypeMachineCode(block);
        } 
        else if (InstructionSet::isValidI3TypeInstruction(block.getOpcode())) {
            code = generateI3TypeMachineCode(block);
        } 
        else if (InstructionSet::isValidSTypeInstruction(block.getOpcode())) {
            code = generateSTypeMachineCode(block);
        } 
        else if (InstructionSet::isValidBTypeInstruction(block.getOpcode())) {
            code = generateBTypeMachineCode(block);
        } 
        else if (InstructionSet::isValidUTypeInstruction(block.getOpcode())) {
            code = generateUTypeMachineCode(block);
        } 
        else if (InstructionSet::isValidJTypeInstruction(block.getOpcode())) {
            code = generateJTypeMachineCode(block);
        }
        machine_code.push_back(code);
    }
    return machine_code;
}



