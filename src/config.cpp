/**
 * @file config.cpp
 * @brief Contains configuration options for the assembler.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "config.h"

void vm_config::ini::Trim(std::string &str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    str = (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::string vm_config::ini::Get(const std::string &section, const std::string &key) {
    std::ifstream file(globals::config_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + globals::config_file_path.string());
    }

    std::string line, current_section;
    while (std::getline(file, line)) {
      Trim(line);

        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }

        if (line[0] == '[' && line.back() == ']') {
            current_section = line.substr(1, line.size() - 2);
          Trim(current_section);
            continue;
        }

        if (current_section == section) {
            size_t eq_pos = line.find('=');
            if (eq_pos == std::string::npos) {
                continue;
            }

            std::string k = line.substr(0, eq_pos);
          Trim(k);

            if (k == key) {
                std::string value = line.substr(eq_pos + 1);
              Trim(value);
                return value;
            }
        }
    }

    throw std::invalid_argument("Key not found: " + key);

}

void vm_config::ini::Set(const std::string &section, const std::string &key, const std::string &value) {
    std::ifstream file(globals::config_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + globals::config_file_path.string());
    }

    std::ostringstream buffer;
    std::string line, current_section;
    bool key_found = false, section_found = false;

    while (std::getline(file, line)) {
        Trim(line);
        if (line[0] == '[' && line.back() == ']') {
            current_section = line.substr(1, line.size() - 2);
          Trim(current_section);
            if (current_section == section) {
                section_found = true;
            }
        }
        if (section_found && !key_found) {
            size_t eq_pos = line.find('=');
            if (eq_pos != std::string::npos) {
                std::string k = line.substr(0, eq_pos);
              Trim(k);

                if (k == key) {
                    line = key + "=" + value;
                    key_found = true;
                }
            }
        }

        buffer << line << "\n";
    }

    file.close();

    if (!section_found) {
        throw std::invalid_argument("Section not found: " + section);
    } else if (!key_found) {
        throw std::invalid_argument("Key not found: " + key);
    }

    std::ofstream out_file(globals::config_file_path, std::ios::trunc);
    if (!out_file.is_open()) {
        throw std::runtime_error("Could not open config file for writing: " + globals::config_file_path.string());
    }
    out_file << buffer.str();
}

vm_config::VmTypes vm_config::GetVmType() {
    std::string vmType = ini::Get("Execution", "processor_type");
    if (vmType == "single_stage") {
        return VmTypes::SINGLE_STAGE;
    } else if (vmType == "multi_stage") {
        return VmTypes::MULTI_STAGE;
    } else {
        throw std::invalid_argument("Unknown VM type: " + vmType);
    }
}