/**
 * File Name: parser.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef PARSER_H
#define PARSER_H

#include "../pch.h"


#include "tokens.h"
#include "code_generator.h"

enum class ErrorType {
    UNEXPECTED_TOKEN,
    UNEXPECTED_EOF,


    INVALID_OPERAND,
    INVALID_DIRECTIVE,
    INVALID_INSTRUCTION,
    INVALID_REGISTER,
    INVALID_IMMEDIATE,
    INVALID_LABEL,
    INVALID_LABEL_REF,
    INVALID_TOKEN,
    INVALID_SYNTAX,

    MISALIGNED_IMMEDIATE,
    MISALIGNED_LABEL,
    IMMEDIATE_OUT_OF_RANGE,
    UNKNOWN_ERROR,
};

struct ParseError {
    ErrorType type;
    std::string message;
    int line;
    int column;

    ParseError(ErrorType type, const std::string& message, int line, int column)
        : type(type), message(message), line(line), column(column) {}
};

class Parser {
private:
    std::vector<Token> tokens_;
    size_t line_number_;
    size_t column_number_;
    size_t pos_;

    int error_count_;
    std::vector<ParseError> errors_;

    std::vector<std::pair<int, uint64_t>> data_buffer_;

    struct SymbolData {
        uint64_t address; // Address or instruction location
        bool isData;      // Flag to indicate if it's data or code
    };
    std::map<std::string, SymbolData> symbol_table_;

    std::vector<int> backPatch;
    std::vector<std::pair<ICUnit, bool>> IntermediateCode;


    Token prevToken();
    Token currentToken();
    Token nextToken();
    Token peekNextToken();
    Token peekToken(int n);
    void skipCurrentLine();
    void checkAndConsumeComma();

public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    const std::vector<ParseError>& getErrors() const;

    void printSymbolTable() const;


    std::vector<std::pair<int, uint64_t>>& getDataBuffer();
    void printDataBuffer() const;


    const std::vector<std::pair<ICUnit, bool>>& getIntermediateCode() const;

    void printIntermediateCode() const;


    




    void parseDataDirective();    
    void parseTextDirective();
    void parse();
    
};


#endif // PARSER_H