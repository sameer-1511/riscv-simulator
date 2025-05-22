/**
 * @file rvss_vm.cpp
 * @brief RVSS VM implementation
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "rvss_vm.h"

#include "../../utils.h"
#include "../../globals.h"
#include "../../common/instructions.h"

#include <cctype>

RVSSVM::RVSSVM() : VMBase() {
    dumpRegisters(globals::registers_dump_file, registers_);
}

RVSSVM::~RVSSVM() = default;

void RVSSVM::fetch() {
    current_instruction_ = memory_controller_.readWord(program_counter_);
    updateProgramCounter(4);
}

void RVSSVM::decode() {
    controlUnit.setControlSignals(current_instruction_);
}

void RVSSVM::execute() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    if (InstructionSet::isFInstruction(current_instruction_)) { // RV64 F
        executeFloat();
        return;
    } else if (InstructionSet::isDInstruction(current_instruction_)) {
        executeDouble();
        return;
    } else if (opcode == 0b1110011) {
        executeCSR();
        return;
    }

    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;

    int32_t imm = imm_generator(current_instruction_);

    uint64_t reg1_value = registers_.readGPR(rs1);
    uint64_t reg2_value = registers_.readGPR(rs2);

    bool overflow = false;

    if (controlUnit.getALUSrc()) {
        reg2_value = static_cast<uint64_t>(static_cast<int64_t>(imm));
    }

    ALU::ALUOp aluOperation = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, overflow) = alu_.execute(aluOperation, reg1_value, reg2_value);

    if (controlUnit.getBranch()) {
        switch (funct3) {
        case 0b000: {// BEQ
            branch_flag_ = (execution_result_ == 0);
            break;
        }
        case 0b001: {// BNE
            branch_flag_ = (execution_result_ != 0);
            break;
        }
        case 0b100: {// BLT
            branch_flag_ = (execution_result_ == 1);
            break;
        }
        case 0b101: {// BGE
            branch_flag_ = (execution_result_ == 0);
            break;
        }
        case 0b110: {// BLTU
            branch_flag_ = (execution_result_ == 1);
            break;
        }
        case 0b111: {// BGEU
            branch_flag_ = (execution_result_ == 0);
            break;
        }
        }
    }

    if (controlUnit.getBranch() && branch_flag_) { // JAL, JALR
        next_pc_ = static_cast<int64_t>(program_counter_); // PC was already updated in fetch()
        updateProgramCounter(-4);
        return_address_ = program_counter_ + 4;
        if (opcode == 0b1100111) { // JALR
            updateProgramCounter((execution_result_ & ~1) - program_counter_);
        } else { // JAL
            updateProgramCounter(imm);
        }
    }

    if (opcode == 0b0010111) { // AUIPC
        execution_result_ = static_cast<int64_t>(program_counter_) - 4 + (imm << 12);

    }
}

void RVSSVM::executeFloat() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;
    uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
    uint8_t rm = funct3;
    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rs3 = (current_instruction_ >> 27) & 0b11111;

    uint8_t fcsr_status = 0;

    int32_t imm = imm_generator(current_instruction_);

    if (rm == 0b111) {
        rm = registers_.readCSR(0x002);
    }

    uint64_t reg1_value = registers_.readFPR(rs1);
    uint64_t reg2_value = registers_.readFPR(rs2);
    uint64_t reg3_value = registers_.readFPR(rs3);
    
    if (funct7 == 0b1101000 || funct7 == 0b1111000 || opcode == 0b0000111 || opcode == 0b0100111) {
        reg1_value = registers_.readGPR(rs1);
    }

    if (controlUnit.getALUSrc()) {
        reg2_value = static_cast<uint64_t>(static_cast<int64_t>(imm));
    }

    ALU::ALUOp aluOperation = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, fcsr_status) = alu_.fpexecute(aluOperation, reg1_value, reg2_value, reg3_value, rm);

    registers_.writeCSR(0x003, fcsr_status); 
}

void RVSSVM::executeDouble() {
    // fexecution_result_ = 1.234; // TODO: Implement double floating point operations
    return;
}

void RVSSVM::executeVector() {
    return;
}

void RVSSVM::executeCSR() {
    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint16_t csr = (current_instruction_ >> 20) & 0xFFF;
    uint64_t csr_val = registers_.readCSR(csr);

    csr_target_address_ = csr;
    csr_old_value_ = csr_val;
    csr_write_val_ = registers_.readGPR(rs1);
    csr_uimm_ = rs1; 
}

void RVSSVM::memory() {
    // uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    // uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    if (InstructionSet::isFInstruction(current_instruction_)) { // RV64 F
        memoryFloat();
        return;
    } else if (InstructionSet::isDInstruction(current_instruction_)) {
        memoryDouble();
        return;
    } 


    if (controlUnit.getMemRead()) {
        switch (funct3) {
        case 0b000: {// LB
            memory_result_ = static_cast<int8_t>(memory_controller_.readByte(execution_result_));
            break;
        }
        case 0b001: {// LH
            memory_result_ = static_cast<int16_t>(memory_controller_.readHalfWord(execution_result_));
            break;
        }
        case 0b010: {// LW
            memory_result_ = static_cast<int32_t>(memory_controller_.readWord(execution_result_));
            break;
        }
        case 0b011: {// LD
            memory_result_ = memory_controller_.readDoubleWord(execution_result_);
            break;
        }
        case 0b100: {// LBU
            memory_result_ = static_cast<uint8_t>(memory_controller_.readByte(execution_result_));
            break;
        }
        case 0b101: {// LHU
            memory_result_ = static_cast<uint16_t>(memory_controller_.readHalfWord(execution_result_));
            break;
        }
        case 0b110: {// LWU
            memory_result_ = static_cast<uint32_t>(memory_controller_.readWord(execution_result_));
            break;
        }
        }
    } 
    if (controlUnit.getMemWrite()) {
        switch (funct3)
        {
        case 0b000: {// SB
            memory_controller_.writeByte(execution_result_, registers_.readGPR(rs2));
            break;
        }
        case 0b001: {// SH
            memory_controller_.writeHalfWord(execution_result_, registers_.readGPR(rs2));
            break;
        }
        case 0b010: {// SW
            memory_controller_.writeWord(execution_result_, registers_.readGPR(rs2));
            break;
        }
        case 0b011: {// SD
            memory_controller_.writeDoubleWord(execution_result_, registers_.readGPR(rs2));
            break;
        }
        }
    }

}

void RVSSVM::memoryFloat() {
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    if (controlUnit.getMemRead()) {
        switch (funct3) {
        case 0b010: {// FLW
            memory_result_ = memory_controller_.readWord(execution_result_);
            break;
        }
        }
    }

    if (controlUnit.getMemWrite()) {
        switch (funct3)
        {
        case 0b010: {// FSW
            memory_controller_.writeWord(execution_result_, registers_.readFPR(rs2));
            break;
        }
        }
    }
    return;
}

void RVSSVM::memoryDouble() {
    return;
}

void RVSSVM::memoryVector() {
    return;
}


void RVSSVM::writeBack() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    // uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    // uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    int32_t imm = imm_generator(current_instruction_);

    if (InstructionSet::isFInstruction(current_instruction_)) { // RV64 F
        writeBackFloat();
        return;
    } else if (InstructionSet::isDInstruction(current_instruction_)) {
        writeBackDouble();
        return;
    } else if (opcode == 0b1110011) { // CSR opcode
        writeBackCSR();
        return;
    } 

    if (controlUnit.getRegWrite() && rd != 0) { // Avoid writing to x0
        switch (opcode)
        {
        case 0b0110011: {// R-Type
            registers_.writeGPR(rd, execution_result_);
            break;
        }
        case 0b0010011: {// I-Type
            registers_.writeGPR(rd, execution_result_);
            break;
        }
        case 0b1100011: // B-Type
            break;
        case 0b0000011: {// Load
            registers_.writeGPR(rd, memory_result_);
            break;
        }
        case 0b0100011: // Store
            break;
        case 0b1100111: {// JALR
            registers_.writeGPR(rd, next_pc_);
            break;
        }
        case 0b1101111: {// JAL
            registers_.writeGPR(rd, next_pc_);
            break;
        }
        case 0b0110111: {// LUI
            registers_.writeGPR(rd, (imm << 12));
            break;
        }
        case 0b0010111: {// AUIPC
            registers_.writeGPR(rd, execution_result_);
            break;
        }
        }
    }

    if (opcode == 0x6F) { // JAL
        // Updated in execute()
    }
    if (opcode == 0x67) { // JALR
        registers_.writeGPR(rd, return_address_); // Write back to rs1
        // Updated in execute()
    }

}

void RVSSVM::writeBackFloat() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    
    if (controlUnit.getRegWrite() && rd != 0){
        // write to GPR
        if (funct7 == 0b1010000
        || funct7 == 0b1100000
        || funct7 == 0b1110000) { // f(eq|lt|le).s, fcvt.(w|wu|l|lu).s
            registers_.writeGPR(rd, execution_result_);
        } 
        // write to FPR
        else if (opcode == 0b0000111) {
            registers_.writeFPR(rd, memory_result_);
        } else {
            registers_.writeFPR(rd, execution_result_);
        }
    }
    return;
}

void RVSSVM::writeBackDouble() {
    return;
}

void RVSSVM::writeBackVector() {
    return;
}


void RVSSVM::writeBackCSR() {
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    switch (funct3) {
        case 0b001: { // CSRRW
            if (rd != 0) registers_.writeGPR(rd, csr_old_value_);
            registers_.writeCSR(csr_target_address_, csr_write_val_);
            break;
        }
        case 0b010: { // CSRRS
            if (rd != 0) registers_.writeGPR(rd, csr_old_value_);
            if (csr_write_val_ != 0) {
                registers_.writeCSR(csr_target_address_, csr_old_value_ | csr_write_val_);
            }
            break;
        }
        case 0b011: { // CSRRC
            if (rd != 0) registers_.writeGPR(rd, csr_old_value_);
            if (csr_write_val_ != 0) {
                registers_.writeCSR(csr_target_address_, csr_old_value_ & ~csr_write_val_);
            }
            break;
        }
        case 0b101: { // CSRRWI
            if (rd != 0) registers_.writeGPR(rd, csr_old_value_);
            registers_.writeCSR(csr_target_address_, csr_uimm_);
            break;
        }
        case 0b110: { // CSRRSI
            if (rd != 0) registers_.writeGPR(rd, csr_old_value_);
            if (csr_uimm_ != 0) {
                registers_.writeCSR(csr_target_address_, csr_old_value_ | csr_uimm_);
            }
            break;
        }
        case 0b111: { // CSRRCI
            if (rd != 0) registers_.writeGPR(rd, csr_old_value_);
            if (csr_uimm_ != 0) {
                registers_.writeCSR(csr_target_address_, csr_old_value_ & ~csr_uimm_);
            }
            break;
        }
    }

}

void RVSSVM::run() {
    while(program_counter_ < program_size_) {
        fetch();
        decode();
        execute();
        memory();
        writeBack();
        instructions_retired_++;
        cycle_s++;
    }
    // dumpRegisters(globals::registers_dump_file, registers_.getRegisters());
}

void RVSSVM::debugRun() {
    while (program_counter_ < program_size_) {
        if (std::find(breakpoints_.begin(), breakpoints_.end(), program_counter_) == breakpoints_.end()) {
            fetch();
            decode();
            execute();
            memory();
            writeBack();
            instructions_retired_++;
            cycle_s++;
        } else {
            std::cout << "Breakpoint hit at address: " << program_counter_ << std::endl;
            break;  
        }
    }
}

void RVSSVM::step() {
    if (program_counter_ < program_size_) {
        fetch();
        decode();
        execute();
        memory();
        writeBack();
        instructions_retired_++;
        cycle_s++;
        dumpRegisters(globals::registers_dump_file, registers_);
        std::cout << "Program Counter: " << program_counter_ << std::endl;
    } else if (program_counter_ == program_size_) {
        std::cout << "Program Counter reached end of program: " << program_counter_ << std::endl;
    } else {
        std::cout << "Program Counter out of bounds: " << program_counter_ << std::endl;
        std::cout << "Program size: " << program_size_ << std::endl;
    }
}

void RVSSVM::reset() {
    program_counter_ = 0;
    instructions_retired_ = 0;
    cycle_s = 0;
    registers_.reset();
    memory_controller_.reset();
    controlUnit.reset();
    branch_flag_ = false;
    next_pc_ = 0;
    execution_result_ = 0;
    memory_result_ = 0;
}
    
void RVSSVM::dumpState(const std::string &filename) {
    (void)filename;
}



