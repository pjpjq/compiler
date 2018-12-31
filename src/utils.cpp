//
// Created by monotasker on 2018/11/29.
//

#include "utils.h"

void skip2before(const std::string &target_token) {
    while (cur_token_idx + 1 < tokens.size() && tokens[cur_token_idx + 1].get_val_string() != target_token &&
           tokens[cur_token_idx + 1].get_val_string() != SEMICOLON_SYM) {
        ++cur_token_idx;
    }
}

void error_message(const std::string &error_info, int line_num) {
    std::cout << "[ERROR] Line " << (line_num > 0 ? line_num : tokens[cur_token_idx].get_line_num()) << ": "
              << error_info << std::endl;
    ++n_errors;
}

void warning_message(const std::string &warning_info, int line_num) {
    std::cout << "[WARNING] Line " << (line_num > 0 ? line_num : tokens[cur_token_idx].get_line_num()) << ": "
              << warning_info << std::endl;
}

bool redirect_cout_to(const std::string &output_file_path) {
    if (output_file_path.empty()) {
        error_message("Output file empty: " + output_file_path);
        return false;
    }
    output_file = std::fstream(output_file_path, std::ios::out);
    if (!output_file.is_open()) {
        error_message("Output file cannot be opened: " + output_file_path);
        return false;
    }
    std::cout.rdbuf(output_file.rdbuf());
    return true;
}

bool cout_output_file(const std::string &output_file_path, std::streambuf *cout_buf) {
    if (output_file_path.empty()) {
        error_message("Output file empty: " + output_file_path);
        return false;
    }
    std::cout.rdbuf(cout_buf);
    output_file = std::fstream(output_file_path, std::ios::in);
    if (!output_file.is_open()) {
        error_message("Output file cannot be opened: " + output_file_path);
        return false;
    }
    std::cout << output_file.rdbuf();
    return true;
}

bool is_string_number(const std::string &str) {
    if (str.empty()) {
        error_message("Empty string passed in checking number: " + str);
        return false;
    }
    if (str[0] == '-') { /* 有 - 号 */
        return str.substr(1).find_first_not_of("0123456789") == std::string::npos;
    } else { /* 正整数 */
        return str.find_first_not_of("0123456789") == std::string::npos;
    }
}


