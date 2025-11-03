#ifndef BIGMUL_UNIT_H
#define BIGMUL_UNIT_H

#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace bigmul_unit {

    uint8_t cacheA[512];
    uint8_t cacheB[512];
    uint8_t resultCache[1024];

    void loadDatafrombuffer(const std::vector<uint8_t>& bufA, const std::vector<uint8_t>& bufB);
    void loadData(uint64_t addr1, uint64_t addr2); // from LDBM
    uint64_t executeBigmul();

} // namespace bigmul_unit

#endif // BIGMUL_UNIT_H