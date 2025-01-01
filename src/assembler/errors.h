/**
 * File Name: errors.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef ERRORS_H
#define ERRORS_H

#include "../pch.h"

const std::string ANSI_code_red = "\033[31m";
const std::string ANSI_code_reset = "\033[0m";


enum class ErrorType {
    UNEXPECTED_TOKEN,
    UNEXPECTED_EOF,
    UNEXPECTED_OPERAND,

    MISSING_OPERAND,
    MISSING_COMMA,
    MISSING_LPAREN,
    MISSING_RPAREN,

    EXPECTED_REGISTER,
    EXPECTED_IMMEDIATE,
    EXPECTED_LABEL,

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


struct SyntaxError {
    std::string main_message;
    std::string sub_message;
    std::string filename;
    int line_number;
    int column_number;
    std::string line_text;

    SyntaxError(std::string main_message, std::string sub_message, std::string filename, int line_number,
                int column_number, std::string line_text)
            : main_message(std::move(main_message)), sub_message(std::move(sub_message)),
              filename(std::move(filename)), line_number(line_number), column_number(column_number),
              line_text(std::move(line_text)) {}

    friend std::ostream &operator<<(std::ostream &os, const SyntaxError &error);
};

struct UnexpectedTokenError {
    std::string message;
    std::string filename;
    int line_number;
    int column_number;
    std::string line_text;

    UnexpectedTokenError(std::string message, std::string filename, int line_number, int column_number,
                         std::string line_text)
            : message(std::move(message)), filename(std::move(filename)), line_number(line_number),
              column_number(column_number), line_text(std::move(line_text)) {}

    friend std::ostream &operator<<(std::ostream &os, const UnexpectedTokenError &error);
};

struct ImmediateOutOfRangeError {
    std::string main_message;
    std::string sub_message;
    std::string filename;
    int line_number;
    int column_number;
    std::string line_text;

    ImmediateOutOfRangeError(std::string main_message, std::string sub_message, std::string filename, int line_number,
                             int column_number, std::string line_text)
            : main_message(std::move(main_message)), sub_message(std::move(sub_message)),
              filename(std::move(filename)), line_number(line_number), column_number(column_number),
              line_text(std::move(line_text)) {}

    friend std::ostream &operator<<(std::ostream &os, const ImmediateOutOfRangeError &error);
};

struct MisallignedImmediateError {
    std::string main_message;
    std::string sub_message;
    std::string filename;
    int line_number;
    int column_number;
    std::string line_text;

    MisallignedImmediateError(std::string main_message, std::string sub_message, std::string filename, int line_number,
                              int column_number, std::string line_text)
            : main_message(std::move(main_message)), sub_message(std::move(sub_message)),
              filename(std::move(filename)), line_number(line_number), column_number(column_number),
              line_text(std::move(line_text)) {}

    friend std::ostream &operator<<(std::ostream &os, const MisallignedImmediateError &error);
};

struct UnexpectedOperandError {
    std::string main_message;
    std::string sub_message;
    std::string filename;
    int line_number;
    int column_number;
    std::string line_text;

    UnexpectedOperandError(std::string main_message, std::string sub_message, std::string filename, int line_number,
                           int column_number, std::string line_text)
            : main_message(std::move(main_message)), sub_message(std::move(sub_message)),
              filename(std::move(filename)), line_number(line_number), column_number(column_number),
              line_text(std::move(line_text)) {}

    friend std::ostream &operator<<(std::ostream &os, const UnexpectedOperandError &error);
};

struct InvalidLabelRefError {
    std::string main_message;
    std::string sub_message;
    std::string filename;
    int line_number;
    int column_number;
    std::string line_text;

    InvalidLabelRefError(std::string main_message, std::string sub_message, std::string filename, int line_number,
                      int column_number, std::string line_text)
            : main_message(std::move(main_message)), sub_message(std::move(sub_message)),
              filename(std::move(filename)), line_number(line_number), column_number(column_number),
              line_text(std::move(line_text)) {}

    friend std::ostream &operator<<(std::ostream &os, const InvalidLabelRefError &error);
};




#endif // ERRORS_H