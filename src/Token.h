//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <string>
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

static std::string token_output_type_strs[] = {"UNKNOWN_TYPE", "CHAR", "STRING",
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
    
    int get_line_num();
    
    std::string get_val_string();

private:
    TokenOutputType token_output_type = UNKNOWN_TYPE;
    int token_val_int = 0;
    std::string token_val_string = "";
    int line_nbr = -12345;
};


extern std::string source_file_in_str;
extern int cur_ch_idx;
extern char cur_ch;
extern std::string token_buffer;
extern int line_count;
extern int n_errors;
extern std::vector<Token> tokens;
extern int cur_token_idx;

#endif //COMPILER_TOKEN_H
