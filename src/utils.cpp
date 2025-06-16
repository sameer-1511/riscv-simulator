/**
 * @file utils.cpp
 * @brief Contains utility functions for the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */


#include "utils.h"
#include "vm/registers.h"
#include "globals.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>

void setupVmStateDirectory() {
  std::filesystem::path vm_state_dir = std::filesystem::path(".") / "vm_state";
  if (!std::filesystem::exists(vm_state_dir)) {
    std::filesystem::create_directories(vm_state_dir);
  }

  std::filesystem::path registers_file = vm_state_dir / "registers_dump.json";
  std::filesystem::path errors_file = vm_state_dir / "errors_dump.json";
  // std::filesystem::path memory_dump_file = vm_state_dir / "memory_dump.json";
  // std::filesystem::path cache_dump_file_path = vm_state_dir / "cache_dump.json";
  std::filesystem::path vm_state_dump_file_path = vm_state_dir / "vm_state_dump.json";

  if (!std::filesystem::exists(registers_file)) {
    std::ofstream(registers_file).close();
  }
  if (!std::filesystem::exists(errors_file)) {
    std::ofstream(errors_file).close();
  }
  if (!std::filesystem::exists(globals::memory_dump_file_path)) {
    std::ofstream(globals::memory_dump_file_path).close();
  }
  if (!std::filesystem::exists(globals::cache_dump_file_path)) {
    std::ofstream(globals::cache_dump_file_path).close();
  }
  if (!std::filesystem::exists(vm_state_dump_file_path)) {
    std::ofstream(vm_state_dump_file_path).close();
  }

  if (!std::filesystem::exists(globals::config_file_path)) {
    SetupConfigFile();
  }

  
}


int64_t CountLines(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::ios_base::failure("Could not open the file.");
  }

  int64_t lines = 0;
  std::string line;
  while (std::getline(file, line)) {
    lines++;
  }

  file.close();
  return lines;
}

std::string GetLineFromFile(const std::string &fileName, unsigned int lineNumber) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    throw std::ios_base::failure("Could not open the file.");
  }

  std::string line;
  unsigned int currentLine = 0;

  while (std::getline(file, line)) {
    if (++currentLine==lineNumber) {
      return line;
    }
  }

  throw std::out_of_range("Line number out of range.");
}

std::string ParseEscapedString(const std::string &input) {
  std::ostringstream oss;
  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i]=='\\' && i + 1 < input.size()) {
      switch (input[i + 1]) {
        case 'n': oss.put('\n');
          ++i;
          break; // Newline
        case 't': oss.put('\t');
          ++i;
          break; // Tab
        case '\\': oss.put('\\');
          ++i;
          break; // Backslash
        case '"': oss.put('"');
          ++i;
          break; // Double quote
        default: oss.put('\\');
          oss.put(input[i + 1]);
          ++i;
          break; // Unhandled escape
      }
    } else {
      oss.put(input[i]);
    }
  }
  return oss.str();
}

void DumpErrors(const std::filesystem::path &filename, const std::vector<ParseError> &errors) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename.string());
  }

  file << "{\n";
  file << "    \"errorCode\": 1,\n";
  file << "    \"errors\": [\n";

  for (size_t i = 0; i < errors.size(); ++i) {
    const auto &error = errors[i];
    file << "        {\n";
    file << "            \"line\": " << error.line << ",\n";
    file << "            \"message\": \"" << error.message << "\"\n";
    file << "        }";
    if (i!=errors.size() - 1) {
      file << ",";
    }
    file << "\n";
  }

  file << "    ]\n";
  file << "}\n";

  file.close();
}

void DumpNoErrors(const std::filesystem::path &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename.string());
  }

  file << "{\n";
  file << "    \"errorCode\": 0,\n";
  file << "    \"errors\": []\n";
  file << "}\n";

  file.close();
}

void DumpRegisters(const std::filesystem::path &filename, RegisterFile &register_file) {

  std::vector<uint64_t> gp_registers = register_file.GetGprValues();
  std::vector<uint64_t> fp_registers = register_file.GetFprValues();

  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename.string());
  }

  file << "{\n";

  file << "    \"control and status registers\": {\n";
  if (!csr_to_address.empty()) {
    auto it = csr_to_address.begin();
    auto end = csr_to_address.end();

    while (true) {
      const auto &key = it->first;
      const auto &value = it->second;

      file << "        \"" << key << "\": \"0x"
           << std::hex << std::setw(16) << std::setfill('0') << register_file.ReadCsr(value)
           << std::setw(0) << std::dec << "\"";

      ++it;
      if (it!=end) {
        file << ",";
      }
      file << "\n";

      if (it==end) break;
    }
  }
  file << "    },\n";

  file << "    \"gp_registers\": {\n";
  for (size_t i = 0; i < gp_registers.size(); ++i) {
    file << "        \"x" << i << "\"";
    file << std::string((i >= 10 ? 0 : 1), ' ');
    file << ": \"0x";
    file << std::hex << std::setw(16) << std::setfill('0')
         << gp_registers[i]
         << std::setw(0) << std::dec << "\"";
    if (i!=gp_registers.size() - 1) {
      file << ",";
    }
    file << "\n";
  }
  file << "    },\n";

  file << "    \"fp_registers\": {\n";
  for (size_t i = 0; i < fp_registers.size(); ++i) {
    file << "        \"f" << i << "\"";
    file << std::string((i >= 10 ? 0 : 1), ' ');
    file << ": \"0x";
    file << std::hex << std::setw(16) << std::setfill('0')
         << fp_registers[i]
         << std::setw(0) << std::dec << "\"";

    if (i!=fp_registers.size() - 1) {
      file << ",";
    }
    file << "\n";
  }
  file << "    }\n";
  // file << "    },\n";

  // file << "    \"vec_registers\": {\n";
  // for (size_t i = 0; i < gp_registers.size(); ++i) {
  //     file << "        \"x" << i << "\": \"0x"
  //          << std::hex << std::setw(16) << std::setfill('0') << gp_registers[i] << std::setw(0) << std::dec << "\"";
  //     if (i != gp_registers.size() - 1) {
  //         file << ",";
  //     }
  //     file << "\n";
  // }
  // file << "    }\n";

  file << "}\n";

  file.close();
}

void SetupConfigFile() {
  std::ofstream config_file(globals::config_file_path);
  if (!config_file.is_open()) {
    throw std::runtime_error("Unable to open config file: " + globals::config_file_path.string());
  }

  config_file << "[General]\n";
  config_file << "name=vm\n\n";

  config_file << "[Execution]\n";
  config_file << "run_step_delay=0   ; in ms\n";
  config_file << "processor_type=single_stage\n";
  config_file << "hazard_detection=false\n";
  config_file << "forwarding=false\n";
  config_file << "branch_prediction=none\n\n";

  config_file << "[Memory]\n";
  config_file << "memory_size=0xffffffffffffffff\n";
  config_file << "block_size=1024\n\n";

  config_file << "[Cache]\n";
  config_file << "cache_enabled=false\n";
  config_file << "cache_size=0\n";
  config_file << "cache_block_size=0\n";
  config_file << "cache_associativity=0\n";
  config_file << "cache_read_miss_policy=read_allocate\n";
  config_file << "cache_replacement_policy=LRU\n";
  config_file << "cache_write_hit_policy=write_back\n";
  config_file << "cache_write_miss_policy=write_allocate\n\n";

  config_file << "[BranchPrediction]\n";
  config_file << "branch_prediction_type=always_not_taken\n";
  config_file << "branch_prediction_table_size=0\n";
  config_file << "branch_prediction_table_associativity=0\n";
  config_file.close();
}