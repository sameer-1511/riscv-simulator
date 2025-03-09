#include <gtest/gtest.h>
#include "../src/vm/alu.h"

TEST(ALUTest, AddTest) {
    ALU::ALU alu;
    auto result = alu.execute<int64_t>(ALU::ALUOp::ADD, 10, 20);
    ASSERT_EQ(result.first, 30);
    ASSERT_FALSE(result.second);
}

TEST(ALUTest, SubTest) {
    ALU::ALU alu;
    auto result = alu.execute<int64_t>(ALU::ALUOp::SUB, 10, 20);
    ASSERT_EQ(result.first, -10);
    ASSERT_FALSE(result.second);
}

TEST(ALUTest, MulTest) {
    ALU::ALU alu;
    auto result = alu.execute<int64_t>(ALU::ALUOp::MUL, 10, 20);
    ASSERT_EQ(result.first, 200);
    ASSERT_FALSE(result.second);
}

TEST(ALUTest, SllTest) {
    ALU::ALU alu;
    auto result = alu.execute<int64_t>(ALU::ALUOp::SLL, 0b1010, 1);
    ASSERT_EQ(result.first, 0b10100);
    ASSERT_FALSE(result.second);
}

TEST(ALUTest, SrlTest) {
    ALU::ALU alu;
    auto result = alu.execute<int64_t>(ALU::ALUOp::SRL, 0b1010, 1);
    ASSERT_EQ(result.first, 0b101);
    ASSERT_FALSE(result.second);
}

TEST(ALUTest, SraTest) {
    ALU::ALU alu;
    auto result = alu.execute<int64_t>(ALU::ALUOp::SRA, 0xfffffffffffffffa, 2);
    ASSERT_EQ(result.first, 0xfffffffffffffffe);
    ASSERT_FALSE(result.second);
}