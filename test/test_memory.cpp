/**
 * File Name: test_memory.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include <gtest/gtest.h>
#include "../src/vm/main_memory.h"

TEST(MemoryTest, ReadWriteTest) {
    Memory memory;
    memory.write(0, 1);
    memory.write(1024, 2);
    memory.write(2048, 3);
    memory.write(268435456, 4);

    EXPECT_EQ(memory.read(0), 1);
    EXPECT_EQ(memory.read(1024), 2);
    EXPECT_EQ(memory.read(2048), 3);
    EXPECT_EQ(memory.read(268435456), 4);

}

TEST(MemoryTest, ReadWriteByteTest) {
    Memory memory;
    memory.writeByte(0, 1);
    memory.writeByte(1024, 2);
    memory.writeByte(2048, 3);
    memory.writeByte(268435456, 4);

    EXPECT_EQ(memory.readByte(0), 1);
    EXPECT_EQ(memory.readByte(1024), 2);
    EXPECT_EQ(memory.readByte(2048), 3);
    EXPECT_EQ(memory.readByte(268435456), 4);

}

TEST(MemoryTest, ReadWriteHalfWordTest) {
    Memory memory;
    memory.writeHalfWord(0, 1);
    memory.writeHalfWord(1024, 2);
    memory.writeHalfWord(2048, 3);
    memory.writeHalfWord(268435456, 4);

    EXPECT_EQ(memory.readHalfWord(0), 1);
    EXPECT_EQ(memory.readHalfWord(1024), 2);
    EXPECT_EQ(memory.readHalfWord(2048), 3);
    EXPECT_EQ(memory.readHalfWord(268435456), 4);

}

TEST(MemoryTest, ReadWriteLargeFloatTest) {
    Memory memory;

    float large_value1 = 5.3242353f;
    float large_value2 = -12345.789f;
    float large_value3 = 9.876543f;
    float large_value4 = 3.4028235e+38f;

    memory.writeFloat(0, large_value1);
    memory.writeFloat(1024, large_value2);
    memory.writeFloat(2048, large_value3);
    memory.writeFloat(4096, large_value4);

    EXPECT_FLOAT_EQ(memory.readFloat(0), large_value1);
    EXPECT_FLOAT_EQ(memory.readFloat(1024), large_value2);
    EXPECT_FLOAT_EQ(memory.readFloat(2048), large_value3);
    EXPECT_FLOAT_EQ(memory.readFloat(4096), large_value4);
}


TEST(MemoryTest, ReadWriteLargeDoubleTest) {
    Memory memory;

    double large_value1 = 5.3242353453425;
    double large_value2 = -123456.789123456;
    double large_value3 = 9.876543210987654;
    double large_value4 = 1.7976931348623157e+308; 

    memory.writeDouble(0, large_value1);
    memory.writeDouble(1024, large_value2);
    memory.writeDouble(2048, large_value3);
    memory.writeDouble(4096, large_value4);

    EXPECT_DOUBLE_EQ(memory.readDouble(0), large_value1);
    EXPECT_DOUBLE_EQ(memory.readDouble(1024), large_value2);
    EXPECT_DOUBLE_EQ(memory.readDouble(2048), large_value3);
    EXPECT_DOUBLE_EQ(memory.readDouble(4096), large_value4);
}


