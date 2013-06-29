/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   17:28
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_GlobalFunction.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_GlobalFunction
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_Type.h"
#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_GlobalFunction.h"

namespace QLanguage
{
    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_GlobalFunction))
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
        , block(block)
    {
    }

    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_GlobalFunction))
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_GlobalFunction::~SyntaxTree_GlobalFunction()
    {
    }

    void SyntaxTree_GlobalFunction::print(ostream& stream, uint indent)const
    {
        returnType.print(stream, indent);
        stream << ' ' << name << '(';
        if (pParamterList) pParamterList->print(stream, indent);
        stream << ") ";
        block.print(stream, indent + parent::indent);
    }

    // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceGlobalFunction2()
    {
        shifts.pop();
        shifts.pop();
        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();

        return true;
    }

    // global_function_desc -> type_desc "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunction4()
    {
        shifts.pop();
        shifts.pop();
        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();

        return true;
    }

    // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceGlobalFunction6()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  *pReturnType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // global_function_desc -> "void" "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunction8()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  *pReturnType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
