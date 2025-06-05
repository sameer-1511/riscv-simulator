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
  setupVmStateDirectory();




  AssembledProgram program;
  RVSSVM vm;
  try {
    program = assemble("/home/vis/Desk/codes/assembler/examples/test2.s");
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

  // vm.LoadProgram(program);

  std::cout << "VM_STARTED" << std::endl;

  std::string command_buffer;
  while (true) {
    // std::cout << "=> ";
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
      // std::cout << "Debugging..." << std::endl;
    } else if (command.type==command_handler::CommandType::STEP) {
      vm.Step();
      // std::cout << "Stepping..." << std::endl;
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
      vm.output_status_ = "VM_STOPPED";
      vm.DumpState(globals::vm_state_dump_file);
      break;
    } else if (command.type==command_handler::CommandType::ADD_BREAKPOINT) {
      vm.AddBreakpoint(std::stoul(command.args[0], nullptr, 10));
    } else if (command.type==command_handler::CommandType::REMOVE_BREAKPOINT) {
      vm.RemoveBreakpoint(std::stoul(command.args[0], nullptr, 10));
    } else if (command.type==command_handler::CommandType::DUMP_MEMORY) {
      vm.memory_controller_.DumpMemory(command.args);
      // std::cout << "Memory dumped." << std::endl;
    } else if (command.type==command_handler::CommandType::PRINT_MEMORY) {
      for (size_t i = 0; i < command.args.size(); i+=2) {
        uint64_t address = std::stoull(command.args[i], nullptr, 16);
        uint64_t rows = std::stoull(command.args[i+1]);
        vm.memory_controller_.PrintMemory(address, rows);
      }
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