#include "pch.h"


#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

int main() {
    int* ptr = new int[10];
// Forget to delete[] ptr; intentionally cause a leak


    std::unique_ptr<Lexer> lexer;  // Use a smart pointer to manage Lexer
    try {
        lexer = std::make_unique<Lexer>("/home/vis/Desk/assembler/examples/e1.s");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;  // Exit the program if an exception is caught
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

    std::vector<ParseError> errors = parser.getErrors();
    for (const ParseError &error: errors) {
        std::cout << "[ERROR] " << error.line << ": " << error.message << std::endl;
    }

    

    parser.printDataBuffers();
    // parser.printSymbolTable();
    // parser.printIntermediateCode();

    std::vector<std::string> machine_code = printIntermediateCode(parser.getIntermediateCode());


     for (const std::string &code: machine_code) {
         std::cout << code << std::endl;
     }
    

    // parser.printErrors();

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