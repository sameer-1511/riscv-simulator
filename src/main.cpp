#include "pch.h"

#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

#include "./assembler/assembler.h"

#include "./vm/memory_controller.h"

#include "./assembler/elf_util.h"
#include "utils.h"
#include "globals.h"

#include "./vm/rvss/rvss_vm.h"

#include "vm_runner.h"

#include "command_parser.h"


int main() {
    // AssembledProgram program;
    // try {
    //     program = assemble("/home/vis/Desk/codes/assembler/examples/e4.s");
    // } catch (const std::runtime_error &e) {
    //     std::cerr << e.what() << '\n';
    //     return 0;
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

    // AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/branch_test.s");
    
    // VMRunner vmRunner;
    // vmRunner.custom();
    // vmRunner.loadProgram(program);

    AssembledProgram program;
    RVSSVM vm;
    // vm.loadProgram(program);

    std::string command_buffer;
    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, command_buffer);
        CommandParser::Command command = CommandParser::parseCommand(command_buffer);
        if (command.type == CommandParser::CommandType::START) {
            program = assemble(command.args[0]);
            vm.loadProgram(program);
            std::cout << "Program loaded: " << command.args[0] << std::endl;
        } else if (command.type == CommandParser::CommandType::RUN) {
            vm.run();
            std::cout << "Program running..." << std::endl;
        } else if (command.type == CommandParser::CommandType::DEBUG) {
            vm.debugRun();
            std::cout << "Debugging..." << std::endl;
        } else if (command.type == CommandParser::CommandType::STEP) {
            vm.step();
            std::cout << "Stepping..." << std::endl;
        } else if (command.type == CommandParser::CommandType::RESET) {
            vm.reset();
            std::cout << "VM reset." << std::endl;
        } else if (command.type == CommandParser::CommandType::EXIT) {
            break;
        } else if (command.type == CommandParser::CommandType::ADD_BREAKPOINT) {
            vm.addBreakpoint(std::stoul(command.args[0], nullptr, 16));
            std::cout << "Breakpoint added at address: " << command.args[0] << std::endl;
        } else if (command.type == CommandParser::CommandType::REMOVE_BREAKPOINT) {
            vm.removeBreakpoint(std::stoul(command.args[0], nullptr, 16));
            std::cout << "Breakpoint removed at address: " << command.args[0] << std::endl;
        } else if (command.type == CommandParser::CommandType::DUMP_MEMORY) {
            std::cout << "Memory dumped." << std::endl;
        } else if (command.type == CommandParser::CommandType::DUMP_CACHE) {
            std::cout << "Cache dumped." << std::endl;
        } else {
            std::cout << "Invalid command." << std::endl;
        }
        
        // Handle other commands here
    }




    // std::string buffer;
    // std::getline(std::cin, buffer);
    // CommandParser::Command command = CommandParser::parseCommand(buffer);
    // std::cout << "Command: " << static_cast<int>(command.type) << std::endl;
    // std::cout << "Arguments: ";
    // for (const auto &arg : command.args) {
    //     std::cout << arg << " ";
    // }
    // std::cout << std::endl;

    // char c;
    // std::cout << "Press 's' to step, 'q' to quit";
    // while (true) {
    //     std::cout << "Input: ";
    //     std::cin >> c;
    //     if (c == 's') {
    //         vm.step();
    //     } else if (c == 'r') {
    //         vm.run();
    //     } else if (c == 'q') {
    //         break;
    //     }
    // }

    // generateElfFile(program, "/home/vis/Desk/codes/assembler/examples/e1.elf");

    // std::cout << "globals::config_file: " << globals::config_file << std::endl;


    return 0;
}