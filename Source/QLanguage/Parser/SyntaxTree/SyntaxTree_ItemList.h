/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   10:36
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ItemList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ItemList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_ITEM_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_ITEM_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ItemList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ItemList();
        virtual ~SyntaxTree_ItemList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ItemList*, &x);
#endif
            return childs.size() == x.childs.size();
        }
    };
}

#endif
