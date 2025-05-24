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

RVSSVM::RVSSVM() : VmBase() {
  DumpRegisters(globals::registers_dump_file, registers_);
}

RVSSVM::~RVSSVM() = default;

void RVSSVM::Fetch() {
  current_instruction_ = memory_controller_.ReadWord(program_counter_);
  UpdateProgramCounter(4);
}

void RVSSVM::Decode() {
  control_unit_.SetControlSignals(current_instruction_);
}

void RVSSVM::Execute() {
  uint8_t opcode = current_instruction_ & 0b1111111;
  uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

  if (instruction_set::isFInstruction(current_instruction_)) { // RV64 F
    ExecuteFloat();
    return;
  } else if (instruction_set::isDInstruction(current_instruction_)) {
    ExecuteDouble();
    return;
  } else if (opcode==0b1110011) {
    ExecuteCsr();
    return;
  }

  uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
  uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;

  int32_t imm = ImmGenerator(current_instruction_);

  uint64_t reg1_value = registers_.ReadGpr(rs1);
  uint64_t reg2_value = registers_.ReadGpr(rs2);

  bool overflow = false;

  if (control_unit_.GetAluSrc()) {
    reg2_value = static_cast<uint64_t>(static_cast<int64_t>(imm));
  }

  // std::cout << "Reg1 value: " << std::hex << reg1_value << std::dec << std::endl;
  // std::cout << "Reg2 value: " << std::hex << reg2_value << std::dec << std::endl;


  alu::AluOp aluOperation = control_unit_.GetAluSignal(current_instruction_, control_unit_.GetAluOp());
  std::tie(execution_result_, overflow) = alu_.execute(aluOperation, reg1_value, reg2_value);
  // std::cout << "Execution result: " << std::hex << execution_result_ << std::dec << std::endl;


  if (control_unit_.GetBranch()) {
    switch (funct3) {
      case 0b000: {// BEQ
        branch_flag_ = (execution_result_==0);
        break;
      }
      case 0b001: {// BNE
        branch_flag_ = (execution_result_!=0);
        break;
      }
      case 0b100: {// BLT
        branch_flag_ = (execution_result_==1);
        break;
      }
      case 0b101: {// BGE
        branch_flag_ = (execution_result_==0);
        break;
      }
      case 0b110: {// BLTU
        branch_flag_ = (execution_result_==1);
        break;
      }
      case 0b111: {// BGEU
        branch_flag_ = (execution_result_==0);
        break;
      }
    }
  }

  if (control_unit_.GetBranch() && branch_flag_) { // JAL, JALR
    next_pc_ = static_cast<int64_t>(program_counter_); // PC was already updated in Fetch()
    UpdateProgramCounter(-4);
    return_address_ = program_counter_ + 4;
    if (opcode==0b1100111) { // JALR
      UpdateProgramCounter((execution_result_ & ~1) - program_counter_);
    } else { // JAL
      UpdateProgramCounter(imm);
    }
  }

  if (opcode==0b0010111) { // AUIPC
    execution_result_ = static_cast<int64_t>(program_counter_) - 4 + (imm << 12);

  }
}

void RVSSVM::ExecuteFloat() {
  uint8_t opcode = current_instruction_ & 0b1111111;
  uint8_t funct3 = (current_instruction_ >> 12) & 0b111;
  uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
  uint8_t rm = funct3;
  uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
  uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
  uint8_t rs3 = (current_instruction_ >> 27) & 0b11111;

  uint8_t fcsr_status = 0;

  int32_t imm = ImmGenerator(current_instruction_);

  if (rm==0b111) {
    rm = registers_.ReadCsr(0x002);
  }

  uint64_t reg1_value = registers_.ReadFpr(rs1);
  uint64_t reg2_value = registers_.ReadFpr(rs2);
  uint64_t reg3_value = registers_.ReadFpr(rs3);

  if (funct7==0b1101000 || funct7==0b1111000 || opcode==0b0000111 || opcode==0b0100111) {
    reg1_value = registers_.ReadGpr(rs1);
  }

  if (control_unit_.GetAluSrc()) {
    reg2_value = static_cast<uint64_t>(static_cast<int64_t>(imm));
  }

  // std::cout << "Reg1 value: " << std::hex << reg1_value << std::dec << std::endl;
  // std::cout << "Reg2 value: " << std::hex << reg2_value << std::dec << std::endl;
  // std::cout << "Reg3 value: " << std::hex << reg3_value << std::dec << std::endl;



  alu::AluOp aluOperation = control_unit_.GetAluSignal(current_instruction_, control_unit_.GetAluOp());
  std::tie(execution_result_, fcsr_status) = alu::Alu::fpexecute(aluOperation, reg1_value, reg2_value, reg3_value, rm);
  // std::cout << "Execution result: " << std::hex << execution_result_ << std::dec << std::endl;

  registers_.WriteCsr(0x003, fcsr_status);
}

void RVSSVM::ExecuteDouble() {
  uint8_t opcode = current_instruction_ & 0b1111111;
  uint8_t funct3 = (current_instruction_ >> 12) & 0b111;
  uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
  uint8_t rm = funct3;
  uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
  uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
  uint8_t rs3 = (current_instruction_ >> 27) & 0b11111;

  uint8_t fcsr_status = 0;

  int32_t imm = ImmGenerator(current_instruction_);

  uint64_t reg1_value = registers_.ReadFpr(rs1);
  uint64_t reg2_value = registers_.ReadFpr(rs2);
  uint64_t reg3_value = registers_.ReadFpr(rs3);

  if (funct7==0b1101001 || funct7==0b1111001 || opcode==0b0000111 || opcode==0b0100111) {
    reg1_value = registers_.ReadGpr(rs1);
  }

  if (control_unit_.GetAluSrc()) {
    reg2_value = static_cast<uint64_t>(static_cast<int64_t>(imm));
  }

  alu::AluOp aluOperation = control_unit_.GetAluSignal(current_instruction_, control_unit_.GetAluOp());
  std::tie(execution_result_, fcsr_status) = alu::Alu::dfpexecute(aluOperation, reg1_value, reg2_value, reg3_value, rm);

}

void RVSSVM::ExecuteVector() {
  return;
}

void RVSSVM::ExecuteCsr() {
  uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
  uint16_t csr = (current_instruction_ >> 20) & 0xFFF;
  uint64_t csr_val = registers_.ReadCsr(csr);

  csr_target_address_ = csr;
  csr_old_value_ = csr_val;
  csr_write_val_ = registers_.ReadGpr(rs1);
  csr_uimm_ = rs1;
}

void RVSSVM::WriteMemory() {
  uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
  uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

  if (instruction_set::isFInstruction(current_instruction_)) { // RV64 F
    WriteMemoryFloat();
    return;
  } else if (instruction_set::isDInstruction(current_instruction_)) {
    WriteMemoryDouble();
    return;
  }

  if (control_unit_.GetMemRead()) {
    switch (funct3) {
      case 0b000: {// LB
        memory_result_ = static_cast<int8_t>(memory_controller_.ReadByte(execution_result_));
        break;
      }
      case 0b001: {// LH
        memory_result_ = static_cast<int16_t>(memory_controller_.ReadHalfWord(execution_result_));
        break;
      }
      case 0b010: {// LW
        memory_result_ = static_cast<int32_t>(memory_controller_.ReadWord(execution_result_));
        break;
      }
      case 0b011: {// LD
        memory_result_ = memory_controller_.ReadDoubleWord(execution_result_);
        break;
      }
      case 0b100: {// LBU
        memory_result_ = static_cast<uint8_t>(memory_controller_.ReadByte(execution_result_));
        break;
      }
      case 0b101: {// LHU
        memory_result_ = static_cast<uint16_t>(memory_controller_.ReadHalfWord(execution_result_));
        break;
      }
      case 0b110: {// LWU
        memory_result_ = static_cast<uint32_t>(memory_controller_.ReadWord(execution_result_));
        break;
      }
    }
  }

  uint64_t addr = execution_result_;
  uint64_t old_mem = memory_controller_.ReadDoubleWord(addr);

  if (control_unit_.GetMemWrite()) {
    switch (funct3) {
      case 0b000: {// SB
        memory_controller_.WriteByte(execution_result_, registers_.ReadGpr(rs2) & 0xFF);
        break;
      }
      case 0b001: {// SH
        memory_controller_.WriteHalfWord(execution_result_, registers_.ReadGpr(rs2) & 0xFFFF);
        break;
      }
      case 0b010: {// SW
        memory_controller_.WriteWord(execution_result_, registers_.ReadGpr(rs2) & 0xFFFFFFFF);
        break;
      }
      case 0b011: {// SD
        memory_controller_.WriteDoubleWord(execution_result_, registers_.ReadGpr(rs2) & 0xFFFFFFFFFFFFFFFF);
        break;
      }
    }
  }

  uint64_t new_mem = memory_controller_.ReadDoubleWord(addr);
  if (old_mem!=new_mem) {
    current_delta_.memory_changes.push_back({addr, old_mem, new_mem});
  }
}

void RVSSVM::WriteMemoryFloat() {
  uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;

  if (control_unit_.GetMemRead()) { // FLW
    memory_result_ = memory_controller_.ReadWord(execution_result_);
  }

  uint64_t addr = 0;
  uint64_t old_mem = 0;
  uint64_t new_mem = 0;

  if (control_unit_.GetMemWrite()) { // FSW
    addr = execution_result_;
    old_mem = memory_controller_.ReadDoubleWord(addr);
    uint32_t val = registers_.ReadFpr(rs2) & 0xFFFFFFFF;
    memory_controller_.WriteWord(execution_result_, val);
    new_mem = memory_controller_.ReadDoubleWord(addr);
  }

  if (old_mem!=new_mem) {
    current_delta_.memory_changes.push_back({addr, old_mem, new_mem});
  }
}

void RVSSVM::WriteMemoryDouble() {
  uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;

  if (control_unit_.GetMemRead()) {// FLD
    memory_result_ = memory_controller_.ReadDoubleWord(execution_result_);
  }

  uint64_t addr = 0;
  uint64_t old_mem = 0;
  uint64_t new_mem = 0;

  if (control_unit_.GetMemWrite()) {// FSD
    addr = execution_result_;
    old_mem = memory_controller_.ReadDoubleWord(addr);
    memory_controller_.WriteDoubleWord(execution_result_, registers_.ReadFpr(rs2));
    new_mem = memory_controller_.ReadDoubleWord(addr);
  }

  if (old_mem!=new_mem) {
    current_delta_.memory_changes.push_back({addr, old_mem, new_mem});
  }
}

void RVSSVM::WriteMemoryVector() {
  return;
}

void RVSSVM::WriteBack() {
  uint8_t opcode = current_instruction_ & 0b1111111;
  uint8_t rd = (current_instruction_ >> 7) & 0b11111;
  int32_t imm = ImmGenerator(current_instruction_);

  if (instruction_set::isFInstruction(current_instruction_)) { // RV64 F
    WriteBackFloat();
    return;
  } else if (instruction_set::isDInstruction(current_instruction_)) {
    WriteBackDouble();
    return;
  } else if (opcode==0b1110011) { // CSR opcode
    WriteBackCsr();
    return;
  }

  uint64_t old_reg = registers_.ReadGpr(rd);
  unsigned int reg_index = rd;
  unsigned int reg_type = 0; // 0 for GPR, 1 for CSR, 2 for FPR


  if (control_unit_.GetRegWrite() && rd!=0) { // Avoid writing to x0
    switch (opcode) {
      case 0b0110011: // R-Type
      case 0b0010011: // I-Type
      case 0b0010111: {// AUIPC
        registers_.WriteGpr(rd, execution_result_);
        break;
      }
      case 0b0000011: {// Load
        registers_.WriteGpr(rd, memory_result_);
        break;
      }
      case 0b1100111: // JALR
      case 0b1101111: {// JAL
        registers_.WriteGpr(rd, next_pc_);
        break;
      }
      case 0b0110111: {// LUI
        registers_.WriteGpr(rd, (imm << 12));
        break;
      }
      default: break;
    }
  }

  if (opcode==0b1101111) { // JAL
    // Updated in Execute()
  }
  if (opcode==0b1100111) { // JALR
    registers_.WriteGpr(rd, return_address_); // Write back to rs1
    // Updated in Execute()
  }

  uint64_t new_reg = registers_.ReadGpr(rd);
  if (old_reg!=new_reg) {
    current_delta_.register_changes.push_back({reg_index, reg_type, old_reg, new_reg});
  }

}

void RVSSVM::WriteBackFloat() {
  uint8_t opcode = current_instruction_ & 0b1111111;
  uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
  uint8_t rd = (current_instruction_ >> 7) & 0b11111;

  uint64_t old_reg = 0;
  unsigned int reg_index = rd;
  unsigned int reg_type = 2; // 0 for GPR, 1 for CSR, 2 for FPR
  uint64_t new_reg = 0;

  if (control_unit_.GetRegWrite() && rd!=0) {
    // write to GPR
    if (funct7==0b1010000
        || funct7==0b1100000
        || funct7==0b1110000) { // f(eq|lt|le).s, fcvt.(w|wu|l|lu).s
      old_reg = registers_.ReadGpr(rd);
      registers_.WriteGpr(rd, execution_result_);
      new_reg = execution_result_;
      reg_type = 0; // GPR

    }
      // write to FPR
    else if (opcode==0b0000111) {
      old_reg = registers_.ReadFpr(rd);
      registers_.WriteFpr(rd, memory_result_);
      new_reg = memory_result_;
      reg_type = 2; // FPR
    } else {
      old_reg = registers_.ReadFpr(rd);
      registers_.WriteFpr(rd, execution_result_);
      new_reg = execution_result_;
      reg_type = 2; // FPR
    }
  }

  if (old_reg!=new_reg) {
    current_delta_.register_changes.push_back({reg_index, reg_type, old_reg, new_reg});
  }
}

void RVSSVM::WriteBackDouble() {
  uint8_t opcode = current_instruction_ & 0b1111111;
  uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
  uint8_t rd = (current_instruction_ >> 7) & 0b11111;

  uint64_t old_reg = 0;
  unsigned int reg_index = rd;
  unsigned int reg_type = 2; // 0 for GPR, 1 for CSR, 2 for FPR
  uint64_t new_reg = 0;

  if (control_unit_.GetRegWrite() && rd!=0) {
    // write to GPR
    if (funct7==0b1010001
        || funct7==0b1100001
        || funct7==0b1110001) { // f(eq|lt|le).d, fcvt.(w|wu|l|lu).d
      old_reg = registers_.ReadGpr(rd);
      registers_.WriteGpr(rd, execution_result_);
      new_reg = execution_result_;
      reg_type = 0; // GPR
    }
      // write to FPR
    else if (opcode==0b0000111) {
      old_reg = registers_.ReadFpr(rd);
      registers_.WriteFpr(rd, memory_result_);
      new_reg = memory_result_;
      reg_type = 2; // FPR
    } else {
      old_reg = registers_.ReadFpr(rd);
      registers_.WriteFpr(rd, execution_result_);
      new_reg = execution_result_;
      reg_type = 2; // FPR
    }
  }

  if (old_reg!=new_reg) {
    current_delta_.register_changes.push_back({reg_index, reg_type, old_reg, new_reg});
  }

  return;
}

void RVSSVM::WriteBackVector() {
  return;
}

void RVSSVM::WriteBackCsr() {
  uint8_t rd = (current_instruction_ >> 7) & 0b11111;
  uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

  switch (funct3) {
    case 0b001: { // CSRRW
      if (rd!=0) registers_.WriteGpr(rd, csr_old_value_);
      registers_.WriteCsr(csr_target_address_, csr_write_val_);
      break;
    }
    case 0b010: { // CSRRS
      if (rd!=0) registers_.WriteGpr(rd, csr_old_value_);
      if (csr_write_val_!=0) {
        registers_.WriteCsr(csr_target_address_, csr_old_value_ | csr_write_val_);
      }
      break;
    }
    case 0b011: { // CSRRC
      if (rd!=0) registers_.WriteGpr(rd, csr_old_value_);
      if (csr_write_val_!=0) {
        registers_.WriteCsr(csr_target_address_, csr_old_value_ & ~csr_write_val_);
      }
      break;
    }
    case 0b101: { // CSRRWI
      if (rd!=0) registers_.WriteGpr(rd, csr_old_value_);
      registers_.WriteCsr(csr_target_address_, csr_uimm_);
      break;
    }
    case 0b110: { // CSRRSI
      if (rd!=0) registers_.WriteGpr(rd, csr_old_value_);
      if (csr_uimm_!=0) {
        registers_.WriteCsr(csr_target_address_, csr_old_value_ | csr_uimm_);
      }
      break;
    }
    case 0b111: { // CSRRCI
      if (rd!=0) registers_.WriteGpr(rd, csr_old_value_);
      if (csr_uimm_!=0) {
        registers_.WriteCsr(csr_target_address_, csr_old_value_ & ~csr_uimm_);
      }
      break;
    }
  }

}

void RVSSVM::Run() {
  while (program_counter_ < program_size_) {
    Fetch();
    Decode();
    Execute();
    WriteMemory();
    WriteBack();
    instructions_retired_++;
    cycle_s_++;
  }
  // DumpRegisters(globals::registers_dump_file, registers_.getRegisters());
}

void RVSSVM::DebugRun() {
  while (program_counter_ < program_size_) {
    if (std::find(breakpoints_.begin(), breakpoints_.end(), program_counter_)==breakpoints_.end()) {
      Fetch();
      Decode();
      Execute();
      WriteMemory();
      WriteBack();
      instructions_retired_++;
      cycle_s_++;
    } else {
      std::cout << "Breakpoint hit at address: " << program_counter_ << std::endl;
      break;
    }
  }
}

void RVSSVM::Step() {
  current_delta_.old_pc = program_counter_;
  if (program_counter_ < program_size_) {
    Fetch();
    Decode();
    Execute();
    WriteMemory();
    WriteBack();
    instructions_retired_++;
    cycle_s_++;
    DumpRegisters(globals::registers_dump_file, registers_);
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
  } else if (program_counter_==program_size_) {
    std::cout << "Program Counter reached end of program: " << program_counter_ << std::endl;
  } else {
    std::cout << "Program Counter out of bounds: " << program_counter_ << std::endl;
    std::cout << "Program size: " << program_size_ << std::endl;
  }
}

void RVSSVM::Undo() {
  if (undo_stack_.empty()) {
    std::cout << "No more Undo steps available." << std::endl;
    return;
  }

  StepDelta last = undo_stack_.top();
  undo_stack_.pop();
  for (const auto &change : last.register_changes) {
    switch (change.reg_type) {
      case 0: { // GPR
        registers_.WriteGpr(change.reg_index, change.old_value);
        break;
      }
      case 1: { // CSR
        registers_.WriteCsr(change.reg_index, change.old_value);
        break;
      }
      case 2: { // FPR
        registers_.WriteFpr(change.reg_index, change.old_value);
        break;
      }
      default:std::cerr << "Invalid register type: " << change.reg_type << std::endl;
        break;
    }
  }

  for (const auto &change : last.memory_changes) {
    memory_controller_.WriteDoubleWord(change.address, change.old_bytes);
  }

  program_counter_ = last.old_pc;
  instructions_retired_--;
  cycle_s_--;
  DumpRegisters(globals::registers_dump_file, registers_);
  std::cout << "Program Counter: " << program_counter_ << std::endl;

  redo_stack_.push(last);
  // current_delta_.register_changes.clear();
  // current_delta_.memory_changes.clear();
  // current_delta_.old_pc = 0;
  // current_delta_.new_pc = 0;



}

void RVSSVM::Redo() {
  if (redo_stack_.empty()) {
    std::cout << "No more Redo steps available." << std::endl;
    return;
  }

  StepDelta next = redo_stack_.top();
  redo_stack_.pop();

  for (const auto &change : next.register_changes) {
    switch (change.reg_type) {
      case 0: { // GPR
        registers_.WriteGpr(change.reg_index, change.new_value);
        break;
      }
      case 1: { // CSR
        registers_.WriteCsr(change.reg_index, change.new_value);
        break;
      }
      case 2: { // FPR
        registers_.WriteFpr(change.reg_index, change.new_value);
        break;
      }
      default:std::cerr << "Invalid register type: " << change.reg_type << std::endl;
        break;
    }
  }

  for (const auto &change : next.memory_changes) {
    memory_controller_.WriteDoubleWord(change.address, change.new_bytes);
  }

  program_counter_ = next.new_pc;
  instructions_retired_++;
  cycle_s_++;
  DumpRegisters(globals::registers_dump_file, registers_);
  std::cout << "Program Counter: " << program_counter_ << std::endl;
  undo_stack_.push(next);
  // current_delta_.register_changes.clear();
  // current_delta_.memory_changes.clear();
  // current_delta_.old_pc = 0;
  // current_delta_.new_pc = 0;
  // std::cout << "Redo Step completed." << std::endl;



}

void RVSSVM::Reset() {
  program_counter_ = 0;
  instructions_retired_ = 0;
  cycle_s_ = 0;
  registers_.Reset();
  memory_controller_.Reset();
  control_unit_.Reset();
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

void RVSSVM::DumpState(const std::string &filename) {
  (void) filename;
}



