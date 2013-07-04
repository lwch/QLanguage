/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   11:28
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Values.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Values
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Value.h"
#include "SyntaxTree_Values.h"

namespace QLanguage
{
    SyntaxTree_Values::SyntaxTree_Values(const SyntaxTree_ValuesList& valuesList)
        : parent(sizeof(SyntaxTree_Values))
        , valuesList(valuesList)
    {
    }

    SyntaxTree_Values::~SyntaxTree_Values()
    {
    }

    void SyntaxTree_Values::print(ostream& stream, uint indent)const
    {
        stream << '{';
        valuesList.print(stream, indent);
        stream << '}';
    }

    // values -> "{" values_list "}"
    bool Parser::reduceValuesTop()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ValuesList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Values* pValues = allocator<SyntaxTree_Values>::allocate();
        construct(pValues, dynamic_cast<const SyntaxTree_ValuesList&>(*syntaxTreeStack.top()));

        context.data.insert(pValues);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValues);

        return true;
    }
}
