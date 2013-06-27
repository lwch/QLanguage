/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   13:45
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ValueList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ValueList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ValueList.h"

namespace QLanguage
{
    SyntaxTree_ValueList::SyntaxTree_ValueList() : parent(sizeof(SyntaxTree_ValueList))
    {
    }

    SyntaxTree_ValueList::~SyntaxTree_ValueList()
    {
    }

    void SyntaxTree_ValueList::print(ostream& stream, uint indent)const
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

    // value_list -> value_list "," exp
    bool Parser::reduceValueList2Size()
    {
        SyntaxTree_ValueList* pValueList = dynamic_cast<SyntaxTree_ValueList*>(syntaxTreeStack[1]);

        pValueList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        shifts.pop();
        return true;
    }

    // value_list -> exp
    bool Parser::reduceValueList1Size()
    {
        SyntaxTree_ValueList* pValueList = allocator<SyntaxTree_ValueList>::allocate();
        construct(pValueList);

        context.data.insert(pValueList);

        pValueList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValueList);
        return true;
    }
}
