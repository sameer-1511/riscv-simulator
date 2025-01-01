/**
 * File Name: tokens.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef TOKENS_H
#define TOKENS_H


enum class TokenType {
    INVALID,
    EOF_,
    IDENTIFIER,
    DIRECTIVE,
    OPCODE,
    REGISTER,
    NUM,
    LABEL,
    LABEL_REF,
    COMMA,
    LPAREN,
    RPAREN,
    STRING,
};

struct Token {
    TokenType type;
    std::string value;
    int line_number;
    int column_number;

    Token(TokenType type = TokenType::INVALID, const std::string &value = "", int line = 0, int column = 0)
            : type(type), value(value), line_number(line), column_number(column) {}

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

};

inline std::string tokenTypeToString(TokenType type);

#endif // TOKENS_H