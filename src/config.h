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
 * @namespace vm_config
 * @brief Namespace for VM configuration management.
 */
namespace vm_config {
enum class VmTypes {
  SINGLE_STAGE,
  MULTI_STAGE
};

/**
 * @namespace ini
 * @brief Namespace for handling ini file operations.
 */
namespace ini {
/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * @param str The string to be trimmed.
 */
[[maybe_unused]] void Trim(std::string &str);

/**
 * @brief Retrieves the value associated with a given section and key.
 *
 * @param section The section containing the key.
 * @param key The key whose value needs to be retrieved.
 * @return The value associated with the key as a string.
 */
std::string Get(const std::string &section, const std::string &key);

/**
 * @brief Sets the value for a given section and key.
 *
 * @param section The section containing the key.
 * @param key The key to be modified.
 * @param value The value to be set for the key.
 */
void Set(const std::string &section, const std::string &key, const std::string &value);

} // namespace ini

VmTypes GetVmType();

} // namespace vm_config


#endif // CONFIG_H
