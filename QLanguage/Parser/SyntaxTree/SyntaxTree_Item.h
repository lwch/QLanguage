/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   14:42
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Item.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Item
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_ITEM_H_
#define _QLANGUAGE_SYNTAX_TREE_ITEM_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Item : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Item(SyntaxTree_Base* pItem);
        virtual ~SyntaxTree_Item();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Item*, &x);
#endif
            return *pItem == *dynamic_cast<const SyntaxTree_Item*>(&x)->pItem;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Item*, &x);
#endif
            return *pItem != *dynamic_cast<const SyntaxTree_Item*>(&x)->pItem;
        }
    protected:
        SyntaxTree_Base* pItem;
    };
}

#endif
