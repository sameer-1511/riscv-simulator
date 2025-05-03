/**
 * File Name: command_parser.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#include "command_parser.h"


CommandParser::Command CommandParser::parseCommand(const std::string &input) {
    std::istringstream iss(input);
    std::string command_str;
    iss >> command_str;

    CommandParser::CommandType command_type = CommandParser::CommandType::INVALID;
    if (command_str == "start") {
        command_type = CommandParser::CommandType::START;
    } else if (command_str == "run") {
        command_type = CommandParser::CommandType::RUN;
    } else if (command_str == "debug") {
        command_type = CommandParser::CommandType::DEBUG;
    } else if (command_str == "step") {
        command_type = CommandParser::CommandType::STEP;
    } else if (command_str == "reset") {
        command_type = CommandParser::CommandType::RESET;
    } else if (command_str == "dump_memory") {
        command_type = CommandParser::CommandType::DUMP_MEMORY;
    } else if (command_str == "dump_cache") {
        command_type = CommandParser::CommandType::DUMP_CACHE;
    } else if (command_str == "add_breakpoint") {
        command_type = CommandParser::CommandType::ADD_BREAKPOINT;
    } else if (command_str == "remove_breakpoint") {
        command_type = CommandParser::CommandType::REMOVE_BREAKPOINT;
    } else if (command_str == "exit") {
        command_type = CommandParser::CommandType::EXIT;
    }

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }

    return CommandParser::Command(command_type, args);
}