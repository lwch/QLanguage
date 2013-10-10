﻿/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_While.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_While
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_WHILE_H_
#define _QLANGUAGE_SYNTAX_TREE_WHILE_H_

#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_While : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_While(SyntaxTree_Exp& exp, SyntaxTree_Block& block);
        virtual ~SyntaxTree_While();
        
        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_While"; }

        virtual bool make(Parser *pParser);
        
        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_While*, &x);
#endif
            return exp == dynamic_cast<const SyntaxTree_While*>(&x)->exp ||
                   block == dynamic_cast<const SyntaxTree_While*>(&x)->block;
        }
        
        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_While*, &x);
#endif
            return exp != dynamic_cast<const SyntaxTree_While*>(&x)->exp ||
                   block != dynamic_cast<const SyntaxTree_While*>(&x)->block;
        }
    protected:
        SyntaxTree_Exp&   exp;
        SyntaxTree_Block& block;
    };
}

#endif
