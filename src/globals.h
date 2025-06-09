/**
 * @file globals.h
 * @brief Contains global definitions and includes for the assembler.
 * @author Vishank Singh, https://github.com/VishankSingh
 */
#ifndef GLOBALS_H
#define GLOBALS_H

#include "pch.h"

namespace globals {
extern std::string config_file;
extern std::string errors_dump_file;
extern std::string registers_dump_file;
extern std::string memory_dump_file;
extern std::string cache_dump_file;
extern std::string vm_state_dump_file;
//extern std::string output_file;

extern bool verbose_errors_print;
extern bool verbose_warnings;

extern unsigned int data_section_start;

void initGlobals();
}

#endif // GLOBALS_H