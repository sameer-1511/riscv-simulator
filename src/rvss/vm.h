/**
 * File Name: vm.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef VM_H
#define VM_H

#include "../pch.h"

class rvss_vm {
    
public:
    rvss_vm();
    ~rvss_vm();

    void run(const std::string &filename);
};

#endif // VM_H
