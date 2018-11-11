//
// Created by monotasker on 2018/11/11.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <string>
#include <cctype>

/* keywords, 不管单/多字符的关键字都用 string 统一化便于比较 */
const std::string PLUS_SYM = "+";
const std::string MINUS_SYM = "-";
const std::string MUL_SYM = "*";
const std::string DIV_SYM = "/";

const std::string CONST_SYM = "const";
const std::string INT_SYM = "int";
const std::string CHAR_SYM = "char";
const std::string VOID_SYM = "void";
const std::string MAIN_SYM = "main";

const std::string IF_SYM = "if";
const std::string ELSE_SYM = "else";
const std::string DO_SYM = "do";
const std::string WHILE_SYM = "while";
const std::string FOR_SYM = "for";
const std::string SCANF_SYM = "scanf";
const std::string PRINTF_SYM = "printf";
const std::string RETURN_SYM = "return";

const std::string SINGLE_QUOTE_SYM = "\'";
const std::string DOUBLE_QUOTE_SYM = "\"";
const std::string COMMA_SYM = ",";
const std::string SEMICOLON_SYM = ";";
const std::string COLON_SYM = ":";
const std::string ASSIGN_SYM = "=";

const std::string EQ_SYM = "==";
const std::string NE_SYM = "!=";
const std::string LT_SYM = "<";
const std::string GT_SYM = ">";
const std::string LE_SYM = "<=";
const std::string GE_SYM = ">=";

const std::string LPARENTHESIS_SYM = "(";
const std::string RPARENTHESIS_SYM = ")";
const std::string LBRACKET_SYM = "[";
const std::string RBRACKET_SYM = "]";
const std::string LBRACE_SYM = "{";
const std::string RBRACE_SYM = "}";


class Lexer {
public:
    Lexer() = default;
    
    bool is_alpha_or_underscore(char ch);
    
    bool is_plus_or_minus_sym(std::string ch);
    
    bool is_mul_or_div_sym(std::string ch);
    
    bool is_const_sym(std::string str);
    
    bool is_int_sym(std::string str);
    
    bool is_char_sym(std::string str);
    
    bool is_void_sym(std::string str);
    
    bool is_main_sym(std::string str);
    
    bool is_if_sym(std::string str);
    
    bool is_else_sym(std::string str);
    
    bool is_do_sym(std::string str);
    
    bool is_while_sym(std::string str);
    
    bool is_for_sym(std::string str);
    
    bool is_scanf_sym(std::string str);
    
    bool is_printf_sym(std::string str);
    
    bool is_return_sym(std::string str);
    
    bool is_single_quote_sym(std::string str);
    
    bool is_double_quote_sym(std::string str);
    
    bool is_comma_sym(std::string ch);
    
    bool is_semicolon_sym(std::string ch);
    
    bool is_colon_sym(std::string ch);
    
    bool is_assign_sym(std::string ch);
    
    bool is_eq_sym(std::string str);
    
    bool is_ne_sym(std::string str);
    
    bool is_lt_sym(std::string str);
    
    bool is_gt_sym(std::string str);
    
    bool is_le_sym(std::string str);
    
    bool is_ge_sym(std::string str);
    
    bool is_lparathesis_sym(std::string ch);
    
    bool is_rparathesis_sym(std::string ch);
    
    bool is_lbracket_sym(std::string ch);
    
    bool is_rbracket_sym(std::string ch);
    
    bool is_lbrace_sym(std::string ch);
    
    bool is_rbrace_sym(std::string ch);

private:
};


#endif //COMPILER_LEXER_H
