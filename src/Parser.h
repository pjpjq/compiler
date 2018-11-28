//
// Created by monotasker on 2018/11/18.
//
// ÿ�� parse ����ʱ��Ҫ׼��һ�� token ������ fetch_token()
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
 * �����ǲ����޷�������
 * ���޷���������  ::= ���������֣��������֣�������
 * @return
 */
bool parse_unsigned_integer();

/**
 * �� "+", "-", ���һλ���� ��ʼ��������
 * @return
 */
bool parse_integer();

/**
 * �� "const" ����� int ���� char ��ʼ������������
 * ���������壾   ::=   int����ʶ��������������{,����ʶ��������������}
 *                          | char����ʶ���������ַ���{,����ʶ���������ַ���}
 * @return
 */
bool parse_const_definition();

/**
 * �� "const" ��ʼ������������
 * ������˵���� ::=  const���������壾;{ const���������壾;}
 * @return
 */
bool parse_const_declarations();


/**
 *
 * ���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ����'['���޷���������']')
 * {,(����ʶ����|����ʶ����'['���޷���������']') }
 *
 * //���޷�����������ʾ����Ԫ�صĸ�������ֵ�����0
 * @return
 */
bool parse_variable_definition();

/**
 *
 * ������˵����  ::= ���������壾;{���������壾;}
 * @return
 */
bool parse_variable_declarations();

/**
 * �� "("  ��ʼ�����з���ֵ�ĺ���
 * ���з���ֵ�������壾  ::=  int/char identifier '('��������')' '{'��������䣾'}��
 * @return
 */
bool parse_non_void_function_definition();

/**
 * ��������    ::= �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
 * @return
 */
bool parse_parameter_list();

/**
 * ��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����䣾��
 * @return
 */
bool parse_block();

/**
 * ����䣾    ::= ��������䣾����ѭ����䣾| '{'������䣾��'}'�����з���ֵ����������䣾;
?                      |���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;����������䣾;
 * @return
 */
bool parse_statements();

/**
 *
 * ��������䣾  ::=  if '('��������')'����䣾��else����䣾��
 * @return
 */
bool parse_conditional_statement();

/**
 * ��ѭ����䣾   ::=  do����䣾while '('��������')' |
 *                   for'('����ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)���޷��������� ')'����䣾
 * @return
 */
bool parse_loop_statement();
/**
 * ����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ����'['�����ʽ��']'=�����ʽ��
 * @return
 */
//bool parse_assignment_statement();

/**
 * �����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}   //[+|-]ֻ�����ڵ�һ��<��>
 * @return
 */
bool parse_expression();

/**
 * ��������    ::=  �����ʽ������ϵ������������ʽ���������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
 * @return
 */
bool parse_condition();

/**
 * ���     ::= �����ӣ�{���˷�������������ӣ�}
 * @return
 */
bool parse_term();

/**
 * �����ӣ�    ::= ����ʶ����������ʶ����'['�����ʽ��']'����������|���ַ��������з���ֵ����������䣾|'('�����ʽ��')��
 * @return
 */
bool parse_factor();

/**
 * ���з���ֵ����������䣾 ::= ����ʶ����'('��ֵ������')' // TODO ��װ��������
 * @return
 */
//bool parse_funtion_call();

/**
 * ��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
 * @return
 */
bool parse_value_parameters();

/**
 * ������䣾    ::=  scanf '('����ʶ����{,����ʶ����}')'
 * @return
 */
bool parse_scanf_statement();

/**
 * ��д��䣾    ::=  printf'('���ַ�����,�����ʽ��')'|printf '('���ַ�����')'|printf '('�����ʽ��')'
 * @return
 */
bool parse_printf_statement();

/**
 * ���޷���ֵ�������壾  ::= void����ʶ����'('��������')''{'��������䣾'}��
 * @return
 */
bool parse_void_function_definition();

/**
 * ����������    ::= void main'('')' '{'��������䣾'}��
 * @return
 */
bool parse_main_function_definition();

/**
 * ����һ�� main
 * @return
 */
//bool parse_functions();

bool parse_program();

#endif //COMPILER_PARSER_H
