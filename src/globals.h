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
    //extern std::string output_file;

    void initGlobals();
}


#endif // GLOBALS_H