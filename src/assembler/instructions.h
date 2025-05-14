/**
 * @file instructions.h
 * @brief Contains the declarations for the InstructionEncoding struct and related functions.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../pch.h"

namespace InstructionSet {

struct RTypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<7> funct7;

    RTypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct7)
        : opcode(opcode), funct3(funct3), funct7(funct7) {}
};

struct R4TypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<2> func2;

    R4TypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int func2)
        : opcode(opcode), funct3(funct3), func2(func2) {}
};

struct I1TypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    I1TypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
        : opcode(opcode), funct3(funct3) {}
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

struct CSR_RTypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    CSR_RTypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
        : opcode(opcode), funct3(funct3) {}
};

struct CSR_ITypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    CSR_ITypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
        : opcode(opcode), funct3(funct3) {}
};

// Fextension instructions===========================================================================

struct FDRTypeInstructionEncoding { // fsgnj
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<7> funct7;

    FDRTypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct7)
        : opcode(opcode), funct3(funct3), funct7(funct7) {}
};

struct FDR1TypeInstructionEncoding { // fadd, fsub, fmul
    std::bitset<7> opcode;
    std::bitset<7> funct7;

    FDR1TypeInstructionEncoding(unsigned int opcode, unsigned int funct7)
        : opcode(opcode), funct7(funct7) {}
};

struct FDR2TypeInstructionEncoding { //fsqrt, have funct5 instead of rs2
    std::bitset<7> opcode;
    std::bitset<5> funct5;
    std::bitset<7> funct7;

    FDR2TypeInstructionEncoding(unsigned int opcode, unsigned int funct5, unsigned int funct7)
        : opcode(opcode), funct5(funct5), funct7(funct7) {}
};

struct FDR3TypeInstructionEncoding { //fcvt
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<5> funct5;
    std::bitset<7> funct7;

    FDR3TypeInstructionEncoding(unsigned int opcode, unsigned int funct3, unsigned int funct5, unsigned int funct7)
        : opcode(opcode), funct3(funct3), funct5(funct5), funct7(funct7) {}
    
};


struct FDR4TypeInstructionEncoding { //fmadd
    std::bitset<7> opcode;
    std::bitset<2> funct2;

    FDR4TypeInstructionEncoding(unsigned int opcode, unsigned int funct2)
        : opcode(opcode), funct2(funct2) {}
};


struct FDITypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    FDITypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
        : opcode(opcode), funct3(funct3) {}
};

struct FDSTypeInstructionEncoding {
    std::bitset<7> opcode;
    std::bitset<3> funct3;

    FDSTypeInstructionEncoding(unsigned int opcode, unsigned int funct3)
        : opcode(opcode), funct3(funct3) {}
};



/**
 * @brief Enum that represents different syntax types for instructions.
 */
enum class SyntaxType {
    O_GPR_C_GPR_C_GPR,       ///< Opcode general-register , general-register , register
    O_GPR_C_GPR_C_I,        ///< Opcode general-register , general-register , immediate
    O_GPR_C_I,            ///< Opcode general-register , immediate
    O_GPR_C_GPR_C_IL,       ///< Opcode general-register , general-register , immediate , instruction_label
    O_GPR_C_GPR_C_DL,       ///< Opcode register , register , immediate , data_label
    O_GPR_C_IL,           ///< Opcode register , instruction_label
    O_GPR_C_DL,           ///< Opcode register , data_label
    O_GPR_C_I_LP_GPR_RP,    ///< Opcode register , immediate , lparen ( register )rparen
    O,                  ///< Opcode
    PSEUDO,              ///< Pseudo instruction

    O_GPR_C_CSR_C_GPR,       ///< Opcode general-register , csr , general-register
    O_GPR_C_CSR_C_I,        ///< Opcode general-register , csr , immediate

    O_FPR_C_FPR_C_FPR_C_FPR,    ///< Opcode floating-point-register , floating-point-register , floating-point-register , floating-point-register
    O_FPR_C_FPR_C_FPR_C_FPR_C_RM,    ///< Opcode floating-point-register , floating-point-register , floating-point-register , rounding_mode
    O_FPR_C_FPR_C_FPR,        ///< Opcode floating-point-register , floating-point-register , floating-point-register
    O_FPR_C_FPR_C_FPR_C_RM,    ///< Opcode floating-point-register , floating-point-register , floating-point-register , rounding_mode
    O_FPR_C_FPR,            ///< Opcode floating-point-register , floating-point-register
    O_FPR_C_FPR_C_RM,           ///< Opcode floating-point-register , floating-point-register , rounding_mode

    O_FPR_C_GPR,            ///< Opcode floating-point-register , general-register
    O_FPR_C_GPR_C_RM,           ///< Opcode floating-point-register , general-register , rounding_mode
    O_GPR_C_FPR,           ///< Opcode general-register , floating-point-register
    O_GPR_C_FPR_C_RM,       ///< Opcode general-register , floating-point-register , rounding_mode
    O_GPR_C_FPR_C_FPR,       ///< Opcode general-register , floating-point-register , floating-point-register
    O_FPR_C_I_LP_GPR_RP,    ///< Opcode floating-point-register , immediate , lparen ( general-register ) rparen
};

/**
 * @brief Enum that represents different types of opcodes.
 */
enum class InstructionType {
    R,      ///< R-type instruction
    R4,     ///< R4-type instruction
    I1,     ///< I-type instruction with 12-bit immediate
    I2,     ///< I-type instruction with 6-bit immediate and a funct6 field
    I3,     ///< I-type instruction of ecall and ebreak
    S,      ///< S-type instruction
    B,      ///< B-type instruction
    U,      ///< U-type instruction
    J,      ///< J-type instruction
    CSR_R,   ///< CSR R-type instruction
    CSR_I,   ///< CSR I-type instruction
    M,      ///< M-extension instruction
    F       ///< F-extension instruction
};

extern std::unordered_map<std::string, RTypeInstructionEncoding> R_type_instruction_encoding_map;
extern std::unordered_map<std::string, R4TypeInstructionEncoding> R4_type_instruction_encoding_map;
extern std::unordered_map<std::string, I1TypeInstructionEncoding> I1_type_instruction_encoding_map;
extern std::unordered_map<std::string, I2TypeInstructionEncoding> I2_type_instruction_encoding_map;
extern std::unordered_map<std::string, I3TypeInstructionEncoding> I3_type_instruction_encoding_map;
extern std::unordered_map<std::string, STypeInstructionEncoding> S_type_instruction_encoding_map;
extern std::unordered_map<std::string, BTypeInstructionEncoding> B_type_instruction_encoding_map;
extern std::unordered_map<std::string, UTypeInstructionEncoding> U_type_instruction_encoding_map;
extern std::unordered_map<std::string, JTypeInstructionEncoding> J_type_instruction_encoding_map;
extern std::unordered_map<std::string, CSR_RTypeInstructionEncoding> CSR_R_type_instruction_encoding_map;
extern std::unordered_map<std::string, CSR_ITypeInstructionEncoding> CSR_I_type_instruction_encoding_map;

extern std::unordered_map<std::string, FDRTypeInstructionEncoding> F_D_R_type_instruction_encoding_map;
extern std::unordered_map<std::string, FDR1TypeInstructionEncoding> F_D_R1_type_instruction_encoding_map;
extern std::unordered_map<std::string, FDR2TypeInstructionEncoding> F_D_R2_type_instruction_encoding_map;
extern std::unordered_map<std::string, FDR3TypeInstructionEncoding> F_D_R3_type_instruction_encoding_map;
extern std::unordered_map<std::string, FDR4TypeInstructionEncoding> F_D_R4_type_instruction_encoding_map;
extern std::unordered_map<std::string, FDITypeInstructionEncoding> F_D_I_type_instruction_encoding_map;
extern std::unordered_map<std::string, FDSTypeInstructionEncoding> F_D_S_type_instruction_encoding_map;


/**
 * @brief A map that associates instruction names with their corresponding opcode type.
 * 
 * This map stores the opcode type for various instructions, indexed by their names.
 */
extern std::unordered_map<std::string, InstructionType> instruction_opcode_type_map;

/**
 * @brief A map that associates instruction names with their expected syntax.
 * 
 * This map stores the expected syntax for various instructions, indexed by their names.
 */
extern std::unordered_map<std::string, std::vector<SyntaxType>> instruction_syntax_map;

bool isValidInstruction(const std::string &instruction);

bool isValidRTypeInstruction(const std::string &name);

bool isValidITypeInstruction(const std::string &instruction);

bool isValidI1TypeInstruction(const std::string &instruction);

bool isValidI2TypeInstruction(const std::string &instruction);

bool isValidI3TypeInstruction(const std::string &instruction);

bool isValidSTypeInstruction(const std::string &instruction);

bool isValidBTypeInstruction(const std::string &instruction);

bool isValidUTypeInstruction(const std::string &instruction);

bool isValidJTypeInstruction(const std::string &instruction);

bool isValidPseudoInstruction(const std::string &instruction);

bool isValidBaseExtensionInstruction(const std::string &instruction);

bool isValidCSRRTypeInstruction(const std::string &instruction);
bool isValidCSRITypeInstruction(const std::string &instruction);
bool isValidCSR_Instruction(const std::string &instruction);

bool isValidFDRTypeInstruction(const std::string &instruction);
bool isValidFDR1TypeInstruction(const std::string &instruction);
bool isValidFDR2TypeInstruction(const std::string &instruction);
bool isValidFDR3TypeInstruction(const std::string &instruction);
bool isValidFDR4TypeInstruction(const std::string &instruction);
bool isValidFDITypeInstruction(const std::string &instruction);
bool isValidFDSTypeInstruction(const std::string &instruction);

bool isValidFExtensionInstruction(const std::string &instruction);

std::string getExpectedSyntaxes(const std::string &opcode);

} // namespace InstructionSet


#endif // INSTRUCTIONS_H
