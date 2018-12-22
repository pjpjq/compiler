#include <iostream>
#include <string>

#include "Compiler.h"

#define CIN_TEST_FILE 0
#define USE_MAC_CLION 1

int main() {
    int n_tests = 1;
    std::string test_file_path = "15241035_test.txt";
    if (CIN_TEST_FILE) {
        std::cout << "Enter test file name: ";
        std::cin >> test_file_path;
    }
    
    std::string messages_file_path = "messages_" + test_file_path;
    std::string dst_file_path = "dst_" + test_file_path + ".asm";
    
    if (USE_MAC_CLION) {
        std::cout << "[Warning] 注意当前代码路径仅适用于 Mac Clion 而不是 CodeBlocks!" << std::endl;
        const std::string mac_clion_tests_prefix = "../tests/";
        test_file_path = mac_clion_tests_prefix + test_file_path;
        
        const std::string mac_clion_outputs_prefix = "../outputs/";
        messages_file_path = mac_clion_outputs_prefix + messages_file_path;
        dst_file_path = mac_clion_outputs_prefix + dst_file_path;
    }
    
    bool cout_messages = true;
    for (int i = 0; i < n_tests; ++i) {
        bool compiled_successfully = compile(test_file_path, messages_file_path, dst_file_path, cout_messages);
        if (!compiled_successfully) {
            std::cout << "Compiled failed!" << std::endl;
        }
    }
    return 0;
}