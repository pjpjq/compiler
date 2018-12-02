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
        tokens[cur_token_idx].get_output_type() == ZERO) { /* �޷������� */
        return parse_unsigned_integer(res);
    }
    /* ����ʽ����ǰ׺ +/- */
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
    std::string const_name = "[INVALID const name !!!]";
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
        /* int/char const ����ű� */
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
    
    /* ȷ���������� */
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
    
    /* ������� */
    int n_variable_definitions = 0;
    do {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_variable_definition = false;
        }
        var_name = tokens[cur_token_idx].get_val_string();
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* ������ */
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
        } else { /* ��������, ���ǵ������� */
            length = 0;
        }
        /* ������� */
        if (function_name.empty()) {
            if (!insert_global_variable(var_name, var_type, length)) {
                is_correct_variable_definition = false;
            }
        } else {
            if (!insert_local_variable(function_name, var_name, var_type, length)) {
                is_correct_variable_definition = false;
            }
        }
        /* ������Ԫʽ */
        emit(Quadruple(DECLARE_VARIABLE_OP, symbol_type_strs[var_type], var_name, std::to_string(length)));
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
             tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM)) { /* ��һ�����Ǳ������� */
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
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM) { /* �ղ����� */
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got ) but wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_parameter_list = false;
        }
        return is_correct_parameter_list;
    }
    std::string parameter_name = "***<<<***?>>>?";
    SymbolType parameter_type = UNKNOWN_SYMBOL_TYPE;
    
    /* �в��� */
    --cur_token_idx; // ��һ�ν�ѭ��Ҫ����һ��
    do {
        ++cur_token_idx;
        /* �������� */
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
        /* ��������� */
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message(
                    "Expected identifier in parameter list, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_parameter_list = false;
        }
        parameter_name = tokens[cur_token_idx].get_val_string();
        /* �������, ������Ԫʽ */
        insert_parameter(function_name, parameter_name, parameter_type);
        emit(Quadruple(DEFINE_PARAMETER_OP, symbol_type_strs[parameter_type], parameter_name, ""));
        
        ++cur_token_idx;
    } while (tokens[cur_token_idx].get_output_type() == SEPARATOR &&
             tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
             get_function(function_name)->parameter_table.size() < MAX_N_PARAMETERS_LIMIT);
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

bool parse_statements(const std::string &function_name) { // TODO: �������ж�, ���ɸ��������Ԫʽ
    if (!is_function(function_name)) {
        error_message("Undefined/No function name passed in statements: " + function_name);
        return false;
    }
    bool is_correct_statements = true;
    if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* һ���ֺŵ���� */
        ++cur_token_idx;
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct ; statement!" << std::endl;
        return is_correct_statements;
    }
    
    if (tokens[cur_token_idx].get_val_string() == RETURN_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* return; ��� */
            emit(Quadruple(RETURN_OP, "", "", ""));
            ++cur_token_idx;
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct return statement!"
                      << std::endl;
            return is_correct_statements;
        }
        /* return (...); ��� */
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("( is expected in return statement, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        
        bool is_return_val_imm;
        int return_val = -23333;
        std::string temp_var_to_return = "[INVALID temp_var_to_return !!!]";
        SymbolType return_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_return_val_imm, return_val, temp_var_to_return, return_type)) {
            error_message("Invalid expression!");
            is_correct_statements = false;
        }
        emit(Quadruple(RETURN_OP, is_return_val_imm ? std::to_string(return_val) : temp_var_to_return, "", ""));
        
        /* ) */
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
    
    // TODO scanf
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
    
    // TODO printf
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
        tokens[cur_token_idx + 1].get_val_string() != LPARENTHESIS_SYM) { /* ��ֵ��� */
        ++cur_token_idx;
        bool is_write_to_array = false;
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* ����Ԫ�ظ�ֵ */
            is_write_to_array = true;
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("Got ] but in wrong type: " + symbol_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_statements = false;
            }
            ++cur_token_idx;
            
            bool is_offset_imm;
            int offset_val = -2333;
            std::string offset_temp_var = "offset";
            SymbolType offset_type = UNKNOWN_SYMBOL_TYPE;
            if (!parse_expression(function_name, is_offset_imm, offset_val, offset_temp_var, offset_type)) {
                error_message("Invalid expression!");
                is_correct_statements = false;
            }
            
            /* ] */
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM ||
                tokens[cur_token_idx].get_output_type() != SEPARATOR) {
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
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got = but in wrong type: " + symbol_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_statements = false;
        }
        ++cur_token_idx;
        
        /* copyee */
        bool is_copyee_imm;
        int copyee_val = -23334444;
        std::string copyee_temp_var = "copyee";
        SymbolType copyee_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_copyee_imm, copyee_val, copyee_temp_var, copyee_type)) {
            error_message("Invalid expression in assignment copyee part!");
            is_correct_statements = false;
        }
        
        /* ; */
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
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER &&
        tokens[cur_token_idx + 1].get_val_string() == LPARENTHESIS_SYM) { /* �������� */
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
    
    bool is_condition_imm;
    int condition_val = -23323233;
    std::string condition_temp_var = "-233-233";
    SymbolType condition_type = UNKNOWN_SYMBOL_TYPE;
    /* �� do while ѭ�� */
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
        
        if (!parse_condition(function_name, is_condition_imm, condition_val, condition_temp_var, condition_type)) {
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
    /* �� for ѭ�� */
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
        
        /* init_copyee */
        bool is_init_copyee_imm;
        int init_copyee_val;
        std::string init_copyee_temp_var;
        SymbolType init_copyee_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_init_copyee_imm, init_copyee_val, init_copyee_temp_var,
                              init_copyee_type)) {
            error_message("Invalid expression in for-loop init part");
            is_correct_loop_statement = false;
        }
        
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected ; in for loop after init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        
        /* condition */
        if (!parse_condition(function_name, is_condition_imm, condition_val, condition_temp_var, condition_type)) {
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
    
    bool is_condition_imm;
    int condition_val = -3333332;
    std::string condition_temp_var = "-2333332";
    SymbolType condition_type = UNKNOWN_SYMBOL_TYPE;
    if (!parse_condition(function_name, is_condition_imm, condition_val, condition_temp_var, condition_type)) {
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

bool parse_condition(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                     SymbolType &res_type) {
    bool is_correct_condition = true;
    
    /* ������� */
    bool is_condition_left_imm;
    int condition_left_val = 996;
    std::string condition_left_temp_var = "996...";
    SymbolType condition_left_type = UNKNOWN_SYMBOL_TYPE;
    if (!parse_expression(function_name, is_condition_left_imm, condition_left_val, condition_left_temp_var,
                          condition_left_type)) {
        error_message("Invalid expression in condition");
        is_correct_condition = false;
    }
    
    /* ��ѡ�ıȽ������ */
    if ((tokens[cur_token_idx].get_val_string() == EQ_SYM || tokens[cur_token_idx].get_val_string() == NE_SYM ||
         tokens[cur_token_idx].get_val_string() == LT_SYM || tokens[cur_token_idx].get_val_string() == GT_SYM ||
         tokens[cur_token_idx].get_val_string() == LE_SYM || tokens[cur_token_idx].get_val_string() == GE_SYM) &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        
        /* �бȽ�����������ұ� */
        bool is_condition_right_imm;
        int condition_right_val = 1145;
        std::string condition_right_temp_var = "11..4..5";
        SymbolType condition_right_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_condition_right_imm, condition_right_val, condition_right_temp_var,
                              condition_right_type)) {
            error_message("Invalid expression after comparison operators in condition");
            is_correct_condition = false;
        }
    }
    /* ������ */
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
    
    /* ����Ҫ����ı��ʽ */
    bool is_printee_imm;
    int printee_val = 88888;
    std::string printee_temp_var = "8888";
    SymbolType printee_type = UNKNOWN_SYMBOL_TYPE;
    if (tokens[cur_token_idx].get_output_type() == STRING) { /* �ַ��� */
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* �ַ��������һ�����ʽ */
            ++cur_token_idx;
            
            if (!parse_expression(function_name, is_printee_imm, printee_val, printee_temp_var, printee_type)) {
                error_message("Invalid expression!");
                is_correct_printf_statement = false;
            }
        }
    } else if (!parse_expression(function_name, is_printee_imm, printee_val, printee_temp_var,
                                 printee_type)) { /* ֻ�б��ʽ */
        error_message("Invalid expression!");
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

bool parse_expression(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                      SymbolType &res_type) {
    bool is_correct_expression = true;
    
    /* ��Ĭ�Ͻ����ֵ */
    is_res_imm = true;
    res_val = 0;
    res_temp_var = create_temp_var();
    res_type = CHAR_SYMBOL_TYPE;
    
    std::string op = PLUS_SYM;
    
    /* ��ѡ��+-�� */
    if ((tokens[cur_token_idx].get_val_string() == MINUS_SYM || tokens[cur_token_idx].get_val_string() == PLUS_SYM) &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        op = tokens[cur_token_idx].get_val_string();
        res_type = INT_SYMBOL_TYPE;
        ++cur_token_idx;
    }
    
    /* ����һ���� */
    int n_terms = 0;
    --cur_token_idx;
    do {
        ++cur_token_idx;
        ++n_terms;
        
        bool is_term_imm = false;
        int term_val = -22333;
        std::string term_temp_var = "[INVALID term_temp_var-" + std::to_string(n_terms) + " ??????????]";
        SymbolType term_type = UNKNOWN_SYMBOL_TYPE;
        
        if (!parse_term(function_name, is_term_imm, term_val, term_temp_var, term_type)) {
            error_message("Invalid term in expression! (term num " + std::to_string(n_terms));
            is_correct_expression = false;
        }
        if (term_type == INT_SYMBOL_TYPE) {
            res_type = INT_SYMBOL_TYPE;
        }
        // TODO further �����ϲ�
        if (is_res_imm) { /* ֮ǰ���һֱ�������� */
            if (!is_term_imm) { /* ���ֵ�һ����ʱ���� */
                is_res_imm = false;
//                res_type = INT_SYMBOL_TYPE;
                emit(Quadruple(op, std::to_string(res_val), term_temp_var, res_temp_var));
            } else { /* ������Ӽ� */
                // DEBUG
                emit(Quadruple(op, std::to_string(res_val), std::to_string(term_val), std::to_string(
                        op == PLUS_SYM ? res_val + term_val : op == MINUS_SYM ? res_val - term_val : -2333333)));
                
                res_val = op == PLUS_SYM ? res_val + term_val : op == MINUS_SYM ? res_val - term_val : -2333333;
            }
        } else { /* ֮ǰ����б��� */
            res_type = INT_SYMBOL_TYPE;
            emit(Quadruple(op, res_temp_var, is_term_imm ? std::to_string(term_val) : term_temp_var, res_temp_var));
        }
        op = tokens[cur_token_idx].get_val_string();
    } while ((tokens[cur_token_idx].get_val_string() == MINUS_SYM ||
              tokens[cur_token_idx].get_val_string() == PLUS_SYM) &&
             tokens[cur_token_idx].get_output_type() == SEPARATOR && n_terms < MAX_N_TERMS_PER_EXPRESSION);
    return is_correct_expression;
}

bool parse_term(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                SymbolType &res_type) {
    bool is_correct_term = true;
    
    /* ��Ĭ�Ͻ����ֵ */
    is_res_imm = true;
    res_val = 1;
    res_temp_var = create_temp_var();
    res_type = CHAR_SYMBOL_TYPE;
    
    std::string op = MUL_SYM;
    
    /* ����һ������ */
    int n_factors = 0;
    --cur_token_idx;
    do {
        ++cur_token_idx;
        ++n_factors;
        
        bool is_factor_imm = false;
        int factor_val = -233;
        std::string factor_temp_var = "[INVALID factor_temp_var-" + std::to_string(n_factors) + " ???????]";
        SymbolType factor_type = UNKNOWN_SYMBOL_TYPE;
        
        if (!parse_factor(function_name, is_factor_imm, factor_val, factor_temp_var, factor_type)) {
            error_message("Invalid factor in term! (factor num " + std::to_string(n_factors));
            is_correct_term = false;
        }
        if (factor_type == INT_SYMBOL_TYPE) {
            res_type = INT_SYMBOL_TYPE;
        }
        
        if (op == DIV_SYM && is_factor_imm && factor_val == 0) {
            error_message("Divided by 0 is not defined!");
            factor_val = 1;
        }
        // TODO further �����ϲ�
        if (is_res_imm) { /* ֮ǰ�����ӽ�����ǳ��� */
            if (!is_factor_imm) { /* ���ֵ�һ����ʱ���� ��δ������ */
                is_res_imm = false;
//                res_type = INT_SYMBOL_TYPE;
                emit(Quadruple(op, std::to_string(res_val), factor_temp_var, res_temp_var));
            } else { /* �����˳� */
                // DEBUG
//                emit(Quadruple(op, std::to_string(res_val), std::to_string(factor_val), std::to_string(
//                        op == MUL_SYM ? res_val * factor_val : op == DIV_SYM ? res_val / factor_val : -23333)));
                res_val = op == MUL_SYM ? res_val * factor_val : op == DIV_SYM ? res_val / factor_val : -23333;
            }
        } else { /* ֮ǰ���Ǳ��� */
            res_type = INT_SYMBOL_TYPE;
            emit(Quadruple(op, res_temp_var, is_factor_imm ? std::to_string(factor_val) : factor_temp_var,
                           res_temp_var));
        }
        op = tokens[cur_token_idx].get_val_string();
    } while ((tokens[cur_token_idx].get_val_string() == MUL_SYM || tokens[cur_token_idx].get_val_string() == DIV_SYM) &&
             tokens[cur_token_idx].get_output_type() == SEPARATOR && n_factors < MAX_N_FACTORS_PER_TERM);
    
    return is_correct_term;
}

bool parse_factor(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                  SymbolType &res_type) {
    bool is_correct_factor = true;
    
    /* ��Ĭ�Ͻ����ֵ */
    is_res_imm = true;
    res_val = 0;
    res_temp_var = "got char imm, so this temp_var is useless!";
    res_type = CHAR_SYMBOL_TYPE;
    
    if (tokens[cur_token_idx].get_output_type() == CHAR) { /* ���ַ� */
        res_val = tokens[cur_token_idx].get_val_int();
        ++cur_token_idx;
        return is_correct_factor;
    }
    
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER) { /* �� �������� �� ���� �� ����������ʶ�� */
        std::string identifier_name = tokens[cur_token_idx].get_val_string();
        
        if (tokens[cur_token_idx + 1].get_val_string() == LPARENTHESIS_SYM) { /* �Ǻ������� */
            if (tokens[cur_token_idx + 1].get_output_type() != SEPARATOR) {
                error_message("Got ( but as wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx + 1].get_output_type()]);
                is_correct_factor = false;
            }
            
            if (!parse_funtion_call(function_name)) {
                error_message("Invalid function call!");
                is_correct_factor = false;
            }
            
            is_res_imm = false;
            res_val = -2333111;
            res_temp_var = create_temp_var();
            res_type = get_function(identifier_name)->get_symbol_type();
            
            if (res_type == VOID_SYMBOL_TYPE) {
                error_message("Trying to get return value of void function: " + identifier_name);
                res_type = INT_SYMBOL_TYPE;
                is_correct_factor = false;
            }
            
            emit(Quadruple(ASSIGN_RETURN_VAL_OP, "", "", res_temp_var));
            return is_correct_factor;
        }
//        std::cout << function_name << " has " << identifier_name << std::endl;
        /* �������� */
        Symbol *var = get_non_function_symbol(function_name, identifier_name);
        if (!var) {
            is_correct_factor = false;
        } else if (typeid(*var).name() == typeid(ConstantSymbol).name()) { /* �ǳ��� */
            is_res_imm = true;
            res_val = var->get_value();
            res_temp_var = "got imm, so this temp_var is useless!";
            res_type = var->get_symbol_type();
            
            ++cur_token_idx;
            return is_correct_factor;
        } else if (typeid(*var).name() == typeid(VariableSymbol).name()) { /* �Ǳ��� */
            is_res_imm = false;
            res_val = -23333;
            res_temp_var = "#" + var->get_name();
            res_type = var->get_symbol_type();
            if (tokens[cur_token_idx + 1].get_val_string() == LBRACKET_SYM) {
                if (var->get_length() <= 0) {
                    error_message("Array variable " + var->get_name() + " should have length >0 instead of: " +
                                  std::to_string(var->get_length()));
                    is_correct_factor = false;
                }
            } else {
                if (var->get_length() > 0) {
                    error_message("Single variable " + var->get_name() + " should have length 0 instead of: " +
                                  std::to_string(var->get_length()));
                    is_correct_factor = false;
                }
            }
        } else {
            error_message("Invalid identifier type as array variable: " + std::string(typeid(var).name()));
            is_correct_factor = false;
        }
        
        if (tokens[cur_token_idx + 1].get_val_string() == LBRACKET_SYM) { /* ������ */
//            std::cout <<"------------>" + identifier_name  + " is array" << std::endl;
            if (tokens[cur_token_idx + 1].get_output_type() != SEPARATOR) {
                error_message("Got [ but as wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx + 1].get_output_type()]);
                is_correct_factor = false;
            }
            ++cur_token_idx;
            ++cur_token_idx;
            
            /* ���� offset */
            bool is_offset_imm = false;
            int offset_val = -2333;
            std::string offset_temp_var = "[INVALID offset symbol !$%$??]";
            SymbolType offset_type = UNKNOWN_SYMBOL_TYPE;
            
            if (!parse_expression(function_name, is_offset_imm, offset_val, offset_temp_var, offset_type)) {
                error_message("Invalid offset expression when accessing the array " + identifier_name);
                is_correct_factor = false;
            }
            
            /* ���ɶ�������Ԫʽ */
            res_temp_var = create_temp_var();
            emit(Quadruple(READ_ARRAY_OP, identifier_name, is_offset_imm ? std::to_string(offset_val) : offset_temp_var,
                           res_temp_var));
            
            /* ] */
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM) {
                error_message("Expected ], instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_factor = false;
            }
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("Got ] but as wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_factor = false;
            }
        }
        ++cur_token_idx;
        return is_correct_factor;
    }
    
    if (tokens[cur_token_idx].get_val_string() == LPARENTHESIS_SYM) { /* ( ���ʽ ) */
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got ( but wrong type: " + symbol_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_factor = false;
        }
        ++cur_token_idx;
        
        if (!parse_expression(function_name, is_res_imm, res_val, res_temp_var, res_type)) {
            error_message("Invalid expression in factor!");
            is_correct_factor = false;
        }
        res_type = INT_SYMBOL_TYPE;
        
        /* ) */
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message(
                    "Expected ) after expression in factor, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_factor = false;
        }
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got ) but wrong type: " + symbol_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_factor = false;
        }
        ++cur_token_idx;
        return is_correct_factor;
    }
    
    /* ������ */
    is_res_imm = true;
    res_val = -2333;
    res_temp_var = "INVALID temp_var because it's int! ";
    res_type = INT_SYMBOL_TYPE;
    if (!parse_integer(res_val)) {
        error_message("Invalid Integer!");
        is_correct_factor = false;
    }
    return is_correct_factor;
}

bool parse_funtion_call(const std::string &function_name) {
    if (!is_function(function_name)) {
        error_message("No/Undefined function passed in function call: " + function_name);
        return false;
    }
    bool is_correct_function_call = true;
    
    /* �������õĺ����� */
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Function call expected identifier as the beginning, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_function_call = false;
    }
    std::string callee = tokens[cur_token_idx].get_val_string();
    ++cur_token_idx;
    
    /* ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( before value parameters in function call, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_function_call = false;
    }
    ++cur_token_idx;
    
    /* ���� */
    if (!parse_value_parameters(function_name, callee)) {
        error_message("Invalid value parameters!");
        is_correct_function_call = false;
    }
    
    /* ) */
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after value parameters in function call, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_function_call = false;
    }
    ++cur_token_idx;
    
    /* ������Ԫʽ */
    emit(Quadruple(CALL_FUNCTION_OP, callee, "", ""));
    return is_correct_function_call;
}

bool parse_value_parameters(const std::string &function_name, const std::string &callee) {
    bool is_correct_value_parameters = true;
    
    std::vector<SymbolType> value_param_types;
    
    /* ֱ�ӵ� } ��, û�в��� */
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        return is_matched_parameter_type(callee, value_param_types);
    }
    
    bool is_param_imm;
    int param_val = 233336666;
    std::string param_temp_var = "233335555";
    SymbolType param_type = UNKNOWN_SYMBOL_TYPE;
    
    /* ����� 1 (2, 3, 4... ) ������, ��һ������Ҫ����һ�� */
    --cur_token_idx;
    do {
        ++cur_token_idx;
        if (!parse_expression(function_name, is_param_imm, param_val, param_temp_var, param_type)) {
            error_message("Invalid expression!");
            is_correct_value_parameters = false;
        }
        value_param_types.push_back(param_type);

//        std::cout << "param_temp_var: " + param_temp_var << std::endl;
        
        emit(Quadruple(PUSH_PARAMETER_OP, is_param_imm ? std::to_string(param_val) : param_temp_var, "", ""));
    } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
             tokens[cur_token_idx].get_output_type() == SEPARATOR &&
             value_param_types.size() < MAX_N_PARAMETERS_LIMIT);
    
    /* ���������ͺ͸����Ƿ���� */
    if (!is_matched_parameter_type(callee, value_param_types)) {
        if (get_function(callee)->parameter_types.size() != value_param_types.size()) { /* �Ǹ������� */
            error_message("Expected " + std::to_string(get_function(callee)->parameter_types.size()) +
                          " parameters in " + callee + ", got instead: " +
                          std::to_string(value_param_types.size()));
        } else { /* �����Ͳ��� */
            int error_param_idx = 0;
            for (; error_param_idx < get_function(callee)->parameter_types.size(); ++error_param_idx) {
                if (get_function(callee)->parameter_types[error_param_idx] !=
                    value_param_types[error_param_idx]) {
                    break;
                }
            }
            error_message("Unmatched value parameters passed in " + callee + "! Expected #" +
                          std::to_string(error_param_idx + 1) + ": " +
                          symbol_type_strs[get_function(callee)->parameter_types[error_param_idx]] +
                          ", got instead: " + symbol_type_strs[value_param_types[error_param_idx]]);
        }
        is_correct_value_parameters = false;
    }
    return is_correct_value_parameters;
}

bool parse_main_function_definition() {
    bool is_correct_main_function_definition = true;
    /* void */
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message(
                "main function should start with void, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got void symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_main_function_definition = false;
    }
    SymbolType return_type = VOID_SYMBOL_TYPE;
    ++cur_token_idx;
    /* main */
    if (tokens[cur_token_idx].get_val_string() != MAIN_SYM) {
        error_message("Expected main, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got main symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_main_function_definition = false;
    }
    std::string function_name = MAIN_SYM;
    insert_function(function_name, return_type);
    ++cur_token_idx;
    /* ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( after main, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    ++cur_token_idx;
    /* ) */
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after main(, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    ++cur_token_idx;
    /* { */
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected {, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    ++cur_token_idx;
    /* �������ڲ� */
    if (!parse_block(MAIN_SYM)) {
        error_message("Invalid block in main function definition!");
        is_correct_main_function_definition = false;
    }
    /* } */
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected } in main function definition, instead got: " + tokens[cur_token_idx].get_val_string());
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
    SymbolType return_type =
            tokens[cur_token_idx].get_val_string() == CHAR_SYM ? CHAR_SYMBOL_TYPE : INT_SYMBOL_TYPE;
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
                "Expected ( in non void function definition, instead got: " +
                tokens[cur_token_idx].get_val_string());
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
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected { in non void function definition, instead got: " +
                tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    
    ++cur_token_idx;
    if (!parse_block(function_name)) {
        error_message("Invalid block in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected } in non void function definition, instead got: " +
                tokens[cur_token_idx].get_val_string());
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
    
    /* ���������� void */
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message(
                "Void functions should start with void, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got void symbol but as wrong type: " +
                      token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_void_function_definition = false;
    }
    SymbolType return_type = VOID_SYMBOL_TYPE;
    ++cur_token_idx;
    
    /* �������� */
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier as name in void function definition, instead got: " +
                      tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    std::string function_name = tokens[cur_token_idx].get_val_string();
    insert_function(function_name, return_type);
    ++cur_token_idx;
    
    /* ���ɺ�������ͷ����Ԫʽ */
    emit(Quadruple(DECLARE_FUNCTION_OP, symbol_type_strs[return_type], function_name, ""));
    
    /* ���� ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected ( in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    ++cur_token_idx;
    
    /* ��������� */
    if (!parse_parameter_list(function_name)) {
        error_message("Invalid parameter list in void function definition!");
        is_correct_void_function_definition = false;
    }
    
    /* ) */
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected ) in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    ++cur_token_idx;
    
    /* { */
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected { in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    ++cur_token_idx;
    
    /* ���뺯���ڲ��� */
    if (!parse_block(function_name)) {
        error_message("Invalid block in void function definition!");
        is_correct_void_function_definition = false;
    }
    
    /* } */
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message(
                "Expected } in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
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
    if (tokens[cur_token_idx].get_val_string() == CONST_SYM) { /* �ǳ������� */
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
        /* �Ǳ��� �� �з���ֵ�������� */
        // Ӧ�� "(" �� "[" �� ";" �� ","
        if ((tokens[cur_token_idx + 2].get_val_string() == LBRACKET_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == SEMICOLON_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM) &&
            tokens[cur_token_idx + 2].get_output_type() == SEPARATOR) { /* �Ǳ������� */
            if (!parse_variable_declarations()) { /* ������������ */
                error_message("Invalid variable declarations!");
                is_correct_program = false;
            }
            std::cout << "-----------------------------------Functions------------------------------" << std::endl;
        } else if (tokens[cur_token_idx + 2].get_val_string() == LPARENTHESIS_SYM &&
                   tokens[cur_token_idx + 2].get_output_type() == SEPARATOR) { /* ���з���ֵ�ĺ��� */
            std::cout << "-----------------------------------Functions------------------------------" << std::endl;
            if (!parse_non_void_function_definition()) { /* ������ǰ���з���ֵ�ĺ��� */
                error_message("Invalid non-void function!");
                is_correct_program = false;
            }
        } else {
            error_message("Expected ( or [ or , or ; instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_program = false;
        }
    }
    
    /* ����֮������ٰ��� void main() �ĺ��� */
    bool reached_main = false;
    int n_functions = 1;
    while (tokens[cur_token_idx].get_val_string() == INT_SYM ||
           tokens[cur_token_idx].get_val_string() == CHAR_SYM ||
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
    
    emit(Quadruple(EXIT_OP, "", "", ""));
    if (is_correct_program && tokens[cur_token_idx].get_line_num() == line_count) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct program!" << std::endl;
    }
    return is_correct_program;
}
