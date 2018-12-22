//
// Created by monotasker on 2018/12/7.
//

#include "CodeGen.h"

struct Reg {
    int offset = -1;
    int n_uses = 0;
    std::string name;
    bool is_active = false;
    bool is_int = 1;
    bool is_global = false;
};
const int N_REGS = 8;
int next_reg_idx = 0;
Reg regs[20];
std::unordered_map<std::string, int> reg_indices;

std::vector<std::string> lines;
std::unordered_map<std::string, int> gp_offsets;
//std::unordered_map<std::string, std::unordered_map<std::string, int> > sp_offsets;
std::unordered_map<std::string, int> local_offsets;
std::string cur_function_name;
//std::unordered_map<std::string, int> function_stack_sizes;
//int gp_stack_size;
int temp_base_addr = 0;
int int_ptr = 0;  // 统一先用再分配给下一个
int cur_addr = 0;
int n_param_read = 0;
std::vector<std::string> value_params;

//unsigned int available_reg_idx;
//std::vector<std::string> REGS = {"$t0", "$t1", "$t2"};

std::string code_fout_name;
std::ofstream code_fout;

Quadruple cur_quad = Quadruple(EXIT_OP, "", "", "");  // for debugging

//std::unordered_map<std::string, Reg *> regs;
//std::unordered_map<std::string, Reg *> content2reg;
//std::set<std::string> free_temps_vars;

void init_code_generator() {
    lines.clear();
    gp_offsets.clear();
//    sp_offsets.clear();
    local_offsets.clear();
//    function_stack_sizes.clear();
//    gp_stack_size = 0;
    cur_function_name = "";
    temp_base_addr = 0;
    int_ptr = 0;
    cur_addr = 0;
    n_param_read = 0;
    value_params.clear();
//    available_reg_idx = 0;
    
    code_fout.open(code_fout_name);
    if (!code_fout) {
        error_message("Cannot open file: " + code_fout_name);
        return;
    }
    
    cur_quad = Quadruple(EXIT_OP, "", "", "");
}

void init_reg_indices() { // save regs
    reg_indices.clear();
    for (int i = 0; i < N_REGS; ++i) {
        if (regs[i].is_active) {
            std::string ptr = regs[i].is_global ? "($gp)" : "($fp)";
            generate_line("sw $s" + std::to_string(i) + ", " + std::to_string(regs[i].offset) + ptr);
            regs[i].is_active = false;
        }
    }
}

void generate_data_section() {
    generate_line(".data");
    generate_line("newline: .asciiz \"\\n\"");
    for (int i = 0; i < printf_strs.size(); ++i) {
        generate_line("str_" + std::to_string(i) + ": .asciiz \"" + printf_strs[i] + "\"");
    }
    generate_line(".space 4");
    generate_line("");
    generate_line("Text: ");
    generate_line(".text");
    generate_line("la $gp, Text"); // ????????????????????
    generate_line("andi $gp, $gp, 0xFFFFFFFC"); // ????????
    generate_line("");
}

std::string get_reg(const std::string &content_name, std::vector<std::string> &conflict_names) {
    bool is_int = true;
    if (!is_temp_var(content_name)) {
        Symbol *var = get_non_function_symbol(cur_function_name, content_name.substr(1));
        if (!var) { return ">>>>>>????"; }
        is_int = var->get_symbol_type() == INT_SYMBOL_TYPE;
    }
    
    if (reg_indices.count(content_name) > 0) {
        return "$s" + std::to_string(reg_indices[content_name]);
    }
    if (!conflict_names.empty()) {
        bool conflict = false;
        while (true) {
            conflict = false;
            for (int i = 0; i < conflict_names.size(); ++i) {
                if (conflict_names[i] == regs[next_reg_idx].name) {
                    conflict = true;
                    break;
                }
            }
            if (conflict) {
                next_reg_idx = (next_reg_idx + 1) % N_REGS;
            } else {
                break;
            }
        }
    }
    bool keep_active = false;
    if (regs[next_reg_idx].is_active) {
        std::string ptr = regs[next_reg_idx].is_global ? "($gp)" : "($fp)";
        generate_line("sw $s" + std::to_string(next_reg_idx) + ", " + std::to_string(regs[next_reg_idx].offset) + ptr);
        keep_active = true;
    } else {
        keep_active = false;
        regs[next_reg_idx].is_active = true;
    }
    
    if (is_temp_var(content_name)) {
        regs[next_reg_idx].offset = temp_base_addr + std::stoi(content_name.substr(1)) * 4;
        cur_addr = std::max(regs[next_reg_idx].offset + 4, cur_addr);
        regs[next_reg_idx].is_global = false;
    } else {
        if (local_offsets.count(content_name) > 0) {
            regs[next_reg_idx].offset = local_offsets[content_name];
            regs[next_reg_idx].is_global = false;
        } else {
            if (gp_offsets.count(content_name) > 0) {
                regs[next_reg_idx].offset = gp_offsets[content_name];
                regs[next_reg_idx].is_global = true;
            } else {
                error_message("No such var: " + content_name);
            }
        }
    }
    
    std::string ptr = regs[next_reg_idx].is_global ? "($gp)" : "($fp)";
    generate_line("lw $s" + std::to_string(next_reg_idx) + ", " + std::to_string(regs[next_reg_idx].offset) + ptr);
    regs[next_reg_idx].n_uses = 0;
    regs[next_reg_idx].is_int = is_int;
    if (keep_active) {
        if (reg_indices.count(regs[next_reg_idx].name) == 0) {
            error_message("No such using content_name: " + regs[next_reg_idx].name);
        } else {
            reg_indices.erase(reg_indices.find(regs[next_reg_idx].name));
        }
    }
    
    regs[next_reg_idx].name = content_name;
    reg_indices[content_name] = next_reg_idx;
    
    std::string res = std::to_string(next_reg_idx);
    next_reg_idx = (next_reg_idx + 1) % N_REGS;
    return "$s" + res;
}

//void record_busy_regs(std::vector<std::string> &busy_regs) {
////    for (const auto &it : regs) {
////        if (it.second->state == BUSY) {
////            busy_regs.push_back(it.second->reg_name);
////        }
////    }
//}
//
//void generate_store_busy_regs(std::vector<std::string> &busy_regs, int offset) {
////    for (const auto &it : busy_regs) {
////        generate_line("sw " + it + ", " + std::to_string(offset) + "($sp)");
////        offset += 4;
////    }
//}
//
//void generate_load_busy_regs(std::vector<std::string> &busy_regs, int offset) {
////    for (const auto &it : busy_regs) {
////        generate_line("lw " + it + "," + std::to_string(offset) + "($sp)");
////        offset += 4;
////    }
//}
//
//void write_back_all_regs() {
////    for (auto &it: regs) {
////        it.second->write_back();
////    }
//}
//
//void init_var_busy_regs() {
////    for (auto &it: regs) {
////        if (it.second->state == BUSY && it.second->reg_name[1] == 's') { // ?
////            it.second->init();
////        }
////    }
//}
//
//void init_all_regs() {
////    for (auto &it : regs) {
////        it.second->init();
////    }
//}
//
//void store_global_regs() {
////    for (auto &it: regs) {
////        if (it.second->is_global) {
////            it.second->store();
////        }
////    }
//}
//
//void before_branch_jump() {
////    local_dirty_regs_op(&Reg::store, false);
////    init_var_busy_regs();
////    local_dirty_regs_op(&Reg::init, true);
//}
//
//void before_label() {
////    local_dirty_regs_op(&Reg::store, false);
////    init_all_regs();
//}
//
//void before_return() {
////    store_global_regs();
////    init_all_regs();
//}

//void local_dirty_regs_op(void (Reg::*op)(), bool reverse) {
////    for (auto &it: regs) {
////        if (is_local_variable(cur_function_name, it.second->content_name) ^ reverse) { // TODO  基本块
////            (it.second->*op)();
////        }
////    }
//}

//std::string get_available_register() {
//    std::string reg = REGS[available_reg_idx];
//    available_reg_idx = (available_reg_idx + 1) % REGS.size();
//    return reg;
//}


void generate_line(const std::string &str) {
    lines.emplace_back(str);
    int width = 45;
    if (COUT_CODE) {
        std::cout.setf(std::ios::left);
        std::cout << std::setw(width) << str;
//        std::cout << std::endl;
        std::cout << "#\t" << cur_quad << std::endl;
    }
    if (FOUT_CODE) {
        code_fout.setf(std::ios::left);
        code_fout << std::setw(width) << str;
//        code_fout << std::endl;
        code_fout << "#\t" << cur_quad << std::endl;
    }
}

void generate_function_declaration(const std::string &function_name) {
    if (!is_function(function_name)) {
        error_message("Defining invalid function: " + function_name);
        return;
    }
    local_offsets.clear();
    cur_function_name = function_name;
    temp_base_addr = 0;
    int_ptr = 0;
    cur_addr = 0;
    n_param_read = 0;
    
    init_reg_indices();
    
    generate_line("##########################################");
    generate_line(function_name + ": "); // function label
}

void generate_var_or_param_declaration(const std::string &function_name, const std::string &ir_var) {
    std::string var_name = ir_var.substr(1);
    if (function_name.empty()) {  /* 是 global */
        if (!is_global_variable(var_name)) {
            error_message("Invalid global var: " + var_name);
            return;
        }
        gp_offsets[ir_var] = int_ptr;
        int_ptr += 4 * (get_global_variable(var_name)->is_array() ? get_global_variable(var_name)->get_length() : 1);
    } else {  /* 是 local */
        if (!is_function(function_name)) {
            error_message("Declaring var/param in undefined function: " + function_name);
            return;
        }
        if (!is_local_variable(function_name, var_name) && !is_parameter(function_name, var_name)) {
            error_message("No such local var/param in " + function_name + ": " + ir_var);
            return;
        }
        local_offsets[ir_var] = int_ptr;
        /* 这里用 get_non_function_symbol(), 是因为优先找 local 而已经确保了 local 有 */
        int_ptr += 4 * (get_non_function_symbol(function_name, var_name)->is_array() ?
                        get_non_function_symbol(function_name, var_name)->get_length() : 1);
    }
    cur_addr = temp_base_addr = int_ptr;
}

void generate_function_call(const std::string &caller, const std::string &callee) {
    if (!is_function(callee)) {
        error_message("Calling undefined function: " + callee);
        return;
    }
    if (callee == MAIN_SYM) { /* 调用 main 函数的特殊情况 */
        generate_line("addiu $fp, $fp, " + std::to_string(cur_addr));
        generate_line("addu $fp, $fp, $gp");
        generate_line("jal " + MAIN_SYM);
        generate_line("li $v0, 10");
        generate_line("syscall");
        return;
    }
    
    /* 栈依次: ra a s, 这里 caller 只负责存 value-a */
    int n_params = value_params.size();
    for (int i = 0; i < n_params; ++i) {
        std::string res_offset = std::to_string(cur_addr + i * 4);
        std::string value_param = value_params.back();
        value_params.pop_back();
        
        if (is_string_number(value_param)) {
            generate_line("li $t0, " + value_param);
            generate_line("sw $t0, " + res_offset + "($fp)");
        } else {
            std::vector<std::string> _;
            std::string val_param_reg = get_reg(value_param, _);
            generate_line("sw " + val_param_reg + ", " + res_offset + "($fp)");
        }
    }
    init_reg_indices();
    
    generate_line("addi $sp, $sp, -8");
    generate_line("sw $ra, 0($sp)");
    generate_line("sw $fp, 4($sp)");
    
    generate_line("addi $fp, $fp, " + std::to_string(cur_addr + n_params * 4));
    generate_line("jal " + callee);
    
    generate_line("lw $ra, 0($sp)");
    generate_line("lw $fp, 4($sp)");
    generate_line("addi $sp, $sp, 8");
}

void generate_calculation(const std::string &function_name, const Quadruple &quadruple) { // TODO 检查
    if (!is_function(function_name)) {
        error_message("Trying calculation in invalid function: " + function_name);
        return;
    }
    std::string reg_left, reg_right, reg_res;
    std::vector<std::string> conflict_names{quadruple.left, quadruple.right, quadruple.result};
    
    bool is_left_imm = !is_ir_var(quadruple.left);
    if (!is_left_imm) {
        reg_left = get_reg(quadruple.left, conflict_names);
    }
    bool is_right_imm = !is_ir_var(quadruple.right);
    if (!is_right_imm) {
        reg_right = get_reg(quadruple.right, conflict_names);
    }
    
    reg_res = get_reg(quadruple.result, conflict_names);
    
    if (quadruple.op == PLUS_SYM) {
        if (is_left_imm) {
            generate_line("addiu " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("addiu " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("addu " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == MINUS_SYM) {
        if (is_left_imm) {
            generate_line("addiu " + reg_res + ", " + reg_right + ", -" + quadruple.left);
            generate_line("negu " + reg_res + ", " + reg_res);
        } else if (is_right_imm) {
            generate_line("addiu " + reg_res + ", " + reg_left + ", -" + quadruple.right);
        } else {
            generate_line("sub " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == MUL_SYM) {
        if (is_left_imm) {
            generate_line("mul " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("mul " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("mul " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == DIV_SYM) {
        if (is_left_imm) {
            generate_line("li " + reg_left + ", " + quadruple.left);
            generate_line("div " + reg_res + ", " + reg_left + ", " + reg_right);
        } else if (is_right_imm) {
            generate_line("div " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("div " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == EQ_SYM) {
        if (is_left_imm) {
            generate_line("seq " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("seq " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("seq " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == NE_SYM) {
        if (is_left_imm) {
            generate_line("sne " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("sne " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("sne " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == LT_SYM) {
        if (is_left_imm) {
            generate_line("sgt " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("negu " + reg_left + ", " + reg_left);
            generate_line("sgt " + reg_res + ", " + reg_left + ", -" + quadruple.right);
        } else {
            generate_line("slt " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == GT_SYM) {
        if (is_left_imm) {
            generate_line("negu " + reg_right + ", " + reg_right);
            generate_line("sgt " + reg_res + ", " + reg_right + ", -" + quadruple.left);
        } else if (is_right_imm) {
            generate_line("sgt " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("sgt " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == LE_SYM) {
        if (is_left_imm) {
            generate_line("sge " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("sle " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("sle " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    } else if (quadruple.op == GE_SYM) {
        if (is_left_imm) {
            generate_line("sle " + reg_res + ", " + reg_right + ", " + quadruple.left);
        } else if (is_right_imm) {
            generate_line("sge " + reg_res + ", " + reg_left + ", " + quadruple.right);
        } else {
            generate_line("sge " + reg_res + ", " + reg_left + ", " + reg_right);
        }
    }
}

void assign_param(const std::string &function, const std::string &param) {
    ++n_param_read;
    std::vector<std::string> _;
    std::string param_reg = get_reg(param, _);
    generate_line("lw " + param_reg + ", " + std::to_string(-n_param_read * 4) + "($fp)");
}

void parse_quadruples() {
    for (const Quadruple &quadruple : quadruples) {
        cur_quad = quadruple;
        if (quadruple.op == PLUS_SYM || quadruple.op == MINUS_SYM || quadruple.op == MUL_SYM || quadruple.op == DIV_SYM
            || quadruple.op == EQ_SYM || quadruple.op == NE_SYM || quadruple.op == LT_SYM ||
            quadruple.op == GT_SYM || quadruple.op == LE_SYM || quadruple.op == GE_SYM) {
            generate_calculation(cur_function_name, quadruple);
        } else if (quadruple.op == DECLARE_FUNCTION_OP) { /* left: 返回类型, right: 函数名 */
            generate_function_declaration(quadruple.right);
        } else if (quadruple.op == DECLARE_VARIABLE_OP) { /* left: 类型, right: 变量名, result: length */
            generate_var_or_param_declaration(cur_function_name, quadruple.right);
        } else if (quadruple.op == DEFINE_PARAMETER_OP) { /* left: 参数类型, right: 参数名 */ // ????????????
            generate_var_or_param_declaration(cur_function_name, quadruple.right);
            assign_param(cur_function_name, quadruple.right);
        } else if (quadruple.op == PUSH_PARAMETER_OP) { /* left: 参数名 */
            value_params.push_back(quadruple.left);
        } else if (quadruple.op == CALL_FUNCTION_OP) { /* left: callee 函数名 */
            generate_function_call(cur_function_name, quadruple.left);
        } else if (quadruple.op == ASSIGN_RETURN_VAL_OP) { /* result: 返回值赋值的变量 */
            std::vector<std::string> _;
            std::string reg = get_reg(quadruple.result, _);
            generate_line("move " + reg + ", $v0");
        } else if (quadruple.op == ASSIGN_OP) { /* left: assign left, right: assign right */
            std::vector<std::string> conflict_names{quadruple.left, quadruple.right};
            std::string dst_reg = get_reg(quadruple.left, conflict_names);
            if (!is_ir_var(quadruple.right)) { /* 常数 */
                generate_line("li " + dst_reg + ", " + quadruple.right);
            } else {
                std::string src_reg = get_reg(quadruple.right, conflict_names);
                generate_line("move " + dst_reg + ", " + src_reg);
            }
        } else if (quadruple.op == RETURN_OP) { /* left: 返回的变量, 可以没有 */
            init_reg_indices();
            if (!quadruple.left.empty()) { // 有返回值就保存到 $v0
                if (!is_ir_var(quadruple.left)) {
                    generate_line("li $v0, " + quadruple.left);
                } else {
                    std::vector<std::string> _;
                    std::string reg = get_reg(quadruple.left, _);
                    generate_line("move $v0, " + reg);
                }
            }
            generate_line("jr $ra");
        } else if (quadruple.op == JUMP_OP) {
            init_reg_indices();
            generate_line("j " + (quadruple.result == MAIN_SYM ? MAIN_SYM : quadruple.result.substr(1)));
        } else if (quadruple.op == BZ_OP) { /* left: var, result: label */
            init_reg_indices();
            std::vector<std::string> _;
            std::string reg = get_reg(quadruple.left, _);
            generate_line("beqz " + reg + ", " + quadruple.result.substr(1));
        } else if (quadruple.op == BNZ_OP) { /* left: var, result: label */
            init_reg_indices();
            std::vector<std::string> _;
            std::string reg = get_reg(quadruple.left, _);
            generate_line("bnez " + reg + ", " + quadruple.result.substr(1));
        } else if (quadruple.op == SCANF_OP) { /* left: var type, right: var name */
            std::string v0_val = quadruple.left == INT_SYM ? "5" : quadruple.left == CHAR_SYM ? "12" : "???";
            generate_line("li $v0, " + v0_val);
            generate_line("syscall");
            std::vector<std::string> _;
            std::string reg = get_reg(quadruple.right, _);
            generate_line("move " + reg + ", $v0");
        } else if (quadruple.op == PRINTF_OP) {
            if (quadruple.left == INT_SYM || quadruple.left == CHAR_SYM) { /* left: var type, right: var name/imm */
                std::string v0_val = quadruple.left == INT_SYM ? "1" : "11";
                generate_line("li $v0, " + v0_val);
                if (is_ir_var(quadruple.right)) {
                    std::vector<std::string> _;
                    std::string reg = get_reg(quadruple.right, _);
                    generate_line("move $a0, " + reg);
                } else { /* imm */
                    generate_line("li $a0, " + quadruple.right);
                }
                generate_line("syscall");
                
                generate_line("li $v0, 4");
                generate_line("la $a0, newline");
                generate_line("syscall");
            } else if (quadruple.left == PRINTF_STRING_TYPE_SYM) { /* left: string type, right: str_i */
                generate_line("li $v0, 4");
                generate_line("la $a0, " + quadruple.right);
                generate_line("syscall");
            }
        } else if (quadruple.op == READ_ARRAY_OP) { /* left: array_name, right: array_offset, result: dst */
            std::vector<std::string> _;
            std::string dst_reg = get_reg(quadruple.result, _);
//            int is_int =
//                    get_non_function_symbol(cur_function_name, quadruple.left)->get_symbol_type() == INT_SYMBOL_TYPE;
            int array_base_offset = 0;
            std::string ptr;
            if (local_offsets.find(quadruple.left) != local_offsets.end()) {
                array_base_offset = local_offsets[quadruple.left];
                ptr = "$fp";
            } else if (gp_offsets.find(quadruple.left) != gp_offsets.end()) {
                array_base_offset = gp_offsets[quadruple.left];
                ptr = "$gp";
            } else {
                error_message("No such array: " + quadruple.left);
                continue;
            }
            if (is_string_number(quadruple.right)) {
                generate_line("lw " + dst_reg + ", " +
                              std::to_string(array_base_offset + 4 * std::stoi(quadruple.right)) + "(" + ptr + ")");
            } else {
                std::string offset_reg = get_reg(quadruple.right, _);
                generate_line("sll $v0, " + offset_reg + ", 2");
                generate_line("addu $v0, $v0, " + ptr);
                generate_line("addu $v0, $v0, " + std::to_string(array_base_offset));
                generate_line("lw " + dst_reg + ", ($v0)");
            }
        } else if (quadruple.op == WRITE_ARRAY_OP) { /* left: array_name, right: array_offset, result: src */
            std::vector<std::string> _;
            std::string src_reg;
            if (!is_ir_var(quadruple.result)) {
                generate_line("li $t0, " + quadruple.result);
                src_reg = "$t0";
            } else {
                src_reg = get_reg(quadruple.result, _);
            }
            int array_base_offset = 0;
            std::string ptr;
            if (local_offsets.find(quadruple.left) != local_offsets.end()) {
                array_base_offset = local_offsets[quadruple.left];
                ptr = "$fp";
            } else if (gp_offsets.find(quadruple.left) != gp_offsets.end()) {
                array_base_offset = gp_offsets[quadruple.left];
                ptr = "$gp";
            } else {
                error_message("No such array: " + quadruple.left);
                continue;
            }
            
            if (is_string_number(quadruple.right)) {
                generate_line("sw " + src_reg + ", " +
                              std::to_string(array_base_offset + 4 * std::stoi(quadruple.right)) + "(" + ptr + ")");
            } else {
                std::string offset_reg = get_reg(quadruple.right, _);
                generate_line("sll $v0, " + offset_reg + ", 2");
                generate_line("addu $v0, $v0, " + ptr);
                generate_line("addu $v0, $v0, " + std::to_string(array_base_offset));
                generate_line("sw " + src_reg + ", ($v0)");
            }
        } else if (quadruple.op == LABEL_OP) { /* left: label_name */
            init_reg_indices();
            generate_line(quadruple.left == MAIN_SYM ? quadruple.left : quadruple.left.substr(1) + ": ");
        } else if (quadruple.op == EXIT_OP) { // 有用吗?????????????
            ;
        } else { /* Shouldn't reach here! */
            error_message("Invalid quadruple op: " + quadruple.op);
        }
    }
}

void generate_code(const std::string &dst_file_path) {
    code_fout_name = dst_file_path;
    init_code_generator();
    generate_data_section();
    parse_quadruples();
}
