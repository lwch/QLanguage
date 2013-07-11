/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   22:06
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Template.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Template
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_TEMPLATE_H_
#define _QLANGUAGE_SYNTAX_TREE_TEMPLATE_H_

#include "SyntaxTree_TemplateList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Template : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Template(SyntaxTree_TemplateList* pTemplateList);
        SyntaxTree_Template();
        virtual ~SyntaxTree_Template();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Template"; }

        virtual const HASH_KEY_TYPE hash()const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Template*, &x);
#endif
            return ((pTemplateList && dynamic_cast<const SyntaxTree_Template*>(&x)->pTemplateList && *pTemplateList == *dynamic_cast<const SyntaxTree_Template*>(&x)->pTemplateList) ||
                    (pTemplateList == NULL && dynamic_cast<const SyntaxTree_Template*>(&x)->pTemplateList == NULL));
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Template*, &x);
#endif
            return ((pTemplateList && (dynamic_cast<const SyntaxTree_Template*>(&x)->pTemplateList == NULL || *pTemplateList != *dynamic_cast<const SyntaxTree_Template*>(&x)->pTemplateList)) ||
                    (pTemplateList == NULL && dynamic_cast<const SyntaxTree_Template*>(&x)->pTemplateList));
        }
    protected:
        SyntaxTree_TemplateList* pTemplateList;
    };
}

#endif
