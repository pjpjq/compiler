cmake_minimum_required(VERSION 3.12)
project(compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(compiler src/main.cpp src/Lexer.cpp src/Lexer.h src/Compiler.cpp src/Compiler.h)