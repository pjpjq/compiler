cmake_minimum_required(VERSION 3.12)
project(compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(compiler src/main.cpp src/Lexer.cpp src/Lexer.h src/Compiler.cpp src/Compiler.h src/Parser.cpp src/Parser.h src/constants.h src/SymbolTable.cpp src/SymbolTable.h src/utils.cpp src/utils.h src/Token.cpp src/Token.h src/IR.cpp src/IR.h src/CodeGen.cpp src/CodeGen.h src/constants.cpp src/Reg.cpp src/Reg.h)