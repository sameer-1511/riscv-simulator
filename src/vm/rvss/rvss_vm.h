/**
 * File Name: rvss_vm.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef RVSS_VM_H
#define RVSS_VM_H

#include "../../pch.h"

#include "../vm_base.h"

#include "rvss_control_unit.h"

class RVSSVM : public VMBase {
private:
    RVSSControlUnit controlUnit;

    std::unordered_map<std::string, int> controlSignals;

    void fetchInstruction();
    void decodeInstruction();
    void executeInstruction();
    void memoryAccess();
    void writeback();

    void printControlSignals();
    
public:
    RVSSVM();
    ~RVSSVM();

    void run() override;
    void step() override;
    void reset() override;

    void printType() {
        std::cout << "rvssvm" << std::endl;
    }

};


#endif // RVSS_VM_H
