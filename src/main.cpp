#define NDEBUG
#include <cassert>

#include <iostream>
#include <string>

#include "Compiler.h"

int main() {
    bool use_mac_clion = true;
    
    /* 测试 n_tests 次 */
    int n_tests = 1;
    
    const std::string test_file_name = "15241035_test.txt";
    const std::string test_result_file_name = "15241035_test_result.txt";
    
    /* Win codeblocks path */
    std::string source_file_path = test_file_name;
    std::string output_file_path = test_result_file_name;
    
    /* Mac clion path */
    if (use_mac_clion) {
        std::cout << "[Warning] 注意当前代码路径仅适用于 Mac Clion 而不是 CodeBlocks!" << std::endl;
        const std::string mac_clion_tests_prefix = "../tests/";
        const std::string mac_clion_outputs_prefix = "../outputs/";
        source_file_path = mac_clion_tests_prefix + test_file_name;
        output_file_path = mac_clion_outputs_prefix + test_result_file_name;
    }
    
    bool output_file_and_std = true;
    for (int i = 0; i < n_tests; ++i) {
        bool compiled_successfully = compile(source_file_path, output_file_path, output_file_and_std);
        if (!compiled_successfully) {
            std::cout << "Compiled failed!" << std::endl;
        }
    }
    
    return 0;
}