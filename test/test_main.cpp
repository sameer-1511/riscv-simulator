/**
 * File Name: test_main.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include <gtest/gtest.h>

#include "../src/vm/main_memory.h"
#include "../src/vm/main_memory.cpp"

#include "../src/globals.h"
#include "../src/globals.cpp"
#include "../src/config.h"
#include "../src/config.cpp"


// TODO: write tests for the Memory class

TEST(MemoryTest, ReadWriteTest) {
    Memory memory;
    memory.write(0, 1);
    memory.write(1024, 2);
    memory.write(2048, 3);
    memory.write(268435456, 4);

    ASSERT_EQ(memory.read(0), 1);
    ASSERT_EQ(memory.read(1024), 2);
    ASSERT_EQ(memory.read(2048), 3);
    ASSERT_EQ(memory.read(268435456), 4);

}

TEST(MemoryTest, ReadWriteByteTest) {
    Memory memory;
    memory.writeByte(0, 1);
    memory.writeByte(1024, 2);
    memory.writeByte(2048, 3);
    memory.writeByte(268435456, 4);

    ASSERT_EQ(memory.readByte(0), 1);
    ASSERT_EQ(memory.readByte(1024), 2);
    ASSERT_EQ(memory.readByte(2048), 3);
    ASSERT_EQ(memory.readByte(268435456), 4);

}

TEST(MemoryTest, ReadWriteHalfWordTest) {
    Memory memory;
    memory.writeHalfWord(0, 1);
    memory.writeHalfWord(1024, 2);
    memory.writeHalfWord(2048, 3);
    memory.writeHalfWord(268435456, 4);

    ASSERT_EQ(memory.readHalfWord(0), 1);
    ASSERT_EQ(memory.readHalfWord(1024), 2);
    ASSERT_EQ(memory.readHalfWord(2048), 3);
    ASSERT_EQ(memory.readHalfWord(268435456), 4);

}


TEST(ConfigTest, GetKeyValueTest) {
    std::string processor_type = vmConfig::INI::get("Execution", "processor_type");
    std::string memory_size = vmConfig::INI::get("Memory", "memory_size");
    ASSERT_EQ(processor_type, "single_cycle");
    ASSERT_EQ(memory_size, "");
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





int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}