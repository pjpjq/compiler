//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include "Token.h"

extern std::string source_file_str;
extern int cur_ch_idx;
extern char cur_ch;
extern std::string token_buffer;
extern int line_count;
extern int n_errors;
extern std::vector<Token> tokens;
extern int cur_token_idx;

/**
 * ��ӡ������Ϣ����ǰ��������λ��(�к�), TODO ���ƴ�����
 * @param error_info
 */
void error_message(const std::string &error_info);

#endif //COMPILER_UTIL_H
