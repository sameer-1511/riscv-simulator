#include "globals.h"
#include <filesystem>


std::string globals::config_file = (std::filesystem::path("..") / "vm_state" / "config.ini").string();
std::string globals::errors_dump_file = (std::filesystem::path("..") / "vm_state" / "errors_dump.json").string();
std::string globals::registers_dump_file = (std::filesystem::path("..") / "vm_state" / "registers_dump.json").string();
std::string globals::memory_dump_file = (std::filesystem::path("..") / "vm_state" / "memory_dump.json").string();
std::string globals::cache_dump_file = (std::filesystem::path("..") / "vm_state" / "cache_dump.json").string();
// std::string globals::elf_dump_file = (fs::path("C:/Users/Vishank/Desktop/codes/assembler/vm_state/elf_dump.json")).string();
// std::string globals::vm_state_dump_file = (fs::path("C:/Users/Vishank/Desktop/codes/assembler/vm_state/vm_state_dump.json")).string();



unsigned int globals::data_section_start = 0x10000000;
