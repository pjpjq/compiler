//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Token.h"

extern std::fstream output_file;

/**
 * 打印错误信息及当前单词所在位置(行号), line_num 默认为 0, 输出当前单词的 line_num, 否则则按指定输出(词法分析时)
 * TODO 完善错误处理
 * @param error_info
 * @param line_num
 */
void error_message(const std::string &error_info, int line_num = 0);

bool redirect_cout(const std::string &output_file_path);

bool cout_output_file(const std::string &output_file_path, std::streambuf *cout_buf);

#endif //COMPILER_UTIL_H
