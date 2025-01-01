/**
 * File Name: utils.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef UTILS_H
#define UTILS_H

#include "pch.h"

int64_t countLines(const std::string &filename);

std::string getLineFromFile(const std::string& fileName, int lineNumber);

#endif // UTILS_H