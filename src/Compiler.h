//
// Created by monotasker on 2018/11/13.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "utils.h"
#include "IR.h"
#include "CodeGen.h"

void init_compiler();

/**
 *
 * @param test_file_path
 * @param messages_file_path
 * @param cout_messages
 * @return
 */
bool compile(const std::string &test_file_path, const std::string &messages_file_path, const std::string &dst_file_path,
             bool cout_messages);

void report_n_errors();

#endif //COMPILER_COMPILER_H
