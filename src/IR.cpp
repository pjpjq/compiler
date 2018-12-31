//
// Created by monotasker on 2018/11/29.
//

#include "IR.h"

std::vector<int> labels;
std::vector<int> temp_vars;
std::vector<Quadruple> quadruples;

const std::string ir_fout_name = "../outputs/ir.txt";
std::ofstream ir_fout;

void init_IR() {
    labels.clear();
    temp_vars.clear();
    quadruples.clear();
    
    ir_fout.open(ir_fout_name);
    if (!ir_fout) {
        ir_fout.open("./ir.txt");
    }
    if (!ir_fout) {
        error_message("IR file cannot be opened!");
    }
}

void emit(Quadruple quadruple) {
    quadruples.push_back(quadruple);
    if (COUT_QUADRUPLE) {
        std::cout << quadruple << std::endl;
    }
    if (FOUT_QUADRUPLE) {
        ir_fout << quadruple << std::endl;
    }
}

std::string create_temp_var() {
    if (temp_vars.empty()) {
        temp_vars.push_back(0);
    } else {
        temp_vars.push_back(temp_vars.back() + 1);
    }
    assert(!temp_vars.empty());
    std::string res = "#" + std::to_string(temp_vars.back());
    assert(is_temp_var(res));
    return res;
}

bool is_temp_var(const std::string &str) {
    return is_ir_var(str) && is_string_number(str.substr(1));
}

bool is_ir_var(const std::string &str) {
    return !str.empty() && str[0] == '#';
}

std::string create_label(const std::string &function_name, const std::string &info) {
    if (labels.empty()) {
        labels.push_back(0);
    } else {
        labels.push_back(labels.back() + 1);
    }
    assert(!labels.empty());
    std::string res = "@label_" + std::to_string(labels.back()) + "_" + function_name + "_" + info;
    assert(is_numbered_label(res));
    return res;
}

bool is_numbered_label(const std::string &str) {
    return !str.empty() && str[0] == '@' && str.find("@label_") != std::string::npos;
}


Quadruple::Quadruple(const std::string &op, const std::string &left, const std::string &right,
                     const std::string &result) : op(op), left(left), right(right), result(result) {}
