#ifndef BIGMUL_UNIT_H
#define BIGMUL_UNIT_H

#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace bigmul_unit {

// External declarations (will be defined in the .cpp file)
extern uint64_t GPR[32];
extern uint64_t FPR[32];
extern std::unordered_map<uint64_t, uint64_t> cache;

// Helper functions
uint64_t readGPR(int idx);
void writeGPR(int idx, uint64_t val);
uint64_t readCache(uint64_t addr);
void writeCache(uint64_t addr, uint64_t val);
uint64_t bigmul(uint64_t a, uint64_t b);
uint64_t executeBigMul(int rd, int rs1, int rs2);

} // namespace bigmul_unit

#endif // BIGMUL_UNIT_H