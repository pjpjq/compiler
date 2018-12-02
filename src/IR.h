//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_IR_H
#define COMPILER_IR_H

#define COUT_QUADRUPLE 0
#define FOUT_QUADRUPLE 1

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "constants.h"
#include "utils.h"

class Quadruple;

/* ��˳���ŵ�ǰ���еı�ǩ */
extern std::vector<int> labels;
/* ��˳���ŵ�ǰ���е���ʱ���� */
extern std::vector<int> temp_vars;
/* �м�������� */
extern std::vector<Quadruple> quadruples;

/**
 * ��ʼ���м����
 */
void init_IR();

/**
 * �ж�һ�� str �ǲ��� label: "@label_i_function name_info" ��
 * @param str
 * @return
 */
bool is_label(const std::string &str);

/**
 * �����ɵ���Ԫʽ�ӵ��м������
 * @param quadruple
 */
void emit(Quadruple quadruple);

/**
 * ��һ���µ���ʱ����
 * @return ����ʱ������ string ��ʾ: "#i"
 */
std::string create_temp_var();

/**
 * �ж�һ�� str �ǲ��� ��ʱ����
 * @param str
 * @return
 */
bool is_temp_var(const std::string &str);

/**
 * ��һ���µ� label
 * @return �µ� label �� string ��ʾ: "@label_i_function name_info"
 */
std::string create_label(const std::string &function_name, const std::string &info);

class Quadruple {
    /**
     * ����ÿ����Ԫʽ�������ʽ
     * @param out
     * @param quadruple
     * @return
     */
    friend std::ostream &operator<<(std::ostream &out, const Quadruple &quadruple) {
        if (quadruple.op == PLUS_SYM || quadruple.op == MINUS_SYM || quadruple.op == MUL_SYM ||
            quadruple.op == DIV_SYM || quadruple.op == EQ_SYM || quadruple.op == NE_SYM || quadruple.op == LT_SYM ||
            quadruple.op == GT_SYM || quadruple.op == LE_SYM || quadruple.op == GE_SYM) {
            /* ��Ϊ����Ҫֱ����� op, ���ԾͲ�����ӿո��׺�� op ��, ����ֱ����֮ǰ�ʷ������ĸ������� sym */
            return out << quadruple.result << " = " << quadruple.left << " " << quadruple.op << " " << quadruple.right;
        }
        if (quadruple.op == DECLARE_FUNCTION_OP) { /* left: ��������, right: ������ */
            if (quadruple.left != INT_SYM && quadruple.left != CHAR_SYM && quadruple.left != VOID_SYM) {
                error_message(
                        "Quadruple declare_function expects int/char/void as left, instead got: " + quadruple.left);
            }
            return out << quadruple.left << " " << quadruple.right << "()";
        }
        if (quadruple.op == DEFINE_PARAMETER_OP) { /* left: ��������, right: ������ */
            if (quadruple.left != INT_SYM && quadruple.left != CHAR_SYM) {
                error_message("Quadruple define_parameter expects int/char as left, instead got: " + quadruple.left);
            }
            return out << "para " << quadruple.left << " " << quadruple.right;
        }
        if (quadruple.op == PUSH_PARAMETER_OP) { /* left: ������ */
            return out << "push " << quadruple.left;
        }
        if (quadruple.op == CALL_FUNCTION_OP) { /* left: ������ */
            return out << "call " << quadruple.left;
        }
        if (quadruple.op == ASSIGN_RETURN_VAL_OP) { /* result: ����ֵ��ֵ�ı��� */
            return out << quadruple.result << " = RET";
        }
        if (quadruple.op == ASSIGN_OP) {
            return out << quadruple.result << " = " << quadruple.left;
        }
        if (quadruple.op == RETURN_OP) { /* left: ���صı��� */
            return out << "ret " << quadruple.left;
        }
        if (quadruple.op == DECLARE_VARIABLE_OP) { /* left: ����, right: ������, result: length */
            if (quadruple.left != INT_SYM && quadruple.left != CHAR_SYM) {
                error_message("Quadruple declare_variable expects int/char as left, instead got: " + quadruple.left);
            }
            out << "var " << quadruple.left << " " << quadruple.right;
            if (std::stoi(quadruple.result) > 0) { /* ������ */
                out << "[" << quadruple.result << "]";
            }
            return out;
        }
//        if (quadruple.op == DECLARE_CONST_OP) {
//            if (quadruple.left != INT_SYM && quadruple.left != CHAR_SYM) {
//                error_message("Quadruple declare_const should begin with int/char, instead got: " + quadruple.left);
//            }
//            return out << "const " << quadruple.left << " " << quadruple.right << " = " << quadruple.result;
//        }
        if (quadruple.op == JUMP_OP) { /* result: label */
            if (!is_label(quadruple.result)) {
                error_message("Quadruple jump_op expects label as result, instead got: " + quadruple.result);
            }
            return out << "j " << quadruple.result;
        }
        if (quadruple.op == BZ_OP) { /* result: label */
            if (!is_label(quadruple.result)) {
                error_message("Quadruple bz_op expects label as result, instead got: " + quadruple.result);
            }
            return out << "bz " << quadruple.result;
        }
        if (quadruple.op == BNZ_OP) { /* result: label */
            if (!is_label(quadruple.result)) {
                error_message("Quadruple bnz_op expects label as result, instead got: " + quadruple.result);
            }
            return out << "bnz " << quadruple.result;
        }
        if (quadruple.op == SCANF_OP) { /* left: var type, right: var name */
            if (quadruple.left != INT_SYM && quadruple.left != CHAR_SYM) {
                error_message("Quadruple scanf_op expects int/char as left, instead got: " + quadruple.left);
            }
            return out << "scanf " << quadruple.left << " " << quadruple.right;
        }
        if (quadruple.op == PRINTF_OP) {
            if (quadruple.left == INT_SYM || quadruple.left == CHAR_SYM) { /* left: var type, right: var name */
                return out << "print " << quadruple.left << " " << quadruple.right;
            } else if (quadruple.left == PRINTF_STRING_TYPE_SYM) { /* left: string type, right: string val */
                return out << "print " << quadruple.left << " " << quadruple.right;
            }
            error_message("Quadruple printf_op expects int/char/string as left, instead got: " + quadruple.left);
            return out << "[ERROR] Invalid printf_op in quadruple" << quadruple.left;
        }
        if (quadruple.op == READ_ARRAY_OP) { /* left: array_name, right: array_offset, result: getter */
            return out << quadruple.result << " = " << quadruple.left << "[" << quadruple.right << "]";
        }
        if (quadruple.op == WRITE_ARRAY_OP) { /* left: array_name, right: array_offset, result: gettee */
            return out << quadruple.left << "[" << quadruple.right << "]" << " = " << quadruple.result;
        }
        if (quadruple.op == LABEL_OP) { /* left: label_name */
            if (!is_label(quadruple.left)) {
                error_message("Quadruple label_op expects label as left, instead got: " + quadruple.left);
            }
            return out << quadruple.left << ":";
        }
        if (quadruple.op == EXIT_OP) {
            return out << "exit";
        }
        /* Shouldn't reach here! */
        error_message("Invalid op in quadruple: " + quadruple.op);
        return out << "[ERROR] " << quadruple.op;
    }

public:
    Quadruple(const std::string &op, const std::string &left, const std::string &right, const std::string &result);

private:
    std::string op;
    std::string left;
    std::string right;
    std::string result;
};

#endif //COMPILER_IR_H
