/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   10:38
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ItemList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ItemList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Item.h"
#include "SyntaxTree_ItemList.h"

namespace QLanguage
{
    SyntaxTree_ItemList::SyntaxTree_ItemList() : parent(sizeof(SyntaxTree_ItemList))
    {
    }

    SyntaxTree_ItemList::~SyntaxTree_ItemList()
    {
    }

    void SyntaxTree_ItemList::print(ostream& stream, uint indent)const
    {
        for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            (*i)->print(stream, indent);
            stream << endl;
        }
    }

    // item_list -> item_list item
    bool Parser::reduceItemList2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ItemList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Item*, syntaxTreeStack.top());
#endif
        SyntaxTree_ItemList* pStmtList = dynamic_cast<SyntaxTree_ItemList*>(syntaxTreeStack[1]);

        pStmtList->pushChild(syntaxTreeStack.top());

        context.data.insert(pStmtList);

        syntaxTreeStack.pop();
        return true;
    }

    // item_list -> item
    bool Parser::reduceItemList1Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Item*, syntaxTreeStack.top());
#endif
        SyntaxTree_ItemList* pStmtList = allocator<SyntaxTree_ItemList>::allocate();
        construct(pStmtList);

        pStmtList->pushChild(syntaxTreeStack.top());

        context.data.insert(pStmtList);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pStmtList);
        return true;
    }
}
