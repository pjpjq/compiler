//
// Created by monotasker on 2018/11/29.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <unordered_map>
#include <string>

class Symbol;

class VariableSymbol;

class ConstantSymbol;

class LocalSymbolTable;

typedef std::unordered_map<std::string, VariableSymbol> VariableTable;
typedef std::unordered_map<std::string, ConstantSymbol> ConstantTable;
typedef std::unordered_map<std::string, LocalSymbolTable> FunctionTable;

enum SymbolType {
    UNKNOWN_SYMBOL_TYPE, INT_SYMBOL_TYPE, CHAR_SYMBOL_TYPE, VOID_SYMBOL_TYPE,
};

static std::string symbol_type_strs[] = {"Unknown symbol type", "int", "char", "void"};

class Symbol {
public:
    std::string get_name();
    
    SymbolType get_symbol_type();

private:
    std::string name;
    SymbolType symbol_type = UNKNOWN_SYMBOL_TYPE;
};

class VariableSymbol : public Symbol {
public:
    int get_length();
    
    bool is_array();

private:
    int length = 0;
};

class ConstantSymbol : public Symbol {
public:
    int get_value();

private:
    int value = 123456;
};


class LocalSymbolTable : public Symbol {
public:
    VariableTable parameter_table;
    ConstantTable constant_table;
    VariableTable variable_table;

private:

};


#endif //COMPILER_SYMBOLTABLE_H
