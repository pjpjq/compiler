//
// Created by monotasker on 2018/11/11.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <string>
#include <cctype>
#include <typeinfo>
#include <unordered_set>
#include <iostream>

#include "nameof.h"


const int MAX_N_DIGITS_INT = 10;

/* keywords 共同为一类 */
std::string CONST_SYM = "const";
std::string INT_SYM = "int";
std::string CHAR_SYM = "char";
std::string VOID_SYM = "void";
std::string MAIN_SYM = "main";
std::string IF_SYM = "if";
std::string ELSE_SYM = "else";
std::string DO_SYM = "do";
std::string WHILE_SYM = "while";
std::string FOR_SYM = "for";
std::string SCANF_SYM = "scanf";
std::string PRINTF_SYM = "printf";
std::string RETURN_SYM = "return";

std::unordered_set<std::string> KEYWORD_SET{CONST_SYM, INT_SYM, CHAR_SYM, VOID_SYM, MAIN_SYM, IF_SYM,
                                            ELSE_SYM, DO_SYM, WHILE_SYM, FOR_SYM, SCANF_SYM, PRINTF_SYM,
                                            RETURN_SYM};

/* separators 分界符, 共同为一类
 * 这里实际采用两种分类型方式, 大类型(SEPARATOR) 用于输出, 小类型用于具体判断 */
std::string PLUS_SYM = "+";
std::string MINUS_SYM = "-";
std::string MUL_SYM = "*";
std::string DIV_SYM = "/";

std::string COMMA_SYM = ",";
std::string SEMICOLON_SYM = ";";
std::string COLON_SYM = ":";
std::string ASSIGN_SYM = "=";

std::string EQ_SYM = "==";
std::string NE_SYM = "!=";
std::string LT_SYM = "<";
std::string GT_SYM = ">";
std::string LE_SYM = "<=";
std::string GE_SYM = ">=";

std::string LPARENTHESIS_SYM = "(";
std::string RPARENTHESIS_SYM = ")";
std::string LBRACKET_SYM = "[";
std::string RBRACKET_SYM = "]";
std::string LBRACE_SYM = "{";
std::string RBRACE_SYM = "}";

std::unordered_set<std::string> SEPARATOR_SET{PLUS_SYM, MINUS_SYM, MUL_SYM, DIV_SYM,
                                              COMMA_SYM, SEMICOLON_SYM, COLON_SYM, ASSIGN_SYM,
                                              EQ_SYM, NE_SYM, LT_SYM, GT_SYM, LE_SYM, GE_SYM,
                                              LPARENTHESIS_SYM, RPARENTHESIS_SYM, LBRACKET_SYM,
                                              RBRACKET_SYM, LBRACE_SYM, RBRACE_SYM};

/* 用于输出的大类型 */
enum TokenOutputType {
    UNKNOWN_TYPE,
    CHAR, STRING,
    UNSIGNED_INTEGER, ZERO,
    IDENTIFIER,
    KEYWORD,
    SEPARATOR,
};

class Token {
public:
    Token() = default;
    
    Token(TokenOutputType token_output_type, int token_val_int, const std::string &token_val_string);
    
    /**
     * 得到单词用于输出的 string 形式: "类型 值"
     * 值: 数字或字符串
     * @return string of "token_output_type token_val_int/token_val_string"
     */
    std::string get_output_string();
    
    TokenOutputType get_output_type();
    
    int get_val_int();
    
    std::string get_val_string();

private:
    TokenOutputType token_output_type = UNKNOWN_TYPE;
    int token_val_int;
    std::string token_val_string;
};

extern std::fstream source_file;
extern char cur_ch;
extern std::string buffer;
extern Token cur_token;
extern int line_count;
extern int n_errors;

/******************************
 ******  词法分析 API *********
 *****************************/

/**
 * 取一个单词
 * @return 取到的单词
 */
Token fetch_token();

/**
 * 用 fetch_char() 取直到不是空格的字符, 更新 cur_ch
 */
void fetch_char_skipping_spaces();

/**
 * 取字符, 更新 cur_ch, line_count,
 * @return cur_ch
 */
char fetch_char();

/**
* 回滚一个 char
*/
void retract();

/**
 * 打印错误信息及位置(行号), TODO 完善错误处理
 * @param error_info
 */
void error_message(const std::string &error_info);

bool is_newline(char ch);

bool is_alpha_or_underscore(char ch);

bool is_keyword(const std::string &str);

bool is_single_quote_sym(char ch);

bool is_double_quote_sym(char ch);

bool is_plus_or_minus(const std::string &str);

bool is_plus(const std::string &str);

bool is_minus(const std::string &str);

bool is_mul_or_div(const std::string &str);

bool is_mul(const std::string &str);

bool is_div(const std::string &str);

bool is_between_single_quote_type(const std::string &str);

bool is_between_double_quote_type(const std::string &str);

bool is_separator(const std::string &str);


#endif //COMPILER_LEXER_H
