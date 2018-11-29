//
// Created by monotasker on 2018/11/29.
//

#include "SymbolTable.h"

std::string Symbol::get_name() {
    return name;
}

SymbolType Symbol::get_symbol_type() {
    return symbol_type;
}

int VariableSymbol::get_length() {
    return length;
}

bool VariableSymbol::is_array() {
    return length > 0;
}

int ConstantSymbol::get_value() {
    return value;
}


