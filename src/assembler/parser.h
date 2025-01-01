/**
 * File Name: parser.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef PARSER_H
#define PARSER_H

#include <utility>

#include "../pch.h"


#include "tokens.h"
#include "code_generator.h"
#include "errors.h"


struct ParseError {
    ErrorType type;
    std::string message;
    int line;
    int column;

    ParseError(ErrorType type, std::string message, int line, int column)
            : type(type), message(std::move(message)), line(line), column(column) {}
};

class Parser {
private:
    std::string filename_;
    std::vector<Token> tokens_;
    size_t line_number_ = 1;
    size_t column_number_ = 1;
    size_t pos_ = 0;

    int error_count_ = 0;
    std::vector<ParseError> errors_;

    std::vector<std::variant<SyntaxError, UnexpectedTokenError, ImmediateOutOfRangeError, MisallignedImmediateError, UnexpectedOperandError, InvalidLabelRefError>> all_errors_;

    std::vector<SyntaxError> syntax_errors_;
    std::vector<UnexpectedTokenError> unexpected_token_errors_;


    std::vector<std::pair<int, uint64_t>> data_buffer_;

    uint64_t data_index_ = 0; // TODO: refactor this

    std::vector<std::pair<int, uint8_t>> byte_buffer_;
    std::vector<std::pair<int, uint16_t>> halfword_buffer_;
    std::vector<std::pair<int, uint32_t>> word_buffer_;
    std::vector<std::pair<int, uint64_t>> dword_buffer_;
    std::vector<std::pair<int, std::string>> string_buffer_;

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
    Parser(const std::string &filename, const std::vector<Token> &tokens);

    ~Parser();

    const std::vector<SyntaxError> &getSyntaxErrors() const;
    const std::vector<UnexpectedTokenError> &getUnexpectedTokenErrors() const;

    void printErrors() const;


    const std::vector<ParseError> &getErrors() const;


    void printSymbolTable() const;


    std::vector<std::pair<int, uint64_t>> &getDataBuffer();

    void printDataBuffers() const;


    const std::vector<std::pair<ICUnit, bool>> &getIntermediateCode() const;

    void printIntermediateCode() const;


    void parseDataDirective();

    void parseTextDirective();

    void parse();

};


#endif // PARSER_H