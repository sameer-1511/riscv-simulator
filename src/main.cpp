#include "pch.h"

#include "main.h"
#include "./assembler/assembler.h"
#include "./assembler/elf_util.h"
#include "utils.h"
#include "globals.h"
#include "./vm/rvss/rvss_vm.h"
#include "vm_runner.h"
#include "command_handler.h"

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cerr << "No arguments provided. Use --help for usage information.\n";
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h") {
        std::cout << "Usage: " << argv[0] << " [options]\n"
                  << "Options:\n"
                  << "  --help, -h           Show this help message\n"
                  << "  --assemble <file>    Assemble the specified file\n"
                  << "  --run <file>         Run the specified file\n"
                  << "  --verbose-errors     Enable verbose error printing\n"
                  << "  --start-vm           Start the VM with the default program\n";
        return 0;

    } else if (arg == "--assemble") {
        if (++i >= argc) {
            std::cerr << "Error: No file specified for assembly.\n";
            return 1;
        }
        try {
            AssembledProgram program = assemble(argv[i]);
            std::cout << "Assembled program: " << program.filename << '\n';
            return 0;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return 1;
        }

    } else if (arg == "--run") {
        if (++i >= argc) {
            std::cerr << "Error: No file specified to run.\n";
            return 1;
        }
        try {
            AssembledProgram program = assemble(argv[i]);
            RVSSVM vm;
            vm.LoadProgram(program);
            vm.Run();
            std::cout << "Program running: " << program.filename << '\n';
            return 0;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return 1;
        }

    } else if (arg == "--verbose-errors") {
        globals::verbose_errors_print = true;
        std::cout << "Verbose error printing enabled.\n";

    } else if (arg == "--start-vm") {
        break;

    } else {
        std::cerr << "Unknown option: " << arg << '\n';
        return 1;
    }
  }


  setupVmStateDirectory();




  AssembledProgram program;
  RVSSVM vm;
  // try {
  //   program = assemble("/home/vis/Desk/codes/assembler/examples/f_d_test.s");
  // } catch (const std::runtime_error &e) {
  //   std::cerr << e.what() << '\n';
  //   return 0;
  // }

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
      try {
        program = assemble(command.args[0]);
        std::cout << "VM_PARSE_SUCCESS" << std::endl;
        vm.output_status_ = "VM_PARSE_SUCCESS";
        vm.DumpState(globals::vm_state_dump_file);
      } catch (const std::runtime_error &e) {
        std::cout << "VM_PARSE_ERROR" << std::endl;
        vm.output_status_ = "VM_PARSE_ERROR";
        vm.DumpState(globals::vm_state_dump_file);
        std::cerr << e.what() << '\n';
        continue;
      }
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
      try {
        vm.memory_controller_.DumpMemory(command.args);
      } catch (const std::out_of_range &e) {
        std::cout << "VM_MEMORY_DUMP_ERROR" << std::endl;
        continue;
      } catch (const std::exception& e) {
        std::cout << "VM_MEMORY_DUMP_ERROR" << std::endl;
        continue;
      }
    } else if (command.type==command_handler::CommandType::PRINT_MEMORY) {
      for (size_t i = 0; i < command.args.size(); i+=2) {
        uint64_t address = std::stoull(command.args[i], nullptr, 16);
        uint64_t rows = std::stoull(command.args[i+1]);
        vm.memory_controller_.PrintMemory(address, rows);
      }
      std::cout << std::endl;
    } else if (command.type==command_handler::CommandType::GET_MEMORY_POINT) {
      if (command.args.size() != 1) {
        std::cout << "VM_GET_MEMORY_POINT_ERROR" << std::endl;
        continue;
      }
      uint64_t address = std::stoull(command.args[0], nullptr, 16);
      std::cout << vm.memory_controller_.GetMemoryPoint(address) << std::endl;
    } 
    
    
    else if (command.type==command_handler::CommandType::DUMP_CACHE) {
      std::cout << "Cache dumped." << std::endl;
    } else {
      std::cout << "Invalid command." << std::endl;
    }

  }






  return 0;
}