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

#include "../../QCore/Library/hashset.h"

#include "SyntaxTree/SyntaxTree_Type.h"
#include "BasicParser.h"

namespace QLanguage
{
#define MEMBER_DESC_MEMBER_DESC_LETTER                           1   // member_desc -> member_desc "." "{Letter}"
#define MEMBER_DESC_LETTER                                       2   // member_desc -> "{Letter}"
#define TYPE_DESC_UNSIGNED_CHAR                                  3   // type_desc -> "unsigned" "char"
#define TYPE_DESC_CHAR                                           4   // type_desc -> "char"
#define TYPE_DESC_UNSIGNED_SHORT                                 5   // type_desc -> "unsigned" "short"
#define TYPE_DESC_SHORT                                          6   // type_desc -> "short"
#define TYPE_DESC_UNSIGNED_INT                                   7   // type_desc -> "unsigned" "int"
#define TYPE_DESC_INT                                            8   // type_desc -> "int"
#define TYPE_DESC_UNSIGNED_LONG                                  9   // type_desc -> "unsigned" "long"
#define TYPE_DESC_LONG                                           10  // type_desc -> "long"
#define TYPE_DESC_FLOAT                                          11  // type_desc -> "float"
#define TYPE_DESC_DOUBLE                                         12  // type_desc -> "double"
#define TYPE_DESC_STRING                                         13  // type_desc -> "string"
#define TYPE_DESC_AUTO                                           14  // type_desc -> "auto"
#define TYPE_DESC_LETTER                                         15  // type_desc -> "{Letter}"
#define VALUE_DESC_TRUE                                          16  // value_desc -> "true"
#define VALUE_DESC_FALSE                                         17  // value_desc -> "false"
#define VALUE_DESC_REAL                                          18  // value_desc -> "{Real}"
#define VALUE_DESC_DIGIT                                         19  // value_desc -> "{Digit}"
#define VALUE_DESC_MEMBER_DESC                                   20  // value_desc -> member_desc
#define VALUE_DESC_STRING                                        21  // value_desc -> "{String}"
#define VALUES_VALUES                                            23  // values -> "{" values "}"
#define VALUES_VALUES_VALUE_DESC                                 24  // values -> values "," value_desc
#define VALUES_VALUE_DESC                                        25  // values -> value_desc
#define ARRAY_VALUE_DESC_VALUES                                  26  // array_value_desc -> "{" values "}"
#define VALUE_LIST_VALUE_LIST_EXP                                27  // value_list -> value_list "," exp
#define VALUE_LIST_EXP                                           28  // value_list -> exp
#define ATTRIBUTE_PUBLIC                                         29  // attribute -> "public"
#define ATTRIBUTE_PRIVATE                                        30  // attribute -> "private"
#define ATTRIBUTE_PROTECTED                                      31  // attribute -> "protected"
#define PARAMTER_LIST_PARAMTER_LIST_PARAMTER                     37  // paramter_list -> paramter_list "," paramter
#define PARAMTER_LIST_PARAMTER                                   38  // paramter_list -> paramter
#define PARAMTER_TYPE_DESC_LETTER                                40  // paramter -> type_desc "{Letter}"
#define PARAMTER_TYPE_DESC                                       41  // paramter -> type_desc
#define ITEM_LIST_ITEM_LIST_ITEM                                 42  // item_list -> item_list item
#define ITEM_LIST_ITEM                                           43  // item_list -> item
#define ITEM_GLOBAL_FUNCTION_DESC                                44  // item -> global_function_desc
#define ITEM_DECLARE_DESC                                        45  // item -> declare_desc ";"
#define ITEM_INTERFACE_DESC                                      46  // item -> interface_desc
#define ITEM_CLASS_DESC                                          47  // item -> class_desc
#define STMT_LIST_STMT_LIST_STMT                                 48  // stmt_list -> stmt_list stmt
#define STMT_LIST_STMT                                           49  // stmt_list -> stmt
#define STMT_LIST_STMT_LIST_BLOCK                                50  // stmt_list -> stmt_list block
#define STMT_LIST_BLOCK                                          51  // stmt_list -> block
#define BLOCK_STMT_LIST                                          52  // block -> "{" stmt_list "}"
#define BLOCK_EMPTY                                              53  // block -> "{" "}"
#define GLOBAL_FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK          61  // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
#define GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK             63  // global_function_desc -> type_desc "{Letter}" "(" ")" block
#define GLOBAL_FUNCTION_DESC_VOID_PARAM_LIST_BLOCK               65  // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
#define GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK                  67  // global_function_desc -> "void" "{Letter}" "(" ")" block
#define FUNCTION_DECLARE_TYPE_DESC_PARAM_LIST                    69  // function_declare -> type_desc "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DECLARE_TYPE_DESC_NOPARAM                       71  // function_declare -> type_desc "{Letter}" "(" ")" ";"
#define FUNCTION_DECLARE_VOID_PARAM_LIST                         73  // function_declare -> "void" "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DECLARE_VOID_NOPARAM                            75  // function_declare -> "void" "{Letter}" "(" ")" ";"
#define INTERFACE_DESC_INTERFACE_CONTENT_LIST                    76  // interface_desc -> "interface" "{Letter}" "{" interface_content "}"
#define INTERFACE_DESC_NO_INTERFACE_CONTENT_LIST                 77  // interface_desc -> "interface" "{Letter}" "{" "}"
#define INTERFACE_CONTENT_INTERFACE_CONTENT_FUNCTION_DECLARE     78  // interface_content -> interface_content function_declare
#define INTERFACE_CONTENT_FUNCTION_DECLARE                       79  // interface_content -> function_declare
#define CLASS_DESC_CLASS_DESC1_CLASS_DESC2_CLASS_CONTENT_LIST    81  // class_desc -> class_desc1 class_desc2 "{" class_content "}"
#define CLASS_DESC_CLASS_DESC1_CLASS_CONTENT_LIST                83  // class_desc -> class_desc1 "{" class_content "}"
#define CLASS_DESC_CLASS_DESC1_CLASS_DESC2_NO_CLASS_CONTENT_LIST 85  // class_desc -> class_desc1 class_desc2 "{" "}"
#define CLASS_DESC_CLASS_DESC1_NO_CLASS_CONTENT_LIST             87  // class_desc -> class_desc1 "{" "}"
#define CLASS_DESC1_CLASS_LETTER                                 88  // class_desc1 -> "class" "{Letter}"
#define CLASS_DESC1_CLASS                                        89  // class_desc1 -> "class"
#define CLASS_DESC2_ATTRIBUTE_IMPLEMENT_LETTER                   90  // class_desc2 -> attribute "implement" "{Letter}"
#define CLASS_DESC2_ATTRIBUTE_EXTEND_LETTER                      91  // class_desc2 -> attribute "extend" "{Letter}"
#define CLASS_CONTENT_LIST_CLASS_CONTENT_LIST_CLASS_CONTENT      92  // class_content_list -> class_content_list class_content
#define CLASS_CONTENT_LIST_CLASS_CONTENT                         93  // class_content_list -> class_content
#define CLASS_CONTENT_FUNCTION_DESC                              94  // class_content -> function_desc
#define CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC                     95  // class_content -> attribute declare_desc ";"
#define CLASS_CONTENT_DECLARE_DESC                               96  // class_content -> declare_desc ";"
#define FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK       98  // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK                 100 // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_NOPARAM_BLOCK          102 // function_desc -> attribute type_desc "{Letter}" "(" ")" block
#define FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK                    104 // function_desc -> type_desc "{Letter}" "(" ")" block
#define FUNCTION_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK            106 // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_VOID_PARAM_LIST_BLOCK                      108 // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
#define FUNCTION_DESC_ATTRIBUTE_VOID_NOPARAM_BLOCK               110 // function_desc -> attribute "void" "{Letter}" "(" ")" block
#define FUNCTION_DESC_VOID_NOPARAM_BLOCK                         112 // function_desc -> "void" "{Letter}" "(" ")" block
#define STMT_ASSIGN_DESC                                         113 // stmt -> assign_desc ";"
#define STMT_CALL_DESC                                           114 // stmt -> call_desc ";"
#define STMT_DECLARE_DESC                                        115 // stmt -> declare_desc ";"
#define STMT_IF_DESC                                             116 // stmt -> if_desc
#define STMT_FOR_DESC                                            117 // stmt -> for_desc
#define STMT_WHILE_DESC                                          118 // stmt -> while_desc
#define STMT_DO_DESC                                             119 // stmt -> do_desc ";"
#define STMT_RETURN_DESC                                         120 // stmt -> return_desc
#define STMT_NO_SEMICOLON_ASSIGN_DESC                            121 // stmt_no_semicolon -> assign_desc
#define STMT_NO_SEMICOLON_CALL_DESC                              122 // stmt_no_semicolon -> call_desc
#define STMT_NO_SEMICOLON_DECLARE_DESC                           123 // stmt_no_semicolon -> declare_desc
#define STMT_NO_SEMICOLON_IF_DESC                                124 // stmt_no_semicolon -> if_desc
#define STMT_NO_SEMICOLON_FOR_DESC                               125 // stmt_no_semicolon -> for_desc
#define STMT_NO_SEMICOLON_WHILE_DESC                             126 // stmt_no_semicolon -> while_desc
#define STMT_NO_SEMICOLON_DO_DESC                                127 // stmt_no_semicolon -> do_desc
#define DECLARE_DESC_DECLARE_DESC_TYPE_DESC_LETTER_EQUAL_EXP     129 // declare_desc -> declare_desc "," "{Letter}" "=" exp
#define DECLARE_DESC_DECLARE_DESC_TYPE_DESC_LETTER               131 // declare_desc -> declare_desc "," "{Letter}"
#define DECLARE_DESC_LETTER_TYPE_DESC_LETTER_EQUAL_EXP           133 // declare_desc -> ttype_desc "{Letter}" "=" exp
#define DECLARE_DESC_TYPE_DESC_LETTER                            135 // declare_desc -> type_desc "{Letter}"
#define ASSIGN_DESC_MEMBER_DESC_ADD_EQUAL_EXP                    136 // assign_desc -> member_desc "+" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_SUB_EQUAL_EXP                    137 // assign_desc -> member_desc "-" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_AND_EQUAL_EXP                    138 // assign_desc -> member_desc "&" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_OR_EQUAL_EXP                     139 // assign_desc -> member_desc "|" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_XOR_EQUAL_EXP                    140 // assign_desc -> member_desc "^" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_LEFT_MOVE_EQUAL_EXP              141 // assign_desc -> member_desc "<" "<" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_RIGHT_MOVE_EQUAL_EXP             142 // assign_desc -> member_desc ">" ">" "=" exp
#define ASSIGN_DESC_MEMBER_DESC_EQUAL_EXP                        143 // assign_desc -> member_desc "=" exp
#define CALL_DESC_VALUE_LIST                                     144 // call_desc -> member_desc "(" value_list ")"
#define CALL_DESC_NOPARAM                                        145 // call_desc -> member_desc "(" ")"
#define IF_DESC_IF_EXP_STMT_ELSE_DESC                            146 // if_desc -> "if" "(" exp ")" stmt else_desc
#define IF_DESC_IF_EXP_STMT                                      147 // if_desc -> "if" "(" exp ")" stmt
#define IF_DESC_IF_EXP_BLOCK_ELSE_DESC                           148 // if_desc -> "if" "(" exp ")" block else_desc
#define IF_DESC_IF_EXP_BLOCK                                     149 // if_desc -> "if" "(" exp ")" block
#define ELSE_DESC_ELSE_STMT                                      150 // else_desc -> "else" stmt
#define ELSE_DESC_ELSE_BLOCK                                     151 // else_desc -> "else" block
#define FOR_DESC_FOR_STMT_EXP_STMT_BLOCK                         152 // for_desc -> "for" "(" stmt ";" exp ";" stmt ")" block
#define WHILE_DESC_WHILE_EXP_BLOCK                               153 // while_desc -> "while" "(" exp ")" block
#define DO_DESC_DO_BLOCK_WHILE_EXP                               154 // do_desc -> "do" block "while" "(" exp ")"
#define RETURN_DESC_EXP                                          156 // return_desc -> "return" exp ";"
#define EXP_GREATER_EQUAL                                        157 // exp -> exp ">" "=" exp1
#define EXP_LESS_EQUAL                                           158 // exp -> exp "<" "=" exp1
#define EXP_EQUAL                                                159 // exp -> exp "=" "=" exp1
#define EXP_GREATER                                              160 // exp -> exp ">" exp1
#define EXP_LESS                                                 161 // exp -> exp "<" exp1
#define EXP_ASSIGN                                               162 // exp -> exp "=" exp1
#define EXP_LOGIC_AND                                            163 // exp -> exp "&" "&" exp1
#define EXP_LOGIC_OR                                             164 // exp -> exp "|" "|" exp1
#define EXP_BIT_AND                                              165 // exp -> exp "&" exp1
#define EXP_BIT_OR                                               166 // exp -> exp "|" exp1
#define EXP_BIT_XOR                                              167 // exp -> exp "^" exp1
#define EXP_EXP_STMT_STMT                                        168 // exp -> exp "?" exp ":" exp
#define EXP_NOT                                                  170 // exp1 -> "!" exp2
#define EXP_POSITIVE                                             171 // exp1 -> "+" exp2
#define EXP_NEGATIVE                                             172 // exp1 -> "-" exp2
#define EXP_ADD                                                  174 // exp2 -> exp2 "+" exp3
#define EXP_SUB                                                  175 // exp2 -> exp2 "-" exp3
#define EXP_MUL                                                  177 // exp3 -> exp3 "*" exp4
#define EXP_DIV                                                  178 // exp3 -> exp3 "/" exp4
#define EXP_MOD                                                  179 // exp3 -> exp3 "%" exp4
#define EXP_BRACKETS                                             181 // exp4 -> "(" exp ")"
#define EXP_CALL_DESC                                            182 // exp4 -> call_desc
#define EXP_VALUE_DESC_AS_TYPE_DESC                              183 // exp4 -> value_desc "as" type_desc
#define EXP_VALUE_DESC                                           184 // exp4 -> value_desc

    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);

        void print(ostream& stream);
    protected:
        bool reduceType1Size(ushort i);
        bool reduceType2Size(ushort i);
        bool reduceMember2Size();
        bool reduceMember1Size();
        bool reduceValueNormal(ushort i);
        bool reduceValueMember();
        bool reduceValueList2Size();
        bool reduceValueList1Size();
        bool reduceValuesTop();
        bool reduceValues2Size();
        bool reduceValues1Size();
        bool reduceAttribute(ushort i);
        bool reduceParamterList2Size();
        bool reduceParamterList1Size();
        bool reduceParamterNamed();
        bool reduceParamterNoName();
        bool reduceItemList2Size();
        bool reduceItemList1Size();
        bool reduceItem(ushort i);
        bool reduceStmtList2Size();
        bool reduceStmtList1Size();
        bool reduceBlockStmts();
        bool reduceBlockEmpty();
        bool reduceGlobalFunction2();
        bool reduceGlobalFunction4();
        bool reduceGlobalFunction6();
        bool reduceGlobalFunction8();
        bool reduceFunctionDeclare2();
        bool reduceFunctionDeclare4();
        bool reduceFunctionDeclare6();
        bool reduceFunctionDeclare8();
        bool reduceInterfaceWidthContent();
        bool reduceInterfaceNoContent();
        bool reduceInterfaceContent2Size();
        bool reduceInterfaceContent1Size();
        bool reduceClass2();
        bool reduceClass4();
        bool reduceClass6();
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
        bool reduceFunction2();
        bool reduceFunction4();
        bool reduceFunction6();
        bool reduceFunction8();
        bool reduceFunction10();
        bool reduceFunction12();
        bool reduceFunction14();
        bool reduceFunction16();
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
        bool reduceCall1();
        bool reduceCall2();
        bool reduceIfWithStmtElse();
        bool reduceIfWithStmt();
        bool reduceIfWithBlockElse();
        bool reduceIfWithBlock();
        bool reduceFor();
        bool reduceWhile();
        bool reduceDo();
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
        stack<SyntaxTree_Base*>  syntaxTreeStack;
        stack<string>            shifts;
#if defined(_DEBUG ) && DEBUG_LEVEL == 3
        fstream                  result;
#endif
    };
}

#endif
