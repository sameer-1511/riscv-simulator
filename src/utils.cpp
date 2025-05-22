/**
 * @file utils.cpp
 * @brief Contains utility functions for the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "pch.h"

#include "utils.h"
#include "vm/registers.h"

int64_t countLines(const std::string &filename) {
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

std::string getLineFromFile(const std::string &fileName, unsigned int lineNumber) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::ios_base::failure("Could not open the file.");
    }

    std::string line;
    unsigned int currentLine = 0;

    while (std::getline(file, line)) {
        if (++currentLine == lineNumber) {
            return line;
        }
    }

    throw std::out_of_range("Line number out of range.");
}

std::string parseEscapedString(const std::string &input) {
    std::ostringstream oss;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size()) {
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

void dumpErrors(const std::string &filename, const std::vector<ParseError> &errors) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
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
        if (i != errors.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "    ]\n";
    file << "}\n";

    file.close();
}

void dumpNoErrors(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    file << "{\n";
    file << "    \"errorCode\": 0,\n";
    file << "    \"errors\": []\n";
    file << "}\n";

    file.close();
}

void dumpRegisters(const std::string &filename, RegisterFile &register_file) {

    std::vector<uint64_t> gp_registers = register_file.getGPRValues();
    std::vector<uint64_t> fp_registers = register_file.getFPRValues();

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
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
                << std::hex << std::setw(16) << std::setfill('0') << register_file.readCSR(value)
                << std::setw(0) << std::dec << "\"";

            ++it;
            if (it != end) {
                file << ",";
            }
            file << "\n";

            if (it == end) break;
        }
    }
    file << "    },\n";

    file << "    \"gp_registers\": {\n";
    for (size_t i = 0; i < gp_registers.size(); ++i) {
        file << "        \"x" << i << "\"" ;
        file << std::string((i >= 10 ? 0 : 1), ' ');
        file  << ": \"0x";
        file << std::hex << std::setw(16) << std::setfill('0') 
             << gp_registers[i] 
             << std::setw(0) << std::dec << "\"";
        if (i != gp_registers.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    file << "    },\n";

    file << "    \"fp_registers\": {\n";
    for (size_t i = 0; i < fp_registers.size(); ++i) {
        file << "        \"x" << i << "\"" ;
        file << std::string((i >= 10 ? 0 : 1), ' ');
        file  << ": \"0x";
        file << std::hex << std::setw(16) << std::setfill('0') 
             << fp_registers[i] 
             << std::setw(0) << std::dec << "\"";

        if (i != fp_registers.size() - 1) {
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
