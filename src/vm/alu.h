/**
 * @file alu.h
 * @brief Contains the definition of the ALU class for performing arithmetic and logic operations.
 * @author Vishank Singh, httpa://github.com/VishankSingh
 */
#ifndef ALU_H
#define ALU_H

#include "../pch.h"
#include <cfenv>
#include <cmath>

// #pragma float_control(precise, on)
// #pragma STDC FENV_ACCESS ON

#pragma GCC diagnostic ignored "-Wstrict-aliasing"

#pragma GCC optimize ("no-fast-math") 


#define FCSR_INVALID_OP   (1 << 0)  // Invalid operation
#define FCSR_DIV_BY_ZERO  (1 << 1)  // Divide by zero
#define FCSR_OVERFLOW     (1 << 2)  // Overflow
#define FCSR_UNDERFLOW    (1 << 3)  // Underflow
#define FCSR_INEXACT      (1 << 4)  // Inexact result

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

    // Floating point operations
    FMADD_S, ///< Floating point multiply-add single operation.
    FMSUB_S, ///< Floating point multiply-subtract single operation.
    FNMADD_S, ///< Floating point negative multiply-add single operation.
    FNMSUB_S, ///< Floating point negative multiply-subtract single operation.

    FADD_S, ///< Floating point addition operation.
    FSUB_S, ///< Floating point subtraction operation.
    FMUL_S, ///< Floating point multiplication operation.
    FDIV_S, ///< Floating point division operation.
    FSQRT_S, ///< Floating point square root operation.
    FSGNJ_S, ///< Floating point sign inject operation.
    FSGNJN_S, ///< Floating point sign inject negative operation.
    FSGNJX_S, ///< Floating point sign inject XOR operation.
    FMIN_S, ///< Floating point minimum operation.
    FMAX_S, ///< Floating point maximum operation.
    FEQ_S, ///< Floating point equal operation.
    FLT_S, ///< Floating point less than operation.
    FLE_S, ///< Floating point less than or equal operation.
    FCLASS_S, ///< Floating point class operation.

    FCVT_W_S, ///< Floating point convert to word operation.
    FCVT_WU_S, ///< Floating point convert to unsigned word operation.
    FCVT_L_S, ///< Floating point convert double to long operation.
    FCVT_LU_S, ///< Floating point convert double to unsigned long operation.
    
    FCVT_S_W, ///< Floating point convert word to operation.
    FCVT_S_WU, ///< Floating point convert unsigned word to operation.
    FCVT_S_L, ///< Floating point convert long to single operation.
    FCVT_S_LU, ///< Floating point convert unsigned long to single operation.

    FMV_X_W, ///< Floating point move to integer operation.
    FMV_W_X, ///< Floating point move from integer operation.

    FMADD_D, ///< Floating point multiply-add double operation.
    FMSUB_D, ///< Floating point multiply-subtract double operation.
    FNMADD_D, ///< Floating point negative multiply-add double operation.
    FNMSUB_D, ///< Floating point negative multiply-subtract double operation.

    FADD_D, ///< Floating point addition double operation.
    FSUB_D, ///< Floating point subtraction double operation.
    FMUL_D, ///< Floating point multiplication double operation.
    FDIV_D, ///< Floating point division double operation.
    FSQRT_D, ///< Floating point square root double operation.
    FSGNJ_D, ///< Floating point sign inject double operation.
    FSGNJN_D, ///< Floating point sign inject negative double operation.
    FSGNJX_D, ///< Floating point sign inject XOR double operation.
    FMIN_D, ///< Floating point minimum double operation.
    FMAX_D, ///< Floating point maximum double operation.
    FEQ_D, ///< Floating point equal double operation.
    FLT_D, ///< Floating point less than double operation.
    FLE_D, ///< Floating point less than or equal double operation.
    FCLASS_D, ///< Floating point class double operation.

    FCVT_W_D, ///< Floating point convert double to word operation.
    FCVT_WU_D, ///< Floating point convert double to unsigned word operation.
    FCVT_L_D, ///< Floating point convert double to long operation.
    FCVT_LU_D, ///< Floating point convert double to unsigned long operation.

    FCVT_D_W, ///< Floating point convert word to double operation.
    FCVT_D_WU, ///< Floating point convert unsigned word to double operation.
    FCVT_D_L, ///< Floating point convert long to double operation.
    FCVT_D_LU, ///< Floating point convert unsigned long to double operation.

    FCVT_S_D, ///< Floating point convert double to single operation.
    FCVT_D_S, ///< Floating point convert single to double operation.

    FMV_D_X, ///< Floating point move to integer double operation.
    FMV_X_D, ///< Floating point move from integer double operation.
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
        case ALUOp::ADDW: os << "ADDW"; break;
        case ALUOp::SUBW: os << "SUBW"; break;
        case ALUOp::MULW: os << "MULW"; break;
        case ALUOp::DIVW: os << "DIVW"; break;
        case ALUOp::DIVUW: os << "DIVUW"; break;
        case ALUOp::REMW: os << "REMW"; break;
        case ALUOp::REMUW: os << "REMUW"; break;
        case ALUOp::MULH: os << "MULH"; break;
        case ALUOp::MULHSU: os << "MULHSU"; break;
        case ALUOp::MULHU: os << "MULHU"; break;
        case ALUOp::SLLW: os << "SLLW"; break;
        case ALUOp::SRLW: os << "SRLW"; break;
        case ALUOp::SRAW: os << "SRAW"; break;
        case ALUOp::FMADD_S: os << "FMADD_S"; break;
        case ALUOp::FMSUB_S: os << "FMSUB_S"; break;
        case ALUOp::FNMADD_S: os << "FNMADD_S"; break;
        case ALUOp::FNMSUB_S: os << "FNMSUB_S"; break;
        case ALUOp::FADD_S: os << "FADD_S"; break;
        case ALUOp::FSUB_S: os << "FSUB_S"; break;
        case ALUOp::FMUL_S: os << "FMUL_S"; break;
        case ALUOp::FDIV_S: os << "FDIV_S"; break;
        case ALUOp::FSQRT_S: os << "FSQRT_S"; break;
        case ALUOp::FSGNJ_S: os << "FSGNJ_S"; break;
        case ALUOp::FSGNJN_S: os << "FSGNJN_S"; break;
        case ALUOp::FSGNJX_S: os << "FSGNJX_S"; break;
        case ALUOp::FMIN_S: os << "FMIN_S"; break;
        case ALUOp::FMAX_S: os << "FMAX_S"; break;
        case ALUOp::FEQ_S: os << "FEQ_S"; break;
        case ALUOp::FLT_S: os << "FLT_S"; break;
        case ALUOp::FLE_S: os << "FLE_S"; break;
        case ALUOp::FCLASS_S: os << "FCLASS_S"; break;
        case ALUOp::FCVT_W_S: os << "FCVT_W_S"; break;
        case ALUOp::FCVT_WU_S: os << "FCVT_WU_S"; break;
        case ALUOp::FCVT_L_S: os << "FCVT_L_S"; break;
        case ALUOp::FCVT_LU_S: os << "FCVT_LU_S"; break;
        case ALUOp::FCVT_S_W: os << "FCVT_S_W"; break;
        case ALUOp::FCVT_S_WU: os << "FCVT_S_WU"; break;
        case ALUOp::FCVT_S_L: os << "FCVT_S_L"; break;
        case ALUOp::FCVT_S_LU: os << "FCVT_S_LU"; break;
        case ALUOp::FMADD_D: os << "FMADD_D"; break;
        case ALUOp::FMSUB_D: os << "FMSUB_D"; break;
        case ALUOp::FNMADD_D: os << "FNMADD_D"; break;
        case ALUOp::FNMSUB_D: os << "FNMSUB_D"; break;
        case ALUOp::FADD_D: os << "FADD_D"; break;
        case ALUOp::FSUB_D: os << "FSUB_D"; break;
        case ALUOp::FMUL_D: os << "FMUL_D"; break;
        case ALUOp::FDIV_D: os << "FDIV_D"; break;
        case ALUOp::FSQRT_D: os << "FSQRT_D"; break;
        case ALUOp::FSGNJ_D: os << "FSGNJ_D"; break;


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

    // TODO: check all the floating point operations

    [[nodiscard]] std::pair<uint64_t, uint8_t> fpexecute(ALUOp op, uint64_t ina, uint64_t inb, uint64_t inc, uint8_t rm) const {
        float a, b, c;
        std::memcpy(&a, &ina, sizeof(float));
        std::memcpy(&b, &inb, sizeof(float));
        std::memcpy(&c, &inc, sizeof(float));
        float result = 0.0;

        uint8_t fcsr = 0;

        int original_rm = std::fegetround();

        switch (rm) {
        case 0b000: std::fesetround(FE_TONEAREST); break;  // RNE
        case 0b001: std::fesetround(FE_TOWARDZERO); break; // RTZ
        case 0b010: std::fesetround(FE_DOWNWARD); break;   // RDN
        case 0b011: std::fesetround(FE_UPWARD); break;     // RUP
        // 0b100 RMM, unsupported 
        default: break; 
        }

        std::feclearexcept(FE_ALL_EXCEPT);
        
        switch (op) {
        case ALUOp::ADD: {
            int64_t sa = static_cast<int64_t>(ina);
            int64_t sb = static_cast<int64_t>(inb);
            int64_t result = sa + sb;
            // bool overflow = __builtin_add_overflow(sa, sb, &result); // TODO: check this
            return {static_cast<uint64_t>(result), 0};
        }
        case ALUOp::FMADD_S: {
            result = std::fma(a, b, c);
            break;
        }
        case ALUOp::FMSUB_S: {
            result = std::fma(a, b, -c);
            break;
        }
        case ALUOp::FNMADD_S: {
            result = std::fma(-a, b, -c);
            break;
        }
        case ALUOp::FNMSUB_S: {
            result = std::fma(-a, b, c);
            break;
        }
        case ALUOp::FSQRT_S: {
            if (a < 0.0f) {
                result = std::numeric_limits<float>::quiet_NaN();
                fcsr |= FCSR_INVALID_OP;
            } else {
                result = std::sqrt(a);
            }
            break;
        }
        case ALUOp::FCVT_W_S: {
            if (!std::isfinite(a) || a > static_cast<float>(INT32_MAX) || a < static_cast<float>(INT32_MIN)) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                int64_t res = static_cast<int64_t>(static_cast<int32_t>(a > 0 ? INT32_MAX : INT32_MIN));
                return {static_cast<uint64_t>(res), fcsr};
            } else {
                int32_t ires = static_cast<int32_t>(std::nearbyint(a));
                int64_t res = static_cast<int64_t>(ires); // sign-extend
                fesetround(original_rm);
                return {static_cast<uint64_t>(res), fcsr};
            }
            break;
        }
        case ALUOp::FCVT_WU_S: {
            if (!std::isfinite(a) || a > static_cast<float>(UINT32_MAX) || a < 0.0f) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);  
                uint32_t saturate = (a < 0.0f) ? 0 : UINT32_MAX;
                int64_t res = static_cast<int64_t>(static_cast<int32_t>(saturate)); // sign-extend
                return {static_cast<uint64_t>(res), fcsr};
            } else {
                uint32_t ires = static_cast<uint32_t>(std::nearbyint(a));
                int64_t res = static_cast<int64_t>(static_cast<int32_t>(ires)); // sign-extend
                fesetround(original_rm);
                return {static_cast<uint64_t>(res), fcsr};
            }
            break;
        }
        case ALUOp::FCVT_L_S: {
            if (!std::isfinite(a) || a > static_cast<float>(INT64_MAX) || a < static_cast<float>(INT64_MIN)) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                int64_t saturate = (a < 0.0f) ? INT64_MIN : INT64_MAX;
                return {static_cast<uint64_t>(saturate), fcsr};
            } else {
                int64_t ires = static_cast<int64_t>(std::nearbyint(a));
                fesetround(original_rm);
                return {static_cast<uint64_t>(ires), fcsr};
            }
            break;
        }
        case ALUOp::FCVT_LU_S: {
            if (!std::isfinite(a) || a > static_cast<float>(UINT64_MAX) || a < 0.0f) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                uint64_t saturate = (a < 0.0f) ? 0 : UINT64_MAX;
                return {saturate, fcsr};
            } else {
                uint64_t ires = static_cast<uint64_t>(std::nearbyint(a));
                fesetround(original_rm);
                return {ires, fcsr};
            }
            break;
        }
        case ALUOp::FCVT_S_W: {
            int32_t ia = static_cast<int32_t>(ina);
            result = static_cast<float>(ia);
            break;

        }
        case ALUOp::FCVT_S_WU: {
            uint32_t ua = static_cast<uint32_t>(ina);
            result = static_cast<float>(ua);
            break;
        }
        case ALUOp::FCVT_S_L: {
            int64_t la = static_cast<int64_t>(ina);
            result = static_cast<float>(la);
            break;

        }
        case ALUOp::FCVT_S_LU: {
            uint64_t ula = static_cast<uint64_t>(ina);
            result = static_cast<float>(ula);
            break;
        }
        case ALUOp::FSGNJ_S: {
            uint32_t a_bits = static_cast<uint32_t>(ina);
            uint32_t b_bits = static_cast<uint32_t>(inb);
            uint32_t temp = (a_bits & 0x7FFFFFFF) | (b_bits & 0x80000000);
            std::memcpy(&result, &temp, sizeof(float));
            break;
        }
        case ALUOp::FSGNJN_S: {
            uint32_t a_bits = static_cast<uint32_t>(ina);
            uint32_t b_bits = static_cast<uint32_t>(inb);
            uint32_t temp = (a_bits & 0x7FFFFFFF) | (~b_bits & 0x80000000);
            std::memcpy(&result, &temp, sizeof(float));
            break;
        }
        case ALUOp::FSGNJX_S: {
            uint32_t a_bits = static_cast<uint32_t>(ina);
            uint32_t b_bits = static_cast<uint32_t>(inb);
            uint32_t temp = (a_bits & 0x7FFFFFFF) | ((a_bits ^ b_bits) & 0x80000000);
            std::memcpy(&result, &temp, sizeof(float));
            break;
        }
        case ALUOp::FMIN_S: {
            if (std::isnan(a) && !std::isnan(b)) {
                result = b;
            } else if (!std::isnan(a) && std::isnan(b)) {
                result = a;
            } else if (std::signbit(a) != std::signbit(b) && a == b) {
                result = -0.0f; // Both zero but with different signs — return -0.0
            } else {
                result = std::fmin(a, b);
            }
            break;
        }
        case ALUOp::FMAX_S: {
            if (std::isnan(a) && !std::isnan(b)) {
                result = b;
            } else if (!std::isnan(a) && std::isnan(b)) {
                result = a;
            } else if (std::signbit(a) != std::signbit(b) && a == b) {
                result = 0.0f; // Both zero but with different signs — return +0.0
            } else {
                result = std::fmax(a, b);
            }
            break;
        }
        case ALUOp::FEQ_S: {
            if (std::isnan(a) || std::isnan(b)) {
                result = 0.0f;
            } else {
                result = (a == b) ? 1.0f : 0.0f;
            }
            break;
        }
        case ALUOp::FLT_S: {
            if (std::isnan(a) || std::isnan(b)) {
                result = 0.0f;
            } else {
                result = (a < b) ? 1.0f : 0.0f;
            }
            break;
        }
        case ALUOp::FLE_S: {
            if (std::isnan(a) || std::isnan(b)) {
                result = 0.0f;
            } else {
                result = (a <= b) ? 1.0f : 0.0f;
            }
            break;
        }
        case ALUOp::FCLASS_S: {
            uint32_t a_bits = static_cast<uint32_t>(ina);
            float af;
            std::memcpy(&af, &a_bits, sizeof(float));
            uint16_t res = 0;

            if (std::signbit(af) && std::isinf(af)) res |= 1 << 0; // -inf
            else if (std::signbit(af) && std::fpclassify(af) == FP_NORMAL) res |= 1 << 1; // -normal
            else if (std::signbit(af) && std::fpclassify(af) == FP_SUBNORMAL) res |= 1 << 2; // -subnormal
            else if (std::signbit(af) && af == 0.0f) res |= 1 << 3; // -zero
            else if (!std::signbit(af) && af == 0.0f) res |= 1 << 4; // +zero
            else if (!std::signbit(af) && std::fpclassify(af) == FP_SUBNORMAL) res |= 1 << 5; // +subnormal
            else if (!std::signbit(af) && std::fpclassify(af) == FP_NORMAL) res |= 1 << 6; // +normal
            else if (!std::signbit(af) && std::isinf(af)) res |= 1 << 7; // +inf
            else if (std::isnan(af) && (a_bits & 0x00400000) == 0) res |= 1 << 8; // signaling NaN
            else if (std::isnan(af)) res |= 1 << 9; // quiet NaN

            std::fesetround(original_rm);
            return {res, fcsr};
        }
        case ALUOp::FMV_X_W: {
            int32_t float_bits;
            std::memcpy(&float_bits, &ina, sizeof(float));
            int64_t sign_extended = static_cast<int64_t>(float_bits);
            return {static_cast<uint64_t>(sign_extended), fcsr};
        }
        case ALUOp::FMV_W_X: {
            uint32_t int_bits = static_cast<uint32_t>(ina & 0xFFFFFFFF);
            std::memcpy(&result, &int_bits, sizeof(float));
            break;
        }
        default: break;
        }

        int raised = std::fetestexcept(FE_ALL_EXCEPT);
        if (raised & FE_INVALID)   fcsr |= FCSR_INVALID_OP;
        if (raised & FE_DIVBYZERO) fcsr |= FCSR_DIV_BY_ZERO;
        if (raised & FE_OVERFLOW)  fcsr |= FCSR_OVERFLOW;
        if (raised & FE_UNDERFLOW) fcsr |= FCSR_UNDERFLOW;
        if (raised & FE_INEXACT)   fcsr |= FCSR_INEXACT;

        std::fesetround(original_rm);

        uint32_t result_bits = 0;
        std::memcpy(&result_bits, &result, sizeof(result));
        return {static_cast<uint64_t>(result_bits), fcsr};
    }

    [[nodiscard]] std::pair<uint64_t, bool> dfpexecute(ALUOp op, uint64_t ina, uint64_t inb, uint64_t inc, uint8_t rm) const {
        double a, b, c;
        std::memcpy(&a, &ina, sizeof(double));
        std::memcpy(&b, &inb, sizeof(double));
        std::memcpy(&c, &inc, sizeof(double));
        double result = 0.0;

        uint8_t fcsr = 0;

        int original_rm = std::fegetround();

        switch (rm) {
        case 0b000: std::fesetround(FE_TONEAREST); break;  // RNE
        case 0b001: std::fesetround(FE_TOWARDZERO); break; // RTZ
        case 0b010: std::fesetround(FE_DOWNWARD); break;   // RDN
        case 0b011: std::fesetround(FE_UPWARD); break;     // RUP
        // 0b100 RMM, unsupported 
        default: break; 
        }

        std::feclearexcept(FE_ALL_EXCEPT);

        
        switch (op) {
        case ALUOp::ADD: {
            int64_t sa = static_cast<int64_t>(ina);
            int64_t sb = static_cast<int64_t>(inb);
            int64_t result = sa + sb;
            // bool overflow = __builtin_add_overflow(sa, sb, &result); // TODO: check this
            return {static_cast<uint64_t>(result), 0};
        }
        case ALUOp::FMADD_D: {
            result = std::fma(a, b, c);
            break;
        }
        case ALUOp::FMSUB_D: {
            result = std::fma(a, b, -c);
            break;
        }
        case ALUOp::FNMADD_D: {
            result = std::fma(-a, b, -c);
            break;
        }
        case ALUOp::FNMSUB_D: {
            result = std::fma(-a, b, c);
            break;
        }
        case ALUOp::FSQRT_D: {
            if (a < 0.0) {
                result = std::numeric_limits<double>::quiet_NaN();
                fcsr |= FCSR_INVALID_OP;
            } else {
                result = std::sqrt(a);
            }
            break;
        }
        case ALUOp::FCVT_W_D: {
            if (!std::isfinite(a) || a > static_cast<double>(INT32_MAX) || a < static_cast<double>(INT32_MIN)) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                int32_t saturate = (a < 0.0) ? INT32_MIN : INT32_MAX;
                int64_t res = static_cast<int64_t>(saturate); // sign-extend to XLEN
                return {static_cast<uint64_t>(res), fcsr};
            } else {
                int32_t ires = static_cast<int32_t>(std::nearbyint(a));
                int64_t res = static_cast<int64_t>(ires); // sign-extend to XLEN
                fesetround(original_rm);
                return {static_cast<uint64_t>(res), fcsr};
            }
            break;
        }
        case ALUOp::FCVT_WU_D: {
            if (!std::isfinite(a) || a > static_cast<double>(UINT32_MAX) || a < 0.0) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                uint32_t saturate = (a < 0.0) ? 0 : UINT32_MAX;
                int64_t res = static_cast<int64_t>(static_cast<int32_t>(saturate)); // sign-extend per spec
                return {static_cast<uint64_t>(res), fcsr};
            } else {
                uint32_t ires = static_cast<uint32_t>(std::nearbyint(a));
                int64_t res = static_cast<int64_t>(static_cast<int32_t>(ires)); // sign-extend
                fesetround(original_rm);
                return {static_cast<uint64_t>(res), fcsr};
            }
            break;
        }
        case ALUOp::FCVT_L_D: {
            if (!std::isfinite(a) || a > static_cast<double>(INT64_MAX) || a < static_cast<double>(INT64_MIN)) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                int64_t saturate = (a < 0.0) ? INT64_MIN : INT64_MAX;
                return {static_cast<uint64_t>(saturate), fcsr};
            } else {
                int64_t ires = static_cast<int64_t>(std::nearbyint(a));
                fesetround(original_rm);
                return {static_cast<uint64_t>(ires), fcsr};
            }
            break;
        }
        case ALUOp::FCVT_LU_D: {
            if (!std::isfinite(a) || a > static_cast<double>(UINT64_MAX) || a < 0.0) {
                fcsr |= FCSR_INVALID_OP;
                fesetround(original_rm);
                uint64_t saturate = (a < 0.0) ? 0 : UINT64_MAX;
                return {saturate, fcsr};
            } else {
                uint64_t ires = static_cast<uint64_t>(std::nearbyint(a));
                fesetround(original_rm);
                return {ires, fcsr};
            }
            break;
        }
        case ALUOp::FCVT_D_W: {
            int32_t ia = static_cast<int32_t>(ina);
            result = static_cast<double>(ia);
            break;

        }
        case ALUOp::FCVT_D_WU: {
            uint32_t ua = static_cast<uint32_t>(ina);
            result = static_cast<double>(ua);
            break;
        }
        case ALUOp::FCVT_D_L: {
            int64_t la = static_cast<int64_t>(ina);
            result = static_cast<double>(la);
            break;

        }
        case ALUOp::FCVT_S_LU: {
            uint64_t ula = static_cast<uint64_t>(ina);
            result = static_cast<double>(ula);
            break;
        }
        case ALUOp::FSGNJ_D: {
            uint64_t a_bits = static_cast<uint64_t>(ina);
            uint64_t b_bits = static_cast<uint64_t>(inb);
            uint64_t temp = (a_bits & 0x7FFFFFFFFFFFFFFF) | (b_bits & 0x8000000000000000);
            std::memcpy(&result, &temp, sizeof(double));
            break;
        }
        case ALUOp::FSGNJN_D: {
            uint64_t a_bits = static_cast<uint64_t>(ina);
            uint64_t b_bits = static_cast<uint64_t>(inb);
            uint64_t temp = (a_bits & 0x7FFFFFFFFFFFFFFF) | (~b_bits & 0x8000000000000000);
            std::memcpy(&result, &temp, sizeof(double));
            break;
        }
        case ALUOp::FSGNJX_D: {
            uint64_t a_bits = static_cast<uint64_t>(ina);
            uint64_t b_bits = static_cast<uint64_t>(inb);
            uint64_t temp = (a_bits & 0x7FFFFFFFFFFFFFFF) | ((a_bits ^ b_bits) & 0x8000000000000000);
            std::memcpy(&result, &temp, sizeof(double));
            break;
        }
        case ALUOp::FMIN_D: {
            if (std::isnan(a) && !std::isnan(b)) {
                result = b;
            } else if (!std::isnan(a) && std::isnan(b)) {
                result = a;
            } else if (std::signbit(a) != std::signbit(b) && a == b) {
                result = -0.0; // Both zero but with different signs — return -0.0
            } else {
                result = std::fmin(a, b);
            }
            break;
        }
        case ALUOp::FMAX_D: {
            if (std::isnan(a) && !std::isnan(b)) {
                result = b;
            } else if (!std::isnan(a) && std::isnan(b)) {
                result = a;
            } else if (std::signbit(a) != std::signbit(b) && a == b) {
                result = 0.0; // Both zero but with different signs — return +0.0
            } else {
                result = std::fmax(a, b);
            }
            break;
        }
        case ALUOp::FEQ_D: {
            if (std::isnan(a) || std::isnan(b)) {
                result = 0.0;
            } else {
                result = (a == b) ? 1.0 : 0.0;
            }
            break;
        }
        case ALUOp::FLT_D: {
            if (std::isnan(a) || std::isnan(b)) {
                result = 0.0;
            } else {
                result = (a < b) ? 1.0 : 0.0;
            }
            break;
        }
        case ALUOp::FLE_D: {
            if (std::isnan(a) || std::isnan(b)) {
                result = 0.0;
            } else {
                result = (a <= b) ? 1.0 : 0.0;
            }
            break;
        }
        case ALUOp::FCLASS_D: {
            uint64_t a_bits = ina;
            double af;
            std::memcpy(&af, &a_bits, sizeof(double));
            uint16_t res = 0;

            if (std::signbit(af) && std::isinf(af)) res |= 1 << 0; // -inf
            else if (std::signbit(af) && std::fpclassify(af) == FP_NORMAL) res |= 1 << 1; // -normal
            else if (std::signbit(af) && std::fpclassify(af) == FP_SUBNORMAL) res |= 1 << 2; // -subnormal
            else if (std::signbit(af) && af == 0.0) res |= 1 << 3; // -zero
            else if (!std::signbit(af) && af == 0.0) res |= 1 << 4; // +zero
            else if (!std::signbit(af) && std::fpclassify(af) == FP_SUBNORMAL) res |= 1 << 5; // +subnormal
            else if (!std::signbit(af) && std::fpclassify(af) == FP_NORMAL) res |= 1 << 6; // +normal
            else if (!std::signbit(af) && std::isinf(af)) res |= 1 << 7; // +inf
            else if (std::isnan(af) && (a_bits & 0x0008000000000000) == 0) res |= 1 << 8; // signaling NaN
            else if (std::isnan(af)) res |= 1 << 9; // quiet NaN

            std::fesetround(original_rm);
            return {res, fcsr};
        }
        case ALUOp::FCVT_D_S: {
            float fa = static_cast<float>(ina);
            result = static_cast<double>(fa);
            break;
        }
        case ALUOp::FCVT_S_D: {
            double da = static_cast<double>(ina);
            result = static_cast<float>(da);
            break;
        }
        case ALUOp::FMV_D_X: {
            uint64_t double_bits;
            std::memcpy(&double_bits, &ina, sizeof(double));
            return {double_bits, fcsr};
        }
        case ALUOp::FMV_X_D: {
            uint64_t int_bits = ina & 0xFFFFFFFFFFFFFFFF;
            double out;
            std::memcpy(&out, &int_bits, sizeof(double));
            result = out;
            break;
        }
        default: break;
        }

        int raised = std::fetestexcept(FE_ALL_EXCEPT);
        if (raised & FE_INVALID)   fcsr |= FCSR_INVALID_OP;
        if (raised & FE_DIVBYZERO) fcsr |= FCSR_DIV_BY_ZERO;
        if (raised & FE_OVERFLOW)  fcsr |= FCSR_OVERFLOW;
        if (raised & FE_UNDERFLOW) fcsr |= FCSR_UNDERFLOW;
        if (raised & FE_INEXACT)   fcsr |= FCSR_INEXACT;

        std::fesetround(original_rm);

        uint64_t result_bits = 0;
        std::memcpy(&result_bits, &result, sizeof(result));
        return {result_bits, fcsr};
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