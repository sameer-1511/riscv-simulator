/**
 * File Name: i_m_formats.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "../parser.h"
#include "../instructions.h"
#include "../../vm/registers.h"
#include "../../utils.h"

bool Parser::parse_O_GPR_C_GPR_C_GPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::GP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::GP_REGISTER
        && (peekToken(6).type == TokenType::EOF_ || peekToken(6).line_number != currentToken().line_number)
            ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);

        std::string reg; 
        reg = reg_alias_to_name.at(peekToken(1).value);
        block.setRd(reg);
        reg = reg_alias_to_name.at(peekToken(3).value);
        block.setRs1(reg);
        reg = reg_alias_to_name.at(peekToken(5).value);
        block.setRs2(reg);

        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_GPR_C_I() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::GP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::NUM
        && (peekToken(6).type == TokenType::EOF_ || peekToken(6).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;

        
        if (InstructionSet::isValidITypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            reg = reg_alias_to_name.at(peekToken(3).value);
            block.setRs1(reg);
            int64_t imm = std::stoll(peekToken(5).value);
            if (-2048 <= imm && imm <= 2047) {
                block.setImm(std::to_string(imm));
            } else {
                errors_.count++;
                recordError(ParseError(peekToken(5).line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -2048 <= imm <= 2047", filename_, peekToken(5).line_number, peekToken(5).column_number, getLineFromFile(filename_, peekToken(5).line_number)));
                skipCurrentLine();
                return true;
            }
        } else if (InstructionSet::isValidBTypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRs1(reg);
            reg = reg_alias_to_name.at(peekToken(3).value);
            block.setRs2(reg);
            int64_t imm = std::stoll(peekToken(5).value);
            if (-4096 <= imm && imm <= 4095) {
                if (imm % 4 == 0) {
                    block.setImm(std::to_string(imm));
                } else {
                    errors_.count++;
                    recordError(ParseError(peekToken(5).line_number, "Misaligned immediate value"));
                    errors_.all_errors.emplace_back(Errors::MisalignedImmediateError("Misaligned immediate value", "Expected: imm % 4 == 0", filename_, peekToken(5).line_number, peekToken(5).column_number, getLineFromFile(filename_, peekToken(5).line_number)));
                    skipCurrentLine();
                    return true;
                }
            } else {
                errors_.count++;
                recordError(ParseError(peekToken(5).line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -4096 <= imm <= 4095", filename_, peekToken(5).line_number, peekToken(5).column_number, getLineFromFile(filename_, peekToken(5).line_number)));
                skipCurrentLine();
                return true;
            }
        }
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_I() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::NUM
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;

        if (InstructionSet::isValidUTypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            int64_t imm = std::stoll(peekToken(3).value);
            if (0 <= imm && imm <= 1048575) {
                block.setImm(std::to_string(imm));
            } else {
                errors_.count++;
                recordError(ParseError(peekToken(3).line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: 0 <= imm <= 1048575", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                skipCurrentLine();
                return true;
            }
        } else if (InstructionSet::isValidJTypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            int64_t imm = std::stoll(peekToken(3).value);
            if (-1048576 <= imm && imm <= 1048575) {
                if (imm % 2 == 0) {
                    block.setImm(std::to_string(imm));
                } else {
                    errors_.count++;
                    recordError(ParseError(peekToken(3).line_number, "Misaligned immediate value"));
                    errors_.all_errors.emplace_back(Errors::MisalignedImmediateError("Misaligned immediate value", "Expected: imm % 4 == 0", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                    skipCurrentLine();
                    return true;
                }
            } else {
                errors_.count++;
                recordError(ParseError(peekToken(3).line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -1048576 <= imm <= 1048575", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                skipCurrentLine();
                return true;
            }
        }
        
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_GPR_C_IL() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::GP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::LABEL_REF
        && (peekToken(6).type == TokenType::EOF_ || peekToken(6).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;

        if (InstructionSet::isValidBTypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRs1(reg);
            reg = reg_alias_to_name.at(peekToken(3).value);
            block.setRs2(reg);
            if (symbol_table_.find(peekToken(5).value) != symbol_table_.end()
                && !symbol_table_[peekToken(5).value].isData) {
                uint64_t address = symbol_table_[peekToken(5).value].address;
                auto offset = static_cast<int64_t>(address - instruction_index_ * 4);
                if (-4096 <= offset && offset <= 4095) {
                    block.setImm(std::to_string(offset));
                    block.setLabel(peekToken(5).value);
                } else {
                    errors_.count++;
                    recordError(ParseError(peekToken(5).line_number, "Immediate value out of range"));
                    errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -4096 <= imm <= 4095", filename_, peekToken(5).line_number, peekToken(5).column_number, getLineFromFile(filename_, peekToken(5).line_number)));
                    skipCurrentLine();
                    return true;
                }
            } else {
                backPatch.push_back(instruction_index_);
                block.setImm(peekToken(5).value);
                IntermediateCode.emplace_back(block, false);
                instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
                instruction_index_++;
                skipCurrentLine();
                return true;
            }
        }
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_GPR_C_DL() {
    return true;
}

bool Parser::parse_O_GPR_C_IL() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::LABEL_REF
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        if (InstructionSet::isValidJTypeInstruction(block.getOpcode())) {
            std::string reg;
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            if (symbol_table_.find(peekToken(3).value) != symbol_table_.end()
                && !symbol_table_[peekToken(3).value].isData) {
                uint64_t address = symbol_table_[peekToken(3).value].address;
                auto offset = static_cast<int64_t>(address - instruction_index_ * 4);
                if (-1048576 <= offset && offset <= 1048575) {
                    block.setImm(std::to_string(offset));
                    block.setLabel(peekToken(3).value);
                } else {
                    errors_.count++;
                    recordError(ParseError(peekToken(3).line_number, "Immediate value out of range"));
                    errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -1048576 <= imm <= 1048575", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                    skipCurrentLine();
                    return true;
                }
            } else {
                backPatch.push_back(instruction_index_);
                block.setImm(peekToken(3).value);
                IntermediateCode.emplace_back(block, false);
                instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
                instruction_index_++;
                return true;
            }
        }
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_DL() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::LABEL_REF
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        if (InstructionSet::isValidITypeInstruction(block.getOpcode())) {
            std::string reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            if (symbol_table_.find(peekToken(3).value) != symbol_table_.end() &&
                symbol_table_[peekToken(3).value].isData) {
                uint64_t address = symbol_table_[peekToken(3).value].address;
                auto offset = static_cast<int64_t>(address);
                if (-2048 <= offset && offset <= 2047) {
                    block.setImm(std::to_string(offset));
                } else {
                    errors_.count++;
                    recordError(ParseError(peekToken(3).line_number, "Immediate value out of range"));
                    errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -2048 <= imm <= 2047", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                    skipCurrentLine();
                    return true;
                }
            } else {
                backPatch.push_back(instruction_index_);
                block.setImm(peekToken(3).value);
                IntermediateCode.emplace_back(block, false);
                instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
                instruction_index_++;
                skipCurrentLine();
                return true;
            }
        }
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_I_LP_GPR_RP() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::NUM
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::LPAREN
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::GP_REGISTER
        && peekToken(6).line_number == currentToken().line_number
        && peekToken(6).type == TokenType::RPAREN
        && (peekToken(7).type == TokenType::EOF_ || peekToken(7).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;
        if (InstructionSet::isValidITypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            int64_t imm = std::stoll(peekToken(3).value);
            if (-2048 <= imm && imm <= 2047) {
                block.setImm(std::to_string(imm));
            } else {
                errors_.count++;
                recordError(ParseError(peekToken(3).line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -2048 <= imm <= 2047", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                skipCurrentLine();
                return true;
            }
            reg = reg_alias_to_name.at(peekToken(5).value);
            block.setRs1(reg);
        } else if (InstructionSet::isValidSTypeInstruction(block.getOpcode())) {
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRs2(reg);
            int64_t imm = std::stoll(peekToken(3).value);
            if (-2048 <= imm && imm <= 2047) {
                block.setImm(std::to_string(imm));
            } else {
                errors_.count++;
                recordError(ParseError(peekToken(3).line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -2048 <= imm <= 2047", filename_, peekToken(3).line_number, peekToken(3).column_number, getLineFromFile(filename_, peekToken(3).line_number)));
                skipCurrentLine();
                return true;
            }
            reg = reg_alias_to_name.at(peekToken(5).value);
            block.setRs1(reg);
        }
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

