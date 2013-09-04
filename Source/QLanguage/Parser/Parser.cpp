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
    const int Parser::ConstantTable::indexOf(const VM::Variant& v)const
    {
        for (size_t i = 0, m = constants.size(); i < m; ++i)
        {
            if (constants[i] == v) return i;
        }
        return -1;
    }

    bool Parser::ConstantTable::push(const VM::Variant& v)
    {
        if (constants.size() >= maxConstantCount) return false;
        constants.push_back(v);
        return true;
    }

    const size_t Parser::ConstantTable::size()const
    {
        return constants.size();
    }

    const VM::Variant& Parser::ConstantTable::operator[](size_t idx)const
    {
        return constants[idx];
    }

    Parser::ContextInfo::ContextInfo(Type type, const HASH_KEY_TYPE& hash, ConstantTable& constantTable)
        : type(type)
        , hash(hash)
        , constantTable(constantTable)
    {
        for (size_t i = 0; i < maxRegisterCount; ++i)
        {
            reg[i].first  = false;
            reg[i].second = -1;
        }
    }

    Parser::FunctionInfo::FunctionInfo(const HASH_KEY_TYPE& hash)
        : hash(hash)
    {
    }

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
        case VALUE_DESC_VALUES:                // value_desc -> values
            return reduceValueContent(i);
        case VALUES_VALUES_LIST:                    // values -> "{" values_list "}"
            return reduceValuesTop();
        case VALUES_LIST_VALUES_LIST_VALUE_DESC:    // values_list -> values_list "," value_desc
            return reduceValuesList2Size();
        case VALUES_LIST_VALUE_DESC:                // values_list -> value_desc
            return reduceValuesList1Size();
        case VALUE_LIST_VALUE_LIST_EXP:        // value_list -> value_list "," exp
            return reduceValueList2Size();
        case VALUE_LIST_EXP:                   // value_list -> exp
            return reduceValueList1Size();
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
            return reduceStmtList2Size(i);
        case STMT_LIST_STMT:                   // stmt_list -> stmt_list block
        case STMT_LIST_BLOCK:                  // stmt_list -> block
            return reduceStmtList1Size(i);
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
            return reduceGlobalFunctionTemplateTypeParamters();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK:               // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunctionTypeParamters();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_NOPARAM_BLOCK:    // global_function_desc -> template_desc type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunctionTemplateType();
        case GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK:                  // global_function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceGlobalFunctionType();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_VOID_PARAM_LIST_BLOCK:      // global_function_desc -> template_desc "void" "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunctionTemplateVoidParamters();
        case GLOBAL_FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:                    // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceGlobalFunctionVoidParamters();
        case GLOBAL_FUNCTION_DESC_TEMPLATE_DESC_VOID_NOPARAM_BLOCK:         // global_function_desc -> template_desc "void" "{Letter}" "(" ")" block
            return reduceGlobalFunctionTemplateVoid();
        case GLOBAL_FUNCTION_DESC_VOID_NOPARAM_BLOCK:                       // global_function_desc -> "void" "{Letter}" "(" ")" block
            return reduceGlobalFunctionVoid();
        case FUNCTION_DECLARE_TEMPLATE_DESC_TYPE_DESC_PARAM_LIST:           // function_declare -> template_desc type_desc "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclareTemplateTypeParamters();
        case FUNCTION_DECLARE_TYPE_DESC_PARAM_LIST:                         // function_declare -> type_desc "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclareTypeParamters();
        case FUNCTION_DECLARE_TEMPLATE_DESC_TYPE_DESC:                      // function_declare -> template_desc type_desc "{Letter}" "(" ")" ";"
            return reduceFunctionDeclareTemplateType();
        case FUNCTION_DECLARE_TYPE_DESC:                                    // function_declare -> type_desc "{Letter}" "(" ")" ";"
            return reduceFunctionDeclareType();
        case FUNCTION_DECLARE_TEMPLATE_DESC_VOID_PARAM_LIST:                // function_declare -> template_desc "void" "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclareTemplateVoidParamters();
        case FUNCTION_DECLARE_VOID_PARAM_LIST:                              // function_declare -> "void" "{Letter}" "(" paramter_list ")" ";"
            return reduceFunctionDeclareVoidParamters();
        case FUNCTION_DECLARE_TEMPLATE_DESC_VOID:                           // function_declare -> template_desc "void" "{Letter}" "(" ")" ";"
            return reduceFunctionDeclareTemplateVoid();
        case FUNCTION_DECLARE_VOID:                                         // function_declare -> "void" "{Letter}" "(" ")" ";"
            return reduceFunctionDeclareVoid();
        case INTERFACE_DESC_INTERFACE_CONTENT_LIST:                    // interface_desc -> "interface" "{Letter}" "{" interface_content "}"
            return reduceInterfaceWidthContent();
        case INTERFACE_DESC_NO_INTERFACE_CONTENT_LIST:                 // interface_desc -> "interface" "{Letter}" "{" "}"
            return reduceInterfaceNoContent();
        case INTERFACE_CONTENT_INTERFACE_CONTENT_FUNCTION_DECLARE:     // interface_content -> interface_content function_declare
            return reduceInterfaceContent2Size();
        case INTERFACE_CONTENT_FUNCTION_DECLARE:                       // interface_content -> function_declare
            return reduceInterfaceContent1Size();
        case CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1_CLASS_DESC2_CLASS_CONTENT_LIST: // class_desc -> template_desc class_desc1 class_desc2 "{" class_content_list "}"
            return reduceClass1();
        case CLASS_DESC_CLASS_DESC1_CLASS_DESC2_CLASS_CONTENT_LIST:               // class_desc -> class_desc1 class_desc2 "{" class_content_list "}"
            return reduceClass2();
        case CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1_CLASS_CONTENT_LIST:             // class_desc -> template_desc class_desc1 "{" class_content_list "}"
            return reduceClass3();
        case CLASS_DESC_CLASS_DESC1_CLASS_CONTENT_LIST:                           // class_desc -> class_desc1 "{" class_content_list "}"
            return reduceClass4();
        case CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1_CLASS_DESC2:                    // class_desc -> template_desc class_desc1 class_desc2 "{" "}"
            return reduceClass5();
        case CLASS_DESC_CLASS_DESC1_CLASS_DESC2:                                  // class_desc -> class_desc1 class_desc2 "{" "}"
            return reduceClass6();
        case CLASS_DESC_TEMPLATE_DESC_CLASS_DESC1:                                // class_desc -> template_desc class_desc1
            return reduceClass7();
        case CLASS_DESC_CLASS_DESC1:                                              // class_desc -> class_desc1 "{" "}"
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
        case FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK: // function_desc -> template_desc attribute type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunctionTemplateAttributeTypeParamters();
        case FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_PARAM_LIST_BLOCK:               // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunctionAttributeTypeParamters();
        case FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_PARAM_LIST_BLOCK:           // function_desc -> template_desc type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunctionTemplateTypeParamters();
        case FUNCTION_DESC_TYPE_DESC_PARAM_LIST_BLOCK:                         // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
            return reduceFunctionTypeParamters();
        case FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_TYPE_DESC_BLOCK:            // function_desc -> template_desc attribute type_desc "{Letter}" "(" ")" block
            return reduceFunctionTemplateAttributeType();
        case FUNCTION_DESC_ATTRIBUTE_TYPE_DESC_BLOCK:                          // function_desc -> attribute type_desc "{Letter}" "(" ")" block
            return reduceFunctionAttributeType();
        case FUNCTION_DESC_TEMPLATE_DESC_TYPE_DESC_BLOCK:                      // function_desc -> template_desc type_desc "{Letter}" "(" ")" block
            return reduceFunctionTemplateType();
        case FUNCTION_DESC_TYPE_DESC_BLOCK:                                    // function_desc -> type_desc "{Letter}" "(" ")" block
            return reduceFunctionType();
        case FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK:      // function_desc -> template_desc attribute "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunctionTemplateAttributeVoidParamters();
        case FUNCTION_DESC_ATTRIBUTE_VOID_PARAM_LIST_BLOCK:                    // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunctionAttributeVoidParamters();
        case FUNCTION_DESC_TEMPLATE_DESC_VOID_PARAM_LIST_BLOCK:                // function_desc -> tempalte_desc "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunctionTemplateVoidParamters();
        case FUNCTION_DESC_VOID_PARAM_LIST_BLOCK:                              // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
            return reduceFunctionVoidParamters();
        case FUNCTION_DESC_TEMPLATE_DESC_ATTRIBUTE_VOID_BLOCK:                 // function_desc -> template_desc attribute "void" "{Letter}" "(" ")" block
            return reduceFunctionTemplateAttributeVoid();
        case FUNCTION_DESC_ATTRIBUTE_VOID_BLOCK:                               // function_desc -> attribute "void" "{Letter}" "(" ")" block
            return reduceFunctionAttributeVoid();
        case FUNCTION_DESC_TEMPLATE_DESC_VOID_BLOCK:                           // function_desc -> template_desc "void" "{Letter}" "(" ")" block
            return reduceFunctionTemplateVoid();
        case FUNCTION_DESC_VOID_BLOCK:                                         // function_desc -> "void" "{Letter}" "(" ")" block
            return reduceFunctionVoid();
        case STMT_ASSIGN_DESC:                       // stmt -> assign_desc ";"
        case STMT_CALL_LIST:                         // stmt -> call_list ";"
        case STMT_DECLARE_DESC:                      // stmt -> declare_desc ";"
        case STMT_DO_DESC:                           // stmt -> do_desc ";"
        case STMT_IF_DESC:                           // stmt -> if_desc
        case STMT_FOR_DESC:                          // stmt -> for_desc
        case STMT_WHILE_DESC:                        // stmt -> while_desc
        case STMT_SWITCH_DESC:                       // stmt -> switch_desc
        case STMT_BREAK_DESC:                        // stmt -> break_desc ";"
        case STMT_CONTINUE_DESC:                     // stmt -> continue_desc ";"
        case STMT_RETURN_DESC:                       // stmt -> return_desc
            return reduceStmt(i);
        case STMT_NO_SEMICOLON_ASSIGN_DESC:          // stmt_no_semicolon -> assign_desc
        case STMT_NO_SEMICOLON_CALL_LIST:            // stmt_no_semicolon -> call_list
        case STMT_NO_SEMICOLON_DECLARE_DESC:         // stmt_no_semicolon -> declare_desc
        case STMT_NO_SEMICOLON_IF_DESC:              // stmt_no_semicolon -> if_desc
        case STMT_NO_SEMICOLON_FOR_DESC:             // stmt_no_semicolon -> for_desc
        case STMT_NO_SEMICOLON_WHILE_DESC:           // stmt_no_semicolon -> while_desc
        case STMT_NO_SEMICOLON_DO_DESC:              // stmt_no_semicolon -> do_desc
        case STMT_NO_SEMICOLON_SWITCH_DESC:          // stmt_no_semicolon -> switch_desc
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
        case CALL_LIST_CALL_LIST_VALUE_LIST:               // call_list -> call_list "(" value_list ")"
            return reduceCallListValueList();
        case CALL_LIST_CALL_LIST:                          // call_list -> call_list "(" ")"
            return reduceCallList();
        case CALL_LIST_CALL_DESC:                          // call_list -> call_desc
            return reduceCallListCall();
        case CALL_DESC_MEMBER_DESC_VALUE_LIST:             // call_desc -> member_desc "(" value_list ")"
            return reduceCall1();
        case CALL_DESC_MEMBER_DESC:                        // call_desc -> member_desc "(" ")"
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
            return reduceElse(i);
	    case FOR_DESC_FOR_STMT_EXP_STMT_BLOCK:       // for_desc -> "for" "(" stmt ";" exp ";" stmt ")" block
	        return reduceFor();
        case WHILE_DESC_WHILE_EXP_BLOCK:             // while_desc -> "while" "(" exp ")" block
            return reduceWhile();
        case DO_DESC_DO_BLOCK_WHILE_EXP:             // do_desc -> "do" block "while" "(" exp ")"
            return reduceDo();
        case SWITCH_DESC_SWITCH_EXP_SWITCH_CONTENT_LIST:             // switch_desc -> "switch" "(" exp ")" "{" switch_content_list "}"
            return reduceSwitch();
        case SWITCH_CONTENT_LIST_SWITCH_CONTENT_LIST_SWITCH_CONTENT: // switch_content_list -> switch_content_list switch_content
            return reduceSwitchContentList2Size();
        case SWITCH_CONTENT_LIST_SWITCH_CONTENT:                     // switch_content_list -> switch_content
            return reduceSwitchContentList1Size();
        case SWITCH_CONTENT_CASE_EXP_STMT_LIST:                      // switch_content -> "case" exp ":" stmt_list
            return reduceSwitchContentCase();
        case SWITCH_CONTENT_DEFAULT_STMT_LIST:                       // switch_content -> "default" ":" stmt_list
            return reduceSwitchContentDefault();
        case BREAK_DESC_BREAK:                       // break_desc -> "break"
            return reduceBreak();
        case CONTINUE_DESC_CONTINUE:                 // continue_desc -> "continue"
            return reduceContinue();
        case RETURN_DESC_BLOCK:                      // return_desc -> "return" block
            return reduceReturnBlock();
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
        case EXP4_CALL_LIST:                         // exp4 -> call_list
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

    const pair<int, ushort> Parser::indexOfConstant(const VM::Variant& v)
    {
        int idx = constantTable.indexOf(v);
        if (idx != -1) return pair<int, ushort>(0, idx);
        for (size_t i = 0, m = makeContext.size(); i < m; ++i)
        {
            idx = makeContext[i].constantTable.indexOf(v);
            if (idx != -1) return pair<int, ushort>(i + 1, idx);
        }
        return pair<int, ushort>(-1, 0);
    }

    pair<size_t, ushort> Parser::pushConstant(const VM::Variant& v)
    {
        if (!constantTable.push(v))
        {
            for (size_t i = 0, m = makeContext.size(); i < m; ++i)
            {
                if (makeContext[i].constantTable.push(v)) return pair<size_t, ushort>(i + 1, makeContext[i].constantTable.size() - 1);
            }
        }
        else return pair<size_t, ushort>(0, constantTable.size() - 1);
        throw error<const char*>("Doesn't have enough capacity", __FILE__, __LINE__);
        return pair<size_t, ushort>(-1, 0);
    }

    const VM::Variant& Parser::getVariant(uchar block, ushort index)const
    {
        if (block == 0) return constantTable[index];
        else {
            const ContextInfo& i = makeContext[block - 1];
            return makeContext[block - 1].constantTable[index];
        }
    }
}
