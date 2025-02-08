/**
 * @file vm_runner.h
 * @brief This file contains the declaration of the VMRunner class
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef VM_RUNNER_H
#define VM_RUNNER_H

#include "pch.h"

#include "vm/vm_base.h"
//#include "vm/rvss/rvss_vm.h"
#include "config.h"

std::unique_ptr<VMBase> createVM(vmConfig::vmTypes vmType) {
    if (vmType == vmConfig::vmTypes::SINGLE_STAGE) {
        // return std::make_unique<RVSSVM>();
    }

    return nullptr;
}

class VMRunner {
    std::unique_ptr<VMBase> vm_;
public:
    VMRunner();
    ~VMRunner();

    void initialize() {
        vmConfig::vmTypes vmType = vmConfig::getVMType();
        vm_ = createVM(vmType);
    }

    // void run() {
    //     if (auto vmInstance = dynamic_cast<RVSSVM*>(vm.get())) {
    //         vmInstance->printType();
    //     } else {
    //         throw std::runtime_error("VM not initialized.");
    //     }
    //     
    // }
 


    

};


#endif // VM_RUNNER_H
