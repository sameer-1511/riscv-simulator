#include "pch.h"


#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

int main() {
    Lexer lexer("/home/vis/Desk/assembler/examples/e1.s");
    std::vector<Token> tokens = lexer.getTokenList();
    //int previous_line = -1; 
    //for (const Token& token : tokens) {
    //    if (token.line != previous_line) {
    //        if (previous_line != -1) {
    //            std::cout << std::endl; 
    //        }
    //        previous_line = token.line;
    //    }
    //    std::cout << token << std::endl;
    //}

    Parser parser(tokens);

    parser.parse();

    std::vector<ParseError> errors = parser.getErrors();
    for (const ParseError& error : errors) {
        std::cout << "Error: " << error.message << " at line " << error.line << " column " << error.column << std::endl;
    }

    // parser.printDataBuffer();
    // parser.printSymbolTable();
    // parser.printIntermediateCode();

    std::vector<std::string> machine_code = printIntermediateCode(parser.getIntermediateCode());

    
    for (const std::string& code : machine_code) {
        std::cout << code << std::endl;
    }

    //for (const int& index : parser.backPatch) {
    //    std::cout << index << std::endl;
    //}

    //std::vector<std::string> machine_code = generateMachineCode(parser.IntermediateCode);
//
    //for (const std::string& code : machine_code) {
    //    std::cout << code << std::endl;
    //}


    return 0;
}