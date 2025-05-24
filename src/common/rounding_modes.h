/**
 * File Name: rounding_modes.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef ROUNDING_MODES_H
#define ROUNDING_MODES_H

#include "../pch.h"

enum class RoundingMode {
  RNE,  // Round to Nearest, ties to Even
  RTZ,  // Round towards Zero
  RDN,  // Round Down (towards -∞)
  RUP,  // Round Up (towards +∞)
  RMM,  // Round to Nearest, ties to Max Magnitude
  DYN   // Dynamic rounding mode (in rm field, means use frm CSR)
};

// For assembler/parser to map strings to enum
inline const std::unordered_map<std::string, RoundingMode> stringToRoundingMode = {
    {"rne", RoundingMode::RNE},
    {"rtz", RoundingMode::RTZ},
    {"rdn", RoundingMode::RDN},
    {"rup", RoundingMode::RUP},
    {"rmm", RoundingMode::RMM},
    {"dyn", RoundingMode::DYN}
};

// Optional: Map enum to encoding (if VM uses integer encodings for rm field)
inline const std::unordered_map<RoundingMode, int> roundingModeEncoding = {
    {RoundingMode::RNE, 0b000},
    {RoundingMode::RTZ, 0b001},
    {RoundingMode::RDN, 0b010},
    {RoundingMode::RUP, 0b011},
    {RoundingMode::RMM, 0b100},
    {RoundingMode::DYN, 0b111}
};

inline bool isValidRoundingMode(const std::string &mode) {
  return stringToRoundingMode.find(mode)!=stringToRoundingMode.end();
}

inline int getRoundingModeEncoding(const std::string &mode) {
  if (isValidRoundingMode(mode)) {
    return roundingModeEncoding.at(stringToRoundingMode.at(mode));
  }
  throw std::invalid_argument("Invalid rounding mode: " + mode);
}

#endif // ROUNDING_MODES_H