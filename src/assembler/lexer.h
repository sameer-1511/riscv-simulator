/**
 * File Name: lexer.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef LEXER_H
#define LEXER_H

#include "../pch.h"
#include "tokens.h"

class Lexer {
private:
    std::string filename_;
    std::ifstream input_;
    std::string current_line_;
    size_t line_number_;
    size_t column_number_;
    size_t pos_;

    std::vector<Token> tokens_;

    void skipWhitespace();

    void skipComment();

    void skipLine();

    Token identifier();

    Token number();

    Token directive();

    Token stringLiteral();

    Token nextToken();

public:
    Lexer(const std::string &filename);

    ~Lexer();

    std::string getFilename() const;

    Token getNextToken();

    std::vector<Token> getTokenList();


};

#endif // LEXER_H