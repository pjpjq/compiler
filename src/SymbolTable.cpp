//
// Created by monotasker on 2018/11/29.
//

#include "SymbolTable.h"

/* ·ûºÅ±í */
ConstantTable global_constant_table;
VariableTable global_variable_table;
FunctionTable function_table;

void init_symbol_tables() {
    global_constant_table.clear();
    global_variable_table.clear();
    function_table.clear();
}

Symbol *get_non_function_symbol(const std::string &function_name, const std::string &symbol_name) {
    if (!function_name.empty() && !is_function(function_name)) {
        error_message("Invalid function_name passed in: " + function_name);
        return nullptr;
    }
    if (is_parameter(function_name, symbol_name)) {
        return get_parameter(function_name, symbol_name);
    }
    if (is_local_const(function_name, symbol_name)) {
        return get_local_const(function_name, symbol_name);
    }
    if (is_local_variable(function_name, symbol_name)) {
        return get_local_variable(function_name, symbol_name);
    }
    
    if (is_global_const(symbol_name)) {
        return get_global_const(symbol_name);
    }
    if (is_global_variable(symbol_name)) {
        return get_global_variable(symbol_name);
    }
    if (is_function(symbol_name)) {
        error_message("Trying to get a function as non-function symbol: " + symbol_name);
    } else {
        error_message("Trying to get an invalid symbol as non-function symbol: " + symbol_name);
    }
    return nullptr;
}

Symbol::Symbol(const std::string &name, SymbolType symbol_type) : name(name), symbol_type(symbol_type) {}

std::string Symbol::get_name() {
    return name;
}

SymbolType Symbol::get_symbol_type() {
    return symbol_type;
}

int Symbol::get_length() {
    error_message("Shouldn't be calling get_length()");
    return -23333;
}

bool Symbol::is_array() {
    error_message("Shouldn't be calling is_array()");
    return false;
}

int Symbol::get_value() {
    error_message("Shouldn't be calling get_value()");
    return -233333333;
}

VariableSymbol::VariableSymbol(const std::string &name, SymbolType symbol_type, int length) :
        Symbol(name, symbol_type), length(length) {}

int VariableSymbol::get_length() {
    return length;
}

bool VariableSymbol::is_array() {
    return length > 0;
}

ConstantSymbol::ConstantSymbol(const std::string &name, SymbolType symbol_type, int value) :
        Symbol(name, symbol_type), value(value) {}

int ConstantSymbol::get_value() {
    return value;
}

LocalSymbolTable::LocalSymbolTable(const std::string &name, SymbolType symbol_type) :
        Symbol(name, symbol_type) {}


bool is_function(const std::string &function_name) {
    return function_table.count(function_name) > 0;
}

bool insert_function(const std::string &function_name, SymbolType return_type) {
    if (is_global_symbol(function_name)) {
        error_message("Redefined global symbol: " + function_name);
        return false;
    }
    if (return_type != INT_SYMBOL_TYPE && return_type != CHAR_SYMBOL_TYPE && return_type != VOID_SYMBOL_TYPE) {
        error_message("function return type should be int/char/void, got instead" + symbol_type_strs[return_type]);
        return false;
    }
    function_table[function_name] = new LocalSymbolTable(function_name, return_type);
    return true;
}

LocalSymbolTable *get_function(const std::string &function_name) {
    if (!is_function(function_name)) {
        error_message("Accessing undefined function: " + function_name);
        return nullptr;
    }
    return function_table[function_name];
}

bool is_global_const(const std::string &const_name) {
    return global_constant_table.count(const_name) > 0;
}

bool insert_global_const(const std::string &const_name, SymbolType symbol_type, int value) {
    if (is_global_symbol(const_name)) {
        error_message("Redefined global symbol: " + const_name);
        return false;
    }
    if (symbol_type != INT_SYMBOL_TYPE && symbol_type != CHAR_SYMBOL_TYPE) {
        error_message("Global const should be int/char, got instead" + symbol_type_strs[symbol_type]);
        return false;
    }
    global_constant_table[const_name] = new ConstantSymbol(const_name, symbol_type, value);
    return true;
}

ConstantSymbol *get_global_const(const std::string &const_name) {
    if (!is_global_const(const_name)) {
        error_message("Accessing undefined global const: " + const_name);
        return nullptr;
    }
    return global_constant_table[const_name];
}

bool is_global_variable(const std::string &var_name) {
    return global_variable_table.count(var_name) > 0;
}

bool insert_global_variable(const std::string &variable_name, SymbolType symbol_type, int length) {
    if (is_global_symbol(variable_name)) {
        error_message("Redefined global symbol: " + variable_name);
        return false;
    }
    if (symbol_type != INT_SYMBOL_TYPE && symbol_type != CHAR_SYMBOL_TYPE) {
        error_message("Global variable should be int/char, got instead" + symbol_type_strs[symbol_type]);
        return false;
    }
    global_variable_table[variable_name] = new VariableSymbol(variable_name, symbol_type, length);
    return true;
}

VariableSymbol *get_global_variable(const std::string &variable_name) {
    if (!is_global_variable(variable_name)) {
        error_message("Accessing undefined global variable: " + variable_name);
        return nullptr;
    }
    return global_variable_table[variable_name];
}

bool is_global_symbol(const std::string &symbol_name) {
    return is_non_function_global_symbol(symbol_name) || is_function(symbol_name);
}

bool is_non_function_global_symbol(const std::string &symbol_name) {
    return is_global_const(symbol_name) || is_global_variable(symbol_name);
}


bool is_parameter(const std::string &function_name, const std::string &parameter_name) {
    return is_function(function_name) && get_function(function_name)->parameter_table.count(parameter_name) > 0;
}

bool insert_parameter(const std::string &function_name, const std::string &parameter_name, SymbolType symbol_type) {
    if (is_parameter(function_name, parameter_name)) {
        error_message("Redefined parameter: " + parameter_name + " in " + function_name);
        return false;
    }
    if (symbol_type != INT_SYMBOL_TYPE && symbol_type != CHAR_SYMBOL_TYPE) {
        error_message("Function parameter should be int/char, got instead" + symbol_type_strs[symbol_type]);
        return false;
    }
    get_function(function_name)->parameter_table[parameter_name] = new VariableSymbol(parameter_name, symbol_type, 0);
    get_function(function_name)->parameter_types.push_back(symbol_type);
    return true;
}

VariableSymbol *get_parameter(const std::string &function_name, const std::string &parameter_name) {
    if (!is_parameter(function_name, parameter_name)) {
        error_message("Accessing undefined parameter: " + parameter_name + " in " + function_name);
        return nullptr;
    }
    return get_function(function_name)->parameter_table[parameter_name];
}

bool is_local_const(const std::string &function_name, const std::string &const_name) {
    return is_function(function_name) && get_function(function_name)->constant_table.count(const_name) > 0;
}

bool insert_local_const(const std::string &function_name, const std::string &const_name, SymbolType symbol_type,
                        int value) {
    if (is_local_symbol(function_name, const_name)) {
        error_message("Redefined local const: " + const_name + " in " + function_name);
        return false;
    }
    if (symbol_type != INT_SYMBOL_TYPE && symbol_type != CHAR_SYMBOL_TYPE) {
        error_message("Local const should be int/char, got instead" + symbol_type_strs[symbol_type]);
        return false;
    }
    get_function(function_name)->constant_table[const_name] = new ConstantSymbol(const_name, symbol_type, value);
    return true;
}

ConstantSymbol *get_local_const(const std::string &function_name, const std::string &const_name) {
    if (!is_local_const(function_name, const_name)) {
        error_message("Accessing undefined const: " + const_name + " in " + function_name);
        return nullptr;
    }
    return get_function(function_name)->constant_table[const_name];
}

bool is_local_variable(const std::string &function_name, const std::string &var_name) {
    return is_function(function_name) && get_function(function_name)->variable_table.count(var_name) > 0;
}

bool insert_local_variable(const std::string &function_name, const std::string &variable_name, SymbolType symbol_type,
                           int length) {
    if (is_local_symbol(function_name, variable_name)) {
        error_message("Redefined local variable: " + variable_name + " in " + function_name);
        return false;
    }
    if (symbol_type != INT_SYMBOL_TYPE && symbol_type != CHAR_SYMBOL_TYPE) {
        error_message("Local variable should be int/char, got instead" + symbol_type_strs[symbol_type]);
        return false;
    }
    get_function(function_name)->variable_table[variable_name] = new VariableSymbol(variable_name, symbol_type, length);
    return true;
}

VariableSymbol *get_local_variable(const std::string &function_name, const std::string &variable_name) {
    if (!is_local_variable(function_name, variable_name)) {
        error_message("Accessing undefined variable: " + variable_name + " in " + function_name);
        return nullptr;
    }
    return get_function(function_name)->variable_table[variable_name];
}

bool is_local_symbol(const std::string &function_name, const std::string &symbol_name) {
    return is_function(function_name) &&
           (is_parameter(function_name, symbol_name) || is_local_const(function_name, symbol_name) ||
            is_local_variable(function_name, symbol_name));
}

bool is_matched_parameter_type(const std::string &callee, const std::vector<SymbolType> &value_parameters) {
    return is_function(callee) && get_function(callee)->parameter_types == value_parameters;
}

