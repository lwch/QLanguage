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
            return reduceValueContent();
        case VALUE_LIST_VALUE_LIST_EXP:        // value_list -> value_list "," exp
            return reduceValueList2Size();
        case VALUE_LIST_EXP:                   // value_list -> exp
            return reduceValueList1Size();
        case VALUES_VALUES:                    // values -> "{" values "}"
            return reduceValuesTop();
        case VALUES_VALUES_VALUE_DESC:         // values -> values "," value_desc
            return reduceValues2Size();
        case VALUES_VALUE_DESC:                // values -> value_desc
            return reduceValues1Size();
        case ARRAY_VALUE_DESC_VALUES:          // array_value_desc -> "{" values "}"
            return reduceArrayValue();
        case ATTRIBUTE_PUBLIC:                 // attribute -> "public"
        case ATTRIBUTE_PRIVATE:                // attribute -> "private"
        case ATTRIBUTE_PROTECTED:              // attribute -> "protected"
            return reduceAttribute(i);
        case ARRAY_LST_ARRAY_LST_EXP:          // array_lst -> array_lst "[" exp "]"
        case ARRAY_LST_ARRAY_LST_NO_EXP:       // array_lst -> array_lst "[" "]"
            return reduceArrayLst2Size(i);
        case ARRAY_LST_EXP:                    // array_lst -> "[" exp "]"
        case ARRAY_LST_NO_EXP:                 // array_lst -> "[" "]"
            return reduceArrayLst1Size(i);
        case PARAMTER_LIST_PARAMTER_LIST_PARAMTER: // paramter_list -> paramter_list "," paramter
            return reduceParamterList2Size();
        case PARAMTER_LIST_PARAMTER:               // paramter_list -> paramter
            return reduceParamterList1Size();
        case PARAMTER_TYPE_DESC_LETTER_ARRAY_LST:  // paramter -> type_desc "{Letter}" array_lst
            return reduceParamterNamedArray();
        case PARAMTER_TYPE_DESC_LETTER:            // paramter -> type_desc "{Letter}"
            return reduceParamterNamed();
        case PARAMTER_TYPE_DESC:                   // paramter -> type_desc
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
        case TEMPLATE_DESC_TEMPLATE_TEMPLATE_LIST:      // template_desc -> "template" "<" template_list ">"
            return reduceTemplateWithParamters();
        case TEMPLATE_DESC_TEMPLATE:                    // template_desc -> "template" "<" ">"
            return reduceTemplateNoParamters();
        case TEMPLATE_LIST_TEMPLATE_LIST_TEMPLATE_ITEM: // template_list -> template_list "," template_item
            return reduceTemplateList2Size();
        case TEMPLATE_LIST_TEMPLATE_ITEM:               // template_list -> template_item
            return reduceTemplateList1Size();
        case TEMPLATE_ITEM_TYPENAME_LETTER:             // template_item -> "typename" "{Letter}"
            return reduceTemplateItemWithTypeName();
        case TEMPLATE_ITEM_TYPE_DESC_LETTER:            // template_item -> type_desc "{Letter}"
            return reduceTemplateItemWithType();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_PARAM_LIST_BLOCK: // global_function_desc -> template_desc type_desc "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction1();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK:               // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction2();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_NOPARAM_BLOCK:    // global_function_desc -> template_desc type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunction3();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK:                  // global_function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunction4();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_VOID_PARAM_LIST_BLOCK:      // global_function_desc -> template_desc "void" "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction5();
        case GLOBAL_FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:                    // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunction6();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_VOID_NOPARAM_BLOCK:         // global_function_desc -> template_desc "void" "{Letter}" "(" ")" block
            return reduceGlobalFunction7();
        case GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK:                       // global_function_desc -> "void" "{Letter}" "(" ")" block
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
        case CLASS_CONTENT_LIST_CLASS_CONTENT_LIST_CLASS_CONTENT: // class_content_list -> class_content_list class_content
            return reduceClassContentList2Size();
        case CLASS_CONTENT_LIST_CLASS_CONTENT:                    // class_content_list -> class_content
            return reduceClassContentList1Size();
        case CLASS_CONTENT_FUNCTION_DESC:                         // class_content -> function_desc
            return reduceClassContentFunction();
        case CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC:                // class_content -> attribute declare_desc ";"
            return reduceClassContentDeclareWithAttribute();
        case CLASS_CONTENT_DECLARE_DESC:                          // class_content -> declare_desc ";"
            return reduceClassContentDeclareNoAttribute();
        case FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK:  // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunction2();
        case FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK:            // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunction4();
        case FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_NOPARAM_BLOCK:     // function_desc -> attribute type_desc "{Letter}" "(" ")" block
            return reduceFunction6();
        case FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK:               // function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceFunction8();
        case FUNCTION_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK:       // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunction10();
        case FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:                 // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunction12();
        case FUNCTION_DESC_ATTRIBUTE_VOID_NOPARAM_BLOCK:          // function_desc -> attribute "void" "{Letter}" "(" ")" block
            return reduceFunction14();
        case FUNCTION_DESC_VOID_NOPARAM_BLOCK:                    // function_desc -> "void" "{Letter}" "(" ")" block
            return reduceFunction16();
        case STMT_ASSIGN_DESC:                       // stmt -> assign_desc ";"
        case STMT_CALL_DESC:                         // stmt -> call_desc ";"
        case STMT_DECLARE_DESC:                      // stmt -> declare_desc ";"
        case STMT_DO_DESC:                           // stmt -> do_desc ";"
        case STMT_IF_DESC:                           // stmt -> if_desc
        case STMT_FOR_DESC:                          // stmt -> for_desc
        case STMT_WHILE_DESC:                        // stmt -> while_desc
        case STMT_RETURN_DESC:                       // stmt -> return_desc
            return reduceStmt(i);
        case STMT_NO_SEMICOLON_ASSIGN_DESC:          // stmt_no_semicolon -> assign_desc
        case STMT_NO_SEMICOLON_CALL_DESC:            // stmt_no_semicolon -> call_desc
        case STMT_NO_SEMICOLON_DECLARE_DESC:         // stmt_no_semicolon -> declare_desc
        case STMT_NO_SEMICOLON_IF_DESC:              // stmt_no_semicolon -> if_desc
        case STMT_NO_SEMICOLON_FOR_DESC:             // stmt_no_semicolon -> for_desc
        case STMT_NO_SEMICOLON_WHILE_DESC:           // stmt_no_semicolon -> while_desc
        case STMT_NO_SEMICOLON_DO_DESC:              // stmt_no_semicolon -> do_desc
            return reduceStmtNoSemicolon(i);
        case DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST_EQUAL_EXP: // declare_desc -> declare_desc "," "{Letter}" array_lst "=" exp
        case DECLARE_DESC_DECLARE_DESC_LETTER_EQUAL_EXP:           // declare_desc -> declare_desc "," "{Letter}" "=" exp
        case DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST_EQUAL_EXP:    // declare_desc -> type_desc "{Letter}" array_lst "=" exp
        case DECLARE_DESC_TYPE_DESC_LETTER_EQUAL_EXP:              // declare_desc -> type_desc "{Letter}" "=" exp
            return reduceDeclareWithAssign(i);
        case DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST:           // declare_desc -> declare_desc "," "{Letter}" array_lst
        case DECLARE_DESC_DECLARE_DESC_LETTER:                     // declare_desc -> declare_desc "," "{Letter}"
        case DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST:              // declare_desc -> type_desc "{Letter}" array_lst
        case DECLARE_DESC_TYPE_DESC_LETTER:                        // declare_desc -> type_desc "{Letter}"
            return reduceDeclareWithoutAssign(i);
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
        case IF_DESC_IF_EXP_STMT_ELSE_DESC:          // if_desc -> "if" "(" exp ")" stmt else_desc
            return reduceIfWithStmtElse();
        case IF_DESC_IF_EXP_STMT:                    // if_desc -> "if" "(" exp ")" stmt
            return reduceIfWithStmt();
        case IF_DESC_IF_EXP_BLOCK_ELSE_DESC:         // if_desc -> "if" "(" exp ")" block else_desc
            return reduceIfWithBlockElse();
        case IF_DESC_IF_EXP_BLOCK:                   // if_desc -> "if" "(" exp ")" block
            return reduceIfWithBlock();
        case ELSE_DESC_ELSE_STMT:                    // else_desc -> "else" stmt
        case ELSE_DESC_ELSE_BLOCK:                   // else_desc -> "else" block
            return pop1Shifts();
	    case FOR_DESC_FOR_STMT_EXP_STMT_BLOCK:       // for_desc -> "for" "(" stmt ";" exp ";" stmt ")" block
	        return reduceFor();
        case WHILE_DESC_WHILE_EXP_BLOCK:             // while_desc -> "while" "(" exp ")" block
            return reduceWhile();
        case DO_DESC_DO_BLOCK_WHILE_EXP:             // do_desc -> "do" block "while" "(" exp ")"
            return reduceDo();
        case RETURN_DESC_EXP:                        // return_desc -> "return" exp ";"
            return reduceReturnExp();
        case EXP_EXP_STMT_STMT:                      // exp -> exp "?" stmt ":" stmt
            return reduceExp3Size();
        case EXP_GREATER_EQUAL_EXP1:                 // exp -> exp ">" "=" exp1
        case EXP_LESS_EQUAL:                         // exp -> exp "<" "=" exp1
        case EXP_EQUAL:                              // exp -> exp "=" "=" exp1
        case EXP_LOGIC_AND:                          // exp -> exp "&" "&" exp1
        case EXP_LOGIC_OR:                           // exp -> exp "|" "|" exp1
            return reduceExp2Size(i);
        case EXP_GREATER:                            // exp -> exp ">" exp1
        case EXP_LESS:                               // exp -> exp "<" exp1
        case EXP_ASSIGN:                             // exp -> exp "=" exp1
        case EXP_BIT_AND:                            // exp -> exp "&" exp1
        case EXP_BIT_OR:                             // exp -> exp "|" exp1
        case EXP_BIT_XOR:                            // exp -> exp "^" exp1
        case EXP_NOT:                                // exp1 -> "!" exp2
        case EXP1_POSITIVE:                          // exp1 -> "+" exp2
        case EXP1_NEGATIVE:                          // exp1 -> "-" exp2
        case EXP2_ADD:                               // exp2 -> exp2 "+" exp3
        case EXP2_SUB:                               // exp2 -> exp2 "-" exp3
        case EXP3_MUL:                               // exp3 -> exp3 "*" exp4
        case EXP3_DIV:                               // exp3 -> exp3 "/" exp4
        case EXP3_MOD:                               // exp3 -> exp3 "%" exp4
            return reduceExp1Size(i);
        case EXP4_BRACKETS:                          // exp4 -> "(" exp ")"
            return pop2Shifts();
        case EXP4_CALL_DESC:                         // exp4 -> call_desc
            return reduceExpCall();
        case EXP4_VALUE_DESC_AS_TYPE_DESC:           // exp4 -> value_desc "as" type_desc
            return reduceExpValueAsType();
        case EXP4_VALUE_DESC:                        // exp4 -> value_desc
            return reduceExpValue();
        }
        return true;
    }

    void Parser::print(ostream& stream)
    {
        if (syntaxTreeStack.size() == 1) syntaxTreeStack.top()->print(stream, 0);
    }
}
