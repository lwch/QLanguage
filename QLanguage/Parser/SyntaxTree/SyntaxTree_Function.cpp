﻿/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   16:31
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Function.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Function
	file ext:	cpp
	author:		lwch
	
	purpose:	若不写attribute则默认为private的
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Function.h"

namespace QLanguage
{
    SyntaxTree_Attribute::Type SyntaxTree_Function::defaultAttribute = SyntaxTree_Attribute::Private;

    SyntaxTree_Function::SyntaxTree_Function(SyntaxTree_Attribute& attribute, const string& name, SyntaxTree_Type& returnType, SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , attribute(attribute)
        , name(name)
        , returnType(returnType)
        , block(block)
    {
    }

    SyntaxTree_Function::SyntaxTree_Function(SyntaxTree_Attribute& attribute, const string& name, SyntaxTree_Type& returnType, SyntaxTree_ParamterList* pParamterList, SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , attribute(attribute)
        , name(name)
        , returnType(returnType)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_Function::~SyntaxTree_Function()
    {
    }

    void SyntaxTree_Function::print(ostream& stream, uint indent)const
    {
    }

    // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunction2()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Attribute&>(*syntaxTreeStack[3]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        return true;
    }

    // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunction4()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  *pAttribute,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        return true;
    }

    // function_desc -> attribute type_desc "{Letter}" "(" ")" block
    bool Parser::reduceFunction6()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Attribute&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        return true;
    }
    
    // function_desc -> type_desc "{Letter}" "(" ")" block
    bool Parser::reduceFunction8()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  *pAttribute,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        return true;
    }

    // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunction10()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Attribute&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  *pType,
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunction12()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  *pAttribute,
                  shifts.top(),
                  *pType,
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // function_desc -> attribute "void" "{Letter}" "(" ")" block
    bool Parser::reduceFunction14()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Attribute&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  *pType,
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // function_desc -> "void" "{Letter}" "(" ")" block
    bool Parser::reduceFunction16()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  *pAttribute,
                  shifts.top(),
                  *pType,
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();
        return true;
    }
}
