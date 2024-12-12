/**
 * File Name: memory.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "memory.h"

uint8_t Memory::read(uint64_t address) {
    uint64_t block_index = getBlockIndex(address);
    uint64_t offset = getBlockOffset(address);

    if (blocks_.find(block_index) == blocks_.end()) {
        return 0;
    }

    return blocks_[block_index].data[offset];
}

void Memory::write(uint64_t address, uint8_t value) {
    uint64_t block_index = getBlockIndex(address);
    uint64_t offset = getBlockOffset(address);

    if (blocks_.find(block_index) == blocks_.end()) {
        blocks_[block_index] = MemoryBlock();
    }

    blocks_[block_index].data[offset] = value;
}

uint8_t Memory::readByte(uint64_t address) {
    return read(address);
}

uint16_t Memory::readHalfWord(uint64_t address) {
    uint16_t value = 0;
    value |= static_cast<uint16_t>(read(address));
    value |= static_cast<uint16_t>(read(address + 1)) << 8;
    return value;
}

uint32_t Memory::readWord(uint64_t address) {
    uint32_t value = 0;
    value |= static_cast<uint32_t>(read(address));
    value |= static_cast<uint32_t>(read(address + 1)) << 8;
    value |= static_cast<uint32_t>(read(address + 2)) << 16;
    value |= static_cast<uint32_t>(read(address + 3)) << 24;
    return value;
}

uint64_t Memory::readDoubleWord(uint64_t address) {
    uint64_t value = 0;
    value |= static_cast<uint64_t>(read(address));
    value |= static_cast<uint64_t>(read(address + 1)) << 8;
    value |= static_cast<uint64_t>(read(address + 2)) << 16;
    value |= static_cast<uint64_t>(read(address + 3)) << 24;
    value |= static_cast<uint64_t>(read(address + 4)) << 32;
    value |= static_cast<uint64_t>(read(address + 5)) << 40;
    value |= static_cast<uint64_t>(read(address + 6)) << 48;
    value |= static_cast<uint64_t>(read(address + 7)) << 56;
    return value;
}

void Memory::writeByte(uint64_t address, uint8_t value) {
    write(address, value);
}

void Memory::writeHalfWord(uint64_t address, uint16_t value) {
    write(address, value & 0xFF);
    write(address + 1, (value >> 8) & 0xFF);
}

void Memory::writeWord(uint64_t address, uint32_t value) {
    write(address, value & 0xFF);
    write(address + 1, (value >> 8) & 0xFF);
    write(address + 2, (value >> 16) & 0xFF);
    write(address + 3, (value >> 24) & 0xFF);
}

void Memory::writeDoubleWord(uint64_t address, uint64_t value) {
    write(address, value & 0xFF);
    write(address + 1, (value >> 8) & 0xFF);
    write(address + 2, (value >> 16) & 0xFF);
    write(address + 3, (value >> 24) & 0xFF);
    write(address + 4, (value >> 32) & 0xFF);
    write(address + 5, (value >> 40) & 0xFF);
    write(address + 6, (value >> 48) & 0xFF);
    write(address + 7, (value >> 56) & 0xFF);
}

void Memory::printMemoryUsage() const {
    std::cout << "Allocated Blocks: " << blocks_.size() << "\n";
    for (const auto& [block_index, block] : blocks_) {
        std::cout << "Block " << block_index << " allocated.\n";
    }
}







