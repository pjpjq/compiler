//
// Created by monotasker on 2018/11/18.
//

#include "Parser.h"

bool parse_unsigned_integer(int &res) {
    bool is_unsigned_integer = true;
    if (tokens[cur_token_idx].get_output_type() != UNSIGNED_INTEGER &&
        tokens[cur_token_idx].get_output_type() != ZERO) {
        error_message(
                "Unsigned integer should be 0/unsigned int, instead got: " + tokens[cur_token_idx].get_val_string());
        is_unsigned_integer = false;
    }
    res = tokens[cur_token_idx].get_val_int();
    if (res < 0) {
        error_message("Something is wrong here: got a negative value for an unsigned integer???????? : " +
                      std::to_string(res));
        is_unsigned_integer = false;
    }
//    std::cout << "[DEBUG] Line " << tokens[cur_token_idx].get_line_num() << ": got unsigned int " << res << std::endl;
    ++cur_token_idx;
    return is_unsigned_integer;
}

bool parse_integer(int &res) {
    bool is_correct_integer = true;
    if (tokens[cur_token_idx].get_output_type() == UNSIGNED_INTEGER ||
        tokens[cur_token_idx].get_output_type() == ZERO) { /* 无符号整数 */
        return parse_unsigned_integer(res);
    }
    /* 有显式符号前缀 +/- */
    if (tokens[cur_token_idx].get_val_string() != PLUS_SYM && tokens[cur_token_idx].get_val_string() != MINUS_SYM) {
        error_message("Explicitly signed Integer should begin with +/-/digit, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_integer = false;
    }
    if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Got +/- in integer sign but wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_integer = false;
    }
    int sign = tokens[cur_token_idx].get_val_string() == PLUS_SYM ? 1 : -1;
    ++cur_token_idx;
    if (!parse_unsigned_integer(res)) {
        error_message("Invalid unsigned integer part in the integer representation!");
        is_correct_integer = false;
    }
    res *= sign;
//    std::cout << "[DEBUG] Line " << tokens[cur_token_idx].get_line_num() << ": got integer " << res << std::endl;
    return is_correct_integer;
}

bool parse_const_definition(const std::string &function_name) {
    if (!function_name.empty() && !is_function(function_name)) {
        error_message("Undefined function name passed in const definition: " + function_name);
        return false;
    }
    bool is_correct_const_definition = true;
    std::string const_name = "???????!!!";
    SymbolType const_type = UNKNOWN_SYMBOL_TYPE;
    int value = -2333;
    
    if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
        error_message(
                "Const definition should begin with int/char, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_const_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got int/char in const definition but wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_const_definition = false;
    }
    if (tokens[cur_token_idx].get_val_string() == INT_SYM) {
        const_type = INT_SYMBOL_TYPE;
    } else if (tokens[cur_token_idx].get_val_string() == CHAR_SYM) {
        const_type = CHAR_SYMBOL_TYPE;
    }
    
    int n_const_definitions = 0;
    do {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_const_definition = false;
        }
        const_name = tokens[cur_token_idx].get_val_string();
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected =, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_const_definition = false;
        }
        ++cur_token_idx;
        if (const_type == INT_SYMBOL_TYPE) {
            if (!parse_integer(value)) {
                error_message("Invalid Integer in const definition!");
                is_correct_const_definition = false;
            }
            const_type = INT_SYMBOL_TYPE;
        } else if (const_type == CHAR_SYMBOL_TYPE) {
            value = tokens[cur_token_idx].get_val_int();
            const_type = CHAR_SYMBOL_TYPE;
            ++cur_token_idx;
        }
        /* int/char const 入符号表 */
        if (function_name.empty()) {
            if (!insert_global_const(const_name, const_type, value)) {
                is_correct_const_definition = false;
            }
        } else {
            if (!insert_local_const(function_name, const_name, const_type, value)) {
                is_correct_const_definition = false;
            }
        }
        ++n_const_definitions;
    } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
             tokens[cur_token_idx].get_output_type() == SEPARATOR && n_const_definitions < MAX_N_VAR_DEFINITIONS);
    
    if (is_correct_const_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct const definition!"
                  << std::endl;
    }
    return is_correct_const_definition;
}

bool parse_const_declarations(const std::string &function_name) {
    bool is_correct_const_declarations = true;
    if (tokens[cur_token_idx].get_val_string() != CONST_SYM) {
        error_message("Const declarations should begin with \"const\", instead got " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_const_declarations = false;
    }
    int n_variable_definitions = 0;
    while (tokens[cur_token_idx].get_val_string() == CONST_SYM && n_variable_definitions < MAX_N_VAR_DEFINITIONS) {
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message(
                    "Got const but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_const_declarations = false;
        }
        ++cur_token_idx;
        if (!parse_const_definition(function_name)) {
            error_message("Invalid const definition!");
            is_correct_const_declarations = false;
        }
        ++n_variable_definitions;
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message("Expected ; after const definition, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_const_declarations = false;
        }
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got ; but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_const_declarations = false;
        }
        ++cur_token_idx;
    }
    if (is_correct_const_declarations) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct const declarations! ("
                  << (function_name.empty() ? "global" : function_name) << ")" << std::endl;
    }
    return is_correct_const_declarations;
}

bool parse_variable_definition(const std::string &function_name) {
    if (!function_name.empty() && !is_function(function_name)) {
        error_message("Undefined function name passed in variable definition: " + function_name);
        return false;
    }
    bool is_correct_variable_definition = true;
    std::string var_name = "***<<<***?????";
    SymbolType var_type = UNKNOWN_SYMBOL_TYPE;
    int length = -111112333;
    
    /* 确定变量类型 */
    if (tokens[cur_token_idx].get_val_string() != CHAR_SYM && tokens[cur_token_idx].get_val_string() != INT_SYM) {
        error_message("Variable definition should begin with char/int, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_variable_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got int/char in variable definition but wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_variable_definition = false;
    }
    if (tokens[cur_token_idx].get_val_string() == INT_SYM) {
        var_type = INT_SYMBOL_TYPE;
    } else if (tokens[cur_token_idx].get_val_string() == CHAR_SYM) {
        var_type = CHAR_SYMBOL_TYPE;
    }
    
    int n_variable_definitions = 0;
    do {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_variable_definition = false;
        }
        var_name = tokens[cur_token_idx].get_val_string();
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* 是数组 */
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message(
                        "Got [ but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_variable_definition = false;
            }
            ++cur_token_idx;
            if (tokens[cur_token_idx].get_output_type() == ZERO) {
                error_message("Array size should be larger than 0!");
                is_correct_variable_definition = false;
            }
            if (!parse_unsigned_integer(length)) {
                error_message("Expected non-zero unsigned integer for array size, instead got: " +
                              tokens[cur_token_idx].get_val_string());
                is_correct_variable_definition = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM) {
                error_message("Expected ], instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_variable_definition = false;
            }
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message(
                        "Got ] but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_variable_definition = false;
            }
            ++cur_token_idx;
        } else { /* 不是数组, 就是单个变量 */
            length = 0;
        }
        /* 变量入表 */
        if (function_name.empty()) {
            if (!insert_global_variable(var_name, var_type, length)) {
                is_correct_variable_definition = false;
            }
        } else {
            if (!insert_local_variable(function_name, var_name, var_type, length)) {
                is_correct_variable_definition = false;
            }
        }
        ++n_variable_definitions;
    } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
             tokens[cur_token_idx].get_output_type() == SEPARATOR && n_variable_definitions < MAX_N_VAR_DEFINITIONS);
    
    if (is_correct_variable_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct variable definition!"
                  << std::endl;
    }
    return is_correct_variable_definition;
}

bool parse_variable_declarations(const std::string &function_name) {
    bool is_correct_variable_declarations = true;
    int n_variable_definitions = 0;
    while (n_variable_definitions < MAX_N_VAR_DEFINITIONS) {
        if (!parse_variable_definition(function_name)) {
            error_message("Invalid variable definition!");
            is_correct_variable_declarations = false;
        }
        ++n_variable_definitions;
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM) {
            error_message("Expected ; instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_variable_declarations = false;
        }
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Got ; but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_variable_declarations = false;
        }
        ++cur_token_idx;
        if ((tokens[cur_token_idx].get_val_string() == CHAR_SYM || tokens[cur_token_idx].get_val_string() == INT_SYM) &&
            tokens[cur_token_idx + 1].get_output_type() == IDENTIFIER &&
            (tokens[cur_token_idx + 2].get_val_string() == SEMICOLON_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == LBRACKET_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM)) { /* 下一个还是变量定义 */
            if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
                error_message("Got int/char in variable declarations but wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_variable_declarations = false;
            }
            if (tokens[cur_token_idx + 2].get_output_type() != SEPARATOR) {
                error_message("Got ; or , or [ in variable declarations but wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx + 2].get_output_type()]);
                is_correct_variable_declarations = false;
            }
        } else {
            break;
        }
    }
    if (is_correct_variable_declarations) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct variable declarations! ("
                  << (function_name.empty() ? "global" : function_name) << ")" << std::endl;
    }
    return is_correct_variable_declarations;
}

bool parse_parameter_list(const std::string &function_name) {
    bool is_correct_parameter_list = true;
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM) { /* 空参数表 */
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got ) but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_parameter_list = false;
        }
        return is_correct_parameter_list;
    }
    std::string parameter_name = "***<<<***?>>>?";
    SymbolType parameter_type = UNKNOWN_SYMBOL_TYPE;
    int n_parameters = 0;
    
    /* 有参数 */
    --cur_token_idx; // 第一次进循环要先退一步
    do {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
            error_message(
                    "Expected int/char in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_parameter_list = false;
        }
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message(
                    "Got int/char but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_parameter_list = false;
        }
        if (tokens[cur_token_idx].get_val_string() == INT_SYM) {
            parameter_type = INT_SYMBOL_TYPE;
        } else if (tokens[cur_token_idx].get_val_string() == CHAR_SYM) {
            parameter_type = CHAR_SYMBOL_TYPE;
        }
        ++cur_token_idx;
        
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message(
                    "Expected identifier in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_parameter_list = false;
        }
        parameter_name = tokens[cur_token_idx].get_val_string();
        insert_parameter(function_name, parameter_name, parameter_type);
        ++cur_token_idx;
        ++n_parameters;
    } while (tokens[cur_token_idx].get_output_type() == SEPARATOR &&
             tokens[cur_token_idx].get_val_string() == COMMA_SYM && n_parameters < MAX_N_PARAMETERS_LIMIT);
    return is_correct_parameter_list;
}

bool parse_block(const std::string &function_name) {
    bool is_correct_block = true;
    if (tokens[cur_token_idx].get_val_string() == CONST_SYM) {
        if (!parse_const_declarations(function_name)) {
            error_message("Invalid const declarations!");
            is_correct_block = false;
        }
    }
    
    if (tokens[cur_token_idx].get_val_string() == CHAR_SYM || tokens[cur_token_idx].get_val_string() == INT_SYM) {
        if (!parse_variable_declarations(function_name)) {
            error_message("Invalid variable declarations!");
            is_correct_block = false;
        }
    }
    
    int n_statements = 0;
    while (tokens[cur_token_idx].get_val_string() != RBRACE_SYM && n_statements < MAX_N_STATEMENTS_LIMIT) {
        if (!parse_statements(function_name)) {
            error_message("Invalid statements!");
            is_correct_block = false;
            break;
        }
        if (tokens[cur_token_idx].get_val_string() == RBRACE_SYM &&
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got } but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_block = false;
        }
        ++n_statements;
    }
    if (is_correct_block) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct block!" << std::endl;
    }
    return is_correct_block;
}

bool parse_statements(const std::string &function_name) { // TODO: 改类型判断, 生成各种语句四元式
    if (function_name.empty() || !is_function(function_name)) {
        error_message("Undefined/No function name passed in statements: " + function_name);
        return false;
    }
    bool is_correct_statements = true;
    if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* 一个分号的语句 */
        ++cur_token_idx;
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct ; statement!" << std::endl;
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == RETURN_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* return; 语句 */
            ++cur_token_idx;
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct return statement!"
                      << std::endl;
            return is_correct_statements;
        }
        /* return (...); 语句 */
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("( is expected in return statement, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (!parse_expression(function_name)) {
            error_message("Invalid expression!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(") is expected in return statement, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "; is expected after return statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct return statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == SCANF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_scanf_statement(function_name)) {
            error_message("Invalid scanf statement!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "; is expected after scanf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct scanf statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == PRINTF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_printf_statement(function_name)) {
            error_message("Invalid printf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "; is expected after printf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct printf statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER &&
        (tokens[cur_token_idx + 1].get_val_string() != LPARENTHESIS_SYM ||
         tokens[cur_token_idx + 1].get_output_type() != SEPARATOR)) { /* 赋值语句 */
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) { /* 数组元素赋值 */
            ++cur_token_idx;
            if (!parse_expression(function_name)) {
                error_message("Invalid expression!");
                is_correct_statements = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM ||
                tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("] is expected in assignment statement! Instead got: " +
                              tokens[cur_token_idx].get_val_string());
                is_correct_statements = false;
            }
            ++cur_token_idx;
        }
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "= is expected in assignment statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (!parse_expression(function_name)) {
            error_message("Invalid expression!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "; is expected after assignment statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct assignment statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER) { /* 函数调用 */
        if (!parse_funtion_call(function_name)) {
            error_message("Invalid function call in function-call statement!");
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("; is expected after function call in function-call statement! Instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct function call statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == IF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_conditional_statement(function_name)) {
            error_message("Invalid conditional statement!");
            is_correct_statements = false;
        }
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct if statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if ((tokens[cur_token_idx].get_val_string() == DO_SYM || tokens[cur_token_idx].get_val_string() == FOR_SYM) &&
        tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_loop_statement(function_name)) {
            error_message("Invalid loop statement!");
            is_correct_statements = false;
        }
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct loop statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == LBRACE_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        int n_inner_statements = 0;
        const int MAX_N_INNER_STATEMENTS = 123456;
        while ((tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
                tokens[cur_token_idx].get_output_type() != SEPARATOR) && n_inner_statements < MAX_N_INNER_STATEMENTS) {
            if (!parse_statements(function_name)) {
                error_message("Invalid inner statements!");
                is_correct_statements = false;
            }
            ++n_inner_statements;
        }
        if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected } after compound statement, got instead: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct compound statements!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    return is_correct_statements;
}

bool parse_loop_statement(const std::string &function_name) {
    bool is_correct_loop_statement = true;
    if (tokens[cur_token_idx].get_val_string() != DO_SYM && tokens[cur_token_idx].get_val_string() != FOR_SYM) {
        error_message(
                "Expected do or for to enter a loop statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_loop_statement = false;
    }
    /* 是 do while 循环 */
    if (tokens[cur_token_idx].get_val_string() == DO_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        ++cur_token_idx;
        if (!parse_statements(function_name)) {
            error_message("Invalid statements in do-while loop!");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != WHILE_SYM || tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message("Expected while in do-while loop, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ( after while, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_condition(function_name)) {
            error_message("Invalid condition in while");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ) after while condition, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        return is_correct_loop_statement;
    }
    /* 是 for 循环 */
    if (tokens[cur_token_idx].get_val_string() == FOR_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ( after for, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for-loop init part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected = in for-loop init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_expression(function_name)) {
            error_message("Invalid expression in for-loop init part");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected ; in for loop after init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_condition(function_name)) {
            error_message("Invalid condition in for-loop!");
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected ; in for-loop after init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for-loop updating part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected = in for-loop updating part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for-loop updating part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if ((tokens[cur_token_idx].get_val_string() != MINUS_SYM &&
             tokens[cur_token_idx].get_val_string() != PLUS_SYM) ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected +/- in for-loop updating part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        int updating_size = -555555;
        if (!parse_unsigned_integer(updating_size)) {
            error_message("Expected an unsigned integer in for-loop updating part, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected ) after for-loop condition, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        if (!parse_statements(function_name)) {
            error_message("Invalid statements in for-loop!");
            is_correct_loop_statement = false;
        }
        return is_correct_loop_statement;
    }
    return is_correct_loop_statement;
}

bool parse_conditional_statement(const std::string &function_name) {
    bool is_correct_conditional_statement = true;
    if (tokens[cur_token_idx].get_val_string() != IF_SYM || tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Expected if in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    if (!parse_condition(function_name)) {
        error_message("Invalid condition!");
        is_correct_conditional_statement = false;
    }
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    if (!parse_statements(function_name)) {
        error_message("Invalid statement in if!");
        is_correct_conditional_statement = false;
    }
    if (tokens[cur_token_idx].get_val_string() == ELSE_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        ++cur_token_idx;
        if (!parse_statements(function_name)) {
            error_message("Invalid statement in else!");
            is_correct_conditional_statement = false;
        }
    }
    return is_correct_conditional_statement;
}

bool parse_condition(const std::string &function_name) {
    bool is_correct_condition = true;
    if (!parse_expression(function_name)) {
        error_message("Invalid expression in condition");
        is_correct_condition = false;
    }
    if ((tokens[cur_token_idx].get_val_string() == EQ_SYM || tokens[cur_token_idx].get_val_string() == NE_SYM ||
         tokens[cur_token_idx].get_val_string() == LT_SYM || tokens[cur_token_idx].get_val_string() == GT_SYM ||
         tokens[cur_token_idx].get_val_string() == LE_SYM || tokens[cur_token_idx].get_val_string() == GE_SYM) &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        if (!parse_expression(function_name)) {
            error_message("Invalid expression after comparison operators in condition");
            is_correct_condition = false;
        }
    }
    return is_correct_condition;
}

bool parse_printf_statement(const std::string &function_name) {
    bool is_correct_printf_statement = true;
    if (tokens[cur_token_idx].get_val_string() != PRINTF_SYM || tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Expected printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( after printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() == STRING) { /* 字符串 */
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* 字符串后面接一个表达式 */
            ++cur_token_idx;
            if (!parse_expression(function_name)) {
                error_message("Expected expression!");
                is_correct_printf_statement = false;
            }
        }
    } else if (!parse_expression(function_name)) { /* 只有表达式 */
        error_message("Expected expression!");
        is_correct_printf_statement = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    return is_correct_printf_statement;
}

bool parse_scanf_statement(const std::string &function_name) {
    bool is_correct_scanf_statement = true;
    if (tokens[cur_token_idx].get_val_string() != SCANF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        error_message("Expected scanf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected (, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    while (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
           tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_scanf_statement = false;
        }
        ++cur_token_idx;
    }
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after scanf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    return is_correct_scanf_statement;
}

bool parse_expression(const std::string &function_name) {
    bool is_correct_expression = true;
    if ((tokens[cur_token_idx].get_val_string() == MINUS_SYM || tokens[cur_token_idx].get_val_string() == PLUS_SYM) &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        /* 这里有个巨坑: 词法分析去了引号, 导致 "+", '+', + 的 val_string 都是 + 傻傻分不清, 还需要通过 output_type 判断!!! */
        ++cur_token_idx;
    }
    
    if (!parse_term(function_name)) {
        error_message("Invalid term!");
        is_correct_expression = false;
    }
    
    while ((tokens[cur_token_idx].get_val_string() == MINUS_SYM ||
            tokens[cur_token_idx].get_val_string() == PLUS_SYM) &&
           tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        if (!parse_term(function_name)) {
            error_message("Invalid term!");
            is_correct_expression = false;
        }
    }
    return is_correct_expression;
}

bool parse_term(const std::string &function_name) {
    bool is_correct_term = true;
    if (!parse_factor(function_name)) {
        error_message("Invalid first factor in term!");
        is_correct_term = false;
    }
    while ((tokens[cur_token_idx].get_val_string() == MUL_SYM || tokens[cur_token_idx].get_val_string() == DIV_SYM) &&
           tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        if (!parse_factor(function_name)) {
            error_message("Invalid optional factors in term!");
            is_correct_term = false;
        }
    }
    return is_correct_term;
}

bool parse_factor(const std::string &function_name) {
    bool is_correct_factor = true;
    if (tokens[cur_token_idx].get_output_type() == CHAR) { /* 字符 */
        ++cur_token_idx;
        return true;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* 数组 */
            ++cur_token_idx;
            if (!parse_expression(function_name)) {
                error_message("Invalid expression!");
                is_correct_factor = false;
            }
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM ||
                tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("Expected ], instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_factor = false;
            }
            ++cur_token_idx;
            return is_correct_factor;
        }
        
        if (tokens[cur_token_idx].get_val_string() == LPARENTHESIS_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* 函数调用 */
            /* 退一个单词, 到函数名 */
            --cur_token_idx;
            
            if (!parse_funtion_call(function_name)) {
                error_message("Invalid function call!");
                is_correct_factor = false;
            }
            return is_correct_factor;
        }
        /* 就是单个标识符 */
        return is_correct_factor;
    }
    
    if (tokens[cur_token_idx].get_val_string() == LPARENTHESIS_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* 表达式 */
        ++cur_token_idx;
        if (!parse_expression(function_name)) {
            error_message("Invalid expression!");
            is_correct_factor = false;
        }
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ) in factor expression, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_factor = false;
        }
        ++cur_token_idx;
        return is_correct_factor;
    }
    
    int res = 666666;
    if (!parse_integer(res)) { /* 整数 */
        error_message("Invalid Integer!");
        is_correct_factor = false;
    }
    return is_correct_factor;
}

bool parse_funtion_call(const std::string &function_name) {
    bool is_correct_function_call = true;
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Function call expected identifier as the beginning, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_function_call = false;
    }
    // TODO: 实现函数调用中间代码
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( before value parameters in function call, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_function_call = false;
    }
    ++cur_token_idx;
    if (!parse_value_parameters(function_name)) {
        error_message("Invalid value parameters!");
        is_correct_function_call = false;
    }
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after value parameters in function call, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_function_call = false;
    }
    ++cur_token_idx;
    return is_correct_function_call;
}

bool parse_value_parameters(const std::string &function_name) {
    bool is_value_parameters = true;
    // TODO: 检查参数是否匹配 (个数和类型)
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* 0 参数 */
        return is_value_parameters;
    }
    if (!parse_expression(function_name)) {
        error_message("Invalid expression!");
        is_value_parameters = false;
    }
    int n_value_parameters = 1;
    while (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
           tokens[cur_token_idx].get_output_type() == SEPARATOR && n_value_parameters < MAX_N_PARAMETERS_LIMIT) {
        ++cur_token_idx;
        if (!parse_expression(function_name)) {
            error_message("Invalid expression!");
            is_value_parameters = false;
        }
        ++n_value_parameters;
    }
    return is_value_parameters;
}

bool parse_main_function_definition() {
    bool is_correct_main_function_definition = true;
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message("main function should start with void, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got void symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_main_function_definition = false;
    }
    SymbolType return_type = VOID_SYMBOL_TYPE;
    ++cur_token_idx;
    
    if (tokens[cur_token_idx].get_val_string() != MAIN_SYM) {
        error_message("Expected main, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got main symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_main_function_definition = false;
    }
    std::string function_name = tokens[cur_token_idx].get_val_string();
    insert_function(function_name, return_type);
    ++cur_token_idx;
    
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( after main, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after main(, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM || tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected {, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    ++cur_token_idx;
    
    if (!parse_block(function_name)) {
        error_message("Invalid block in main function definition!");
        is_correct_main_function_definition = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM || tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected } in main function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    if (is_correct_main_function_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num()
                  << ": finish correct void function definition! (" + function_name + ")" << std::endl;
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
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got int/char symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_non_void_function_definiton = false;
    }
    SymbolType return_type = tokens[cur_token_idx].get_val_string() == CHAR_SYM ? CHAR_SYMBOL_TYPE : INT_SYMBOL_TYPE;
    ++cur_token_idx;
    
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier as name in non void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    std::string function_name = tokens[cur_token_idx].get_val_string();
    insert_function(function_name, return_type);
    ++cur_token_idx;
    
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected ( in non void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    ++cur_token_idx;
    
    if (!parse_parameter_list(function_name)) {
        error_message("Invalid parameter list in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after parameter list in non void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM || tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected { in non void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (!parse_block(function_name)) {
        error_message("Invalid block in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM || tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected } in non void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (is_correct_non_void_function_definiton) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num()
                  << ": finish correct non void function definition! (" << function_name << ")" << std::endl;
    }
    return is_correct_non_void_function_definiton;
}

bool parse_void_function_definition() {
    bool is_correct_void_function_definition = true;
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message("Void functions should start with void, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got void symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_void_function_definition = false;
    }
    SymbolType return_type = VOID_SYMBOL_TYPE;
    ++cur_token_idx;
    
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier as name in void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    std::string function_name = tokens[cur_token_idx].get_val_string();
    insert_function(function_name, return_type);
    ++cur_token_idx;
    
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (!parse_parameter_list(function_name)) {
        error_message("Invalid parameter list in void function definition!");
        is_correct_void_function_definition = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM || tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected { in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (!parse_block(function_name)) {
        error_message("Invalid block in void function definition!");
        is_correct_void_function_definition = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM || tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected } in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    
    ++cur_token_idx;
    if (is_correct_void_function_definition) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num()
                  << ": finish correct void function definition! (" << function_name << ")" << std::endl;
    }
    return is_correct_void_function_definition;
}

bool parse_program() {
    bool is_correct_program = true;
    tokenize();
    init_symbol_tables();
    
    std::cout << "----------------------Optional Global Constant declarations----------------------" << std::endl;
    if (tokens[cur_token_idx].get_val_string() == CONST_SYM) { /* 是常量声明 */
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message(
                    "Got const but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_program = false;
        }
        if (!parse_const_declarations()) {
            error_message("Invalid constant declarations!");
            is_correct_program = false;
        }
    }
    
    std::cout << "----------------------Optional Global Variable declarations----------------------" << std::endl;
    if ((tokens[cur_token_idx].get_val_string() == INT_SYM || tokens[cur_token_idx].get_val_string() == CHAR_SYM) &&
        tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (tokens[cur_token_idx + 1].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx + 1].get_val_string());
            is_correct_program = false;
        }
        /* 是变量 或 有返回值函数声明 */
        // 应是 "(" 或 "[" 或 ";" 或 ","
        if ((tokens[cur_token_idx + 2].get_val_string() == LBRACKET_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == SEMICOLON_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM) &&
            tokens[cur_token_idx + 2].get_output_type() == SEPARATOR) { /* 是变量声明 */
            if (!parse_variable_declarations()) { /* 分析变量声明 */
                error_message("Invalid variable declarations!");
                is_correct_program = false;
            }
            std::cout << "-----------------------------------Functions------------------------------" << std::endl;
        } else if (tokens[cur_token_idx + 2].get_val_string() == LPARENTHESIS_SYM &&
                   tokens[cur_token_idx + 2].get_output_type() == SEPARATOR) { /* 是有返回值的函数 */
            std::cout << "-----------------------------------Functions------------------------------" << std::endl;
            if (!parse_non_void_function_definition()) { /* 分析当前该有返回值的函数 */
                error_message("Invalid non-void function!");
                is_correct_program = false;
            }
        } else {
            error_message("Expected ( or [ or , or ; instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_program = false;
        }
    }
    
    /* 分析之后的至少包括 void main() 的函数 */
    bool reached_main = false;
    int n_functions = 1;
    while (tokens[cur_token_idx].get_val_string() == INT_SYM || tokens[cur_token_idx].get_val_string() == CHAR_SYM ||
           tokens[cur_token_idx].get_val_string() == VOID_SYM) {
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message("Got int/char/void symbol but as wrong type: " +
                          token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_program = false;
        }
        if (tokens[cur_token_idx + 1].get_val_string() == MAIN_SYM) {
            if (tokens[cur_token_idx + 1].get_output_type() != KEYWORD) {
                error_message("Got main symbol but as wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx + 1].get_output_type()]);
                is_correct_program = false;
            }
            reached_main = true;
            parse_main_function_definition();
        } else if (tokens[cur_token_idx + 1].get_output_type() == IDENTIFIER &&
                   tokens[cur_token_idx + 2].get_val_string() == LPARENTHESIS_SYM) {
            if (tokens[cur_token_idx + 2].get_output_type() != SEPARATOR) {
                error_message("Got } symbol but as wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx + 2].get_output_type()]);
                is_correct_program = false;
            }
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
        if (n_functions > MAX_N_FUNCTIONS_LIMIT || cur_token_idx >= tokens.size()) {
            error_message("Exceed max number of functions limit " + std::to_string(MAX_N_FUNCTIONS_LIMIT) + "!");
            break;
        }
    }
    
    if (is_correct_program && tokens[cur_token_idx].get_line_num() == line_count) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct program!" << std::endl;
    }
    return is_correct_program;
}
