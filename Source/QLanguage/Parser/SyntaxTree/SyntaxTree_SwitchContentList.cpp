/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   14:56
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_SwitchContentList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_SwitchContentList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_SwitchContent.h"
#include "SyntaxTree_SwitchContentList.h"

namespace QLanguage
{
    SyntaxTree_SwitchContentList::SyntaxTree_SwitchContentList()
        : parent(sizeof(SyntaxTree_SwitchContentList))
    {
    }

    SyntaxTree_SwitchContentList::~SyntaxTree_SwitchContentList()
    {
    }

    void SyntaxTree_SwitchContentList::print(ostream& stream, uint indent)const
    {
        for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            this->printIndent(stream, indent);
            (*i)->print(stream, indent);
        }
    }

    // switch_content_list -> switch_content_list switch_content
    bool Parser::reduceSwitchContentList2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_SwitchContentList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_SwitchContent*, syntaxTreeStack.top());
#endif
        SyntaxTree_SwitchContentList* pSwitchContentList = dynamic_cast<SyntaxTree_SwitchContentList*>(syntaxTreeStack[1]);

        pSwitchContentList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        return true;
    }

    // switch_content_list -> switch_content
    bool Parser::reduceSwitchContentList1Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_SwitchContent*, syntaxTreeStack.top());
#endif
        SyntaxTree_SwitchContentList* pSwitchContentList = allocator<SyntaxTree_SwitchContentList>::allocate();
        construct(pSwitchContentList);

        context.data.insert(pSwitchContentList);

        pSwitchContentList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pSwitchContentList);

        return true;
    }
}
