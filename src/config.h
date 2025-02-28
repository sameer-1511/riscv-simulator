/**
 * @file config.h
 * @brief Contains configuration options for the assembler.
 * @author Vishank Singh, https://github.com/VishankSingh
 */
#ifndef CONFIG_H
#define CONFIG_H

#include "pch.h"
#include "globals.h"

/**
 * @namespace vmConfig
 * @brief Namespace for VM configuration management.
 */
namespace vmConfig {
enum class vmTypes {
    SINGLE_STAGE,
    MULTI_STAGE
};

/**
 * @brief Retrieves the value associated with a given key from the configuration.
 *
 * @param key The key whose value needs to be retrieved.
 * @return The value associated with the key as a string.
 */
std::string getKeyValue(const std::string &key);

/**
 * @namespace INI
 * @brief Namespace for handling INI file operations.
 */
namespace INI {
/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * @param str The string to be trimmed.
 */
[[maybe_unused]] void trim(std::string &str);

/**
 * @brief Retrieves the value associated with a given section and key.
 *
 * @param section The section containing the key.
 * @param key The key whose value needs to be retrieved.
 * @return The value associated with the key as a string.
 */
std::string get(const std::string &section, const std::string &key);

/**
 * @brief Sets the value for a given section and key.
 *
 * @param section The section containing the key.
 * @param key The key to be modified.
 * @param value The value to be set for the key.
 */
void set(const std::string &section, const std::string &key, const std::string &value);

} // namespace INI

vmTypes getVMType();

} // namespace vmConfig


#endif // CONFIG_H
