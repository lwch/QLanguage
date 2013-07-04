/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   21:53
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_TemplateList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_TemplateList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_TemplateItem.h"
#include "SyntaxTree_TemplateList.h"

namespace QLanguage
{
    SyntaxTree_TemplateList::SyntaxTree_TemplateList()
        : parent(sizeof(SyntaxTree_TemplateList))
    {
    }

    SyntaxTree_TemplateList::~SyntaxTree_TemplateList()
    {
    }

    void SyntaxTree_TemplateList::print(ostream& stream, uint indent)const
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

    // template_list -> template_list "," template_item
    bool Parser::reduceTemplateList2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_TemplateList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_TemplateItem*, syntaxTreeStack.top());
#endif
        shifts.pop();

        SyntaxTree_TemplateList* pTemplateList = dynamic_cast<SyntaxTree_TemplateList*>(syntaxTreeStack[1]);

        pTemplateList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        return true;
    }

    // template_list -> template_item
    bool Parser::reduceTemplateList1Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_TemplateItem*, syntaxTreeStack.top());
#endif
        SyntaxTree_TemplateList* pTemplateList = allocator<SyntaxTree_TemplateList>::allocate();
        construct(pTemplateList);
        
        context.data.insert(pTemplateList);

        pTemplateList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pTemplateList);

        return true;
    }
}
