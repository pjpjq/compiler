//
// Created by monotasker on 2018/11/13.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <fstream>
#include <string>

#include "Lexer.h"

extern std::fstream source_file;
char cur_ch;
std::string buffer;
Token cur_token;
int line_count;
int n_errors;

void init_compiler();

/**
 * C0 编译器 API
 * @param source_file_path 源代码文件路径
 */
int compile(const std::string &source_file_path);

void print_compiler_results();

#endif //COMPILER_COMPILER_H
