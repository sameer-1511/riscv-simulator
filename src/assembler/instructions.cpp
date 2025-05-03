/** @cond DOXYGEN_IGNORE */
/**
 * File Name: instructions.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
/** @endcond */

#include "instructions.h"

namespace InstructionSet {
static const std::unordered_set<std::string> valid_instructions = {
    "add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu",
    "addw", "subw", "sllw", "srlw", "sraw",
    "addi", "xori", "ori", "andi", "slli", "srli", "srai", "slti", "sltiu",
    "addiw", "slliw", "srliw", "sraiw",
    "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
    "sb", "sh", "sw", "sd",
    "beq", "bne", "blt", "bge", "bltu", "bgeu",
    "lui", "auipc",
    "jal", "jalr",
    "ecall", "ebreak",

    "la", "nop", "li", "mv", "not", "neg", "negw",
    "sext.w", "seqz", "snez", "sltz", "sgtz",
    "beqz", "bnez", "blez", "bgez", "bltz", "bgtz",
    "bgt", "ble", "bgtu", "bleu",
    "j", "jr", "ret", "call", "tail", "fence", "fence_i",

    "mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu",
    "mulw", "divw", "divuw", "remw", "remuw",

    "flw", "fsw", "fmadd.s", "fmsub.d", "fnmsub.s", "fnmadd.s",
    "fadd.s", "fsub.s", "fmul.s", "fdiv.s", "fsqrt.s",
    "fsgnj.s", "fsgnjn.s", "fsgnjx.s",
    "fmin.s", "fmax.s",
    "fcvt.w.s", "fcvt.wu.s", "fmv.x.w",
    "feq.s", "flt.s", "fle.s",
    "fclass.s", "fcvt.s.w", "fcvt.s.wu", "fmv.w.x",
    "fcvt.l.s", "fcvt.lu.s", "fcvt.s.l", "fcvt.s.lu",

};

static const std::unordered_set<std::string> RTypeInstructions = {
    // Base RV32I
    "add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu",

    // RV64
    "addw", "subw", "sllw", "srlw", "sraw",

    // M Extension
    "mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu",

    // M Extension RV64
    "mulw", "divw", "divuw", "remw", "remuw",

    // F Extension
    "fmadd.s", "fmsub.d", "fnmsub.s", "fnmadd.s",
    "fadd.s", "fsub.s", "fmul.s", "fdiv.s", "fsqrt.s",
    "fsgnj.s", "fsgnjn.s", "fsgnjx.s",
    "fmin.s", "fmax.s",
    "fcvt.w.s", "fcvt.wu.s", "fmv.x.w",
    "feq.s", "flt.s", "fle.s",
    "fclass.s", "fcvt.s.w", "fcvt.s.wu", "fmv.w.x",
    "fcvt.l.s", "fcvt.lu.s", "fcvt.s.l", "fcvt.s.lu",

};

static const std::unordered_set<std::string> ITypeInstructions = {
    "addi", "xori", "ori", "andi", "slli", "srli", "srai", "slti", "sltiu",
    "addiw", "slliw", "srliw", "sraiw",
    "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
    "jalr"
};

static const std::unordered_set<std::string> I1TypeInstructions = {
    "addi", "xori", "ori", "andi", "sltiu", "slti",
    "addiw",
    "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
    "jalr"
};

static const std::unordered_set<std::string> I2TypeInstructions = {
    "slli", "srli", "srai",
    "slliw", "srliw", "sraiw"
};

static const std::unordered_set<std::string> I3TypeInstructions = {
    "ecall", "ebreak"
};

static const std::unordered_set<std::string> STypeInstructions = {
    "sb", "sh", "sw", "sd"
};

static const std::unordered_set<std::string> BTypeInstructions = {
    "beq", "bne", "blt", "bge", "bltu", "bgeu"
};

static const std::unordered_set<std::string> UTypeInstructions = {
    "lui", "auipc"
};

static const std::unordered_set<std::string> JTypeInstructions = {
    "jal"
};

static const std::unordered_set<std::string> PseudoInstructions = {
    "la", "nop", "li", "mv", "not", "neg", "negw",
    "sext.w", "seqz", "snez", "sltz", "sgtz",
    "beqz", "bnez", "blez", "bgez", "bltz", "bgtz",
    "bgt", "ble", "bgtu", "bleu",
    "j", "jr", "ret", "call", "tail", "fence", "fence_i",
};

static const std::unordered_set<std::string> BaseExtensionInstructions = {
    "add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu",
    "addw", "subw", "sllw", "srlw", "sraw",
    "addi", "xori", "ori", "andi", "slli", "srli", "srai", "slti", "sltiu",
    "addiw", "slliw", "srliw", "sraiw",
    "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
    "sb", "sh", "sw", "sd",
    "beq", "bne", "blt", "bge", "bltu", "bgeu",
    "lui", "auipc",
    "jal", "jalr",
    "ecall", "ebreak",
};

static const std::unordered_set<std::string> MExtensionInstructions = {
    "mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu",
    "mulw", "divw", "divuw", "remw", "remuw"
};

static const std::unordered_set<std::string> FExtensionInstructions = {
    "flw", "fsw", "fmadd.s", "fmsub.d", "fnmsub.s", "fnmadd.s",
    "fadd.s", "fsub.s", "fmul.s", "fdiv.s", "fsqrt.s",
    "fsgnj.s", "fsgnjn.s", "fsgnjx.s",
    "fmin.s", "fmax.s",
    "fcvt.w.s", "fcvt.wu.s", "fmv.x.w",
    "feq.s", "flt.s", "fle.s",
    "fclass.s", "fcvt.s.w", "fcvt.s.wu", "fmv.w.x",
    "fcvt.l.s", "fcvt.lu.s", "fcvt.s.l", "fcvt.s.lu",
};

static const std::unordered_set<std::string> DExtensionInstructions = {
    "fld", "fsd", "fmadd.d", "fmsub.d", "fnmsub.d", "fnmadd.d",
    "fadd.d", "fsub.d", "fmul.d", "fdiv.d", "fsqrt.d",
    "fsgnj.d", "fsgnjn.d", "fsgnjx.d",
    "fmin.d", "fmax.d",
    "fcvt.s.d", "fcvt.d.s",
    "feq.d", "flt.d", "fle.d",
    "fclass.d", "fcvt.w.d", "fcvt.wu.d", "fmv.x.d",
    "fcvt.l.d", "fcvt.lu.d", "fcvt.d.l", "fcvt.d.lu",
};

std::unordered_map<std::string, RTypeInstructionEncoding> R_type_instruction_encoding_map = {
    {"add",     {0b0110011, 0b000, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"sub",     {0b0110011, 0b000, 0b0100000}}, // O_GPR_C_GPR_C_GPR
    {"xor",     {0b0110011, 0b100, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"or",      {0b0110011, 0b110, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"and",     {0b0110011, 0b111, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"sll",     {0b0110011, 0b001, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"srl",     {0b0110011, 0b101, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"sra",     {0b0110011, 0b101, 0b0100000}}, // O_GPR_C_GPR_C_GPR
    {"slt",     {0b0110011, 0b010, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"sltu",    {0b0110011, 0b011, 0b0000000}}, // O_GPR_C_GPR_C_GPR

    {"addw",    {0b0111011, 0b000, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"subw",    {0b0111011, 0b000, 0b0100000}}, // O_GPR_C_GPR_C_GPR
    {"sllw",    {0b0111011, 0b001, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"srlw",    {0b0111011, 0b101, 0b0000000}}, // O_GPR_C_GPR_C_GPR
    {"sraw",    {0b0111011, 0b101, 0b0100000}}, // O_GPR_C_GPR_C_GPR

//==RV64M======================================================================================
    {"mul",     {0b0110011, 0b000, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"mulh",    {0b0110011, 0b001, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"mulhsu",  {0b0110011, 0b010, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"mulhu",   {0b0110011, 0b011, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"div",     {0b0110011, 0b100, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"divu",    {0b0110011, 0b101, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"rem",     {0b0110011, 0b110, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"remu",    {0b0110011, 0b111, 0b0000001}}, // O_GPR_C_GPR_C_GPR

    {"mulw",    {0b0111011, 0b000, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"divw",    {0b0111011, 0b100, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"divuw",   {0b0111011, 0b101, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"remw",    {0b0111011, 0b110, 0b0000001}}, // O_GPR_C_GPR_C_GPR
    {"remuw",   {0b0111011, 0b111, 0b0000001}}, // O_GPR_C_GPR_C_GPR
};

std::unordered_map<std::string, I1TypeInstructionEncoding> I1_type_instruction_encoding_map = {
    {"addi",    {0b0010011, 0b000}}, // O_GPR_C_GPR_C_I
    {"xori",    {0b0010011, 0b100}}, // O_GPR_C_GPR_C_I
    {"ori",     {0b0010011, 0b110}}, // O_GPR_C_GPR_C_I
    {"andi",    {0b0010011, 0b111}}, // O_GPR_C_GPR_C_I
    {"sltiu",   {0b0010011, 0b011}}, // O_GPR_C_GPR_C_I
    {"slti",    {0b0010011, 0b010}}, // O_GPR_C_GPR_C_I

    {"addiw",   {0b0011011, 0b000}}, // O_GPR_C_GPR_C_I

    {"lh",      {0b0000011, 0b001}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL
    {"lw",      {0b0000011, 0b010}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL
    {"lb",      {0b0000011, 0b000}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL
    {"ld",      {0b0000011, 0b011}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL
    {"lbu",     {0b0000011, 0b100}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL
    {"lhu",     {0b0000011, 0b101}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL
    {"lwu",     {0b0000011, 0b110}}, // O_GPR_C_I_LP_GPR_RP, O_GPR_C_DL

    {"jalr",    {0b1100111, 0b000}}, // O_GR_C_I, O_GPR_C_IL
};

std::unordered_map<std::string, I3TypeInstructionEncoding> I3_type_instruction_encoding_map = {
    {"ecall",   {0b1110011, 0b000, 0b0000000}}, // O
    {"ebreak",  {0b1110011, 0b001, 0b0000000}}, // O
};

std::unordered_map<std::string, I2TypeInstructionEncoding> I2_type_instruction_encoding_map = {
    {"slli",    {0b0010011, 0b001, 0b000000}}, // O_GPR_C_GPR_C_I
    {"srli",    {0b0010011, 0b101, 0b000000}}, // O_GPR_C_GPR_C_I
    {"srai",    {0b0010011, 0b101, 0b010000}}, // O_GPR_C_GPR_C_I

    {"slliw",   {0b0011011, 0b001, 0b000000}}, // O_GPR_C_GPR_C_I
    {"srliw",   {0b0011011, 0b101, 0b000000}}, // O_GPR_C_GPR_C_I
    {"sraiw",   {0b0011011, 0b101, 0b010000}}, // O_GPR_C_GPR_C_I
};

std::unordered_map<std::string, STypeInstructionEncoding> S_type_instruction_encoding_map = {
    {"sb",      {0b0100011, 0b000}}, // O_GPR_C_GPR_C_I
    {"sh",      {0b0100011, 0b001}}, // O_GPR_C_GPR_C_I
    {"sw",      {0b0100011, 0b010}}, // O_GPR_C_GPR_C_I
    {"sd",      {0b0100011, 0b011}}, // O_GPR_C_GPR_C_I
};

std::unordered_map<std::string, BTypeInstructionEncoding> B_type_instruction_encoding_map = {
    {"beq",     {0b1100011, 0b000}}, // O_GPR_C_GPR_C_I, O_GPR_C_GPR_C_IL
    {"bne",     {0b1100011, 0b001}}, // O_GPR_C_GPR_C_I, O_GPR_C_GPR_C_IL
    {"blt",     {0b1100011, 0b100}}, // O_GPR_C_GPR_C_I, O_GPR_C_GPR_C_IL
    {"bge",     {0b1100011, 0b101}}, // O_GPR_C_GPR_C_I, O_GPR_C_GPR_C_IL
    {"bltu",    {0b1100011, 0b110}}, // O_GPR_C_GPR_C_I, O_GPR_C_GPR_C_IL
    {"bgeu",    {0b1100011, 0b111}}, // O_GPR_C_GPR_C_I, O_GPR_C_GPR_C_IL
};

std::unordered_map<std::string, UTypeInstructionEncoding> U_type_instruction_encoding_map = {
    {"lui",     {0b0110111}}, // O_GR_C_I
    {"auipc",   {0b0010111}}, // O_GR_C_I
};

std::unordered_map<std::string, JTypeInstructionEncoding> J_type_instruction_encoding_map = {
    {"jal",     {0b1101111}}, // O_GPR_C_IL
};

std::unordered_map<std::string, InstructionType> instruction_opcode_type_map = {
    {"add",         InstructionType::R},
    {"sub",         InstructionType::R},
    {"xor",         InstructionType::R},
    {"or",          InstructionType::R},
    {"and",         InstructionType::R},
    {"sll",         InstructionType::R},
    {"srl",         InstructionType::R},
    {"sra",         InstructionType::R},
    {"slt",         InstructionType::R},
    {"sltu",        InstructionType::R},

    {"addw",        InstructionType::R},
    {"subw",        InstructionType::R},
    {"sllw",        InstructionType::R},
    {"srlw",        InstructionType::R},
    {"sraw",        InstructionType::R},

    {"addi",        InstructionType::I1},
    {"xori",        InstructionType::I1},
    {"ori",         InstructionType::I1},
    {"andi",        InstructionType::I1},
    {"slli",        InstructionType::I2},
    {"srli",        InstructionType::I2},
    {"srai",        InstructionType::I2},
    {"slti",        InstructionType::I1},
    {"sltiu",       InstructionType::I1},

    {"addiw",       InstructionType::I1},
    {"slliw",       InstructionType::I2},
    {"srliw",       InstructionType::I2},
    {"sraiw",       InstructionType::I2},

    {"lb",          InstructionType::I1},
    {"lh",          InstructionType::I1},
    {"lw",          InstructionType::I1},
    {"ld",          InstructionType::I1},
    {"lbu",         InstructionType::I1},
    {"lhu",         InstructionType::I1},
    {"lwu",         InstructionType::I1},

    {"jalr",        InstructionType::I1},

    {"sb",          InstructionType::S},
    {"sh",          InstructionType::S},
    {"sw",          InstructionType::S},
    {"sd",          InstructionType::S},

    {"beq",         InstructionType::B},
    {"bne",         InstructionType::B},
    {"blt",         InstructionType::B},
    {"bge",         InstructionType::B},
    {"bltu",        InstructionType::B},
    {"bgeu",        InstructionType::B},

    {"lui",         InstructionType::U},
    {"auipc",       InstructionType::U},

    {"jal",         InstructionType::J},

    {"ecall",       InstructionType::I3},
    {"ebreak",      InstructionType::I3},

//==RV64M======================================================================================
    {"mul",         InstructionType::R},
    {"mulh",        InstructionType::R},
    {"mulhsu",      InstructionType::R},
    {"mulhu",       InstructionType::R},
    {"div",         InstructionType::R},
    {"divu",        InstructionType::R},
    {"rem",         InstructionType::R},
    {"remu",        InstructionType::R},

    {"mulw",        InstructionType::R},
    {"divw",        InstructionType::R},
    {"divuw",       InstructionType::R},
    {"remw",        InstructionType::R},
    {"remuw",       InstructionType::R},

//==RV64F======================================================================================
    {"flw",         InstructionType::I1},
    {"fsw",         InstructionType::S},

    {"fmadd.s",     InstructionType::R4},
    {"fmsub.s",     InstructionType::R4},
    {"fnmsub.s",    InstructionType::R4},
    {"fnmadd.s",    InstructionType::R4},

    {"fadd.s",      InstructionType::R},
    {"fsub.s",      InstructionType::R},
    {"fmul.s",      InstructionType::R},
    {"fdiv.s",      InstructionType::R},
    {"fsqrt.s",     InstructionType::R},
    {"fsgnj.s",     InstructionType::R},
    {"fsgnjn.s",    InstructionType::R},
    {"fsgnjx.s",    InstructionType::R},
    {"fmin.s",      InstructionType::R},
    {"fmax.s",      InstructionType::R},
    {"feq.s",       InstructionType::R},
    {"flt.s",       InstructionType::R},
    {"fle.s",       InstructionType::R},

    {"fcvt.w.s",    InstructionType::R},
    {"fcvt.wu.s",   InstructionType::R},
    {"fcvt.s.w",    InstructionType::R},
    {"fcvt.s.wu",   InstructionType::R},

    {"fcvt.l.s",    InstructionType::R},
    {"fcvt.lu.s",   InstructionType::R},
    {"fcvt.s.l",    InstructionType::R},
    {"fcvt.s.lu",   InstructionType::R},

    {"fclass.s",    InstructionType::R},


    {"fmv.x.w",     InstructionType::R},
    {"fmv.w.x",     InstructionType::R},

};

/*
    O_FPR_C_FPR_C_FPR_C_FPR,    ///< Opcode floating-point-register , floating-point-register , floating-point-register , floating-point-register
    O_FPR_C_FPR_C_FPR,        ///< Opcode floating-point-register , floating-point-register , floating-point-register
    O_FPR_C_FPR_C_I,        ///< Opcode floating-point-register , floating-point-register , immediate
    O_FPR_C_FPR,            ///< Opcode floating-point-register , floating-point-register

    O_FPR_C_GPR,            ///< Opcode floating-point-register , general-register
    O_GPR_C_FPR,           ///< Opcode general-register , floating-point-register
    O_GPR_C_FPR_C_FPR,       ///< Opcode general-register , floating-point-register , floating-point-register
    O_FPR_C_I_LP_GPR_RP,    ///< Opcode floating-point-register , immediate , lparen , general-register , rparen

    O_GPR_C_GPR_C_GPR,       ///< Opcode general-register , general-register , register
    O_GPR_C_GPR_C_I,        ///< Opcode general-register , general-register , immediate
    O_GPR_C_I,            ///< Opcode general-register , immediate
    O_GPR_C_GPR_C_IL,       ///< Opcode general-register , general-register , immediate , instruction_label
    O_GPR_C_GPR_C_DL,       ///< Opcode register , register , immediate , data_label
    O_GPR_C_IL,           ///< Opcode register , instruction_label
    O_GPR_C_DL,           ///< Opcode register , data_label
    O_GPR_C_I_LP_GPR_RP,    ///< Opcode register , immediate , lparen , register , rparen
    O,                  ///< Opcode
    PSEUDO              ///< Pseudo instruction

    DL -> Data Label
    IL -> Instruction Label
    

*/
std::unordered_map<std::string, std::vector<SyntaxType>> instruction_syntax_map = {
    {"add",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"sub",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"xor",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"or",          {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"and",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"sll",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"srl",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"sra",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"slt",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"sltu",        {SyntaxType::O_GPR_C_GPR_C_GPR}},

    {"addi",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"xori",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"ori",         {SyntaxType::O_GPR_C_GPR_C_I}},
    {"andi",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"slli",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"srli",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"srai",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"slti",        {SyntaxType::O_GPR_C_GPR_C_I}},
    {"sltiu",       {SyntaxType::O_GPR_C_GPR_C_I}},

    {"lb",          {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},
    {"lh",          {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},
    {"lw",          {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},
    {"ld",          {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},
    {"lbu",         {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},
    {"lhu",         {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},
    {"lwu",         {SyntaxType::O_GPR_C_I_LP_GPR_RP, SyntaxType::O_GPR_C_DL}},

    {"sb",          {SyntaxType::O_GPR_C_I_LP_GPR_RP}},
    {"sh",          {SyntaxType::O_GPR_C_I_LP_GPR_RP}},
    {"sw",          {SyntaxType::O_GPR_C_I_LP_GPR_RP}},
    {"sd",          {SyntaxType::O_GPR_C_I_LP_GPR_RP}},

    {"beq",         {SyntaxType::O_GPR_C_GPR_C_I,     SyntaxType::O_GPR_C_GPR_C_IL}},
    {"bne",         {SyntaxType::O_GPR_C_GPR_C_I,     SyntaxType::O_GPR_C_GPR_C_IL}},
    {"blt",         {SyntaxType::O_GPR_C_GPR_C_I,     SyntaxType::O_GPR_C_GPR_C_IL}},
    {"bge",         {SyntaxType::O_GPR_C_GPR_C_I,     SyntaxType::O_GPR_C_GPR_C_IL}},
    {"bltu",        {SyntaxType::O_GPR_C_GPR_C_I,     SyntaxType::O_GPR_C_GPR_C_IL}},
    {"bgeu",        {SyntaxType::O_GPR_C_GPR_C_I,     SyntaxType::O_GPR_C_GPR_C_IL}},

    {"lui",         {SyntaxType::O_GPR_C_I}},
    {"auipc",       {SyntaxType::O_GPR_C_I}},

    {"jal",         {SyntaxType::O_GPR_C_I,         SyntaxType::O_GPR_C_IL}},

    {"jalr",        {SyntaxType::O_GPR_C_I_LP_GPR_RP}},

    {"ecall",       {SyntaxType::O}},
    {"ebreak",      {SyntaxType::O}},

//////////////////////////////////////////////////////////////////////////////

    {"nop",         {SyntaxType::PSEUDO}},
    {"li",          {SyntaxType::PSEUDO}},
    {"la",          {SyntaxType::PSEUDO}},
    {"mv",          {SyntaxType::PSEUDO}},
    {"not",         {SyntaxType::PSEUDO}},
    {"neg",         {SyntaxType::PSEUDO}},
    {"negw",        {SyntaxType::PSEUDO}},
    {"sext.w",      {SyntaxType::PSEUDO}},
    {"seqz",        {SyntaxType::PSEUDO}},
    {"snez",        {SyntaxType::PSEUDO}},
    {"sltz",        {SyntaxType::PSEUDO}},
    {"sgtz",        {SyntaxType::PSEUDO}},
    {"beqz",        {SyntaxType::PSEUDO}},
    {"bnez",        {SyntaxType::PSEUDO}},
    {"blez",        {SyntaxType::PSEUDO}},
    {"bgez",        {SyntaxType::PSEUDO}},
    {"bltz",        {SyntaxType::PSEUDO}},
    {"bgtz",        {SyntaxType::PSEUDO}},
    {"bgt",         {SyntaxType::PSEUDO}},
    {"ble",         {SyntaxType::PSEUDO}},
    {"bgtu",        {SyntaxType::PSEUDO}},
    {"bleu",        {SyntaxType::PSEUDO}},
    {"j",           {SyntaxType::PSEUDO}},
    {"jr",          {SyntaxType::PSEUDO}},
    {"ret",         {SyntaxType::PSEUDO}},
    {"call",        {SyntaxType::PSEUDO}},
    {"tail",        {SyntaxType::PSEUDO}},
    {"fence",       {SyntaxType::PSEUDO}},
    {"fence_i",     {SyntaxType::PSEUDO}},

//////////////////////////////////////////////////////////////////////////////
    {"mul",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"mulh",        {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"mulhsu",      {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"mulhu",       {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"div",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"divu",        {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"rem",         {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"remu",        {SyntaxType::O_GPR_C_GPR_C_GPR}},

    {"mulw",        {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"divw",        {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"divuw",       {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"remw",        {SyntaxType::O_GPR_C_GPR_C_GPR}},
    {"remuw",       {SyntaxType::O_GPR_C_GPR_C_GPR}},

    {"flw",         {SyntaxType::O_FPR_C_I_LP_GPR_RP}},
    {"fsw",         {SyntaxType::O_FPR_C_I_LP_GPR_RP}},

    {"fmadd.s",     {SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR}},
    {"fmsub.d",     {SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR}},
    {"fnmsub.s",    {SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR}},
    {"fnmadd.s",    {SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR}},
    {"fadd.s",      {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fsub.s",      {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fmul.s",      {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fdiv.s",      {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fsqrt.s",     {SyntaxType::O_FPR_C_FPR}},
    {"fsgnj.s",     {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fsgnjn.s",    {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fsgnjx.s",    {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fmin.s",      {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fmax.s",      {SyntaxType::O_FPR_C_FPR_C_FPR}},
    {"fcvt.w.s",    {SyntaxType::O_GPR_C_FPR}},
    {"fcvt.wu.s",   {SyntaxType::O_GPR_C_FPR}},
    {"fmv.x.w",     {SyntaxType::O_GPR_C_FPR}},
    {"feq.s",       {SyntaxType::O_GPR_C_FPR_C_FPR}},
    {"flt.s",       {SyntaxType::O_GPR_C_FPR_C_FPR}},
    {"fle.s",       {SyntaxType::O_GPR_C_FPR_C_FPR}},
    {"fclass.s",    {SyntaxType::O_GPR_C_FPR}},
    {"fcvt.s.w",    {SyntaxType::O_FPR_C_GPR}},
    {"fcvt.s.wu",   {SyntaxType::O_FPR_C_GPR}},
    {"fmv.w.x",     {SyntaxType::O_FPR_C_GPR}},
};

bool isValidInstruction(const std::string &instruction) {
    return valid_instructions.find(instruction) != valid_instructions.end();
}

bool isValidRTypeInstruction(const std::string &instruction) {
    return RTypeInstructions.find(instruction) != RTypeInstructions.end();
}

bool isValidITypeInstruction(const std::string &instruction) {
    return (I1TypeInstructions.find(instruction) != I1TypeInstructions.end()) ||
        (I2TypeInstructions.find(instruction) != I2TypeInstructions.end()) ||
        (I3TypeInstructions.find(instruction) != I3TypeInstructions.end());
}

bool isValidI1TypeInstruction(const std::string &instruction) {
    return I1TypeInstructions.find(instruction) != I1TypeInstructions.end();
}

bool isValidI2TypeInstruction(const std::string &instruction) {
    return I2TypeInstructions.find(instruction) != I2TypeInstructions.end();
}

bool isValidI3TypeInstruction(const std::string &instruction) {
    return I3TypeInstructions.find(instruction) != I3TypeInstructions.end();
}

bool isValidSTypeInstruction(const std::string &instruction) {
    return STypeInstructions.find(instruction) != STypeInstructions.end();
}

bool isValidBTypeInstruction(const std::string &instruction) {
    return BTypeInstructions.find(instruction) != BTypeInstructions.end();
}

bool isValidUTypeInstruction(const std::string &instruction) {
    return UTypeInstructions.find(instruction) != UTypeInstructions.end();
}

bool isValidJTypeInstruction(const std::string &instruction) {
    return JTypeInstructions.find(instruction) != JTypeInstructions.end();
}

bool isValidPseudoInstruction(const std::string &instruction) {
    return PseudoInstructions.find(instruction) != PseudoInstructions.end();
}

bool isValidBaseExtensionInstruction(const std::string &instruction) {
    return BaseExtensionInstructions.find(instruction) != BaseExtensionInstructions.end();
}

bool isValidFExtensionInstruction(const std::string &instruction) {
    return FExtensionInstructions.find(instruction) != FExtensionInstructions.end();
}

std::string getExpectedSyntaxes(const std::string &opcode) {
    static const std::unordered_map<std::string, std::string> opcodeSyntaxMap = {
        {"nop", "nop"},
        {"li", "li <reg>, <imm>"},
        {"mv", "mv <reg>, <reg>"},
        {"not", "not <reg>, <reg>"},
        {"neg", "neg <reg>, <reg>"},
        {"seqz", "seqz <reg>, <reg>"},
        {"snez", "snez <reg>, <reg>"},
        {"sltz", "sltz <reg>, <reg>"},
        {"sgtz", "sgtz <reg>, <reg>"},
        {"beqz", "beqz <reg>, <text label>"},
        {"bnez", "bnez <reg>, <text label>"},
        {"blez", "blez <reg>, <text label>"},
        {"bgez", "bgez <reg>, <text label>"},
        {"bltz", "bltz <reg>, <text label>"},
        {"bgtz", "bgtz <reg>, <text label>"},
        {"j", "j <text label>"},
        {"jr", "jr <reg>"},
        {"la", "la <reg>, <text label>"},
        {"call", "call <text label>"},
        {"tail", "tail <text label>"},
        {"fence", "fence"}
    };

    auto opcodeIt = opcodeSyntaxMap.find(opcode);
    if (opcodeIt != opcodeSyntaxMap.end()) {
        return opcodeIt->second;
    }

    static const std::unordered_map<SyntaxType, std::string> syntaxTypeToString = {
        {SyntaxType::O, "<empty>"},
        {SyntaxType::O_GPR_C_GPR_C_GPR, "<gp-reg>, <gp-reg>, <gp-reg>"},
        {SyntaxType::O_GPR_C_GPR_C_I, "<gp-reg>, <gp-reg>, <imm>"},
        {SyntaxType::O_GPR_C_GPR_C_IL, "<gp-reg>, <gp-reg>, <text-label>"},
        {SyntaxType::O_GPR_C_GPR_C_DL, "<gp-reg>, <gp-reg>, <data-label>"},
        {SyntaxType::O_GPR_C_I_LP_GPR_RP, "<gp-reg>, <gp-imm>(<gp-reg>)"},
        {SyntaxType::O_GPR_C_I, "<gp-reg>, <imm>"},
        {SyntaxType::O_GPR_C_IL, "<gp-reg>, <text-label>"},
        {SyntaxType::O_GPR_C_DL, "<gp-reg>, <data-label>"},
        {SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR, "<fp-reg>, <fp-reg>, <fp-reg>, <fp-reg>"},
        {SyntaxType::O_FPR_C_FPR_C_FPR, "<fp-reg>, <fp-reg>, <fp-reg>"},
        {SyntaxType::O_FPR_C_FPR, "<fp-reg>, <fp-reg>"},
        {SyntaxType::O_FPR_C_GPR, "<fp-reg>, <gp-reg>"},
        {SyntaxType::O_GPR_C_FPR, "<gp-reg>, <fp-reg>"},
        {SyntaxType::O_GPR_C_FPR_C_FPR, "<gp-reg>, <fp-reg>, <fp-reg>"},
        {SyntaxType::O_FPR_C_I_LP_GPR_RP, "<fp-reg>, <imm>(<gp-reg>)"},
    };

    std::string syntaxes;
    const auto &syntaxList = instruction_syntax_map[opcode];
    for (size_t i = 0; i < syntaxList.size(); ++i) {
        if (i > 0) {
            syntaxes += " or ";
        }
        auto syntaxIt = syntaxTypeToString.find(syntaxList[i]);
        if (syntaxIt != syntaxTypeToString.end()) {
            syntaxes += opcode + " " + syntaxIt->second;
        }
    }

    return syntaxes;
}

} // namespace InstructionSet