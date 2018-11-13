#include <iostream>
#include <cassert>
#include <string>

#include "Compiler.h"

int main() {
    /* ≤‚ ‘ n_tests ¥Œ */
    int n_tests = 1;
    const std::string source_file_path = "tests/test1.txt";
    for (int i = 0; i < n_tests; ++i) {
        compile(source_file_path);
    }
    return 0;
}