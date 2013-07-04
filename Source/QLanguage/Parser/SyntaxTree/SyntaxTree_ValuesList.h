/********************************************************************
	created:	2013/07/04
	created:	4:7:2013   23:29
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ValuesList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ValuesList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_VALUES_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_VALUES_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ValuesList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ValuesList();
        virtual ~SyntaxTree_ValuesList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_ValuesList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ValuesList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ValuesList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
