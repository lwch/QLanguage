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
    class SyntaxTree_Value;

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
            TrueFalse,
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
        SyntaxTree_Exp(const SyntaxTree_Base& OP1, SyntaxTree_Base* pOP2, SyntaxTree_Base* pOP3);
        SyntaxTree_Exp(Type type, const SyntaxTree_Base& OP1, SyntaxTree_Base* pOP2);
        SyntaxTree_Exp(Type type, const SyntaxTree_Base& OP);
        virtual ~SyntaxTree_Exp();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Exp"; }

        virtual const HASH_KEY_TYPE hash()const;

        virtual bool make(Parser *pParser);

        inline virtual const bool isValue()const { return _type == Value; }
        inline virtual const bool isConstValue()const { return bConstant; }
        virtual const bool toBool()const;
        inline virtual const uchar getConstantBlock()const { return constantBlock; }
        inline virtual const ushort getConstantIndex()const { return constantIndex; }

        static const VM::Variant eval(const SyntaxTree_Value& v, Type type);
        static const VM::Variant eval(const SyntaxTree_Value& v1, const SyntaxTree_Value& v2, Type type);

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Exp*, &x);
#endif
            return _type == dynamic_cast<const SyntaxTree_Exp*>(&x)->_type &&
                   OP1 == dynamic_cast<const SyntaxTree_Exp*>(&x)->OP1 &&
                   ((pOP2 && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2 && *pOP2 == *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2) ||
                   (pOP2 == NULL && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2 == NULL)) &&
                   ((pOP3 && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP3 && *pOP3 == *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP3) ||
                   (pOP3 == NULL && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP3 == NULL));
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Exp*, &x);
#endif
            return _type != dynamic_cast<const SyntaxTree_Exp*>(&x)->_type ||
                   OP1 != dynamic_cast<const SyntaxTree_Exp*>(&x)->OP1 ||
                   ((pOP2 && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2 && *pOP2 != *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2) ||
                   (pOP2 == NULL && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP2)) ||
                   ((pOP3 && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP3 && *pOP3 != *dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP3) ||
                   (pOP3 == NULL && dynamic_cast<const SyntaxTree_Exp*>(&x)->pOP3));
        }
    protected:
        const SyntaxTree_Base& OP1;
        SyntaxTree_Base*       pOP2;
        SyntaxTree_Base*       pOP3;
        Type                   _type;

        // 在make时使用
        bool                   bConstant;
        uchar                  constantBlock;
        ushort                 constantIndex;
    };
}

#endif
