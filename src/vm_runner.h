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

std::unique_ptr<VMBase> createVM(const std::string& vmType) {
    if (vmType == "single_cycle") {
        // return std::make_unique<RVSSVM>();
    } else {
        throw std::invalid_argument("Unknown VM type: " + vmType);
    }

    return nullptr;
}

class VMRunner {
    std::unique_ptr<VMBase> vm_;
public:
    VMRunner();
    ~VMRunner();

    void initialize(const std::string& configPath) {
        (void)configPath;
        std::string vmType = "single_cycle";
        vm_ = createVM(vmType);
    }

    void run() {
        //if (auto vmInstance = dynamic_cast<RVSSVM*>(vm.get())) {
        //    vmInstance->printType();
        //} else {
        //    throw std::runtime_error("VM not initialized.");
        //}
        
    }
 


    

};


#endif // VM_RUNNER_H
