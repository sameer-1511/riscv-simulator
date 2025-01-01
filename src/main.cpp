#include "pch.h"


#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

int main() {
    Lexer lexer("/home/vis/Desk/assembler/examples/e1.s");
    std::vector<Token> tokens = lexer.getTokenList();
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

    Parser parser(lexer.getFilename(), tokens);

    parser.parse();

    std::vector<ParseError> errors = parser.getErrors();
    for (const ParseError &error: errors) {
        std::cout << "Error: " << error.message << " at line " << error.line << std::endl;
    }

    // parser.printDataBuffers();
    // parser.printSymbolTable();
    // parser.printIntermediateCode();

    std::vector<std::string> machine_code = printIntermediateCode(parser.getIntermediateCode());


    for (const std::string &code: machine_code) {
        std::cout << code << std::endl;
    }



    // for (const SyntaxError &error: parser.getSyntaxErrors()) {
    //     std::cout << error << std::endl;
    // }

    parser.printErrors();

    //for (const int& index : parser.backPatch) {
    //    std::cout << index << std::endl;
    //}

    //std::vector<std::string> machine_code = generateMachineCode(parser.IntermediateCode);
//
    //for (const std::string& code : machine_code) {
    //    std::cout << code << std::endl;
    //}

    //std::cout << "\n";

    //SyntaxError error("Syntax Error", "Expected: add <reg>,<reg>,<reg>", "test/errortest1", 1, 17, "label1: label3: add x0,0,zero     ");
    //std::cout << error << std::endl;


    return 0;
}