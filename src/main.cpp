#include "pch.h"

#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

#include "./assembler/assembler.h"
#include "./common/instructions.h"

#include "./vm/memory_controller.h"

#include "./assembler/elf_util.h"
#include "utils.h"
#include "globals.h"

#include "./vm/rvss/rvss_vm.h"

#include "vm_runner.h"

#include "command_handler.h"

int main() {
  // AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/f_d_test.s");

  // VMRunner vmRunner;
  // vmRunner.custom();
  // vmRunner.LoadProgram(program);





  AssembledProgram program;
  RVSSVM vm;
  try {
    program = assemble("/home/vis/Desk/codes/assembler/examples/f_d_test.s");
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    return 0;
  }

  // std::cout << "Program: " << program.filename << std::endl;

  // unsigned int count = 0;
  // for (const std::bitset<32> &instruction : program.text_buffer) {
  //     std::cout << instruction
  //               << " | "
  //               << std::setw(8) << std::setfill('0') << std::hex << instruction.to_ulong()
  //               << " | "
  //               << std::setw(0) << count
  //               << std::dec << "\n";
  //     count += 4;
  // }

  vm.LoadProgram(program);

  std::string command_buffer;
  while (true) {
    std::cout << "=> ";
    std::getline(std::cin, command_buffer);
    command_handler::Command command = command_handler::ParseCommand(command_buffer);
    if (command.type==command_handler::CommandType::LOAD) {
      program = assemble(command.args[0]);
      vm.LoadProgram(program);
      std::cout << "Program loaded: " << command.args[0] << std::endl;
    } else if (command.type==command_handler::CommandType::RUN) {
      vm.Run();
      std::cout << "Program running..." << std::endl;
    } else if (command.type==command_handler::CommandType::DEBUG) {
      vm.DebugRun();
      std::cout << "Debugging..." << std::endl;
    } else if (command.type==command_handler::CommandType::STEP) {
      vm.Step();
      std::cout << "Stepping..." << std::endl;
    } else if (command.type==command_handler::CommandType::UNDO) {
      vm.Undo();
      std::cout << "Undoing..." << std::endl;
    } else if (command.type==command_handler::CommandType::REDO) {
      vm.Redo();
      std::cout << "Redoing..." << std::endl;
    } else if (command.type==command_handler::CommandType::RESET) {
      vm.Reset();
      std::cout << "VM Reset." << std::endl;
    } else if (command.type==command_handler::CommandType::EXIT) {
      break;
    } else if (command.type==command_handler::CommandType::ADD_BREAKPOINT) {
      vm.AddBreakpoint(std::stoul(command.args[0], nullptr, 16));
      std::cout << "Breakpoint added at address: " << command.args[0] << std::endl;
    } else if (command.type==command_handler::CommandType::REMOVE_BREAKPOINT) {
      vm.RemoveBreakpoint(std::stoul(command.args[0], nullptr, 16));
      std::cout << "Breakpoint removed at address: " << command.args[0] << std::endl;
    } else if (command.type==command_handler::CommandType::DUMP_MEMORY) {
      // uint64_t address = std::stoull(command.args[0], nullptr, 16);
      // uint64_t rows = std::stoull(command.args[1]);
      // vm.memory_controller_.DumpMemory(address, rows);
      // std::cout << "Memory dumped." << std::endl;
    } else if (command.type==command_handler::CommandType::PRINT_MEMORY) {
      uint64_t address = std::stoull(command.args[0], nullptr, 16);
      uint64_t rows = std::stoull(command.args[1]);
      vm.memory_controller_.PrintMemory(address, rows);
      std::cout << std::endl;
    } else if (command.type==command_handler::CommandType::DUMP_CACHE) {
      std::cout << "Cache dumped." << std::endl;
    } else {
      std::cout << "Invalid command." << std::endl;
    }

    // Handle other commands here
  }



  // generateElfFile(program, "/home/vis/Desk/codes/assembler/examples/e1.elf");

  // std::cout << "globals::config_file: " << globals::config_file << std::endl;


  return 0;
}