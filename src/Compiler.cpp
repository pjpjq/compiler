//
// Created by monotasker on 2018/11/13.
//

#include "Compiler.h"

std::fstream output_file;
std::string source_file_in_str;

/* 词法分析 */
int cur_ch_idx;
char cur_ch;
std::string token_buffer;
int line_count;
int n_errors;
std::vector<Token> tokens;
int cur_token_idx;

void init_compiler() {
    cur_ch_idx = 0;
    cur_ch = '0';
    token_buffer = "";
    line_count = 1;
    n_errors = 0;
    tokens.clear();
    cur_token_idx = 0;
    
    init_symbol_tables();
    init_IR();
}

void report_n_errors() {
    std::cout << std::endl << "Finished compilation with " << n_errors << " errors!" << std::endl;
}

bool compile(const std::string &test_file_path, const std::string &messages_file_path, const std::string &dst_file_path,
             bool cout_messages) {
    /* 把源文件存在 string 里 */
    std::ifstream source_file(test_file_path);
    if (!source_file.is_open()) {
        std::cout << "[ERROR] " << test_file_path << " cannot be opened!" << std::endl;
        return false;
    }
    std::stringstream ss;
    ss << source_file.rdbuf();
    source_file_in_str = ss.str();
    
    /* messages 输出到文件 */
    std::streambuf *cout_buf = std::cout.rdbuf();
    redirect_cout_to(messages_file_path);
    
    /*------------开始计时.-----------*/
    auto start = std::chrono::high_resolution_clock::now();
    init_compiler();
    parse_program();
    generate_code(dst_file_path);
    report_n_errors();
    /* -----------停止计时. -----------*/
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Compiled " << test_file_path << " -> " << dst_file_path << " (" << duration.count() << " mu-secs)\n";
    if (cout_messages) {    /* 输出 messages 到 stdout */
        return cout_output_file(messages_file_path, cout_buf);
    }
    return true;
}
