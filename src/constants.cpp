//
// Created by monotasker on 2018/12/13.
//
#include "constants.h"

const int MAX_LIMIT_N_CONSECUTIVE_SPACES = 152333;
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
const int MAX_N_FACTORS_PER_TERM = 23452;
/* һ�� scanf ��� var �� */
const int MAX_N_SCANF_VARS = 51234;
/* �����������ڲ���� */
const int MAX_N_INNER_STATEMENTS = 52333;

/* keywords ��ͬΪһ�� */
std::string CONST_SYM = "const";
std::string INT_SYM = "int";
std::string CHAR_SYM = "char";
std::string VOID_SYM = "void";
std::string MAIN_SYM = "main";
std::string IF_SYM = "if";
std::string ELSE_SYM = "else";
std::string DO_SYM = "do";
std::string WHILE_SYM = "while";
std::string FOR_SYM = "for";
std::string SCANF_SYM = "scanf";
std::string PRINTF_SYM = "printf";
std::string RETURN_SYM = "return";

std::unordered_set<std::string> KEYWORD_SET{CONST_SYM, INT_SYM, CHAR_SYM, VOID_SYM, MAIN_SYM, IF_SYM,
                                            ELSE_SYM, DO_SYM, WHILE_SYM, FOR_SYM, SCANF_SYM, PRINTF_SYM,
                                            RETURN_SYM};

/* separators �ֽ��, ��ͬΪһ��
 * ����ʵ�ʲ������ַ����ͷ�ʽ, ������(SEPARATOR) �������, С�������ھ����ж� */
std::string PLUS_SYM = "+";
std::string MINUS_SYM = "-";
std::string MUL_SYM = "*";
std::string DIV_SYM = "/";

std::string COMMA_SYM = ",";
std::string SEMICOLON_SYM = ";";
std::string COLON_SYM = ":";
std::string ASSIGN_SYM = "=";

std::string EQ_SYM = "==";
std::string NE_SYM = "!=";
std::string LT_SYM = "<";
std::string GT_SYM = ">";
std::string LE_SYM = "<=";
std::string GE_SYM = ">=";

std::string LPARENTHESIS_SYM = "(";
std::string RPARENTHESIS_SYM = ")";
std::string LBRACKET_SYM = "[";
std::string RBRACKET_SYM = "]";
std::string LBRACE_SYM = "{";
std::string RBRACE_SYM = "}";

std::unordered_set<std::string> SEPARATOR_SET{PLUS_SYM, MINUS_SYM, MUL_SYM, DIV_SYM,
                                              COMMA_SYM, SEMICOLON_SYM, COLON_SYM, ASSIGN_SYM,
                                              EQ_SYM, NE_SYM, LT_SYM, GT_SYM, LE_SYM, GE_SYM,
                                              LPARENTHESIS_SYM, RPARENTHESIS_SYM, LBRACKET_SYM,
                                              RBRACKET_SYM, LBRACE_SYM, RBRACE_SYM};

/**
 * ��Ԫʽ������, ĩβ�ӿո�ʹ֮���ܱ���Ϊ��ʶ��
 */
std::string DECLARE_FUNCTION_OP = "declare_function ";
std::string DEFINE_PARAMETER_OP = "define_parameter ";
std::string PUSH_PARAMETER_OP = "push_parameter ";
std::string CALL_FUNCTION_OP = "call_function ";
std::string ASSIGN_RETURN_VAL_OP = "assign_return_val ";
std::string ASSIGN_OP = "assign ";
std::string RETURN_OP = "return ";
std::string DECLARE_VARIABLE_OP = "declare_variable ";
std::string JUMP_OP = "jump ";
std::string BZ_OP = "bz ";
std::string BNZ_OP = "bnz ";
std::string SCANF_OP = "scanf ";
std::string PRINTF_OP = "printf ";
std::string READ_ARRAY_OP = "read_array ";
std::string WRITE_ARRAY_OP = "write_array ";
std::string LABEL_OP = "label_op ";
std::string EXIT_OP = "exit ";
/* printf ����Ԫʽ���һ������ */
std::string PRINTF_STRING_TYPE_SYM = "string";