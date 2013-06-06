/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   21:07
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_StmtList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_StmtList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_STMT_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_STMT_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_StmtList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_StmtList();
        virtual ~SyntaxTree_StmtList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_StmtList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_StmtList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
