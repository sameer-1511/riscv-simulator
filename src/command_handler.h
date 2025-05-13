/**
 * File Name: command_handler.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#include "pch.h"

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

namespace CommandHandler {
    enum class CommandType {
        INVALID,
        LOAD,
        RUN,
        DEBUG,
        STEP,
        RESET,
        DUMP_MEMORY,
        PRINT_MEMORY,
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

    void executeCommand(Command command);
    


} // namespace CommandParser





#endif // COMMAND_HANDLER_H