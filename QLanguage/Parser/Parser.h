﻿/********************************************************************
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
#define TYPE_DESC_UNSIGNED_CHAR                                  1   // type_desc -> "unsigned" "char"
#define TYPE_DESC_CHAR                                           2   // type_desc -> "char"
#define TYPE_DESC_UNSIGNED_SHORT                                 3   // type_desc -> "unsigned" "short"
#define TYPE_DESC_SHORT                                          4   // type_desc -> "short"
#define TYPE_DESC_UNSIGNED_INT                                   5   // type_desc -> "unsigned" "int"
#define TYPE_DESC_INT                                            6   // type_desc -> "int"
#define TYPE_DESC_UNSIGNED_LONG                                  7   // type_desc -> "unsigned" "long"
#define TYPE_DESC_LONG                                           8   // type_desc -> "long"
#define TYPE_DESC_FLOAT                                          9   // type_desc -> "float"
#define TYPE_DESC_DOUBLE                                         10  // type_desc -> "double"
#define TYPE_DESC_STRING                                         11  // type_desc -> "string"
#define TYPE_DESC_AUTO                                           12  // type_desc -> "auto"
#define TYPE_DESC_LETTER                                         13  // type_desc -> "{Letter}"
#define MEMBER_DESC_MEMBER_DESC_LETTER                           14  // member_desc -> member_desc "." "{Letter}"
#define MEMBER_DESC_LETTER                                       15  // member_desc -> "{Letter}"
#define VALUE_DESC_TRUE                                          16  // value_desc -> "true"
#define VALUE_DESC_FALSE                                         17  // value_desc -> "false"
#define VALUE_DESC_REAL                                          18  // value_desc -> "{Real}"
#define VALUE_DESC_DIGIT                                         19  // value_desc -> "{Digit}"
#define VALUE_DESC_MEMBER_DESC                                   20  // value_desc -> member_desc
#define VALUE_DESC_STRING                                        21  // value_desc -> "{String}"
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
#define FUNCTION_DESC_TYPE_DESC_PARAM_LIST                       69  // function_desc -> type_desc "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DESC_TYPE_DESC_NOPARAM                          71  // function_desc -> type_desc "{Letter}" "(" ")" ";"
#define FUNCTION_DESC_VOID_PARAM_LIST                            73  // function_desc -> "void" "{Letter}" "(" paramter_list ")" ";"
#define FUNCTION_DESC_VOID_NOPARAM                               75  // function_desc -> "void" "{Letter}" "(" ")" ";"
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
#define CLASS_CONTENT_CLASS_CONTENT_FUNCTION_DESC                92  // class_content -> class_content function_desc
#define CLASS_CONTENT_FUNCTION_DESC                              93  // class_content -> function_desc
#define CLASS_CONTENT_CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC       94  // class_content -> class_content attribute declare_desc ";"
#define CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC                     95  // class_content -> attribute declare_desc ";"
#define CLASS_CONTENT_CLASS_CONTENT_DECLARE_DESC                 96  // class_content -> class_content declare_desc ";"
#define CLASS_CONTENT_DECLARE_DESC                               97  // class_content -> declare_desc ";"
#define STMT_CALL                                                115 // stmt -> call_desc ";"
#define STMT_DECLARE                                             116 // stmt -> declare_desc ";"
#define DECLARE_DESC_DECLARE_DESC_LETTER                         125 // declare_desc -> declare_desc "," "{Letter}"
#define DECLARE_DESC_LETTER                                      129 // declare_desc -> type_desc "{Letter}"
#define CALL_DESC_VALUE_LIST                                     138 // call_desc -> member_desc "(" value_list ")"
#define CALL_DESC_NOPARAM                                        139 // call_desc -> member_desc "(" ")"
#define RETURN_DESC_EXP                                          150 // return_desc -> "return" exp ";"

    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);
    protected:
        bool reduceType1Size(ushort i);
        bool reduceType2Size(ushort i);
        bool reduceMember2Size();
        bool reduceMember1Size();
        bool reduceValueNormal(ushort i);
        bool reduceValueMember();
        bool reduceValueList2Size();
        bool reduceValueList1Size();
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
        bool reduceClassContent2Size(ushort i);
        bool reduceClassContent1Size(ushort i);
        bool reduceDeclare48(ushort i);
        bool reduceCall1();
        bool reduceCall2();
        bool reduceReturnExp();

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
