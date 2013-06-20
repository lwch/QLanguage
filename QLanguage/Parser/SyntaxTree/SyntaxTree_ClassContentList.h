/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   17:01
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassContentList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassContentList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CLASS_CONTENT_H_
#define _QLANGUAGE_SYNTAX_TREE_CLASS_CONTENT_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ClassContentList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ClassContentList();
        virtual ~SyntaxTree_ClassContentList();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_ClassContentList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassContentList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassContentList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
