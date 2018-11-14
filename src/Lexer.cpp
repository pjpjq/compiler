//
// Created by monotasker on 2018/11/11.
//

#include "Lexer.h"

Token::Token(TokenOutputType token_output_type, int token_val_int, const std::string &token_val_string)
        : token_output_type(token_output_type), token_val_int(token_val_int), token_val_string(token_val_string) {}

std::string Token::get_output_string() {
    return token_output_names[token_output_type] + " " + token_val_string;
}

TokenOutputType Token::get_output_type() {
    return token_output_type;
}

int Token::get_val_int() {
    return token_val_int;
}

std::string Token::get_val_string() {
    return token_val_string;
}


Token fetch_token() {
    buffer.clear();
    fetch_char_skipping_spaces(); /* 跳过空格取一个字符 */
    if (is_alpha_or_underscore(cur_ch)) { /* identifier or keyword */
        buffer.push_back(cur_ch);
        fetch_char();
        while (is_alpha_or_underscore(cur_ch) || isdigit(cur_ch)) { /* 一直读到不是字母或数字的字符 */
            buffer.push_back(cur_ch);
            fetch_char();
        }
        retract();
        if (is_keyword(buffer)) {
            return Token(KEYWORD, 0, buffer);
        } else { /* 不是 keyword 就是标识符 */
            return Token(IDENTIFIER, 0, buffer);
        }
    }
    
    if (isdigit(cur_ch)) { /* unsigned int or 0. */
        buffer.push_back(cur_ch);
        if (cur_ch == '0') {
            return Token(ZERO, 0, buffer);
        } else {
            fetch_char();
            while (isdigit(cur_ch)) {
                buffer.push_back(cur_ch);
                fetch_char();
            }
            retract();
            if (buffer.size() >= MAX_N_DIGITS_INT) {
                error_message("Integer too large: " + buffer + "with length " + std::to_string(buffer.size()));
            }
            return Token(UNSIGNED_INTEGER, std::atoi(buffer.c_str()), buffer);
        }
    }
    
    if (is_single_quote_sym(cur_ch)) { /* 是单引号 */
        fetch_char();
        if (is_between_single_quote_type(std::string{cur_ch})) { /* 是单引号内字符 */
            buffer.push_back(cur_ch);
            fetch_char();
            if (is_single_quote_sym(cur_ch)) { /* 配对 ' */
                return Token(CHAR, buffer[0], buffer);
            } else {
                error_message("Expected single quote, now get " + std::string{cur_ch});
                return Token(UNKNOWN_TYPE, cur_ch, std::string{cur_ch});
            }
        } else {
            error_message("Invalid symbol for single quote char: " + std::string{cur_ch});
            return Token(UNKNOWN_TYPE, cur_ch, std::string{cur_ch});
        }
    }
    
    if (is_double_quote_sym(cur_ch)) { /* 是双引号 */
        fetch_char();
        while (is_between_double_quote_type(std::string{cur_ch})) { /* 直到取不出符合双引号内字符为止 */
            buffer.push_back(cur_ch);
            fetch_char();
        }
        
        for (char ch : buffer) {
            assert(ch != '"');
        }
        
        if (is_double_quote_sym(cur_ch)) { /* 匹配双引号 */
            return Token(STRING, (int) buffer.size(), buffer);
        } else {
            error_message("Expected double quote, now get " + std::string{cur_ch});
            return Token(UNKNOWN_TYPE, cur_ch, std::string{cur_ch});
        }
    }
    
    if (cur_ch == '<' || cur_ch == '>' || cur_ch == '=') { /* <= >= == < > = */
        buffer.push_back(cur_ch);
        fetch_char();
        if (cur_ch == '=') {
            buffer.push_back(cur_ch);
        } else {
            retract();
        }
        return Token(SEPARATOR, buffer[0], buffer);
    }
    
    if (cur_ch == '!') { /* != */
        buffer.push_back(cur_ch);
        fetch_char();
        if (cur_ch == '=') {
            buffer.push_back(cur_ch);
            return Token(SEPARATOR, buffer[0], buffer);
        } else {
            error_message("Expected \"=\" after \"!\" now get " + std::string{cur_ch});
            return Token(UNKNOWN_TYPE, buffer[0], buffer);
        }
    }
    
    if (cur_ch == '+' || cur_ch == '-' || cur_ch == '*' || cur_ch == '/' ||
        cur_ch == ',' || cur_ch == ';' || cur_ch == ':' ||
        cur_ch == '(' || cur_ch == ')' || cur_ch == '[' || cur_ch == ']' || cur_ch == '{' || cur_ch == '}') {
        buffer.push_back(cur_ch);
        return Token(SEPARATOR, buffer[0], buffer);
    }
    
    if (cur_ch == EOF) {
        return Token(END_OF_FILE, -1, "EOF");
    }
    /* Shouldn't reach here! */
    assert(!is_separator(std::string{cur_ch}));
    error_message("Unknown token type: " + std::string{cur_ch});
    return Token(UNKNOWN_TYPE, buffer[0], buffer);
}

void fetch_char_skipping_spaces() {
    while (isspace(fetch_char())) { ; }
}

char fetch_char() {
    cur_ch = (char) source_file.get();
    assert (source_file.is_open());
    if (is_newline(cur_ch)) {
        ++line_count;
    }
//    std::cout << cur_ch - 0 << std::endl;
    return cur_ch;
}

void retract() {
    source_file.putback(cur_ch);
    if (is_newline(cur_ch)) {
        --line_count;
    }
    cur_ch = '\0';
}


void error_message(const std::string &error_info) {
    std::cout << "[ERROR] in line " << line_count << ": " << error_info << std::endl;
    ++n_errors;
}

bool is_newline(char ch) {
    return ch == '\r' || ch == '\n';
}

bool is_alpha_or_underscore(char ch) {
    return isalpha(ch) || ch == '_';
}

bool is_keyword(const std::string &str) {
    return KEYWORD_SET.count(str) > 0;
}

bool is_single_quote_sym(char ch) {
    return ch == '\'';
}

bool is_double_quote_sym(char ch) {
    return ch == '\"';
}

bool is_plus_or_minus(const std::string &str) {
    return is_plus(str) || is_minus(str);
}

bool is_plus(const std::string &str) {
    return str.size() == 1 && str[0] == '+';
}

bool is_minus(const std::string &str) {
    return str.size() == 1 && str[0] == '-';
}

bool is_mul_or_div(const std::string &str) {
    return is_mul(str) || is_div(str);
}

bool is_mul(const std::string &str) {
    return str.size() == 1 && str[0] == '*';
}

bool is_div(const std::string &str) {
    return str.size() == 1 && str[0] == '/';
}

bool is_between_single_quote_type(const std::string &str) {
    return is_plus_or_minus(str) || is_mul_or_div(str) ||
           (str.size() == 1 && (isdigit(str[0]) || is_alpha_or_underscore(str[0])));
}

bool is_between_double_quote_type(const std::string &str) {
    return str.size() == 1 && (str[0] >= 32 && str[0] <= 126 && str[0] != 34);
}

bool is_separator(const std::string &str) {
    return SEPARATOR_SET.count(str) > 0;
}


