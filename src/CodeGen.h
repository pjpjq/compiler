//
// Created by monotasker on 2018/12/7.
//

#ifndef COMPILER_CODEGEN_H
#define COMPILER_CODEGEN_H

#define COUT_CODE 0
#define FOUT_CODE 1

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>

#include "IR.h"
#include "utils.h"
#include "constants.h"
#include "SymbolTable.h"
#include "Parser.h"
//#include "Reg.h"

/* 要输出的 string */
extern std::vector<std::string> printf_strs;
/* 按顺序存放当前已有的标签 */
extern std::vector<int> labels;
/* 按顺序存放当前已有的临时变量 */
extern std::vector<int> temp_vars;
/* 中间代码序列 */
extern std::vector<Quadruple> quadruples;


void init_code_generator();

void init_reg_indices();

void generate_data_section();

std::string get_reg(const std::string &ir_var_name, std::vector<std::string> &conflict_ir_vars);

void generate_line(const std::string &str);

void generate_function_declaration(const std::string &function_name);

void generate_var_or_param_declaration(const std::string &function_name, const std::string &ir_var);

void generate_function_call(const std::string &function_name, const std::string &callee);

void generate_calculation(const std::string &function_name, const Quadruple &quadruple);

void assign_param(const std::string &function, const std::string &param);

void parse_quadruples();

void generate_code(const std::string &dst_file_path);
//void record_busy_regs(std::vector<std::string> &busy_regs);
//void generate_store_busy_regs(std::vector<std::string> &busy_regs, int offset);
//void generate_load_busy_regs(std::vector<std::string> &busy_regs, int offset);
//void write_back_all_regs();
//void init_var_busy_regs();
//void init_all_regs();
//void store_global_regs();
//void before_branch_jump();
//void before_label();
//void before_return();
//void local_dirty_regs_op(void(Reg::*op)(), bool reverse = false);
//void reg2ir_var(const std::string &function_name, const std::string &reg, const std::string &ir_var);
//void ir_var2reg(const std::string &function_name, const std::string &ir_var, const std::string &reg);

#endif //COMPILER_CODEGEN_H
