/**
 * File Name: test_vm.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include <gtest/gtest.h>
#include "../src/vm/rvss/rvss_vm.h"


TEST(VmTest, ImmGenTest1) {
    RVSSVM vm;
    uint32_t lui_instruction = 0x100001b7;
    int32_t imm = vm.imm_generator(lui_instruction);
    ASSERT_EQ(imm, 0x10000);

    uint32_t auipc_instruction = 0x10000197;
    imm = vm.imm_generator(auipc_instruction);
    ASSERT_EQ(imm, 0x10000);

    uint32_t jal_instruction = 0x1320056f;
    imm = vm.imm_generator(jal_instruction);
    ASSERT_EQ(imm, 0x132);

    uint32_t jalr_instruction = 0x00008067;
    imm = vm.imm_generator(jalr_instruction);
    ASSERT_EQ(imm, 0x0);

    uint32_t branch_instruction = 0xfc000ae3;
    imm = vm.imm_generator(branch_instruction);
    ASSERT_EQ(imm, -44);

    uint32_t load_instruction = 0x00002283;
    imm = vm.imm_generator(load_instruction);
    ASSERT_EQ(imm, 0x0);

    uint32_t store_instruction = 0x00112023;
    imm = vm.imm_generator(store_instruction);
    ASSERT_EQ(imm, 0x0);

    uint32_t immediate_instruction = 0x00100093;
    imm = vm.imm_generator(immediate_instruction);
    ASSERT_EQ(imm, 0x1);

    uint32_t lui_instruction2 = 0x000000b7;
    imm = vm.imm_generator(lui_instruction2);
    ASSERT_EQ(imm, 0x0);

    uint32_t auipc_instruction2 = 0x00000097;
    imm = vm.imm_generator(auipc_instruction2);
    ASSERT_EQ(imm, 0x0);
}

