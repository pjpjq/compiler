//
// Created by monotasker on 2018/11/18.
//
// 每个 parse 结束时都要准备一个 token 即调用 fetch_token()
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "Lexer.h"

extern std::string source_file_str;
extern std::string token_buffer;
extern int line_count;
extern int n_errors;
extern int cur_token_idx;

/**
 * 分析是不是无符号整数
 * ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝｜０
 * @return
 */
bool parse_unsigned_integer();

/**
 * 从 "+", "-", 或第一位数字 开始分析整数
 * @return
 */
bool parse_integer();

/**
 * 从 "const" 后面的 int 或者 char 开始分析常量定义
 * ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
 *                          | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
 * @return
 */
bool parse_const_definition();

/**
 * 从 "const" 开始分析常量声明
 * ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
 * @return
 */
bool parse_const_declarations();


/**
 *
 * ＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']')
 * {,(＜标识符＞|＜标识符＞'['＜无符号整数＞']') }
 *
 * //＜无符号整数＞表示数组元素的个数，其值需大于0
 * @return
 */
bool parse_variable_definition();

/**
 *
 * ＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
 * @return
 */
bool parse_variable_declarations();

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
bool parse_parameter_list();

/**
 * ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］｛＜语句＞｝
 * @return
 */
bool parse_block();

/**
 * ＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| '{'｛＜语句＞｝'}'｜＜有返回值函数调用语句＞;
?                      |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;
 * @return
 */
bool parse_statements();

/**
 *
 * ＜条件语句＞  ::=  if '('＜条件＞')'＜语句＞［else＜语句＞］
 * @return
 */
bool parse_conditional_statement();

/**
 * ＜循环语句＞   ::=  do＜语句＞while '('＜条件＞')' |
 *                   for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜无符号整数＞ ')'＜语句＞
 * @return
 */
bool parse_loop_statement();
/**
 * ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
 * @return
 */
//bool parse_assignment_statement();

/**
 * ＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
 * @return
 */
bool parse_expression();

/**
 * ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
 * @return
 */
bool parse_condition();

/**
 * ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
 * @return
 */
bool parse_term();

/**
 * ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|'('＜表达式＞')’
 * @return
 */
bool parse_factor();

/**
 * ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')' // TODO 封装到函数里
 * @return
 */
//bool parse_funtion_call();

/**
 * ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
 * @return
 */
bool parse_value_parameters();

/**
 * ＜读语句＞    ::=  scanf '('＜标识符＞{,＜标识符＞}')'
 * @return
 */
bool parse_scanf_statement();

/**
 * ＜写语句＞    ::=  printf'('＜字符串＞,＜表达式＞')'|printf '('＜字符串＞')'|printf '('＜表达式＞')'
 * @return
 */
bool parse_printf_statement();

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
