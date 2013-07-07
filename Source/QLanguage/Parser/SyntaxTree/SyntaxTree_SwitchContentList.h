/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   14:52
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_SwitchContentList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_SwitchContentList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_SWITCH_CONTENT_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_SWITCH_CONTENT_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_SwitchContentList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_SwitchContentList();
        virtual ~SyntaxTree_SwitchContentList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_SwitchContentList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_SwitchContentList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_SwitchContentList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
