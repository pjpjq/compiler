//
// Created by monotasker on 2018/11/18.
//
// 每个 parse 结束时都要准备一个 token: ++cur_token_idx, 或者调 parse_xxx()
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <typeinfo>

#include "constants.h"
#include "Lexer.h"
#include "utils.h"
#include "SymbolTable.h"
#include "IR.h"

/**
 * 分析是不是无符号整数
 * ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝｜０
 * @param res 无符号数结果
 * @return 判断结果
 */
bool parse_unsigned_integer(int &res);

/**
 * 从 "+", "-", 或第一位数字 开始分析整数
 * @param res 有符号数结果
 * @return 判断结果
 */
bool parse_integer(int &res);

/**
 *  从 "const" 后面的 int 或者 char 开始分析常量定义
 * ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
 *                          | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
 * @param function_name 空就是全局, 非空就是给定的函数局部
 * @return
 */
bool parse_const_definition(const std::string &function_name = "");

/**
 * 从 "const" 开始分析常量声明
 * ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
 * @param function_name 空就是全局, 非空就是给定的函数局部
 * @return
 */
bool parse_const_declarations(const std::string &function_name = "");

/**
 *
 * ＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']')
 * {,(＜标识符＞|＜标识符＞'['＜无符号整数＞']') }
 *
 * //＜无符号整数＞表示数组元素的个数，其值需大于0
 * @param function_name 空就是全局, 非空就是给定的函数局部
 * @return
 */
bool parse_variable_definition(const std::string &function_name = "");

/**
 *
 * ＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
 *
 * @param function_name 空就是全局, 非空就是给定的函数局部
 * @return
 */
bool parse_variable_declarations(const std::string &function_name = "");

/**
 * 从 "("  开始分析有返回值的函数
 * ＜有返回值函数定义＞  ::=  int/char identifier '('＜参数表＞')' '{'＜复合语句＞'}’
 * @return
 */
bool parse_non_void_function_definition();

/**
 * ＜参数表＞    ::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
 * @return
 */
bool parse_parameter_list(const std::string &function_name);

/**
 * ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］｛＜语句＞｝
 * @return
 */
bool parse_block(const std::string &function_name);

/**
 * ＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| '{'｛＜语句＞｝'}'｜＜有返回值函数调用语句＞;
?                      |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;
 * @return
 */
bool parse_statements(const std::string &function_name, bool &has_return_statement);

/**
 *
 * ＜条件语句＞  ::=  if '('＜条件＞')'＜语句＞［else＜语句＞］
 * @return
 */
bool parse_conditional_statement(const std::string &function_name, bool &has_return_statement);

/**
 * ＜循环语句＞   ::=  do＜语句＞while '('＜条件＞')' |
 *                   for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜无符号整数＞ ')'＜语句＞
 * @return
 */
bool parse_loop_statement(const std::string &function_name, bool &has_return_statement);
/**
 * ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
 * @return
 */
//bool parse_assignment_statement();

/**
 * ＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
 * @return
 */
bool parse_expression(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                      SymbolType &res_type);

/**
 * ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
 * @return
 */
bool parse_condition(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                     SymbolType &res_type);

/**
 * ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
 * @return
 */
bool parse_term(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                SymbolType &res_type);

/**
 * ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|'('＜表达式＞')’
 * @return
 */
bool parse_factor(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                  SymbolType &res_type);

/**
 *  // TODO: 要区分有无返回值吗???
 * ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
 * @return
 */
bool parse_funtion_call(const std::string &function_name);

/**
 * ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
 * @return
 */
bool parse_value_parameters(const std::string &function_name, const std::string &callee);

/**
 * ＜读语句＞    ::=  scanf '('＜标识符＞{,＜标识符＞}')'
 * @return
 */
bool parse_scanf_statement(const std::string &function_name);

/**
 * ＜写语句＞    ::=  printf'('＜字符串＞,＜表达式＞')'|printf '('＜字符串＞')'|printf '('＜表达式＞')'
 * @return
 */
bool parse_printf_statement(const std::string &function_name);

/**
 * ＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}’
 * @return
 */
bool parse_void_function_definition();

/**
 * ＜主函数＞    ::= void main'('')' '{'＜复合语句＞'}’
 * @return
 */
bool parse_main_function_definition();

/**
 * 至少一个 main
 * @return
 */
//bool parse_functions();

bool parse_program();

#endif //COMPILER_PARSER_H
