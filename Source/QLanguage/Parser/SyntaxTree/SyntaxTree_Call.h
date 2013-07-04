/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   14:01
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Call.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Call
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CALL_H_
#define _QLANGUAGE_SYNTAX_TREE_CALL_H_

#include "SyntaxTree_MemberList.h"
#include "SyntaxTree_ValueList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Call : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Call(const SyntaxTree_MemberList& memberList);
        SyntaxTree_Call(const SyntaxTree_MemberList& memberList, SyntaxTree_ValueList* pValueList);
        virtual ~SyntaxTree_Call();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Call"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Call*, &x);
#endif
            return memberList == dynamic_cast<const SyntaxTree_Call*>(&x)->memberList &&
                   parent::checkEqual(pValueList, dynamic_cast<const SyntaxTree_Call*>(&x)->pValueList);
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Call*, &x);
#endif
            return memberList != dynamic_cast<const SyntaxTree_Call*>(&x)->memberList ||
                   parent::checkNotEqual(pValueList, dynamic_cast<const SyntaxTree_Call*>(&x)->pValueList);
        }
    protected:
        const SyntaxTree_MemberList& memberList;
        SyntaxTree_ValueList*        pValueList;
    };
}

#endif
