/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   14:47
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Item.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Item
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_GlobalFunction.h"
#include "SyntaxTree_DeclareList.h"
#include "SyntaxTree_Interface.h"
#include "SyntaxTree_Call.h"
#include "SyntaxTree_Item.h"

namespace QLanguage
{
    SyntaxTree_Item::SyntaxTree_Item(SyntaxTree_Base* pItem) : parent(sizeof(SyntaxTree_Item)), pItem(pItem)
    {
    }

    SyntaxTree_Item::~SyntaxTree_Item()
    {
    }

    void SyntaxTree_Item::print(ostream& stream, uint indent)const
    {
        pItem->print(stream, indent);
    }

    // item -> global_function_desc
    // item -> declare_desc ";"
    // item -> interface_desc
    // item -> class_desc
    bool Parser::reduceItem(ushort i)
    {
#ifdef _DEBUG
        switch (i)
        {
        case ITEM_GLOBAL_FUNCTION_DESC:
            TRY_CAST(SyntaxTree_GlobalFunction*, syntaxTreeStack.top());
            break;
        case ITEM_DECLARE_DESC:
            TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack.top());
            break;
        case ITEM_INTERFACE_DESC:
            TRY_CAST(SyntaxTree_Interface*, syntaxTreeStack.top());
            break;
        case ITEM_CLASS_DESC:
            TRY_CAST(SyntaxTree_Call*, syntaxTreeStack.top());
            break;
        }
#endif
        SyntaxTree_Item* pItem = allocator<SyntaxTree_Item>::allocate();
        construct(pItem, syntaxTreeStack.top());

        context.data.insert(pItem);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pItem);

        if (i == ITEM_DECLARE_DESC) shifts.pop();

        return true;
    }
}
