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
        SUB, ///< Subtraction operation.
        MUL, ///< Multiplication operation.
        DIV, ///< Division operation.
        DIVU, ///< Unsigned division operation.
        REM, ///< Remainder operation.
        REMU, ///< Unsigned remainder operation.
        AND, ///< Bitwise AND operation.
        OR, ///< Bitwise OR operation.
        XOR, ///< Bitwise XOR operation.
        SLL, ///< Shift left logical operation.
        SRL, ///< Shift right logical operation.
        SRA, ///< Shift right arithmetic operation.
        SLT, ///< Set less than operation.
        SLTU, ///< Unsigned set less than operation.            
    };

    /**
     * @brief The ALU class is responsible for performing arithmetic and logic operations.
     */
    class ALU {
    public:
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
        template<typename T>
        constexpr std::pair<T, bool> execute(ALUOp op, T a, T b) const {
            switch (op) {
                case ALUOp::ADD:  return { static_cast<T>(a + b), __builtin_add_overflow(a, b, &a) };
                case ALUOp::SUB:  return { static_cast<T>(a - b), __builtin_sub_overflow(a, b, &a) };
                case ALUOp::MUL:  return { static_cast<T>(a * b), __builtin_mul_overflow(a, b, &a) };
                case ALUOp::DIV:  return { (b != 0) ? static_cast<T>(a / b) : 0, false };
                case ALUOp::DIVU: return { (b != 0) ? static_cast<T>(static_cast<std::make_unsigned_t<T>>(a) / static_cast<std::make_unsigned_t<T>>(b)) : 0, false };
                case ALUOp::REM:  return { (b != 0) ? static_cast<T>(a % b) : 0, false };
                case ALUOp::REMU: return { (b != 0) ? static_cast<T>(static_cast<std::make_unsigned_t<T>>(a) % static_cast<std::make_unsigned_t<T>>(b)) : 0, false };
                case ALUOp::AND:  return { static_cast<T>(a & b), false };
                case ALUOp::OR:   return { static_cast<T>(a | b), false };
                case ALUOp::XOR:  return { static_cast<T>(a ^ b), false };
                case ALUOp::SLL:  return { static_cast<T>(a << (b & (sizeof(T) * 8 - 1))), false };
                case ALUOp::SRL:  return { static_cast<T>(static_cast<std::make_unsigned_t<T>>(a) >> (b & (sizeof(T) * 8 - 1))), false };
                case ALUOp::SRA:  return { static_cast<T>(a >> (b & (sizeof(T) * 8 - 1))), false };
                case ALUOp::SLT:  return { static_cast<T>(a < b), false };
                case ALUOp::SLTU: return { static_cast<T>(static_cast<std::make_unsigned_t<T>>(a) < static_cast<std::make_unsigned_t<T>>(b)), false };
                default:          return { 0, false };  // Undefined behavior
            }      
        }      
    };
}



#endif // ALU_H