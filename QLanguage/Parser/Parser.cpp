﻿/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:35
	filename: 	\QLanguage\Parser\Parser.cpp
	file path:	\QLanguage\Parser
	file base:	Parser
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "SyntaxTree/SyntaxTree_Type.h"

#include "Parser.h"

namespace QLanguage
{
    Parser::Parser(const vector<Production>& productions)
        : BasicParser(productions)
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        , result("Result.txt", fstream::out | fstream::text)
#endif
    {
    }

    Parser::~Parser()
    {
        for (hashset<SyntaxTree_Base*>::const_iterator i = context.data.begin(), m = context.data.end(); i != m; ++i)
        {
            destruct(*i, has_destruct(**i));
            allocator<SyntaxTree_Base>::deallocateWithSize(*i, (*i)->size());
        }
    }

    bool Parser::shift(const string& s)
    {
        shifts.push(s);
        return true;
    }

    bool Parser::reduce(ushort i)
    {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        result << "reduce by rule: " << i << endl;
#endif
        switch (i)
        {
        case 0:   // begin -> start
            cout << "finish";
            break;
        case TYPE_DESC_UNSIGNED_CHAR:          // type_desc -> "unsigned" "char"
        case TYPE_DESC_UNSIGNED_SHORT:         // type_desc -> "unsigned" "short"
        case TYPE_DESC_UNSIGNED_INT:           // type_desc -> "unsigned" "int"
        case TYPE_DESC_UNSIGNED_LONG:          // type_desc -> "unsigned" "long"
            return reduceType2Size(i);
        case TYPE_DESC_CHAR:                   // type_desc -> "char"
        case TYPE_DESC_SHORT:                  // type_desc -> "short"
        case TYPE_DESC_INT:                    // type_desc -> "int"
        case TYPE_DESC_LONG:                   // type_desc -> "long"
        case TYPE_DESC_FLOAT:                  // type_desc -> "float"
        case TYPE_DESC_DOUBLE:                 // type_desc -> "double"
        case TYPE_DESC_STRING:                 // type_desc -> "string"
        case TYPE_DESC_AUTO:                   // type_desc -> "auto"
        case TYPE_DESC_LETTER:                 // type_desc -> "{Letter}"
            return reduceType1Size(i);
        case MEMBER_DESC_MEMBER_DESC_LETTER:   // member_desc -> member_desc "." "{Letter}"
            return reduceMember2Size();
        case MEMBER_DESC_LETTER:               // member_desc -> "{Letter}"
            return reduceMember1Size();
        case VALUE_DESC_TRUE:                  // value_desc -> "true"
        case VALUE_DESC_FALSE:                 // value_desc -> "false"
        case VALUE_DESC_REAL:                  // value_desc -> "{Real}"
        case VALUE_DESC_DIGIT:                 // value_desc -> "{Digit}"
        case VALUE_DESC_STRING:                // value_desc -> "{String}"
            return reduceValueNormal(i);
        case VALUE_DESC_MEMBER_DESC:           // value_desc -> member_desc
            return reduceValueMember();
        case VALUE_LIST_VALUE_LIST_EXP:        // value_list -> value_list "," exp
            return reduceValueList2Size();
        case VALUE_LIST_EXP:                   // value_list -> exp
            return reduceValueList1Size();
        case ATTRIBUTE_PUBLIC:                 // attribute -> "public"
        case ATTRIBUTE_PRIVATE:                // attribute -> "private"
        case ATTRIBUTE_PROTECTED:              // attribute -> "protected"
            return reduceAttribute(i);
        case PARAMTER_LIST_PARAMTER_LIST_PARAMTER: // paramter_list -> paramter_list "," paramter
            return reduceParamterList2Size();
        case PARAMTER_LIST_PARAMTER:               // paramter_list -> paramter
            return reduceParamterList1Size();
        case PARAMTER_TYPE_DESC_LETTER:        // paramter -> type_desc "{Letter}"
            return reduceParamterNamed();
        case PARAMTER_TYPE_DESC:               // paramter -> type_desc
            return reduceParamterNoName();
        case ITEM_LIST_ITEM_LIST_ITEM:         // item_list -> item_list item
            return reduceItemList2Size();
        case ITEM_LIST_ITEM:                   // item_list -> item
            return reduceItemList1Size();
        case ITEM_GLOBAL_FUNCTION_DESC:        // item -> global_function_desc
        case ITEM_DECLARE_DESC:                // item -> declare_desc ";"
        case ITEM_INTERFACE_DESC:              // item -> interface_desc
        case ITEM_CLASS_DESC:                  // item -> class_desc
            return reduceItem(i);
        case STMT_LIST_STMT_LIST_STMT:         // stmt_list -> stmt_list stmt
        case STMT_LIST_STMT_LIST_BLOCK:        // stmt_list -> stmt
            return reduceStmtList2Size();
        case STMT_LIST_STMT:                   // stmt_list -> stmt_list block
        case STMT_LIST_BLOCK:                  // stmt_list -> block
            return reduceStmtList1Size();
        case BLOCK_STMT_LIST:                  // block -> "{" stmt_list "}"
            return reduceBlockStmts();
        case BLOCK_EMPTY:                      // block -> "{" "}"
            return reduceBlockEmpty();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK: // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction2();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK:    // global_function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunction4();
        case GLOBAL_FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:      // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction6();
        case GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK:         // global_function_desc -> "void" "{Letter}" "(" ")" block
            return reduceGlobalFunction8();
        case FUNCTION_DESC_TYPE_DESC_PARAM_LIST:              // function_desc -> type_desc "{Letter}" "(" paramter_list ")" ";"
            return reduceGlobalFunction2();
        case FUNCTION_DESC_TYPE_DESC_NOPARAM:                 // function_desc -> type_desc "{Letter}" "(" ")" ";"
            return reduceGlobalFunction4();
        case FUNCTION_DESC_VOID_PARAM_LIST:                   // function_desc -> "void" "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclare6();
        case FUNCTION_DESC_VOID_NOPARAM:                      // function_desc -> "void" "{Letter}" "(" ")" ";"
            return reduceFunctionDeclare8();
        case STMT_CALL:                        // stmt -> call_desc ";"
        case STMT_DECLARE:                     // stmt -> declare_desc ";"
            return pop1Shifts();
        case DECLARE_DESC_DECLARE_DESC_LETTER: // declare_desc -> declare_desc "," "{Letter}"
        case DECLARE_DESC_LETTER:              // declare_desc -> type_desc "{Letter}"
            return reduceDeclare48(i);
        case CALL_DESC_VALUE_LIST:             // call_desc -> member_desc "(" value_list ")"
            return reduceCall1();
        case CALL_DESC_NOPARAM:                // call_desc -> member_desc "(" ")"
            return reduceCall2();
        case RETURN_DESC_EXP:                  // return_desc -> "return" exp ";"
            return reduceReturnExp();
        }
        return true;
    }
}