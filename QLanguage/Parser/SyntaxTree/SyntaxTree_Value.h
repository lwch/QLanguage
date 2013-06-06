/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:00
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Value.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Value
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_VALUE_H_
#define _QLANGUAGE_SYNTAX_TREE_VALUE_H_

#include "SyntaxTree_MemberList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Value : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Boolean,
            Real,
            Digit,
            String,
            Member
        };
    public:
        SyntaxTree_Value(const string& value, Type type);
        SyntaxTree_Value(SyntaxTree_MemberList* pMemberList);
        virtual ~SyntaxTree_Value();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Value*, &x);
#endif
            if (type != dynamic_cast<const SyntaxTree_Value*>(&x)->type) return false;
            if (type == Member) return *pMemberList == *dynamic_cast<const SyntaxTree_Value*>(&x)->pMemberList;
            else return value == dynamic_cast<const SyntaxTree_Value*>(&x)->value;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Value*, &x);
#endif
            if (type != dynamic_cast<const SyntaxTree_Value*>(&x)->type) return true;
            if (type == Member) return *pMemberList != *dynamic_cast<const SyntaxTree_Value*>(&x)->pMemberList;
            else return value != dynamic_cast<const SyntaxTree_Value*>(&x)->value;
        }
    protected:
        string           value;
        Type             type;
        SyntaxTree_MemberList* pMemberList;
    };
}

#endif
