/********************************************************************
	created:	2013/06/13
	created:	13:6:2013   15:53
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Assign.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Assign
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_ASSIGN_H_
#define _QLANGUAGE_SYNTAX_TREE_ASSIGN_H_

#include "SyntaxTree_MemberList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Assign : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            AddEqual,
            SubEqual,
            AndEqual,
            OrEqual,
            XorEqual,
            LeftMoveEqual,
            RightMoveEqual,
            Equal
        };
    public:
        SyntaxTree_Assign(const SyntaxTree_MemberList& memberList, const SyntaxTree_Base& exp, Type type);
        virtual ~SyntaxTree_Assign();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Assign*, &x);
#endif
            return type == dynamic_cast<const SyntaxTree_Assign*>(&x)->type &&
                   memberList == dynamic_cast<const SyntaxTree_Assign*>(&x)->memberList;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Assign*, &x);
#endif
            return type != dynamic_cast<const SyntaxTree_Assign*>(&x)->type ||
                   memberList != dynamic_cast<const SyntaxTree_Assign*>(&x)->memberList;
        }
    protected:
        Type                         type;
        const SyntaxTree_MemberList& memberList;
        const SyntaxTree_Base&       exp;
    };
}

#endif
