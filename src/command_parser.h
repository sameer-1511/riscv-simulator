/**
 * File Name: command_parser.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#include "pch.h"

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

namespace CommandParser {
    
    enum class CommandType {
        INVALID,
        START,
        RUN,
        DEBUG,
        STEP,
        RESET,
        DUMP_MEMORY,
        DUMP_CACHE,
        ADD_BREAKPOINT,
        REMOVE_BREAKPOINT,
        EXIT
    };

    enum class CommandArgumentType {
        NONE,
        FILE,
        ADDRESS,
        REGISTER,
        VALUE
    };

    struct Command {
        CommandType type;
        std::vector<std::string> args;

        Command(CommandType type, const std::vector<std::string> &args)
            : type(type), args(args) {}

        
    };

    Command parseCommand(const std::string &input);
    


} // namespace CommandParser





#endif // COMMAND_PARSER_H