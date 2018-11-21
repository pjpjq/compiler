//
// Created by monotasker on 2018/11/13.
//

#include "Compiler.h"

std::fstream output_file;

std::string source_file_str;

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

void print_compiler_results() {
    std::cout << std::endl << "Finished compilation with " << n_errors << " errors!" << std::endl;
}

bool compile(const std::string &source_file_path, const std::string &output_file_path,
             bool output_file_and_std) {
    /* 把源文件存在 string 里 */
    std::ifstream source_file(source_file_path);
    if (!source_file.is_open()) {
        std::cout << "[ERROR] " << source_file_path << " cannot be opened!" << std::endl;
        return false;
    }
    std::stringstream ss;
    ss << source_file.rdbuf();
    source_file_str = ss.str();
    
    /* 输出路径非空就输出到文件 */
    std::streambuf *cout_buf = std::cout.rdbuf();
    if (!output_file_path.empty()) {
        if (!redirect_cout(output_file_path)) {
            return false;
        }
    }
    
    /* -----------------------------------开始计时. -------------------------------------------*/
    auto start = std::chrono::high_resolution_clock::now();
    
    init_compiler();
    /* Parse the source program. */
    parse_program();
    print_compiler_results();
    
    /* -----------------------------------停止计时. -------------------------------------------*/
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Compiled in: " << duration.count() << " microseconds." << std::endl;
    
    /* 测试 peek_token() */
//    bool print_tokens = false;
//    for (int i = 0; i < 5; ++i) {
//    while (!source_file.eof()) {
//        fetch_token();
//        Token next_token = peek_token();
//        std::cout << n_tokens << " " << next_token.get_output_string() << " in line " << line_count << std::endl;
//        assert(next_token.get_output_string() == cur_token.get_output_string());
//        std::cout << n_tokens << " " << cur_token.get_output_string() << " in line " << line_count << std::endl;
//        if (print_tokens) {
//            std::cout << n_tokens << " " << cur_token.get_output_string() << " in line " << line_count << std::endl;
//        }
//    }
    
    /* 输出到文件以及 stdout */
    if (!output_file_path.empty() && output_file_and_std) {
        if (!cout_output_file(output_file_path, cout_buf)) {
            return false;
        }
    }
    return true;
}
