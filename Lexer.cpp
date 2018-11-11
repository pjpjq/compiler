//
// Created by monotasker on 2018/11/11.
//

#include "Lexer.h"

bool Lexer::is_alpha_or_underscore(char ch) {
    return std::isalpha(ch) || ch == '_';
}

bool Lexer::is_plus_or_minus_sym(std::string ch) {
    return ch == PLUS_SYM || ch == MINUS_SYM;
}

bool Lexer::is_mul_or_div_sym(std::string ch) {
    return ch == MUL_SYM || ch == DIV_SYM;
}

bool Lexer::is_const_sym(std::string str) {
    return str == CONST_SYM;
}

bool Lexer::is_int_sym(std::string str) {
    return str == INT_SYM;
}

bool Lexer::is_char_sym(std::string str) {
    return str == CHAR_SYM;
}

bool Lexer::is_void_sym(std::string str) {
    return str == VOID_SYM;
}

bool Lexer::is_main_sym(std::string str) {
    return str == MAIN_SYM;
}

bool Lexer::is_if_sym(std::string str) {
    return str == IF_SYM;
}

bool Lexer::is_else_sym(std::string str) {
    return str == ELSE_SYM;
}

bool Lexer::is_do_sym(std::string str) {
    return str == DO_SYM;
}

bool Lexer::is_while_sym(std::string str) {
    return str == WHILE_SYM;
}

bool Lexer::is_for_sym(std::string str) {
    return str == FOR_SYM;
}

bool Lexer::is_scanf_sym(std::string str) {
    return str == SCANF_SYM;
}

bool Lexer::is_printf_sym(std::string str) {
    return str == PRINTF_SYM;
}

bool Lexer::is_return_sym(std::string str) {
    return str == RETURN_SYM;
}

bool Lexer::is_single_quote_sym(std::string str) {
    return str == SINGLE_QUOTE_SYM;
}

bool Lexer::is_double_quote_sym(std::string str) {
    return str == DOUBLE_QUOTE_SYM;
}

bool Lexer::is_comma_sym(std::string ch) {
    return ch == COMMA_SYM;
}

bool Lexer::is_semicolon_sym(std::string ch) {
    return ch == SEMICOLON_SYM;
}

bool Lexer::is_colon_sym(std::string ch) {
    return ch == COLON_SYM;
}

bool Lexer::is_assign_sym(std::string ch) {
    return ch == ASSIGN_SYM;
}

bool Lexer::is_eq_sym(std::string str) {
    return str == EQ_SYM;
}

bool Lexer::is_ne_sym(std::string str) {
    return str == NE_SYM;
}

bool Lexer::is_lt_sym(std::string str) {
    return str == LT_SYM;
}

bool Lexer::is_gt_sym(std::string str) {
    return str == GT_SYM;
}

bool Lexer::is_le_sym(std::string str) {
    return str == LE_SYM;
}

bool Lexer::is_ge_sym(std::string str) {
    return str == GE_SYM;
}

bool Lexer::is_lparathesis_sym(std::string ch) {
    return ch == LPARENTHESIS_SYM;
}

bool Lexer::is_rparathesis_sym(std::string ch) {
    return ch == RPARENTHESIS_SYM;
}

bool Lexer::is_lbracket_sym(std::string ch) {
    return ch == LBRACKET_SYM;
}

bool Lexer::is_rbracket_sym(std::string ch) {
    return ch == RBRACKET_SYM;
}

bool Lexer::is_lbrace_sym(std::string ch) {
    return ch == LBRACE_SYM;
}

bool Lexer::is_rbrace_sym(std::string ch) {
    return ch == RBRACE_SYM;
}


