/**
 * @file rvss_vm.h
 * @brief RVSS VM definition
 * @author Vishank Singh, https://github.com/VishankSingh
 */
#ifndef RVSS_VM_H
#define RVSS_VM_H

#include "../../pch.h"

#include "../vm_base.h"

#include "rvss_control_unit.h"

struct RegisterChange {
  unsigned int reg_index;
  unsigned int reg_type; // 0 for GPR, 1 for CSR, 2 for FPR
  uint64_t old_value;
  uint64_t new_value;
};

struct MemoryChange {
  uint64_t address;
  uint64_t old_bytes;
  uint64_t new_bytes;
};

struct StepDelta {
  uint64_t old_pc;
  uint64_t new_pc;
  std::vector<RegisterChange> register_changes;
  std::vector<MemoryChange> memory_changes;
};

class RVSSVM : public VmBase {
 public:
  RVSSControlUnit control_unit_;

  std::stack<StepDelta> undo_stack_;
  std::stack<StepDelta> redo_stack_;
  StepDelta current_delta_;

  // intermediate variables
  int64_t execution_result_{};
  int64_t memory_result_{};
  // int64_t memory_address_{};
  // int64_t memory_data_{};
  uint64_t return_address_{};

  bool branch_flag_ = false;
  int64_t next_pc_{}; // for jal, jalr,

  // CSR intermediate variables
  uint16_t csr_target_address_{};
  uint64_t csr_old_value_{};
  uint64_t csr_write_val_{};
  uint8_t csr_uimm_{};

  void Fetch();

  void Decode();

  void Execute();
  void ExecuteFloat();
  void ExecuteDouble();
  void ExecuteVector();
  void ExecuteCsr();

  void WriteMemory();
  void WriteMemoryFloat();
  void WriteMemoryDouble();
  void WriteMemoryVector();

  void WriteBack();
  void WriteBackFloat();
  void WriteBackDouble();
  void WriteBackVector();
  void WriteBackCsr();

  RVSSVM();
  ~RVSSVM();

  void Run() override;
  void DebugRun() override;
  void Step() override;
  void Undo() override;
  void Redo() override;
  void Reset() override;
  void DumpState(const std::string &filename) override;

  void PrintType() {
    std::cout << "rvssvm" << std::endl;
  }
};

#endif // RVSS_VM_H
