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

/* ��������Ĵ����� */
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
     * �õ�������������� string ��ʽ: "���� ֵ"
     * ֵ: ���ֻ��ַ���
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
 **************************  �ʷ����� API ****************************
 ********************************************************************/

/**
 * ��ȫ�����ʴ浽 tokens ��
 */
void tokenize();

/**
 * ȡһ������, ���Կո�
 * ά�� buffer, cur_ch, line_count(�س��Ļ�), n_tokens(++), cur_token
 * @return ȡ���ĵ���
 */
Token fetch_token();

void retract_token();

/**
 * TODO
 * ����һ������.
 * ����ʵ��: ���浱ǰ״̬, ȡһ������Ȼ��Ż�, �ָ�֮ǰ״̬
 * @return ��һ������
 */
Token peek_token();

/**
 * �� fetch_char() ȡֱ�����ǿո���ַ�, ���� cur_ch
 */
void fetch_char_skipping_spaces();

/**
 * ȡ�ַ�, ���� cur_ch, line_count,
 * @return cur_ch
 */
char fetch_char();

/**
* �ع���ǰ char
*/
void retract_char();


/**
 * ��ӡ������Ϣ��λ��(�к�), TODO ���ƴ�����
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
