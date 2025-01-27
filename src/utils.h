/**
 * @file utils.h
 * @brief Contains utility functions for file I/O and string manipulation.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef UTILS_H
#define UTILS_H

#include "pch.h"
#include "./assembler/parser.h"

/**
 * @brief Counts the number of lines in a given file.
 * 
 * @param filename The name of the file to count lines in.
 * @return The number of lines in the file.
 * @throws std::ios_base::failure If the file cannot be opened.
 */
int64_t countLines(const std::string &filename);

/**
 * @brief Retrieves a specific line from a file.
 * 
 * @param fileName The name of the file to read from.
 * @param lineNumber The 1-based index of the line to retrieve.
 * @return The content of the specified line as a string.
 * @throws std::out_of_range If the lineNumber is invalid.
 * @throws std::ios_base::failure If the file cannot be opened.
 */
std::string getLineFromFile(const std::string& fileName, unsigned int lineNumber);

/**
 * @brief Parses a string containing escaped characters into its unescaped form.
 * 
 * @param input The input string with escaped characters.
 * @return The unescaped string.
 */
std::string parseEscapedString(const std::string &input);

void dumpErrors(const std::string &filename, const std::vector<ParseError> &errors);


#endif // UTILS_H
