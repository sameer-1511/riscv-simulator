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

struct I1TypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    I1TypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
            : opcode(opcode), funct3(funct3){}
};

struct I2TypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<6> funct6;

    I2TypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct6)
            : opcode(opcode), funct3(funct3), funct6(funct6) {}
};

struct I3TypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<7> funct7;

    I3TypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct7)
            : opcode(opcode), funct3(funct3), funct7(funct7) {}
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
 * @brief Enum that represents different types of opcodes.
 */
enum class InstructionType {
    R,      ///< R-type instruction
    I1,     ///< I-type instruction with 12-bit immediate
    I2,     ///< I-type instruction with 6-bit immediate and a funct6 field
    I3,     ///< I-type instruction of ecall and ebreak
    S,      ///< S-type instruction
    B,      ///< B-type instruction
    U,      ///< U-type instruction
    J,      ///< J-type instruction
    M,      ///< M-extension instruction
    F       ///< F-extension instruction
};


extern std::unordered_map<std::string, RTypeInstructionEncoding> R_type_instruction_encoding_map;
extern std::unordered_map<std::string, I1TypeInstructionEncoding> I1_type_instruction_encoding_map;
extern std::unordered_map<std::string, I2TypeInstructionEncoding> I2_type_instruction_encoding_map;
extern std::unordered_map<std::string, I3TypeInstructionEncoding> I3_type_instruction_encoding_map;
extern std::unordered_map<std::string, STypeInstructionEncoding> S_type_instruction_encoding_map;
extern std::unordered_map<std::string, BTypeInstructionEncoding> B_type_instruction_encoding_map;
extern std::unordered_map<std::string, UTypeInstructionEncoding> U_type_instruction_encoding_map;
extern std::unordered_map<std::string, JTypeInstructionEncoding> J_type_instruction_encoding_map;


/**
 * @brief A map that associates instruction names with their corresponding opcode type.
 * 
 * This map stores the opcode type for various instructions, indexed by their names.
 */
extern std::unordered_map<std::string, InstructionType> instruction_opcode_type_map;

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
 * @brief Validates if a given I1-type instruction is valid.
 * 
 * @param instruction The I1-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidI1TypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given I2-type instruction is valid.
 * 
 * @param instruction The I2-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidI2TypeInstruction(const std::string &instruction);

/**
 * @brief Validates if a given I3-type instruction is valid.
 * 
 * @param instruction The I3-type instruction string to validate.
 * @return True if the instruction is valid, false otherwise.
 */
bool isValidI3TypeInstruction(const std::string &instruction);

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
