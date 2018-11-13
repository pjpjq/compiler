//
// Created by monotasker on 2018/11/13.
//

#include <iostream>
#include "Compiler.h"

void init_compiler() {
    cur_ch = '0';
    buffer = "";
    line_count = 0;
    n_errors = 0;
}

int compile(const std::string &source_file_path) {
    source_file = std::fstream(source_file_path, std::ios::in);
    if (!source_file) {
        std::cout << "[ERROR] " << source_file_path << " doesn't exist!" << std::endl;
        return -1;
    }
    init_compiler();
    while (!source_file.eof()) {
        fetch_token();
    }
    return 0;
}
