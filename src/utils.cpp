/**
 * @file utils.cpp
 * @brief Contains utility functions for the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "pch.h"

int64_t countLines(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Could not open the file.");
    }

    int64_t lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        lines++;
    }

    file.close();
    return lines;
}

std::string getLineFromFile(const std::string& fileName, unsigned int lineNumber) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::ios_base::failure("Could not open the file.");
    }

    std::string line;
    unsigned int currentLine = 0;

    while (std::getline(file, line)) {
        if (++currentLine == lineNumber) {
            return line;
        }
    }

    throw std::out_of_range("Line number out of range.");
}

std::string parseEscapedString(const std::string &input) {
    std::ostringstream oss;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size()) {
            switch (input[i + 1]) {
                case 'n': oss.put('\n'); ++i; break; // Newline
                case 't': oss.put('\t'); ++i; break; // Tab
                case '\\': oss.put('\\'); ++i; break; // Backslash
                case '"': oss.put('"'); ++i; break; // Double quote
                default: oss.put('\\'); oss.put(input[i + 1]); ++i; break; // Unhandled escape
            }
        } else {
            oss.put(input[i]);
        }
    }
    return oss.str();
}


