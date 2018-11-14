#include <iostream>
#include <cassert>
#include <string>

#include "Compiler.h"

int main() {
    /* ≤‚ ‘ n_tests ¥Œ */
    int n_tests = 1;
    const std::string source_file_path = "../tests/test1.txt";
    const std::string output_file_path = "../outputs/test1_result.txt";
    bool output_file_and_std = true;
    for (int i = 0; i < n_tests; ++i) {
        compile(source_file_path, output_file_path, output_file_and_std);
    }
    return 0;
}