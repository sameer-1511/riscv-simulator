/**
 * File Name: command_handler.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#include "command_handler.h"

#include <string>
#include <sstream>
#include <vector>

namespace command_handler {
Command ParseCommand(const std::string &input) {
  std::istringstream iss(input);
  std::string command_str;
  iss >> command_str;

  command_handler::CommandType command_type = command_handler::CommandType::INVALID;
  if (command_str=="load" || command_str=="l") {
    command_type = command_handler::CommandType::LOAD;
  } else if (command_str=="run") {
    command_type = command_handler::CommandType::RUN;
  } else if (command_str=="debug") {
    command_type = command_handler::CommandType::DEBUG;
  } else if (command_str=="step" || command_str=="s") {
    command_type = command_handler::CommandType::STEP;
  } else if (command_str=="undo" || command_str=="u") {
    command_type = command_handler::CommandType::UNDO;
  } else if (command_str=="redo" || command_str=="r") {
    command_type = command_handler::CommandType::REDO;
  } else if (command_str=="reset") {
    command_type = command_handler::CommandType::RESET;
  } else if (command_str=="modify_register" || command_str=="mreg") {
    command_type = command_handler::CommandType::MODIFY_REGISTER;
  } else if (command_str=="dump_mem" || command_str=="dmem") {
    command_type = command_handler::CommandType::DUMP_MEMORY;
  } else if (command_str=="print_mem" || command_str=="pmem") {
    command_type = command_handler::CommandType::PRINT_MEMORY;
  } else if (command_str=="get_mem_point" || command_str=="gmp") {
    command_type = command_handler::CommandType::GET_MEMORY_POINT;
  } else if (command_str=="dump_cache") {
    command_type = command_handler::CommandType::DUMP_CACHE;
  } else if (command_str=="add_breakpoint") {
    command_type = command_handler::CommandType::ADD_BREAKPOINT;
  } else if (command_str=="remove_breakpoint") {
    command_type = command_handler::CommandType::REMOVE_BREAKPOINT;
  } else if (command_str=="exit" || command_str=="quit" || command_str=="q") {
    command_type = command_handler::CommandType::EXIT;
  }

  std::vector<std::string> args;
  std::string arg;
  while (iss >> arg) {
    args.push_back(arg);
  }

  return Command(command_type, args);
}

void ExecuteCommand(const Command &command, RVSSVM& vm) {
  (void)vm;
  (void)command;
}

} // namespace command_handler
