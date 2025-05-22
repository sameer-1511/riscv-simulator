/**
 * @file parser.cpp
 * @brief Contains the implementation of the Parser class for parsing tokens and generating intermediate code.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "parser.h"

#include "../common/instructions.h"
#include "../vm/registers.h"
#include "../utils.h"
#include "../common/rounding_modes.h"

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

bool Parser::parse_O() {
    if (peekToken(1).type == TokenType::EOF_ || peekToken(1).line_number != currentToken().line_number
        ) {
        ICUnit block;
        block.setOpcode(currentToken().value);
        skipCurrentLine();
        IntermediateCode.emplace_back(block, true);
        instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
        instruction_index_++;
        return true;
    }
    return false;
}

bool Parser::parse_pseudo() {
    if (currentToken().value == "la") {
        if (peekToken(1).line_number == currentToken().line_number
        && peekToken(1).type == TokenType::GP_REGISTER
        && peekToken(2).line_number == currentToken().line_number
        && peekToken(2).type == TokenType::COMMA
        && peekToken(3).line_number == currentToken().line_number
        && peekToken(3).type == TokenType::LABEL_REF
        && (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)
        ) {
            if (symbol_table_.find(peekToken(3).value) != symbol_table_.end()
                && symbol_table_[peekToken(3).value].isData) {
                    std::cout << "Found label" << std::endl;
                    // TODO: Implement la pseudo instruction
            }
            skipCurrentLine();
            instruction_index_++;
            return true;
        }
        return false;
    }

        // nop
    else if (currentToken().value == "nop") {
        if (peekToken(1).type == TokenType::EOF_
            || peekToken(1).line_number != currentToken().line_number) {
            ICUnit block;
            block.setOpcode(currentToken().value);
            block.setLineNumber(currentToken().line_number);
            block.setOpcode("addi");
            block.setRd("x0");
            block.setRs1("x0");
            block.setRs2("x0");
            block.setImm("0");
            IntermediateCode.emplace_back(block, true);
            instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
            instruction_index_++;
            nextToken();
            return true;
        }
        return false;
    }
        // li
    else if (currentToken().value == "li") {
        if (peekToken(1).line_number == currentToken().line_number
            && peekToken(1).type == TokenType::GP_REGISTER
            && peekToken(2).line_number == currentToken().line_number
            && peekToken(2).type == TokenType::COMMA
            && peekToken(3).line_number == currentToken().line_number
            && peekToken(3).type == TokenType::NUM
            &&
                (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)) {
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
                IntermediateCode.emplace_back(block, true);
                instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
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
                IntermediateCode.emplace_back(luiBlock, true);
                instruction_number_line_number_mapping[instruction_index_] = luiBlock.getLineNumber();
                instruction_index_++;

                // Emit addi instruction (if lower part is non-zero)
                if (lower != 0) {
                    ICUnit addiBlock;
                    addiBlock.setLineNumber(currentToken().line_number);
                    addiBlock.setOpcode("addi");
                    addiBlock.setRd(reg);
                    addiBlock.setRs1(reg);
                    addiBlock.setImm(std::to_string(lower));
                    IntermediateCode.emplace_back(addiBlock, true);
                    instruction_number_line_number_mapping[instruction_index_] = addiBlock.getLineNumber();
                    instruction_index_++;
                }
            } else {
                errors_.count++;
                recordError(ParseError(currentToken().line_number, "Immediate value out of range"));
                errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range", "Expected: -1048576 <= imm <= 1048575", filename_, currentToken().line_number, currentToken().column_number, getLineFromFile(filename_, currentToken().line_number)));
            }
            skipCurrentLine();
            return true;
        }
        return false;
    }
        // mv
    else if (currentToken().value == "mv") {
        if (peekToken(1).line_number == currentToken().line_number
            && peekToken(1).type == TokenType::GP_REGISTER
            && peekToken(2).line_number == currentToken().line_number
            && peekToken(2).type == TokenType::COMMA
            && peekToken(3).line_number == currentToken().line_number
            && peekToken(3).type == TokenType::GP_REGISTER
            &&
                (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)) {
            ICUnit block;
            block.setOpcode("add");
            block.setLineNumber(currentToken().line_number);
            std::string reg;
            reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            reg = reg_alias_to_name.at(peekToken(3).value);
            block.setRs1(reg);
            block.setRs2("x0");
            IntermediateCode.emplace_back(block, true);
            instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
            instruction_index_++;
            skipCurrentLine();
            return true;
        }
        return false;
    }
        // not
    else if (currentToken().value == "not") {
        if (peekToken(1).line_number == currentToken().line_number
            && peekToken(1).type == TokenType::GP_REGISTER
            && peekToken(2).line_number == currentToken().line_number
            && peekToken(2).type == TokenType::COMMA
            && peekToken(3).line_number == currentToken().line_number
            && peekToken(3).type == TokenType::GP_REGISTER
            &&
                (peekToken(4).type == TokenType::EOF_ || peekToken(4).line_number != currentToken().line_number)) {
            ICUnit block;
            block.setOpcode("xori");
            block.setLineNumber(currentToken().line_number);
            std::string reg = reg_alias_to_name.at(peekToken(1).value);
            block.setRd(reg);
            reg = reg_alias_to_name.at(peekToken(3).value);
            block.setRs1(reg);
            block.setImm("-1");
            IntermediateCode.emplace_back(block, true);
            instruction_number_line_number_mapping[instruction_index_] = block.getLineNumber();
            instruction_index_++;
            skipCurrentLine();
            return true;
        }
        return false;
    }

        // neg
    else if (currentToken().value == "neg") {
        skipCurrentLine();
        return true;
    }
        // negw
    else if (currentToken().value == "negw") {
        skipCurrentLine();
        return true;
    }
    return false;
}


//=================================================================================


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
        } else if (currentToken().value == "float") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                && (currentToken().type == TokenType::FLOAT
                    || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::FLOAT) {
                    data_buffer_.emplace_back(static_cast<float>(std::stof(currentToken().value)));
                }
                data_index_ += 4;
                nextToken();
            }
        } else if (currentToken().value == "double") {
            nextToken();
            while (currentToken().type != TokenType::EOF_
                && (currentToken().type == TokenType::FLOAT
                    || currentToken().type == TokenType::COMMA)) {

                if (currentToken().type == TokenType::FLOAT) {
                    data_buffer_.emplace_back(static_cast<double>(std::stod(currentToken().value)));
                }
                data_index_ += 8;
                nextToken();
            }
        }
        
        
        
        
        
        
        
        
        
        else if (currentToken().value == "string") {
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
            errors_.count++;
            recordError(ParseError(currentToken().line_number,
                                   "Invalid directive: Expected .dword, .word, .halfword, .byte, .string"));
            errors_.all_errors.emplace_back(
                Errors::SyntaxError("Invalid directive", "Expected: dword, word, halfword, byte, string",
                                    filename_, currentToken().line_number,
                                    currentToken().column_number,
                                    getLineFromFile(filename_, currentToken().line_number)));
            nextToken();
        }

    }
}



void Parser::parseTextDirective() {

    while (currentToken().type != TokenType::DIRECTIVE
        && currentToken().type != TokenType::EOF_) {

        if (currentToken().type == TokenType::LABEL) {
            if (symbol_table_.find(currentToken().value) != symbol_table_.end()) {
                errors_.count++;
                recordError(ParseError(currentToken().line_number,
                                       "Label redefinition: already defined at line " + std::to_string(
                                           symbol_table_[currentToken().value].line_number)));
                errors_.all_errors.emplace_back(Errors::LabelRedefinitionError("Label redefinition",
                                                                               "Label already defined at line " +
                                                                                   std::to_string(
                                                                                       symbol_table_[currentToken().value].line_number),
                                                                               filename_,
                                                                               currentToken().line_number,
                                                                               currentToken().column_number,
                                                                               getLineFromFile(filename_,
                                                                                               currentToken().line_number)));
                nextToken();
                continue;
            }
            symbol_table_[currentToken().value] = {instruction_index_ * 4, currentToken().line_number, false};
            nextToken();
        } else if (currentToken().type == TokenType::OPCODE) {
            std::vector<InstructionSet::SyntaxType>
                syntaxes = InstructionSet::instruction_syntax_map[currentToken().value];

            bool valid_syntax = false;

            for (InstructionSet::SyntaxType &syntax : syntaxes) {
                switch (syntax) {
                    case InstructionSet::SyntaxType::O_GPR_C_GPR_C_GPR: {
                        valid_syntax = parse_O_GPR_C_GPR_C_GPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_GPR_C_I: {
                        valid_syntax = parse_O_GPR_C_GPR_C_I();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_GPR_C_IL: {
                        valid_syntax = parse_O_GPR_C_GPR_C_IL();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_I_LP_GPR_RP: {
                        valid_syntax = parse_O_GPR_C_I_LP_GPR_RP();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_I: {
                        valid_syntax = parse_O_GPR_C_I();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_IL: {
                        valid_syntax = parse_O_GPR_C_IL();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_DL: {
                        valid_syntax = parse_O_GPR_C_DL();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_GPR_C_DL: {
                        break;
                    }

                    case InstructionSet::SyntaxType::O: {
                        valid_syntax = parse_O();
                        break;
                    }

                    case InstructionSet::SyntaxType::PSEUDO: {
                        valid_syntax = parse_pseudo();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_CSR_C_GPR: {
                        valid_syntax = parse_O_GPR_C_CSR_C_GPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_CSR_C_I: {
                        valid_syntax = parse_O_GPR_C_CSR_C_I();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR: {
                        valid_syntax = parse_O_FPR_C_FPR_C_FPR_C_FPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_FPR_C_FPR_C_FPR_C_RM: {
                        valid_syntax = parse_O_FPR_C_FPR_C_FPR_C_FPR_C_RM();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_FPR_C_FPR: {
                        valid_syntax = parse_O_FPR_C_FPR_C_FPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_FPR_C_FPR_C_RM: {
                        valid_syntax = parse_O_FPR_C_FPR_C_FPR_C_RM();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_FPR: {
                        valid_syntax = parse_O_FPR_C_FPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_FPR_C_RM: {
                        valid_syntax = parse_O_FPR_C_FPR_C_RM();
                        break;
                    }
                    
                    case InstructionSet::SyntaxType::O_FPR_C_GPR: {
                        valid_syntax = parse_O_FPR_C_GPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_GPR_C_RM: {
                        valid_syntax = parse_O_FPR_C_GPR_C_RM();
                        break;
                    }
                    
                    case InstructionSet::SyntaxType::O_GPR_C_FPR: {
                        valid_syntax = parse_O_GPR_C_FPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_GPR_C_FPR_C_RM: {
                        valid_syntax = parse_O_GPR_C_FPR_C_RM();
                        break;
                    }
                    
                    case InstructionSet::SyntaxType::O_GPR_C_FPR_C_FPR: {
                        valid_syntax = parse_O_GPR_C_FPR_C_FPR();
                        break;
                    }

                    case InstructionSet::SyntaxType::O_FPR_C_I_LP_GPR_RP: {
                        valid_syntax = parse_O_FPR_C_I_LP_GPR_RP();
                        break;
                    }   

                    default: {
                        break;
                    }
                }
                if (valid_syntax) {
                    break;
                }
            }
            if (valid_syntax == false) {
                errors_.count++;
                recordError(ParseError(currentToken().line_number,
                                       "Invalid syntax: Expected: "
                                           + InstructionSet::getExpectedSyntaxes(currentToken().value)));
                errors_.all_errors.emplace_back(
                    Errors::SyntaxError("Syntax error",
                                        "Expected: " + InstructionSet::getExpectedSyntaxes(currentToken().value),
                                        filename_,
                                        currentToken().line_number,
                                        currentToken().column_number,
                                        getLineFromFile(filename_, currentToken().line_number)));

                skipCurrentLine();
            }

        } else {
            errors_.count++;
            recordError(ParseError(currentToken().line_number, "Unexpected token: " + currentToken().value));
            errors_.all_errors.emplace_back(Errors::UnexpectedTokenError("Unexpected token",
                                                                         filename_,
                                                                         currentToken().line_number,
                                                                         currentToken().column_number,
                                                                         getLineFromFile(filename_,
                                                                                         currentToken().line_number)));

            skipCurrentLine();
            continue;
        }
    }
}

// TODO: implement bss directive

void Parser::parse() {
    instruction_index_ = 0;
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
            errors_.count++;
            recordError(ParseError(currentToken().line_number,
                                   "Invalid token: Expected .data, .text, .bss or <opcode> or <label>"));
            errors_.all_errors.emplace_back(
                Errors::SyntaxError("Invalid token", "Expected: .data, .text, .bss or <opcode> or <label>",
                                    filename_,
                                    currentToken().line_number,
                                    currentToken().column_number,
                                    getLineFromFile(filename_, currentToken().line_number)));
            nextToken();
        }
    }

    for (unsigned int index : backPatch) {
        ICUnit block = IntermediateCode[index].first;
        if (symbol_table_.find(block.getImm()) != symbol_table_.end()) {

            if (InstructionSet::isValidBTypeInstruction(block.getOpcode())) {
                if (!symbol_table_[block.getImm()].isData) {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    auto offset = static_cast<int64_t>(address - index * 4);
                    if (-4096 <= offset && offset <= 4095) {
                        block.setImm(std::to_string(offset));
                    } else {
                        errors_.count++;
                        recordError(ParseError(block.getLineNumber(), "Immediate value out of range"));
                        errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range",
                                                                                         "Expected: -4096 <= imm <= 4095",
                                                                                         filename_,
                                                                                         block.getLineNumber(),
                                                                                         0,
                                                                                         getLineFromFile(filename_,
                                                                                                         block.getLineNumber())));
                        continue;
                    }
                } else {
                    errors_.count++;
                    recordError(ParseError(block.getLineNumber(), "Invalid label reference: Label references data"));
                    errors_.all_errors.emplace_back(
                        Errors::InvalidLabelRefError("Invalid label reference", "Label references data",
                                                     filename_,
                                                     block.getLineNumber(),
                                                     0,
                                                     getLineFromFile(filename_, block.getLineNumber())));
                }
            } else if (InstructionSet::isValidJTypeInstruction(block.getOpcode())) {
                if (!symbol_table_[block.getImm()].isData) {
                    uint64_t address = symbol_table_[block.getImm()].address;
                    auto offset = static_cast<int64_t>(address - index * 4);
                    if (-1048576 <= offset && offset <= 1048575) {
                        block.setImm(std::to_string(offset));
                        block.setLabel(block.getImm());
                    } else {
                        errors_.count++;
                        recordError(ParseError(block.getLineNumber(), "Immediate value out of range"));
                        errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range",
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
                        errors_.count++;
                        recordError(ParseError(block.getLineNumber(), "Immediate value out of range"));
                        errors_.all_errors.emplace_back(Errors::ImmediateOutOfRangeError("Immediate value out of range",
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
                errors_.count++;
                recordError(ParseError(block.getLineNumber(), "Invalid label reference: Label references data"));
                errors_.all_errors.emplace_back(
                    Errors::InvalidLabelRefError("Invalid label reference", "Label reference not found", filename_,
                                                 block.getLineNumber(), 0,
                                                 getLineFromFile(filename_, block.getLineNumber())));
                continue;
            }

            IntermediateCode[index].first = block;
            IntermediateCode[index].second = true;
        } else {
            errors_.count++;
            recordError(ParseError(block.getLineNumber(), "Invalid label reference: Label reference not found"));
            errors_.all_errors.emplace_back(
                Errors::InvalidLabelRefError("Invalid label reference", "Label reference not found", filename_,
                                             block.getLineNumber(), 0,
                                             getLineFromFile(filename_, block.getLineNumber())));
        }
    }

}

unsigned int Parser::getErrorCount() const {
    return errors_.count;
}

const std::vector<ParseError> &Parser::getErrors() const {
    return errors_.parse_errors;
}

void Parser::printErrors() const {
    for (const auto &error : errors_.all_errors) {
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

    for (const auto &pair : symbol_table_) {
        std::cout << pair.first << " -> " << pair.second.address << " " << pair.second.isData << '\n';
    }
}

std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, std::string, float, double>> &Parser::getDataBuffer() {
    return data_buffer_;
}

void Parser::printDataBuffers() const {
    auto stringToHex = [](const std::string &str) -> std::string {
        std::string hexString;
        hexString.reserve(str.size() * 3);
        char buffer[4];
        for (unsigned char ch : str) {
            std::snprintf(buffer, sizeof(buffer), "%02x ", ch);
            hexString.append(buffer);
        }
        return hexString;
    };

    for (const auto &data : data_buffer_) {
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

    for (const auto &pair : IntermediateCode) {
        std::cout << pair.first << " -> " << pair.second << '\n';
    }
}

const std::vector<std::pair<ICUnit, bool>> &Parser::getIntermediateCode() const {
    return IntermediateCode;
}

const std::map<unsigned int, unsigned int> &Parser::getInstructionNumberLineNumberMapping() const {
    return instruction_number_line_number_mapping;
}


