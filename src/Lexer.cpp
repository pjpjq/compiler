//
// Created by monotasker on 2018/11/11.
//

#include "Lexer.h"

void fetch_char_skipping_spaces() {
    int n_consecutive_spaces = 0;
    while (isspace(fetch_char()) && n_consecutive_spaces < MAX_LIMIT_N_CONSECUTIVE_SPACES) {
        ++n_consecutive_spaces;
    }
}

char fetch_char() {
    if (cur_ch_idx >= source_file_in_str.size()) {
        return EOF;
    }
//    assert(cur_ch_idx >= 0 && cur_ch_idx < source_file_in_str.size());
    cur_ch = source_file_in_str[cur_ch_idx];
    if (is_newline(cur_ch)) {
        ++line_count;
    }
//    std::cout << cur_ch - 0 << std::endl;
    ++cur_ch_idx;
    return cur_ch;
}

void retract_char() {
    if (cur_ch_idx <= 0) {
        error_message("Cannot retract_char: " + std::string{cur_ch}, line_count);
    }
    if (is_newline(cur_ch)) {
        --line_count;
    }
    --cur_ch_idx;
    cur_ch = source_file_in_str[cur_ch_idx];
}

bool is_newline(char ch) { return ch == '\r' || ch == '\n'; }

bool is_alpha_or_underscore(char ch) { return isalpha(ch) || ch == '_'; }

bool is_keyword(const std::string &str) { return KEYWORD_SET.count(str) > 0; }

bool is_single_quote_sym(char ch) { return ch == '\''; }

bool is_double_quote_sym(char ch) { return ch == '\"'; }

bool is_plus_or_minus(const std::string &str) { return is_plus(str) || is_minus(str); }

bool is_plus(const std::string &str) { return str.size() == 1 && str[0] == '+'; }

bool is_minus(const std::string &str) { return str.size() == 1 && str[0] == '-'; }

bool is_mul_or_div(const std::string &str) { return is_mul(str) || is_div(str); }

bool is_mul(const std::string &str) { return str.size() == 1 && str[0] == '*'; }

bool is_div(const std::string &str) { return str.size() == 1 && str[0] == '/'; }

bool is_between_single_quote_type(const std::string &str) {
    return is_plus_or_minus(str) || is_mul_or_div(str) ||
           (str.size() == 1 && (isdigit(str[0]) || is_alpha_or_underscore(str[0])));
}

bool is_between_double_quote_type(const std::string &str) {
    return str.size() == 1 && (str[0] >= 32 && str[0] <= 126 && str[0] != 34);
}

bool is_separator(const std::string &str) { return SEPARATOR_SET.count(str) > 0; }

Token fetch_token() {
    token_buffer.clear();
    fetch_char_skipping_spaces(); /* 跳过空格取一个字符 */
    if (is_alpha_or_underscore(cur_ch)) { /* identifier or keyword */
        token_buffer.push_back(cur_ch);
        fetch_char();
        while (is_alpha_or_underscore(cur_ch) || isdigit(cur_ch)) { /* 一直读到不是字母或数字的字符 */
            token_buffer.push_back(cur_ch);
            fetch_char();
        }
        retract_char();
        if (is_keyword(token_buffer)) {
            tokens.emplace_back(Token(KEYWORD, 0, token_buffer, line_count));
        } else { /* 不是 keyword 就是标识符 */
            tokens.emplace_back(Token(IDENTIFIER, 0, token_buffer, line_count));
        }
        return tokens.back();
    }
    
    if (isdigit(cur_ch)) { /* unsigned int or 0. */
        token_buffer.push_back(cur_ch);
        if (cur_ch == '0') {
            tokens.emplace_back(Token(ZERO, 0, token_buffer, line_count));
            return tokens.back();
        } else {
            fetch_char();
            while (isdigit(cur_ch)) {
                token_buffer.push_back(cur_ch);
                fetch_char();
            }
            retract_char();
            if (token_buffer.size() >= MAX_N_DIGITS_INT) {
                error_message("Integer too large: " + token_buffer + ", len: " + std::to_string(token_buffer.size()),
                              line_count);
            }
            tokens.emplace_back(Token(UNSIGNED_INTEGER, std::atoi(token_buffer.c_str()), token_buffer, line_count));
            return tokens.back();
        }
    }
    
    if (is_single_quote_sym(cur_ch)) { /* 是单引号 */
        fetch_char();
        if (is_between_single_quote_type(std::string{cur_ch})) { /* 是单引号内字符 */
            token_buffer.push_back(cur_ch);
            fetch_char();
            if (is_single_quote_sym(cur_ch)) { /* 配对 ' */
                tokens.emplace_back(Token(CHAR, token_buffer[0], token_buffer, line_count));
            } else {
                error_message("Expected single quote, instead got " + std::string{cur_ch}, line_count);
                tokens.emplace_back(Token(UNKNOWN_TYPE, cur_ch, std::string{cur_ch}, line_count));
            }
        } else {
            error_message("Invalid symbol for single quote char: " + std::string{cur_ch}, line_count);
            tokens.emplace_back(Token(UNKNOWN_TYPE, cur_ch, std::string{cur_ch}, line_count));
        }
        return tokens.back();
    }
    
    if (is_double_quote_sym(cur_ch)) { /* 是双引号 */
        fetch_char();
        while (is_between_double_quote_type(std::string{cur_ch})) { /* 直到取不出符合双引号内字符为止 */
            token_buffer.push_back(cur_ch);
            fetch_char();
        }
        for (char ch : token_buffer) { assert(ch != '"'); }
        if (is_double_quote_sym(cur_ch)) { /* 匹配双引号 */
            tokens.emplace_back(Token(STRING, (int) token_buffer.size(), token_buffer, line_count));
        } else {
            error_message("Expected double quote, instead got " + std::string{cur_ch}, line_count);
            tokens.emplace_back(Token(UNKNOWN_TYPE, cur_ch, std::string{cur_ch}, line_count));
        }
        return tokens.back();
    }
    
    if (cur_ch == '<' || cur_ch == '>' || cur_ch == '=') { /* <= >= == < > = */
        token_buffer.push_back(cur_ch);
        fetch_char();
        if (cur_ch == '=') {
            token_buffer.push_back(cur_ch);
        } else {
            retract_char();
        }
        tokens.emplace_back(Token(SEPARATOR, token_buffer[0], token_buffer, line_count));
        return tokens.back();
    }
    
    if (cur_ch == '!') { /* != */
        token_buffer.push_back(cur_ch);
        fetch_char();
        if (cur_ch == '=') {
            token_buffer.push_back(cur_ch);
            tokens.emplace_back(Token(SEPARATOR, token_buffer[0], token_buffer, line_count));
        } else {
            error_message("Expected = after !, instead got " + std::string{cur_ch}, line_count);
            tokens.emplace_back(Token(UNKNOWN_TYPE, token_buffer[0], token_buffer, line_count));
        }
        return tokens.back();
    }
    
    if (cur_ch == '+' || cur_ch == '-' || cur_ch == '*' || cur_ch == '/' ||
        cur_ch == ',' || cur_ch == ';' || cur_ch == ':' ||
        cur_ch == '(' || cur_ch == ')' || cur_ch == '[' || cur_ch == ']' || cur_ch == '{' || cur_ch == '}') {
        token_buffer.push_back(cur_ch);
        tokens.emplace_back(Token(SEPARATOR, token_buffer[0], token_buffer, line_count));
        return tokens.back();
    }
    
    assert (!is_separator(std::string{cur_ch}));
    if (cur_ch == EOF) {
        tokens.emplace_back(Token(END_OF_FILE, -1, "EOF", line_count));
        return tokens.back();
    }
    /* Shouldn't reach here! Report error. */
    error_message("Unknown token type: " + std::string{cur_ch}, line_count);
    tokens.emplace_back(Token(UNKNOWN_TYPE, token_buffer[0], token_buffer, line_count));
    return tokens.back();
}

void tokenize() {
    while (cur_ch_idx < source_file_in_str.size() - 1) {
//        std::cout <<  fetch_token().get_output_string() << std::endl;
        fetch_token();
    }
//    for (auto &token: tokens) { std::cout << token.get_output_string() << std::endl; }
}





