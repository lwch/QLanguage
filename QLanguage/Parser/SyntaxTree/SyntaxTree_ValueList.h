/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   13:43
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ValueList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ValueList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_VALUE_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_VALUE_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ValueList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ValueList();
        virtual ~SyntaxTree_ValueList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ValueList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ValueList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
