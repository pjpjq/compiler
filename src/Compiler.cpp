//
// Created by monotasker on 2018/11/13.
//

#include <iostream>
#include "Compiler.h"

std::fstream source_file;
std::fstream output_file;
char cur_ch;
std::string buffer;
Token cur_token;
int line_count;
int n_errors;
int n_tokens;

void init_compiler() {
    cur_ch = '0';
    buffer = "";
    line_count = 1;
    n_errors = 0;
    n_tokens = 0;
}

bool redirect_cout(const std::string &output_file_path) {
    assert (!output_file_path.empty());
    
    output_file = std::fstream(output_file_path, std::ios::out);
    if (!output_file.is_open()) {
        std::cout << "[ERROR] " << output_file_path << " cannot be opened!" << std::endl;
        return false;
    }
    std::cout.rdbuf(output_file.rdbuf());
    return true;
}

bool cout_output_file(const std::string &output_file_path, std::streambuf *cout_buf) {
    assert (!output_file_path.empty());
    
    std::cout.rdbuf(cout_buf);
    output_file = std::fstream(output_file_path, std::ios::in);
    if (!output_file.is_open()) {
        std::cout << "[ERROR] " << output_file_path << " cannot be opened!" << std::endl;
        return false;
    }
    std::cout << output_file.rdbuf();
    return true;
}

bool compile(const std::string &source_file_path, const std::string &output_file_path,
             bool output_file_and_std) {
    source_file = std::fstream(source_file_path, std::ios::in);
    if (!source_file.is_open()) {
        std::cout << "[ERROR] " << source_file_path << " doesn't exist!" << std::endl;
        return false;
    }
    
    /* 输出路径非空就输出到文件 */
    std::streambuf *cout_buf = std::cout.rdbuf();
    if (!output_file_path.empty()) {
        if (!redirect_cout(output_file_path)) {
            return false;
        }
    }
    
    init_compiler();
    while (!source_file.eof()) {
        cur_token = fetch_token();
        ++n_tokens;
        std::cout << n_tokens << " " << cur_token.get_output_string() << " in line " << line_count << std::endl;
    }
    
    /* 两个都输出 */
    if (!output_file_path.empty() && output_file_and_std) {
        if (!cout_output_file(output_file_path, cout_buf)) {
            return false;
        }
    }
    return true;
}
