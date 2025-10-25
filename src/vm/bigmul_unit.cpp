#include "bigmul_unit.h"

namespace bigmul_unit {

// Initialize global arrays and objects
uint64_t GPR[32] = {0};
uint64_t FPR[32] = {0};
std::unordered_map<uint64_t, uint64_t> cache;

// Helper functions with bounds checking
uint64_t readGPR(int idx) { 
    if (idx >= 0 && idx < 32) {
        return GPR[idx];
    }
    std::cerr << "Error: GPR index out of bounds: " << idx << std::endl;
    return 0;
}

void writeGPR(int idx, uint64_t val) { 
    if (idx >= 0 && idx < 32) {
        GPR[idx] = val;
    } else {
        std::cerr << "Error: GPR index out of bounds: " << idx << std::endl;
    }
}

// Cache operations
uint64_t readCache(uint64_t addr) {
    auto it = cache.find(addr);
    if (it != cache.end()) return it->second;
    return 0;
}

void writeCache(uint64_t addr, uint64_t val) {
    cache[addr] = val;
}

// Naive multiplication of two 64-bit integers as example
uint64_t bigmul(uint64_t a, uint64_t b) {
    // For very large integers, replace this with your own algorithm
    return a * b;
}

// Execute bigmul on two GPR registers and store result in rd
uint64_t executeBigMul(int rd, int rs1, int rs2) {
    uint64_t a = readGPR(rs1);
    uint64_t b = readGPR(rs2);
    uint64_t result = bigmul(a, b);
    writeGPR(rd, result);
    return result;
}

} // namespace bigmul_unit