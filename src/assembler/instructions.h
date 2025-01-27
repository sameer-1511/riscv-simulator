/**
 * @file instructions.h
 * @brief Contains the declarations for the InstructionEncoding struct and related functions.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../pch.h"

namespace InstructionSet {

/**
 * @brief Encodes the components of an instruction.
 * 
 * This struct holds the encoding for the opcode, funct3, funct7, and funct6 fields
 * of an instruction, which are used for representing the instruction in machine code.
 */
struct InstructionEncoding {
    std::string opcode;   ///< The opcode part of the instruction.
    std::string funct3;   ///< The funct3 part of the instruction.
    std::string funct7;   ///< The funct7 part of the instruction.
    std::string funct6;   ///< The funct6 part of the instruction.

    /**
     * @brief Constructs an InstructionEncoding object.
     * 
     * @param opcode The opcode field.
     * @param funct3 The funct3 field.
     * @param funct7 The funct7 field.
     * @param funct6 The funct6 field.
     */
    InstructionEncoding(std::string opcode, std::string funct3, std::string funct7, std::string funct6)
            : opcode(std::move(opcode)), funct3(std::move(funct3)), funct7(std::move(funct7)), funct6(std::move(funct6)) {}
};

struct RTypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<7> funct7;

    RTypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct7)
            : opcode(opcode), funct3(funct3), funct7(funct7) {}
};

struct ITypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::optional<std::bitset<6>> funct6;

    ITypeInstructionEncoding(unsigned int opcode, unsigned int funct3, std::optional<unsigned int> funct6)
            : opcode(opcode), funct3(funct3), funct6(funct6) {}
};

struct STypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    STypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
            : opcode(opcode), funct3(funct3) {}
};

struct BTypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    BTypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
            : opcode(opcode), funct3(funct3) {}
};

struct UTypeInstructionEncoding {
    std::bitset<7> opcode;

    UTypeInstructionEncoding(unsigned int opcode)
            : opcode(opcode) {}
};

struct JTypeInstructionEncoding {
    std::bitset<7> opcode;

    JTypeInstructionEncoding(unsigned int opcode)
            : opcode(opcode) {}
};

struct MExtensionInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<7> funct7;

    MExtensionInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct7)
            : opcode(opcode), funct3(funct3), funct7(funct7) {}
};

struct FExtensionInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::optional<std::bitset<7>> funct7;

    FExtensionInstructionEncoding(unsigned int opcode, unsigned int funct3)
            : opcode(opcode), funct3(funct3) {}
};

/**
 * @brief Enum that represents different syntax types for instructions.
 */
enum class SyntaxType {
    O_R_C_R_C_R,        ///< Opcode register , register , register
    O_R_C_R_C_I,        ///< Opcode register , register , immediate
    O_R_C_I,            ///< Opcode register , immediate
    O_R_C_R_C_IL,       ///< Opcode register , register , immediate , instruction_label
    O_R_C_R_C_DL,       ///< Opcode register , register , immediate , data_label
    O_R_C_IL,           ///< Opcode register , instruction_label
    O_R_C_DL,           ///< Opcode register , data_label
    O_R_C_I_LP_R_RP,    ///< Opcode register , immediate , lparen , register , rparen
    O,                  ///< Opcode
    PSEUDO              ///< Pseudo instruction
};

/**
 * @brief A map that associates instruction names with their corresponding encoding.
 * 
 * This map stores the encoding details for various instructions, indexed by their names.
 */
extern std::unordered_map<std::string, InstructionEncoding> instruction_encoding_map;

/**
 * @brief A map that associates instruction names with their expected syntax.
 * 
 * This map stores the expected syntax for various instructions, indexed by their names.
 */
extern std::unordered_map<std::string, std::vector<SyntaxType>> instruction_syntax_map;

/**
 * @brief Validates if a given instruction is valid.
 * 
 * @param instruction The instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidInstruction(const std::string &instruction);

/**
 * @brief Validates if a given R-type instruction is valid.
 * 
 * @param instruction The R-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidRTypeInstruction(const std::string &name);

/**
 * @brief Validates if a given I-type instruction is valid.
 * 
 * @param instruction The I-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidITypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given S-type instruction is valid.
 * 
 * @param instruction The S-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidSTypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given B-type instruction is valid.
 * 
 * @param instruction The B-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidBTypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given U-type instruction is valid.
 * 
 * @param instruction The U-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidUTypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given J-type instruction is valid.
 * 
 * @param instruction The J-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidJTypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given pseudo-instruction is valid.
 * 
 * @param instruction The pseudo-instruction string to validate.
 * @return True if the pseudo-instruction is valid, false otherwise.
 */
bool isValidPseudoInstruction(const std::string &instruction);

/**
 * @brief Retrieves the expected syntax for a given opcode.
 * 
 * @param opcode The opcode of the instruction.
 * @return A string representing the expected syntax of the instruction.
 */
std::string getExpectedSyntaxes(const std::string &opcode);

} // namespace InstructionSet


#endif // INSTRUCTIONS_H
