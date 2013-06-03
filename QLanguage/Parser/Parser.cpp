/********************************************************************
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
    Parser::Parser(const vector<Production>& productions) : BasicParser(productions)
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
        case VALUE_LIST_VALUE_LIST_EXP:        // value_list -> value_list "," exp
            return reduceValueList2Size();
        case VALUE_LIST_EXP:                   // value_list -> exp
            return reduceValueList1Size();
        case ITEM_LIST_ITEM_LIST_ITEM:         // item_list -> item_list item
            return reduceItemList2Size();
        case ITEM_LIST_ITEM:                   // item_list -> item
            return reduceItemList1Size();
        case ITEM_DECLARE_DESC:                // item -> declare_desc ";"
            return pop1Shifts();
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
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK: // global_function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunction4();
        case GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK:
            return reduceGlobalFunction8();
        case STMT_DECLARE:                     // stmt -> declare_desc ";"
            return pop1Shifts();
        case DECLARE_DESC_DECLARE_DESC_LETTER: // declare_desc -> declare_desc "," "{Letter}"
        case DECLARE_DESC_LETTER:              // declare_desc -> type_desc "{Letter}"
            return reduceDeclare48(i);
        case RETURN_DESC_EXP:                  // return_desc -> "return" exp ";"
            return reduceReturnExp();
        }
        return true;
    }
}