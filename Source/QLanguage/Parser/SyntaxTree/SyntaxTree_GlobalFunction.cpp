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
#include "SyntaxTree_GlobalFunction.h"

namespace QLanguage
{
    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(const string& name, SyntaxTree_Type& returnType) : parent(sizeof(SyntaxTree_GlobalFunction)), name(name), returnType(returnType)
    {
    }

    SyntaxTree_GlobalFunction::~SyntaxTree_GlobalFunction()
    {
    }

    void SyntaxTree_GlobalFunction::print(ostream& stream, uint indent)const
    {
    }

    const bool SyntaxTree_GlobalFunction::operator==(const SyntaxTree_GlobalFunction& x)const
    {
        if (params.size() != x.size()) return false;
        for (size_t i = 0, m = params.size(); i < m; ++i)
        {
            if (*params[i] != *x.params[i]) return false;
        }
        return name == x.name && returnType == x.returnType;
    }

    // global_function_desc -> type_desc "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunction4()
    {
        shifts.pop();
        shifts.pop();
        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction, shifts.top(), dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        shifts.pop();
        return true;
    }
}
