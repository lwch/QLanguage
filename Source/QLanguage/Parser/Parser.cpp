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

#include "../../QCore/Library/almighty_container.h"

    enum AA
    {
        A, B
    };

    template <typename T>
    inline void t(const T& p)
    {
        t(QLanguage::Library::almighty_container<const T&>(p));
    }

    template <typename T>
    inline void t(T& p)
    {
        t(QLanguage::Library::almighty_container<T&>(p));
    }

    template <typename T>
    inline void t(T* p)
    {
        t(QLanguage::Library::almighty_container<T*>(p));
    }

    template <typename T>
    inline void t(const QLanguage::Library::almighty_container<T>& p)
    {
        printf("");
    }

    bool Parser::shift(const string& s)
    {
        t(const_cast<string&>(s));
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
            cout << "finish" << endl;
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
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK:          // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction2();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK:             // global_function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunction4();
        case GLOBAL_FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:               // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction6();
        case GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK:                  // global_function_desc -> "void" "{Letter}" "(" ")" block
            return reduceGlobalFunction8();
        case FUNCTION_DECLARE_TYPE_DESC_PARAM_LIST:                    // function_desc -> type_desc "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclare2();
        case FUNCTION_DECLARE_TYPE_DESC_NOPARAM:                       // function_desc -> type_desc "{Letter}" "(" ")" ";"
            return reduceFunctionDeclare4();
        case FUNCTION_DECLARE_VOID_PARAM_LIST:                         // function_desc -> "void" "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclare6();
        case FUNCTION_DECLARE_VOID_NOPARAM:                            // function_desc -> "void" "{Letter}" "(" ")" ";"
            return reduceFunctionDeclare8();
        case INTERFACE_DESC_INTERFACE_CONTENT_LIST:                    // interface_desc -> "interface" "{Letter}" "{" interface_content "}"
            return reduceInterfaceWidthContent();
        case INTERFACE_DESC_NO_INTERFACE_CONTENT_LIST:                 // interface_desc -> "interface" "{Letter}" "{" "}"
            return reduceInterfaceNoContent();
        case INTERFACE_CONTENT_INTERFACE_CONTENT_FUNCTION_DECLARE:     // interface_content -> interface_content function_declare
            return reduceInterfaceContent2Size();
        case INTERFACE_CONTENT_FUNCTION_DECLARE:                       // interface_content -> function_declare
            return reduceInterfaceContent1Size();
        case CLASS_DESC_CLASS_DESC1_CLASS_DESC2_CLASS_CONTENT_LIST:    // class_desc -> class_desc1 class_desc2 "{" class_content "}"
            return reduceClass2();
        case CLASS_DESC_CLASS_DESC1_CLASS_CONTENT_LIST:                // class_desc -> class_desc1 "{" class_content "}"
            return reduceClass4();
        case CLASS_DESC_CLASS_DESC1_CLASS_DESC2_NO_CLASS_CONTENT_LIST: // class_desc -> class_desc1 class_desc2 "{" "}"
            return reduceClass6();
        case CLASS_DESC_CLASS_DESC1_NO_CLASS_CONTENT_LIST:             // class_desc -> class_desc1 "{" "}"
            return reduceClass8();
        case CLASS_DESC1_CLASS_LETTER:               // class_desc1 -> "class" "{Letter}"
            return reduceClassWithName();
        case CLASS_DESC1_CLASS:                      // class_desc1 -> "class"
            return reduceClassWithoutName();
        case CLASS_DESC2_ATTRIBUTE_IMPLEMENT_LETTER: // class_desc2 -> attribute "implement" "{Letter}"
            return reduceClassImplement();
        case CLASS_DESC2_ATTRIBUTE_EXTEND_LETTER:    // class_desc2 -> attribute "extend" "{Letter}"
            return reduceClassExtend();
        case CLASS_CONTENT_CLASS_CONTENT_FUNCTION_DESC:          // class_content -> class_content function_desc
        case CLASS_CONTENT_CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC: // class_content -> class_content attribute declare_desc ";"
        case CLASS_CONTENT_CLASS_CONTENT_DECLARE_DESC:           // class_content -> class_content declare_desc ";"
            return reduceClassContent2Size(i);
        case CLASS_CONTENT_FUNCTION_DESC:            // class_content -> function_desc
        case CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC:   // class_content -> attribute declare_desc ";"
        case CLASS_CONTENT_DECLARE_DESC:             // class_content -> declare_desc ";"
            return reduceClassContent1Size(i);
        case FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK: // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunction2();
        case FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK:           // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunction4();
        case FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_NOPARAM_BLOCK:    // function_desc -> attribute type_desc "{Letter}" "(" ")" block
            return reduceFunction6();
        case FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK:              // function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceFunction8();
        case FUNCTION_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK:      // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunction10();
        case FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:                // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunction12();
        case FUNCTION_DESC_ATTRIBUTE_VOID_NOPARAM_BLOCK:         // function_desc -> attribute "void" "{Letter}" "(" ")" block
            return reduceFunction14();
        case FUNCTION_DESC_VOID_NOPARAM_BLOCK:                   // function_desc -> "void" "{Letter}" "(" ")" block
            return reduceFunction16();
        case STMT_ASSIGN_DESC:                       // stmt -> assign_desc ";"
        case STMT_CALL_DESC:                         // stmt -> call_desc ";"
        case STMT_DECLARE_DESC:                      // stmt -> declare_desc ";"
        case STMT_DO_DESC:                           // stmt -> do_desc ";"
            return pop1Shifts();
        case STMT_IF_DESC:                           // stmt -> if_desc
        case STMT_FOR_DESC:                          // stmt -> for_desc
        case STMT_WHILE_DESC:                        // stmt -> while_desc
        case STMT_RETURN_DESC:                       // stmt -> return_desc
            break;
        case DECLARE_DESC_DECLARE_DESC_LETTER:       // declare_desc -> declare_desc "," "{Letter}"
        case DECLARE_DESC_LETTER:                    // declare_desc -> type_desc "{Letter}"
            return reduceDeclare48(i);
        case ASSIGN_DESC_MEMBER_DESC_ADD_EQUAL_EXP:        // assign_desc -> member_desc "+" "=" exp
            return reduceAssignAddEqual();
        case ASSIGN_DESC_MEMBER_DESC_SUB_EQUAL_EXP:        // assign_desc -> member_desc "-" "=" exp
            return reduceAssignSubEqual();
        case ASSIGN_DESC_MEMBER_DESC_AND_EQUAL_EXP:        // assign_desc -> member_desc "&" "=" exp
            return reduceAssignAndEqual();
        case ASSIGN_DESC_MEMBER_DESC_OR_EQUAL_EXP:         // assign_desc -> member_desc "|" "=" exp
            return reduceAssignOrEqual();
        case ASSIGN_DESC_MEMBER_DESC_XOR_EQUAL_EXP:        // assign_desc -> member_desc "^" "=" exp
            return reduceAssignXorEqual();
        case ASSIGN_DESC_MEMBER_DESC_LEFT_MOVE_EQUAL_EXP:  // assign_desc -> member_desc "<" "<" "=" exp
            return reduceAssignLeftMoveEqual();
        case ASSIGN_DESC_MEMBER_DESC_RIGHT_MOVE_EQUAL_EXP: // assign_desc -> member_desc ">" ">" "=" exp
            return reduceAssignRightMoveEqual();
        case ASSIGN_DESC_MEMBER_DESC_EQUAL_EXP:            // assign_desc -> member_desc "=" exp
            return reduceAssignEqual();
        case CALL_DESC_VALUE_LIST:                   // call_desc -> member_desc "(" value_list ")"
            return reduceCall1();
        case CALL_DESC_NOPARAM:                      // call_desc -> member_desc "(" ")"
            return reduceCall2();
        case RETURN_DESC_EXP:                        // return_desc -> "return" exp ";"
            return reduceReturnExp();
        }
        return true;
    }
}