/**
 * File Name: f_d_formats.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "../parser.h"
#include "../instructions.h"
#include "../../vm/registers.h"
#include "../../utils.h"
#include "../../common/rounding_modes.h"

bool Parser::parse_O_FPR_C_FPR_C_FPR_C_FPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::FP_REGISTER
        && peekToken(6).line_number == currentToken().line_number
        && peekToken(6).type == TokenType::COMMA
        && peekToken(7).line_number == currentToken().line_number
        && peekToken(7).type == TokenType::FP_REGISTER
        && (peekToken(8).type == TokenType::EOF_ || peekToken(8).line_number != currentToken().line_number)
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
        reg = reg_alias_to_name.at(peekToken(7).value);
        block.setRs3(reg);
        block.setRm(0b111);
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_FPR_C_FPR_C_FPR_C_RM() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::FP_REGISTER
        && peekToken(6).line_number == currentToken().line_number
        && peekToken(6).type == TokenType::COMMA
        && peekToken(7).line_number == currentToken().line_number
        && peekToken(7).type == TokenType::FP_REGISTER
        && peekToken(8).line_number == currentToken().line_number
        && peekToken(8).type == TokenType::COMMA
        && peekToken(9).line_number == currentToken().line_number
        && peekToken(9).type == TokenType::RM
        && (peekToken(10).type == TokenType::EOF_ || peekToken(10).line_number != currentToken().line_number)
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
        reg = reg_alias_to_name.at(peekToken(7).value);
        block.setRs3(reg);
        
        std::string rm = peekToken(9).value;
        uint8_t rmEncoding = getRoundingModeEncoding(rm);
        block.setRm(rmEncoding);

        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_FPR_C_FPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::FP_REGISTER
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
        block.setRm(0b111);
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_FPR_C_FPR_C_RM() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::FP_REGISTER
        && peekToken(6).line_number == currentToken().line_number
        && peekToken(6).type == TokenType::COMMA
        && peekToken(7).line_number == currentToken().line_number
        && peekToken(7).type == TokenType::RM
        && (peekToken(8).type == TokenType::EOF_ || peekToken(8).line_number != currentToken().line_number)
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
        
        std::string rm = peekToken(7).value;
        uint8_t rmEncoding = getRoundingModeEncoding(rm);
        block.setRm(rmEncoding);



        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_FPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;
        reg = reg_alias_to_name.at(peekToken(1).value);
        block.setRd(reg);
        reg = reg_alias_to_name.at(peekToken(3).value);
        block.setRs1(reg);
        block.setRm(0b111);
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_FPR_C_RM() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::RM
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
        
        std::string rm = peekToken(5).value;
        uint8_t rmEncoding = getRoundingModeEncoding(rm);
        block.setRm(rmEncoding);

        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_GPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::GP_REGISTER
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;
        reg = reg_alias_to_name.at(peekToken(1).value);
        block.setRd(reg);
        reg = reg_alias_to_name.at(peekToken(3).value);
        block.setRs1(reg);
        block.setRm(0b111);
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_FPR_C_GPR_C_RM() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::GP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::RM
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
        
        std::string rm = peekToken(5).value;
        uint8_t rmEncoding = getRoundingModeEncoding(rm);
        block.setRm(rmEncoding);

        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_FPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        block.setLineNumber(currentToken().line_number);
        std::string reg;
        reg = reg_alias_to_name.at(peekToken(1).value);
        block.setRd(reg);
        reg = reg_alias_to_name.at(peekToken(3).value);
        block.setRs1(reg);
        block.setRm(0b111);
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_FPR_C_RM() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::RM
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
        
        std::string rm = peekToken(5).value;
        uint8_t rmEncoding = getRoundingModeEncoding(rm);
        block.setRm(rmEncoding);

        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_O_GPR_C_FPR_C_FPR() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::FP_REGISTER
        && peekToken(4).line_number == currentToken().line_number
        && peekToken(4).type == TokenType::COMMA
        && peekToken(5).line_number == currentToken().line_number
        && peekToken(5).type == TokenType::FP_REGISTER
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

bool Parser::parse_O_FPR_C_I_LP_GPR_RP() {
    if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::FP_REGISTER
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

        if (InstructionSet::isValidFDITypeInstruction(block.getOpcode())) {
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
        } else if (InstructionSet::isValidFDSTypeInstruction(block.getOpcode())) {
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