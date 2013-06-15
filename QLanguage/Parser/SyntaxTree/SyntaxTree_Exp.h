/********************************************************************
	created:	2013/06/15
	created:	15:6:2013   17:15
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Exp.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Exp
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_EXP_H_
#define _QLANGUAGE_SYNTAX_TREE_EXP_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Exp : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            GreaterEqual,
            LessEqual,
            Equal,
            Greater,
            Less,
            Assign,
            LogicAnd,
            LogicOr,
            BitAnd,
            BitOr,
            BitXor,
            Not,
            Positive,
            Negative,
            Add,
            Sub,
            Mul,
            Div,
            Mod,
            Call,
            ValueAsType,
            Value
        };
    public:
        SyntaxTree_Exp(Type type, SyntaxTree_Base* pOP1, SyntaxTree_Base* pOP2);
        SyntaxTree_Exp(Type type, SyntaxTree_Base* pOP);
        virtual ~SyntaxTree_Exp();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Exp*, &x);
#endif
            return type == dynamic_cast<const SyntaxTree_Exp*>(&x)->type &&
                   *pOP1 == *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP1 &&
                   ((pOP2 && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2 && *pOP2 == *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2) ||
                   (pOP2 == NULL && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2 == NULL));
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Exp*, &x);
#endif
            return type != dynamic_cast<const SyntaxTree_Exp*>(&x)->type ||
                   *pOP1 != *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP1 ||
                   ((pOP2 && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2 && *pOP2 != *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2) ||
                   (pOP2 == NULL && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2));
        }
    protected:
        SyntaxTree_Base* pOP1;
        SyntaxTree_Base* pOP2;
        Type             type;
    };
}

#endif
