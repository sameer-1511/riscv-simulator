/**
 * @file vm_runner.h
 * @brief This file contains the declaration of the VMRunner class
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef VM_RUNNER_H
#define VM_RUNNER_H

#include "pch.h"

#include "vm/vm_base.h"
#include "vm/rvss/rvss_vm.h"
#include "config.h"
#include "vm_asm_mw.h"

inline std::unique_ptr<VMBase> createVM(vmConfig::vmTypes vmType) {
    if (vmType == vmConfig::vmTypes::SINGLE_STAGE) {
        return std::make_unique<RVSSVM>();
    }

    return nullptr;
}

class VMRunner {
    std::unique_ptr<VMBase> vm_;
public:
    VMRunner() {
        vmConfig::vmTypes vmType = vmConfig::getVMType();
        vm_ = createVM(vmType);
    }
    
    ~VMRunner() = default;

    void loadProgram(const AssembledProgram &program) {
        vm_->loadProgram(program);
    }

    void custom() {
        if (auto vmInstance = dynamic_cast<RVSSVM*>(vm_.get())) {
            vmInstance->printType();
        } else {
            throw std::runtime_error("VM not initialized.");
        }
    }

    void step() {
        vm_->step();
    }





};

#endif // VM_RUNNER_H
