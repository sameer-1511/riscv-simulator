/**
 * File Name: rvss_vm.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef RVSS_VM_H
#define RVSS_VM_H

#include "../../pch.h"

#include "../vm_base.h"

class RVSSVM : public VMBase {
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
