//
// Created by monotasker on 2018/11/18.
//

#include "Parser.h"

bool parse_unsigned_integer(int &res) {
    bool is_unsigned_integer = true;
    if (tokens[cur_token_idx].get_output_type() != UNSIGNED_INTEGER &&
        tokens[cur_token_idx].get_output_type() != ZERO) {
        error_message("Unsigned int should be >= 0, instead got: " + tokens[cur_token_idx].get_val_string());
        is_unsigned_integer = false;
    }
    res = tokens[cur_token_idx].get_val_int();
    if (res < 0) {
        error_message("Something wrong here: got a negative value for an unsigned integer?? : " + std::to_string(res));
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
        error_message("Signed int begins with +/-, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_integer = false;
    }
    if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("+/- as integer sign but wrong type: " +
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
    /* int/char */
    if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
        error_message("Const definition begins with int/char, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_const_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("int/char in const definition in wrong type: " +
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
        error_message("Const declarations should begin with const, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_const_declarations = false;
    }
    
    int n_const_definitions = 0;
    while (tokens[cur_token_idx].get_val_string() == CONST_SYM && n_const_definitions < MAX_N_VAR_DEFINITIONS) {
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message("const in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_const_declarations = false;
        }
        ++cur_token_idx;
        
        if (!parse_const_definition(function_name)) {
            error_message("Invalid const definition!");
            is_correct_const_declarations = false;
        }
        ++n_const_definitions;
        /* ; */
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
    std::string var_name = "[INVALID var_name!] ";
    SymbolType var_type = UNKNOWN_SYMBOL_TYPE;
    int length = -111112333;
    
    /* ȷ���������� */
    if (tokens[cur_token_idx].get_val_string() != CHAR_SYM && tokens[cur_token_idx].get_val_string() != INT_SYM) {
        error_message("Variable definition begins with char/int, not: " + tokens[cur_token_idx].get_val_string());
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
        
        if (tokens[cur_token_idx].get_val_string() != LBRACKET_SYM) { /* �������� */
            length = 0;
        } else { /* ������ */
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("[ in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_variable_definition = false;
            }
            ++cur_token_idx;
            /* length */
            if (tokens[cur_token_idx].get_output_type() == ZERO) {
                error_message("Array size should be larger than 0!");
                is_correct_variable_definition = false;
            }
            if (!parse_unsigned_integer(length)) {
                error_message("Expected >0 int for array size, not: " + tokens[cur_token_idx].get_val_string());
                is_correct_variable_definition = false;
            }
            /* ] */
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM) {
                error_message("Expected ], instead got: " + tokens[cur_token_idx].get_val_string());
                is_correct_variable_definition = false;
            }
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("] in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_variable_definition = false;
            }
            ++cur_token_idx;
        }
        /* ������� */
        if (function_name.empty()) {
            is_correct_variable_definition &= insert_global_variable(var_name, var_type, length);
        } else {
            is_correct_variable_definition &= insert_local_variable(function_name, var_name, var_type, length);
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
            error_message("; in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_variable_declarations = false;
        }
        ++cur_token_idx;
        
        if ((tokens[cur_token_idx].get_val_string() == CHAR_SYM || tokens[cur_token_idx].get_val_string() == INT_SYM) &&
            tokens[cur_token_idx + 1].get_output_type() == IDENTIFIER &&
            (tokens[cur_token_idx + 2].get_val_string() == SEMICOLON_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == LBRACKET_SYM ||
             tokens[cur_token_idx + 2].get_val_string() == COMMA_SYM)) { /* ��һ�����Ǳ������� */
            if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
                error_message("int/char in variable declarations but wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_variable_declarations = false;
            }
            if (tokens[cur_token_idx + 2].get_output_type() != SEPARATOR) {
                error_message("Got ; or , or [ in variable declarations but wrong type: " +
                              token_output_type_strs[tokens[cur_token_idx + 2].get_output_type()]);
                is_correct_variable_declarations = false;
            }
        } else { /* ��ɱ������� */
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
    std::string parameter_name = "[INVALID parameter_name]";
    SymbolType parameter_type = UNKNOWN_SYMBOL_TYPE;
    /* �в��� */
    --cur_token_idx; // ��һ�ν�ѭ��Ҫ����һ��
    do {
        ++cur_token_idx;
        /* �������� */
        if (tokens[cur_token_idx].get_val_string() != INT_SYM && tokens[cur_token_idx].get_val_string() != CHAR_SYM) {
            error_message("Expected int/char in parameter list, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_parameter_list = false;
        }
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message("int/char in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
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
            error_message("Expected identifier in parameter list, not: " + tokens[cur_token_idx].get_val_string());
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
    if (!is_function(function_name)) {
        error_message("Undefined/No function name passed in block: " + function_name);
        return false;
    }
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
    
    bool has_return_statement = false;
    int n_statements = 0;
    while (tokens[cur_token_idx].get_val_string() != RBRACE_SYM && n_statements < MAX_N_STATEMENTS_LIMIT) {
        if (!parse_statements(function_name, has_return_statement)) {
            error_message("Invalid statements!");
            is_correct_block = false;
            break;
        }
        /* } */
        if (tokens[cur_token_idx].get_val_string() == RBRACE_SYM &&
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got } in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_block = false;
        }
        ++n_statements;
    }
    /* ��� return */
    if ((get_function(function_name)->get_symbol_type() == INT_SYMBOL_TYPE ||
         get_function(function_name)->get_symbol_type() == CHAR_SYMBOL_TYPE) && !has_return_statement) {
        error_message("No return statement in non-void function: " + function_name);
        is_correct_block = false;
    }
    if (is_correct_block) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct block!" << std::endl;
    }
    return is_correct_block;
}

bool parse_statements(const std::string &function_name, bool &has_return_statement) {
    if (!is_function(function_name)) {
        error_message("Undefined/No function name passed in statements: " + function_name);
        return false;
    }
    bool is_correct_statements = true;
    /* һ���ֺŵ���� */
    if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct ; statement!" << std::endl;
        return is_correct_statements;
    }
    /* return ��� */
    if (tokens[cur_token_idx].get_val_string() == RETURN_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        has_return_statement = true;
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == SEMICOLON_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* return; ��� */
            if (get_function(function_name)->get_symbol_type() != VOID_SYMBOL_TYPE) { /* ��鷵������ */
                error_message("Non-void function got \"return ;\": " + function_name);
                is_correct_statements = false;
            }
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
        /* expression */
        bool is_return_val_imm;
        int return_val = -23333;
        std::string temp_var_to_return = "[INVALID temp_var_to_return !!!]";
        SymbolType return_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_return_val_imm, return_val, temp_var_to_return, return_type)) {
            error_message("Invalid expression!");
            is_correct_statements = false;
        }
        if (get_function(function_name)->get_symbol_type() != return_type) { /* ��鷵������ */
            error_message(
                    "Function expects return type " + symbol_type_strs[get_function(function_name)->get_symbol_type()] +
                    " , not " + symbol_type_strs[return_type]);
            is_correct_statements = false;
        }
        std::string return_str = temp_var_to_return;
        if (is_return_val_imm) {
            return_str = return_type == INT_SYMBOL_TYPE ? std::to_string(return_val) : std::string{(char) return_val};
        }
        emit(Quadruple(RETURN_OP, return_str, "", ""));
        /* ) */
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(") is expected in return statement, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("; is expected after return statement, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct return statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* scanf */
    if (tokens[cur_token_idx].get_val_string() == SCANF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_scanf_statement(function_name)) {
            error_message("Invalid scanf statement!");
            is_correct_statements = false;
        }
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("; is expected after scanf statement, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct scanf statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* printf */
    if (tokens[cur_token_idx].get_val_string() == PRINTF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_printf_statement(function_name)) {
            error_message("Invalid printf statement! Instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("; is expected after printf statement, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct printf statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* �Ǹ�ֵ���, ��������Ԫ��, Ҳ���ܵ������� */
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER &&
        tokens[cur_token_idx + 1].get_val_string() != LPARENTHESIS_SYM) {
        Symbol *var = get_non_function_symbol(function_name, tokens[cur_token_idx].get_val_string());
        if (!var) {
            return false;
        } else if (typeid(*var).name() == typeid(ConstantSymbol).name()) {
            error_message("Cannot assign to const: " + var->get_name());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        std::string offset_str = "[INVALID offset_str]";
        if (tokens[cur_token_idx].get_val_string() == LBRACKET_SYM) { /* ������Ԫ�ظ�ֵ */
            if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("Got [ but in wrong type: " + symbol_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_statements = false;
            }
            if (!var->is_array()) {
                error_message("Use [ for single variable as assignment left: " + var->get_name());
            }
            ++cur_token_idx;
            /* ���� offset */
            bool is_offset_imm = false;
            int offset_val = -2333;
            std::string offset_temp_var = "[INVALID offset val]";
            SymbolType offset_type = UNKNOWN_SYMBOL_TYPE;
            if (!parse_expression(function_name, is_offset_imm, offset_val, offset_temp_var, offset_type)) {
                error_message("Invalid expression as offset!");
                is_correct_statements = false;
            }
            offset_str = is_offset_imm ? std::to_string(offset_val) : offset_temp_var;
            /* ] */
            if (tokens[cur_token_idx].get_val_string() != RBRACKET_SYM ||
                tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("] is expected in assignment statement, not: " + tokens[cur_token_idx].get_val_string());
                is_correct_statements = false;
            }
            ++cur_token_idx;
        }
        /* = */
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM) {
            error_message("= is expected in assignment statement, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        if (tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Got = but in wrong type: " + symbol_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_statements = false;
        }
        ++cur_token_idx;
        /* assignment right */
        bool is_right_imm;
        int right_val = -23334444;
        std::string right_temp_var = "[INVALID right_temp_var]";
        SymbolType right_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_right_imm, right_val, right_temp_var, right_type)) {
            error_message("Invalid expression in assignment right part!");
            is_correct_statements = false;
        }
        std::string right_str = right_temp_var;
        if (is_right_imm) {
            right_str = right_type == CHAR_SYMBOL_TYPE ? std::string{(char) right_val} : std::to_string(right_val);
        }
        /* ��������Ƿ���ͬ */
        if (var->get_symbol_type() != right_type) {
            error_message("Different type assignment, left: " + symbol_type_strs[var->get_symbol_type()] + ", right: " +
                          symbol_type_strs[right_type]);
            is_correct_statements = false;
        }
        /* ���ɸ�ֵ��Ԫʽ */
        if (var->is_array()) {
            emit(Quadruple(WRITE_ARRAY_OP, var->get_name(), offset_str, right_str));
        } else {
            emit(Quadruple(ASSIGN_OP, var->get_name(), right_str, ""));
        }
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("; is expected after assignment statement, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct assignment statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* �Ǻ�������, ��Ԫʽ�� parse_function_call ������, ���ﲻ��д */
    if (tokens[cur_token_idx].get_output_type() == IDENTIFIER &&
        tokens[cur_token_idx + 1].get_val_string() == LPARENTHESIS_SYM) {
        if (!parse_funtion_call(function_name)) {
            error_message("Invalid function call in function-call statement!");
            is_correct_statements = false;
        }
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("; is expected after function call, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_statements = false;
        }
        ++cur_token_idx;
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct function call statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* if */
    if (tokens[cur_token_idx].get_val_string() == IF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_conditional_statement(function_name, has_return_statement)) {
            error_message("Invalid conditional statement!");
            is_correct_statements = false;
        }
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct if statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* do while / for */
    if ((tokens[cur_token_idx].get_val_string() == DO_SYM || tokens[cur_token_idx].get_val_string() == FOR_SYM) &&
        tokens[cur_token_idx].get_output_type() == KEYWORD) {
        if (!parse_loop_statement(function_name, has_return_statement)) {
            error_message("Invalid loop statement!");
            is_correct_statements = false;
        }
        if (is_correct_statements) {
            std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct loop statement!"
                      << std::endl;
        }
        return is_correct_statements;
    }
    /* compound statements: {...} */
    if (tokens[cur_token_idx].get_val_string() == LBRACE_SYM &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        int n_inner_statements = 0;
        while (tokens[cur_token_idx].get_val_string() != RBRACE_SYM && n_inner_statements < MAX_N_INNER_STATEMENTS) {
            if (tokens[cur_token_idx].get_val_string() == RBRACE_SYM &&
                tokens[cur_token_idx].get_output_type() != SEPARATOR) {
                error_message("} in wrong type " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
                is_correct_statements = false;
                break;
            }
            /* inner statements */
            ++n_inner_statements;
            if (!parse_statements(function_name, has_return_statement)) {
                error_message("Invalid inner statements (num " + std::to_string(n_inner_statements));
                is_correct_statements = false;
            }
        }
        /* ȷ�� } */
        if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected } after compound statement, not: " + tokens[cur_token_idx].get_val_string());
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

bool parse_loop_statement(const std::string &function_name, bool &has_return_statement) {
    bool is_correct_loop_statement = true;
    if (tokens[cur_token_idx].get_val_string() != DO_SYM && tokens[cur_token_idx].get_val_string() != FOR_SYM) {
        error_message("Expected do/for to enter a loop statement, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_loop_statement = false;
    }
    /* �������� */
    bool is_condition_imm = false;
    int condition_val = -23323233;
    std::string condition_temp_var = "[INVALID condition_temp_var]";
    SymbolType condition_type = UNKNOWN_SYMBOL_TYPE;
    /* �� do while ѭ�� */
    if (tokens[cur_token_idx].get_val_string() == DO_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        /* ��һ�� enter do while label */
        std::string enter_do_while_label = create_label(function_name, "enter_do_while");
        emit(Quadruple(LABEL_OP, enter_do_while_label, "", ""));
        ++cur_token_idx;
        /* ѭ���ڲ���� */
        if (!parse_statements(function_name, has_return_statement)) {
            error_message("Invalid statements in do-while loop!");
            is_correct_loop_statement = false;
        }
        /* while */
        if (tokens[cur_token_idx].get_val_string() != WHILE_SYM || tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message("Expected while in do-while loop, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* ( */
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ( after while, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* condition */
        if (!parse_condition(function_name, is_condition_imm, condition_val, condition_temp_var, condition_type)) {
            error_message("Invalid condition in while");
            is_correct_loop_statement = false;
        }
        /* ������ת��Ԫʽ */
        if (is_condition_imm) {
            if (condition_val > 0) { /* while ����������, j �� enter */
                emit(Quadruple(JUMP_OP, "", "", enter_do_while_label));
            }
        } else { /* ��֪, ��Ϊ��ʱ�� �� bnz �� enter */
            emit(Quadruple(BNZ_OP, "", "", enter_do_while_label));
        }
        /* ) */
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
        /* Ҫ�õ��� label �� */
        std::string enter_for_label = create_label(function_name, "enter_for_loop");
        std::string done_for_label = create_label(function_name, "done_for_loop");
        /* ( */
        if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ( after for, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* ��ʶ�� */
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for-loop init part, not " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        Symbol *init_var = get_non_function_symbol(function_name, tokens[cur_token_idx].get_val_string());
        if (!init_var) {
            return false;
        } else if (typeid(*init_var).name() == typeid(ConstantSymbol).name()) {
            error_message("Cannot assign to const in for-loop init: " + init_var->get_name());
            is_correct_loop_statement = false;
        } else if (init_var->is_array()) {
            error_message("Cannot assign to array in for-loop init: " + init_var->get_name());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* = */
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected = in for-loop init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* init_right */
        bool is_init_right_imm = false;
        int init_right_val = -23333;
        std::string init_right_temp_var = "[INVALID init_right_temp_var]";
        SymbolType init_right_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_init_right_imm, init_right_val, init_right_temp_var, init_right_type)) {
            error_message("Invalid expression in for-loop init part");
            is_correct_loop_statement = false;
        }
        std::string init_right_str = init_right_temp_var;
        if (is_init_right_imm) {
            init_right_str = init_right_type == CHAR_SYMBOL_TYPE ? std::string{(char) init_right_val} : std::to_string(
                    init_right_val);
        }
        /* ��������Ƿ���ͬ */
        if (init_var->get_symbol_type() != init_right_type) {
            error_message("Different type assignment in for-loop init, left: " +
                          symbol_type_strs[init_var->get_symbol_type()] + ", right: " +
                          symbol_type_strs[init_right_type]);
            is_correct_loop_statement = false;
        }
        /* ���ɸ�ֵ��� */
        emit(Quadruple(ASSIGN_OP, init_var->get_name(), init_right_str, ""));
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected ; in for loop after init part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* ���� enter_for label ��Ԫʽ */
        emit(Quadruple(LABEL_OP, enter_for_label, "", ""));
        /* condition */
        if (!parse_condition(function_name, is_condition_imm, condition_val, condition_temp_var, condition_type)) {
            error_message("Invalid condition in for-loop!");
            is_correct_loop_statement = false;
        }
        /* ������ת��Ԫʽ */
        if (is_condition_imm) {
            if (condition_val == 0) { /* for �������, j ������ */
                emit(Quadruple(JUMP_OP, "", "", done_for_label));
            }
        } else { /* ������֪, ����ʱ�������� */
            emit(Quadruple(BZ_OP, condition_temp_var, "", done_for_label));
        }
        /* ; */
        if (tokens[cur_token_idx].get_val_string() != SEMICOLON_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ; in for-loop after init part, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* updating left identifier */
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for-loop updating left, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        Symbol *updating_left_var = get_non_function_symbol(function_name, tokens[cur_token_idx].get_val_string());
        if (!updating_left_var) {
            return false;
        } else if (typeid(*updating_left_var).name() == typeid(ConstantSymbol).name()) {
            error_message("Cannot assign to const in for-loop update: " + updating_left_var->get_name());
            is_correct_loop_statement = false;
        } else if (updating_left_var->is_array()) {
            error_message("Cannot assign to array in for-loop update: " + updating_left_var->get_name());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* = */
        if (tokens[cur_token_idx].get_val_string() != ASSIGN_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message(
                    "Expected = in for-loop updating part, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* updating right identifier */
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier in for-loop updating right, instead got: " +
                          tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        Symbol *updating_right_var = get_non_function_symbol(function_name, tokens[cur_token_idx].get_val_string());
        if (!updating_right_var) {
            return false;
        } else if (updating_right_var->is_array()) {
            error_message("Cannot assign from array in for-loop update: " + updating_right_var->get_name());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* +/- */
        if ((tokens[cur_token_idx].get_val_string() != MINUS_SYM &&
             tokens[cur_token_idx].get_val_string() != PLUS_SYM) ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected +/- in for-loop updating part, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        std::string op = tokens[cur_token_idx].get_val_string();
        ++cur_token_idx;
        /* updating_size */
        int updating_size = -555555;
        if (!parse_unsigned_integer(updating_size)) {
            error_message("Expected >=0 int as for-loop updating size, not " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        /* ��������Ƿ���ͬ, ������Ǽ�������ǲ��� INT */
        if (updating_left_var->get_symbol_type() != INT_SYMBOL_TYPE) {
            error_message("For-loop updating left identifier should be int, not: " +
                          symbol_type_strs[updating_left_var->get_symbol_type()]);
            is_correct_loop_statement = false;
        }
        /* ) */
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
            tokens[cur_token_idx].get_output_type() != SEPARATOR) {
            error_message("Expected ) after for-loop condition, not: " + tokens[cur_token_idx].get_val_string());
            is_correct_loop_statement = false;
        }
        ++cur_token_idx;
        /* ѭ���ڲ���� */
        if (!parse_statements(function_name, has_return_statement)) {
            error_message("Invalid statements in for-loop!");
            is_correct_loop_statement = false;
        }
        /* ���� +/- ��Ԫʽ, ���ﲻ���� updating_right_var �ǲ��ǳ����� */
        emit(Quadruple(op, "#" + updating_right_var->get_name(), std::to_string(updating_size),
                       "#" + updating_left_var->get_name()));
        /* ���� j �� enter_for ��Ԫʽ */
        emit(Quadruple(JUMP_OP, "", "", enter_for_label));
        /* ��һ�� done_for label */
        emit(Quadruple(LABEL_OP, done_for_label, "", ""));
        return is_correct_loop_statement;
    }
    return is_correct_loop_statement;
}

bool parse_conditional_statement(const std::string &function_name, bool &has_return_statement) {
    bool is_correct_conditional_statement = true;
    /* if */
    if (tokens[cur_token_idx].get_val_string() != IF_SYM || tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Expected if in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    /* ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    /* condition */
    bool is_condition_imm = true;
    int condition_val = -3333332;
    std::string condition_temp_var = "[INVALID condition temp var]";
    SymbolType condition_type = UNKNOWN_SYMBOL_TYPE;
    if (!parse_condition(function_name, is_condition_imm, condition_val, condition_temp_var, condition_type)) {
        error_message("Invalid condition!");
        is_correct_conditional_statement = false;
    }
    /* �жϽ�� */
    std::string else_label = create_label(function_name, "else");
    std::string done_if_label = create_label(function_name, "done_if");
    if (is_condition_imm) {
        if (condition_val == 0) { /* ��Ϊ��, j �� else label */
            emit(Quadruple(JUMP_OP, "", "", else_label));
        }
    } else { /* Ϊ��ʱ��, �� bz �� else label */
        emit(Quadruple(BZ_OP, condition_temp_var, "", else_label));
    }
    /* ) */
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) in conditional statement, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_conditional_statement = false;
    }
    ++cur_token_idx;
    /* if true ����� */
    if (!parse_statements(function_name, has_return_statement)) {
        error_message("Invalid statement after if!");
        is_correct_conditional_statement = false;
    }
    /* ֮�� j done_if_label */
    emit(Quadruple(JUMP_OP, "", "", done_if_label));
    /* ��һ�� else label, ��Ȼ����û�� else */
    emit(Quadruple(LABEL_OP, else_label, "", ""));
    /* ��ѡ�� else */
    if (tokens[cur_token_idx].get_val_string() == ELSE_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        ++cur_token_idx;
        /* if false ����� */
        if (!parse_statements(function_name, has_return_statement)) {
            error_message("Invalid statement after else!");
            is_correct_conditional_statement = false;
        }
    }
    /* ��һ�� done_if label */
    emit(Quadruple(LABEL_OP, done_if_label, "", ""));
    return is_correct_conditional_statement;
}

bool parse_condition(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                     SymbolType &res_type) {
    bool is_correct_condition = true;
    /* ��� */
    if (!parse_expression(function_name, is_res_imm, res_val, res_temp_var, res_type)) {
        error_message("Invalid expression as condition left!");
        is_correct_condition = false;
    }
    /* ��ѡ�ıȽ������ */
    std::string op = tokens[cur_token_idx].get_val_string();
    if ((op == EQ_SYM || op == NE_SYM || op == LT_SYM || op == GT_SYM || op == LE_SYM || op == GE_SYM) &&
        tokens[cur_token_idx].get_output_type() == SEPARATOR) {
        ++cur_token_idx;
        /* �бȽ�����������ұ� */
        bool is_condition_right_imm = false;
        int condition_right_val = -1145;
        std::string condition_right_temp_var = "[INVALID right temp var]";
        SymbolType condition_right_type = UNKNOWN_SYMBOL_TYPE;
        if (!parse_expression(function_name, is_condition_right_imm, condition_right_val, condition_right_temp_var,
                              condition_right_type)) {
            error_message("Invalid expression as condition right!");
            is_correct_condition = false;
        }
        /* ������ */
        if (is_res_imm) {
            if (is_condition_right_imm) {/* ���߶����� */
                res_val = op == EQ_SYM ? res_val == condition_right_val :
                          op == NE_SYM ? res_val != condition_right_val :
                          op == LT_SYM ? res_val < condition_right_val :
                          op == GT_SYM ? res_val > condition_right_val :
                          op == LE_SYM ? res_val <= condition_right_val :
                          op == GE_SYM ? res_val >= condition_right_val : -23333;
                std::cout << "[Line " << tokens[cur_token_idx].get_line_num() << ": always " << res_val
                          << " condition!]"
                          << std::endl; // debug
            } else { /* ��֪�Ҳ�֪ */
                emit(Quadruple(op, std::to_string(res_val), condition_right_temp_var, res_temp_var));
            }
        } else {
            if (is_condition_right_imm) { /* ��֪��֪ */
                emit(Quadruple(op, res_temp_var, std::to_string(condition_right_val), res_temp_var));
            } else { /* ����֪ */
                emit(Quadruple(op, res_temp_var, condition_right_temp_var, res_temp_var));
            }
        }
        is_res_imm &= is_condition_right_imm;
    }
    return is_correct_condition;
}

bool parse_printf_statement(const std::string &function_name) {
    bool is_correct_printf_statement = true;
    /* printf */
    if (tokens[cur_token_idx].get_val_string() != PRINTF_SYM || tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Expected printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    /* ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( after printf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_printf_statement = false;
    }
    ++cur_token_idx;
    /* Ҫ����ı��ʽ */
    bool is_printee_imm = false;
    int printee_val = 88888;
    std::string printee_temp_var = "[INVALID printee_temp_var]";
    SymbolType printee_type = UNKNOWN_SYMBOL_TYPE;
    if (tokens[cur_token_idx].get_output_type() == STRING) { /* ���ַ��� */
        emit(Quadruple(PRINTF_OP, PRINTF_STRING_TYPE_SYM, tokens[cur_token_idx].get_val_string(), ""));
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
            tokens[cur_token_idx].get_output_type() == SEPARATOR) { /* �ַ��������һ�����ʽ */
            ++cur_token_idx;
            if (!parse_expression(function_name, is_printee_imm, printee_val, printee_temp_var, printee_type)) {
                error_message("Invalid expression after string in printf!");
                is_correct_printf_statement = false;
            }
            std::string printee_str = printee_temp_var;
            if (is_printee_imm) {
                printee_str = printee_type == CHAR_SYMBOL_TYPE ? std::string{(char) printee_val} : std::to_string(
                        printee_val);
            }
            emit(Quadruple(PRINTF_OP, symbol_type_strs[printee_type], printee_str, ""));
        }
    } else { /* ֻ�б��ʽ */
        if (!parse_expression(function_name, is_printee_imm, printee_val, printee_temp_var, printee_type)) {
            error_message("Invalid single expression in printf!");
            is_correct_printf_statement = false;
        }
        std::string printee_str = printee_temp_var;
        if (is_printee_imm) {
            printee_str = printee_type == CHAR_SYMBOL_TYPE ? std::string{(char) printee_val} : std::to_string(
                    printee_val);
        }
        emit(Quadruple(PRINTF_OP, symbol_type_strs[printee_type], printee_str, ""));
    }
    /* ) */
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
    /* scanf */
    if (tokens[cur_token_idx].get_val_string() != SCANF_SYM && tokens[cur_token_idx].get_output_type() == KEYWORD) {
        error_message("Expected scanf, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    ++cur_token_idx;
    /* ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected (, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_scanf_statement = false;
    }
    /* ���ٶ���һ������ */
    int n_scanf_vars = 0;
    do {
        ++cur_token_idx;
        if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
            error_message("Expected identifier, instead got: " + tokens[cur_token_idx].get_val_string());
            is_correct_scanf_statement = false;
        }
        Symbol *var = get_non_function_symbol(function_name, tokens[cur_token_idx].get_val_string());
        if (!var) {
            is_correct_scanf_statement = false;
        } else if (typeid(*var).name() == typeid(ConstantSymbol).name()) {
            error_message("Cannot scanf const: " + var->get_name());
            is_correct_scanf_statement = false;
        } else if (var->is_array()) {
            error_message("Cannot scanf array: " + var->get_name());
            is_correct_scanf_statement = false;
        } else { /* ���ɶ�������Ԫʽ */
            emit(Quadruple(SCANF_OP, symbol_type_strs[var->get_symbol_type()], var->get_name(), ""));
        }
        ++cur_token_idx;
        ++n_scanf_vars;
    } while (tokens[cur_token_idx].get_val_string() == COMMA_SYM &&
             tokens[cur_token_idx].get_output_type() == SEPARATOR && n_scanf_vars < MAX_N_SCANF_VARS);
    /* ) */
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
    /* ��Ĭ�Ͻ����ֵ0, ����Ϊ plus */
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
            if (!is_term_imm) { /* ���ֵ�һ������, ��δ������ */
                is_res_imm = false;
                if (res_val == 0) {
                    emit(Quadruple(ASSIGN_OP, res_temp_var, term_temp_var, ""));
                } else {
                    emit(Quadruple(op,
                                   res_type == CHAR_SYMBOL_TYPE ? std::string{(char) res_val} : std::to_string(res_val),
                                   term_temp_var, res_temp_var));
                }
            } else { /* ������Ӽ� */
                res_val = op == PLUS_SYM ? res_val + term_val : op == MINUS_SYM ? res_val - term_val : -2333333;
            }
        } else { /* ֮ǰ����б��� */
            res_type = INT_SYMBOL_TYPE;
            std::string right = term_temp_var;
            if (is_term_imm) {
                right = term_type == CHAR_SYMBOL_TYPE ? std::string{(char) term_val} : std::to_string(term_val);
            }
            emit(Quadruple(op, res_temp_var, right, res_temp_var));
        }
        /* ��һ�� op */
        op = tokens[cur_token_idx].get_val_string();
    } while ((tokens[cur_token_idx].get_val_string() == MINUS_SYM || tokens[cur_token_idx].get_val_string() == PLUS_SYM)
             && tokens[cur_token_idx].get_output_type() == SEPARATOR && n_terms < MAX_N_TERMS_PER_EXPRESSION);
    return is_correct_expression;
}

bool parse_term(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                SymbolType &res_type) {
    bool is_correct_term = true;
    /* ��Ĭ�Ͻ����ֵ1, ����Ϊ mul */
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
        /* ��� / 0 */
        if (op == DIV_SYM && is_factor_imm && factor_val == 0) {
            error_message("Divided by 0 is not defined!");
            factor_val = 1;
        }
        // TODO further �����ϲ�
        if (is_res_imm) { /* ֮ǰ�����ӽ�����ǳ��� */
            if (!is_factor_imm) { /* ���ֵ�һ����ʱ���� ��δ������ */
                is_res_imm = false;
                if (res_val == 1) {
                    emit(Quadruple(ASSIGN_OP, res_temp_var, factor_temp_var, ""));
                } else {
                    emit(Quadruple(op,
                                   res_type == CHAR_SYMBOL_TYPE ? std::string{(char) res_val} : std::to_string(res_val),
                                   factor_temp_var, res_temp_var));
                }
            } else { /* �����˳� */
                res_val = op == MUL_SYM ? res_val * factor_val : op == DIV_SYM ? res_val / factor_val : -233666433;
            }
        } else { /* ֮ǰ������Ǳ��� */
            std::string right = factor_temp_var;
            if (is_factor_imm) {
                right = factor_type == CHAR_SYMBOL_TYPE ? std::string{(char) factor_val} : std::to_string(factor_val);
            }
            emit(Quadruple(op, res_temp_var, right, res_temp_var));
        }
        /* ��һ�� op */
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
                error_message("( in wrong type " + token_output_type_strs[tokens[cur_token_idx + 1].get_output_type()]);
                is_correct_factor = false;
            }
            
            if (!parse_funtion_call(function_name)) {
                error_message("Invalid function call!");
                is_correct_factor = false;
            }
            /* ���������ý�� */
            is_res_imm = false;
            res_val = -2333111;
            res_temp_var = create_temp_var();
            res_type = get_function(identifier_name)->get_symbol_type();
            if (res_type == VOID_SYMBOL_TYPE) {
                error_message("Trying to get the return value from void function: " + identifier_name);
                res_type = INT_SYMBOL_TYPE;
                is_correct_factor = false;
            }
            /* ȡ�������÷���ֵ����Ԫʽ */
            emit(Quadruple(ASSIGN_RETURN_VAL_OP, "", "", res_temp_var));
            return is_correct_factor;
        }
        /* �������� */
        Symbol *var = get_non_function_symbol(function_name, identifier_name);
        if (!var) {
            is_correct_factor = false;
        } else if (typeid(*var).name() == typeid(ConstantSymbol).name()) { /* �ǳ��� */
            is_res_imm = true;
            res_val = var->get_value();
            res_temp_var = "[INVALID: got imm, so this temp_var is useless!]";
            res_type = var->get_symbol_type();
            
            ++cur_token_idx;
            return is_correct_factor;
        } else if (typeid(*var).name() == typeid(VariableSymbol).name()) { /* �Ǳ��� */
            is_res_imm = false;
            res_val = -23333;
            res_temp_var = "#" + var->get_name();
            res_type = var->get_symbol_type();
            if (tokens[cur_token_idx + 1].get_val_string() == LBRACKET_SYM) { /* ������ */
                if (var->get_length() <= 0) {
                    error_message("Array " + var->get_name() + " should have len>0, not: " +
                                  std::to_string(var->get_length()));
                    is_correct_factor = false;
                }
            } else {
                if (var->get_length() > 0) {
                    error_message("Single variable " + var->get_name() + " should have length 0, not: " +
                                  std::to_string(var->get_length()));
                    is_correct_factor = false;
                }
            }
        } else {
            error_message("Invalid variable type: " + var->get_name() + " is " + std::string(typeid(var).name()));
            is_correct_factor = false;
        }
        
        if (tokens[cur_token_idx + 1].get_val_string() == LBRACKET_SYM) { /* ������ */
            if (tokens[cur_token_idx + 1].get_output_type() != SEPARATOR) {
                error_message("[ in wrong type " + token_output_type_strs[tokens[cur_token_idx + 1].get_output_type()]);
                is_correct_factor = false;
            }
            cur_token_idx += 2;
            /* ���� offset */
            bool is_offset_imm = false;
            int offset_val = -2333;
            std::string offset_temp_var = "[INVALID offset temp_var $]";
            SymbolType offset_type = UNKNOWN_SYMBOL_TYPE;
            if (!parse_expression(function_name, is_offset_imm, offset_val, offset_temp_var, offset_type)) {
                error_message("Invalid offset expression when accessing array " + identifier_name);
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
                error_message("] in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
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
        /* expression */
        if (!parse_expression(function_name, is_res_imm, res_val, res_temp_var, res_type)) {
            error_message("Invalid expression in factor!");
            is_correct_factor = false;
        }
        res_type = INT_SYMBOL_TYPE;
        /* ) */
        if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM) {
            error_message("Expected ) after expression in factor, not: " + tokens[cur_token_idx].get_val_string());
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
    res_temp_var = "[INVALID temp_var because it's int! $]";
    res_type = INT_SYMBOL_TYPE;
    if (!parse_integer(res_val)) {
        error_message("Invalid Integer as factor!");
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
        error_message("Expected ) after value params in function call, not: " + tokens[cur_token_idx].get_val_string());
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
    if (tokens[cur_token_idx].get_val_string() == RPARENTHESIS_SYM) {
        return is_matched_parameter_type(callee, value_param_types);
    }
    
    bool is_param_imm;
    int param_val = 233336666;
    std::string param_temp_var = "[INVALID param_temp_var]";
    SymbolType param_type = UNKNOWN_SYMBOL_TYPE;
    
    /* ����� 1 (2, 3, 4... ) ������, ��һ������Ҫ����һ�� */
    --cur_token_idx;
    do {
        ++cur_token_idx;
        if (!parse_expression(function_name, is_param_imm, param_val, param_temp_var, param_type)) {
            error_message("Invalid expression as value params!");
            is_correct_value_parameters = false;
        }
        value_param_types.push_back(param_type);
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
            for (int i = 0; i < get_function(callee)->parameter_types.size(); ++i) {
                if (get_function(callee)->parameter_types[i] != value_param_types[i]) {
                    error_message("Unmatched value params passed in " + callee + ", expected #" + std::to_string(i + 1)
                                  + ": " + symbol_type_strs[get_function(callee)->parameter_types[i]] + ", not: " +
                                  symbol_type_strs[value_param_types[i]]);
                }
            }
        }
        is_correct_value_parameters = false;
    }
    return is_correct_value_parameters;
}

bool parse_main_function_definition() {
    bool is_correct_main_function_definition = true;
    /* void */
    if (tokens[cur_token_idx].get_val_string() != VOID_SYM) {
        error_message("main function starts with void, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_main_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got void in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
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
        error_message("Got main in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_main_function_definition = false;
    }
    /* main ������� */
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
        error_message("Expected } in main function definition, not: " + tokens[cur_token_idx].get_val_string());
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
        error_message("Non void functions starts with int/char, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got int/char in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_non_void_function_definiton = false;
    }
    SymbolType return_type =
            tokens[cur_token_idx].get_val_string() == CHAR_SYM ? CHAR_SYMBOL_TYPE : INT_SYMBOL_TYPE;
    ++cur_token_idx;
    /* identifier */
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier in function definition, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    std::string function_name = tokens[cur_token_idx].get_val_string();
    /* �������, ������Ԫʽ */
    insert_function(function_name, return_type);
    emit(Quadruple(DECLARE_FUNCTION_OP, symbol_type_strs[return_type], function_name, ""));
    ++cur_token_idx;
    /* ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( in non void function definition, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    ++cur_token_idx;
    /* ������ */
    if (!parse_parameter_list(function_name)) {
        error_message("Invalid parameter list in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    /* ) */
    if (tokens[cur_token_idx].get_val_string() != RPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ) after params in function definition, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    ++cur_token_idx;
    /* { */
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected { in non void function definition, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_non_void_function_definiton = false;
    }
    ++cur_token_idx;
    /* �ڲ� */
    if (!parse_block(function_name)) {
        error_message("Invalid block in non void function definition!");
        is_correct_non_void_function_definiton = false;
    }
    /* } */
    if (tokens[cur_token_idx].get_val_string() != RBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected } in non void function definition, not: " + tokens[cur_token_idx].get_val_string());
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
        error_message("Void functions starts with void, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
        error_message("Got void in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
        is_correct_void_function_definition = false;
    }
    SymbolType return_type = VOID_SYMBOL_TYPE;
    ++cur_token_idx;
    /* �������� */
    if (tokens[cur_token_idx].get_output_type() != IDENTIFIER) {
        error_message("Expected identifier in function definition, not: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    std::string function_name = tokens[cur_token_idx].get_val_string();
    /* ������� ���ɺ�������ͷ����Ԫʽ */
    insert_function(function_name, return_type);
    emit(Quadruple(DECLARE_FUNCTION_OP, symbol_type_strs[return_type], function_name, ""));
    ++cur_token_idx;
    /* ���� ( */
    if (tokens[cur_token_idx].get_val_string() != LPARENTHESIS_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected ( in void function definition, not: " + tokens[cur_token_idx].get_val_string());
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
        error_message("Expected ) in void function definition, instead got: " + tokens[cur_token_idx].get_val_string());
        is_correct_void_function_definition = false;
    }
    ++cur_token_idx;
    /* { */
    if (tokens[cur_token_idx].get_val_string() != LBRACE_SYM ||
        tokens[cur_token_idx].get_output_type() != SEPARATOR) {
        error_message("Expected { in  function definition, instead got: " + tokens[cur_token_idx].get_val_string());
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
    if (tokens[cur_token_idx].get_val_string() == CONST_SYM) { /* �ǳ������� */
        if (tokens[cur_token_idx].get_output_type() != KEYWORD) {
            error_message("const in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
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
            error_message(
                    "int/char/void in wrong type: " + token_output_type_strs[tokens[cur_token_idx].get_output_type()]);
            is_correct_program = false;
        }
        if (tokens[cur_token_idx + 1].get_val_string() == MAIN_SYM) {
            if (tokens[cur_token_idx + 1].get_output_type() != KEYWORD) {
                error_message(
                        "main in wrong type " + token_output_type_strs[tokens[cur_token_idx + 1].get_output_type()]);
                is_correct_program = false;
            }
            reached_main = true;
            parse_main_function_definition();
        } else if (tokens[cur_token_idx + 1].get_output_type() == IDENTIFIER &&
                   tokens[cur_token_idx + 2].get_val_string() == LPARENTHESIS_SYM) {
            if (tokens[cur_token_idx + 2].get_output_type() != SEPARATOR) {
                error_message("} in wrong type " + token_output_type_strs[tokens[cur_token_idx + 2].get_output_type()]);
                is_correct_program = false;
            }
            if (reached_main) {
                error_message("Last function should be main, got " + tokens[cur_token_idx + 1].get_val_string() +
                              " after main!");
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
    /* �����˳���Ԫʽ */
    emit(Quadruple(EXIT_OP, "", "", ""));
    if (is_correct_program && tokens[cur_token_idx].get_line_num() == line_count) {
        std::cout << "Line " << tokens[cur_token_idx].get_line_num() << ": finish correct program!" << std::endl;
    }
    return is_correct_program;
}
