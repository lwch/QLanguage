/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   11:27
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ArrayValue.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ArrayValue
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#include "../Parser.h"
#include "SyntaxTree_ArrayValue.h"

namespace QLanguage
{
    SyntaxTree_ArrayValue::SyntaxTree_ArrayValue(SyntaxTree_Values* pValues)
        : parent(sizeof(SyntaxTree_ArrayValue))
        , pValues(pValues)
    {
    }

    SyntaxTree_ArrayValue::~SyntaxTree_ArrayValue()
    {
    }

    void SyntaxTree_ArrayValue::print(ostream& stream, uint indent)const
    {
        stream << '{';
        pValues->print(stream, indent);
        stream << '}';
    }

    // array_value_desc -> "{" values "}"
    bool Parser::reduceArrayValue()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Values*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_ArrayValue* pArrayValue = allocator<SyntaxTree_ArrayValue>::allocate();
        construct(pArrayValue, dynamic_cast<SyntaxTree_Values*>(syntaxTreeStack.top()));

        context.data.insert(pArrayValue);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pArrayValue);

        return true;
    }
}
