/**
 * File Name: test_main.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include <gtest/gtest.h>

#include "../src/memory.h"
#include "../src/memory.cpp"


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



int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}