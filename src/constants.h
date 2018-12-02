//
// Created by monotasker on 2018/11/28.
//

#ifndef COMPILER_CONSTANTS_H
#define COMPILER_CONSTANTS_H

#include <string>
#include <unordered_set>

/* �����10λ�� */
const int MAX_N_DIGITS_INT = 10;
/* �������Ŀ���� */
const int MAX_N_FUNCTIONS_LIMIT = 12345;
/* ��������Ŀ���� */
const int MAX_N_PARAMETERS_LIMIT = 20;
/* ��������Ŀ���� */
const int MAX_N_STATEMENTS_LIMIT = 123456;
/* ������������Ŀ���� */
const int MAX_N_VAR_DEFINITIONS = 12345;
/* һ�����ʽ������������� */
const int MAX_N_TERMS_PER_EXPRESSION = 12345;
/* һ��������������������� */
const int MAX_N_FACTORS_PER_TERM = 2345;

/* keywords ��ͬΪһ�� */
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

/* separators �ֽ��, ��ͬΪһ��
 * ����ʵ�ʲ������ַ����ͷ�ʽ, ������(SEPARATOR) �������, С�������ھ����ж� */
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

/**
 * ��Ԫʽ������, ĩβ�ӿո�ʹ֮���ܱ���Ϊ��ʶ��
 */
static std::string DECLARE_FUNCTION_OP = "declare_function ";
static std::string DEFINE_PARAMETER_OP = "define_parameter ";
static std::string PUSH_PARAMETER_OP = "push_parameter ";
static std::string CALL_FUNCTION_OP = "call_function ";
static std::string ASSIGN_RETURN_VAL_OP = "assign_return_val ";
static std::string ASSIGN_OP = "assign_ ";
static std::string RETURN_OP = "return ";
static std::string DECLARE_VARIABLE_OP = "declare_variable ";
//static std::string DECLARE_CONST_OP = "declare_const ";
static std::string JUMP_OP = "jump ";
static std::string BZ_OP = "bz ";
static std::string BNZ_OP = "bnz ";
static std::string SCANF_OP = "scanf ";
static std::string PRINTF_OP = "printf ";
static std::string READ_ARRAY_OP = "read_array ";
static std::string WRITE_ARRAY_OP = "write_array ";
static std::string LABEL_OP = "label_op ";
static std::string EXIT_OP = "exit ";
/* printf ����Ԫʽ���һ������ */
static std::string PRINTF_STRING_TYPE_SYM = "string";

#endif //COMPILER_CONSTANTS_H
