/** @cond DOXYGEN_IGNORE */
/**
 * File Name: parser.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
/** @endcond */


#include "parser.h"

#include "instructions.h"
#include "../registers.h"
#include "../utils.h"


Parser::Parser(const std::string &filename, const std::vector<Token> &tokens)
        : filename_(std::move(filename)), tokens_(tokens) {
}

Parser::~Parser() {

}

Token Parser::prevToken() {
    if (pos_ > 0) {
        return tokens_[pos_ - 1];
    }
    return {TokenType::EOF_, "", 1, 1};
}

Token Parser::currentToken() {
    if (pos_ < tokens_.size()) {
        return tokens_[pos_];
    }
    return {TokenType::EOF_, "", 1, 1};
}

Token Parser::nextToken() {
    if (pos_ < tokens_.size()) {
        return tokens_[pos_++];
    }
    return {TokenType::EOF_, "", 1, 1};
}

Token Parser::peekNextToken() {
    if (pos_ < tokens_.size()) {
        return tokens_[pos_ + 1];
    }
    return {TokenType::EOF_, "", 1, 1};
}

Token Parser::peekToken(int n) {
    if (pos_ + n < tokens_.size()) {
        return tokens_[pos_ + n];
    }
    return {TokenType::EOF_, "", 1, 1};
}

void Parser::skipCurrentLine() {
    unsigned int currLine = currentToken().line_number;
    while (currentToken().line_number == currLine && currentToken().type != TokenType::EOF_) {
        nextToken();
    }
}

void Parser::recordError(const ParseError &error) {
    errors_.parse_errors.emplace_back(error);
    errors_.count++;
}


const std::vector<ParseError> &Parser::getErrors() const {
    return errors_.parse_errors;
}

void Parser::printErrors() const {
    for (const auto &error: errors_.all_errors) {
        std::visit([](auto &&arg) { 
            std::cout << arg; 
        }, error);
    }
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

std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, std::string>> &Parser::getDataBuffer() {
    return data_buffer_;
}

void Parser::printDataBuffers() const {
    auto stringToHex = [](const std::string &str) -> std::string {
        std::ostringstream oss;
        for (unsigned char ch : str) {
            oss << std::uppercase << std::setfill('0') << std::setw(2)
                << std::hex << static_cast<int>(ch) << ' ';
        }
        return oss.str();
    };

    for (const auto &data: data_buffer_ ) {
        std::cout << std::hex;
        if (std::holds_alternative<uint8_t>(data)) {
            std::cout << std::get<uint8_t>(data);
        } else if (std::holds_alternative<uint16_t>(data)) {
            std::cout << std::get<uint16_t>(data);
        } else if (std::holds_alternative<uint32_t>(data)) {
            std::cout << std::get<uint32_t>(data);
        } else if (std::holds_alternative<uint64_t>(data)) {
            std::cout << std::get<uint64_t>(data);
        } else if (std::holds_alternative<std::string>(data)) {
            std::cout << stringToHex(std::get<std::string>(data));
        }

        std::cout << std::dec;
        std::cout << '\n';
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
    while (currentToken().value != "text"
           && currentToken().value != "data"
           && currentToken().value != "bss"
           && currentToken().type != TokenType::EOF_) {

        if (currentToken().type == TokenType::LABEL) {
            symbol_table_[currentToken().value] = {data_index_, currentToken().line_number, true};
            nextToken();
            continue;
        }

        // TODO: complete error handling
        // TODO: do testing

        if (currentToken().value == "dword") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM) {
                    data_buffer_.emplace_back(static_cast<uint64_t>(std::stoull(currentToken().value)));
                }
                data_index_ += 8;
                nextToken();
            }
        } else if (currentToken().value == "word") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM) {
                    data_buffer_.emplace_back(static_cast<uint32_t>(std::stoull(currentToken().value)));
                }
                data_index_ += 4;
                nextToken();
            }

        } else if (currentToken().value == "halfword") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM) {
                    data_buffer_.emplace_back(static_cast<uint16_t>(std::stoull(currentToken().value)));
                }
                data_index_ += 2;
                nextToken();
            }
        } else if (currentToken().value == "byte") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::NUM
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::NUM) {
                    data_buffer_.emplace_back(static_cast<uint8_t>(std::stoull(currentToken().value)));
                }
                data_index_ += 1;
                nextToken();
            }
        } else if (currentToken().value == "string") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                   && (currentToken().type == TokenType::STRING
                       || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::STRING) {
                    std::string rawString = currentToken().value;
                    std::string processedString = parseEscapedString(rawString);
                    data_buffer_.emplace_back(static_cast<std::string>(processedString));
                    data_index_ += processedString.size();
                }
                nextToken();
            }
        } else {
            recordError(ParseError(currentToken().line_number, "Invalid directive: Expected .dword, .word, .halfword, .byte, .string"));
            errors_.all_errors.emplace_back(SyntaxError("Invalid directive", "Expected: dword, word, halfword, byte, string",
                                                 filename_, currentToken().line_number,
                                                 currentToken().column_number,
                                                 getLineFromFile(filename_, currentToken().line_number)));
            nextToken();
        }

    }
}


// TODO: complete error handling
// TODO: do testing

void Parser::parseTextDirective() {
    unsigned int instruction_index = 0;
    while (currentToken().type != TokenType::DIRECTIVE
           && currentToken().type != TokenType::EOF_) {

        if (currentToken().type == TokenType::LABEL) {
            if (symbol_table_.find(currentToken().value) != symbol_table_.end()) {
                recordError(ParseError(currentToken().line_number, "Label redefinition: already defined at line " + std::to_string(
                        symbol_table_[currentToken().value].line_number)));
                errors_.all_errors.emplace_back(LabelRedefinitionError("Label redefinition",
                                                                "Label already defined at line " + std::to_string(
                                                                        symbol_table_[currentToken().value].line_number),
                                                                filename_,
                                                                currentToken().line_number,
                                                                currentToken().column_number,
                                                                getLineFromFile(filename_,
                                                                                currentToken().line_number)));
                nextToken();
                continue;
            }
            symbol_table_[currentToken().value] = {instruction_index * 4, currentToken().line_number, false};
            nextToken();
        } else if (currentToken().type == TokenType::OPCODE) {

            std::vector<SyntaxType> syntaxes = instruction_syntax_map[currentToken().value];

            bool valid_syntax = false;

            for (const SyntaxType &syntax: syntaxes) {
                unsigned int current_line = currentToken().line_number;

                switch (syntax) {
                case SyntaxType::O_R_C_R_C_R: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::REGISTER
                        && peekToken(4).line_number == current_line
                        && peekToken(4).type == TokenType::COMMA
                        && peekToken(5).line_number == current_line
                        && peekToken(5).type == TokenType::REGISTER
                        && peekToken(6).line_number != current_line
                            ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidRTypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
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
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }

                case SyntaxType::O_R_C_R_C_I: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::REGISTER
                        && peekToken(4).line_number == current_line
                        && peekToken(4).type == TokenType::COMMA
                        && peekToken(5).line_number == current_line
                        && peekToken(5).type == TokenType::NUM
                        && peekToken(6).line_number != current_line
                        ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidITypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
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
                                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                                  "Expected: -2048 <= imm <= 2047",
                                                                                  filename_,
                                                                                  currentToken().line_number,
                                                                                  currentToken().column_number,
                                                                                  getLineFromFile(filename_,
                                                                                                  currentToken().line_number)));
                                skipCurrentLine();
                                break;
                            }
                            nextToken();
                        } else if (isValidBTypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
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
                                    recordError(ParseError(currentToken().line_number, "Misaligned immediate value"));
                                    errors_.all_errors.emplace_back(MisalignedImmediateError("Misaligned immediate value",
                                                                                       "Expected: imm % 4 == 0",
                                                                                       filename_,
                                                                                       currentToken().line_number,
                                                                                       currentToken().column_number,
                                                                                       getLineFromFile(filename_,
                                                                                                       currentToken().line_number)));
                                    skipCurrentLine();
                                    break;
                                }
                            } else {
                                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                                  "Expected: -4096 <= imm <= 4095",
                                                                                  filename_,
                                                                                  currentToken().line_number,
                                                                                  currentToken().column_number,
                                                                                  getLineFromFile(filename_,
                                                                                                  currentToken().line_number)));
                                skipCurrentLine();
                                break;
                            }
                            nextToken();
                        }
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }
                
                case SyntaxType::O_R_C_R_C_IL: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::REGISTER
                        && peekToken(4).line_number == current_line
                        && peekToken(4).type == TokenType::COMMA
                        && peekToken(5).line_number == current_line
                        && peekToken(5).type == TokenType::LABEL_REF
                        && peekToken(6).line_number != current_line
                        ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidBTypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
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
                                auto offset = static_cast<int64_t>(address - instruction_index * 4);
                                if (-4096 <= offset && offset <= 4095) {
                                    block.setImm(std::to_string(offset));
                                    block.setLabel(currentToken().value);
                                } else {
                                    recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                    errors_.all_errors.emplace_back(
                                            ImmediateOutOfRangeError("Immediate value out of range",
                                                                     "Expected: -4096 <= imm <= 4095",
                                                                     filename_,
                                                                     currentToken().line_number,
                                                                     currentToken().column_number,
                                                                     getLineFromFile(filename_,
                                                                                     currentToken().line_number)));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken();
                            } else {
                                backPatch.push_back(instruction_index);
                                block.setImm(currentToken().value);
                                IntermediateCode.emplace_back(block, false);
                                instruction_index++;
                                nextToken();
                                break;
                            }
                        }
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }
                
                case SyntaxType::O_R_C_I_LP_R_RP: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::NUM
                        && peekToken(4).line_number == current_line
                        && peekToken(4).type == TokenType::LPAREN
                        && peekToken(5).line_number == current_line
                        && peekToken(5).type == TokenType::REGISTER
                        && peekToken(6).line_number == current_line
                        && peekToken(6).type == TokenType::RPAREN
                        && peekToken(7).line_number != current_line
                        ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidITypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
                            std::string reg = reg_alias_to_name.at(currentToken().value);
                            block.setRd(reg);
                            nextToken(); // skip register
                            nextToken(); // skip comma
                            int64_t imm = std::stoll(currentToken().value);
                            if (-2048 <= imm && imm <= 2047) {
                                block.setImm(std::to_string(imm));
                            } else {
                                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                                  "Expected: -2048 <= imm <= 2047",
                                                                                  filename_,
                                                                                  currentToken().line_number,
                                                                                  currentToken().column_number,
                                                                                  getLineFromFile(filename_,
                                                                                                  currentToken().line_number)));
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
                            block.setLineNumber(current_line);
                            std::string reg = reg_alias_to_name.at(currentToken().value);
                            block.setRs2(reg);
                            nextToken(); // skip register
                            nextToken(); // skip comma
                            int64_t imm = std::stoll(currentToken().value);
                            if (-2048 <= imm && imm <= 2047) {
                                block.setImm(std::to_string(imm));
                            } else {
                                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                                  "Expected: -2048 <= imm <= 2047",
                                                                                  filename_,
                                                                                  currentToken().line_number,
                                                                                  currentToken().column_number,
                                                                                  getLineFromFile(filename_,
                                                                                                  currentToken().line_number)));
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
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }
                
                case SyntaxType::O_R_C_I: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::NUM
                        && peekToken(4).line_number != current_line
                        ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidUTypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
                            std::string reg = reg_alias_to_name.at(currentToken().value);
                            block.setRd(reg);
                            nextToken(); // skip register
                            nextToken(); // skip comma
                            int64_t imm = std::stoll(currentToken().value);
                            if (0 <= imm && imm <= 1048575) {
                                block.setImm(std::to_string(imm));
                            } else {
                                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                                  "Expected: 0 <= imm <= 1048575",
                                                                                  filename_,
                                                                                  currentToken().line_number,
                                                                                  currentToken().column_number,
                                                                                  getLineFromFile(filename_,
                                                                                                  currentToken().line_number)));
                                skipCurrentLine();
                                break;
                            }
                            nextToken(); // skip immediate
                        } else if (isValidJTypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
                            std::string reg = reg_alias_to_name.at(currentToken().value);
                            block.setRd(reg);
                            nextToken(); // skip register
                            nextToken(); // skip comma
                            int64_t imm = std::stoll(currentToken().value);
                            if (-1048576 <= imm && imm <= 1048575) {
                                if (imm % 4 == 0) {
                                    block.setImm(std::to_string(imm));
                                } else {
                                    recordError(ParseError(currentToken().line_number, "Misaligned immediate value"));
                                    errors_.all_errors.emplace_back(MisalignedImmediateError("Misaligned immediate value",
                                                                                       "Expected: imm % 4 == 0",
                                                                                       filename_,
                                                                                       currentToken().line_number,
                                                                                       currentToken().column_number,
                                                                                       getLineFromFile(filename_,
                                                                                                       currentToken().line_number)));
                                    skipCurrentLine();
                                    break;
                                }
                            } else {
                                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                                  "Expected: -1048576 <= imm <= 1048575",
                                                                                  filename_,
                                                                                  currentToken().line_number,
                                                                                  currentToken().column_number,
                                                                                  getLineFromFile(filename_,
                                                                                                  currentToken().line_number)));
                                skipCurrentLine();
                                break;
                            }
                            nextToken(); // skip immediate
                        }
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }
                
                case SyntaxType::O_R_C_IL: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::LABEL_REF
                        && peekToken(4).line_number != current_line
                        ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidJTypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
                            std::string reg = reg_alias_to_name.at(currentToken().value);
                            block.setRd(reg);
                            nextToken(); // skip register
                            nextToken(); // skip comma
                            if (symbol_table_.find(currentToken().value) != symbol_table_.end()
                                && !symbol_table_[currentToken().value].isData) {
                                uint64_t address = symbol_table_[currentToken().value].address;
                                auto offset = static_cast<int64_t>(address - instruction_index * 4);
                                if (-1048576 <= offset && offset <= 1048575) {
                                    block.setImm(std::to_string(offset));
                                    block.setLabel(currentToken().value);
                                } else {
                                    recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                    errors_.all_errors.emplace_back(
                                            ImmediateOutOfRangeError("Immediate value out of range",
                                                                     "Expected: -1048576 <= imm <= 1048575",
                                                                     filename_,
                                                                     currentToken().line_number,
                                                                     currentToken().column_number,
                                                                     getLineFromFile(filename_,
                                                                                     currentToken().line_number)));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken();
                            } else {
                                backPatch.push_back(instruction_index);
                                block.setImm(currentToken().value);
                                IntermediateCode.emplace_back(block, false);
                                instruction_index++;
                                nextToken();
                                break;
                            }
                        }
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }
                
                case SyntaxType::O_R_C_DL: {
                    if (peekToken(1).line_number == current_line
                        && peekToken(1).type == TokenType::REGISTER
                        && peekToken(2).line_number == current_line
                        && peekToken(2).type == TokenType::COMMA
                        && peekToken(3).line_number == current_line
                        && peekToken(3).type == TokenType::LABEL_REF
                        && peekToken(4).line_number != current_line
                        ) {
                        ICUnit block;
                        block.setOpcode(currentToken().value);
                        nextToken(); // skip opcode
                        valid_syntax = true;
                        if (isValidITypeInstruction(block.getOpcode())) {
                            block.setLineNumber(current_line);
                            std::string reg = reg_alias_to_name.at(currentToken().value);
                            block.setRd(reg);
                            nextToken(); // skip register
                            nextToken(); // skip comma
                            if (symbol_table_.find(currentToken().value) != symbol_table_.end() &&
                                symbol_table_[currentToken().value].isData) {
                                uint64_t address = symbol_table_[currentToken().value].address;
                                auto offset = static_cast<int64_t>(address);
                                if (-2048 <= offset && offset <= 2047) {
                                    block.setImm(std::to_string(offset));
                                } else {
                                    recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                                    errors_.all_errors.emplace_back(
                                            ImmediateOutOfRangeError("Immediate value out of range",
                                                                     "Expected: -2048 <= imm <= 2047",
                                                                     filename_,
                                                                     currentToken().line_number,
                                                                     currentToken().column_number,
                                                                     getLineFromFile(filename_,
                                                                                     currentToken().line_number)));
                                    skipCurrentLine();
                                    break;
                                }
                                nextToken();
                            } else {
                                backPatch.push_back(instruction_index);
                                block.setImm(currentToken().value);
                                IntermediateCode.emplace_back(block, false);
                                instruction_index++;
                                nextToken();
                                break;
                            }
                        }
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                    }
                    break;
                }
                
                case SyntaxType::O_R_C_R_C_DL: {
                    break;
                }
                
                case SyntaxType::O: {
                    break;
                }
                
                default: {
                    break;
                }
                }
            }

            // pseudo-insructions
            // la
            if (currentToken().value == "la") {
                valid_syntax = true;
                skipCurrentLine();
            }
           
            // nop
            else if (currentToken().value == "nop") {
                if (peekToken(4).type == TokenType::EOF_ 
                 || peekToken(4).line_number != currentToken().line_number) 
                 {
                    ICUnit block;
                    block.setOpcode(currentToken().value);
                    valid_syntax = true;
                    block.setLineNumber(currentToken().line_number);
                    block.setOpcode("addi");
                    block.setRd("x0");
                    block.setRs1("x0");
                    block.setRs2("x0");
                    block.setImm("0");
                    IntermediateCode.emplace_back(block, true);
                    instruction_index++;
                    nextToken();
                }
                
            }
                // li
            else if (currentToken().value == "li") {
                if (peekToken(1).line_number == currentToken().line_number
                    && peekToken(1).type == TokenType::REGISTER
                    && peekToken(2).line_number == currentToken().line_number
                    && peekToken(2).type == TokenType::COMMA
                    && peekToken(3).line_number == currentToken().line_number
                    && peekToken(3).type == TokenType::NUM
                    &&
                    (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)) {
                    ICUnit block;
                    block.setOpcode(currentToken().value);
                    valid_syntax = true;
                    int64_t imm = std::stoll(peekToken(3).value);
                    if (-2048 <= imm && imm <= 2047) {
                        block.setLineNumber(currentToken().line_number);
                        block.setOpcode("addi");
                        std::string reg = reg_alias_to_name.at(peekToken(1).value);
                        block.setRd(reg);
                        block.setRs1("x0");
                        block.setImm(peekToken(3).value);
                        IntermediateCode.emplace_back(block, true);
                        instruction_index++;
                        nextToken(); // skip li
                        nextToken(); // skip register
                        nextToken(); // skip comma
                        nextToken(); // skip immediate
                    } else if (-1048576 <= imm && imm <= 1048575) {
                        int64_t upper = (imm + (1 << 11)) >> 12;
                        int64_t lower = imm - (upper << 12);

                        // Emit lui instruction
                        ICUnit luiBlock;
                        luiBlock.setLineNumber(currentToken().line_number);
                        luiBlock.setOpcode("lui");
                        std::string reg = reg_alias_to_name.at(peekToken(1).value);
                        luiBlock.setRd(reg);
                        luiBlock.setImm(std::to_string(upper));
                        IntermediateCode.emplace_back(luiBlock, true);
                        instruction_index++;

                        // Emit addi instruction (if lower part is non-zero)
                        if (lower != 0) {
                            ICUnit addiBlock;
                            addiBlock.setLineNumber(currentToken().line_number);
                            addiBlock.setOpcode("addi");
                            addiBlock.setRd(reg);
                            addiBlock.setRs1(reg);
                            addiBlock.setImm(std::to_string(lower));
                            IntermediateCode.emplace_back(addiBlock, true);
                            instruction_index++;
                        }

                        nextToken(); // skip li
                        nextToken(); // skip register
                        nextToken(); // skip comma
                        nextToken(); // skip immediate



                    } else {
                        recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                        errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                          "Expected: -1048576 <= imm <= 1048575",
                                                                          filename_,
                                                                          currentToken().line_number,
                                                                          currentToken().column_number,
                                                                          getLineFromFile(filename_,
                                                                                          currentToken().line_number)));

                        skipCurrentLine();
                    }

                }
            }
                // mv
            else if (currentToken().value == "mv") {
                if (peekToken(1).line_number == currentToken().line_number
                    && peekToken(1).type == TokenType::REGISTER
                    && peekToken(2).line_number == currentToken().line_number
                    && peekToken(2).type == TokenType::COMMA
                    && peekToken(3).line_number == currentToken().line_number
                    && peekToken(3).type == TokenType::REGISTER
                    &&
                    (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)) {
                    ICUnit block;
                    block.setOpcode("add");
                    valid_syntax = true;
                    block.setLineNumber(currentToken().line_number);
                    std::string reg = reg_alias_to_name.at(peekToken(1).value);
                    block.setRd(reg);
                    reg = reg_alias_to_name.at(peekToken(3).value);
                    block.setRs1(reg);
                    block.setRs2("x0");
                    IntermediateCode.emplace_back(block, true);
                    instruction_index++;
                    nextToken(); // skip mv
                    nextToken(); // skip register
                    nextToken(); // skip comma
                    nextToken(); // skip register
                }
            }
                // not
            else if (currentToken().value == "not") {
                if (peekToken(1).line_number == currentToken().line_number
                    && peekToken(1).type == TokenType::REGISTER
                    && peekToken(2).line_number == currentToken().line_number
                    && peekToken(2).type == TokenType::COMMA
                    && peekToken(3).line_number == currentToken().line_number
                    && peekToken(3).type == TokenType::REGISTER
                    &&
                    (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)) {
                    ICUnit block;
                    block.setOpcode("xori");
                    valid_syntax = true;
                    block.setLineNumber(currentToken().line_number);
                    std::string reg = reg_alias_to_name.at(peekToken(1).value);
                    block.setRd(reg);
                    reg = reg_alias_to_name.at(peekToken(3).value);
                    block.setRs1(reg);
                    block.setImm("-1");
                    IntermediateCode.emplace_back(block, true);
                    instruction_index++;
                    nextToken(); // skip not
                    nextToken(); // skip register
                    nextToken(); // skip comma
                    nextToken(); // skip register
                }
            }

                // neg
            else if (currentToken().value == "neg") {
                valid_syntax = true;
                skipCurrentLine();
            }
                // negw
            else if (currentToken().value == "negw") {
                valid_syntax = true;
                skipCurrentLine();
            }


            if (!valid_syntax) {
                recordError(ParseError(currentToken().line_number, "Invalid syntax: Expected: " + getExpectedSyntaxes(currentToken().value)));
                errors_.all_errors.emplace_back(
                        SyntaxError("Syntax error", "Expected: " + getExpectedSyntaxes(currentToken().value), filename_,
                                    currentToken().line_number, currentToken().column_number,
                                    getLineFromFile(filename_, currentToken().line_number)));

                skipCurrentLine();
            }


        } else {
            recordError(ParseError(currentToken().line_number, "Unexpected token: " + currentToken().value));
            errors_.all_errors.emplace_back(UnexpectedTokenError("Unexpected token", filename_, currentToken().line_number,
                                                          currentToken().column_number,
                                                          getLineFromFile(filename_, currentToken().line_number)));

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
            recordError(ParseError(currentToken().line_number, "Invalid token: Expected .data, .text, .bss or <opcode> or <label>"));
            errors_.all_errors.emplace_back(
                    SyntaxError("Invalid token", "Expected: .data, .text, .bss or <opcode> or <label>", filename_,
                                currentToken().line_number,
                                currentToken().column_number,
                                getLineFromFile(filename_, currentToken().line_number)));
            nextToken();
        }
    }

    for (unsigned int index: backPatch) {
        ICUnit block = IntermediateCode[index].first;
        if (symbol_table_.find(block.getImm()) != symbol_table_.end()) {

            // TODO: complete I type instructions
            if (isValidITypeInstruction(block.getOpcode()) &&
                symbol_table_[block.getImm()].isData) {
                // uint64_t address = symbol_table_[block.getImm()].address;

                block.setImm(std::to_string(symbol_table_[block.getImm()].address));
            } else if (isValidBTypeInstruction(block.getOpcode())) {
                if (!symbol_table_[block.getImm()].isData) {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    auto offset = static_cast<int64_t>(address - index * 4);
                    if (-4096 <= offset && offset <= 4095) {
                        block.setImm(std::to_string(offset));
                    } else {
                        recordError(ParseError(block.getLineNumber(), "Immediate value out of range"));
                        errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                          "Expected: -4096 <= imm <= 4095",
                                                                          filename_,
                                                                          block.getLineNumber(),
                                                                          0,
                                                                          getLineFromFile(filename_,
                                                                                          block.getLineNumber())));
                        continue;
                    }
                } else {
                    recordError(ParseError(block.getLineNumber(), "Invalid label reference: Label references data"));
                    errors_.all_errors.emplace_back(InvalidLabelRefError("Invalid label reference", "Label references data",
                                                                  filename_,
                                                                  block.getLineNumber(),
                                                                  0,
                                                                  getLineFromFile(filename_, block.getLineNumber())));
                }
            } else if (isValidJTypeInstruction(block.getOpcode())) {
                if (!symbol_table_[block.getImm()].isData) {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    auto offset = static_cast<int64_t>(address - index * 4);
                    if (-1048576 <= offset && offset <= 1048575) {
                        block.setImm(std::to_string(offset));
                        block.setLabel(block.getImm());
                    } else {
                        recordError(ParseError(block.getLineNumber(), "Immediate value out of range"));
                        errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                          "Expected: -1048576 <= imm <= 1048575",
                                                                          filename_,
                                                                          block.getLineNumber(),
                                                                          0,
                                                                          getLineFromFile(filename_,
                                                                                          block.getLineNumber())));
                        continue;
                    }
                } else {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    auto offset = static_cast<int64_t>(address - index * 4);
                    if (-1048576 <= offset && offset <= 1048575) {
                        block.setImm(std::to_string(offset));
                        block.setLabel(block.getImm());
                    } else {
                        recordError(ParseError(block.getLineNumber(), "Immediate value out of range"));
                        errors_.all_errors.emplace_back(ImmediateOutOfRangeError("Immediate value out of range",
                                                                          "Expected: -1048576 <= imm <= 1048575",
                                                                          filename_,
                                                                          block.getLineNumber(),
                                                                          0,
                                                                          getLineFromFile(filename_,
                                                                                          block.getLineNumber())));
                        continue;
                    }
                }
            } else {
                recordError(ParseError(block.getLineNumber(), "Invalid label reference: Label references data"));
                errors_.all_errors.emplace_back(
                        InvalidLabelRefError("Invalid label reference", "Label reference not found", filename_,
                                             block.getLineNumber(), 0,
                                             getLineFromFile(filename_, block.getLineNumber())));
                continue;
            }


            IntermediateCode[index].first = block;
            IntermediateCode[index].second = true;
        } else {
            recordError(ParseError(block.getLineNumber(), "Invalid label reference: Label reference not found"));
            errors_.all_errors.emplace_back(
                    InvalidLabelRefError("Invalid label reference", "Label reference not found", filename_,
                                         block.getLineNumber(), 0,
                                         getLineFromFile(filename_, block.getLineNumber())));
        }
    }


}






