/**
 * @file registers.h
 * @brief Contains the definition of the RegisterFile class for managing registers in the simulator.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#include "../pch.h"

/**
 * @brief Represents a register file containing integer, floating-point, and vector registers.
 */
class RegisterFile {
private:
    static constexpr size_t NUM_GPR = 32; ///< Number of General-Purpose Registers (GPR).
    static constexpr size_t NUM_FPR = 32; ///< Number of Floating-Point Registers (FPR).
    static constexpr size_t NUM_VR = 32;  ///< Number of Vector Registers (VR).

    std::array<int64_t, NUM_GPR> gpr_ = {}; ///< Array for storing GPR values.
    std::array<double, NUM_FPR> fpr_ = {}; ///< Array for storing FPR values.
    std::vector<std::array<uint64_t, 8>> vr_ = 
        std::vector<std::array<uint64_t, 8>>(NUM_VR); ///< Vector of arrays for VR values.

    /**
     * @brief Formats the values of registers as a string for display.
     * @tparam T The type of the register values.
     * @param values The array of register values.
     * @return A formatted string representation of the register values.
     */
    template <typename T>
    [[nodiscard]] static std::string formatRegisterValues(const std::array<T, NUM_GPR>& values);

public:
    /**
     * @brief Enum representing the type of a register.
     */
    enum class RegisterType {
        INVALID,         ///< Invalid register type.
        INTEGER,         ///< General-purpose integer register.
        FLOATING_POINT,  ///< Floating-point register.
        VECTOR,          ///< Vector register.
        CSR              ///< Control and Status Register (CSR).
    };

    /**
     * @brief Constructs a RegisterFile object and initializes the registers.
     */
    RegisterFile();

    /**
     * @brief Resets all registers to their default values.
     */
    void reset();

    /**
     * @brief Reads the value of a General-Purpose Register (GPR).
     * @param reg The index of the GPR to read.
     * @return The value of the GPR at the specified index.
     */
    [[nodiscard]] int64_t readGPR(size_t reg) const;

    /**
     * @brief Writes a value to a General-Purpose Register (GPR).
     * @param reg The index of the GPR to write.
     * @param value The value to write.
     */
    void writeGPR(size_t reg, int64_t value);

    /**
     * @brief Reads the value of a Floating-Point Register (FPR).
     * @param reg The index of the FPR to read.
     * @return The value of the FPR at the specified index.
     */
    [[nodiscard]] double readFPR(size_t reg) const;

    /**
     * @brief Writes a value to a Floating-Point Register (FPR).
     * @param reg The index of the FPR to write.
     * @param value The value to write.
     */
    void writeFPR(size_t reg, double value);

    /**
     * @brief Reads the value of a Vector Register (VR).
     * @param reg The index of the VR to read.
     * @return The array representing the value of the VR at the specified index.
     */
    [[nodiscard]] std::array<uint64_t, 8> readVR(size_t reg) const;

    /**
     * @brief Writes a value to a Vector Register (VR).
     * @param reg The index of the VR to write.
     * @param value The array representing the value to write.
     */
    void writeVR(size_t reg, const std::array<uint64_t, 8>& value);

    /**
     * @brief Retrieves the values of all General-Purpose Registers (GPR).
     * @return A vector containing the values of all GPRs.
     */
    [[nodiscard]] std::vector<int64_t> getGPRValues() const;

    /**
     * @brief Retrieves the values of all Floating-Point Registers (FPR).
     * @return A vector containing the values of all FPRs.
     */
    [[nodiscard]] std::vector<double> getFPRValues() const;
    
    /**
     * @brief Retrieves the values of all Vector Registers (VR).
     * @return A vector containing the arrays of values of all VRs.
     */
    [[nodiscard]] std::vector<std::array<uint64_t, 8>> getVRValues() const;

    /**
     * @brief Converts the General-Purpose Register (GPR) values to a string.
     * @return A string representation of the GPR values.
     */
    [[nodiscard]] std::string gprToString() const;

    /**
     * @brief Converts the Floating-Point Register (FPR) values to a string.
     * @return A string representation of the FPR values.
     */
    [[nodiscard]] std::string fprToString() const;
    /**
     * @brief Converts the Vector Register (VR) values to a string.
     * @return A string representation of the VR values.
     */
    [[nodiscard]] std::string vrToString() const;

    /**
     * @brief Gets the count of registers of a specific type.
     * @param type The type of register (GPR, FPR, or VR).
     * @return The number of registers of the specified type.
     */
    static size_t getRegisterCount(RegisterType type);

    /**
     * @brief Gets the name of a General-Purpose Register (GPR).
     * @param reg The index of the GPR.
     * @return The name of the GPR.
     */
    static std::string getGPRName(size_t reg);

    /**
     * @brief Gets the name of a Floating-Point Register (FPR).
     * @param reg The index of the FPR.
     * @return The name of the FPR.
     */
    static std::string getFPRName(size_t reg);

    /**
     * @brief Gets the name of a Vector Register (VR).
     * @param reg The index of the VR.
     * @return The name of the VR.
     */
    static std::string getVRName(size_t reg);
};

/**
 * @brief Set of valid register names for validation.
 */
extern const std::unordered_set<std::string> valid_registers;

/**
 * @brief Map of register aliases to their actual names.
 */
extern const std::unordered_map<std::string, std::string> reg_alias_to_name;

/**
 * @brief Checks whether a given register name is valid.
 * @param reg The register name to check.
 * @return True if the register name is valid, false otherwise.
 */
bool isValidRegister(const std::string &reg);


#endif // REGISTERS_H