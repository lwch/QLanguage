/********************************************************************
	created:	2013/06/15
	created:	15:6:2013   16:32
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_If.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_If
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_IF_H_
#define _QLANGUAGE_SYNTAX_TREE_IF_H_

#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Else.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_If : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_If(const SyntaxTree_Exp& exp, const SyntaxTree_Base& op1, SyntaxTree_Else* pElse);
        SyntaxTree_If(const SyntaxTree_Exp& exp, const SyntaxTree_Base& op1);
        virtual ~SyntaxTree_If();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_If"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_If*, &x);
#endif
            return exp == dynamic_cast<const SyntaxTree_If*>(&x)->exp &&
                   op1 == dynamic_cast<const SyntaxTree_If*>(&x)->op1 &&
                   parent::checkEqual(pElse, dynamic_cast<const SyntaxTree_If*>(&x)->pElse);
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_If*, &x);
#endif
            return exp != dynamic_cast<const SyntaxTree_If*>(&x)->exp ||
                   op1 != dynamic_cast<const SyntaxTree_If*>(&x)->op1 ||
                   parent::checkNotEqual(pElse, dynamic_cast<const SyntaxTree_If*>(&x)->pElse);
        }
    protected:
        const SyntaxTree_Exp&  exp;
        const SyntaxTree_Base& op1;
        SyntaxTree_Else*       pElse;
    };
}

#endif
