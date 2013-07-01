/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   21:49
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_TemplateList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_TemplateList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_TEMPLATE_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_TEMPLATE_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_TemplateList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_TemplateList();
        virtual ~SyntaxTree_TemplateList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_TemplateList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_TemplateList*, &x);
#endif
            return childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_TemplateList*, &x);
#endif
            return childs.size() != x.childs.size();
        }
    };
}

#endif
