/** @cond DOXYGEN_IGNORE */
/**
 * File Name: code_generator.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
/** @endcond */

#include "code_generator.h"
#include "../pch.h"
#include "instructions.h"

std::vector<std::string> printIntermediateCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode) {
    std::vector<std::string> ICList;
    for (const auto &pair: IntermediateCode) {
        const ICUnit &block = pair.first;
        std::string code;

        if (isValidRTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getRs1() + " " + block.getRs2();
        } else if (isValidITypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getRs1() + " " + block.getImm();
        } else if (isValidSTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRs2() + " " + block.getImm() + "(" + block.getRs1() + ")";
        } else if (isValidBTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRs1() + " " + block.getRs2() + " " + block.getImm() + " <" +
                   block.getLabel() + ">";
        } else if (isValidUTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getImm();
        } else if (isValidJTypeInstruction(block.getOpcode())) {
            code = block.getOpcode() + " " + block.getRd() + " " + block.getImm() + " <" + block.getLabel() + ">";
        } else {
            code = block.getOpcode() + " " + block.getImm();
        }


        ICList.push_back(code);
    }
    return ICList;
}

std::string generateRTypeMachineCode(const ICUnit &block) {
    InstructionEncoding encoding = instruction_encoding_map.at(block.getOpcode());

    std::string rd = block.getRd();
    rd = rd.substr(1);
    int rd_int = std::stoi(rd);
    rd = std::bitset<5>(rd_int).to_string();

    std::string rs1 = block.getRs1();
    rs1 = rs1.substr(1);
    int rs1_int = std::stoi(rs1);
    rs1 = std::bitset<5>(rs1_int).to_string();

    std::string rs2 = block.getRs2();
    rs2 = rs2.substr(1);
    int rs2_int = std::stoi(rs2);
    rs2 = std::bitset<5>(rs2_int).to_string();

    std::string machine_code = encoding.funct7 + rs2 + rs1 + encoding.funct3 + rd + encoding.opcode;
    return machine_code;
}

std::string generateITypeMachineCode(const ICUnit &block) {
    InstructionEncoding encoding = instruction_encoding_map.at(block.getOpcode());

    std::string imm = block.getImm();
    int imm_int = std::stoi(imm);
    imm = std::bitset<12>(imm_int).to_string();

    std::string rd = block.getRd();
    rd = rd.substr(1);
    int rd_int = std::stoi(rd);
    rd = std::bitset<5>(rd_int).to_string();

    std::string rs1 = block.getRs1();
    rs1 = rs1.substr(1);
    int rs1_int = std::stoi(rs1);
    rs1 = std::bitset<5>(rs1_int).to_string();

    std::string machine_code = imm + rs1 + encoding.funct3 + rd + encoding.opcode;
    return machine_code;
}

std::string generateSTypeMachineCode(const ICUnit &block) {
    InstructionEncoding encoding = instruction_encoding_map.at(block.getOpcode());

    std::string imm = block.getImm();
    int imm_int = std::stoi(imm);
    imm = std::bitset<12>(imm_int).to_string();

    std::string rs1 = block.getRs1();
    rs1 = rs1.substr(1);
    int rs1_int = std::stoi(rs1);
    rs1 = std::bitset<5>(rs1_int).to_string();

    std::string rs2 = block.getRs2();
    rs2 = rs2.substr(1);
    int rs2_int = std::stoi(rs2);
    rs2 = std::bitset<5>(rs2_int).to_string();

    std::string machine_code = imm.substr(0, 7) + rs2 + rs1 + encoding.funct3 + imm.substr(7, 5) + encoding.opcode;
    return machine_code;
}

std::string generateBTypeMachineCode(const ICUnit &block) {
    InstructionEncoding encoding = instruction_encoding_map.at(block.getOpcode());

    std::string imm = block.getImm();
    int imm_int = std::stoi(imm);
    imm = std::bitset<13>(imm_int).to_string();

    std::string rs1 = block.getRs1();
    rs1 = rs1.substr(1);
    int rs1_int = std::stoi(rs1);
    rs1 = std::bitset<5>(rs1_int).to_string();

    std::string rs2 = block.getRs2();
    rs2 = rs2.substr(1);
    int rs2_int = std::stoi(rs2);
    rs2 = std::bitset<5>(rs2_int).to_string();

    std::string machine_code =
            imm.substr(0, 1) + imm.substr(2, 6) + rs2 + rs1 + encoding.funct3 + imm.substr(8, 4) + imm.substr(1, 1) +
            encoding.opcode;
    return machine_code;
}

std::string generateUTypeMachineCode(const ICUnit &block) {
    InstructionEncoding encoding = instruction_encoding_map.at(block.getOpcode());


    std::string imm = block.getImm();
    int imm_int = std::stoi(imm);
    imm = std::bitset<20>(imm_int).to_string();

    std::string rd = block.getRd();
    rd = rd.substr(1);
    int rd_int = std::stoi(rd);
    rd = std::bitset<5>(rd_int).to_string();

    std::string machine_code = imm + rd + encoding.opcode;
    return machine_code;
}

std::string generateJTypeMachineCode(const ICUnit &block) {
    InstructionEncoding encoding = instruction_encoding_map.at(block.getOpcode());

    std::string imm = block.getImm();
    int imm_int = std::stoi(imm);
    imm = std::bitset<21>(imm_int).to_string();

    std::string rd = block.getRd();
    rd = rd.substr(1);
    int rd_int = std::stoi(rd);
    rd = std::bitset<5>(rd_int).to_string();

    std::string machine_code =
            imm.substr(0, 1) + imm.substr(10, 10) + imm.substr(9, 1) + imm.substr(1, 8) + rd + encoding.opcode;
    return machine_code;
}


std::vector<std::string> generateMachineCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode) {
    std::vector<std::string> machine_code;
    for (const auto &pair: IntermediateCode) {
        const ICUnit &block = pair.first;
        std::string code;
        if (isValidRTypeInstruction(block.getOpcode())) {
            code = generateRTypeMachineCode(block);
        } else if (isValidITypeInstruction(block.getOpcode())) {
            code = generateITypeMachineCode(block);
        } else if (isValidSTypeInstruction(block.getOpcode())) {
            code = generateSTypeMachineCode(block);
        } else if (isValidBTypeInstruction(block.getOpcode())) {
            code = generateBTypeMachineCode(block);
        } else if (isValidUTypeInstruction(block.getOpcode())) {
            code = generateUTypeMachineCode(block);
        } else if (isValidJTypeInstruction(block.getOpcode())) {
            code = generateJTypeMachineCode(block);
        }
        machine_code.push_back(code);
    }
    return machine_code;
}



