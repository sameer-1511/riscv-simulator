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
    for (const auto &pair : IntermediateCode) {
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

std::bitset<32> generateRTypeMachineCode(const ICUnit &block) {
    InstructionSet::RTypeInstructionEncoding
        encoding = InstructionSet::R_type_instruction_encoding_map.at(block.getOpcode());
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
    auto rd = std::bitset<5>(0);
    auto rs1 = std::bitset<5>(0);
    auto imm = std::bitset<12>(0);
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
    std::bitset<5> rs1 = std::bitset<5>(std::stoi(block.getRs1().substr(1)));
    std::bitset<5> rs2 = std::bitset<5>(std::stoi(block.getRs2().substr(1)));
    std::bitset<12> imm = std::bitset<12>(std::stoi(block.getImm()));
    code |= encoding.opcode.to_ulong();
    code |= (imm.to_ulong() & 0x1F) << 7; // imm[4:0]
    code |= (imm.to_ulong() & 0xFE0) << 20; // imm[11:5]
    code |= (rs2.to_ulong() << 20);
    code |= (rs1.to_ulong() << 15);
    code |= (encoding.funct3.to_ulong() << 12);

    return code;
}

std::bitset<32> generateBTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::B_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rs1 = std::bitset<5>(std::stoi(block.getRs1().substr(1)));
    std::bitset<5> rs2 = std::bitset<5>(std::stoi(block.getRs2().substr(1)));
    std::bitset<13> imm = std::bitset<13>(std::stoi(block.getImm()));
    code |= encoding.opcode.to_ulong();
    code |= ((imm.to_ulong() & 0x1E) << 7); // imm[4:1]
    code |= ((imm.to_ulong() & 0x800) >> 4); // imm[11]
    code |= ((imm.to_ulong() & 0x7E0) << 20); // imm[10:5]
    code |= ((imm.to_ulong() & 0x1000) << 19); // imm[12]
    code |= (rs2.to_ulong() << 20);
    code |= (rs1.to_ulong() << 15);
    code |= (encoding.funct3.to_ulong() << 12);
    return code;
}

std::bitset<32> generateUTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::U_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<20> imm = std::bitset<20>(std::stoi(block.getImm()));
    code |= encoding.opcode.to_ulong();
    code |= (imm.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    return code;
}

std::bitset<32> generateJTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::J_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<20> imm = std::bitset<20>(std::stoi(block.getImm()));
    code |= encoding.opcode.to_ulong();
    code |= ((imm.to_ulong() & 0xFF000) << 0); // imm[19:12]
    code |= ((imm.to_ulong() & 0x800) << 9); // imm[11]
    code |= ((imm.to_ulong() & 0x7FE) << 20); // imm[10:1]
    code |= ((imm.to_ulong() & 0x80000) << 12); // imm[20]
    code |= (rd.to_ulong() << 7);
    return code;
}

std::bitset<32> genarateCSRRTypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::CSR_R_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<5> rs1 = std::bitset<5>(std::stoi(block.getRs1().substr(1)));
    std::bitset<12> csr = std::bitset<12>(block.getCsr() & 0xFFF);
    code |= encoding.opcode.to_ulong();
    code |= (rs1.to_ulong() << 15);
    code |= (csr.to_ulong() << 20);
    code |= (encoding.funct3.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    return code;
}

std::bitset<32> genarateCSRITypeMachineCode(const ICUnit &block) {
    auto encoding = InstructionSet::CSR_I_type_instruction_encoding_map.at(block.getOpcode());
    std::bitset<32> code;
    std::bitset<5> rd = std::bitset<5>(std::stoi(block.getRd().substr(1)));
    std::bitset<3> imm = std::bitset<3>(std::stoi(block.getImm()));
    std::bitset<12> csr = std::bitset<12>(block.getCsr() & 0xFFF);
    code |= encoding.opcode.to_ulong();
    code |= (imm.to_ulong() << 15);
    code |= (csr.to_ulong() << 20);
    code |= (encoding.funct3.to_ulong() << 12);
    code |= (rd.to_ulong() << 7);
    return code;
}

// TODO: START HERE
std::bitset<32> generateFDRTypeMachineCode(const ICUnit &block) {

}

std::bitset<32> generateFDR1TypeMachineCode(const ICUnit &block) {

}

std::bitset<32> generateFDR2TypeMachineCode(const ICUnit &block) {

}

std::bitset<32> generateFDR3TypeMachineCode(const ICUnit &block) {

}

std::bitset<32> generateFDR4TypeMachineCode(const ICUnit &block) {

}

std::bitset<32> generateFDITypeMachineCode(const ICUnit &block) {

}

std::bitset<32> generateFDSTypeMachineCode(const ICUnit &block) {

}


std::vector<std::bitset<32>> generateMachineCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode) {
    std::vector<std::bitset<32>> machine_code;
    for (const auto &pair : IntermediateCode) {
        const ICUnit &block = pair.first;
        std::bitset<32> code;
        if (InstructionSet::isValidRTypeInstruction(block.getOpcode())) {
            code = generateRTypeMachineCode(block);
        } else if (InstructionSet::isValidI1TypeInstruction(block.getOpcode())) {
            code = generateI1TypeMachineCode(block);
        } else if (InstructionSet::isValidI2TypeInstruction(block.getOpcode())) {
            code = generateI2TypeMachineCode(block);
        } else if (InstructionSet::isValidI3TypeInstruction(block.getOpcode())) {
            code = generateI3TypeMachineCode(block);
        } else if (InstructionSet::isValidSTypeInstruction(block.getOpcode())) {
            code = generateSTypeMachineCode(block);
        } else if (InstructionSet::isValidBTypeInstruction(block.getOpcode())) {
            code = generateBTypeMachineCode(block);
        } else if (InstructionSet::isValidUTypeInstruction(block.getOpcode())) {
            code = generateUTypeMachineCode(block);
        } else if (InstructionSet::isValidJTypeInstruction(block.getOpcode())) {
            code = generateJTypeMachineCode(block);
        } else if (InstructionSet::isValidCSRRTypeInstruction(block.getOpcode())) {
            code = genarateCSRRTypeMachineCode(block);
        } else if (InstructionSet::isValidCSRITypeInstruction(block.getOpcode())) {
            code = genarateCSRITypeMachineCode(block);
        } else if (InstructionSet::isValidFDRTypeInstruction(block.getOpcode())) {
            code = generateFDRTypeMachineCode(block);
        } else if (InstructionSet::isValidFDR1TypeInstruction(block.getOpcode())) {
            code = generateFDR1TypeMachineCode(block);
        } else if (InstructionSet::isValidFDR2TypeInstruction(block.getOpcode())) {
            code = generateFDR2TypeMachineCode(block);
        } else if (InstructionSet::isValidFDR3TypeInstruction(block.getOpcode())) {
            code = generateFDR3TypeMachineCode(block);
        } else if (InstructionSet::isValidFDR4TypeInstruction(block.getOpcode())) {
            code = generateFDR4TypeMachineCode(block);
        } else if (InstructionSet::isValidFDITypeInstruction(block.getOpcode())) {
            code = generateFDITypeMachineCode(block);
        } else if (InstructionSet::isValidFDSTypeInstruction(block.getOpcode())) {
            code = generateFDSTypeMachineCode(block);
        }
        
        
        
        
        
        
        
        else {
            throw std::runtime_error("Invalid instruction type");
        }
        machine_code.push_back(code);
    }
    return machine_code;
}



