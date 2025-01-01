/**
 * File Name: errors.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "errors.h"

#include "../pch.h"

std::ostream &operator<<(std::ostream &os, const SyntaxError &error) {
    os << error.filename << ":" << error.line_number << ":" << error.column_number << ": "
       << ANSI_code_red << "[ERROR]" << ANSI_code_reset << " "
       << error.main_message << "\n"
       << std::setw(6) << std::right << error.line_number << " | " << error.line_text << "\n"
       << std::setw(6) << " " << " | " << ANSI_code_red << std::setw(error.column_number) << std::right << "^"
       << ANSI_code_reset << "\n"
       << std::setw(6) << " " << " | " << error.sub_message << "\n"
       << std::setw(6) << " " << " | " << "\n"
       << std::setw(0); // Reset std::setw to default
    return os;
}

std::ostream &operator<<(std::ostream &os, const UnexpectedTokenError &error) {
    os << error.filename << ":" << error.line_number << ":" << error.column_number << ": "
       << ANSI_code_red << "[ERROR]" << ANSI_code_reset << " "
       << error.message << "\n"
       << std::setw(6) << std::right << error.line_number << " | " << error.line_text << "\n"
       << std::setw(6) << " " << " | " << ANSI_code_red << std::setw(error.column_number) << std::right << "^"
       << ANSI_code_reset << "\n"
       << std::setw(6) << " " << " | " << "\n"
       << std::setw(0); // Reset std::setw to default
    return os;
}

std::ostream &operator<<(std::ostream &os, const ImmediateOutOfRangeError &error) {
    os << error.filename << ":" << error.line_number << ":" << error.column_number << ": "
       << ANSI_code_red << "[ERROR]" << ANSI_code_reset << " "
       << error.main_message << "\n"
       << std::setw(6) << std::right << error.line_number << " | " << error.line_text << "\n"
       << std::setw(6) << " " << " | " << ANSI_code_red << std::setw(error.column_number) << std::right << "^"
       << ANSI_code_reset << "\n"
       << std::setw(6) << " " << " | " << error.sub_message << "\n"
       << std::setw(6) << " " << " | " << "\n"
       << std::setw(0); // Reset std::setw to default
    return os;
}

std::ostream &operator<<(std::ostream &os, const MisallignedImmediateError &error) {
    os << error.filename << ":" << error.line_number << ":" << error.column_number << ": "
       << ANSI_code_red << "[ERROR]" << ANSI_code_reset << " "
       << error.main_message << "\n"
       << std::setw(6) << std::right << error.line_number << " | " << error.line_text << "\n"
       << std::setw(6) << " " << " | " << ANSI_code_red << std::setw(error.column_number) << std::right << "^"
       << ANSI_code_reset << "\n"
       << std::setw(6) << " " << " | " << error.sub_message << "\n"
       << std::setw(6) << " " << " | " << "\n"
       << std::setw(0); // Reset std::setw to default
    return os;
}

std::ostream &operator<<(std::ostream &os, const UnexpectedOperandError &error) {
    os << error.filename << ":" << error.line_number << ":" << error.column_number << ": "
       << ANSI_code_red << "[ERROR]" << ANSI_code_reset << " "
       << error.main_message << "\n"
       << std::setw(6) << std::right << error.line_number << " | " << error.line_text << "\n"
       << std::setw(6) << " " << " | " << ANSI_code_red << std::setw(error.column_number) << std::right << "^"
       << ANSI_code_reset << "\n"
       << std::setw(6) << " " << " | " << error.sub_message << "\n"
       << std::setw(6) << " " << " | " << "\n"
       << std::setw(0); // Reset std::setw to default
    return os;
}

std::ostream &operator<<(std::ostream &os, const InvalidLabelRefError &error) {
    os << error.filename << ":" << error.line_number << ":" << error.column_number << ": "
       << ANSI_code_red << "[ERROR]" << ANSI_code_reset << " "
       << error.main_message << "\n"
       << std::setw(6) << std::right << error.line_number << " | " << error.line_text << "\n"
       << std::setw(6) << " " << " | " << ANSI_code_red << std::setw(error.column_number) << std::right << "^"
       << ANSI_code_reset << "\n"
       << std::setw(6) << " " << " | " << error.sub_message << "\n"
       << std::setw(6) << " " << " | " << "\n"
       << std::setw(0); // Reset std::setw to default
    return os;
}

