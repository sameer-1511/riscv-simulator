/**
 * @file memory.cpp
 * @brief Contains the implementation of the Memory class.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "memory.h"

uint8_t Memory::read(uint64_t address) {
    uint64_t block_index = getBlockIndex(address);
    uint64_t offset = getBlockOffset(address);
    ensureBlockExists(block_index);
    return blocks_[block_index].data[offset];
}

void Memory::write(uint64_t address, uint8_t value) {
    uint64_t block_index = getBlockIndex(address);
    uint64_t offset = getBlockOffset(address);
    ensureBlockExists(block_index);
    blocks_[block_index].data[offset] = value;
}

uint64_t Memory::getBlockIndex(uint64_t address) const {
    return address / block_size_;
}

uint64_t Memory::getBlockOffset(uint64_t address) const {
    return address % block_size_;
}

void Memory::ensureBlockExists(uint64_t block_index) {
    if (blocks_.find(block_index) == blocks_.end()) {
        blocks_.emplace(block_index, MemoryBlock());
    }
}

template <typename T>
T Memory::readGeneric(uint64_t address) {
    T value = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        value |= static_cast<T>(read(address + i)) << (8 * i);
    }
    return value;
}

template <typename T>
void Memory::writeGeneric(uint64_t address, T value) {
    for (size_t i = 0; i < sizeof(T); ++i) {
        write(address + i, static_cast<uint8_t>(value >> (8 * i)));
    }
}

uint8_t Memory::readByte(uint64_t address) {
    return read(address);
}

uint16_t Memory::readHalfWord(uint64_t address) {
    return readGeneric<uint16_t>(address);
}

uint32_t Memory::readWord(uint64_t address) {
    return readGeneric<uint32_t>(address);
}

uint64_t Memory::readDoubleWord(uint64_t address) {
    return readGeneric<uint64_t>(address);
}

void Memory::writeByte(uint64_t address, uint8_t value) {
    write(address, value);
}

void Memory::writeHalfWord(uint64_t address, uint16_t value) {
    writeGeneric<uint16_t>(address, value);
}

void Memory::writeWord(uint64_t address, uint32_t value) {
    writeGeneric<uint32_t>(address, value);
}

void Memory::writeDoubleWord(uint64_t address, uint64_t value) {
    writeGeneric<uint32_t>(address, value);
}

void Memory::printMemoryUsage() const {
    std::cout << "Memory Usage Report:\n";
        std::cout << "---------------------\n";
        std::cout << "Block Count: " << blocks_.size() << "\n";
        for (const auto& [block_index, block] : blocks_) {
            size_t used_bytes = std::count_if(block.data.begin(), block.data.end(),
                                              [](uint8_t byte) { return byte != 0; });
            if (used_bytes > 0) {
                std::cout << "Block " << block_index << ": " << used_bytes
                          << " / " << block_size_ << " bytes used\n";
            }
        }

}







