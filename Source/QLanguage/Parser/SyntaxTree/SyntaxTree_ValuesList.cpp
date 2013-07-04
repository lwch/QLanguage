/********************************************************************
	created:	2013/07/04
	created:	4:7:2013   23:38
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ValuesList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ValuesList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Value.h"
#include "SyntaxTree_ValuesList.h"

namespace QLanguage
{
    SyntaxTree_ValuesList::SyntaxTree_ValuesList()
        : parent(sizeof(SyntaxTree_ValuesList))
    {
    }

    SyntaxTree_ValuesList::~SyntaxTree_ValuesList()
    {
    }

    void SyntaxTree_ValuesList::print(ostream& stream, uint indent)const
    {
        if (childs.size())
        {
            childs[0]->print(stream, indent);
            for (size_t i = 1, m = childs.size(); i < m; ++i)
            {
                stream << ", ";
                childs[i]->print(stream, indent);
            }
        }
    }

    // values_list -> values_list "," value_desc
    bool Parser::reduceValuesList2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ValuesList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Value*, syntaxTreeStack.top());
#endif
        shifts.pop();

        SyntaxTree_ValuesList* pValues = dynamic_cast<SyntaxTree_ValuesList*>(syntaxTreeStack[1]);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        return true;
    }

    // values_list -> value_desc
    bool Parser::reduceValuesList1Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Value*, syntaxTreeStack.top());
#endif
        SyntaxTree_ValuesList* pValues = allocator<SyntaxTree_ValuesList>::allocate();
        construct(pValues);

        context.data.insert(pValues);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValues);

        return true;
    }
}
