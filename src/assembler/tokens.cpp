/** @cond DOXYGEN_IGNORE */
/**
 * File Name: tokens.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
/** @endcond */

#include "../pch.h"
#include "tokens.h"


std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "Token(Type: " << tokenTypeToString(token.type)
       << ", Value: \"" << token.value
       << "\", Line: " << token.line_number
       << ", Column: " << token.column_number << ")";
    return os;
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INVALID:
            return "INVALID     ";
        case TokenType::EOF_:
            return "EOF         ";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER  ";
        case TokenType::DIRECTIVE:
            return "DIRECTIVE   ";
        case TokenType::OPCODE:
            return "OPCODE      ";
        case TokenType::REGISTER:
            return "REGISTER    ";
        case TokenType::NUM:
            return "NUM         ";
        case TokenType::LABEL:
            return "LABEL       ";
        case TokenType::LABEL_REF:
            return "LABEL_REF   ";
        case TokenType::COMMA:
            return "COMMA       ";
        case TokenType::LPAREN:
            return "LPAREN      ";
        case TokenType::RPAREN:
            return "RPAREN      ";
        case TokenType::STRING:
            return "STRING      ";
        default:
            return "UNKNOWN     ";
    }
}