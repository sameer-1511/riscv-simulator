/**
 * File Name: utils.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "pch.h"

int64_t countLines(const std::string& filename) {
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

