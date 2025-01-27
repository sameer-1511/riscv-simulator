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
        ADD, ///< Addition operation.
        SUB, ///< Subtraction operation.
        MUL, ///< Multiplication operation.
        DIV, ///< Division operation.
        MOD, ///< Modulus operation.
        AND, ///< Bitwise AND operation.
        OR, ///< Bitwise OR operation.
        XOR, ///< Bitwise XOR operation.
        NOT, ///< Bitwise NOT operation.
        SHL, ///< Shift left operation.
        SHR, ///< Shift right operation.
        ROL, ///< Rotate left operation.
        ROR, ///< Rotate right operation.
        CMP, ///< Compare operation.
        TEST, ///< Test operation.
        INC, ///< Increment operation.
        DEC, ///< Decrement operation.
        NEG, ///< Negate operation.
        LOGIC_SHIFT ///< Logical shift operation.
    };

    /**
     * @brief The ALU class is responsible for performing arithmetic and logic operations.
     */
    class ALU {
    public:
        /**
         * @brief Constructs an ALU instance.
         */
        ALU() = default;

        /**
         * @brief Destroys the ALU instance.
         */
        ~ALU() = default;

        /**
         * @brief Performs an arithmetic or logic operation.
         * @param op The operation to perform.
         * @param a The first operand.
         * @param b The second operand.
         * @return The result of the operation.
         */
        template<typename T>
        T execute(ALUOp op, T a, T b);            
    };
}



#endif // ALU_H