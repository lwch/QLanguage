/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:37
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Block.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Block
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_BLOCK_H_
#define _QLANGUAGE_SYNTAX_TREE_BLOCK_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Block : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Block();
        SyntaxTree_Block(SyntaxTree_Base* pStmts);
        virtual ~SyntaxTree_Block();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Block*, &x);
#endif
            return *pStmts == *dynamic_cast<const SyntaxTree_Block*>(&x)->pStmts;
        }
    protected:
        SyntaxTree_Base* pStmts;
    };
}

#endif
