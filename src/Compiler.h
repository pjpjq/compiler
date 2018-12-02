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

void init_compiler();

/**
 *
 * @param source_file_path
 * @param output_file_path
 * @param output_file_and_std
 * @return
 */
bool compile(const std::string &source_file_path, const std::string &output_file_path = "",
             bool output_file_and_std = true);

void print_compiler_results();

#endif //COMPILER_COMPILER_H
