//
// Created by monotasker on 2018/11/29.
//

#include "utils.h"

void error_message(const std::string &error_info) {
    std::cout << "[ERROR] Line " << tokens[cur_token_idx].get_line_nbr() << " : " << error_info << std::endl;
    ++n_errors;
}