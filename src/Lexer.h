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
#include <fstream>


const int MAX_N_DIGITS_INT = 10;

/* keywords 共同为一类 */
static std::string CONST_SYM = "const";
static std::string INT_SYM = "int";
static std::string CHAR_SYM = "char";
static std::string VOID_SYM = "void";
static std::string MAIN_SYM = "main";
static std::string IF_SYM = "if";
static std::string ELSE_SYM = "else";
static std::string DO_SYM = "do";
static std::string WHILE_SYM = "while";
static std::string FOR_SYM = "for";
static std::string SCANF_SYM = "scanf";
static std::string PRINTF_SYM = "printf";
static std::string RETURN_SYM = "return";

static std::unordered_set<std::string> KEYWORD_SET{CONST_SYM, INT_SYM, CHAR_SYM, VOID_SYM, MAIN_SYM, IF_SYM,
                                                   ELSE_SYM, DO_SYM, WHILE_SYM, FOR_SYM, SCANF_SYM, PRINTF_SYM,
                                                   RETURN_SYM};

/* separators 分界符, 共同为一类
 * 这里实际采用两种分类型方式, 大类型(SEPARATOR) 用于输出, 小类型用于具体判断 */
static std::string PLUS_SYM = "+";
static std::string MINUS_SYM = "-";
static std::string MUL_SYM = "*";
static std::string DIV_SYM = "/";

static std::string COMMA_SYM = ",";
static std::string SEMICOLON_SYM = ";";
static std::string COLON_SYM = ":";
static std::string ASSIGN_SYM = "=";

static std::string EQ_SYM = "==";
static std::string NE_SYM = "!=";
static std::string LT_SYM = "<";
static std::string GT_SYM = ">";
static std::string LE_SYM = "<=";
static std::string GE_SYM = ">=";

static std::string LPARENTHESIS_SYM = "(";
static std::string RPARENTHESIS_SYM = ")";
static std::string LBRACKET_SYM = "[";
static std::string RBRACKET_SYM = "]";
static std::string LBRACE_SYM = "{";
static std::string RBRACE_SYM = "}";

static std::unordered_set<std::string> SEPARATOR_SET{PLUS_SYM, MINUS_SYM, MUL_SYM, DIV_SYM,
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
    END_OF_FILE,
};

static std::string token_output_names[] = {"UNKNOWN_TYPE", "CHAR", "STRING",
                                          "UNSIGNED_INTEGER", "ZERO",
                                          "IDENTIFIER", "KEYWORD", "SEPARATOR", "END_OF_FILE"};

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
