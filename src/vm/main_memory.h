/**
 * @file main_memory.h
 * @brief Contains the definition of the MemoryBlock and Memory classes.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H

#include "../pch.h"
#include "../config.h"

/**
 * @brief Represents a memory block containing 1 KB of memory.
 */
struct MemoryBlock {
    std::vector<uint8_t> data; ///< A vector representing the memory block data.
    unsigned int block_size = vmConfig::INI::get("Memory", "block_size").empty() 
                            ? 1024 : std::stoi(vmConfig::INI::get("Memory", "block_size")); ///< The size of the memory block in bytes.

    /**
     * @brief Constructs a MemoryBlock with a size of 1 KB initialized to 0.
     */
    MemoryBlock() {
        data.resize(block_size, 0);
    }
};

/**
 * @brief Represents a memory management system with dynamic memory block allocation.
 */
class Memory {
// TODO: change to private after testing
public:
    std::unordered_map<uint64_t, MemoryBlock> blocks_; ///< A map storing memory blocks, indexed by block index.
    unsigned int block_size_; ///< The size of each memory block in bytes.
    uint64_t memory_size_ = vmConfig::INI::get("Memory", "memory_size").empty() 
                            ? 0xffffffffffffffff : std::stoull(vmConfig::INI::get("Memory", "memory_size").substr(2), nullptr, 16); ///< The total memory size in bytes.

    /**
     * @brief Gets the block index for a given memory address.
     * @param address The memory address.
     * @return The block index corresponding to the address.
     */
    uint64_t getBlockIndex(uint64_t address) const;

    /**
     * @brief Gets the offset within a block for a given memory address.
     * @param address The memory address.
     * @return The offset within the block corresponding to the address.
     */
    uint64_t getBlockOffset(uint64_t address) const;

    /**
     * @brief Checks if a memory block is present at the specified index.
     * @param block_index The index of the block to check.
     * @return True if the block is present, false otherwise.
     */
    bool isBlockPresent(uint64_t block_index) const;

    /**
     * @brief Ensures that a memory block exists at the specified index, if not then adds it.
     * @param block_index The index of the block to check or create.
     */
    void ensureBlockExists(uint64_t block_index);

    /**
     * @brief Generic function to read data of type T from the memory.
     * @tparam T The type of data to read.
     * @param address The memory address to read from.
     * @return The value read from the specified memory address.
     */
    template <typename T>
    T readGeneric(uint64_t address);

    /**
     * @brief Generic function to write data of type T to the memory.
     * @tparam T The type of data to write.
     * @param address The memory address to write to.
     * @param value The value to write to the specified memory address.
     */
    template <typename T>
    void writeGeneric(uint64_t address, T value);

public:
    /**
     * @brief Constructs a Memory object.
     */
    Memory() {
        block_size_ = vmConfig::INI::get("Memory", "block_size").empty() 
                    ? 1024 : std::stoi(vmConfig::INI::get("Memory", "block_size"));
    }
    /**
     * @brief Destroys the Memory object.
     */
    ~Memory() = default;

    void reset() {
        blocks_.clear();
    }

    /**
     * @brief Reads a single byte from the given memory address.
     * @param address The memory address to read from.
     * @return The byte value at the given address.
     */
    uint8_t read(uint64_t address);

    /**
     * @brief Writes a single byte to the given memory address.
     * @param address The memory address to write to.
     * @param value The byte value to write.
     */
    void write(uint64_t address, uint8_t value);

     /**
     * @brief Reads a single byte from the given memory address.
     * @param address The memory address to read from.
     * @return The byte value at the given address.
     */
    uint8_t readByte(uint64_t address);

    /**
     * @brief Reads a 16-bit halfword from the given memory address.
     * @param address The memory address to read from.
     * @return The 16-bit value at the given address.
     */
    uint16_t readHalfWord(uint64_t address);

    /**
     * @brief Reads a 32-bit word from the given memory address.
     * @param address The memory address to read from.
     * @return The 32-bit value at the given address.
     */
    uint32_t readWord(uint64_t address);

    /**
     * @brief Reads a 64-bit double word from the given memory address.
     * @param address The memory address to read from.
     * @return The 64-bit value at the given address.
     */
    uint64_t readDoubleWord(uint64_t address);

    float readFloat(uint64_t address);

    double readDouble(uint64_t address);

    /**
     * @brief Writes a single byte to the given memory address.
     * @param address The memory address to write to.
     * @param value The byte value to write.
     */
    void writeByte(uint64_t address, uint8_t value);

    /**
     * @brief Writes a 16-bit halfword to the given memory address.
     * @param address The memory address to write to.
     * @param value The 16-bit value to write.
     */
    void writeHalfWord(uint64_t address, uint16_t value);

    /**
     * @brief Writes a 32-bit word to the given memory address.
     * @param address The memory address to write to.
     * @param value The 32-bit value to write.
     */
    void writeWord(uint64_t address, uint32_t value);

    /**
     * @brief Writes a 64-bit double word to the given memory address.
     * @param address The memory address to write to.
     * @param value The 64-bit value to write.
     */
    void writeDoubleWord(uint64_t address, uint64_t value);

    void writeFloat(uint64_t address, float value);

    void writeDouble(uint64_t address, double value);

    void printMemory(const uint64_t address, uint rows);

    void dumpMemory(const uint64_t address, uint rows);

    void printMemoryUsage() const;
};


#endif // MAIN_MEMORY_H
