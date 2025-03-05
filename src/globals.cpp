#include "globals.h"

std::string globals::config_file = "/home/vis/Desk/codes/assembler/vm_state/config.ini";
std::string globals::errors_dump_file = "/home/vis/Desk/codes/assembler/vm_state/errors_dump.json";
std::string globals::registers_dump_file = "/home/vis/Desk/codes/assembler/vm_state/registers_dump.json";
std::string globals::memory_dump_file = "/home/vis/Desk/codes/assembler/vm_state/memory_dump.json";
std::string globals::cache_dump_file = "/home/vis/Desk/codes/assembler/vm_state/cache_dump.json";
// std::string globals::elf_dump_file = "/home/vis/Desk/codes/assembler/vm_state/elf_dump.json";
// std::string globals::vm_state_dump_file = "/home/vis/Desk/codes/assembler/vm_state/vm_state_dump.json";


unsigned int globals::data_section_start = 0x10000000;
