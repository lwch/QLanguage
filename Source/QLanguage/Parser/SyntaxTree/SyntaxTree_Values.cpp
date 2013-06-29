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
#include "SyntaxTree_Values.h"

namespace QLanguage
{
    SyntaxTree_Values::SyntaxTree_Values(bool bTop /* = false */)
        : parent(sizeof(SyntaxTree_Values))
        , bTop(bTop)
    {
    }

    SyntaxTree_Values::~SyntaxTree_Values()
    {
    }

    void SyntaxTree_Values::print(ostream& stream, uint indent)const
    {
        if (bTop)
        {
            stream << '{';
            childs[0]->print(stream, indent);
            stream << '}';
        }
        else if (childs.size())
        {
            childs[0]->print(stream, indent);
            for (size_t i = 1, m = childs.size(); i < m; ++i)
            {
                stream << ", ";
                childs[i]->print(stream, indent);
            }
        }
    }

    // values -> "{" values "}"
    bool Parser::reduceValuesTop()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Values* pValues = allocator<SyntaxTree_Values>::allocate();
        construct(pValues, true);

        context.data.insert(pValues);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValues);

        return true;
    }

    // values -> values "," value_desc
    bool Parser::reduceValues2Size()
    {
        shifts.pop();

        SyntaxTree_Values* pValues = dynamic_cast<SyntaxTree_Values*>(syntaxTreeStack[1]);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        return true;
    }

    // values -> value_desc
    bool Parser::reduceValues1Size()
    {
        SyntaxTree_Values* pValues = allocator<SyntaxTree_Values>::allocate();
        construct(pValues);

        context.data.insert(pValues);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValues);

        return true;
    }
}
