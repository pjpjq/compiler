//
// Created by monotasker on 2018/11/13.
//

#include "Compiler.h"

std::fstream output_file;
std::string source_file_str;

/* �ʷ����� */
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

void print_compiler_results() {
    std::cout << std::endl << "Finished compilation with " << n_errors << " errors!" << std::endl;
}

bool compile(const std::string &source_file_path, const std::string &output_file_path,
             bool output_file_and_std) {
    /* ��Դ�ļ����� string �� */
    std::ifstream source_file(source_file_path);
    if (!source_file.is_open()) {
        std::cout << "[ERROR] " << source_file_path << " cannot be opened!" << std::endl;
        return false;
    }
    std::stringstream ss;
    ss << source_file.rdbuf();
    source_file_str = ss.str();
    
    /* ���·���ǿվ�������ļ� */
    std::streambuf *cout_buf = std::cout.rdbuf();
    if (!output_file_path.empty()) {
        if (!redirect_cout(output_file_path)) {
            return false;
        }
    }
    
    /* -----------------------------------��ʼ��ʱ. -------------------------------------------*/
    auto start = std::chrono::high_resolution_clock::now();
    
    init_compiler();
    /* Parse the source program. */
    parse_program();
    print_compiler_results();
    
    /* -----------------------------------ֹͣ��ʱ. -------------------------------------------*/
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Compiled in: " << duration.count() << " microseconds." << std::endl;
    
    if (!output_file_path.empty() && output_file_and_std) {  /* ������ļ��Լ� stdout */
        if (!cout_output_file(output_file_path, cout_buf)) {
            return false;
        }
    }
    return true;
}
