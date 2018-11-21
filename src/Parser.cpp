//
// Created by monotasker on 2018/11/18.
//

#include "Parser.h"


bool parse_unsigned_integer() {
    bool is_unsigned_integer = true;
    if (tokens[cur_token_idx].get_output_type() != UNSIGNED_INTEGER &&
        tokens[cur_token_idx].get_output_type() != ZERO) {
        error_message("Unsigned integer should be 0 or a unsigned int, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_unsigned_integer = false;
    }
    ++cur_token_idx;
    return is_unsigned_integer;
}

bool parse_integer() {
    bool is_correct_integer = true;
    if (tokens[cur_token_idx].get_output_type() == UNSIGNED_INTEGER ||
        tokens[cur_token_idx].get_output_type() == ZERO) { /* 无符号整数 */
        ++cur_token_idx;
        return true;
    }
    if (tokens[cur_token_idx].get_val_string() != PLUS_SYM && tokens[cur_token_idx].get_val_string() != MINUS_SYM) {
        error_message("Integer should begin with +/-/digit, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_integer = false;
    }
    ++cur_token_idx;
    if (!parse_unsigned_integer()) {
        error_message("Invalid unsigned integer!");
        is_correct_integer = false;
    }
    return is_correct_integer;
}

bool parse_const_definition() {
    bool is_correct_const_definition = true;
    if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
        error_message("Constant definition should begin with \"int\"/\"char\", instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_const_definition = false;
    }
    if (tokens[cur_token_idx].get_val_string() == INT_SYM) {
        do {
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
                error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_const_definition = false;
            }
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM) {
                error_message("Expected \"=\", instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_const_definition = false;
            }
            ++cur_token_idx;
            if (!parse_integer()) {
                error_message("Invalid Integer!");
                is_correct_const_definition = false;
            }
        } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM);
    } else if (tokens[cur_token_idx].get_val_string() == CHAR_SYM) {
        do {
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
                error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_const_definition = false;
            }
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM) {
                error_message("Expected \"=\", instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_const_definition = false;
            }
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_output_type() != CHAR) {
                error_message("Invalid char!");
                is_correct_const_definition = false;
            }
            ++cur_token_idx;
        } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM);
    }
    if (is_correct_const_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct constant definition!"
                  << std::endl;
    }
    return is_correct_const_definition;
}

bool parse_const_declarations() {
    bool is_correct_const_declarations = true;
    if (tokens[cur_token_idx].get_val_string() != CONST_SYM) {
        error_message("Constant declarations should begin with \"const\", instead got " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_const_declarations = false;
    }
    while (tokens[cur_token_idx].get_val_string() == CONST_SYM) {
        ++cur_token_idx;
        if (!parse_const_definition()) {
            error_message("Invalid constant definition!");
            is_correct_const_declarations = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message("Expected \";\", instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_const_declarations = false;
        }
        ++cur_token_idx;
    }
    if (is_correct_const_declarations) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct constant declarations!"
                  << std::endl;
    }
    return is_correct_const_declarations;
}

bool parse_variable_definition() {
    bool is_correct_variable_definition = true;
    if (tokens[cur_token_idx].get_val_string() != CHAR_SYM && tokens[cur_token_idx].get_val_string() != INT_SYM) {
        error_message("Variable definition should begin with char/int, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_variable_definition = false;
    }
    do {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_variable_definition = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* 数组 */
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_output_type() == ZERO) {
                error_message("Array size should be larger than 0!");
                is_correct_variable_definition = false;
            }
            if (!parse_unsigned_integer()) {
                error_message("Expected unsigned integer for array size, instead got: " +
                              tokens[cur_token_idx].get_val_string());
                is_correct_variable_definition = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM) {
                error_message("Expected \"]\", instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_variable_definition = false;
            }
            ++cur_token_idx;
        }
    } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM);
    
    if (is_correct_variable_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct variable definition!"
                  << std::endl;
    }
    return is_correct_variable_definition;
}

bool parse_variable_declarations() {
    bool is_correct_variable_declarations = true;
    while (true) {
        if (!parse_variable_definition()) {
            error_message("Invalid variable definition!");
            is_correct_variable_declarations = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message("Expected ; instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_variable_declarations = false;
        }
        
        ++cur_token_idx;
        if ((tokens[cur_token_idx].get_val_string() == CHAR_SYM || tokens[cur_token_idx].get_val_string() == INT_SYM) &&
            tokens[cur_token_idx + 1].get_output_type() == IDENTIFIER &&
            (tokens[cur_token_idx + 2].get_val_string() == SEMICOLON_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == LBRACKET_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM)) {
            continue;
        } else {
            break;
        }
    }
    if (is_correct_variable_declarations) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct variable declarations!"
                  << std::endl;
    }
    return is_correct_variable_declarations;
}

bool parse_parameter_list() {
    bool is_correct_parameter_list = true;
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM) { /* 空参数表 */
        return is_correct_parameter_list;
    }
    
    if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
        error_message("Expected int/char in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_parameter_list = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_parameter_list = false;
    }
    
    ++cur_token_idx;
    while (tokens[cur_token_idx].get_val_string() == COMMA_SYM) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
            error_message(
                    "Expected int/char in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_parameter_list = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message(
                    "Expected identifier in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_parameter_list = false;
        }
        ++cur_token_idx;
    }
    return is_correct_parameter_list;
}

bool parse_block() {
    bool is_correct_block = true;
    if (tokens[cur_token_idx].get_val_string() == CONST_SYM) {
        if (!parse_const_declarations()) {
            error_message("Invalid constant declarations!");
            is_correct_block = false;
        }
    }
    
    if (tokens[cur_token_idx].get_val_string() == INT_SYM || tokens[cur_token_idx].get_val_string() == CHAR_SYM) {
        if (!parse_variable_declarations()) {
            error_message("Invalid variable declarations!");
            is_correct_block = false;
        }
    }
    
    int n_statements = 0;
    const int MAX_N_STATEMENTS_LIMIT = 12345;
    while (tokens[cur_token_idx].get_val_string() != RBRACE_SYM && n_statements < MAX_N_STATEMENTS_LIMIT) {
        if (!parse_statements()) {
            error_message("Invalid statements!");
            is_correct_block = false;
            break;
        }
        ++n_statements;
    }
    if (is_correct_block) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct block!" << std::endl;
    }
    return is_correct_block;
}

bool parse_statements() {
    bool is_correct_statements = true;
    if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM) {
        ++cur_token_idx;
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct ; statement" << std::endl;
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == RETURN_SYM) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM) { /* return; */
            ++cur_token_idx;
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct return statement!"
                      << std::endl;
            return true;
        }
        /* return ( */
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
            error_message("( is expected in return statement, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (!parse_expression()) {
            error_message("Invalid expression!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message(") is expected in return statement, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "; is expected after return statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct return statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == SCANF_SYM) {
        if (!parse_scanf_statement()) {
            error_message("Invalid scanf statement!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "; is expected after scanf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct scanf statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == PRINTF_SYM) {
        if (!parse_printf_statement()) {
            error_message("Invalid printf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "; is expected after printf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct printf statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER &&
        tokens[cur_token_idx + 1].get_val_string() != LPARENTHESIS_SYM) { /* 赋值语句 */
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* 数组元素赋值 */
            ++cur_token_idx;
            if (!parse_expression()) {
                error_message("Invalid expression!");
                is_correct_statements = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM) {
                error_message("] is expected in assignment statement! Instead got: " +
                              tokens[cur_token_idx].get_val_string());
                is_correct_statements = false;
            }
            ++cur_token_idx;
        }
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM) {
            error_message(
                    "= is expected in assignment statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (!parse_expression()) {
            error_message("Invalid expression!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "; is expected after assignment statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct assignment statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER) { /* 函数调用 */
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
            error_message("Expected ( in function call, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (!parse_value_parameters()) {
            error_message("Invalid value parameters!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message("Expected ) in function call, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "; is expected after function call statement! Instead got: " +
                    tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct function call statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == IF_SYM) {
        if (!parse_conditional_statement()) {
            error_message("Invalid conditional statement!");
            is_correct_statements = false;
        }
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct if statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == DO_SYM || tokens[cur_token_idx].get_val_string() == FOR_SYM) {
        if (!parse_loop_statement()) {
            error_message("Invalid loop statement!");
            is_correct_statements = false;
        }
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct loop statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == LBRACE_SYM) {
        ++cur_token_idx;
        int n_inner_statements = 0;
        const int MAX_N_INNER_STATEMENTS = 123456;
        while (tokens[cur_token_idx].get_val_string() != RBRACE_SYM && n_inner_statements < MAX_N_INNER_STATEMENTS) {
            if (!parse_statements()) {
                error_message("Invalid inner statements!");
                is_correct_statements = false;
            }
            ++n_inner_statements;
        }
        if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM) {
            error_message("Invalid inner statements!");
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct complex statements!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    return is_correct_statements;
}

bool parse_loop_statement() {
    bool is_correct_loop_statement = true;
    if (tokens[cur_token_idx].get_val_string() != DO_SYM && tokens[cur_token_idx].get_val_string() != FOR_SYM) {
        error_message("Expected do or for in loop statement!");
        is_correct_loop_statement = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() == DO_SYM) {
        ++cur_token_idx;
        if (!parse_statements()) {
            error_message("Invalid statements in do while loop!");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != WHILE_SYM) {
            error_message("Expected while in do while loop, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
            error_message("Expected ( after while, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_condition()) {
            error_message("Invalid condition in while");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message("Expected ) after while, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        return is_correct_loop_statement;
    }
    
    if (tokens[cur_token_idx].get_val_string() == FOR_SYM) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
            error_message("Expected ( after for, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for loop init part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM) {
            error_message("Expected = in for loop init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_expression()) {
            error_message("Invalid expression in for loop init part");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "Expected ; in for loop after init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_condition()) {
            error_message("Invalid condition in for loop!");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message(
                    "Expected ; in for loop after init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for loop updating part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM) {
            error_message(
                    "Expected = in for loop updating part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for loop updating part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != MINUS_SYM && tokens[cur_token_idx].get_val_string() != PLUS_SYM) {
            error_message(
                    "Expected +/- in for loop updating part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_unsigned_integer()) {
            error_message("Expected unsigned int in for loop, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message("Expected ) after for, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_statements()) {
            error_message("Invalid statements in for loop!");
            is_correct_loop_statement = false;
        }
        return is_correct_loop_statement;
    }
    return is_correct_loop_statement;
}

bool parse_conditional_statement() {
    bool is_correct_conditional_statement = true;
    if (tokens[cur_token_idx].get_val_string() != IF_SYM) {
        error_message("Expected if in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
        error_message("Expected ( in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    if (!parse_condition()) {
        error_message("Invalid condition!");
        is_correct_conditional_statement = false;
    }
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
        error_message("Expected ) in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    if (!parse_statements()) {
        error_message("Invalid statement in if!");
        is_correct_conditional_statement = false;
    }
    while (tokens[cur_token_idx].get_val_string() == ELSE_SYM) {
        if (!parse_statements()) {
            error_message("Invalid statement in else!");
            is_correct_conditional_statement = false;
        }
    }
    return is_correct_conditional_statement;
}

bool parse_condition() {
    bool is_correct_condition = true;
    if (!parse_expression()) {
        error_message("Invalid expression in condition");
        is_correct_condition = false;
    }
    if (tokens[cur_token_idx].get_val_string() == EQ_SYM || tokens[cur_token_idx].get_val_string() == NE_SYM ||
        tokens[cur_token_idx].get_val_string() == LT_SYM || tokens[cur_token_idx].get_val_string() == GT_SYM ||
        tokens[cur_token_idx].get_val_string() == LE_SYM || tokens[cur_token_idx].get_val_string() == GE_SYM) {
        ++cur_token_idx;
        if (!parse_expression()) {
            error_message("Invalid expression after comparison operators in condition");
            is_correct_condition = false;
        }
    }
    return is_correct_condition;
}

bool parse_printf_statement() {
    bool is_correct_printf_statement = true;
    if (tokens[cur_token_idx].get_val_string() != PRINTF_SYM) {
        error_message("Expected printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
        error_message("Expected ( after printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() == STRING) { /* 字符串 */
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == COMMA_SYM) { /* 字符串后面接一个表达式 */
            ++cur_token_idx;
            if (!parse_expression()) {
                error_message("Expected expression!");
                is_correct_printf_statement = false;
            }
        }
    } else if (!parse_expression()) { /* 只有表达式 */
        error_message("Expected expression!");
        is_correct_printf_statement = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
        error_message("Expected ) after printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    return is_correct_printf_statement;
}

bool parse_scanf_statement() {
    bool is_correct_scanf_statement = true;
    if (tokens[cur_token_idx].get_val_string() != SCANF_SYM) {
        error_message("Expected scanf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
        error_message("Expected (, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    while (tokens[cur_token_idx].get_val_string() == COMMA_SYM) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_scanf_statement = false;
        }
        ++cur_token_idx;
    }
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
        error_message("Expected ) after scanf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    return is_correct_scanf_statement;
}

bool parse_expression() {
    bool is_correct_expression = true;
    if (tokens[cur_token_idx].get_val_string() == MINUS_SYM || tokens[cur_token_idx].get_val_string() == PLUS_SYM) {
        ++cur_token_idx;
    }
    
    if (!parse_term()) {
        error_message("Invalid term!");
        is_correct_expression = false;
    }
    
    while (tokens[cur_token_idx].get_val_string() == MINUS_SYM || tokens[cur_token_idx].get_val_string() == PLUS_SYM) {
        ++cur_token_idx;
        if (!parse_term()) {
            error_message("Invalid term!");
            is_correct_expression = false;
        }
    }
    return is_correct_expression;
}

bool parse_term() {
    bool is_correct_term = true;
    if (!parse_factor()) {
        error_message("Invalid first factor in term!");
        is_correct_term = false;
    }
    while (tokens[cur_token_idx].get_val_string() == MUL_SYM || tokens[cur_token_idx].get_val_string() == DIV_SYM) {
        ++cur_token_idx;
        if (!parse_factor()) {
            error_message("Invalid optional factors in term!");
            is_correct_term = false;
        }
    }
    return is_correct_term;
}

bool parse_factor() {
    bool is_correct_factor = true;
    if (tokens[cur_token_idx].get_output_type() == CHAR) { /* 字符 */
        ++cur_token_idx;
        return true;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* 数组 */
            ++cur_token_idx;
            if (!parse_expression()) {
                error_message("Invalid expression!");
                is_correct_factor = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM) {
                error_message("Expected ], instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_factor = false;
            }
            ++cur_token_idx;
            return is_correct_factor;
        }
        
        if (tokens[cur_token_idx].get_val_string() == LPARENTHESIS_SYM) { /* 函数调用 */
            ++cur_token_idx;
            if (!parse_value_parameters()) {
                error_message("Invalid value parameters!");
                is_correct_factor = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
                error_message("Expected ) in function call in factor, instead got: " +
                              tokens[cur_token_idx].get_val_string());
                is_correct_factor = false;
            }
            ++cur_token_idx;
            return is_correct_factor;
        }
        /* 就是单个标识符 */
        return is_correct_factor;
    }
    
    if (tokens[cur_token_idx].get_val_string() == LPARENTHESIS_SYM) { /* 表达式 */
        ++cur_token_idx;
        if (!parse_expression()) {
            error_message("Invalid expression!");
            is_correct_factor = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message("Expected ) in factor expression, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_factor = false;
        }
        ++cur_token_idx;
        return is_correct_factor;
    }
    
    if (!parse_integer()) {
        error_message("Invalid Integer!");
        is_correct_factor = false;
    }
    return is_correct_factor;
}

bool parse_value_parameters() {
    bool is_value_parameters = true;
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM) {
        return is_value_parameters;
    }
    if (!parse_expression()) {
        error_message("Invalid expression!");
        is_value_parameters = false;
    }
    while (tokens[cur_token_idx].get_val_string() == COMMA_SYM) {
        ++cur_token_idx;
        if (!parse_expression()) {
            error_message("Invalid expression!");
            is_value_parameters = false;
        }
    }
    return is_value_parameters;
}

bool parse_main_function_definition() {
    bool is_correct_main_function_definition = true;
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message("main function should start with void, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != MAIN_SYM) {
        error_message("Expected main, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
        error_message("Expected ( after main, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
        error_message("Expected ) after main(, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM) {
        error_message("Expected {, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    
    ++cur_token_idx;
    if (!parse_block()) {
        error_message("Invalid block!");
        is_correct_main_function_definition = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM) {
        error_message("Expected }, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    
    assert(cur_token_idx >= 0 && cur_token_idx < tokens.size());
//    ++cur_token_idx;
    if (is_correct_main_function_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct main function definition!"
                  << std::endl;
    }
    return is_correct_main_function_definition;
    
}

bool parse_non_void_function_definition() {
    bool is_correct_non_void_function_definiton = true;
    if (tokens[cur_token_idx].get_val_string() != CHAR_SYM && tokens[cur_token_idx].get_val_string() != INT_SYM) {
        error_message("Non void functions should start with int/char, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier in non void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
        error_message(
                "Expected ( in non void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (!parse_parameter_list()) {
        error_message("Invalid parameter list in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
        error_message("Expected ) after parameter list in non void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM) {
        error_message(
                "Expected { in non void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (!parse_block()) {
        error_message("Invalid block in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM) {
        error_message(
                "Expected } in non void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (is_correct_non_void_function_definiton) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct non void function definition!"
                  << std::endl;
    }
    return is_correct_non_void_function_definiton;
}

bool parse_void_function_definition() {
    bool is_correct_void_function_definition = true;
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message("Void functions should start with void, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier in void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM) {
        error_message("Expected ( in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (!parse_parameter_list()) {
        error_message("Invalid parameter list in void function definition!");
        is_correct_void_function_definition = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
        error_message("Expected ) in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM) {
        error_message("Expected { in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (!parse_block()) {
        error_message("Invalid block in void function definition!");
        is_correct_void_function_definition = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM) {
        error_message("Expected } in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (is_correct_void_function_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct void function definition!"
                  << std::endl;
    }
    return is_correct_void_function_definition;
}

bool parse_program() {
    bool is_correct_program = true;
    tokenize();
    
    std::cout << "----------------------Optional Constant declarations----------------------" << std::endl;
    if (tokens[cur_token_idx].get_val_string() == CONST_SYM) { /* 是常量声明 */
        if (!parse_const_declarations()) {
            error_message("Invalid constant declarations!");
            is_correct_program = false;
        }
    }
    
    std::cout << "----------------------Optional Variable declarations----------------------" << std::endl;
    if (tokens[cur_token_idx].get_val_string() == INT_SYM || tokens[cur_token_idx].get_val_string() == CHAR_SYM) {
        if (tokens[cur_token_idx + 1].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx + 1].get_val_string());
            is_correct_program = false;
        }
        /* 是变量 或 有返回值函数声明 */
        // 应是 "(" 或 "[" 或 ";" 或 ","
        if (tokens[cur_token_idx + 2].get_val_string() == LBRACKET_SYM ||
            tokens[cur_token_idx + 2].get_val_string() == SEMICOLON_SYM ||
            tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM) { /* 是变量声明 */
            if (!parse_variable_declarations()) { /* 分析变量声明 */
                error_message("Invalid variable declarations!");
                is_correct_program = false;
            }
            std::cout << "-----------------------------------Functions------------------------------" << std::endl;
        } else if (tokens[cur_token_idx + 2].get_val_string() == LPARENTHESIS_SYM) { /* 是有返回值的函数 */
            std::cout << "-----------------------------------Functions------------------------------" << std::endl;
            if (!parse_non_void_function_definition()) { /* 分析当前该有返回值的函数 */
                error_message("Invalid non-void function!");
                is_correct_program = false;
            }
        } else {
            error_message("Expected \"(\" or \"[\" or \";\" or \",\", instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_program = false;
        }
    }
    
    /* 分析之后的至少有 void main() 的函数 */
    bool reached_main = false;
    int n_functions = 1;
    const int MAX_N_FUNCTIONS_LIMIT = 123;
    while (tokens[cur_token_idx].get_val_string() == INT_SYM || tokens[cur_token_idx].get_val_string() == CHAR_SYM ||
           tokens[cur_token_idx].get_val_string() == VOID_SYM) {
        if (tokens[cur_token_idx + 1].get_val_string() == MAIN_SYM) {
            reached_main = true;
            parse_main_function_definition();
        } else if (tokens[cur_token_idx + 1].get_output_type() == IDENTIFIER &&
                   tokens[cur_token_idx + 2].get_val_string() == LPARENTHESIS_SYM) {
            if (reached_main) {
                error_message("Last function should be main()!");
                is_correct_program = false;
            }
            
            if (tokens[cur_token_idx].get_val_string() == VOID_SYM) {
                parse_void_function_definition();
            } else {
                parse_non_void_function_definition();
            }
        } else {
            error_message("Invalid function prototype!");
            is_correct_program = false;
            break;
        }
        ++n_functions;
        if (n_functions > MAX_N_FUNCTIONS_LIMIT || cur_token_idx >= tokens.size() - 1) {
            break;
        }
    }
    
    if (is_correct_program) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_nbr() << ": finish correct program!" << std::endl;
    }
    return is_correct_program;
}





