/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   11:18
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_InterfaceContentList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_InterfaceContentList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_INTERFACE_CONTENT_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_INTERFACE_CONTENT_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_InterfaceContentList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_InterfaceContentList();
        virtual ~SyntaxTree_InterfaceContentList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_InterfaceContentList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_InterfaceContentList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
