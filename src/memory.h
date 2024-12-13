/**
 * File Name: memory.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef MEMORY_H
#define MEMORY_H

#include "pch.h"

struct MemoryBlock {
    std::vector<uint8_t> data; // 1 KB memory block
    MemoryBlock() : data(1024, 0) {}
};


class Memory {
private:
    std::unordered_map<uint64_t, MemoryBlock> blocks_;
    const int block_size_ = 1024;

    uint64_t getBlockIndex(uint64_t address) const {
        return address / block_size_;
    }

    uint64_t getBlockOffset(uint64_t address) const {
        return address % block_size_;
    }

public:
    uint8_t read(uint64_t address);

    void write(uint64_t address, uint8_t value);

    uint8_t readByte(uint64_t address);

    uint16_t readHalfWord(uint64_t address);

    uint32_t readWord(uint64_t address);

    uint64_t readDoubleWord(uint64_t address);

    void writeByte(uint64_t address, uint8_t value);

    void writeHalfWord(uint64_t address, uint16_t value);

    void writeWord(uint64_t address, uint32_t value);

    void writeDoubleWord(uint64_t address, uint64_t value);


    void printMemoryUsage() const;


};


#endif // MEMORY_H