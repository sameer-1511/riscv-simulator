/**
 * File Name: test_vm.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include <gtest/gtest.h>
#include "../src/vm/rvss/rvss_vm.h"
#include "../src/assembler/assembler.h"


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

TEST(VmTest, ExecutionTest1) {
    RVSSVM vm;
    AssembledProgram program;
    program.text_buffer.push_back(0x00b50633);
    program.text_buffer.push_back(0x00b55633);
    vm.loadProgram(program);
    vm.registers_.writeGPR(10, 10);
    vm.registers_.writeGPR(11, 20);
    // uint32_t add_instruction = 0x00b50633;
    // vm.current_instruction_ = add_instruction;
    vm.fetch();
    vm.decode();
    vm.execute();
    vm.memory();
    vm.writeBack();
    int64_t result = vm.registers_.readGPR(12);
    ASSERT_EQ(result, 30);
    ASSERT_EQ(vm.execution_result_, 30);
    vm.registers_.writeGPR(10, 0b1101);
    vm.registers_.writeGPR(11, 2);
    vm.fetch();
    vm.decode();
    vm.execute();
    vm.memory();
    vm.writeBack();
    int64_t result2 = vm.registers_.readGPR(12);
    ASSERT_EQ(result2, 3);
    ASSERT_EQ(vm.execution_result_, 3);

}


TEST(VmTest, ExecutionTest2) {
    RVSSVM vm;
    AssembledProgram program;
    program.text_buffer.push_back(0x00b55633);
    vm.loadProgram(program);
    vm.registers_.writeGPR(10, 0b1101);
    vm.registers_.writeGPR(11, 2);
    vm.fetch();
    vm.decode();
    vm.execute();
    vm.memory();
    vm.writeBack();
    int64_t result = vm.registers_.readGPR(12);
    ASSERT_EQ(result, 3);
    ASSERT_EQ(vm.execution_result_, 3);
}

TEST(VmTest, ExecutionTest3) {
    RVSSVM vm;
    AssembledProgram program;
    program.text_buffer.push_back(0x01700513);
    program.text_buffer.push_back(0x01b50513);
    vm.loadProgram(program);
    vm.fetch();
    vm.decode();
    vm.execute();
    vm.memory();
    vm.writeBack();
    int64_t result = vm.registers_.readGPR(10);
    ASSERT_EQ(result, 23);
    ASSERT_EQ(vm.execution_result_, 23);
    vm.fetch();
    vm.decode();
    vm.execute();
    vm.memory();
    vm.writeBack();
    int64_t result2 = vm.registers_.readGPR(10);
    ASSERT_EQ(result2, 50);
    ASSERT_EQ(vm.execution_result_, 50);
}


TEST(VmTest, ExecutionTest4) {
    AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/branch_test.s");
    RVSSVM vm;
    vm.loadProgram(program);
    vm.step();
    vm.step();
    vm.fetch();
    vm.decode();
    vm.execute();
    ASSERT_EQ(vm.branch_flag_, true);
    ASSERT_EQ(vm.program_counter_, 0x0);
    vm.step();

    // vm.memory();
    // vm.writeBack();
}

TEST(VmTest, ExecutionTest5) {
    AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/load_test.s");
    RVSSVM vm;
    vm.loadProgram(program);
    vm.fetch();
    vm.decode();
    vm.execute();
    ASSERT_EQ(vm.execution_result_, 0);
    vm.memory();
    vm.writeBack();

    ASSERT_EQ(vm.registers_.readGPR(10), 0x0000000000003503);

}

TEST(VmTest, ExecutionTest6) {
    AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/load_store_test_1.s");
    RVSSVM vm;
    vm.loadProgram(program);
    vm.fetch();
    vm.decode();
    vm.execute();
    vm.memory();
    vm.writeBack();
    ASSERT_EQ(vm.registers_.readGPR(10), 0x1000059300003503);
    vm.step();
    ASSERT_EQ(vm.registers_.readGPR(11), 0x0000000000000100);
    vm.step();
    uint64_t result3 = vm.memory_controller_.readDoubleWord(0x100);
    ASSERT_EQ(result3, 0x1000059300003503);

}

TEST(VmTest, ExecutionTest7) {
    AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/load_store_test_2.s");
    RVSSVM vm;
    vm.loadProgram(program);
    vm.step();
    ASSERT_EQ(vm.registers_.readGPR(12), 0xffffffffffffffff);
    vm.step();
    ASSERT_EQ(vm.registers_.readGPR(10), 0x0000000000000100);
    vm.step();
    uint64_t result3 = vm.memory_controller_.readByte(0x100);
    ASSERT_EQ(result3, 0xff);
    uint64_t result4 = vm.memory_controller_.readDoubleWord(0x100);
    ASSERT_EQ(result4, 0xff);
    vm.step();
    ASSERT_EQ(vm.registers_.readGPR(13), 0xffffffffffffffff);


}

TEST(VmTest, ExecutionTest8) {
    AssembledProgram program = assemble("/home/vis/Desk/codes/assembler/examples/load_test_2.s");
    RVSSVM vm;
    vm.loadProgram(program);
    vm.registers_.writeGPR(3, 0x10000000); // set the data section address
    vm.step();
    ASSERT_EQ(vm.registers_.readGPR(10), 121);
}
