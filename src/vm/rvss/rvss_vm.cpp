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

    // std::cout << "Reg1 value: " << std::hex << reg1_value << std::dec << std::endl;
    // std::cout << "Reg2 value: " << std::hex << reg2_value << std::dec << std::endl;


    ALU::ALUOp aluOperation = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, overflow) = alu_.execute(aluOperation, reg1_value, reg2_value);
    // std::cout << "Execution result: " << std::hex << execution_result_ << std::dec << std::endl;


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

    // std::cout << "Reg1 value: " << std::hex << reg1_value << std::dec << std::endl;
    // std::cout << "Reg2 value: " << std::hex << reg2_value << std::dec << std::endl;
    // std::cout << "Reg3 value: " << std::hex << reg3_value << std::dec << std::endl;



    ALU::ALUOp aluOperation = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, fcsr_status) = alu_.fpexecute(aluOperation, reg1_value, reg2_value, reg3_value, rm);
    // std::cout << "Execution result: " << std::hex << execution_result_ << std::dec << std::endl;

    registers_.writeCSR(0x003, fcsr_status); 
}

void RVSSVM::executeDouble() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;
    uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
    uint8_t rm = funct3;
    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rs3 = (current_instruction_ >> 27) & 0b11111;

    uint8_t fcsr_status = 0;

    int32_t imm = imm_generator(current_instruction_);

    uint64_t reg1_value = registers_.readFPR(rs1);
    uint64_t reg2_value = registers_.readFPR(rs2);
    uint64_t reg3_value = registers_.readFPR(rs3);

    if (funct7 == 0b1101001 || funct7 == 0b1111001 || opcode == 0b0000111 || opcode == 0b0100111) {
        reg1_value = registers_.readGPR(rs1);
    }

    if (controlUnit.getALUSrc()) {
        reg2_value = static_cast<uint64_t>(static_cast<int64_t>(imm));
    }

    ALU::ALUOp aluOperation = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, fcsr_status) = alu_.fpexecute(aluOperation, reg1_value, reg2_value, reg3_value, rm);

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
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
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
    
    uint64_t addr = execution_result_;
    uint64_t old_mem = memory_controller_.readDoubleWord(addr);

    if (controlUnit.getMemWrite()) {
        switch (funct3) {
        case 0b000: {// SB
            memory_controller_.writeByte(execution_result_, registers_.readGPR(rs2) & 0xFF);
            break;
        }
        case 0b001: {// SH
            memory_controller_.writeHalfWord(execution_result_, registers_.readGPR(rs2) & 0xFFFF);
            break;
        }
        case 0b010: {// SW
            memory_controller_.writeWord(execution_result_, registers_.readGPR(rs2) & 0xFFFFFFFF);
            break;
        }
        case 0b011: {// SD
            memory_controller_.writeDoubleWord(execution_result_, registers_.readGPR(rs2) & 0xFFFFFFFFFFFFFFFF);
            break;
        }
        }
    }

    uint64_t new_mem = memory_controller_.readDoubleWord(addr);
    if (old_mem != new_mem) {
        current_delta_.memory_changes.push_back({addr, old_mem, new_mem});
    }
}

void RVSSVM::memoryFloat() {
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    if (controlUnit.getMemRead()) { // FLW
        memory_result_ = memory_controller_.readWord(execution_result_);
    }

    uint64_t addr = 0;
    uint64_t old_mem = 0;
    uint64_t new_mem = 0;

    if (controlUnit.getMemWrite()) { // FSW
        addr = execution_result_;
        old_mem = memory_controller_.readDoubleWord(addr);
        uint32_t val = registers_.readFPR(rs2) & 0xFFFFFFFF;
        memory_controller_.writeWord(execution_result_, val);
        new_mem = memory_controller_.readDoubleWord(addr);
    }

    if (old_mem != new_mem) {
        current_delta_.memory_changes.push_back({addr, old_mem, new_mem});
    }
}

void RVSSVM::memoryDouble() {
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    if (controlUnit.getMemRead()) {// FLD
        memory_result_ = memory_controller_.readDoubleWord(execution_result_);
    }

    uint64_t addr = 0;
    uint64_t old_mem = 0;
    uint64_t new_mem = 0;

    if (controlUnit.getMemWrite()) {// FSD
        addr = execution_result_;
        old_mem = memory_controller_.readDoubleWord(addr);
        memory_controller_.writeDoubleWord(execution_result_, registers_.readFPR(rs2));
        new_mem = memory_controller_.readDoubleWord(addr);
    }

    if (old_mem != new_mem) {
        current_delta_.memory_changes.push_back({addr, old_mem, new_mem});
    }

    return;
}

void RVSSVM::memoryVector() {
    return;
}


void RVSSVM::writeBack() {
    uint8_t opcode = current_instruction_ & 0b1111111;
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

    uint64_t old_reg = registers_.readGPR(rd);
    unsigned int reg_index = rd;
    unsigned int reg_type = 0; // 0 for GPR, 1 for CSR, 2 for FPR


    if (controlUnit.getRegWrite() && rd != 0) { // Avoid writing to x0
        switch (opcode)
        {
        case 0b0110011: // R-Type
        case 0b0010011: // I-Type
        case 0b0010111: {// AUIPC
            registers_.writeGPR(rd, execution_result_);
            break;
        }
        case 0b0000011: {// Load
            registers_.writeGPR(rd, memory_result_);
            break;
        }
        case 0b1100111: // JALR
        case 0b1101111: {// JAL
            registers_.writeGPR(rd, next_pc_);
            break;
        }
        case 0b0110111: {// LUI
            registers_.writeGPR(rd, (imm << 12));
            break;
        }
        }
    }

    if (opcode == 0b1101111) { // JAL
        // Updated in execute()
    }
    if (opcode == 0b1100111) { // JALR
        registers_.writeGPR(rd, return_address_); // Write back to rs1
        // Updated in execute()
    }

    uint64_t new_reg = registers_.readGPR(rd);
    if (old_reg != new_reg) {
        current_delta_.register_changes.push_back({reg_index, reg_type, old_reg, new_reg});
    }


}

void RVSSVM::writeBackFloat() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;

    uint64_t old_reg = 0;
    unsigned int reg_index = rd;
    unsigned int reg_type = 2; // 0 for GPR, 1 for CSR, 2 for FPR
    uint64_t new_reg = 0;
    
    if (controlUnit.getRegWrite() && rd != 0){
        // write to GPR
        if (funct7 == 0b1010000
        || funct7 == 0b1100000
        || funct7 == 0b1110000) { // f(eq|lt|le).s, fcvt.(w|wu|l|lu).s
            old_reg = registers_.readGPR(rd);
            registers_.writeGPR(rd, execution_result_);
            new_reg = execution_result_;
            reg_type = 0; // GPR

        } 
        // write to FPR
        else if (opcode == 0b0000111) {
            old_reg = registers_.readFPR(rd);
            registers_.writeFPR(rd, memory_result_);
            new_reg = memory_result_;
            reg_type = 2; // FPR
        } else {
            old_reg = registers_.readFPR(rd);
            registers_.writeFPR(rd, execution_result_);
            new_reg = execution_result_;
            reg_type = 2; // FPR
        }
    }

    if (old_reg != new_reg) {
        current_delta_.register_changes.push_back({reg_index, reg_type, old_reg, new_reg});
    }

    return;
}

void RVSSVM::writeBackDouble() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;

    uint64_t old_reg = 0;
    unsigned int reg_index = rd;
    unsigned int reg_type = 2; // 0 for GPR, 1 for CSR, 2 for FPR
    uint64_t new_reg = 0;
    
    if (controlUnit.getRegWrite() && rd != 0){
        // write to GPR
        if (funct7 == 0b1010001
        || funct7 == 0b1100001
        || funct7 == 0b1110001) { // f(eq|lt|le).d, fcvt.(w|wu|l|lu).d
            old_reg = registers_.readGPR(rd);
            registers_.writeGPR(rd, execution_result_);
            new_reg = execution_result_;
            reg_type = 0; // GPR
        } 
        // write to FPR
        else if (opcode == 0b0000111) {
            old_reg = registers_.readFPR(rd);
            registers_.writeFPR(rd, memory_result_);
            new_reg = memory_result_;
            reg_type = 2; // FPR
        } else {
            old_reg = registers_.readFPR(rd);
            registers_.writeFPR(rd, execution_result_);
            new_reg = execution_result_;
            reg_type = 2; // FPR
        }
    }

    if (old_reg != new_reg) {
        current_delta_.register_changes.push_back({reg_index, reg_type, old_reg, new_reg});
    }

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
    current_delta_.old_pc = program_counter_;
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

        current_delta_.new_pc = program_counter_;
        undo_stack_.push(current_delta_);
        while (!redo_stack_.empty()) {
            redo_stack_.pop();
        }

        current_delta_.register_changes.clear();
        current_delta_.memory_changes.clear();
        current_delta_.old_pc = 0;
        current_delta_.new_pc = 0;
    } else if (program_counter_ == program_size_) {
        std::cout << "Program Counter reached end of program: " << program_counter_ << std::endl;
    } else {
        std::cout << "Program Counter out of bounds: " << program_counter_ << std::endl;
        std::cout << "Program size: " << program_size_ << std::endl;
    }
}

void RVSSVM::undo() {
    if (undo_stack_.empty()) {
        std::cout << "No more undo steps available." << std::endl;
        return;
    }

    StepDelta last = undo_stack_.top();
    undo_stack_.pop();
    for (const auto& change: last.register_changes) {
        switch (change.reg_type) {
            case 0: { // GPR
                registers_.writeGPR(change.reg_index, change.old_value);
                break;
            }
            case 1: { // CSR
                registers_.writeCSR(change.reg_index, change.old_value);
                break;
            }
            case 2: { // FPR
                registers_.writeFPR(change.reg_index, change.old_value);
                break;
            }
            default:
                std::cerr << "Invalid register type: " << change.reg_type << std::endl;
                break;
        }
    }

    for (const auto& change: last.memory_changes) {
        memory_controller_.writeDoubleWord(change.address, change.old_bytes);
    }

    program_counter_ = last.old_pc;
    instructions_retired_--;
    cycle_s--;
    dumpRegisters(globals::registers_dump_file, registers_);
    std::cout << "Program Counter: " << program_counter_ << std::endl;

    redo_stack_.push(last);
    // current_delta_.register_changes.clear();
    // current_delta_.memory_changes.clear();
    // current_delta_.old_pc = 0;
    // current_delta_.new_pc = 0;



}

void RVSSVM::redo() {
    if (redo_stack_.empty()) {
        std::cout << "No more redo steps available." << std::endl;
        return;
    }

    StepDelta next = redo_stack_.top();
    redo_stack_.pop();

    for (const auto& change: next.register_changes) {
        switch (change.reg_type) {
            case 0: { // GPR
                registers_.writeGPR(change.reg_index, change.new_value);
                break;
            }
            case 1: { // CSR
                registers_.writeCSR(change.reg_index, change.new_value);
                break;
            }
            case 2: { // FPR
                registers_.writeFPR(change.reg_index, change.new_value);
                break;
            }
            default:
                std::cerr << "Invalid register type: " << change.reg_type << std::endl;
                break;
        }
    }

    for (const auto& change: next.memory_changes) {
        memory_controller_.writeDoubleWord(change.address, change.new_bytes);
    }

    program_counter_ = next.new_pc;
    instructions_retired_++;
    cycle_s++;
    dumpRegisters(globals::registers_dump_file, registers_);
    std::cout << "Program Counter: " << program_counter_ << std::endl;
    undo_stack_.push(next);
    // current_delta_.register_changes.clear();
    // current_delta_.memory_changes.clear();
    // current_delta_.old_pc = 0;
    // current_delta_.new_pc = 0;
    // std::cout << "Redo step completed." << std::endl;



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

    return_address_ = 0;
    csr_target_address_ = 0;
    csr_old_value_ = 0;
    csr_write_val_ = 0;
    csr_uimm_ = 0;
    current_delta_.register_changes.clear();
    current_delta_.memory_changes.clear();
    current_delta_.old_pc = 0;
    current_delta_.new_pc = 0;
    undo_stack_ = std::stack<StepDelta>();
    redo_stack_ = std::stack<StepDelta>();

}
    
void RVSSVM::dumpState(const std::string &filename) {
    (void)filename;
}



