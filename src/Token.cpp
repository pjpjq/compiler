//
// Created by monotasker on 2018/11/29.
//

#include "Token.h"

Token::Token(TokenOutputType token_output_type, int token_val_int, const std::string &token_val_string, int line_nbr)
        : token_output_type(token_output_type), token_val_int(token_val_int), token_val_string(token_val_string),
          line_nbr(line_nbr) {}

std::string Token::get_output_string() {
    return token_output_type_strs[token_output_type] + " " + token_val_string + " Line " + std::to_string(line_nbr);
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

int Token::get_line_nbr() {
    return line_nbr;
}