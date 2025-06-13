/**
 * @file pseudo_formats.cpp
 * @brief 
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "assembler/parser.h"
#include "common/instructions.h"
#include "vm/registers.h"
#include "utils.h"

#include <string>

bool Parser::parse_pseudo() {
  if (currentToken().value=="la") {
    if (peekToken(1).line_number==currentToken().line_number
        && peekToken(1).type==TokenType::GP_REGISTER
        && peekToken(2).line_number==currentToken().line_number
        && peekToken(2).type==TokenType::COMMA
        && peekToken(3).line_number==currentToken().line_number
        && peekToken(3).type==TokenType::LABEL_REF
        && (peekToken(4).type==TokenType::EOF_ || peekToken(4).line_number!=currentToken().line_number)
        ) {
      if (symbol_table_.find(peekToken(3).value)!=symbol_table_.end()
          && symbol_table_[peekToken(3).value].isData) {
        // TODO: Implement la pseudo instruction
      }
      skipCurrentLine();
      instruction_index_++;
      return true;
    }
    return false;
  }

    // nop
  else if (currentToken().value=="nop") {
    if (peekToken(1).type==TokenType::EOF_
        || peekToken(1).line_number!=currentToken().line_number) {
      ICUnit block;
      block.setOpcode(currentToken().value);
      block.setLineNumber(currentToken().line_number);
      block.setOpcode("addi");
      block.setRd("x0");
      block.setRs1("x0");
      block.setRs2("x0");
      block.setImm("0");
      intermediate_code_.emplace_back(block, true);
      instruction_number_line_number_mapping_[instruction_index_] = block.getLineNumber();
      instruction_index_++;
      nextToken();
      return true;
    }
    return false;
  }
    // li
  else if (currentToken().value=="li") {
    if (peekToken(1).line_number==currentToken().line_number
        && peekToken(1).type==TokenType::GP_REGISTER
        && peekToken(2).line_number==currentToken().line_number
        && peekToken(2).type==TokenType::COMMA
        && peekToken(3).line_number==currentToken().line_number
        && peekToken(3).type==TokenType::NUM
        &&
            (peekToken(4).type==TokenType::EOF_ || peekToken(4).line_number!=currentToken().line_number)) {
      ICUnit block;
      block.setOpcode(currentToken().value);
      int64_t imm = std::stoll(peekToken(3).value);
      if (-2048 <= imm && imm <= 2047) {
        block.setLineNumber(currentToken().line_number);
        block.setOpcode("addi");
        std::string reg;
        reg = reg_alias_to_name.at(peekToken(1).value);
        block.setRd(reg);
        block.setRs1("x0");
        block.setImm(peekToken(3).value);
        intermediate_code_.emplace_back(block, true);
        instruction_number_line_number_mapping_[instruction_index_] = block.getLineNumber();
        instruction_index_++;
      } else if (-1048576 <= imm && imm <= 1048575) {
        int64_t upper = (imm + (1 << 11)) >> 12;
        int64_t lower = imm - (upper << 12);

        // Emit lui instruction
        ICUnit luiBlock;
        luiBlock.setLineNumber(currentToken().line_number);
        luiBlock.setOpcode("lui");
        std::string reg;
        reg = reg_alias_to_name.at(peekToken(1).value);
        luiBlock.setRd(reg);
        luiBlock.setImm(std::to_string(upper));
        intermediate_code_.emplace_back(luiBlock, true);
        instruction_number_line_number_mapping_[instruction_index_] = luiBlock.getLineNumber();
        instruction_index_++;

        // Emit addi instruction (if lower part is non-zero)
        if (lower!=0) {
          ICUnit addiBlock;
          addiBlock.setLineNumber(currentToken().line_number);
          addiBlock.setOpcode("addi");
          addiBlock.setRd(reg);
          addiBlock.setRs1(reg);
          addiBlock.setImm(std::to_string(lower));
          intermediate_code_.emplace_back(addiBlock, true);
          instruction_number_line_number_mapping_[instruction_index_] = addiBlock.getLineNumber();
          instruction_index_++;
        }
      } else {
        errors_.count++;
        recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
        errors_.all_errors.emplace_back(errors::ImmediateOutOfRangeError("Immediate value out of range",
                                                                         "Expected: -1048576 <= imm <= 1048575",
                                                                         filename_,
                                                                         currentToken().line_number,
                                                                         currentToken().column_number,
                                                                         GetLineFromFile(filename_,
                                                                                         currentToken().line_number)));
      }
      skipCurrentLine();
      return true;
    }
    return false;
  }
    // mv
  else if (currentToken().value=="mv") {
    if (peekToken(1).line_number==currentToken().line_number
        && peekToken(1).type==TokenType::GP_REGISTER
        && peekToken(2).line_number==currentToken().line_number
        && peekToken(2).type==TokenType::COMMA
        && peekToken(3).line_number==currentToken().line_number
        && peekToken(3).type==TokenType::GP_REGISTER
        &&
            (peekToken(4).type==TokenType::EOF_ || peekToken(4).line_number!=currentToken().line_number)) {
      ICUnit block;
      block.setOpcode("add");
      block.setLineNumber(currentToken().line_number);
      std::string reg;
      reg = reg_alias_to_name.at(peekToken(1).value);
      block.setRd(reg);
      reg = reg_alias_to_name.at(peekToken(3).value);
      block.setRs1(reg);
      block.setRs2("x0");
      intermediate_code_.emplace_back(block, true);
      instruction_number_line_number_mapping_[instruction_index_] = block.getLineNumber();
      instruction_index_++;
      skipCurrentLine();
      return true;
    }
    return false;
  }
    // not
  else if (currentToken().value=="not") {
    if (peekToken(1).line_number==currentToken().line_number
        && peekToken(1).type==TokenType::GP_REGISTER
        && peekToken(2).line_number==currentToken().line_number
        && peekToken(2).type==TokenType::COMMA
        && peekToken(3).line_number==currentToken().line_number
        && peekToken(3).type==TokenType::GP_REGISTER
        &&
            (peekToken(4).type==TokenType::EOF_ || peekToken(4).line_number!=currentToken().line_number)) {
      ICUnit block;
      block.setOpcode("xori");
      block.setLineNumber(currentToken().line_number);
      std::string reg = reg_alias_to_name.at(peekToken(1).value);
      block.setRd(reg);
      reg = reg_alias_to_name.at(peekToken(3).value);
      block.setRs1(reg);
      block.setImm("-1");
      intermediate_code_.emplace_back(block, true);
      instruction_number_line_number_mapping_[instruction_index_] = block.getLineNumber();
      instruction_index_++;
      skipCurrentLine();
      return true;
    }
    return false;
  }

    // neg
  else if (currentToken().value=="neg") {
    skipCurrentLine();
    return true;
  }
    // negw
  else if (currentToken().value=="negw") {
    skipCurrentLine();
    return true;
  }
  return false;
}

