/********************************************************************
	created:	2013/07/06
	created:	6:7:2013   17:24
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_CallList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_CallList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CALL_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_CALL_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_CallList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_CallList();
        virtual ~SyntaxTree_CallList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_CallList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
            return childs.size() != x.childs.size();
        }
    };
}

#endif
