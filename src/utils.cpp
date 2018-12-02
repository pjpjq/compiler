//
// Created by monotasker on 2018/11/29.
//

#include "utils.h"

void error_message(const std::string &error_info, int line_num) {
    std::cout << "[ERROR] Line " << (line_num > 0 ? line_num : tokens[cur_token_idx].get_line_num()) << ": "
              << error_info << std::endl;
    ++n_errors;
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