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
 * ��ӡ������Ϣ����ǰ��������λ��(�к�), line_num Ĭ��Ϊ 0, �����ǰ���ʵ� line_num, ������ָ�����(�ʷ�����ʱ)
 * TODO ���ƴ�����
 * @param error_info
 * @param line_num
 */
void error_message(const std::string &error_info, int line_num = 0);

bool redirect_cout(const std::string &output_file_path);

bool cout_output_file(const std::string &output_file_path, std::streambuf *cout_buf);

#endif //COMPILER_UTIL_H
