/**
 * File Name: lexer.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "../pch.h"

#include "lexer.h"
#include "instructions.h"
#include "../registers.h"

#include <regex>


Lexer::Lexer(const std::string &filename) : filename_(filename), pos_(0) {
    input_.open(filename_);
    if (!input_.is_open()) {
        std::cerr << "Error: Could not open file " << filename_ << std::endl;
        exit(1);
    }

    //dword_buffer_ = std::make_unique<std::vector<uint64_t>>();
    //word_buffer_ = std::make_unique<std::vector<uint32_t>>();
    //halfword_buffer_ = std::make_unique<std::vector<uint16_t>>();
    //byte_buffer_ = std::make_unique<std::vector<uint8_t>>();
    //string_buffer_ = std::make_unique<std::vector<std::string>>();

}

Lexer::~Lexer() {
    input_.close();
}


void Lexer::skipWhitespace() {
    while (pos_ < current_line_.size() && std::isspace(current_line_[pos_])) {
        if (current_line_[pos_] == '\n') {
            ++line_number_;
            column_number_ = 0;
        } else {
            ++column_number_;
        }
        ++pos_;
    }
}

void Lexer::skipComment() {
    while (pos_ < current_line_.size() && current_line_[pos_] != '\n') {
        ++pos_;
        ++column_number_;
    }
    if (pos_ < current_line_.size() && current_line_[pos_] == '\n') {
        ++line_number_;
        column_number_ = 0;
    }
}

void Lexer::skipLine() {
    while (pos_ < current_line_.size() && current_line_[pos_] != '\n') {
        ++pos_;
        ++column_number_;
    }
    if (pos_ < current_line_.size() && current_line_[pos_] == '\n') {
        ++line_number_;
        column_number_ = 0;
    }
}


Token Lexer::identifier() {
    size_t start_pos = pos_;
    while (pos_ < current_line_.size() &&
           (std::isalnum(current_line_[pos_]) || current_line_[pos_] == '_' || current_line_[pos_] == '.')) {
        ++pos_;
        ++column_number_;
    }
    if (pos_ < current_line_.size() && current_line_[pos_] == ':') {
        std::string value = current_line_.substr(start_pos, pos_ - start_pos);
        if (value.find('.') != std::string::npos) {
            ++pos_;
            ++column_number_;
            return Token(TokenType::INVALID, value, line_number_, start_pos);
        }
        ++pos_;
        ++column_number_;
        return Token(TokenType::LABEL, value, line_number_, start_pos);
    }
    std::string value = current_line_.substr(start_pos, pos_ - start_pos);


    if (isValidInstruction(value)) {
        return Token(TokenType::OPCODE, value, line_number_, start_pos);
    } else if (isValidRegister(value)) {
        return Token(TokenType::REGISTER, value, line_number_, start_pos);
    } else if (pos_ < current_line_.size() && current_line_[pos_] == ':') {
        return Token(TokenType::LABEL, value, line_number_, start_pos);
    } else {
        return Token(TokenType::LABEL_REF, value, line_number_, start_pos);
    }


    return Token(TokenType::IDENTIFIER, value, line_number_, start_pos);
}

// TODO: add support for floating point numbers
Token Lexer::number() {
    size_t start_pos = pos_;

    while (pos_ < current_line_.size()
           && (std::isdigit(current_line_[pos_])
               || current_line_[pos_] == '-'
               || current_line_[pos_] == 'x'
               || current_line_[pos_] == 'X'
               || current_line_[pos_] == 'o'
               || current_line_[pos_] == 'O'
               || current_line_[pos_] == 'b'
               || current_line_[pos_] == 'B')) {
        ++pos_;
        ++column_number_;
    }

    std::string value = current_line_.substr(start_pos, pos_ - start_pos);

    std::regex hex_regex("^-?0[xX][0-9a-fA-F]+$");
    std::regex binary_regex("^-?0[bB][01]+$");
    std::regex octal_regex("^-?0[oO][0-7]+$");
    std::regex decimal_regex("^-?[0-9]+$");


    int64_t num = 0;

    if (std::regex_match(value, hex_regex)) {
        num = std::stoll(value, nullptr, 16);
    } else if (std::regex_match(value, binary_regex)) {
        num = std::stoll(value, nullptr, 2);
    } else if (std::regex_match(value, octal_regex)) {
        num = std::stoll(value, nullptr, 8);
    } else if (std::regex_match(value, decimal_regex)) {
        num = std::stoll(value);
    } else {
        return Token(TokenType::INVALID, value, line_number_, start_pos);
    }

    return Token(TokenType::NUM, std::to_string(num), line_number_, start_pos);

    /*
        The below code is for character by character parsing of numbers.
        The code is given for reference and is not used in the current implementation.
        The regex based implementation, although slower, is more robust and easier to understand.
        It is recommended to use character by character parsing in real world applications for better performance.
    */
    //{
    //    size_t start_pos = pos_;
    //    bool is_negative = false;
    //
    //    // Check for a leading minus sign
    //    if (current_line_[pos_] == '-') {
    //        is_negative = true;
    //        ++start_pos;
    //        ++pos_;
    //        ++column_number_;
    //
    //        // Ensure there's a digit or a valid number after the minus sign
    //        if (pos_ >= current_line_.size() || 
    //            (!std::isdigit(current_line_[pos_]) && current_line_[pos_] != '0')) {
    //            return Token(TokenType::INVALID, "-", line_number_, start_pos);
    //        }
    //    }
    //
    //    // Check for special number formats (hex, octal, binary)
    //    if (current_line_[pos_] == '0' && pos_ + 1 < current_line_.size()) {
    //        char next_char = current_line_[pos_ + 1];
    //        if (next_char == 'x' || next_char == 'X') {  // Hexadecimal
    //            pos_ += 2;  // Skip "0x"
    //            column_number_ += 2;
    //            while (pos_ < current_line_.size() && 
    //                   (std::isdigit(current_line_[pos_]) || 
    //                    (current_line_[pos_] >= 'a' && current_line_[pos_] <= 'f') || 
    //                    (current_line_[pos_] >= 'A' && current_line_[pos_] <= 'F'))) {
    //                ++pos_;
    //                ++column_number_;
    //            }
    //            std::string value = current_line_.substr(start_pos, pos_ - start_pos);
    //            if (is_negative) {
    //                value = "-" + value;
    //            }
    //            return Token(TokenType::HEX_NUM, value, line_number_, start_pos);
    //        } else if (next_char == 'o' || next_char == 'O') {  // Octal
    //            pos_ += 2;  // Skip "0o"
    //            column_number_ += 2;
    //            while (pos_ < current_line_.size() && 
    //                   current_line_[pos_] >= '0' && current_line_[pos_] <= '7') {
    //                ++pos_;
    //                ++column_number_;
    //            }
    //            std::string value = current_line_.substr(start_pos, pos_ - start_pos);
    //            if (is_negative) {
    //                value = "-" + value;
    //            }
    //            return Token(TokenType::OCTAL_NUM, value, line_number_, start_pos);
    //        } else if (next_char == 'b' || next_char == 'B') {  // Binary
    //            pos_ += 2;  // Skip "0b"
    //            column_number_ += 2;
    //            while (pos_ < current_line_.size() && 
    //                   (current_line_[pos_] == '0' || current_line_[pos_] == '1')) {
    //                ++pos_;
    //                ++column_number_;
    //            }
    //            std::string value = current_line_.substr(start_pos, pos_ - start_pos);
    //            if (is_negative) {
    //                value = "-" + value;
    //            }
    //            return Token(TokenType::BINARY_NUM, value, line_number_, start_pos);
    //        }
    //    }
    //
    //    // Parse decimal numbers
    //    while (pos_ < current_line_.size() && std::isdigit(current_line_[pos_])) {
    //        ++pos_;
    //        ++column_number_;
    //    }
    //
    //    // Construct the value string, including the minus sign if necessary
    //    std::string value = current_line_.substr(start_pos, pos_ - start_pos);
    //    if (is_negative) {
    //        value = "-" + value;
    //    }
    //
    //    return Token(TokenType::DECIMAL_NUM, value, line_number_, start_pos);
    //}
}

Token Lexer::directive() {
    ++pos_;
    size_t start_pos = pos_;
    while (pos_ < current_line_.size() && std::isalpha(current_line_[pos_])) {
        ++pos_;
        ++column_number_;
    }
    std::string value = current_line_.substr(start_pos, pos_ - start_pos);
    return Token(TokenType::DIRECTIVE, value, line_number_, start_pos);
}

Token Lexer::stringLiteral() {
    ++pos_;
    ++column_number_;
    size_t start_pos = pos_;

    while (pos_ < current_line_.size() && current_line_[pos_] != '"') {
        ++pos_;
        ++column_number_;
    }

    if (pos_ == current_line_.size() || current_line_[pos_] != '"') {
        std::cerr << "Error: Unterminated string literal at line " << line_number_ << std::endl;
        return Token(TokenType::INVALID, "", line_number_, start_pos);
    }

    std::string value = current_line_.substr(start_pos, pos_ - start_pos);
    ++pos_;
    ++column_number_;
    return Token(TokenType::STRING, value, line_number_, start_pos);
}

Token Lexer::getNextToken() {
    skipWhitespace();

    if (pos_ >= current_line_.size()) {
        return Token(TokenType::EOF_, "", line_number_, pos_);
    }

    char current_char = current_line_[pos_];

    if (std::isalpha(current_char) || current_char == '_') {
        return identifier();
    } else if (std::isdigit(current_char) || current_char == '-') {
        return number();
    } else if (current_char == ',') {
        ++pos_;
        ++column_number_;
        return Token(TokenType::COMMA, ",", line_number_, pos_);
    } else if (current_char == '"') {
        return stringLiteral();
    } else if (current_char == '(') {
        ++pos_;
        ++column_number_;
        return Token(TokenType::LPAREN, "(", line_number_, pos_);
    } else if (current_char == ')') {
        ++pos_;
        ++column_number_;
        return Token(TokenType::RPAREN, ")", line_number_, pos_);
    } else if (current_char == '.') {
        return directive();
    } else if (current_char == '#' || current_char == ';') {
        skipComment();
        return getNextToken();
    } else {
        //std::cerr << "Error: Invalid character " << current_char << " at line " << line_number_ << std::endl;
        skipLine();
        return Token(TokenType::INVALID, "", line_number_, pos_);
    }

}

std::vector<Token> Lexer::getTokenList() {
    std::vector<Token> tokens;
    line_number_ = 0;

    while (std::getline(input_, current_line_)) {
        pos_ = 0;
        column_number_ = 0;
        line_number_++;
        while (pos_ < current_line_.size()) {
            Token token = getNextToken();
            if (token.type == TokenType::INVALID) {
                std::cerr << "Error: Invalid token at line " << line_number_ << std::endl;
            }
            if (token.type != TokenType::INVALID && token.type != TokenType::EOF_) {
                tokens.push_back(token);
            }
        }
    }

    tokens.push_back(Token(TokenType::EOF_, "", line_number_, column_number_));
    return tokens;
}
