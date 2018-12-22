//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <vector>

#include "utils.h"

class Symbol;

class VariableSymbol;

class ConstantSymbol;

class LocalSymbolTable;

typedef std::unordered_map<std::string, ConstantSymbol *> ConstantTable;
typedef std::unordered_map<std::string, VariableSymbol *> VariableTable;
typedef std::unordered_map<std::string, LocalSymbolTable *> FunctionTable;

extern ConstantTable global_constant_table;
extern VariableTable global_variable_table;
extern FunctionTable function_table;

/* SymbolType 枚举类型与 symbol_type_strs 一一对应 */
enum SymbolType {
    UNKNOWN_SYMBOL_TYPE, INT_SYMBOL_TYPE, CHAR_SYMBOL_TYPE, VOID_SYMBOL_TYPE,
};

extern std::string symbol_type_strs[];

class Symbol {
public:
    Symbol(const std::string &name, SymbolType symbol_type);
    
    std::string get_name();
    
    SymbolType get_symbol_type();
    
    virtual int get_length();
    
    virtual bool is_array();
    
    virtual int get_value();

private:
    std::string name;
    SymbolType symbol_type = UNKNOWN_SYMBOL_TYPE;
};

class VariableSymbol : public Symbol {
public:
    VariableSymbol(const std::string &name, SymbolType symbol_type, int length);
    
    virtual int get_length() override;
    
    virtual bool is_array() override;

private:
    int length;
};

class ConstantSymbol : public Symbol {
public:
    ConstantSymbol(const std::string &name, SymbolType symbol_type, int value);
    
    virtual int get_value() override;

private:
    int value;
};

class LocalSymbolTable : public Symbol {
public:
    LocalSymbolTable(const std::string &name, SymbolType symbol_type);
    
    VariableTable parameter_table;
    ConstantTable constant_table;
    VariableTable variable_table;
    
    std::vector<SymbolType> parameter_types;
    
//    int get_n_words();

private:
};
/* -----------------------------------------------------API --------------------------------------------------*/

/**
 * 编译开始时清空各符号表
 */
void init_symbol_tables();

/**
 * 取一个变量/常量 (不是函数), 先查局部, 再查全局
 * @param function_name: local scope
 * @param symbol_name: symbol to get
 * @return  查到的符号, nullptr 就是没有
 */
Symbol *get_non_function_symbol(const std::string &function_name, const std::string &symbol_name);

/* --------------------------------------------------Global------------------------------------------------------------*/

bool is_function(const std::string &function_name);

bool insert_function(const std::string &function_name, SymbolType return_type);

LocalSymbolTable *get_function(const std::string &function_name);


bool is_global_const(const std::string &const_name);

bool insert_global_const(const std::string &const_name, SymbolType symbol_type, int value);

ConstantSymbol *get_global_const(const std::string &const_name);


bool is_global_variable(const std::string &var_name);

bool insert_global_variable(const std::string &variable_name, SymbolType symbol_type, int length = 0);

VariableSymbol *get_global_variable(const std::string &variable_name);


bool is_global_symbol(const std::string &symbol_name);

bool is_non_function_global_symbol(const std::string &symbol_name);

/* ------------------------------------------------Local-------------------------------------------------------------*/

bool is_parameter(const std::string &function_name, const std::string &parameter_name);

bool insert_parameter(const std::string &function_name, const std::string &parameter_name, SymbolType symbol_type);

VariableSymbol *get_parameter(const std::string &function_name, const std::string &parameter_name);


bool is_local_const(const std::string &function_name, const std::string &const_name);

bool insert_local_const(const std::string &function_name, const std::string &const_name, SymbolType symbol_type,
                        int value);

ConstantSymbol *get_local_const(const std::string &function_name, const std::string &const_name);


bool is_local_variable(const std::string &function_name, const std::string &var_name);

bool insert_local_variable(const std::string &function_name, const std::string &variable_name, SymbolType symbol_type,
                           int length = 0);

VariableSymbol *get_local_variable(const std::string &function_name, const std::string &variable_name);


bool is_local_symbol(const std::string &function_name, const std::string &symbol_name);

bool is_matched_parameter_type(const std::string &callee, const std::vector<SymbolType> &value_parameters);

#endif //COMPILER_SYMBOLTABLE_H
