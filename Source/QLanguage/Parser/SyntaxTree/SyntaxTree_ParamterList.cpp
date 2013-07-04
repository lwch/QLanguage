/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   15:04
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ParamterList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ParamterList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Paramter.h"
#include "SyntaxTree_ParamterList.h"

namespace QLanguage
{
    SyntaxTree_ParamterList::SyntaxTree_ParamterList() : parent(sizeof(SyntaxTree_ParamterList))
    {
    }

    SyntaxTree_ParamterList::~SyntaxTree_ParamterList()
    {
    }

    void SyntaxTree_ParamterList::print(ostream& stream, uint indent)const
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

    // paramter_list -> paramter_list "," paramter
    bool Parser::reduceParamterList2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Paramter*, syntaxTreeStack.top());
#endif
        SyntaxTree_ParamterList* pParamterList = dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]);

        pParamterList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        shifts.pop();

        return true;
    }

    // paramter_list -> paramter
    bool Parser::reduceParamterList1Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Paramter*, syntaxTreeStack.top());
#endif
        SyntaxTree_ParamterList* pParamterList = allocator<SyntaxTree_ParamterList>::allocate();
        construct(pParamterList);

        context.data.insert(pParamterList);

        pParamterList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pParamterList);

        return true;
    }
}
