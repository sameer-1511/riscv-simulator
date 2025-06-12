#include "globals.h"
#include <filesystem>

std::filesystem::path globals::config_file_path = (std::filesystem::path(".")/"vm_state"/"config.ini");
std::filesystem::path globals::errors_dump_file_path = (std::filesystem::path(".")/"vm_state"/"errors_dump.json");
std::filesystem::path globals::registers_dump_file_path = (std::filesystem::path(".")/"vm_state"/"registers_dump.json");
std::filesystem::path globals::memory_dump_file_path = (std::filesystem::path(".")/"vm_state"/"memory_dump.json");
std::filesystem::path globals::cache_dump_file_path = (std::filesystem::path(".")/"vm_state"/"cache_dump.json");
std::filesystem::path globals::vm_state_dump_file_path = (std::filesystem::path(".")/"vm_state"/"vm_state_dump.json");

bool globals::verbose_errors_print = false;
bool globals::verbose_warnings = false;


unsigned int globals::data_section_start = 0x10000000;
