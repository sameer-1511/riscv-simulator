/**
 * @file code_generator.h
 * @brief Contains the definition of the ICUnit struct and functions for generating machine code.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "../pch.h"

/**
 * @brief Represents a unit of intermediate code used for generating machine code.
 * 
 * This struct stores the details of an intermediate code block, including the line number, 
 * opcode, register destinations, immediate values, and labels. It provides setter and getter 
 * methods to manage the data safely.
 */
struct ICUnit {
    unsigned int line_number; ///< Line number in the source code corresponding to this block.
    std::array<char, 21> opcode;  ///< Opcode string (up to 20 characters, null-terminated).
    std::array<char, 21> rd;      ///< Destination register name (up to 20 characters, null-terminated).
    std::array<char, 21> rs1;     ///< Source register 1 name (up to 20 characters, null-terminated).
    std::array<char, 21> rs2;     ///< Source register 2 name (up to 20 characters, null-terminated).
    std::array<char, 33> imm;     ///< Immediate value (up to 32 characters, null-terminated).
    std::string label;            ///< Label associated with this code block, if any.

    /**
     * @brief Default constructor initializing all members to default values.
     */
    ICUnit() : line_number{}, opcode{}, rd{}, rs1{}, rs2{}, imm{}, label{} {}

     /**
     * @brief Overloaded stream insertion operator for displaying the ICUnit.
     * 
     * @param os The output stream.
     * @param unit The ICUnit to display.
     * @return The modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const ICUnit &unit) {
        os << unit.opcode.data() << " "
           << unit.rd.data() << " "
           << unit.rs1.data() << " "
           << unit.rs2.data() << " "
           << unit.imm.data() << " "
           << unit.label;
        return os;
    }

    /**
     * @brief Sets the line number for the intermediate code block.
     * 
     * @param value The line number to set.
     */
    void setLineNumber(unsigned int value) {
        line_number = value;
    }

    /**
     * @brief Sets the opcode for the intermediate code block.
     * 
     * @param value The opcode string (up to 20 characters).
     */
    void setOpcode(const std::string &value) {
        strncpy(opcode.data(), value.c_str(), 20);
        opcode[20] = '\0';  // Null-terminate to ensure no overflow
    }

    /**
     * @brief Sets the destination register (rd).
     * 
     * @param value The destination register string (up to 20 characters).
     */
    void setRd(const std::string &value) {
        strncpy(rd.data(), value.c_str(), 20);
        rd[20] = '\0';
    }

    /**
     * @brief Sets the source register 1 (rs1).
     * 
     * @param value The source register 1 string (up to 20 characters).
     */
    void setRs1(const std::string &value) {
        strncpy(rs1.data(), value.c_str(), 20);
        rs1[20] = '\0';
    }

    /**
     * @brief Sets the source register 2 (rs2).
     * 
     * @param value The source register 2 string (up to 20 characters).
     */
    void setRs2(const std::string &value) {
        strncpy(rs2.data(), value.c_str(), 20);
        rs2[20] = '\0';
    }

    /**
     * @brief Sets the immediate value for the intermediate code block.
     * 
     * @param value The immediate value string (up to 32 characters).
     */
    void setImm(const std::string &value) {
        strncpy(imm.data(), value.c_str(), 32);
        imm[32] = '\0';
    }

    /**
     * @brief Sets the label for the intermediate code block.
     * 
     * @param value The label string.
     */
    void setLabel(const std::string &value) {
        label = value;
    }

    /**
     * @brief Gets the line number for the intermediate code block.
     * 
     * @return The line number.
     */
    [[nodiscard]] unsigned int getLineNumber() const {
        return line_number;
    }

    /**
     * @brief Gets the opcode for the intermediate code block.
     * 
     * @return The opcode string.
     */
    [[nodiscard]] std::string getOpcode() const {
        return opcode.data();
    }

    /**
     * @brief Gets the destination register (rd).
     * 
     * @return The destination register string.
     */
    [[nodiscard]] std::string getRd() const {
        return rd.data();
    }

    /**
     * @brief Gets the source register 1 (rs1).
     * 
     * @return The source register 1 string.
     */
    [[nodiscard]] std::string getRs1() const {
        return rs1.data();
    }

    /**
     * @brief Gets the source register 2 (rs2).
     * 
     * @return The source register 2 string.
     */
    [[nodiscard]] std::string getRs2() const {
        return rs2.data();
    }

    /**
     * @brief Gets the immediate value for the intermediate code block.
     * 
     * @return The immediate value string.
     */
    [[nodiscard]] std::string getImm() const {
        return imm.data();
    }

    /**
     * @brief Gets the label for the intermediate code block.
     * 
     * @return The label string.
     */
    [[nodiscard]] std::string getLabel() const {
        return label;
    }
};

/**
 * @brief Prints the intermediate code to a vector of strings.
 * 
 * @param IntermediateCode A vector of pairs containing ICUnit and a boolean flag.
 * @return A vector of strings representing the intermediate code.
 */
std::vector<std::string> printIntermediateCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode);

/**
 * @brief Generates machine code for an R-type instruction.
 * 
 * @param block The ICUnit representing the instruction.
 * @return The machine code string.
 */
std::string generateRTypeMachineCode(const ICUnit &block);

/**
 * @brief Generates machine code for an I-type instruction.
 * 
 * @param block The ICUnit representing the instruction.
 * @return The machine code string.
 */
std::string generateITypeMachineCode(const ICUnit &block);

/**
 * @brief Generates machine code for an S-type instruction.
 * 
 * @param block The ICUnit representing the instruction.
 * @return The machine code string.
 */
std::string generateSTypeMachineCode(const ICUnit &block);

/**
 * @brief Generates machine code for a B-type instruction.
 * 
 * @param block The ICUnit representing the instruction.
 * @return The machine code string.
 */
std::string generateBTypeMachineCode(const ICUnit &block);

/**
 * @brief Generates machine code for a U-type instruction.
 * 
 * @param block The ICUnit representing the instruction.
 * @return The machine code string.
 */
std::string generateUTypeMachineCode(const ICUnit &block);

/**
 * @brief Generates machine code for a J-type instruction.
 * 
 * @param block The ICUnit representing the instruction.
 * @return The machine code string.
 */
std::string generateJTypeMachineCode(const ICUnit &block);

/**
 * @brief Generates machine code from a vector of intermediate code blocks.
 * 
 * @param IntermediateCode A vector of pairs containing ICUnit and a boolean flag.
 * @return A vector of strings representing the machine code.
 */
std::vector<std::string> generateMachineCode(const std::vector<std::pair<ICUnit, bool>> &IntermediateCode);


#endif // CODE_GENERATOR_H
