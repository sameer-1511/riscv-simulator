/** @cond DOXYGEN_IGNORE */
/**
 * File Name: assembler.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
/** @endcond */

#include "assembler.h"
#include "../utils.h"
#include "../globals.h"

AssembledProgram assemble(const std::string &filename) {
    std::unique_ptr<Lexer> lexer;
    try {
        lexer = std::make_unique<Lexer>(filename);
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<Token> tokens = lexer->getTokenList();
    // int previous_line = -1; 
    // for (const Token& token : tokens) {
    //     if (token.line_number != previous_line) {
    //         if (previous_line != -1) {
    //             std::cout << std::endl; 
    //         }
    //         previous_line = token.line_number;
    //     }
    //     std::cout << token << std::endl;
    // }

    Parser parser(lexer->getFilename(), tokens);
    parser.parse();

    AssembledProgram program;

    if (parser.getErrorCount() == 0) {
        // parser.printDataBuffers();
        // parser.printSymbolTable();
        // parser.printIntermediateCode();

        std::vector<std::pair<ICUnit, bool>> IntermediateCode = parser.getIntermediateCode();
        //std::vector<std::string> machine_code = printIntermediateCode(IntermediateCode);
        //for (const std::string &code: machine_code) {
        //    std::cout << code << std::endl;
        //}

        std::vector<std::bitset<32>> machine_code_bits = generateMachineCode(IntermediateCode);

        program.instruction_buffer = machine_code_bits;
        

        // std::vector<std::string> machine_code = printIntermediateCode(parser.getIntermediateCode());
        // for (const std::string &code: machine_code) {
        //     std::cout << code << std::endl;
        // }
    } else {
        // std::vector<ParseError> errors = parser.getErrors();
        // for (const ParseError &error: errors) {
        //     std::cout << "[ERROR] " << error.line << ": " << error.message << std::endl;
        // }
        parser.printErrors();
        dumpErrors(globals::errors_dump_file, parser.getErrors());
        throw std::runtime_error("Failed to parse file: " + filename);
    }

    program.filename = filename;
    // program.instruction_buffer = {
    //         std::bitset<32>(0b00000000000000000000000000110011),
    //         std::bitset<32>(0b00000000000000000000000000110011),
    // };
    return program;
}





