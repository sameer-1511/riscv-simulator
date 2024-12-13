/**
 * File Name: parser.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */


#include "parser.h"
#include "instructions.h"
#include "../registers.h"

Parser::Parser(const std::vector<Token> &tokens)
        : tokens_(tokens), line_number_(1), column_number_(1), pos_(0) {
    error_count_ = 0;
}

Parser::~Parser() {

}

Token Parser::prevToken() {
    return tokens_[pos_ - 1];
}

Token Parser::currentToken() {
    if (pos_ < tokens_.size()) {
        return tokens_[pos_];
    }
    return Token(TokenType::EOF_, "", line_number_, column_number_);
}

Token Parser::nextToken() {
    if (pos_ < tokens_.size()) {
        return tokens_[pos_++];
    }
    return Token(TokenType::EOF_, "", line_number_, column_number_);
}

Token Parser::peekNextToken() {
    if (pos_ < tokens_.size()) {
        return tokens_[pos_ + 1];
    }
    return Token(TokenType::EOF_, "", line_number_, column_number_);
}

Token Parser::peekToken(int n) {
    if (pos_ + n < tokens_.size()) {
        return tokens_[pos_ + n];
    }
    return Token(TokenType::EOF_, "", line_number_, column_number_);
}

void Parser::skipCurrentLine() {
    int currLine = currentToken().line;
    while (currentToken().line == currLine && currentToken().type != TokenType::EOF_) {
        nextToken();
    }
}

const std::vector<ParseError> &Parser::getErrors() const {
    return errors_;
}

void Parser::printSymbolTable() const {
    if (symbol_table_.empty()) {
        std::cout << "Symbol table is empty." << std::endl;
        return;
    }

    for (const auto &pair: symbol_table_) {
        std::cout << pair.first << " -> " << pair.second.address << " " << pair.second.isData << '\n';
    }
}

std::vector<std::pair<int, uint64_t>> &Parser::getDataBuffer() {
    return data_buffer_;
}

void Parser::printDataBuffer() const {
    if (data_buffer_.empty()) {
        std::cout << "Data buffer is empty." << std::endl;
        return;
    }

    for (const auto &pair: data_buffer_) {
        std::cout << pair.first << " -> " << pair.second << '\n';
    }
}

void Parser::printIntermediateCode() const {
    if (IntermediateCode.empty()) {
        std::cout << "Intermediate code is empty." << std::endl;
        return;
    }

    for (const auto &pair: IntermediateCode) {
        std::cout << pair.first << " -> " << pair.second << '\n';
    }
}

const std::vector<std::pair<ICUnit, bool>> &Parser::getIntermediateCode() const {
    return IntermediateCode;
}


void Parser::parseDataDirective() {
    uint64_t data_index_ = 0;
    while (currentToken().value != "text"
           && currentToken().value != "data"
           && currentToken().value != "bss"
           && currentToken().type != TokenType::EOF_) {

        if (currentToken().type == TokenType::LABEL) {
            symbol_table_[currentToken().value] = {data_index_, true};
            nextToken();
        }

        // TODO: complete error handling
        // TODO: do testing

        if (currentToken().value == "dword") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM)
                    data_buffer_.push_back(std::make_pair(8, std::stoull(currentToken().value)));
                data_index_ += 8;
                nextToken();
            }
        } else if (currentToken().value == "word") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM)
                    data_buffer_.push_back(std::make_pair(4, std::stoull(currentToken().value)));
                data_index_ += 4;
                nextToken();
            }

        } else if (currentToken().value == "halfword") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM)
                    data_buffer_.push_back(std::make_pair(2, std::stoull(currentToken().value)));
                data_index_ += 2;
                nextToken();
            }
        } else if (currentToken().value == "byte") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM)
                    data_buffer_.push_back(std::make_pair(1, std::stoull(currentToken().value)));
                data_index_ += 1;
                nextToken();
            }
        } else if (currentToken().value == "string") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::STRING
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::STRING)
                    data_buffer_.push_back(
                            std::make_pair(currentToken().value.size(), std::stoull(currentToken().value)));
                data_index_ += currentToken().value.size();
                nextToken();
            }
        } else {
            errors_.push_back(ParseError(ErrorType::INVALID_DIRECTIVE, "Invalid directive", currentToken().line,
                                         currentToken().column));
            nextToken();
        }

    }
}


// TODO: complete error handling
// TODO: do testing

void Parser::parseTextDirective() {
    uint64_t instruction_index = 0;
    while (currentToken().type != TokenType::DIRECTIVE
           && currentToken().type != TokenType::EOF_) {

        if (currentToken().type == TokenType::LABEL) {
            symbol_table_[currentToken().value] = {instruction_index * 4, false};
            nextToken();
        } else if (currentToken().type == TokenType::OPCODE) {
            ICUnit block;
            block.setOpcode(currentToken().value);
            std::vector<SyntaxType> syntaxes = instruction_syntax_map[currentToken().value];

            bool valid_syntax = false;

            for (const SyntaxType &syntax: syntaxes) {
                int current_line = currentToken().line;

                switch (syntax) {
                    case SyntaxType::O_R_C_R_C_R: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && peekToken(3).type == TokenType::REGISTER
                            && peekToken(4).line == current_line
                            && peekToken(4).type == TokenType::COMMA
                            && peekToken(5).line == current_line
                            && peekToken(5).type == TokenType::REGISTER) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidRTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs1(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs2(reg);
                                nextToken(); // skip register
                            }
                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;
                    }


                    case SyntaxType::O_R_C_R_C_I: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && peekToken(3).type == TokenType::REGISTER
                            && peekToken(4).line == current_line
                            && peekToken(4).type == TokenType::COMMA
                            && peekToken(5).line == current_line
                            && (peekToken(5).type == TokenType::NUM)) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidITypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs1(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma

                                int64_t imm = std::stoll(currentToken().value);
                                if (-2048 <= imm && imm <= 2047) {
                                    block.setImm(std::to_string(imm));
                                } else {
                                    errors_.push_back(
                                            ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range",
                                                       currentToken().line, currentToken().column));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken();
                            } else if (isValidBTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs1(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs2(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma

                                int64_t imm = std::stoll(currentToken().value);
                                if (-4096 <= imm && imm <= 4095) {
                                    if (imm % 4 == 0) {
                                        block.setImm(std::to_string(imm));
                                    } else {
                                        errors_.push_back(ParseError(ErrorType::MISALIGNED_IMMEDIATE,
                                                                     "Misaligned immediate value", currentToken().line,
                                                                     currentToken().column));
                                        skipCurrentLine();
                                        break;
                                    }
                                } else {
                                    errors_.push_back(
                                            ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range",
                                                       currentToken().line, currentToken().column));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken();
                            }

                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;
                    }


                    case SyntaxType::O_R_C_R_C_IL: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && peekToken(3).type == TokenType::REGISTER
                            && peekToken(4).line == current_line
                            && peekToken(4).type == TokenType::COMMA
                            && peekToken(5).line == current_line
                            && (peekToken(5).type == TokenType::LABEL_REF)) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidBTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs1(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs2(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                if (symbol_table_.find(currentToken().value) != symbol_table_.end()
                                    && !symbol_table_[currentToken().value].isData) {
                                    uint64_t address = symbol_table_[currentToken().value].address;
                                    int64_t offset = static_cast<int64_t>(address - instruction_index * 4);
                                    if (-4096 <= offset && offset <= 4095) {
                                        block.setImm(std::to_string(offset));
                                        block.setLabel(currentToken().value);
                                    } else {
                                        errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE,
                                                                     "Immediate value out of range",
                                                                     currentToken().line, currentToken().column));
                                        skipCurrentLine();
                                        break;
                                    }
                                    nextToken();
                                } else {
                                    backPatch.push_back(instruction_index);
                                    block.setImm(currentToken().value);
                                    IntermediateCode.push_back(std::make_pair(block, false));
                                    instruction_index++;
                                    nextToken();
                                    break;
                                }
                            }
                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;
                    }


                    case SyntaxType::O_R_C_I_LP_R_RP: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && peekToken(3).type == TokenType::NUM
                            && peekToken(4).line == current_line
                            && peekToken(4).type == TokenType::LPAREN
                            && peekToken(5).line == current_line
                            && peekToken(5).type == TokenType::REGISTER
                            && peekToken(6).line == current_line
                            && peekToken(6).type == TokenType::RPAREN) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidITypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                int64_t imm = std::stoll(currentToken().value);
                                if (-2048 <= imm && imm <= 2047) {
                                    block.setImm(std::to_string(imm));
                                } else {
                                    errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE,
                                                                 "Immediate value out of range", currentToken().line,
                                                                 currentToken().column));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken(); // skip immediate
                                nextToken(); // skip lparen
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs1(reg);
                                nextToken(); // skip register
                                nextToken(); // skip rparen
                            } else if (isValidSTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs2(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                int64_t imm = std::stoll(currentToken().value);
                                if (-2048 <= imm && imm <= 2047) {
                                    block.setImm(std::to_string(imm));
                                } else {
                                    errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE,
                                                                 "Immediate value out of range", currentToken().line,
                                                                 currentToken().column));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken(); // skip immediate
                                nextToken(); // skip lparen
                                reg = reg_alias_to_name.at(currentToken().value);
                                block.setRs1(reg);
                                nextToken(); // skip register
                                nextToken(); // skip rparen
                            }
                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;
                    }


                    case SyntaxType::O_R_C_I: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && (peekToken(3).type == TokenType::NUM)) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidUTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                int64_t imm = std::stoll(currentToken().value);
                                if (0 <= imm && imm <= 1048575) {
                                    block.setImm(std::to_string(imm));
                                } else {
                                    errors_.push_back(
                                            ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range",
                                                       currentToken().line, currentToken().column));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken(); // skip immediate
                            } else if (isValidJTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                int64_t imm = std::stoll(currentToken().value);
                                if (-1048576 <= imm && imm <= 1048575) {
                                    if (imm % 4 == 0) {
                                        block.setImm(std::to_string(imm));
                                    } else {
                                        errors_.push_back(ParseError(ErrorType::MISALIGNED_IMMEDIATE,
                                                                     "Misaligned immediate value", currentToken().line,
                                                                     currentToken().column));
                                        skipCurrentLine();
                                        break;
                                    }
                                } else {
                                    errors_.push_back(
                                            ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range",
                                                       currentToken().line, currentToken().column));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken(); // skip immediate
                            }
                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;
                    }


                    case SyntaxType::O_R_C_IL: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && peekToken(3).type == TokenType::LABEL_REF) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidJTypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                if (symbol_table_.find(currentToken().value) != symbol_table_.end()
                                    && !symbol_table_[currentToken().value].isData) {
                                    uint64_t address = symbol_table_[currentToken().value].address;
                                    int64_t offset = static_cast<int64_t>(address - instruction_index * 4);
                                    if (-1048576 <= offset && offset <= 1048575) {
                                        block.setImm(std::to_string(offset));
                                        block.setLabel(currentToken().value);
                                    } else {
                                        errors_.push_back(
                                                ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range",
                                                           currentToken().line, currentToken().column));
                                        skipCurrentLine();
                                        break;
                                    }
                                    nextToken();
                                } else {
                                    backPatch.push_back(instruction_index);
                                    block.setImm(currentToken().value);
                                    IntermediateCode.push_back(std::make_pair(block, false));
                                    instruction_index++;
                                    nextToken();
                                    break;
                                }
                            }
                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;
                    }


                    case SyntaxType::O_R_C_DL: {
                        if (peekToken(1).line == current_line
                            && peekToken(1).type == TokenType::REGISTER
                            && peekToken(2).line == current_line
                            && peekToken(2).type == TokenType::COMMA
                            && peekToken(3).line == current_line
                            && peekToken(3).type == TokenType::LABEL_REF) {
                            nextToken(); // skip opcode
                            valid_syntax = true;
                            if (isValidITypeInstruction(block.getOpcode())) {
                                std::string reg = reg_alias_to_name.at(currentToken().value);
                                block.setRd(reg);
                                nextToken(); // skip register
                                nextToken(); // skip comma
                                if (symbol_table_.find(currentToken().value) != symbol_table_.end() &&
                                    symbol_table_[currentToken().value].isData) {
                                    uint64_t address = symbol_table_[currentToken().value].address;
                                    int64_t offset = static_cast<int64_t>(address);
                                    if (-2048 <= offset && offset <= 2047) {
                                        block.setImm(std::to_string(offset));
                                    } else {
                                        errors_.push_back(
                                                ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range",
                                                           currentToken().line, currentToken().column));
                                        skipCurrentLine();
                                        break;
                                    }
                                    nextToken();
                                } else {
                                    backPatch.push_back(instruction_index);
                                    block.setImm(currentToken().value);
                                    IntermediateCode.push_back(std::make_pair(block, false));
                                    instruction_index++;
                                    nextToken();
                                    break;
                                }
                            }
                            IntermediateCode.push_back(std::make_pair(block, true));
                            instruction_index++;
                        }
                        break;

                    }

                    default:
                        errors_.push_back(ParseError(ErrorType::INVALID_SYNTAX, "Invalid syntax", currentToken().line,
                                                     currentToken().column));
                        skipCurrentLine();
                        break;
                }
                /*
                      else if (syntax == "O_R_C_R_C_I") {
                          if (peekToken(1).line == current_line
                          && peekToken(1).type == TokenType::REGISTER
                          && peekToken(2).line == current_line
                          && peekToken(2).type == TokenType::COMMA
                          && peekToken(3).line == current_line
                          && peekToken(3).type == TokenType::REGISTER
                          && peekToken(4).line == current_line
                          && peekToken(4).type == TokenType::COMMA
                          && peekToken(5).line == current_line
                          && (peekToken(5).type == TokenType::NUM)) {
                              nextToken(); // skip opcode
                              valid_syntax = true;
                              if (isValidITypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRd(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs1(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                              }
                              else if (isValidBTypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs1(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs2(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                              }

                              if (isValidITypeInstruction(block.getOpcode())) {
                                  int64_t imm = std::stoll(currentToken().value);
                                  if (-2048 <= imm && imm <= 2047) {
                                      block.setImm(std::to_string(imm));
                                  }
                                  else {
                                      errors_.push_back(ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range", currentToken().line, currentToken().column));
                                      skipCurrentLine();
                                      break;;
                                  }
                                  nextToken();
                              }
                              else if (isValidBTypeInstruction(block.getOpcode())) {
                                  int64_t imm = std::stoll(currentToken().value);
                                  if (-4096 <= imm && imm <= 4095) {
                                      block.setImm(std::to_string(imm));
                                  }
                                  else {
                                      errors_.push_back(ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range", currentToken().line, currentToken().column));
                                      skipCurrentLine();
                                      break;
                                  }
                                  nextToken();
                              }

                              IntermediateCode.push_back(std::make_pair(block, true));
                              instruction_index++;
                              break;
                          }
                      }

                      else if (syntax == "O_R_C_R_C_IL") {
                          if (peekToken(1).line == current_line
                          && peekToken(1).type == TokenType::REGISTER
                          && peekToken(2).line == current_line
                          && peekToken(2).type == TokenType::COMMA
                          && peekToken(3).line == current_line
                          && peekToken(3).type == TokenType::REGISTER
                          && peekToken(4).line == current_line
                          && peekToken(4).type == TokenType::COMMA
                          && peekToken(5).line == current_line
                          && (peekToken(5).type == TokenType::LABEL_REF)) {
                              nextToken(); // skip opcode
                              valid_syntax = true;
                              if (isValidBTypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs1(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs2(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma

                                  if (symbol_table_.find(currentToken().value) != symbol_table_.end()
                                  && !symbol_table_[currentToken().value].isData) {
                                      uint64_t address = symbol_table_[currentToken().value].address;
                                      int64_t offset = static_cast<int64_t>(address - instruction_index * 4);
                                      if (-4096 <= offset && offset <= 4095) {
                                          block.setImm(std::to_string(offset));
                                          block.setLabel(currentToken().value);
                                      }
                                      else {
                                          errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE, "Immediate value out of range", currentToken().line, currentToken().column));
                                          skipCurrentLine();
                                          break;
                                      }
                                      nextToken();
                                  }
                                  else {
                                      backPatch.push_back(instruction_index);
                                      block.setImm(currentToken().value);
                                      IntermediateCode.push_back(std::make_pair(block, false));
                                      instruction_index++;
                                      nextToken();
                                      break;
                                  }
                              }

                              IntermediateCode.push_back(std::make_pair(block, true));
                              instruction_index++;
                              break;
                          }
                      }



                      else if (syntax == "O_R_C_I_LP_R_RP") {
                          if (peekToken(1).line == current_line
                          && peekToken(1).type == TokenType::REGISTER
                          && peekToken(2).line == current_line
                          && peekToken(2).type == TokenType::COMMA
                          && peekToken(3).line == current_line
                          && peekToken(3).type == TokenType::NUM
                          && peekToken(4).line == current_line
                          && peekToken(4).type == TokenType::LPAREN
                          && peekToken(5).line == current_line
                          && peekToken(5).type == TokenType::REGISTER
                          && peekToken(6).line == current_line
                          && peekToken(6).type == TokenType::RPAREN) {
                              nextToken(); // skip opcode
                              valid_syntax = true;

                              if (isValidITypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRd(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  int64_t imm = std::stoll(currentToken().value);
                                  if (-2048 <= imm && imm <= 2047) {
                                      block.setImm(std::to_string(imm));
                                  }
                                  else {
                                      errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE, "Immediate value out of range", currentToken().line, currentToken().column));
                                      skipCurrentLine();
                                      break;
                                  }
                                  nextToken(); // skip immediate
                                  nextToken(); // skip lparen
                                  reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs1(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip rparen
                              }
                              else if (isValidSTypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs2(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  int64_t imm = std::stoll(currentToken().value);
                                  if (-2048 <= imm && imm <= 2047) {
                                      block.setImm(std::to_string(imm));
                                  }
                                  else {
                                      errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE, "Immediate value out of range", currentToken().line, currentToken().column));
                                      skipCurrentLine();
                                      break;
                                  }
                                  nextToken(); // skip immediate
                                  nextToken(); // skip lparen
                                  reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRs1(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip rparen
                              }

                              IntermediateCode.push_back(std::make_pair(block, true));
                              instruction_index++;
                              break;
                          }
                      }

                      else if (syntax == "O_R_C_I") {
                          if (peekToken(1).line == current_line
                          && peekToken(1).type == TokenType::REGISTER
                          && peekToken(2).line == current_line
                          && peekToken(2).type == TokenType::COMMA
                          && peekToken(3).line == current_line
                          && (peekToken(3).type == TokenType::NUM)) {
                              nextToken(); // skip opcode
                              valid_syntax = true;
                              if (isValidUTypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRd(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  int64_t imm = std::stoll(currentToken().value);
                                  if (0 <= imm && imm <= 1048575) {
                                      block.setImm(std::to_string(imm));
                                  }
                                  else {
                                      errors_.push_back(ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range", currentToken().line, currentToken().column));
                                      skipCurrentLine();
                                      break;
                                  }
                                  nextToken(); // skip immediate
                              }
                              else if (isValidJTypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRd(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  int64_t imm = std::stoll(currentToken().value);
                                  if (-1048576 <= imm && imm <= 1048575) {
                                      block.setImm(std::to_string(imm));
                                  }
                                  else {
                                      errors_.push_back(ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range", currentToken().line, currentToken().column));
                                      skipCurrentLine();
                                      break;
                                  }
                                  nextToken(); // skip immediate
                              }

                              IntermediateCode.push_back(std::make_pair(block, true));
                              instruction_index++;
                              break;
                          }
                      }

                      else if (syntax == "O_R_C_L") {
                          if (peekToken(1).line == current_line
                          && peekToken(1).type == TokenType::REGISTER
                          && peekToken(2).line == current_line
                          && peekToken(2).type == TokenType::COMMA
                          && peekToken(3).line == current_line
                          && peekToken(3).type == TokenType::LABEL_REF) {
                              nextToken(); // skip opcode
                              valid_syntax = true;

                              // TODO: needs work, use auipc for portability
                              if (isValidITypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRd(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  if (symbol_table_.find(currentToken().value) != symbol_table_.end()
                                  && symbol_table_[currentToken().value].isData) {
                                      uint64_t address = symbol_table_[currentToken().value].address;
                                      int64_t offset = static_cast<int64_t>(address);
                                      if (-2048 <= offset && offset <= 2047) {
                                          block.setImm(std::to_string(offset));
                                      }
                                      else {
                                          errors_.push_back(ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range", currentToken().line, currentToken().column));
                                          skipCurrentLine();
                                          break;
                                      }
                                      nextToken();
                                  }
                                  else {
                                      backPatch.push_back(instruction_index);
                                      block.setImm(currentToken().value);
                                      IntermediateCode.push_back(std::make_pair(block, false));
                                      instruction_index++;
                                      nextToken();
                                      break;
                                  }
                              }
                              else if (isValidJTypeInstruction(block.getOpcode())) {
                                  std::string reg = reg_alias_to_name.at(currentToken().value);
                                  block.setRd(reg);
                                  nextToken(); // skip register
                                  nextToken(); // skip comma
                                  if (symbol_table_.find(currentToken().value) != symbol_table_.end()
                                  && !symbol_table_[currentToken().value].isData) {
                                      uint64_t address = symbol_table_[currentToken().value].address;
                                      int64_t offset = static_cast<int64_t>(address - instruction_index * 4);
                                      if (-1048576 <= offset && offset <= 1048575) {
                                          block.setImm(std::to_string(offset));
                                          block.setLabel(currentToken().value);
                                      }
                                      else {
                                          errors_.push_back(ParseError(ErrorType::INVALID_IMMEDIATE, "Immediate value out of range", currentToken().line, currentToken().column));
                                          skipCurrentLine();
                                          break;
                                      }
                                      nextToken();
                                  }
                                  else {
                                      backPatch.push_back(instruction_index);
                                      block.setImm(currentToken().value);
                                      IntermediateCode.push_back(std::make_pair(block, false));
                                      instruction_index++;
                                      nextToken();
                                      break;
                                  }
                              }

                              IntermediateCode.push_back(std::make_pair(block, true));
                              instruction_index++;
                              break;
                          }
                      }
                */

            }

            if (!valid_syntax) {
                errors_.push_back(ParseError(ErrorType::INVALID_SYNTAX, "Invalid syntax", currentToken().line,
                                             currentToken().column));
                skipCurrentLine();
            }


        } else {
            errors_.push_back(ParseError(ErrorType::INVALID_DIRECTIVE, "Invalid token", currentToken().line,
                                         currentToken().column));
            skipCurrentLine();
            continue;
        }
    }
}



// TODO: implement bss directive

void Parser::parse() {
    while (currentToken().type != TokenType::EOF_) {
        if (currentToken().value == "data" && currentToken().type == TokenType::DIRECTIVE) {
            nextToken();
            parseDataDirective();
        } else if (currentToken().value == "text" && currentToken().type == TokenType::DIRECTIVE) {
            nextToken();
            parseTextDirective();
        } else if (currentToken().type == TokenType::LABEL || currentToken().type == TokenType::OPCODE) {
            parseTextDirective();
        }
            //else if (currentToken().value == "bss") {
            //    parseBssDirective();
            //}
        else {
            errors_.push_back(ParseError(ErrorType::INVALID_DIRECTIVE, "Invalid directive", currentToken().line,
                                         currentToken().column));
            nextToken();
        }
    }

    // TODO:: complete backpatcher
    for (int index: backPatch) {
        ICUnit block = IntermediateCode[index].first;
        if (symbol_table_.find(block.getImm()) != symbol_table_.end()) {

            if (isValidITypeInstruction(block.getOpcode()) &&
                symbol_table_[block.getImm()].isData) {
                uint64_t address = symbol_table_[block.getImm()].address;

                block.setImm(std::to_string(symbol_table_[block.getImm()].address));
            } else if (isValidBTypeInstruction(block.getOpcode())) {
                if (!symbol_table_[block.getImm()].isData) {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    int64_t offset = static_cast<int64_t>(address - index * 4);
                    if (-4096 <= offset && offset <= 4095) {
                        block.setImm(std::to_string(offset));
                    } else {
                        errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE,
                                                     "Immediate value out of range",
                                                     currentToken().line, currentToken().column));
                        skipCurrentLine();
                        break;
                    }
                } else {
                    errors_.push_back(ParseError(ErrorType::INVALID_LABEL_REF, "Label reference not found",
                                                 currentToken().line,
                                                 currentToken().column));
                }
            } else if (isValidJTypeInstruction(block.getOpcode())) {
                if (symbol_table_[block.getImm()].isData) {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    block.setImm(std::to_string(symbol_table_[block.getImm()].address));
                } else {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    int64_t offset = static_cast<int64_t>(address - index * 4);
                    if (-1048576 <= offset && offset <= 1048575) {
                        block.setImm(std::to_string(offset));
                        block.setLabel(block.getImm());
                    } else {
                        errors_.push_back(ParseError(ErrorType::IMMEDIATE_OUT_OF_RANGE,
                                                     "Immediate value out of range",
                                                     currentToken().line, currentToken().column));
                        skipCurrentLine();
                        break;
                    }
                }
            }

            //uint64_t address = symbol_table_[block.getImm()].address;
//
            //if (isValidITypeInstruction(block.getOpcode())) {
            //    block.setImm(std::to_string(address));
            //} 
            //else if (isValidBTypeInstruction(block.getOpcode())) {
            //    block.setImm(std::to_string(address - index * 4));
            //}
            //else if (isValidJTypeInstruction(block.getOpcode())) {
            //    block.setImm(std::to_string(address - index * 4));
            //}


            IntermediateCode[index].first = block;
            IntermediateCode[index].second = true;
        } else {
            errors_.push_back(ParseError(ErrorType::INVALID_LABEL_REF, "Label reference not found", currentToken().line,
                                         currentToken().column));
        }
    }


}






