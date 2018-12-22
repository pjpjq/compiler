//
// Created by monotasker on 2018/11/28.
//

#ifndef COMPILER_CONSTANTS_H
#define COMPILER_CONSTANTS_H

#include <string>
#include <unordered_set>

/* 最多连续空格 */
extern const int MAX_LIMIT_N_CONSECUTIVE_SPACES;
/* 整数最长10位数 */
extern const int MAX_N_DIGITS_INT;
/* 最大函数数目限制 */
extern const int MAX_N_FUNCTIONS_LIMIT;
/* 最大参数数目限制 */
extern const int MAX_N_PARAMETERS_LIMIT;
/* 最大语句数目限制 */
extern const int MAX_N_STATEMENTS_LIMIT;
/* 最大变量定义数目限制 */
extern const int MAX_N_VAR_DEFINITIONS;
/* 一个表达式里最多连续的项 */
extern const int MAX_N_TERMS_PER_EXPRESSION;
/* 一个项里最多连续的因子数 */
extern const int MAX_N_FACTORS_PER_TERM;
/* 一句 scanf 最多 var 数 */
extern const int MAX_N_SCANF_VARS;
/* 复合语句最多内部语句 */
extern const int MAX_N_INNER_STATEMENTS;

/* keywords 共同为一类 */
extern std::string CONST_SYM;
extern std::string INT_SYM;
extern std::string CHAR_SYM;
extern std::string VOID_SYM;
extern std::string MAIN_SYM;
extern std::string IF_SYM;
extern std::string ELSE_SYM;
extern std::string DO_SYM;
extern std::string WHILE_SYM;
extern std::string FOR_SYM;
extern std::string SCANF_SYM;
extern std::string PRINTF_SYM;
extern std::string RETURN_SYM;

extern std::unordered_set<std::string> KEYWORD_SET;

/* separators 分界符, 共同为一类
 * 这里实际采用两种分类型方式, 大类型(SEPARATOR) 用于输出, 小类型用于具体判断 */
extern std::string PLUS_SYM;
extern std::string MINUS_SYM;
extern std::string MUL_SYM;
extern std::string DIV_SYM;

extern std::string COMMA_SYM;
extern std::string SEMICOLON_SYM;
extern std::string COLON_SYM;
extern std::string ASSIGN_SYM;

extern std::string EQ_SYM;
extern std::string NE_SYM;
extern std::string LT_SYM;
extern std::string GT_SYM;
extern std::string LE_SYM;
extern std::string GE_SYM;

extern std::string LPARENTHESIS_SYM;
extern std::string RPARENTHESIS_SYM;
extern std::string LBRACKET_SYM;
extern std::string RBRACKET_SYM;
extern std::string LBRACE_SYM;
extern std::string RBRACE_SYM;

extern std::unordered_set<std::string> SEPARATOR_SET;

/**
 * 四元式的运算, 末尾加空格使之不能被作为标识符
 */
extern std::string DECLARE_FUNCTION_OP;
extern std::string DEFINE_PARAMETER_OP;
extern std::string PUSH_PARAMETER_OP;
extern std::string CALL_FUNCTION_OP;
extern std::string ASSIGN_RETURN_VAL_OP;
extern std::string ASSIGN_OP;
extern std::string RETURN_OP;
extern std::string DECLARE_VARIABLE_OP;
//extern std::string DECLARE_CONST_OP = "declare_const ";
extern std::string JUMP_OP;
extern std::string BZ_OP;
extern std::string BNZ_OP;
extern std::string SCANF_OP;
extern std::string PRINTF_OP;
extern std::string READ_ARRAY_OP;
extern std::string WRITE_ARRAY_OP;
extern std::string LABEL_OP;
extern std::string EXIT_OP;
/* printf 的四元式里的一种类型 */
extern std::string PRINTF_STRING_TYPE_SYM;

#endif //COMPILER_CONSTANTS_H
