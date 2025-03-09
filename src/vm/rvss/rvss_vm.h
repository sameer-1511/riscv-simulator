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

class RVSSVM : public VMBase {
private:
public: // TODO: take public down
    RVSSControlUnit controlUnit;


    // intermediate variables
    int64_t execution_result_ = 0;
    int64_t memory_result_ = 0;
    int64_t memory_address_ = 0;
    int64_t memory_data_ = 0;
    
    double fexecution_result_ = 0.0;
    double fmemory_result_ = 0.0;
    // same as memory_address_
    double fmemory_data_ = 0.0;

    bool branch_flag_ = false;


    void fetch();

    void decode();

    void execute();
    void executeFloat();
    void executeVector();

    void memory();
    void memoryFloat();
    void memoryVector();

    void writeBack();
    void writeBackFloat();
    void writeBackVector();

    void printControlSignals();
    // TODO: bring public here
    RVSSVM();
    ~RVSSVM();

    void run() override;
    void debugRun() override;
    void step() override;
    void reset() override;
    void dumpState(const std::string &filename) override;

    void printType() {
        std::cout << "rvssvm" << std::endl;
    }

};


#endif // RVSS_VM_H
