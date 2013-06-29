/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   12:23
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ArrayLst.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ArrayLst
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_ARRAY_LST_H_
#define _QLANGUAGE_SYNTAX_TREE_ARRAY_LST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ArrayLst : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ArrayLst();
        virtual ~SyntaxTree_ArrayLst();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_ArrayLst"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ArrayLst*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ArrayLst*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
