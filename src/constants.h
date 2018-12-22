//
// Created by monotasker on 2018/11/28.
//

#ifndef COMPILER_CONSTANTS_H
#define COMPILER_CONSTANTS_H

#include <string>
#include <unordered_set>

/* ��������ո� */
extern const int MAX_LIMIT_N_CONSECUTIVE_SPACES;
/* �����10λ�� */
extern const int MAX_N_DIGITS_INT;
/* �������Ŀ���� */
extern const int MAX_N_FUNCTIONS_LIMIT;
/* ��������Ŀ���� */
extern const int MAX_N_PARAMETERS_LIMIT;
/* ��������Ŀ���� */
extern const int MAX_N_STATEMENTS_LIMIT;
/* ������������Ŀ���� */
extern const int MAX_N_VAR_DEFINITIONS;
/* һ�����ʽ������������� */
extern const int MAX_N_TERMS_PER_EXPRESSION;
/* һ��������������������� */
extern const int MAX_N_FACTORS_PER_TERM;
/* һ�� scanf ��� var �� */
extern const int MAX_N_SCANF_VARS;
/* �����������ڲ���� */
extern const int MAX_N_INNER_STATEMENTS;

/* keywords ��ͬΪһ�� */
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

/* separators �ֽ��, ��ͬΪһ��
 * ����ʵ�ʲ������ַ����ͷ�ʽ, ������(SEPARATOR) �������, С�������ھ����ж� */
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
 * ��Ԫʽ������, ĩβ�ӿո�ʹ֮���ܱ���Ϊ��ʶ��
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
/* printf ����Ԫʽ���һ������ */
extern std::string PRINTF_STRING_TYPE_SYM;

#endif //COMPILER_CONSTANTS_H
