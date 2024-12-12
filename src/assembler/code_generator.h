/**
 * File Name: code_generator.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "../pch.h"

/*
 * This struct is used to store the intermediate code block
 * which is used to generate the final machine code.
*/
struct ICUnit {
    std::array<char, 21> opcode;  // 20 characters + null terminator
    std::array<char, 21> rd;
    std::array<char, 21> rs1;
    std::array<char, 21> rs2;
    std::array<char, 33> imm;
    std::string label;

    ICUnit() : opcode{}, rd{}, rs1{}, rs2{}, imm{}, label{} {}

    friend std::ostream& operator<<(std::ostream& os, const ICUnit& unit) {
        os << unit.opcode.data() << " " 
           << unit.rd.data() << " " 
           << unit.rs1.data() << " " 
           << unit.rs2.data() << " " 
           << unit.imm.data() << " "
           << unit.label;
        return os;
    } 
    
    
    // Setter functions to ensure max size is 20 characters
    void setOpcode(const std::string& value) {
        strncpy(opcode.data(), value.c_str(), 20);
        opcode[20] = '\0';  // Null-terminate to ensure no overflow
    }

    void setRd(const std::string& value) {
        strncpy(rd.data(), value.c_str(), 20);
        rd[20] = '\0';
    }

    void setRs1(const std::string& value) {
        strncpy(rs1.data(), value.c_str(), 20);
        rs1[20] = '\0';
    }

    void setRs2(const std::string& value) {
        strncpy(rs2.data(), value.c_str(), 20);
        rs2[20] = '\0';
    }

    void setImm(const std::string& value) {
        strncpy(imm.data(), value.c_str(), 32);
        imm[32] = '\0';
    }

    void setLabel(const std::string& value) {
        label = value;
    }

    std::string getOpcode() const {
        return opcode.data();
    }

    std::string getRd() const {
        return rd.data();
    }

    std::string getRs1() const {
        return rs1.data();
    }

    std::string getRs2() const {
        return rs2.data();
    }

    std::string getImm() const {
        return imm.data();
    }

    std::string getLabel() const {
        return label;
    }
};

std::vector<std::string> printIntermediateCode(const std::vector<std::pair<ICUnit, bool>>& IntermediateCode);

std::string generateRTypeMachineCode(const ICUnit& block);
std::string generateITypeMachineCode(const ICUnit& block);
std::string generateSTypeMachineCode(const ICUnit& block);
std::string generateBTypeMachineCode(const ICUnit& block);
std::string generateUTypeMachineCode(const ICUnit& block);
std::string generateJTypeMachineCode(const ICUnit& block);

std::vector<std::string> generateMachineCode(const std::vector<std::pair<ICUnit, bool>>& IntermediateCode);


#endif // CODE_GENERATOR_H