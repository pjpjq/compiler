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
#include <cassert>
#include <vector>

#include "constants.h"

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
    
    Token(TokenOutputType token_output_type, int token_val_int, const std::string &token_val_string, int line_nbr);
    
    /**
     * 得到单词用于输出的 string 形式: "类型 值"
     * 值: 数字或字符串
     * @return string of "token_output_type token_val_int/token_val_string"
     */
    std::string get_output_string();
    
    TokenOutputType get_output_type();
    
    int get_val_int();
    
    int get_line_nbr();
    
    std::string get_val_string();

private:
    TokenOutputType token_output_type = UNKNOWN_TYPE;
    int token_val_int;
    std::string token_val_string;
    int line_nbr;
};

extern std::string source_file_str;
extern int cur_ch_idx;
extern char cur_ch;
extern std::string token_buffer;
extern int line_count;
extern int n_errors;
extern std::vector<Token> tokens;
extern int cur_token_idx;


/********************************************************************
 **************************  词法分析 API ****************************
 ********************************************************************/

/**
 * 把全部单词存到 tokens 里
 */
void tokenize();

/**
 * 取一个单词, 忽略空格
 * 维护 buffer, cur_ch, line_count(回车的话), n_tokens(++), cur_token
 * @return 取到的单词
 */
Token fetch_token();

void retract_token();

/**
 * TODO
 * 看下一个单词.
 * 具体实现: 保存当前状态, 取一个单词然后放回, 恢复之前状态
 * @return 下一个单词
 */
Token peek_token();

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
* 回滚当前 char
*/
void retract_char();


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
