//
// Created by monotasker on 2018/11/28.
//

#ifndef COMPILER_CONSTANTS_H
#define COMPILER_CONSTANTS_H

#include <string>
#include <unordered_set>

/* 整数最长10位数 */
const int MAX_N_DIGITS_INT = 10;

/* keywords 共同为一类 */
static std::string CONST_SYM = "const";
static std::string INT_SYM = "int";
static std::string CHAR_SYM = "char";
static std::string VOID_SYM = "void";
static std::string MAIN_SYM = "main";
static std::string IF_SYM = "if";
static std::string ELSE_SYM = "else";
static std::string DO_SYM = "do";
static std::string WHILE_SYM = "while";
static std::string FOR_SYM = "for";
static std::string SCANF_SYM = "scanf";
static std::string PRINTF_SYM = "printf";
static std::string RETURN_SYM = "return";

static std::unordered_set<std::string> KEYWORD_SET{CONST_SYM, INT_SYM, CHAR_SYM, VOID_SYM, MAIN_SYM, IF_SYM,
                                                   ELSE_SYM, DO_SYM, WHILE_SYM, FOR_SYM, SCANF_SYM, PRINTF_SYM,
                                                   RETURN_SYM};

/* separators 分界符, 共同为一类
 * 这里实际采用两种分类型方式, 大类型(SEPARATOR) 用于输出, 小类型用于具体判断 */
static std::string PLUS_SYM = "+";
static std::string MINUS_SYM = "-";
static std::string MUL_SYM = "*";
static std::string DIV_SYM = "/";

static std::string COMMA_SYM = ",";
static std::string SEMICOLON_SYM = ";";
static std::string COLON_SYM = ":";
static std::string ASSIGN_SYM = "=";

static std::string EQ_SYM = "==";
static std::string NE_SYM = "!=";
static std::string LT_SYM = "<";
static std::string GT_SYM = ">";
static std::string LE_SYM = "<=";
static std::string GE_SYM = ">=";

static std::string LPARENTHESIS_SYM = "(";
static std::string RPARENTHESIS_SYM = ")";
static std::string LBRACKET_SYM = "[";
static std::string RBRACKET_SYM = "]";
static std::string LBRACE_SYM = "{";
static std::string RBRACE_SYM = "}";

static std::unordered_set<std::string> SEPARATOR_SET{PLUS_SYM, MINUS_SYM, MUL_SYM, DIV_SYM,
                                                     COMMA_SYM, SEMICOLON_SYM, COLON_SYM, ASSIGN_SYM,
                                                     EQ_SYM, NE_SYM, LT_SYM, GT_SYM, LE_SYM, GE_SYM,
                                                     LPARENTHESIS_SYM, RPARENTHESIS_SYM, LBRACKET_SYM,
                                                     RBRACKET_SYM, LBRACE_SYM, RBRACE_SYM};


#endif //COMPILER_CONSTANTS_H
