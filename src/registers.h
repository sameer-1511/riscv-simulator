/**
 * File Name: registers.h
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
#ifndef REGISTERS_H
#define REGISTERS_H

#include "pch.h"

enum class RegisterType {
    INVALID,
    INTEGER,
    FLOATING_POINT,
    VECTOR,
};

extern const std::unordered_set<std::string> valid_registers;

extern const std::unordered_map<std::string, std::string> reg_alias_to_name;

bool isValidRegister(const std::string& reg);

#endif // REGISTERS_H