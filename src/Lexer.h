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
#include <vector>
#include <cassert>

#include "constants.h"
#include "utils.h"
#include "Token.h"

extern std::string source_file_in_str;
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
 * 维护 buffer, cur_ch, line_count(回车的话), n_tokens(++)
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
* 回滚当前 char
*/
void retract_char();

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
