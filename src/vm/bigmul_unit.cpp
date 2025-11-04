#include "vm/bigmul_unit.h"
#include <cstring>
#include <algorithm>

namespace bigmul_unit {

// Define the arrays that were declared as extern in the header
uint8_t cacheA[512];
uint8_t cacheB[512];
uint8_t resultCache[1024];

void loadDatafrombuffer(const std::vector<uint8_t>& bufA, const std::vector<uint8_t>& bufB) {
    // Copy data from buffers to caches, ensuring we don't overflow
    size_t sizeA = std::min(bufA.size(), sizeof(cacheA));
    size_t sizeB = std::min(bufB.size(), sizeof(cacheB));
    
    std::memcpy(cacheA, bufA.data(), sizeA);
    std::memcpy(cacheB, bufB.data(), sizeB);
    
    // Zero out remaining bytes if any
    if (sizeA < sizeof(cacheA)) {
        std::memset(cacheA + sizeA, 0, sizeof(cacheA) - sizeA);
    }
    if (sizeB < sizeof(cacheB)) {
        std::memset(cacheB + sizeB, 0, sizeof(cacheB) - sizeB);
    }
}

void loadData(uint64_t addr1, uint64_t addr2) {
    (void) addr1;
    (void) addr2;
    // This function would need access to memory system
    // For now, just zero out the caches as placeholder
    std::memset(cacheA, 0, sizeof(cacheA));
    std::memset(cacheB, 0, sizeof(cacheB));
}

std::size_t executeBigmul() {
    // Simple schoolbook multiplication algorithm for arbitrarily large numbers
    std::memset(resultCache, 0, sizeof(resultCache));
    
    // Find the actual sizes by looking for last non-zero byte
    size_t sizeA = sizeof(cacheA);
    while (sizeA > 0 && cacheA[sizeA - 1] == 0) --sizeA;
    
    size_t sizeB = sizeof(cacheB);
    while (sizeB > 0 && cacheB[sizeB - 1] == 0) --sizeB;
    
    // If either number is zero, result is zero
    if (sizeA == 0 || sizeB == 0) {
        return 0;
    }
    
    // Perform multiplication
    for (size_t i = 0; i < sizeA; i++) {
        uint16_t carry = 0;
        for (size_t j = 0; j < sizeB; j++) {
            uint32_t product = static_cast<uint32_t>(cacheA[i]) * static_cast<uint32_t>(cacheB[j]);
            uint32_t sum = product + carry + resultCache[i + j];
            resultCache[i + j] = sum & 0xFF;
            carry = sum >> 8;
        }
        if (carry && i + sizeB < sizeof(resultCache)) {
            resultCache[i + sizeB] = carry;
        }
    }
    
    // Calculate result size
    size_t resultSize = sizeA + sizeB;
    while (resultSize > 0 && resultCache[resultSize - 1] == 0) --resultSize;
    
    return resultSize;
}

std::size_t getResultSize() {
    // Find the first non-zero byte from the end
    size_t size = sizeof(resultCache);
    while (size > 0 && resultCache[size - 1] == 0) {
        --size;
    }
    return size;
}

void invalidateCaches() {
    // Clear all caches
    std::memset(cacheA, 0, sizeof(cacheA));
    std::memset(cacheB, 0, sizeof(cacheB));
    std::memset(resultCache, 0, sizeof(resultCache));
}

} // namespace bigmul_unit