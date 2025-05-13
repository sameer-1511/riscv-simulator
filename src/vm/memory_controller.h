/**
 * @file memory_controller.h
 * @brief Contains the declaration of the MemoryController class for managing memory in the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H

#include "../pch.h"
#include "../config.h"
#include "main_memory.h"

/**
 * @brief The MemoryController class is responsible for managing memory in the VM.
 */
class MemoryController {
private:
    Memory memory_; ///< The main memory object.
public:
    /**
     * @brief Constructs a MemoryController object.
     */
    MemoryController() = default;

    void reset() {
        memory_.reset();
    }

    void printCacheStatus() const {

        std::string cacheEnabled = vmConfig::INI::get("Cache", "cache_enabled");
        std::cout << "Cache enabled: " << cacheEnabled << std::endl;
    }

    void writeByte(uint64_t address, uint8_t value) {
        memory_.writeByte(address, value);
    }

    void writeHalfWord(uint64_t address, uint16_t value) {
        memory_.writeHalfWord(address, value);
    }

    void writeWord(uint64_t address, uint32_t value) {
        memory_.writeWord(address, value);
    }

    void writeDoubleWord(uint64_t address, uint64_t value) {
        memory_.writeDoubleWord(address, value);
    }

    uint8_t readByte(uint64_t address) {
        return memory_.readByte(address);
    }

    uint16_t readHalfWord(uint64_t address) {
        return memory_.readHalfWord(address);
    }

    uint32_t readWord(uint64_t address) {
        return memory_.readWord(address);

    }

    uint64_t readDoubleWord(uint64_t address) {
        return memory_.readDoubleWord(address);
    }

    void printMemory(const uint64_t address, uint rows) {
        memory_.printMemory(address, rows);
    }

    void dumpMemory(const uint64_t address, uint rows) {
        memory_.dumpMemory(address, rows);
    }

};

#endif // MEMORY_CONTROLLER_H

