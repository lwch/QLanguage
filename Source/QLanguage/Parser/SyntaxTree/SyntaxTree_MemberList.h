/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   11:17
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_MemberList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_MemberList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_MEMBER_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_MEMBER_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_MemberList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_MemberList();
        virtual ~SyntaxTree_MemberList();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_MemberList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_MemberList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_MemberList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
