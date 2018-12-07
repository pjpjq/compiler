//
// Created by monotasker on 2018/11/18.
//
// ÿ�� parse ����ʱ��Ҫ׼��һ�� token: ++cur_token_idx, ���ߵ� parse_xxx()
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
 * �����ǲ����޷�������
 * ���޷���������  ::= ���������֣��������֣�������
 * @param res �޷��������
 * @return �жϽ��
 */
bool parse_unsigned_integer(int &res);

/**
 * �� "+", "-", ���һλ���� ��ʼ��������
 * @param res �з��������
 * @return �жϽ��
 */
bool parse_integer(int &res);

/**
 *  �� "const" ����� int ���� char ��ʼ������������
 * ���������壾   ::=   int����ʶ��������������{,����ʶ��������������}
 *                          | char����ʶ���������ַ���{,����ʶ���������ַ���}
 * @param function_name �վ���ȫ��, �ǿվ��Ǹ����ĺ����ֲ�
 * @return
 */
bool parse_const_definition(const std::string &function_name = "");

/**
 * �� "const" ��ʼ������������
 * ������˵���� ::=  const���������壾;{ const���������壾;}
 * @param function_name �վ���ȫ��, �ǿվ��Ǹ����ĺ����ֲ�
 * @return
 */
bool parse_const_declarations(const std::string &function_name = "");

/**
 *
 * ���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ����'['���޷���������']')
 * {,(����ʶ����|����ʶ����'['���޷���������']') }
 *
 * //���޷�����������ʾ����Ԫ�صĸ�������ֵ�����0
 * @param function_name �վ���ȫ��, �ǿվ��Ǹ����ĺ����ֲ�
 * @return
 */
bool parse_variable_definition(const std::string &function_name = "");

/**
 *
 * ������˵����  ::= ���������壾;{���������壾;}
 *
 * @param function_name �վ���ȫ��, �ǿվ��Ǹ����ĺ����ֲ�
 * @return
 */
bool parse_variable_declarations(const std::string &function_name = "");

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
bool parse_parameter_list(const std::string &function_name);

/**
 * ��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����䣾��
 * @return
 */
bool parse_block(const std::string &function_name);

/**
 * ����䣾    ::= ��������䣾����ѭ����䣾| '{'������䣾��'}'�����з���ֵ����������䣾;
?                      |���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;����������䣾;
 * @return
 */
bool parse_statements(const std::string &function_name, bool &has_return_statement);

/**
 *
 * ��������䣾  ::=  if '('��������')'����䣾��else����䣾��
 * @return
 */
bool parse_conditional_statement(const std::string &function_name, bool &has_return_statement);

/**
 * ��ѭ����䣾   ::=  do����䣾while '('��������')' |
 *                   for'('����ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)���޷��������� ')'����䣾
 * @return
 */
bool parse_loop_statement(const std::string &function_name, bool &has_return_statement);
/**
 * ����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ����'['�����ʽ��']'=�����ʽ��
 * @return
 */
//bool parse_assignment_statement();

/**
 * �����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}   //[+|-]ֻ�����ڵ�һ��<��>
 * @return
 */
bool parse_expression(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                      SymbolType &res_type);

/**
 * ��������    ::=  �����ʽ������ϵ������������ʽ���������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
 * @return
 */
bool parse_condition(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                     SymbolType &res_type);

/**
 * ���     ::= �����ӣ�{���˷�������������ӣ�}
 * @return
 */
bool parse_term(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                SymbolType &res_type);

/**
 * �����ӣ�    ::= ����ʶ����������ʶ����'['�����ʽ��']'����������|���ַ��������з���ֵ����������䣾|'('�����ʽ��')��
 * @return
 */
bool parse_factor(const std::string &function_name, bool &is_res_imm, int &res_val, std::string &res_temp_var,
                  SymbolType &res_type);

/**
 *  // TODO: Ҫ�������޷���ֵ��???
 * ���з���ֵ����������䣾 ::= ����ʶ����'('��ֵ������')'
 * @return
 */
bool parse_funtion_call(const std::string &function_name);

/**
 * ��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
 * @return
 */
bool parse_value_parameters(const std::string &function_name, const std::string &callee);

/**
 * ������䣾    ::=  scanf '('����ʶ����{,����ʶ����}')'
 * @return
 */
bool parse_scanf_statement(const std::string &function_name);

/**
 * ��д��䣾    ::=  printf'('���ַ�����,�����ʽ��')'|printf '('���ַ�����')'|printf '('�����ʽ��')'
 * @return
 */
bool parse_printf_statement(const std::string &function_name);

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
