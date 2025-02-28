/**
 * @file rvss_vm.cpp
 * @brief RVSS VM implementation
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "rvss_vm.h"

RVSSVM::RVSSVM() : VMBase() {
    // Initialize control signals
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
    // uint8_t funct3 = (current_instruction_ >> 12) & 0b111;
    // uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;

    if (opcode == 0x53) { // RV32F & RV64F
        executeFloat();
        return;
    } // TODO: add vector support

    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;

    int32_t imm = imm_generator(current_instruction_);

    int32_t reg1 = registers_.readGPR(rs1);
    int32_t reg2 = registers_.readGPR(rs2);

    int64_t result = 0;
    bool overflow = false;

    
    if (controlUnit.getALUSrc()) {
        reg2 = imm;
    }

    ALU::ALUOp aluOp = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, overflow) = alu_.execute<int64_t>(aluOp, reg1, reg2);

    
    // START HERE
    memory_address_ = execution_result_;

    if (controlUnit.getMemRead() || controlUnit.getMemWrite()) {
        memory_address_ = execution_result_;
    } else {
        if (controlUnit.getRegWrite()) {
            registers_.writeGPR(rd, execution_result_);
        }
    }

}

void RVSSVM::executeFloat() {
    // if (controlUnit.getALUSrc()) {
    //     freg2 = imm;
    // }

    // ALU::ALUOp aluOp = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    // std::tie(fexecution_result_, overflow) = alu_.execute<_Float32>(aluOp, freg1, freg2);
}

void RVSSVM::executeVector() {
    return;
}

void RVSSVM::memory() {
    // if (controlUnit.getMemRead()) {
    //     memory_result_ = memory_controller_.readWord(memory_address_);
    // }
    // if (controlUnit.getMemWrite()) {
    //     memory_controller_.writeWord(memory_address_, memory_data_);
    // }

    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;

    if (controlUnit.getMemRead()) {
        registers_.writeGPR(rd, memory_controller_.readWord(execution_result_));
    } 
    if (controlUnit.getMemWrite()) {
        memory_controller_.writeWord(execution_result_, registers_.readGPR(rs2));
    }

}

void RVSSVM::writeback() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    int32_t imm = imm_generator(current_instruction_);

    // TODO: Implement floating point support

    if (controlUnit.getRegWrite() && rd != 0) { // Avoid writing to x0
        registers_.writeGPR(rd, execution_result_);
    }

    if (opcode == 0x6F) { // JAL
        registers_.writeGPR(rd, program_counter_ + 4);
        updateProgramCounter(imm - 4);
    }
    if (opcode == 0x67) { // JALR
        registers_.writeGPR(rd, program_counter_ + 4);
        updateProgramCounter((rs1 + imm) & ~1); // Ensure LSB is 0
    }

}




void RVSSVM::run() {
    while(program_counter_ < program_size_) {
        fetch();
        decode();
        execute();
        memory();
        writeback();
    }
}

void RVSSVM::debugRun() {
    while (program_counter_ < program_size_) {
        if (std::find(breakpoints_.begin(), breakpoints_.end(), program_counter_) == breakpoints_.end()) {
            fetch();
            decode();
            execute();
            memory();
            writeback();
        } else {
            // TODO:: handle breakpoints
            break;
        }
    }
}

void RVSSVM::step() {
    fetch();
    decode();
    execute();
    memory();
    writeback();
}

void RVSSVM::reset() {
    
}
void RVSSVM::dumpState(const std::string &filename) {
    (void)filename;

}



