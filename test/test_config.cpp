/**
 * File Name: test_config.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include <gtest/gtest.h>

#include "../src/config.h"


TEST(ConfigTest, GetKeyValueTest) {
    std::string processor_type = vmConfig::INI::get("Execution", "processor_type");
    std::string memory_size = vmConfig::INI::get("Memory", "memory_size");
    ASSERT_EQ(processor_type, "single_stage");
    ASSERT_EQ(memory_size, "0xffffffffffffffff");
}


TEST(ConfigTest, SetKeyValueTest) {
    vmConfig::INI::set("Execution", "hazard_detection", "false");
    std::string hazard_detection_status = vmConfig::INI::get("Execution", "hazard_detection");
    ASSERT_EQ(hazard_detection_status, "false");

    vmConfig::INI::set("Cache", "cache_enabled", "false");
    std::string cache_enabled = vmConfig::INI::get("Cache", "cache_enabled");
    ASSERT_EQ(cache_enabled, "false");
}

TEST(ConfigTest, InvalidKeyValueTest) {
    ASSERT_THROW(vmConfig::INI::get("NonExistentSection", "non_existent_key"), std::invalid_argument);
    ASSERT_THROW(vmConfig::INI::set("NonExistentSection", "non_existent_key", "value"), std::invalid_argument);
}
