/**
 * @file alu.h
 * @brief Contains the definition of the ALU class for performing arithmetic and logic operations.
 * @author Vishank Singh, httpa://github.com/VishankSingh
 */
#ifndef ALU_H
#define ALU_H

#include "../pch.h"

namespace ALU {
enum class ALUOp {
    NONE, ///< No operation.
    ADD, ///< Addition operation.
    ADDW, ///< Addition word operation.
    SUB, ///< Subtraction operation.
    SUBW, ///< Subtraction word operation.
    MUL, ///< Multiplication operation.
    MULH, ///< Multiplication high operation.
    MULHSU, ///< Multiplication high signed and unsigned operation.
    MULHU, ///< Multiplication high unsigned operation.
    MULW, ///< Multiplication word operation.
    DIV, ///< Division operation.
    DIVW, ///< Division word operation.
    DIVU, ///< Unsigned division operation.
    DIVUW, ///< Unsigned division word operation.
    REM, ///< Remainder operation.
    REMW, ///< Remainder word operation.
    REMU, ///< Unsigned remainder operation.
    REMUW, ///< Unsigned remainder word operation.
    AND, ///< Bitwise AND operation.
    OR, ///< Bitwise OR operation.
    XOR, ///< Bitwise XOR operation.
    SLL, ///< Shift left logical operation.
    SLLW, ///< Shift left logical word operation.
    SRL, ///< Shift right logical operation.
    SRLW, ///< Shift right logical word operation.
    SRA, ///< Shift right arithmetic operation.
    SRAW, ///< Shift right arithmetic word operation.
    SLT, ///< Set less than operation.
    SLTU, ///< Unsigned set less than operation.
};

inline std::ostream& operator<<(std::ostream& os, const ALUOp& op) {
    switch (op) {
        case ALUOp::NONE: os << "NONE"; break;
        case ALUOp::ADD: os << "ADD"; break;
        case ALUOp::SUB: os << "SUB"; break;
        case ALUOp::MUL: os << "MUL"; break;
        case ALUOp::DIV: os << "DIV"; break;
        case ALUOp::DIVU: os << "DIVU"; break;
        case ALUOp::REM: os << "REM"; break;
        case ALUOp::REMU: os << "REMU"; break;
        case ALUOp::AND: os << "AND"; break;
        case ALUOp::OR: os << "OR"; break;
        case ALUOp::XOR: os << "XOR"; break;
        case ALUOp::SLL: os << "SLL"; break;
        case ALUOp::SRL: os << "SRL"; break;
        case ALUOp::SRA: os << "SRA"; break;
        case ALUOp::SLT: os << "SLT"; break;
        case ALUOp::SLTU: os << "SLTU"; break;
        default: os << "UNKNOWN"; break;
    }
    return os;
}
/**
 * @brief The ALU class is responsible for performing arithmetic and logic operations.
 */
class ALU {
public:
    bool carry_ = false; ///< Carry flag.
    bool zero_ = false; ///< Zero flag.
    bool negative_ = false; ///< Negative flag.
    bool overflow_ = false; ///< Overflow flag.

    ALU() = default;
    ~ALU() = default;

    /**
     * @brief Executes the given ALU operation.
     * @tparam T Integer type (int32_t, uint32_t, etc.).
     * @param op The ALU operation.
     * @param a First operand.
     * @param b Second operand.
     * @return A pair (result, overflow_flag).
     */
    constexpr std::pair<uint64_t, bool> execute(ALUOp op, uint64_t a, uint64_t b) const {
        switch (op) {
            case ALUOp::ADD: {
                int64_t sa = static_cast<int64_t>(a);
                int64_t sb = static_cast<int64_t>(b);
                int64_t result = sa + sb;
                bool overflow = __builtin_add_overflow(sa, sb, &result);
                return {static_cast<uint64_t>(result), overflow};
            }
            case ALUOp::ADDW:{
                int32_t sa = static_cast<int32_t>(a);
                int32_t sb = static_cast<int32_t>(b);
                int32_t result = sa + sb;
                bool overflow = __builtin_add_overflow(sa, sb, &result);
                return {static_cast<uint64_t>(static_cast<int32_t>(result)), overflow};
            }
            case ALUOp::SUB: {
                int64_t sa = static_cast<int64_t>(a);
                int64_t sb = static_cast<int64_t>(b);
                int64_t result = sa - sb;
                bool overflow = __builtin_sub_overflow(sa, sb, &result);
                return {static_cast<uint64_t>(result), overflow};
            }
            case ALUOp::SUBW: {
                int32_t sa = static_cast<int32_t>(a);
                int32_t sb = static_cast<int32_t>(b);
                int32_t result = sa - sb;
                bool overflow = __builtin_sub_overflow(sa, sb, &result);
                return {static_cast<uint64_t>(static_cast<int32_t>(result)), overflow};
            }
            case ALUOp::MUL: {
                int64_t sa = static_cast<int64_t>(a);
                int64_t sb = static_cast<int64_t>(b);
                int64_t result = sa * sb;
                bool overflow = __builtin_mul_overflow(sa, sb, &result);
                std::cout << "MUL: " << result << std::endl;
                return {static_cast<uint64_t>(result), overflow};
            }
            case ALUOp::MULH: {
                int64_t sa = static_cast<int64_t>(a);
                int64_t sb = static_cast<int64_t>(b);
                // TODO: do something about this, msvc doesnt support __int128

                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Wpedantic"
                __int128 result = static_cast<__int128>(sa) * static_cast<__int128>(sb); 
                int64_t high_result = static_cast<int64_t>(result >> 64);
                #pragma GCC diagnostic pop

                return {static_cast<uint64_t>(high_result), false};
            }
            case ALUOp::MULHSU: {
                int64_t sa = static_cast<int64_t>(a);
                uint64_t sb = static_cast<uint64_t>(b); 

                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Wpedantic"
                __int128 result = static_cast<__int128>(sa) * static_cast<__int128>(sb);
                int64_t high_result = static_cast<int64_t>(result >> 64);
                #pragma GCC diagnostic pop

                return {static_cast<uint64_t>(high_result), false};
            }
            case ALUOp::MULHU: {
                uint64_t ua = static_cast<uint64_t>(a);
                uint64_t ub = static_cast<uint64_t>(b);

                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Wpedantic"
                __int128 result = static_cast<__int128>(ua) * static_cast<__int128>(ub);
                int64_t high_result = static_cast<int64_t>(result >> 64);
                #pragma GCC diagnostic pop

                return {static_cast<uint64_t>(high_result), false};
            }
            case ALUOp::MULW: {
                int32_t sa = static_cast<int32_t>(a);
                int32_t sb = static_cast<int32_t>(b);
                int64_t result = static_cast<int64_t>(sa) * static_cast<int64_t>(sb);
                int32_t lower_result = static_cast<int32_t>(result);
                bool overflow = (result != static_cast<int64_t>(static_cast<int32_t>(result)));
                return {static_cast<uint64_t>(lower_result), overflow};
            }
            case ALUOp::DIV: {
                int64_t sa = static_cast<int64_t>(a);
                int64_t sb = static_cast<int64_t>(b);
                if (sb == 0) {
                    return {0, false};
                }
                if (sa == INT64_MIN && sb == -1) {
                    return {static_cast<uint64_t>(INT64_MAX), true};
                }
                int64_t result = sa / sb;
                return {static_cast<uint64_t>(result), false};
            }
            case ALUOp::DIVW: {
                int32_t sa = static_cast<int32_t>(a);
                int32_t sb = static_cast<int32_t>(b);
                if (sb == 0) {
                    return {0, false};
                }
                if (sa == INT32_MIN && sb == -1) {
                    return {static_cast<uint64_t>(INT32_MIN), true};
                }
                int32_t result = sa / sb;
                return {static_cast<uint64_t>(result), false};
            }
            case ALUOp::DIVU: {
                if (b == 0) {
                    return {0, false};
                }
                uint64_t result = a / b;
                return {result, false};
            }
            case ALUOp::DIVUW:{
                if (b == 0) {
                    return {0, false};
                }
                uint64_t result = static_cast<uint32_t>(a) / static_cast<uint32_t>(b);
                return {static_cast<uint64_t>(result), false};
            }
            case ALUOp::REM: {
                if (b == 0) {
                    return {0, false};
                }
                int64_t result = static_cast<int64_t>(a) % static_cast<int64_t>(b);
                return {static_cast<uint64_t>(result), false};
            }
            case ALUOp::REMW: {
                if (b == 0) {
                    return {0, false};
                }
                int32_t result = static_cast<int32_t>(a) % static_cast<int32_t>(b);
                return {static_cast<uint64_t>(result), false};
            }
            case ALUOp::REMU:{
                if (b == 0) {
                    return {0, false};
                }
                uint64_t result = a % b;
                return {result, false};
            }
            case ALUOp::REMUW:{
                if (b == 0) {
                    return {0, false};
                }
                uint64_t result = static_cast<uint32_t>(a) % static_cast<uint32_t>(b);
                return {static_cast<uint64_t>(result), false};
            }
            case ALUOp::AND: {
                return {static_cast<uint64_t>(a & b), false};
            }
            case ALUOp::OR:  {
                return {static_cast<uint64_t>(a | b), false};
            }
            case ALUOp::XOR: {
                return {static_cast<uint64_t>(a ^ b), false};
            }
            case ALUOp::SLL: {
                uint64_t result = a << (b & 63);
                return {result, false};
            }
            case ALUOp::SLLW: {
                uint32_t sa = static_cast<uint32_t>(a);
                uint32_t sb = static_cast<uint32_t>(b);
                uint32_t result = sa << (sb & 31);
                return {static_cast<uint64_t>(static_cast<int32_t>(result)), false};
            }
            case ALUOp::SRL: {
                uint64_t result = a >> (b & 63);
                return {result, false};
            }
            case ALUOp::SRLW: {
                uint32_t sa = static_cast<uint32_t>(a);
                uint32_t sb = static_cast<uint32_t>(b);
                uint32_t result = sa >> (sb & 31);
                return {static_cast<uint64_t>(static_cast<int32_t>(result)), false};
            }
            case ALUOp::SRA: {
                int64_t sa = static_cast<int64_t>(a);
                return {static_cast<uint64_t>(sa >> (b & 63)), false};
            }
            case ALUOp::SRAW: {
                int32_t sa = static_cast<int32_t>(a);
                return {static_cast<uint64_t>(sa >> (b & 31)), false};
            }
            case ALUOp::SLT: {
                int64_t sa = static_cast<int64_t>(a);
                int64_t sb = static_cast<int64_t>(b);
                return {static_cast<uint64_t>(sa < sb), false};
            }
            case ALUOp::SLTU: {
                return {static_cast<uint64_t>(a < b), false};
            }
            default: return {0, false}; 
        }
    }

    

    void setFlags(bool carry, bool zero, bool negative, bool overflow) {
        carry_ = carry;
        zero_ = zero;
        negative_ = negative;
        overflow_ = overflow;
    }
    

};
}

#endif // ALU_H