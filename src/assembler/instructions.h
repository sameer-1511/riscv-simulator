/**
 * File Name: instructions.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../pch.h"

#include <optional>
#include <bitset>


struct InstructionEncoding {
    std::string opcode;
    std::string funct3;
    std::string funct7;
    std::string funct6;

    InstructionEncoding(const std::string &opcode, const std::string &funct3, const std::string &funct7,
                        const std::string &funct6)
            : opcode(opcode), funct3(funct3), funct7(funct7), funct6(funct6) {}
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

enum class SyntaxType {
    O_R_C_R_C_R,        // Opcode register , register , register
    O_R_C_R_C_I,        // Opcode register , register , immediate
    O_R_C_I,            // Opcode register , immediate
    O_R_C_R_C_IL,       // Opcode register , register , immediate , instruction_label
    O_R_C_R_C_DL,       // Opcode register , register , immediate , data_label
    O_R_C_IL,           // Opcode register , instruction_label
    O_R_C_DL,           // Opcode register , data_label
    O_R_C_I_LP_R_RP,    // Opcode register , immediate , lparen , register , rparen
    O,                  // Opcode
};

extern std::unordered_map<std::string, InstructionEncoding> instruction_encoding_map;

extern std::unordered_map<std::string, std::vector<SyntaxType>> instruction_syntax_map;

bool isValidInstruction(const std::string &instruction);

bool isValidRTypeInstruction(const std::string &instruction);

bool isValidITypeInstruction(const std::string &instruction);

bool isValidSTypeInstruction(const std::string &instruction);

bool isValidBTypeInstruction(const std::string &instruction);

bool isValidUTypeInstruction(const std::string &instruction);

bool isValidJTypeInstruction(const std::string &instruction);

std::string getExpectedSyntaxes(const std::string &opcode);


#endif // INSTRUCTIONS_H