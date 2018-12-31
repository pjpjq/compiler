//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

#include "Token.h"
#include "constants.h"

extern std::fstream output_file;

extern std::vector<Token> tokens;
extern int cur_token_idx;

void skip2before(const std::string &dst_token = "");
/**
 * ��ӡ������Ϣ����ǰ��������λ��(�к�), line_num Ĭ��Ϊ 0, �����ǰ���ʵ� line_num, ������ָ�����(�ʷ�����ʱ)
 * @param error_info
 * @param line_num
 */
void error_message(const std::string &error_info, int line_num = 0);

void warning_message(const std::string &warning_info, int line_num = 0);

bool redirect_cout_to(const std::string &output_file_path);

bool cout_output_file(const std::string &output_file_path, std::streambuf *cout_buf);

/**
 * ���һ�� str �ǲ�����: ���Դ� - ��, �������Դ� + ��
 * @param str
 * @return
 */
bool is_string_number(const std::string &str);

#endif //COMPILER_UTIL_H
