/**
 * File Name: utils.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "pch.h"

int64_t countLines(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file \"" << filename << "\"" << std::endl;
        return -1;
    }

    int64_t lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        lines++;
    }

    file.close();
    return lines;
}

std::string getLineFromFile(const std::string& fileName, int lineNumber) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file.");
    }

    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) {
        if (++currentLine == lineNumber) {
            return line;
        }
    }

    throw std::out_of_range("Line number out of range.");
}

