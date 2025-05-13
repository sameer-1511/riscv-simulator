/**
 * File Name: command_handler.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#include "command_handler.h"


CommandHandler::Command CommandHandler::parseCommand(const std::string &input) {
    std::istringstream iss(input);
    std::string command_str;
    iss >> command_str;

    CommandHandler::CommandType command_type = CommandHandler::CommandType::INVALID;
    if (command_str == "load" || command_str == "l") {
        command_type = CommandHandler::CommandType::LOAD;
    } else if (command_str == "run") {
        command_type = CommandHandler::CommandType::RUN;
    } else if (command_str == "debug") {
        command_type = CommandHandler::CommandType::DEBUG;
    } else if (command_str == "step" || command_str == "s") {
        command_type = CommandHandler::CommandType::STEP;
    } else if (command_str == "reset") {
        command_type = CommandHandler::CommandType::RESET;
    } else if (command_str == "dump_mem") {
        command_type = CommandHandler::CommandType::DUMP_MEMORY;
    } else if (command_str == "print_mem" || command_str == "pmem") {
        command_type = CommandHandler::CommandType::PRINT_MEMORY;
    } else if (command_str == "dump_cache") {
        command_type = CommandHandler::CommandType::DUMP_CACHE;
    } else if (command_str == "add_breakpoint") {
        command_type = CommandHandler::CommandType::ADD_BREAKPOINT;
    } else if (command_str == "remove_breakpoint") {
        command_type = CommandHandler::CommandType::REMOVE_BREAKPOINT;
    } else if (command_str == "exit") {
        command_type = CommandHandler::CommandType::EXIT;
    }

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }

    return CommandHandler::Command(command_type, args);
}

void CommandHandler::executeCommand(CommandHandler::Command command) {
    (void)command; // Suppress unused variable warning
}