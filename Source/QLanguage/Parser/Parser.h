/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:35
	filename: 	\QLanguage\Parser\Parser.h
	file path:	\QLanguage\Parser
	file base:	Parser
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_PARSER_H_
#define _QLANGUAGE_PARSER_H_

#include <climits>

#include "../../QCore/Library/hashset.h"
#include "../../QCore/Library/hashmap.h"

#include "../VirtualMachine/opcodes.h"
#include "../VirtualMachine/Variant.h"

#include "SyntaxTree/SyntaxTree_Base.h"
#include "BasicParser.h"

namespace QLanguage
{
#define MEMBER_DESC_MEMBER_DESC_LETTER                                      1   // member_desc -> member_desc "." "{Letter}"
#define MEMBER_DESC_LETTER                                                  2   // member_desc -> "{Letter}"
#define TYPE_DESC_UNSIGNED_CHAR                                             3   // type_desc -> "unsigned" "char"
#define TYPE_DESC_CHAR                                                      4   // type_desc -> "char"
#define TYPE_DESC_UNSIGNED_SHORT                                            5   // type_desc -> "unsigned" "short"
#define TYPE_DESC_SHORT                                                     6   // type_desc -> "short"
#define TYPE_DESC_UNSIGNED_INT                                              7   // type_desc -> "unsigned" "int"
#define TYPE_DESC_INT                                                       8   // type_desc -> "int"
#define TYPE_DESC_UNSIGNED_LONG                                             9   // type_desc -> "unsigned" "long"
#define TYPE_DESC_LONG                                                      10  // type_desc -> "long"
#define TYPE_DESC_FLOAT                                                     11  // type_desc -> "float"
#define TYPE_DESC_DOUBLE                                                    12  // type_desc -> "double"
#define TYPE_DESC_STRING                                                    13  // type_desc -> "string"
#define TYPE_DESC_AUTO                                                      14  // type_desc -> "auto"
#define TYPE_DESC_LETTER                                                    15  // type_desc -> "{Letter}"
#define VALUE_DESC_TRUE                                                     16  // value_desc -> "true"
#define VALUE_DESC_FALSE                                                    17  // value_desc -> "false"
#define VALUE_DESC_REAL                                                     18  // value_desc -> "{Real}"
#define VALUE_DESC_DIGIT                                                    19  // value_desc -> "{Digit}"
#define VALUE_DESC_MEMBER_DESC                                              20  // value_desc -> member_desc
#define VALUE_DESC_STRING                                                   21  // value_desc -> "{String}"
#define VALUE_DESC_VALUES                                                   22  // value_desc -> values
#define VALUES_VALUES_LIST                                                  23  // values -> "{" values_list "}"
#define VALUES_LIST_VALUES_LIST_VALUE_DESC                                  24  // values_list -> values_list "," value_desc
#define VALUES_LIST_VALUE_DESC                                              25  // values_list -> value_desc
#define VALUE_LIST_VALUE_LIST_EXP                                           26  // value_list -> value_list "," exp
#define VALUE_LIST_EXP                                                      27  // value_list -> exp
#define ATTRIBUTE_PUBLIC                                                    28  // attribute -> "public"
#define ATTRIBUTE_PRIVATE                                                   29  // attribute -> "private"
#define ATTRIBUTE_PROTECTED                                                 30  // attribute -> "protected"
#define ARRAY_LST_ARRAY_LST_EXP                                             31  // array_lst -> array_lst "[" exp "]"
#define ARRAY_LST_ARRAY_LST_NO_EXP                                          32  // array_lst -> array_lst "[" "]"
#define ARRAY_LST_EXP                                                       33  // array_lst -> "[" exp "]"
#define ARRAY_LST_NO_EXP                                                    34  // array_lst -> "[" "]"
#define START_ITEM_LIST                                                     35  // start -> item_list
#define PARAMTER_LIST_PARAMTER_LIST_PARAMTER                                36  // paramter_list -> paramter_list "," paramter
#define PARAMTER_LIST_PARAMTER                                              37  // paramter_list -> paramter
#define PARAMTER_TYPE_DESC_LETTER_ARRAY_LST                                 38  // paramter -> type_desc "{Letter}" array_lst
#define PARAMTER_TYPE_DESC_LETTER                                           39  // paramter -> type_desc "{Letter}"
#define PARAMTER_TYPE_DESC                                                  40  // paramter -> type_desc
#define ITEM_LIST_ITEM_LIST_ITEM                                            41  // item_list -> item_list item
#define ITEM_LIST_ITEM                                                      42  // item_list -> item
#define ITEM_GLOBAL_FUNCTION_DESC                                           43  // item -> global_function_desc
#define ITEM_DECLARE_DESC                                                   44  // item -> declare_desc ";"
#define ITEM_INTERFACE_DESC                                                 45  // item -> interface_desc
#define ITEM_CLASS_DESC                                                     46  // item -> class_desc
#define STMT_LIST_STMT_LIST_STMT                                            47  // stmt_list -> stmt_list stmt
#define STMT_LIST_STMT                                                      48  // stmt_list -> stmt
#define STMT_LIST_STMT_LIST_BLOCK                                           49  // stmt_list -> stmt_list block
#define STMT_LIST_BLOCK                                                     50  // stmt_list -> block
#define BLOCK_STMT_LIST                                                     51  // block -> "{" stmt_list "}"
#define BLOCK_EMPTY                                                         52  // block -> "{" "}"
#define TEMPLATE_DESC_TEMPLATE_TEMPLATE_LIST                                53  // template_desc -> "template" "<" template_list ">"
#define TEMPLATE_DESC_TEMPLATE                                              54  // template_desc -> "template" "<" ">"
#define TEMPLATE_LIST_TEMPLATE_LIST_TEMPLATE_ITEM                           55  // template_list -> template_list "," template_item
#define TEMPLATE_LIST_TEMPLATE_ITEM                                         56  // template_list -> template_item
#define TEMPLATE_ITEM_TYPENAME_LETTER                                       57  // template_item -> "typename" "{Letter}"
#define TEMPLATE_ITEM_TYPE_DESC_LETTER                                      58  // template_item -> type_desc "{Letter}"
#define GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_PARAM_LIST_BLOCK       59  // global_function_desc -> template_desc type_desc "{Letter}" "(" paramter_list ")" block
#define GLOBAL_FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK                     60  // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
#define GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_NOPARAM_BLOCK          61  // global_function_desc -> template_desc type_desc "{Letter}" "(" ")" block
#define GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK                        62  // global_function_desc -> type_desc "{Letter}" "(" ")" block
#define GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_VOID_PARAM_LIST_BLOCK            63  // global_function_desc -> template_desc "void" "{Letter}" "(" paramter_list ")" block
#define GLOBAL_FUNCTION_DESC_VOID_PARAM_LIST_BLOCK                          64  // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
#define GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_VOID_NOPARAM_BLOCK               65  // global_function_desc -> template_desc "void" "{Letter}" "(" ")" block
#define GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK                             66  // global_function_desc -> "void" "{Letter}" "(" ")" block
#define FUNCTION_DECLARE_TEMPLATE_DESC_TYPE_DESC_PARAM_LIST                 67  // function_declare -> template_desc type_desc "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DECLARE_TYPE_DESC_PARAM_LIST                               68  // function_declare -> type_desc "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DECLARE_TEMPLATE_DESC_TYPE_DESC                            69  // function_declare -> template_desc type_desc "{Letter}" "(" ")" ";"
#define FUNCTION_DECLARE_TYPE_DESC                                          70  // function_declare -> type_desc "{Letter}" "(" ")" ";"
#define FUNCTION_DECLARE_TEMPLATE_DESC_VOID_PARAM_LIST                      71  // function_declare -> template_desc "void" "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DECLARE_VOID_PARAM_LIST                                    72  // function_declare -> "void" "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DECLARE_TEMPLATE_DESC_VOID                                 73  // function_declare -> template_desc "void" "{Letter}" "(" ")" ";"
#define FUNCTION_DECLARE_VOID                                               74  // function_declare -> "void" "{Letter}" "(" ")" ";"
#define INTERFACE_DESC_INTERFACE_CONTENT_LIST                               75  // interface_desc -> "interface" "{Letter}" "{" interface_content "}"
#define INTERFACE_DESC_NO_INTERFACE_CONTENT_LIST                            76  // interface_desc -> "interface" "{Letter}" "{" "}"
#define INTERFACE_CONTENT_INTERFACE_CONTENT_FUNCTION_DECLARE                77  // interface_content -> interface_content function_declare
#define INTERFACE_CONTENT_FUNCTION_DECLARE                                  78  // interface_content -> function_declare
#define CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1_CLASS_DESC2_CLASS_CONTENT_LIST 79  // class_desc -> template_desc class_desc1 class_desc2 "{" class_content_list "}"
#define CLASS_DESC_CLASS_DESC1_CLASS_DESC2_CLASS_CONTENT_LIST               80  // class_desc -> class_desc1 class_desc2 "{" class_content_list "}"
#define CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1_CLASS_CONTENT_LIST             81  // class_desc -> template_desc class_desc1 "{" class_content_list "}"
#define CLASS_DESC_CLASS_DESC1_CLASS_CONTENT_LIST                           82  // class_desc -> class_desc1 "{" class_content_list "}"
#define CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1_CLASS_DESC2                    83  // class_desc -> template_desc class_desc1 class_desc2 "{" "}"
#define CLASS_DESC_CLASS_DESC1_CLASS_DESC2                                  84  // class_desc -> class_desc1 class_desc2 "{" "}"
#define CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1                                85  // class_desc -> template_desc class_desc1 "{" "}"
#define CLASS_DESC_CLASS_DESC1                                              86  // class_desc -> class_desc1 "{" "}"
#define CLASS_DESC1_CLASS_LETTER                                            87  // class_desc1 -> "class" "{Letter}"
#define CLASS_DESC1_CLASS                                                   88  // class_desc1 -> "class"
#define CLASS_DESC2_ATTRIBUTE_IMPLEMENT_LETTER                              89  // class_desc2 -> attribute "implement" "{Letter}"
#define CLASS_DESC2_ATTRIBUTE_EXTEND_LETTER                                 90  // class_desc2 -> attribute "extend" "{Letter}"
#define CLASS_CONTENT_LIST_CLASS_CONTENT_LIST_CLASS_CONTENT                 91  // class_content_list -> class_content_list class_content
#define CLASS_CONTENT_LIST_CLASS_CONTENT                                    92  // class_content_list -> class_content
#define CLASS_CONTENT_FUNCTION_DESC                                         93  // class_content -> function_desc
#define CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC                                94  // class_content -> attribute declare_desc ";"
#define CLASS_CONTENT_DECLARE_DESC                                          95  // class_content -> declare_desc ";"
#define FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK    96  // function_desc -> template_desc attribute type_desc "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK                  97  // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_PARAM_LIST_BLOCK              98  // function_desc -> template_desc type_desc "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK                            99  // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_TYPE_DESC_BLOCK               100 // function_desc -> template_desc attribute type_desc "{Letter}" "(" ")" block
#define FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_BLOCK                             101 // function_desc -> attribute type_desc "{Letter}" "(" ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_BLOCK                         102 // function_desc -> template_desc type_desc "{Letter}" "(" ")" block
#define FUNCTION_DESC_TYPE_DESC_BLOCK                                       103 // function_desc -> type_desc "{Letter}" "(" ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK         104 // function_desc -> template_desc attribute "void" "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK                       105 // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_VOID_PARAM_LIST_BLOCK                   106 // function_desc -> template_desc "void" "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_VOID_PARAM_LIST_BLOCK                                 107 // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_VOID_BLOCK                    108 // function_desc -> template_desc attribute "void" "{Letter}" "(" ")" block
#define FUNCTION_DESC_ATTRIBUTE_VOID_BLOCK                                  109 // function_desc -> attribute "void" "{Letter}" "(" ")" block
#define FUNCTION_DESC_TEMPLATE_DESC_VOID_BLOCK                              110 // function_desc -> template_desc "void" "{Letter}" "(" ")" block
#define FUNCTION_DESC_VOID_BLOCK                                            111 // function_desc -> "void" "{Letter}" "(" ")" block
#define STMT_ASSIGN_DESC                                                    112 // stmt -> assign_desc ";"
#define STMT_CALL_LIST                                                      113 // stmt -> call_list ";"
#define STMT_DECLARE_DESC                                                   114 // stmt -> declare_desc ";"
#define STMT_IF_DESC                                                        115 // stmt -> if_desc
#define STMT_FOR_DESC                                                       116 // stmt -> for_desc
#define STMT_WHILE_DESC                                                     117 // stmt -> while_desc
#define STMT_DO_DESC                                                        118 // stmt -> do_desc ";"
#define STMT_SWITCH_DESC                                                    119 // stmt -> switch_desc
#define STMT_BREAK_DESC                                                     120 // stmt -> break_desc ";"
#define STMT_CONTINUE_DESC                                                  121 // stmt -> continue_desc ";"
#define STMT_RETURN_DESC                                                    122 // stmt -> return_desc
#define STMT_NO_SEMICOLON_ASSIGN_DESC                                       123 // stmt_no_semicolon -> assign_desc
#define STMT_NO_SEMICOLON_CALL_LIST                                         124 // stmt_no_semicolon -> call_desc
#define STMT_NO_SEMICOLON_DECLARE_DESC                                      125 // stmt_no_semicolon -> declare_desc
#define STMT_NO_SEMICOLON_IF_DESC                                           126 // stmt_no_semicolon -> if_desc
#define STMT_NO_SEMICOLON_FOR_DESC                                          127 // stmt_no_semicolon -> for_desc
#define STMT_NO_SEMICOLON_WHILE_DESC                                        128 // stmt_no_semicolon -> while_desc
#define STMT_NO_SEMICOLON_DO_DESC                                           129 // stmt_no_semicolon -> do_desc
#define STMT_NO_SEMICOLON_SWITCH_DESC                                       130 // stmt_no_semicolon -> switch_desc
#define DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST_EQUAL_EXP                131 // declare_desc -> declare_desc "," "{Letter}" array_lst "=" exp
#define DECLARE_DESC_DECLARE_DESC_LETTER_EQUAL_EXP                          132 // declare_desc -> declare_desc "," "{Letter}" "=" exp
#define DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST                          133 // declare_desc -> declare_desc "," "{Letter}" array_lst
#define DECLARE_DESC_DECLARE_DESC_LETTER                                    134 // declare_desc -> declare_desc "," "{Letter}"
#define DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST_EQUAL_EXP                   135 // declare_desc -> type_desc "{Letter}" array_lst "=" exp
#define DECLARE_DESC_TYPE_DESC_LETTER_EQUAL_EXP                             136 // declare_desc -> type_desc "{Letter}" "=" exp
#define DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST                             137 // declare_desc -> type_desc "{Letter}" array_lst
#define DECLARE_DESC_TYPE_DESC_LETTER                                       138 // declare_desc -> type_desc "{Letter}"
#define ASSIGN_DESC_MEMBER_DESC_ADD_EQUAL_EXP                               139 // assign_desc -> member_desc "+" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_SUB_EQUAL_EXP                               140 // assign_desc -> member_desc "-" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_AND_EQUAL_EXP                               141 // assign_desc -> member_desc "&" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_OR_EQUAL_EXP                                142 // assign_desc -> member_desc "|" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_XOR_EQUAL_EXP                               143 // assign_desc -> member_desc "^" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_LEFT_MOVE_EQUAL_EXP                         144 // assign_desc -> member_desc "<" "<" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_RIGHT_MOVE_EQUAL_EXP                        145 // assign_desc -> member_desc ">" ">" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_EQUAL_EXP                                   146 // assign_desc -> member_desc "=" exp
#define CALL_LIST_CALL_LIST_VALUE_LIST                                      147 // call_list -> call_list "(" value_list ")"
#define CALL_LIST_CALL_LIST                                                 148 // call_list -> call_list "(" ")"
#define CALL_LIST_CALL_DESC                                                 149 // call_list -> call_desc
#define CALL_DESC_MEMBER_DESC_VALUE_LIST                                    150 // call_desc -> member_desc "(" value_list ")"
#define CALL_DESC_MEMBER_DESC                                               151 // call_desc -> member_desc "(" ")"
#define IF_DESC_IF_EXP_STMT_ELSE_DESC                                       152 // if_desc -> "if" "(" exp ")" stmt else_desc
#define IF_DESC_IF_EXP_STMT                                                 153 // if_desc -> "if" "(" exp ")" stmt
#define IF_DESC_IF_EXP_BLOCK_ELSE_DESC                                      154 // if_desc -> "if" "(" exp ")" block else_desc
#define IF_DESC_IF_EXP_BLOCK                                                155 // if_desc -> "if" "(" exp ")" block
#define ELSE_DESC_ELSE_STMT                                                 156 // else_desc -> "else" stmt
#define ELSE_DESC_ELSE_BLOCK                                                157 // else_desc -> "else" block
#define FOR_DESC_FOR_STMT_EXP_STMT_BLOCK                                    158 // for_desc -> "for" "(" stmt ";" exp ";" stmt ")" block
#define WHILE_DESC_WHILE_EXP_BLOCK                                          159 // while_desc -> "while" "(" exp ")" block
#define DO_DESC_DO_BLOCK_WHILE_EXP                                          160 // do_desc -> "do" block "while" "(" exp ")"
#define SWITCH_DESC_SWITCH_EXP_SWITCH_CONTENT_LIST                          161 // switch_desc -> "switch" "(" exp ")" "{" switch_content_list "}"
#define SWITCH_CONTENT_LIST_SWITCH_CONTENT_LIST_SWITCH_CONTENT              162 // switch_content_list -> switch_content_list switch_content
#define SWITCH_CONTENT_LIST_SWITCH_CONTENT                                  163 // switch_content_list -> switch_content
#define SWITCH_CONTENT_CASE_EXP_STMT_LIST                                   164 // switch_content -> "case" exp ":" stmt_list
#define SWITCH_CONTENT_DEFAULT_STMT_LIST                                    165 // switch_content -> "default" ":" stmt_list
#define BREAK_DESC_BREAK                                                    166 // break_desc -> "break"
#define CONTINUE_DESC_CONTINUE                                              167 // continue_desc -> "continue"
#define RETURN_DESC_BLOCK                                                   168 // return_desc -> "return" block
#define RETURN_DESC_EXP                                                     169 // return_desc -> "return" exp ";"
#define EXP_GREATER_EQUAL_EXP1                                              170 // exp -> exp ">" "=" exp1
#define EXP_LESS_EQUAL                                                      171 // exp -> exp "<" "=" exp1
#define EXP_EQUAL                                                           172 // exp -> exp "=" "=" exp1
#define EXP_GREATER                                                         173 // exp -> exp ">" exp1
#define EXP_LESS                                                            174 // exp -> exp "<" exp1
#define EXP_ASSIGN                                                          175 // exp -> exp "=" exp1
#define EXP_LOGIC_AND                                                       176 // exp -> exp "&" "&" exp1
#define EXP_LOGIC_OR                                                        177 // exp -> exp "|" "|" exp1
#define EXP_BIT_AND                                                         178 // exp -> exp "&" exp1
#define EXP_BIT_OR                                                          179 // exp -> exp "|" exp1
#define EXP_BIT_XOR                                                         180 // exp -> exp "^" exp1
#define EXP_EXP_STMT_STMT                                                   181 // exp -> exp "?" exp ":" exp
#define EXP_EXP1                                                            182 // exp -> exp1
#define EXP_NOT                                                             183 // exp1 -> "!" exp2
#define EXP1_POSITIVE                                                       184 // exp1 -> "+" exp2
#define EXP1_NEGATIVE                                                       185 // exp1 -> "-" exp2
#define EXP1_EXP2                                                           186 // exp1 -> exp2
#define EXP2_ADD                                                            187 // exp2 -> exp2 "+" exp3
#define EXP2_SUB                                                            188 // exp2 -> exp2 "-" exp3
#define EXP2_EXP3                                                           189 // exp2 -> exp3
#define EXP3_MUL                                                            190 // exp3 -> exp3 "*" exp4
#define EXP3_DIV                                                            191 // exp3 -> exp3 "/" exp4
#define EXP3_MOD                                                            192 // exp3 -> exp3 "%" exp4
#define EXP3_EXP4                                                           193 // exp3 -> exp4
#define EXP4_BRACKETS                                                       194 // exp4 -> "(" exp ")"
#define EXP4_CALL_LIST                                                      195 // exp4 -> call_list
#define EXP4_VALUE_DESC_AS_TYPE_DESC                                        196 // exp4 -> value_desc "as" type_desc
#define EXP4_VALUE_DESC                                                     197 // exp4 -> value_desc

    class Parser : public BasicParser
    {
        enum
        {
            maxRegisterCount = UCHAR_MAX + 1,
            maxConstantCount = USHRT_MAX + 1
        };
    public:
        struct ContextInfo
        {
            enum Type
            {
                GlobalFunction,
                FunctionDeclare,
                Function,
                Block
            }type;

            HASH_KEY_TYPE hash;

            // 每个语句块或函数都有0-255个寄存器
            // used && hash的索引
            // 若是一个临时对象则hash为-1
            pair<bool, HASH_KEY_TYPE> reg[maxRegisterCount];

            ContextInfo(Type type, const HASH_KEY_TYPE& hash);
        };

    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);

        void print(ostream& stream);

        inline bool make()
        {
            if (syntaxTreeStack.size() == 1) return syntaxTreeStack.top()->make(this);
            return false;
        }
    protected:
        bool reduceType1Size(ushort i);
        bool reduceType2Size(ushort i);
        bool reduceMember2Size();
        bool reduceMember1Size();
        bool reduceValueNormal(ushort i);
        bool reduceValueContent(ushort i);
        bool reduceValuesTop();
        bool reduceValuesList2Size();
        bool reduceValuesList1Size();
        bool reduceArrayValue();
        bool reduceValueList2Size();
        bool reduceValueList1Size();
        bool reduceAttribute(ushort i);
        bool reduceArrayLst2Size(ushort i);
        bool reduceArrayLst1Size(ushort i);
        bool reduceParamterList2Size();
        bool reduceParamterList1Size();
        bool reduceParamterNamedArray();
        bool reduceParamterNamed();
        bool reduceParamterNoName();
        bool reduceItemList2Size();
        bool reduceItemList1Size();
        bool reduceItem(ushort i);
        bool reduceStmtList2Size(ushort i);
        bool reduceStmtList1Size(ushort i);
        bool reduceBlockStmts();
        bool reduceBlockEmpty();
        bool reduceTemplateWithParamters();
        bool reduceTemplateNoParamters();
        bool reduceTemplateList2Size();
        bool reduceTemplateList1Size();
        bool reduceTemplateItemWithTypeName();
        bool reduceTemplateItemWithType();
        bool reduceGlobalFunctionTemplateTypeParamters();
        bool reduceGlobalFunctionTypeParamters();
        bool reduceGlobalFunctionTemplateType();
        bool reduceGlobalFunctionType();
        bool reduceGlobalFunctionTemplateVoidParamters();
        bool reduceGlobalFunctionVoidParamters();
        bool reduceGlobalFunctionTemplateVoid();
        bool reduceGlobalFunctionVoid();
        bool reduceFunctionDeclareTemplateTypeParamters();
        bool reduceFunctionDeclareTypeParamters();
        bool reduceFunctionDeclareTemplateType();
        bool reduceFunctionDeclareType();
        bool reduceFunctionDeclareTemplateVoidParamters();
        bool reduceFunctionDeclareVoidParamters();
        bool reduceFunctionDeclareTemplateVoid();
        bool reduceFunctionDeclareVoid();
        bool reduceInterfaceWidthContent();
        bool reduceInterfaceNoContent();
        bool reduceInterfaceContent2Size();
        bool reduceInterfaceContent1Size();
        bool reduceClass1();
        bool reduceClass2();
        bool reduceClass3();
        bool reduceClass4();
        bool reduceClass5();
        bool reduceClass6();
        bool reduceClass7();
        bool reduceClass8();
        bool reduceClassWithName();
        bool reduceClassWithoutName();
        bool reduceClassImplement();
        bool reduceClassExtend();
        bool reduceClassContentList2Size();
        bool reduceClassContentList1Size();
        bool reduceClassContentFunction();
        bool reduceClassContentDeclareWithAttribute();
        bool reduceClassContentDeclareNoAttribute();
        bool reduceFunctionTemplateAttributeTypeParamters();
        bool reduceFunctionAttributeTypeParamters();
        bool reduceFunctionTemplateTypeParamters();
        bool reduceFunctionTypeParamters();
        bool reduceFunctionTemplateAttributeType();
        bool reduceFunctionAttributeType();
        bool reduceFunctionTemplateType();
        bool reduceFunctionType();
        bool reduceFunctionTemplateAttributeVoidParamters();
        bool reduceFunctionAttributeVoidParamters();
        bool reduceFunctionTemplateVoidParamters();
        bool reduceFunctionVoidParamters();
        bool reduceFunctionTemplateAttributeVoid();
        bool reduceFunctionAttributeVoid();
        bool reduceFunctionTemplateVoid();
        bool reduceFunctionVoid();
        bool reduceStmt(ushort i);
        bool reduceStmtNoSemicolon(ushort i);
        bool reduceDeclareWithAssign(ushort i);
        bool reduceDeclareWithoutAssign(ushort i);
        bool reduceAssignAddEqual();
        bool reduceAssignSubEqual();
        bool reduceAssignAndEqual();
        bool reduceAssignOrEqual();
        bool reduceAssignXorEqual();
        bool reduceAssignLeftMoveEqual();
        bool reduceAssignRightMoveEqual();
        bool reduceAssignEqual();
        bool reduceCallListValueList();
        bool reduceCallList();
        bool reduceCallListCall();
        bool reduceCall1();
        bool reduceCall2();
        bool reduceIfWithStmtElse();
        bool reduceIfWithStmt();
        bool reduceIfWithBlockElse();
        bool reduceIfWithBlock();
        bool reduceElse(ushort i);
        bool reduceFor();
        bool reduceWhile();
        bool reduceDo();
        bool reduceSwitch();
        bool reduceSwitchContentList2Size();
        bool reduceSwitchContentList1Size();
        bool reduceSwitchContentCase();
        bool reduceSwitchContentDefault();
        bool reduceBreak();
        bool reduceContinue();
        bool reduceReturnBlock();
        bool reduceReturnExp();
        bool reduceExp3Size();
        bool reduceExp2Size(ushort i);
        bool reduceExp1Size(ushort i);
        bool reduceExpCall();
        bool reduceExpValueAsType();
        bool reduceExpValue();

        inline bool pop1Shifts()
        {
            shifts.pop();
            return true;
        }

        inline bool pop2Shifts()
        {
            shifts.pop();
            shifts.pop();
            return true;
        }
    protected:
        struct
        {
            hashset<SyntaxTree_Base*> data;
        }context;
        stack<SyntaxTree_Base*> syntaxTreeStack;
        stack<string>           shifts;

        list<VM::Instruction>   instructions;
        stack<ContextInfo>      makeContext;

        VM::Variant*            pConstantTable;
        size_t                  constantCount;
#if defined(_DEBUG ) && DEBUG_LEVEL == 3
        fstream                  result;
#endif
    };
}

#endif
